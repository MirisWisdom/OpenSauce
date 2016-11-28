/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#pragma once

__MCPP_CODE_START__

namespace LowLevel { namespace Compression
{
	using namespace System;

	public mcpp_class LZMA abstract sealed
	{
mcpp_public
		delegate static void CompressionCallback(UInt32 /*ProcessedSize*/, UInt32 /*CompressedSize*/);

		mcpp_enum LZMAFormatValue
		{
			LZMAFormatValue_Head,
			LZMAFormatValue_Foot,
			LZMAFormatValue_Props,
			LZMAFormatValue_UncompressedSize32,
			LZMAFormatValue_UncompressedSize64,
			LZMAFormatValue_CompressedData,
			LZMAFormatValue_End,
		};

		static array<LZMAFormatValue>^ LZMAStandardLayout =
		{
			LZMAFormatValue::LZMAFormatValue_Props,
			LZMAFormatValue::LZMAFormatValue_UncompressedSize64,
			LZMAFormatValue::LZMAFormatValue_CompressedData,
			LZMAFormatValue::LZMAFormatValue_End
		};

		static mcpp_bool Decompress(array<LZMAFormatValue>^ layout,
			array<System::Byte>^ input,
			mcpp_out(array<System::Byte>^) output);

		static mcpp_bool LZMA::Compress(array<LZMAFormatValue>^ layout,
			IO::Stream^ stream_in,
			IO::Stream^ stream_out,
			mcpp_uint dictionary_size,
			mcpp_uint level,
			CompressionCallback^ progress_callback);
	};
};};

__MCPP_CODE_END__