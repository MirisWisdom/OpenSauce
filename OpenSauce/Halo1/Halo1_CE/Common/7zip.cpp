/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Common/7zip.hpp"

#if !PLATFORM_IS_DEDI

#include <7zip/7z.h>
#include <7zip/7zAlloc.h>
#include <7zip/7zCrc.h>
#include <7zip/7zFile.h>
#include <7zip/7zVersion.h>

namespace Yelo
{
	namespace Compression { namespace SevenZip
	{
		static void* AllocFunc(void* p, size_t size)
		{
			if(size == 0)
				return NULL;
			return new char[size];
		}

		static void FreeFunc(void* p, void* address)
		{
			delete [] address;
		}

		/*!
		 * \brief
		 * Decompresses a single file entry from a 7zip archive, into memory.
		 * 
		 * \param archive_stream
		 * The stream to read the compressed data from.
		 * 
		 * \param archive_desc
		 * The archives description struct.
		 * 
		 * \param look_stream
		 * The stream associated with the archive stream.
		 * 
		 * \param file_index
		 * Index of the file to extract.
		 * 
		 * \param uncompressed_data
		 * The pointer reference to set when allocating memory for the uncompressed data.
		 * 
		 * \param uncompressed_data_size
		 * Reference to set the uncompressed datas size to.
		 * 
		 * \returns
		 * Returns SZ_OK if decompression was successfully, otherwise returns a non-zero value.
		 * 
		 * Decompresses a single file entry from a 7zip archive, into memory.
		 */
		SRes Decompress(CFileInStream& archive_stream,
			CSzArEx& archive_desc,
			CLookToRead& look_stream,
			uint32 file_index,
			byte*& uncompressed_data,
			size_t& uncompressed_data_size)
		{
			ISzAlloc alloc_imp;
			SRes result = SZ_OK;

			// set functions 7zip will use to allocated and free memory
			alloc_imp.Alloc = AllocFunc;
			alloc_imp.Free = FreeFunc;

			UInt32 block_index = 0xFFFFFFFF;
			size_t offset = 0;
			size_t out_size_processed = 0;

			// decompress the requested file, saving the uncompressed data into memory
			result = SzArEx_Extract(&archive_desc, &look_stream.s, file_index,
				&block_index, &uncompressed_data, &uncompressed_data_size,
				&offset, &out_size_processed,
				&alloc_imp, &alloc_imp);

			if (result != SZ_OK)
			{
				// the decompression failed, so delete the allocated data
				IAlloc_Free(&alloc_imp, uncompressed_data);
				uncompressed_data = NULL;

				uncompressed_data_size = 0;
			}

			return result;
		}

		/*!
		 * \brief
		 * Decompresses a single file from a 7zip archive.
		 * 
		 * \param archive_file
		 * Path to the archive to extract the file from.
		 * 
		 * \param file_name
		 * The fine within the archive to extract.
		 * 
		 * \param uncompressed_data
		 * Reference parameter to set the uncompressed datas pointer to.
		 * 
		 * \param uncompressed_data_size
		 * Reference parameter to set the uncompressed data size to.
		 * 
		 * \returns
		 * Returns true if the decompresseion was successfull, otherwise returns false.
		 * 
		 * Decompresses a single file from a 7zip archive.
		 */
		bool DecompressFileEntry(const char* archive_file,
			const char* file_name,
			byte*& uncompressed_data,
			size_t& uncompressed_data_size)
		{
			CFileInStream archive_stream;
			CLookToRead look_stream;
			CSzArEx archive_desc;
			SRes result;
			ISzAlloc alloc_imp;
			wchar_t wide_file_name[MAX_PATH] = L"";

			// open the archive for reading
			if (InFile_Open(&archive_stream.file, archive_file))
				return false;

			// convert the search file name to a wide char string for comparison later
			if(-1 == swprintf_s(wide_file_name, MAX_PATH, L"%S", file_name))
				return false;

			// set up the memory handling functions
			alloc_imp.Alloc = AllocFunc;
			alloc_imp.Free = FreeFunc;

			FileInStream_CreateVTable(&archive_stream);
			LookToRead_CreateVTable(&look_stream, False);

			look_stream.realStream = &archive_stream.s;
			LookToRead_Init(&look_stream);

			CrcGenerateTable();

			SzArEx_Init(&archive_desc);
			result = SzArEx_Open(&archive_desc, &look_stream.s, &alloc_imp, &alloc_imp);

			if(result == SZ_OK)
			{
				int index = -1;
				// search for the file in the archive
				for (uint32 i = 0; (index == -1) && (i < archive_desc.db.NumFiles); i++)
				{
					if(archive_desc.db.Files[i].IsDir)
						continue;

					// get the name of the file in the archive
					uint32 name_length = SzArEx_GetFileNameUtf16(&archive_desc, i, NULL);
					wchar_t* archive_file_name = new wchar_t[name_length];
					SzArEx_GetFileNameUtf16(&archive_desc, i, (UInt16*)archive_file_name);

					// compare the file names
					if(wcscmp(wide_file_name, archive_file_name) == 0)
						index = i;

					// delete allocated memory
					delete [] archive_file_name;
				}

				if(index > -1)
				{
					// file found, so decompress it
					result = Decompress(archive_stream,
						archive_desc,
						look_stream,
						index,
						uncompressed_data,
						uncompressed_data_size);
				}
			}

			SzArEx_Free(&archive_desc, &alloc_imp);

			File_Close(&archive_stream.file);

			return result == SZ_OK;
		}
	};};
};
#endif