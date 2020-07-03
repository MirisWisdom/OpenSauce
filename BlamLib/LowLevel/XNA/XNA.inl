/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/

__CPP_CODE_START__
namespace Internal
{
	#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CPP
	#include "XNA/MathImpl.Halfs.inl"


//////////////////////////////////////////////////////////////////////////
	#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CPP
	#include "XNA/MathImpl.Color.inl"

//////////////////////////////////////////////////////////////////////////
	#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CPP
	#include "XNA/MathImpl.UByte4.inl"

//////////////////////////////////////////////////////////////////////////
	#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CPP
	#include "XNA/MathImpl.Short2.inl"

//////////////////////////////////////////////////////////////////////////
	#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CPP
	#include "XNA/MathImpl.Short4.inl"

//////////////////////////////////////////////////////////////////////////
	#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CPP
	#include "XNA/MathImpl.UByte4N.inl"

//////////////////////////////////////////////////////////////////////////
	#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CPP
	#include "XNA/MathImpl.Short2N.inl"

//////////////////////////////////////////////////////////////////////////
	#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CPP
	#include "XNA/MathImpl.Short4N.inl"

//////////////////////////////////////////////////////////////////////////
	#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CPP
	#include "XNA/MathImpl.UShort2N.inl"

//////////////////////////////////////////////////////////////////////////
	#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CPP
	#include "XNA/MathImpl.UShort4N.inl"

//////////////////////////////////////////////////////////////////////////
	#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CPP
	#include "XNA/MathImpl.UDec3.inl"

//////////////////////////////////////////////////////////////////////////
	#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CPP
	#include "XNA/MathImpl.Dec3N.inl"

//////////////////////////////////////////////////////////////////////////
	#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CPP
	#include "XNA/MathImpl.Float16Two.inl"

//////////////////////////////////////////////////////////////////////////
	#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CPP
	#include "XNA/MathImpl.Float16Four.inl"

//////////////////////////////////////////////////////////////////////////
	#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CPP
	#include "XNA/MathImpl.UShort2.inl"

//////////////////////////////////////////////////////////////////////////
	#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CPP
	#include "XNA/MathImpl.UShort4.inl"

//////////////////////////////////////////////////////////////////////////
	#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CPP
	#include "XNA/MathImpl.UDHen3N.inl"

//////////////////////////////////////////////////////////////////////////
	#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CPP
	#include "XNA/MathImpl.DHen3N.inl"

//////////////////////////////////////////////////////////////////////////
	#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CPP
	#include "XNA/MathImpl.UHenD3N.inl"

//////////////////////////////////////////////////////////////////////////
	#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CPP
	#include "XNA/MathImpl.UDec4N.inl"

//////////////////////////////////////////////////////////////////////////
	#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CPP
	#include "XNA/MathImpl.Byte4N.inl"

// Legacy:

//////////////////////////////////////////////////////////////////////////
	#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CPP
	#include "XNA/MathImpl.HenD3N.inl"

//////////////////////////////////////////////////////////////////////////

#ifndef LOWLEVEL_NO_X360
	static cpp_bool FormatIsTiled(cpp_uint fmt)
	{
		return ::XGIsTiledFormat((D3DFORMAT)fmt) > 0; // XGUntileSurface, XGUntileTextureLevel, XGUntileVolume
	}

	static cpp_uint GetFormatSize(cpp_uint fmt)
	{
		return ::XGBytesPerPixelFromFormat((D3DFORMAT)fmt);
	}

	static void EndianSwapMemory32(void* dst, const void* src, cpp_int count)
	{
		XGEndianSwapMemory(dst, src, XGENDIAN_8IN32, sizeof(DWORD), (cpp_uint)count);
	}

	static void ConvertSurface(cpp_byte* src,
		cpp_int src_row_pitch, cpp_uint src_fmt,
		cpp_byte* dst,
		cpp_int dst_row_pitch, cpp_uint dst_fmt, cpp_uint flags, cpp_uint width, cpp_uint height, 
		cpp_uint* out_result)
	{
		HRESULT hr = XGCopySurface(
			dst, dst_row_pitch, 
			width, height, (D3DFORMAT)dst_fmt, 
			cpp_null, 
			src, src_row_pitch, (D3DFORMAT)src_fmt,
			cpp_null, 
			flags, 0.0f);

		if(out_result) *out_result = hr;
	}

	static cpp_uint SetTextureHeader(cpp_uint width, cpp_uint height, cpp_uint levels,
		cpp_uint format, cpp_uint base_offset, cpp_uint mip_offset, cpp_uint pitch,
		cpp_byte* texture_header, cpp_int* out_base_size, cpp_int* out_mip_size)
	{
		return XGSetTextureHeaderEx(width, height, levels,
			0,
			static_cast<D3DFORMAT>(format),
			0, 0, base_offset, XGHEADER_CONTIGUOUS_MIP_OFFSET, pitch,
			reinterpret_cast<D3DTexture*>(texture_header), 
			reinterpret_cast<cpp_uint*>(out_base_size), reinterpret_cast<cpp_uint*>(out_mip_size));
	}

	static XMEMDECOMPRESSION_CONTEXT ContextCreateForDecompression(
		cpp_uint type, cpp_uint flags, XMEMCODEC_PARAMETERS_LZX* params = NULL)
	{
		XMEMDECOMPRESSION_CONTEXT ctxt = NULL;

		if(SUCCEEDED(XMemCreateDecompressionContext(cpp_cast_to(XMEMCODEC_TYPE,type), params, flags, &ctxt)))
			return ctxt;

		return NULL;
	}

	static XMEMDECOMPRESSION_CONTEXT ContextCreateForCompression(
		cpp_uint type, cpp_uint flags, XMEMCODEC_PARAMETERS_LZX* params = NULL)
	{
		XMEMDECOMPRESSION_CONTEXT ctxt = NULL;

		if(SUCCEEDED(XMemCreateCompressionContext(cpp_cast_to(XMEMCODEC_TYPE,type), params, flags, &ctxt)))
			return ctxt;

		return NULL;
	}

	static SIZE_T ContextCalcSizeForCompression(
		cpp_uint type, cpp_uint flags, XMEMCODEC_PARAMETERS_LZX* params = NULL)
	{
		return XMemGetCompressionContextSize(cpp_cast_to(XMEMCODEC_TYPE,type), params, flags);
	}
	static XMEMDECOMPRESSION_CONTEXT ContextCreateForCompression(
		cpp_uint type, cpp_uint flags, void* ctxt_data, SIZE_T ctxt_size, XMEMCODEC_PARAMETERS_LZX* params = NULL)
	{
		return XMemInitializeCompressionContext(cpp_cast_to(XMEMCODEC_TYPE,type), params, flags, ctxt_data, ctxt_size);
	}
#endif
};
__CPP_CODE_END__