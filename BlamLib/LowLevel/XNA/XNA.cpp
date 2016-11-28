/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#include "Precompile.hpp"
#include "XNA/XNA.hpp"
#using <System.Drawing.dll>

// specify these headers explicitly so we don't get C4793 warnings
// "function compiled as native"
__CPP_CODE_START__

//#ifndef _X86_
//	#define _X86_
//#endif
#include <D3dx9math.h>
#include <d3d9.h>
#include <xnamath.h>
#ifndef LOWLEVEL_NO_X360
	#include <XGraphics.h>
#endif

#include "XNA/XNA.inl"
__CPP_CODE_END__


__MCPP_CODE_START__
namespace LowLevel { namespace Xna {
	#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CLR_CPP
	#include "XNA/MathImpl.Halfs.inl"

//////////////////////////////////////////////////////////////////////////
//

	#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CLR_CPP
	#include "XNA/MathImpl.Color.inl"

	//////////////////////////////////////////////////////////////////////////
	#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CLR_CPP
	#include "XNA/MathImpl.UByte4.inl"

	//////////////////////////////////////////////////////////////////////////
	#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CLR_CPP
	#include "XNA/MathImpl.Short2.inl"
	
	//////////////////////////////////////////////////////////////////////////
	#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CLR_CPP
	#include "XNA/MathImpl.Short4.inl"

	//////////////////////////////////////////////////////////////////////////
	#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CLR_CPP
	#include "XNA/MathImpl.UByte4N.inl"

	//////////////////////////////////////////////////////////////////////////
	#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CLR_CPP
	#include "XNA/MathImpl.Short2N.inl"

	//////////////////////////////////////////////////////////////////////////
	#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CLR_CPP
	#include "XNA/MathImpl.Short4N.inl"

	//////////////////////////////////////////////////////////////////////////
	#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CLR_CPP
	#include "XNA/MathImpl.UShort2N.inl"

	//////////////////////////////////////////////////////////////////////////
	#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CLR_CPP
	#include "XNA/MathImpl.UShort4N.inl"

	//////////////////////////////////////////////////////////////////////////
	#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CLR_CPP
	#include "XNA/MathImpl.UDec3.inl"

	//////////////////////////////////////////////////////////////////////////
	#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CLR_CPP
	#include "XNA/MathImpl.Dec3N.inl"

	//////////////////////////////////////////////////////////////////////////
	#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CLR_CPP
	#include "XNA/MathImpl.Float16Two.inl"

	//////////////////////////////////////////////////////////////////////////
	#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CLR_CPP
	#include "XNA/MathImpl.Float16Four.inl"

	//////////////////////////////////////////////////////////////////////////
	#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CLR_CPP
	#include "XNA/MathImpl.UShort2.inl"

	//////////////////////////////////////////////////////////////////////////
	#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CLR_CPP
	#include "XNA/MathImpl.UShort4.inl"

	//////////////////////////////////////////////////////////////////////////
	#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CLR_CPP
	#include "XNA/MathImpl.UDHen3N.inl"

	//////////////////////////////////////////////////////////////////////////
	#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CLR_CPP
	#include "XNA/MathImpl.DHen3N.inl"

	//////////////////////////////////////////////////////////////////////////
	#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CLR_CPP
	#include "XNA/MathImpl.UHenD3N.inl"

	//////////////////////////////////////////////////////////////////////////
	#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CLR_CPP
	#include "XNA/MathImpl.UDec4N.inl"

	//////////////////////////////////////////////////////////////////////////
	#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CLR_CPP
	#include "XNA/MathImpl.Byte4N.inl"

// Legacy:

	//////////////////////////////////////////////////////////////////////////
	#define __XNA_MATH_IMPL_CODE_INCLUDE_ID __XNA_MATH_IMPL_CODE_CLR_CPP
	#include "XNA/MathImpl.HenD3N.inl"
}; };
//////////////////////////////////////////////////////////////////////////

#ifndef LOWLEVEL_NO_X360
namespace LowLevel { namespace Xbox360 {

	mcpp_bool Graphics::FormatIsTiled(mcpp_uint fmt)
	{
		return ::Internal::FormatIsTiled(fmt);
	}

	mcpp_uint Graphics::GetFormatSize(mcpp_uint fmt)
	{
		return ::Internal::GetFormatSize(fmt);
	}

	void Graphics::EndianSwapMemory32(System::IntPtr dst, System::IntPtr src, mcpp_int count)
	{
		return ::Internal::EndianSwapMemory32(dst.ToPointer(), src.ToPointer(), count);
	}


	//////////////////////////////////////////////////////////////////////////
	// Decompress
	System::IntPtr Compression::ContextCreateForDecompression(Compression::CodecType type, Compression::ContextFlags flags)
	{
		return System::IntPtr(::Internal::ContextCreateForDecompression(
			mcpp_cast_to(cpp_uint,type), mcpp_cast_to(cpp_uint,flags)));
	}
	System::IntPtr Compression::ContextCreateForDecompression(Compression::CodecType type, Compression::ContextFlags flags, Compression::LzxParameters params)
	{
		XMEMCODEC_PARAMETERS_LZX cpp_params = {
			params.Flags, params.WindowSize, params.CompressionPartitionSize
		};
		return System::IntPtr(::Internal::ContextCreateForDecompression(
			mcpp_cast_to(cpp_uint,type), mcpp_cast_to(cpp_uint,flags), &cpp_params));
	}

	mcpp_uint Compression::Decompress(System::IntPtr ctxt, array<mcpp_byte>^ dst, mcpp_out(mcpp_uint) dst_size,
		array<mcpp_byte>^ src, mcpp_int decompress_amount)
	{
		dst_size = 0;

		pin_ptr<cpp_byte> dst_ptr = &dst[0];
		pin_ptr<const cpp_byte> src_ptr = &dst[0];
		SIZE_T dest_size = dst->Length;
		if(decompress_amount <= 0) decompress_amount = src->Length;

		HRESULT hr = XMemDecompress(ctxt.ToPointer(), 
			dst_ptr, &dest_size, src_ptr, decompress_amount);

		if(SUCCEEDED(hr)) dst_size = mcpp_cast_to(mcpp_uint,dest_size);
		return hr;
	}

	mcpp_uint Compression::DecompressStream(System::IntPtr ctxt, array<mcpp_byte>^ dst, mcpp_out(mcpp_uint) dst_size,
		array<mcpp_byte>^ src, mcpp_ref(mcpp_uint) decompress_amount)
	{
		dst_size = 0;

		pin_ptr<cpp_byte> dst_ptr = &dst[0];
		pin_ptr<const cpp_byte> src_ptr = &dst[0];
		SIZE_T dest_size = dst->Length;
		if(decompress_amount <= 0) decompress_amount = src->Length;
		SIZE_T src_size = decompress_amount;

		HRESULT hr = XMemDecompressStream(ctxt.ToPointer(), 
			dst_ptr, &dest_size, src_ptr, &src_size);

		if(SUCCEEDED(hr))
		{
			dst_size = mcpp_cast_to(mcpp_uint,dest_size);
			decompress_amount = mcpp_cast_to(mcpp_uint,src_size);
		}

		return hr;
	}
	void Compression::DecompressContextReset(System::IntPtr ctxt)
	{
		XMemResetDecompressionContext(ctxt.ToPointer());
	}

	void Compression::DecompressContextDispose(mcpp_ref(System::IntPtr) ctxt)
	{
		if(ctxt != System::IntPtr::Zero)
		{
			XMemDestroyDecompressionContext( ctxt.ToPointer() );
			ctxt = System::IntPtr::Zero;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// Compress
	System::IntPtr Compression::ContextCreateForCompression(Compression::CodecType type, Compression::ContextFlags flags, 
		mcpp_out(array<mcpp_byte>^) ctxt_data)
	{
		return System::IntPtr(::Internal::ContextCreateForCompression(
			mcpp_cast_to(cpp_uint,type), mcpp_cast_to(cpp_uint,flags)));
	}
	System::IntPtr Compression::ContextCreateForCompression(Compression::CodecType type, Compression::ContextFlags flags, 
		mcpp_out(array<mcpp_byte>^) ctxt_data, Compression::LzxParameters params)
	{
		XMEMCODEC_PARAMETERS_LZX cpp_params = {
			params.Flags, params.WindowSize, params.CompressionPartitionSize
		};
		return System::IntPtr(::Internal::ContextCreateForCompression(
			mcpp_cast_to(cpp_uint,type), mcpp_cast_to(cpp_uint,flags), &cpp_params));
	}

	mcpp_uint Compression::Compress(System::IntPtr ctxt, array<mcpp_byte>^ dst, mcpp_out(mcpp_uint) dst_size,
		array<mcpp_byte>^ src, mcpp_int compress_amount)
	{
		dst_size = 0;

		pin_ptr<cpp_byte> dst_ptr = &dst[0];
		pin_ptr<const cpp_byte> src_ptr = &dst[0];
		SIZE_T dest_size = dst->Length;
		// Passing a value of 0 concludes the data stream, so don't check <= 0
		if(compress_amount < 0) compress_amount = src->Length;

		HRESULT hr = XMemCompress(ctxt.ToPointer(), 
			dst_ptr, &dest_size, src_ptr, compress_amount);

		if(SUCCEEDED(hr)) dst_size = mcpp_cast_to(mcpp_uint,dest_size);
		return hr;
	}

	mcpp_uint Compression::CompressStream(System::IntPtr ctxt, array<mcpp_byte>^ dst, mcpp_out(mcpp_uint) dst_size,
		array<mcpp_byte>^ src, mcpp_ref(mcpp_uint) compress_amount)
	{
		dst_size = 0;

		pin_ptr<cpp_byte> dst_ptr = &dst[0];
		pin_ptr<const cpp_byte> src_ptr = &dst[0];
		SIZE_T dest_size = dst->Length;
		if(compress_amount <= 0) compress_amount = src->Length;
		SIZE_T src_size = compress_amount;

		HRESULT hr = XMemCompressStream(ctxt.ToPointer(), 
			dst_ptr, &dest_size, src_ptr, &src_size);

		if(SUCCEEDED(hr))
		{
			dst_size = mcpp_cast_to(mcpp_uint,dest_size);
			compress_amount = mcpp_cast_to(mcpp_uint,src_size);
		}

		return hr;
	}
	void Compression::CompressContextReset(System::IntPtr ctxt)
	{
		XMemResetCompressionContext(ctxt.ToPointer());
	}

	void Compression::CompressContextDispose(mcpp_ref(System::IntPtr) ctxt)
	{
		if(ctxt != System::IntPtr::Zero)
		{
			XMemDestroyCompressionContext( ctxt.ToPointer() );
			ctxt = System::IntPtr::Zero;
		}
	}

}; };
#endif
__MCPP_CODE_END__