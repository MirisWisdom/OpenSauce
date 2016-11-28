/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
#pragma once

XBOX_PROC(LPVOID, XMemAlloc, (
		  SIZE_T dwSize,
		  DWORD dwAllocAttributes
		  ));
#define ApiMemAlloc XboxApi.XMemAlloc



XBOX_PROC(VOID, XMemFree, (
		  PVOID pAddress,
		  DWORD dwAllocAttributes
		  ));
#define ApiMemFree XboxApi.XMemFree




//_XMemSizeDefault@8