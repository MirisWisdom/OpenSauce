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
#pragma once

__MCPP_CODE_START__

namespace LowLevel { namespace Xma {

	public mcpp_struct RebuildParameters
	{
		mcpp_uint Offset;
		mcpp_uint BlockSize;
		mcpp_int Channels;
		mcpp_int Version;

		mcpp_bool Strict;
		mcpp_bool IgnorePacketSkip;
	};

	public mcpp_class Interface
	{
#ifdef LOWLEVEL_NO_X360_XMA
		static mcpp_string^ Xma2EncoderExePath;
		static mcpp_string^ Xma2EncoderExeFile;
		static System::Diagnostics::ProcessStartInfo^ Xma2EncoderExeStartInfo;

		static void RunXma2Encode(mcpp_string^ xma_file, mcpp_string^ pcm_file);
#endif
	mcpp_public
		// If we're not linking directly to the XMA encoder lib, try to dynamically use the EXE tool instead
#ifdef LOWLEVEL_NO_X360_XMA
		static void SetXma2EncoderExePath(mcpp_string^ exe_path);
#endif

		static mcpp_readonly array<mcpp_byte>^ kMonoFooter = mcpp_new array<mcpp_byte> {
			0x58, 0x4D, 0x41, 0x32, 0x2C, 0x00, 0x00, 0x00, 0x04, 
			0x01, 0x00, 0xFF, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 
			0x8A, 0x00, 0x00, 0x00, 0xAB, 0xD2, 0x00, 0x00, 0x10, 
			0xD6, 0x00, 0x00, 0x3D, 0x14, 0x00, 0x01, 0x00, 0x00, 
			0x00, 0x00, 0x8A, 0x00, 0x00, 0x00, 0x88, 0x80, 0x00, 
			0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x01, 0x73, 0x65, 
			0x65, 0x6B, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8A, 
			0x00
		};
		static mcpp_readonly array<mcpp_byte>^ kStereoFooter = mcpp_new array<mcpp_byte> {
			0x58, 0x4D, 0x41, 0x32, 0x2C, 0x00, 0x00, 0x00, 0x04, 
			0x01, 0x00, 0xFF, 0x00, 0x00, 0x01, 0x80, 0x00, 0x01, 
			0x0F, 0x80, 0x00, 0x00, 0xAC, 0x44, 0x00, 0x00, 0x10, 
			0xD6, 0x00, 0x00, 0x3D, 0x14, 0x00, 0x01, 0x00, 0x00, 
			0x00, 0x01, 0x10, 0x00, 0x00, 0x01, 0x0E, 0x00, 0x00, 
			0x00, 0x00, 0x01, 0x02, 0x00, 0x02, 0x01, 0x73, 0x65, 
			0x65, 0x6B, 0x04, 0x00, 0x00, 0x00, 0x00, 0x01, 0x10, 
			0x00
		};

		static mcpp_bool Decode(mcpp_string^ xma_file, mcpp_string^ pcm_file);
		static array<mcpp_byte>^ Rebuild(array<mcpp_byte>^ buffer, RebuildParameters params);
		static bool Rebuild(mcpp_string^ in_file, mcpp_string^ out_file, mcpp_string^ rebuild_file, 
			RebuildParameters params);
	};

}; };

__MCPP_CODE_END__