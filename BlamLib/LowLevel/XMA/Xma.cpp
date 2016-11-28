/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#include "Precompile.hpp"
#include "XMA/Xma.hpp"

#ifdef _DEBUG

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

#ifdef LOWLEVEL_NO_X360_XMA
	void Interface::SetXma2EncoderExePath(mcpp_string^ exe_path)
	{
		if(mcpp_string::IsNullOrEmpty(exe_path))
			throw mcpp_new System::ArgumentNullException("Invalid file name", "exe_path");
		else if(!System::IO::File::Exists(exe_path))
			throw mcpp_new System::IO::FileNotFoundException("exe_path does not exist", exe_path);

		Xma2EncoderExeFile = System::IO::Path::GetFullPath(exe_path);
		Xma2EncoderExePath = System::IO::Path::GetDirectoryName(Xma2EncoderExeFile);

		Xma2EncoderExeStartInfo =  mcpp_new System::Diagnostics::ProcessStartInfo(
			Xma2EncoderExeFile);
		Xma2EncoderExeStartInfo->WorkingDirectory = Xma2EncoderExePath;
		Xma2EncoderExeStartInfo->UseShellExecute = mcpp_false;
		Xma2EncoderExeStartInfo->CreateNoWindow = mcpp_true;
	}

	void Interface::RunXma2Encode(mcpp_string^ xma_file, mcpp_string^ pcm_file)
	{
		mcpp_string^ cmdline = mcpp_string::Format("\"{0}\" /DecodeToPCM \"{1}\"",
			xma_file, pcm_file);

		Xma2EncoderExeStartInfo->Arguments = cmdline;
		System::Diagnostics::Process^ p = mcpp_new System::Diagnostics::Process();
		p->Start(Xma2EncoderExeStartInfo);
	}
#endif

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
		if(!System::IO::File::Exists(Xma2EncoderExePath))
			throw mcpp_new System::Exception("You need to call SetXma2EncoderExePath before calling Decode()");
		if(!System::IO::File::Exists(xma_file))
			throw mcpp_new System::IO::FileNotFoundException("xma_file does not exist", xma_file);

		// TODO: check to see if input file names are relative paths to the xma2encode exe 
		// as we should allow this to be valid behavior

		RunXma2Encode(xma_file, pcm_file);

		return mcpp_true; // assume successful
#endif
	}

	array<mcpp_byte>^ Interface::Rebuild(array<mcpp_byte>^ buffer, RebuildParameters params)
	{
		if(buffer == mcpp_null)
			throw mcpp_new System::ArgumentNullException("buffer");

		pin_ptr<mcpp_byte> buffer_pin_ptr = &buffer[0];
		cpp_byte* buffer_ptr = buffer_pin_ptr;

		array<mcpp_byte>^ result = mcpp_null;

		XMA::s_xma_parse_context ctx;
		RebuildParametersToNative(buffer->Length, params, ctx);

		XMA::c_xma_rebuilder* rebuilder = cpp_new XMA::c_xma_rebuilder(CAST_PTR(char*,buffer_ptr), ctx);

		if(rebuilder->try_rebuild())
		{
			char* output_buf;
			size_t output_buf_size;
			rebuilder->get_output_data(output_buf, output_buf_size);

			result = mcpp_new array<mcpp_byte>(output_buf_size);
			pin_ptr<mcpp_byte> output_pin_ptr = &result[0];
			cpp_byte* ptr = output_pin_ptr;

			memcpy(ptr, output_buf, output_buf_size);

			delete output_buf;
		}

		delete rebuilder;

		return result;
	}

	bool Interface::Rebuild(mcpp_string^ in_file, mcpp_string^ out_file, mcpp_string^ rebuild_file, 
		RebuildParameters params)
	{
		if(mcpp_string::IsNullOrEmpty(in_file))
			throw mcpp_new System::ArgumentNullException("Invalid file name", "in_file");
		else if(!System::IO::File::Exists(in_file))
			throw mcpp_new System::IO::FileNotFoundException("in_file does not exist", in_file);

		if(mcpp_string::IsNullOrEmpty(out_file) && mcpp_string::IsNullOrEmpty(rebuild_file))
			throw mcpp_new System::ArgumentNullException("out_file || rebuild_file");


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

#endif