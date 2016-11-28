/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
#include "XboxLib.hpp"
#include "XboxInterface.hpp"

#include "XboxKrnl.hpp"
#include "XboxApi.hpp"

namespace XboxLib
{
	BOOL IsEmulatorPresent()
	{
		return k_xbox_lib_emulation_mode != FALSE;
	}

	void XBOXAPI Initialize()
	{
		BuildKernelExportTable();
	}

	void XBOXAPI Dispose() {}

	API_FUNC_NAKED void XBOXAPI SetLedStatus(DWORD led_state)
	{
		API_FUNC_NAKED_START()

			mov		eax, led_state
			push	eax
			push	FALSE
			push	PIC16L_CMD_LED_REGISTER
			push	SMBDEV_PIC16L
			call	K_KERNEL_EXPORTS.HalWriteSMBusValue

			push	LED_MODE_SUBCMD_CUSTOM_MODE
			push	FALSE
			push	PIC16L_CMD_LED_MODE
			push	SMBDEV_PIC16L
			call	K_KERNEL_EXPORTS.HalWriteSMBusValue

		API_FUNC_NAKED_END(1);
	}

	API_FUNC_NAKED void XBOXAPI EjectTray()
	{
		API_FUNC_NAKED_START_()

			push	EJECT_SUBCMD_EJECT
			push	FALSE
			push	PIC16L_CMD_EJECT
			push	SMBDEV_PIC16L
			call	K_KERNEL_EXPORTS.HalWriteSMBusValue

		API_FUNC_NAKED_END_()
	}

	API_FUNC_NAKED void XBOXAPI LoadTry()
	{
		API_FUNC_NAKED_START_()

			push	EJECT_SUBCMD_LOAD
			push	FALSE
			push	PIC16L_CMD_EJECT
			push	SMBDEV_PIC16L
			call	K_KERNEL_EXPORTS.HalWriteSMBusValue

		API_FUNC_NAKED_END_()
	}

	API_FUNC_NAKED void XBOXAPI RebootToDash()
	{
		API_FUNC_NAKED_START_()

			// TODO: Add XLaunchNewImage support

		API_FUNC_NAKED_END_()
	}

	API_FUNC_NAKED void XBOXAPI Reset()
	{
		API_FUNC_NAKED_START_()

			push	POWER_SUBCMD_RESET
			push	FALSE
			push	PIC16L_CMD_POWER
			push	SMBDEV_PIC16L
			call	K_KERNEL_EXPORTS.HalWriteSMBusValue

		API_FUNC_NAKED_END_()
	}

	API_FUNC_NAKED void XBOXAPI PowerOff()
	{
		API_FUNC_NAKED_START_()

			push	POWER_SUBCMD_POWER_OFF
			push	FALSE
			push	PIC16L_CMD_POWER
			push	SMBDEV_PIC16L
			call	K_KERNEL_EXPORTS.HalWriteSMBusValue

		API_FUNC_NAKED_END_()
	}

	API_FUNC_NAKED void XBOXAPI PowerCycle()
	{
		API_FUNC_NAKED_START_()

			push	POWER_SUBCMD_CYCLE
			push	FALSE
			push	PIC16L_CMD_POWER
			push	SMBDEV_PIC16L
			call	K_KERNEL_EXPORTS.HalWriteSMBusValue

		API_FUNC_NAKED_END_()
	}

	API_FUNC_NAKED void XBOXAPI GetCpuTemp(DWORD* out_value)
	{
		API_FUNC_NAKED_START()

			mov		eax, out_value
			push	eax
			push	FALSE
			push	9
			push	SMBDEV_PIC16L
			call	K_KERNEL_EXPORTS.HalReadSMBusValue

		API_FUNC_NAKED_END(1);
	}

	API_FUNC_NAKED void XBOXAPI GetSysTemp(DWORD* out_value)
	{
		API_FUNC_NAKED_START()

			mov		eax, out_value
			push	eax
			push	FALSE
			push	10
			push	SMBDEV_PIC16L
			call	K_KERNEL_EXPORTS.HalReadSMBusValue

		API_FUNC_NAKED_END(1);
	}

	API_FUNC_NAKED void XBOXAPI GetFanSpeed(DWORD* out_value)
	{
		API_FUNC_NAKED_START()

			mov		eax, out_value
			push	eax
			push	FALSE
			push	16
			push	SMBDEV_PIC16L
			call	K_KERNEL_EXPORTS.HalReadSMBusValue

		API_FUNC_NAKED_END(1);
	}


	void* __fastcall _memcpy(void* dst, const void* src, size_t count)
	{
		void* ret = dst;

        while (count--) {
                *CAST_PTR(byte*, dst) = *CAST_PTR(const byte*, src);
                dst = CAST_PTR(byte*, dst) + 1;
                src = CAST_PTR(const byte*, src) + 1;
        }

		return ret;
	}
	API_FUNC_NAKED void XBOXAPI _memzero(void* dst, size_t count)
	{
		API_FUNC_NAKED_START()
			push	edi

			xor		eax, eax
			mov		ecx, count
			mov		edi, dst
			rep stosb

			pop		edi
		API_FUNC_NAKED_END(2);
	}

	API_FUNC_NAKED bool XBOXAPI IsAddressValid(void* address)
	{
		API_FUNC_NAKED_START()

			mov		ecx, address
			mov		eax, ecx
			shr		eax, 0x14
			and		eax, 0xFFC
			sub		eax, 0x3FD00000
			mov		eax, [eax]
			test	al, 1
			jz		_bad_addr
			test	al, al
			js		_good_addr

			shr		ecx, 0xA
			and		ecx, 0x3FFFFC
			sub		ecx, 0x40000000
			mov		eax, [ecx]
			test	al, 1
			jz		_bad_addr
			test	al, al
			js		_good_addr

			add		ecx, 0x800
			shl		ecx, 0x14
			cmp		ecx, 0xFFFFFFF
			ja		_good_addr

_bad_addr:
			xor		eax, eax
			jmp		_the_fucking_end
_good_addr:
			mov		eax, 1

_the_fucking_end:
		API_FUNC_NAKED_END(1);
	}

	bool XBOXAPI Screenshot()
	{
		const void* nv_frame_buffer = K_NV_FRAME_BUFFER_ADDRESS;
		const DWORD nv_frame_buffer_size = K_NV_FRAME_BUFFER_SIZE;

		if(nv_frame_buffer != NULL) // We don't check IsEmulatorPresent here in case the emulator provides an address to something useful!
		{
			HANDLE ssHandle;
			char ssName[32];
			FILETIME ssTime;
			DWORD ssWritten;

			if( ApiCreateDir("T:\\$s", NULL) )
			{
				ApiGetSystemTimeAsFileTime(&ssTime);
				Std_sprintf(ssName, "T:\\$s\\%.8X%.8X", ssTime.dwHighDateTime, ssTime.dwLowDateTime);
				ssHandle = ApiCreateFile(
					ssName,
					GENERIC_WRITE,
					NULL,
					NULL,
					CREATE_ALWAYS,
					FILE_ATTRIBUTE_NORMAL,
					NULL
				);

				nv_frame_buffer = CAST_PTR(const void*, 
					*CAST_PTR(const DWORD*, nv_frame_buffer) | 0x80000000);

				if(	ssHandle != CAST_PTR(HANDLE, NULL_HANDLE) &&
					ApiWriteFile(ssHandle, nv_frame_buffer, nv_frame_buffer_size, &ssWritten, NULL) &&
					ApiCloseHandle(ssHandle)
					)
					return true;
				else
					DisplayLastError();
			}
		}

		return false;
	}
};