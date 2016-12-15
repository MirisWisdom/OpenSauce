/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
#include "XboxLib.hpp"
#include "XboxKrnl.hpp"
#pragma warning(push)
#pragma warning(disable : 4312)

//CAST_PTR(, 0x),

XBOX_KERNEL_VTABLE K_KERNEL_5933 = {
// ******************************************************************
// * Device
// ******************************************************************
	CAST_PTR(_D3DDevice_Present, 0x800629C0),

		// ******************************************************************
		// * SetRenderState
		// ******************************************************************
		CAST_PTR(_D3DDevice_SetRenderState_MultiSampleType, 0x8005FF50),
		CAST_PTR(_D3DDevice_SetRenderState_PSTextureModes, 0x8005FF90),
		CAST_PTR(_D3DDevice_SetRenderState_EdgeAntiAlias, 0x800600F0),
		CAST_PTR(_D3DDevice_SetRenderState_ShadowFunc, 0x80060140),
		CAST_PTR(_D3DDevice_SetRenderState_FogColor, 0x80060190),
		CAST_PTR(_D3DDevice_SetRenderState_CullMode, 0x80060200),
		CAST_PTR(_D3DDevice_SetRenderState_FrontFace, 0x80060290),
};

//XBOX_KERNEL_DATA K_KERNELDATA_5933 = {
//	CAST_PTR(unsigned char*, 0x8005F900),
//	CAST_PTR(unsigned char*, 0x8005F910),
//	CAST_PTR(unsigned char*, 0x8005F920),
//	CAST_PTR(unsigned char*, 0x8005FA20),
//	CAST_PTR(unsigned char*, 0x8005FA30),
//	CAST_PTR(unsigned char*, 0x8005FA48),
//};

XBOX_EXPORT_TABLE K_KERNEL_EXPORTS;

__declspec(naked) void BuildKernelExportTable()
{
	enum { k_kernel_base_address = 0x80010000, };

	static boost::int32_t KrnlExportCount;

	__asm
	{
		mov		eax, dword ptr K_KERNEL_EXPORTS[0]
		test	eax, eax
		jnz		exports_exist

		push	ebx
		push	ecx
		push	edx
		push	esi

		mov		ebx, k_kernel_base_address

		// gets the export table info
		mov		eax, dword ptr [ebx + 0x3C]
		mov		eax, dword ptr [ebx + eax + 0x78]
		mov		ecx, dword ptr [ebx + eax + 0x14]	// export count
		mov		KrnlExportCount, ecx
		mov		eax, dword ptr [ebx + eax + 0x1C]
		add		eax, ebx							// base address

		// builds separate kernel export table
		mov		ecx, 1
loop_start:
		cmp		ecx, KrnlExportCount
		jg		loop_end

		dec		ecx									// adjust for counter starting at 1

		// gets kernel function address
		mov		edx, dword ptr [eax + ecx * 4]		// gets function address
		add		edx, ebx							// adjusts for actual address

		// copies to export table
		lea		esi, K_KERNEL_EXPORTS[ecx * 4]		// get table address
		mov		[esi], edx							// save to table

		add		ecx, 2								// mike's a h4xer

		jmp		loop_start
loop_end:

		pop		esi
		pop		edx
		pop		ecx
		pop		ebx

exports_exist:
		retn
	};
}

#pragma warning(pop)


const void* K_NV_FRAME_BUFFER_ADDRESS = CAST_PTR(void*, 0xFD600800);
const DWORD K_NV_FRAME_BUFFER_SIZE = 640 * 480 * sizeof(DWORD);


#include "XboxApi.hpp" // Exports references some Api definitions
// We define the exports here in hopes that the symbols will never be removed by the linker 
#include "ModuleExports.inl"