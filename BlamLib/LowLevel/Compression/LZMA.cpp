/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#include "Precompile.hpp"
#include "Compression/LZMA.hpp"

#include "Compression/LZMA/LzmaLib.h"
#include "Compression/LZMA/LzmaEnc.h"
#include "Compression/LZMA/LzmaDec.h"

namespace LowLevel { namespace Compression
{
	using namespace System;

	static void * LzmaAlloc(void *p, size_t size)
	{
		return malloc(size);
	}

	static void LzmaFree(void *p, void *address)
	{
		free(address);
	}

	static ISzAlloc LzmaMemoryFunctions =
	{
		&LzmaAlloc, &LzmaFree
	};

	struct LZMAInStream
	{
		ISeqInStream SeqInStream;
		gcroot<IO::Stream^> m_stream;
	};

	struct LZMAOutStream
	{
		ISeqOutStream SeqOutStream;
		gcroot<IO::Stream^> m_stream;
	};

	struct LZMACompressionProgress
	{
		ICompressProgress CompressProgress;
		gcroot<LZMA::CompressionCallback^> m_callback;
	};

	static SRes LZMAInStream_Read(void* p, void* buf, size_t* size)
	{
		LZMAInStream* in_stream = CAST_PTR(LZMAInStream*, p);

		// if position + size is more than the length of the stream, change the size to whatever is left
		if((in_stream->m_stream->Position + *size) > in_stream->m_stream->Length)
			*size = (size_t)(in_stream->m_stream->Length - in_stream->m_stream->Position);

		if(*size)
		{
			// read the steam into a temporary buffer
			array<System::Byte>^ buffer = mcpp_new array<System::Byte>(*size);
			in_stream->m_stream->Read(buffer, 0, *size);

			// get a pointer to the memory and copy it to the LZMA buffer
			pin_ptr<System::Byte> buffer_ptr = &buffer[0];
			memcpy_s(buf, *size, buffer_ptr, *size);
		}
		return SZ_OK;
	}

	static size_t LZMAOutStream_Write(void* p, const void* buf, size_t size)
	{
		LZMAOutStream* out_stream = CAST_PTR(LZMAOutStream*, p);

		if(size)
		{
			// create a new managed buffer and get a pointer to it
			array<System::Byte>^ buffer = mcpp_new array<System::Byte>(size);
			pin_ptr<System::Byte> buffer_ptr = &buffer[0];

			// copy the LZMA data to the managed buffer
			memcpy_s(buffer_ptr, size, buf, size);
			buffer_ptr = mcpp_null;

			// write the managed buffer to the output stream
			out_stream->m_stream->Write(buffer, 0, size);
		}
		return size;
	}

	static SRes LZMACompressionProgress_Callback(void *p, unsigned long long inSize, unsigned long long outSize)
	{
		LZMACompressionProgress* compression_progress = CAST_PTR(LZMACompressionProgress*, p);

		// call the progress callback
		compression_progress->m_callback->Invoke((unsigned int)inSize, (unsigned int)outSize);

		return SZ_OK;
	}

	mcpp_bool LZMA::Decompress(array<LZMAFormatValue>^ layout,
		array<System::Byte>^ input,
		mcpp_out(array<System::Byte>^) output)
	{
		// get pointers to the input data and data layout
		pin_ptr<unsigned char> data_pointer = &input[0];
		pin_ptr<LZMAFormatValue> data_layout = &layout[0];

		unsigned int* head_pointer = NULL;
		unsigned int* foot_pointer = NULL;
		byte* props_pointer = NULL;
		unsigned int* uncompressed_size_32_pointer = NULL;
		unsigned long long* uncompressed_size_64_pointer = NULL;
		byte* compressed_data = NULL;

		// calculate the required pointers from the layout
		byte* data_offset = data_pointer;
		byte* data_end = data_pointer + input->Length;
		while(*data_layout != LZMAFormatValue::LZMAFormatValue_End)
		{
			// buffer overrun check
			if(data_offset == data_end)
				return mcpp_false;

			switch(*data_layout)
			{
			case LZMAFormatValue::LZMAFormatValue_Head:
				head_pointer = CAST_PTR(unsigned int*, data_offset);
				data_offset += sizeof(unsigned int);
				break;
			case LZMAFormatValue::LZMAFormatValue_Foot:
				foot_pointer = CAST_PTR(unsigned int*, data_offset);
				data_offset += sizeof(unsigned int);
				break;
			case LZMAFormatValue::LZMAFormatValue_Props:
				props_pointer = data_offset;
				data_offset += LZMA_PROPS_SIZE;
				break;
			case LZMAFormatValue::LZMAFormatValue_UncompressedSize32:
				uncompressed_size_32_pointer = CAST_PTR(unsigned int*, data_offset);
				data_offset += sizeof(unsigned int);
				break;
			case LZMAFormatValue::LZMAFormatValue_UncompressedSize64:
				uncompressed_size_64_pointer = CAST_PTR(unsigned long long*, data_offset);
				data_offset += sizeof(unsigned long long);
				break;
			case LZMAFormatValue::LZMAFormatValue_CompressedData:
				// the compressed data entry MUST come last as we cannot move data_offset past it without knowing its length
				compressed_data = data_offset;
				break;
			default:
				return mcpp_false;
			}

			data_layout++;
		}

		// check that the head and foot values are correct if used
		if(head_pointer && (*head_pointer != 'head'))
			return mcpp_false;

		if(foot_pointer && (*foot_pointer != 'foot'))
			return mcpp_false;

		// get the uncompressed data size
		SizeT uncompressed_data_size = 0;
		if(uncompressed_size_32_pointer)
		{
			// check that the uncompressed size is a valid size
			if(*uncompressed_size_32_pointer == 0)
				return mcpp_false;

			uncompressed_data_size = *uncompressed_size_32_pointer;
		}
		else if(uncompressed_size_64_pointer)
		{
			// when the archive uses a 64 bit size value it is cast down to 32 bit

			// check if the size is too large for a 32 bit allocation
			if(*uncompressed_size_64_pointer & 0xFFFFFFFF00000000)
				return mcpp_false;

			// check that the uncompressed size is a valid size
			if(*uncompressed_size_64_pointer == 0)
				return mcpp_false;

			uncompressed_data_size = (unsigned int)*uncompressed_size_64_pointer;
		}
		else
			return mcpp_false;

		// allocated managed memory for the uncompressed data, and get its pointer
		output = mcpp_new array<System::Byte>(uncompressed_data_size);
		pin_ptr<System::Byte> uncompressed_data = &output[0];

		// calculate the compressed data size
		SizeT compressed_data_size = input->Length - LZMA_PROPS_SIZE;
		
		if(head_pointer)					compressed_data_size -= sizeof(unsigned int);
		if(foot_pointer)					compressed_data_size -= sizeof(unsigned int);
		if(uncompressed_size_32_pointer)	compressed_data_size -= sizeof(unsigned int);
		if(uncompressed_size_64_pointer)	compressed_data_size -= sizeof(unsigned long long);

		// decompress the data
		SRes result = LzmaUncompress(
			uncompressed_data,
			&uncompressed_data_size,
			compressed_data,
			&compressed_data_size,
			props_pointer,
			LZMA_PROPS_SIZE);

		// delete the data if the decompression failed
		if(result)
		{
			output = mcpp_null;

			return false;
		}

		return mcpp_true;
	}

	mcpp_bool LZMA::Compress(array<LZMAFormatValue>^ layout,
			IO::Stream^ stream_in,
			IO::Stream^ stream_out,
			mcpp_uint dictionary_size,
			mcpp_uint level,
			CompressionCallback^ progress_callback)
	{
		// get a pointer to the data layout
		pin_ptr<LZMAFormatValue> data_layout_start = &layout[0];

		// calculate the header size
		int header_size = 0;
		LZMAFormatValue* data_layout = data_layout_start;
		while(*data_layout != LZMAFormatValue::LZMAFormatValue_End)
		{
			switch(*data_layout)
			{
			case LZMAFormatValue::LZMAFormatValue_Head:
				header_size += sizeof(unsigned int);
				break;
			case LZMAFormatValue::LZMAFormatValue_Foot:
				header_size += sizeof(unsigned int);
				break;
			case LZMAFormatValue::LZMAFormatValue_Props:
				header_size += LZMA_PROPS_SIZE;
				break;
			case LZMAFormatValue::LZMAFormatValue_UncompressedSize32:
				header_size += sizeof(unsigned int);
				break;
			case LZMAFormatValue::LZMAFormatValue_UncompressedSize64:
				header_size += sizeof(unsigned long long);
				break;
			case LZMAFormatValue::LZMAFormatValue_CompressedData:
				break;
			default:
				return mcpp_false;
			}

			data_layout++;
		}

		// seek to the beginning of the input stream
		stream_in->Seek(0, System::IO::SeekOrigin::Begin);

		// seek to the end of the header in the output stream
		stream_out->SetLength(header_size);
		stream_out->Seek(header_size, System::IO::SeekOrigin::Begin);

		CLzmaEncHandle encoding_handle = LzmaEnc_Create(&LzmaMemoryFunctions);

		// set up the compression properties
		CLzmaEncProps props;
		LzmaEncProps_Init(&props);
		props.writeEndMark = 0;
		props.algo = 1;
		props.numThreads = 2;

		props.dictSize = dictionary_size;
		props.level = level;

		SRes result = LzmaEnc_SetProps(encoding_handle, &props);

		if(result)
		{
			LzmaEnc_Destroy(encoding_handle, &LzmaMemoryFunctions, &LzmaMemoryFunctions);
			return false;
		}

		// for streaming compression we have to pass our own read/write/callback functions via these structs
		LZMAInStream lzma_stream_in =
		{
			&LZMAInStream_Read,
			stream_in
		};

		LZMAOutStream lzma_stream_out =
		{
			&LZMAOutStream_Write,
			stream_out
		};

		LZMACompressionProgress lzma_compression_callback =
		{
			&LZMACompressionProgress_Callback,
			progress_callback
		};

		ICompressProgress* callback = NULL;
		if(progress_callback != mcpp_null)
			callback = (ICompressProgress*)&lzma_compression_callback;

		// compress the data
		result = LzmaEnc_Encode(encoding_handle,
			(ISeqOutStream*)&lzma_stream_out,
			(ISeqInStream*)&lzma_stream_in,
			callback,
			&LzmaMemoryFunctions,
			&LzmaMemoryFunctions);

		if(result)
		{
			LzmaEnc_Destroy(encoding_handle, &LzmaMemoryFunctions, &LzmaMemoryFunctions);
			return false;
		}

		// create the header and get its pointer
		array<System::Byte>^ header = mcpp_new array<System::Byte>(header_size);
		pin_ptr<System::Byte> header_ptr = &header[0];

		// set the header values
		unsigned int header_offset = 0;
		data_layout = data_layout_start;
		while(*data_layout != LZMAFormatValue::LZMAFormatValue_End)
		{
			switch(*data_layout)
			{
			case LZMAFormatValue::LZMAFormatValue_Head:
				{
					*CAST_PTR(unsigned int*, &header_ptr[header_offset]) = 'head';
					header_offset += sizeof(unsigned int);
				}
				break;
			case LZMAFormatValue::LZMAFormatValue_Foot:
				{
					*CAST_PTR(unsigned int*, &header_ptr[header_offset]) = 'foot';
					header_offset += sizeof(unsigned int);
				}
				break;
			case LZMAFormatValue::LZMAFormatValue_Props:
				{
					unsigned int props_size = LZMA_PROPS_SIZE;
					LzmaEnc_WriteProperties(encoding_handle, &header_ptr[header_offset], &props_size);
					header_offset += LZMA_PROPS_SIZE;
				}
				break;
			case LZMAFormatValue::LZMAFormatValue_UncompressedSize32:
				{
					*CAST_PTR(unsigned int*, &header_ptr[header_offset]) = (unsigned int)stream_in->Length;
					header_offset += sizeof(unsigned int);
				}
				break;
			case LZMAFormatValue::LZMAFormatValue_UncompressedSize64:
				{
					*CAST_PTR(unsigned long long*, &header_ptr[header_offset]) = (unsigned long long)stream_in->Length;
				}
				break;
			case LZMAFormatValue::LZMAFormatValue_CompressedData:
				break;
			default:
				break;
			}

			data_layout++;
		}
		header_ptr = mcpp_null;

		// write the header to the output stream
		stream_out->Seek(0, System::IO::SeekOrigin::Begin);
		stream_out->Write(header, 0, header_size);

		// release the streams and destroy any allocated LZMA memory
		LzmaEnc_Destroy(encoding_handle, &LzmaMemoryFunctions, &LzmaMemoryFunctions);

		return mcpp_true;
	}
};};