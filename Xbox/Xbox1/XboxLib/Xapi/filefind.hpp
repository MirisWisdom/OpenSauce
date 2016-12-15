/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
#pragma once

XBOX_PROC(HANDLE, FindFirstFileA, (
		  __in  LPCSTR lpFileName,
		  __out LPWIN32_FIND_DATAA lpFindFileData
		  ));
#define ApiFindFirstFile XboxApi.FindFirstFile



XBOX_PROC(BOOL, FindNextFileA, (
		  __in  HANDLE hFindFile,
		  __out LPWIN32_FIND_DATAA lpFindFileData
		  ));
#define ApiFindNextFile XboxApi.FindNextFile