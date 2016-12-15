/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/

//////////////////////////////////////////////////////////////////////////
// Precompile.cpp
#if __EL_INCLUDE_FILE_ID == __EL_COMMON_PRECOMPILE
namespace Common
{
//////////////////////////////////////////////////////////////////////////
// XBOX_API_VTABLE

	// ******************************************************************
	// * Directory I\O
	// ******************************************************************
	FUNC_PTR(_CreateDirectoryA,			FUNC_PTR_UNKNOWN);
	FUNC_PTR(_RemoveDirectoryA,			FUNC_PTR_UNKNOWN);

	// ******************************************************************
	// * File I\O
	// ******************************************************************
	FUNC_PTR(_GetFileSize,				FUNC_PTR_UNKNOWN);
	FUNC_PTR(_GetFileSizeEx,			FUNC_PTR_UNKNOWN);
	FUNC_PTR(_WriteFile,				FUNC_PTR_UNKNOWN);
	FUNC_PTR(_ReadFile,					FUNC_PTR_UNKNOWN);
	FUNC_PTR(_FlushFileBuffers,			FUNC_PTR_UNKNOWN);
	FUNC_PTR(_SetEndOfFile,				FUNC_PTR_UNKNOWN);
	FUNC_PTR(_SetFilePointer,			FUNC_PTR_UNKNOWN);
	FUNC_PTR(_SetFilePointerEx,			FUNC_PTR_UNKNOWN);
	FUNC_PTR(_GetFileTime,				FUNC_PTR_UNKNOWN);
	FUNC_PTR(_CreateFileA,				FUNC_PTR_UNKNOWN);
	FUNC_PTR(_DeleteFileA,				FUNC_PTR_UNKNOWN);
	FUNC_PTR(_FindFirstFileA,			FUNC_PTR_UNKNOWN);
	FUNC_PTR(_FindNextFileA,			FUNC_PTR_UNKNOWN);

	// ******************************************************************
	// * Misc
	// ******************************************************************
	FUNC_PTR(_GetSystemTimeAsFileTime,	FUNC_PTR_UNKNOWN);
	FUNC_PTR(_CloseHandle,				FUNC_PTR_UNKNOWN);
	FUNC_PTR(_GetLastError,				FUNC_PTR_UNKNOWN);
	FUNC_PTR(_MultiByteToWideChar,		FUNC_PTR_UNKNOWN);
	FUNC_PTR(_WideCharToMultiByte,		FUNC_PTR_UNKNOWN);

	// ******************************************************************
	// * Networking
	// ******************************************************************
	FUNC_PTR(_XNetGetTitleXnAddr,		0x43FA7D);

	// ******************************************************************
	// * Threading
	// ******************************************************************
	FUNC_PTR(_CreateThread,				FUNC_PTR_UNKNOWN);
	FUNC_PTR(_GetCurrentThreadId,		FUNC_PTR_UNKNOWN);
	FUNC_PTR(_CreateEventA,				FUNC_PTR_UNKNOWN);
	FUNC_PTR(_CreateSemaphoreA,			FUNC_PTR_UNKNOWN);

	// ******************************************************************
	// * Signature
	// ******************************************************************
	FUNC_PTR(_XCalculateSignatureBegin,		FUNC_PTR_UNKNOWN);
	FUNC_PTR(_XCalculateSignatureBeginEx,	FUNC_PTR_UNKNOWN);
	FUNC_PTR(_XCalculateSignatureEnd,		FUNC_PTR_UNKNOWN);
	FUNC_PTR(_XCalculateSignatureGetSize,	FUNC_PTR_UNKNOWN);
	FUNC_PTR(_XCalculateSignatureUpdate,	FUNC_PTR_UNKNOWN);

	// ******************************************************************
	// * Virtual Memory
	// ******************************************************************
	FUNC_PTR(_VirtualAlloc,				FUNC_PTR_UNKNOWN);
	FUNC_PTR(_VirtualFree,				FUNC_PTR_UNKNOWN);
	FUNC_PTR(_VirtualProtect,			FUNC_PTR_UNKNOWN);

	FUNC_PTR(_XMemAlloc,				FUNC_PTR_UNKNOWN);
	FUNC_PTR(_XMemFree,					FUNC_PTR_UNKNOWN);


//////////////////////////////////////////////////////////////////////////
// XBOX_STD_VTABLE
	// ******************************************************************
	// * File I\O
	// ******************************************************************
	FUNC_PTR(__fsopen,		FUNC_PTR_UNKNOWN);
	FUNC_PTR(_fclose,		FUNC_PTR_UNKNOWN);
	FUNC_PTR(_fflush,		FUNC_PTR_UNKNOWN);
	FUNC_PTR(_fopen,		FUNC_PTR_UNKNOWN);
	FUNC_PTR(_fgets,		FUNC_PTR_UNKNOWN);
	FUNC_PTR(_fseek,		FUNC_PTR_UNKNOWN);
	FUNC_PTR(_ftell,		FUNC_PTR_UNKNOWN);

	// ******************************************************************
	// * Math
	// ******************************************************************
	FUNC_PTR(_srand,		FUNC_PTR_UNKNOWN);
	FUNC_PTR(_rand,			FUNC_PTR_UNKNOWN);

	// ******************************************************************
	// * Memory
	// ******************************************************************
	FUNC_PTR(_malloc,		FUNC_PTR_UNKNOWN);

	// ******************************************************************
	// * Networking
	// ******************************************************************
	FUNC_PTR(_socket,		0x43FA88);
	FUNC_PTR(_closesocket,	0x43FA93);
	FUNC_PTR(_shutdown,		0x43FA9E);
	FUNC_PTR(_ioctlsocket,	0x43FAA9);
	FUNC_PTR(_setsockopt,	0x43FAB4);
	FUNC_PTR(_getsockopt,	0x43FAC3);
	FUNC_PTR(_bind,			0x43FAD2);
	FUNC_PTR(_connect,		0x43FADD);
	FUNC_PTR(_select,		0x43FAE8);
	FUNC_PTR(_recv,			0x43FB11);
	FUNC_PTR(_recvfrom,		0x43FB2B);
	FUNC_PTR(_send,			0x43FB3A);
	FUNC_PTR(_sendto,		0x43FB54);
	FUNC_PTR(_inet_addr,	0x44091E);
	FUNC_PTR(_htons,		0x440A7B);
	FUNC_PTR(_ntohl,		0x440A8A);

	// ******************************************************************
	// * Strings
	// ******************************************************************
	FUNC_PTR(_strlen,		FUNC_PTR_UNKNOWN);
	FUNC_PTR(__snprintf,	FUNC_PTR_UNKNOWN);
	FUNC_PTR(_strncpy,		FUNC_PTR_UNKNOWN);
	FUNC_PTR(__strnicmp,	FUNC_PTR_UNKNOWN);
	FUNC_PTR(__stricmp,		FUNC_PTR_UNKNOWN);
	FUNC_PTR(__snwprintf,	FUNC_PTR_UNKNOWN);
	FUNC_PTR(_sprintf,		FUNC_PTR_UNKNOWN);
	FUNC_PTR(_wcsncpy,		FUNC_PTR_UNKNOWN);
	FUNC_PTR(_wcscat,		FUNC_PTR_UNKNOWN);
	FUNC_PTR(_wcscpy,		FUNC_PTR_UNKNOWN);
	FUNC_PTR(_wcslen,		FUNC_PTR_UNKNOWN);
	FUNC_PTR(_memcpy,		FUNC_PTR_UNKNOWN);
	FUNC_PTR(_atexit,		FUNC_PTR_UNKNOWN);
	FUNC_PTR(_strncmp,		FUNC_PTR_UNKNOWN);
	FUNC_PTR(_qsort,		FUNC_PTR_UNKNOWN);
	FUNC_PTR(_atol,			FUNC_PTR_UNKNOWN);
	FUNC_PTR(__atoi64,		FUNC_PTR_UNKNOWN);
	FUNC_PTR(_strstr,		FUNC_PTR_UNKNOWN);
	FUNC_PTR(_wcschr,		FUNC_PTR_UNKNOWN);
	FUNC_PTR(_wcsncmp,		FUNC_PTR_UNKNOWN);
	FUNC_PTR(_wcsstr,		FUNC_PTR_UNKNOWN);
	FUNC_PTR(__wcsnicmp,	FUNC_PTR_UNKNOWN);
	FUNC_PTR(_strrchr,		FUNC_PTR_UNKNOWN);
	FUNC_PTR(_wcsncat,		FUNC_PTR_UNKNOWN);
	FUNC_PTR(_vsprintf,		FUNC_PTR_UNKNOWN);


//////////////////////////////////////////////////////////////////////////
// LIB_API_VTABLE
	FUNC_PTR(_new,			FUNC_PTR_UNKNOWN);
	FUNC_PTR(_delete,		FUNC_PTR_UNKNOWN);
	FUNC_PTR(_new_vector,	FUNC_PTR_UNKNOWN);
	FUNC_PTR(_delete_vector,FUNC_PTR_UNKNOWN);
};

#else
	#error Undefined engine layout include for: __EL_INCLUDE_OPEN_SAUCE
#endif