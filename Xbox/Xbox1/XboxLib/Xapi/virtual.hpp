/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
#pragma once

XBOX_PROC(LPVOID, VirtualAlloc, (
		  __in_opt LPVOID lpAddress,
		  __in     SIZE_T dwSize,
		  __in     DWORD flAllocationType,
		  __in     DWORD flProtect
		  ));
#define ApiVAlloc XboxApi.VirtualAlloc



XBOX_PROC(BOOL, VirtualFree, (
		  __in LPVOID lpAddress,
		  __in SIZE_T dwSize,
		  __in DWORD dwFreeType
		  ));
#define ApiVFree XboxApi.VirtualFree



XBOX_PROC(BOOL, VirtualProtect, (
		  __in  LPVOID lpAddress,
		  __in  SIZE_T dwSize,
		  __in  DWORD flNewProtect,
		  __out PDWORD lpflOldProtect
		  ));
#define ApiVProtect VirtualProtect



//_VirtualQuery@12
//_VirtualAllocEx@20
//_VirtualFreeEx@16
//_VirtualProtectEx@20
//_VirtualQueryEx@16
//_GlobalMemoryStatus@4