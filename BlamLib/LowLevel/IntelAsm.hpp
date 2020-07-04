/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#pragma once

namespace LowLevel { namespace Intel {

	using namespace System;

	public mcpp_class x86
	{
	public:
		mcpp_const mcpp_byte pushad = 0x60;
		mcpp_const mcpp_byte popad = 0x61;

		mcpp_const mcpp_byte push_prefix_dword = 0x68;

		mcpp_const mcpp_byte mov_eax_prefix_dword = 0xB8;
		mcpp_const mcpp_byte mov_ecx_prefix_dword = 0xB9;
		mcpp_const mcpp_byte mov_edx_prefix_dword = 0xBA;
		mcpp_const mcpp_byte mov_ebx_prefix_dword = 0xBB;
		mcpp_const mcpp_byte mov_esp_prefix_dword = 0xBC;
		mcpp_const mcpp_byte mov_ebp_prefix_dword = 0xBD;
		mcpp_const mcpp_byte mov_esi_prefix_dword = 0xBE;
		mcpp_const mcpp_byte mov_edi_prefix_dword = 0xBF;

		mcpp_const mcpp_ushort call_prefix_absolute =	0x00FF | 0x1500;
		mcpp_const mcpp_ushort call_eax =				0x00FF | 0xD000;
	};

	public mcpp_class SSE
	{
	public:
		mcpp_const mcpp_uint movss_xmm0_prefix_addr = 0x05100FF3;
		mcpp_const mcpp_uint movss_xmm1_prefix_addr = 0x0D100FF3;
		mcpp_const mcpp_uint movss_xmm2_prefix_addr = 0x15100FF3;
		mcpp_const mcpp_uint movss_xmm3_prefix_addr = 0x1D100FF3;
		mcpp_const mcpp_uint movss_xmm4_prefix_addr = 0x25100FF3;
		mcpp_const mcpp_uint movss_xmm5_prefix_addr = 0x2D100FF3;
		mcpp_const mcpp_uint movss_xmm6_prefix_addr = 0x35100FF3;
		mcpp_const mcpp_uint movss_xmm7_prefix_addr = 0x3D100FF3;
	};

}; };