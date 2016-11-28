/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
#pragma once

XBOX_PROC(BOOL, CreateDirectoryA, (
		  __in     LPCSTR lpPathName,
		  __in_opt LPSECURITY_ATTRIBUTES lpSecurityAttributes
		  ));
#define ApiCreateDir XboxApi.CreateDirectory



XBOX_PROC(BOOL, RemoveDirectoryA, (
		  __in LPCSTR lpPathName
		  ));
#define ApiRemoveDir XboxApi.RemoveDirectory