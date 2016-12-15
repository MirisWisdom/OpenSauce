/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
#pragma once

//_SetFileAttributesA@8
//_GetFileAttributesA@4
//_GetFileAttributesExA@12
//_MoveFileA@8



XBOX_PROC(BOOL, DeleteFileA, (
		  __in LPCSTR lpFileName
		  ));
#define ApiDeleteFile XboxApi.DeleteFile



//_XSetFileCacheSize@4
//_XGetFileCacheSize@0
//_XapiGetDirectoryDisplayBlocks@12
//_XGetDisplayBlocks@4
//_MoveFileWithProgressA@20
//_MoveFileExA@12