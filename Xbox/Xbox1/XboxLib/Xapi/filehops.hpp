/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
#pragma once

XBOX_PROC(BOOL, ReadFile, (
		  __in        HANDLE hFile,
		  __out_bcount_part(nNumberOfBytesToRead, *lpNumberOfBytesRead) LPVOID lpBuffer,
		  __in        DWORD nNumberOfBytesToRead,
		  __out_opt   LPDWORD lpNumberOfBytesRead,
		  __inout_opt LPOVERLAPPED lpOverlapped
		  ));
#define ApiReadFile XboxApi.ReadFile



XBOX_PROC(BOOL, WriteFile, (
		  __in        HANDLE hFile,
		  __in_bcount(nNumberOfBytesToWrite) LPCVOID lpBuffer,
		  __in        DWORD nNumberOfBytesToWrite,
		  __out_opt   LPDWORD lpNumberOfBytesWritten,
		  __inout_opt LPOVERLAPPED lpOverlapped
		  ));
#define ApiWriteFile XboxApi.WriteFile



XBOX_PROC(BOOL, SetEndOfFile, (
		  __in HANDLE hFile
		  ));
#define ApiSetEndOfFile XboxApi.SetEndOfFile



XBOX_PROC(DWORD, SetFilePointer, (
		  __in     HANDLE hFile,
		  __in     LONG lDistanceToMove,
		  __in_opt PLONG lpDistanceToMoveHigh,
		  __in     DWORD dwMoveMethod
		  ));
#define ApiSetFilePointer XboxApi.SetFilePointer



XBOX_PROC(BOOL, SetFilePointerEx, (
		  __in      HANDLE hFile,
		  __in      LARGE_INTEGER liDistanceToMove,
		  __out_opt PLARGE_INTEGER lpNewFilePointer,
		  __in      DWORD dwMoveMethod
		  ));
#define ApiSetFilePointerEx XboxApi.SetFilePointerEx



//_GetFileInformationByHandle@8



XBOX_PROC(BOOL, GetFileTime, (
		  __in      HANDLE hFile,
		  __out_opt LPFILETIME lpCreationTime,
		  __out_opt LPFILETIME lpLastAccessTime,
		  __out_opt LPFILETIME lpLastWriteTime
		  ));
#define ApiGetFileTime XboxApi.GetFileTime



//_SetFileTime@16



XBOX_PROC(BOOL, FlushFileBuffers, (
		  __in HANDLE hFile
		  ));
#define ApiFlushFileBuffers XboxApi.FlushFileBuffers



XBOX_PROC(BOOL, GetFileSizeEx, (
		  __in  HANDLE hFile,
		  __out PLARGE_INTEGER lpFileSize
		  ));
#define ApiGetFileSizeEx XboxApi.GetFileSizeEx



//_ReadFileEx@20
//_WriteFileEx@20
//_DeviceIoControl@32
//_ReadFileScatter@20
//_WriteFileGather@20
//_XGetFilePhysicalSortKey@4



XBOX_PROC(DWORD, GetFileSize, (
		  __in      HANDLE hFile,
		  __out_opt LPDWORD lpFileSizeHigh
		  ));
#define ApiGetFileSize XboxApi.GetFileSize