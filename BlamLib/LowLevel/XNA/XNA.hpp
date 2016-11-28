/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#pragma once
#include "Math/Math.hpp"

#define __XNA_MATH_IMPL_CODE_CLR_HPP 1
#define __XNA_MATH_IMPL_CODE_CLR_CPP 2
#define __XNA_MATH_IMPL_CODE_CPP 3


__MCPP_CODE_START__
namespace LowLevel { namespace Xna {

	public mcpp_class Math abstract sealed {
	mcpp_public

		#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CLR_HPP
		#include "XNA/MathImpl.Halfs.inl"

//////////////////////////////////////////////////////////////////////////
//

		//////////////////////////////////////////////////////////////////////////
		// XMCOLOR
		#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CLR_HPP
		#include "XNA/MathImpl.Color.inl"

		//////////////////////////////////////////////////////////////////////////
		// XMUBYTE4
		#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CLR_HPP
		#include "XNA/MathImpl.UByte4.inl"

		//////////////////////////////////////////////////////////////////////////
		// XMSHORT2
		#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CLR_HPP
		#include "XNA/MathImpl.Short2.inl"

		//////////////////////////////////////////////////////////////////////////
		// XMSHORT4
		#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CLR_HPP
		#include "XNA/MathImpl.Short4.inl"

		//////////////////////////////////////////////////////////////////////////
		// XMUBYTEN4
		#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CLR_HPP
		#include "XNA/MathImpl.UByte4N.inl"

		//////////////////////////////////////////////////////////////////////////
		// XMSHORTN2
		#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CLR_HPP
		#include "XNA/MathImpl.Short2N.inl"

		//////////////////////////////////////////////////////////////////////////
		// XMSHORTN4
		#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CLR_HPP
		#include "XNA/MathImpl.Short4N.inl"

		//////////////////////////////////////////////////////////////////////////
		// XMUSHORTN2
		#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CLR_HPP
		#include "XNA/MathImpl.UShort2N.inl"

		//////////////////////////////////////////////////////////////////////////
		// XMUSHORTN4
		#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CLR_HPP
		#include "XNA/MathImpl.UShort4N.inl"

		//////////////////////////////////////////////////////////////////////////
		// Uses XMUDEC4
		#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CLR_HPP
		#include "XNA/MathImpl.UDec3.inl"

		//////////////////////////////////////////////////////////////////////////
		// Uses XMDECN4
		#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CLR_HPP
		#include "XNA/MathImpl.Dec3N.inl"

		//////////////////////////////////////////////////////////////////////////
		// XMHALF2
		#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CLR_HPP
		#include "XNA/MathImpl.Float16Two.inl"

		//////////////////////////////////////////////////////////////////////////
		// XMHALF4
		#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CLR_HPP
		#include "XNA/MathImpl.Float16Four.inl"

		//////////////////////////////////////////////////////////////////////////
		// XMUSHORT2
		#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CLR_HPP
		#include "XNA/MathImpl.UShort2.inl"

		//////////////////////////////////////////////////////////////////////////
		// XMUSHORT4
		#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CLR_HPP
		#include "XNA/MathImpl.UShort4.inl"

		//////////////////////////////////////////////////////////////////////////
		// XMUDHEN3
		#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CLR_HPP
		#include "XNA/MathImpl.UDHen3N.inl"

		//////////////////////////////////////////////////////////////////////////
		// XMDHEN3
		#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CLR_HPP
		#include "XNA/MathImpl.DHen3N.inl"

		//////////////////////////////////////////////////////////////////////////
		// XMUHENDN3
		#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CLR_HPP
		#include "XNA/MathImpl.UHenD3N.inl"

		//////////////////////////////////////////////////////////////////////////
		// XMUDECN4
		#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CLR_HPP
		#include "XNA/MathImpl.UDec4N.inl"

		//////////////////////////////////////////////////////////////////////////
		// XMBYTEN4
		#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CLR_HPP
		#include "XNA/MathImpl.Byte4N.inl"

// Legacy:

		//////////////////////////////////////////////////////////////////////////
		// XMHENDN3
		#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CLR_HPP
		#include "XNA/MathImpl.HenD3N.inl"


//////////////////////////////////////////////////////////////////////////
	};
}; };

#ifndef LOWLEVEL_NO_X360
#include <xcompress.h>

namespace LowLevel { namespace Xbox360 {
	public mcpp_class Graphics abstract sealed {
	mcpp_public

		static mcpp_bool FormatIsTiled(mcpp_uint fmt);

		static mcpp_uint GetFormatSize(mcpp_uint fmt);

		static void EndianSwapMemory32(System::IntPtr dst, System::IntPtr src, mcpp_int count);
	};

	public mcpp_class Compression abstract sealed {
	mcpp_public
		mcpp_enum CodecType
		{
			Default = XMEMCODEC_DEFAULT,
			Lzx = XMEMCODEC_LZX,
		};
		[System::Flags]
		mcpp_enum ContextFlags
		{
			Stream = XMEMCOMPRESS_STREAM,
		};

		mcpp_struct LzxParameters {
			mcpp_uint Flags;
			mcpp_uint WindowSize;
			mcpp_uint CompressionPartitionSize;
		};

		static System::IntPtr ContextCreateForDecompression(CodecType type, ContextFlags flags);
		static System::IntPtr ContextCreateForDecompression(CodecType type, ContextFlags flags, LzxParameters params);

		static mcpp_uint Decompress(System::IntPtr ctxt, array<mcpp_byte>^ dst, mcpp_out(mcpp_uint) dst_size,
			array<mcpp_byte>^ src, mcpp_int decompress_amount);
		static mcpp_uint DecompressStream(System::IntPtr ctxt, array<mcpp_byte>^ dst, mcpp_out(mcpp_uint) dst_size,
			array<mcpp_byte>^ src, mcpp_ref(mcpp_uint) decompress_amount);
		static void DecompressContextReset(System::IntPtr ctxt);
		static void DecompressContextDispose(mcpp_ref(System::IntPtr) ctxt);

		static System::IntPtr ContextCreateForCompression(CodecType type, ContextFlags flags, 
			mcpp_out(array<mcpp_byte>^) ctxt_data);
		static System::IntPtr ContextCreateForCompression(CodecType type, ContextFlags flags, 
			mcpp_out(array<mcpp_byte>^) ctxt_data, LzxParameters params);

		static mcpp_uint Compress(System::IntPtr ctxt, array<mcpp_byte>^ dst, mcpp_out(mcpp_uint) dst_size,
			array<mcpp_byte>^ src, mcpp_int compress_amount);
		static mcpp_uint CompressStream(System::IntPtr ctxt, array<mcpp_byte>^ dst, mcpp_out(mcpp_uint) dst_size,
			array<mcpp_byte>^ src, mcpp_ref(mcpp_uint) compress_amount);
		static void CompressContextReset(System::IntPtr ctxt);
		static void CompressContextDispose(mcpp_ref(System::IntPtr) ctxt);
	};

}; };
#endif
__MCPP_CODE_END__