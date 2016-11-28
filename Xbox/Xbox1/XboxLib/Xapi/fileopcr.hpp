/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
#pragma once

XBOX_PROC(HANDLE, CreateFileA, (
		  __in     LPCSTR lpFileName,
		  __in     DWORD dwDesiredAccess,
		  __in     DWORD dwShareMode,
		  __in_opt LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		  __in     DWORD dwCreationDisposition,
		  __in     DWORD dwFlagsAndAttributes,
		  __in_opt HANDLE hTemplateFile
		  ));
#define ApiCreateFile XboxApi.CreateFile



//_CopyFileExA@24
//_CopyFileA@12