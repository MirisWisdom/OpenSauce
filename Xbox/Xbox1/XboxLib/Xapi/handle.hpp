/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
#pragma once

XBOX_PROC(BOOL, CloseHandle, (
		  HANDLE obj
		  ));
#define ApiCloseHandle XboxApi.CloseHandle



//_DuplicateHandle@28