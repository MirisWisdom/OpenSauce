/*
    BlamLib: .NET SDK for the Blam Engine

    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "Precompile.hpp"
#include "XMA/Xma.hpp"
#include "XMA/XmaParse.hpp"

#include "XMA/Xma.Lib.inl"

__MCPP_CODE_START__

void RebuildParametersToNative(mcpp_uint buffer_size, 
							   LowLevel::Xma::RebuildParameters params, ::XMA::s_xma_parse_context& ctx)
{
	XMA::s_xma_parse_context value = {
		params.Channels > 1,
		params.Strict,
		cpp_false,				// Verbose
		params.IgnorePacketSkip,
		params.Version,

		params.Offset,
		buffer_size,
		params.BlockSize
	};

	ctx = value;
}

namespace LowLevel { namespace Xma {

	mcpp_bool Interface::Decode(mcpp_string^ xma_file, mcpp_string^ pcm_file)
	{
#ifndef LOWLEVEL_NO_X360_XMA
		pin_ptr<const WCHAR> xma = PtrToStringChars(xma_file);
		pin_ptr<const WCHAR> pcm = PtrToStringChars(pcm_file);

		// I disassembled xma2encode.exe and this is basically what it does for /DecodeToPCM cases
		CXMATarget* obj;
		CreateXMATarget(obj);
		HRESULT result = obj->DecodeWave(xma, pcm);
		FreeXMATarget(obj);

		return SUCCEEDED(result);
#else
		return mcpp_false;
#endif
	}

	array<mcpp_byte>^ Interface::Rebuild(array<mcpp_byte>^ buffer, RebuildParameters params)
	{
		pin_ptr<mcpp_byte> buffer_pin_ptr = &buffer[0];
		cpp_byte* buffer_ptr = buffer_pin_ptr;

		array<mcpp_byte>^ result = mcpp_null;

		XMA::s_xma_parse_context ctx;
		RebuildParametersToNative(buffer->Length, params, ctx);

		XMA::c_xma_rebuilder rebuilder(CAST_PTR(char*,buffer_ptr), ctx);

		if(rebuilder.try_rebuild())
		{
			char* output_buf;
			size_t output_buf_size;
			rebuilder.get_output_data(output_buf, output_buf_size);

			result = mcpp_new array<mcpp_byte>(output_buf_size);
			pin_ptr<mcpp_byte> output_pin_ptr = &result[0];
			cpp_byte* ptr = output_pin_ptr;

			memcpy(ptr, output_buf, output_buf_size);

			delete output_buf;
		}

		return result;
	}

	bool Interface::Rebuild(mcpp_string^ in_file, mcpp_string^ out_file, mcpp_string^ rebuild_file, 
		RebuildParameters params)
	{
		mcpp_bool result = mcpp_false;

		XMA::s_xma_parse_context ctx;
		RebuildParametersToNative(NONE, params, ctx);

		const char* in_f, * out_f, * rb_f;

		in_f = (const char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(in_file);
		out_f = out_file ? (const char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(out_file) : cpp_null;
		rb_f = rebuild_file ? (const char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(rebuild_file) : cpp_null;

		XMA::c_xma_rebuilder* rebuilder = cpp_new XMA::c_xma_rebuilder(in_f, ctx, out_f, rb_f);
		result = rebuilder->try_rebuild();
		delete rebuilder;

		System::Runtime::InteropServices::Marshal::FreeHGlobal((System::IntPtr)(void*)in_f);
		if(out_f != cpp_null) System::Runtime::InteropServices::Marshal::FreeHGlobal((System::IntPtr)(void*)out_f);
		if(rb_f != cpp_null) System::Runtime::InteropServices::Marshal::FreeHGlobal((System::IntPtr)(void*)rb_f);

		return result;
	}

}; };

__MCPP_CODE_END__