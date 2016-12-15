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
	FUNC_PTR(_XNetGetTitleXnAddr,		FUNC_PTR_UNKNOWN);

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
	FUNC_PTR(_socket,		FUNC_PTR_UNKNOWN);
	FUNC_PTR(_closesocket,	FUNC_PTR_UNKNOWN);
	FUNC_PTR(_shutdown,		FUNC_PTR_UNKNOWN);
	FUNC_PTR(_ioctlsocket,	FUNC_PTR_UNKNOWN);
	FUNC_PTR(_setsockopt,	FUNC_PTR_UNKNOWN);
	FUNC_PTR(_getsockopt,	FUNC_PTR_UNKNOWN);
	FUNC_PTR(_bind,			FUNC_PTR_UNKNOWN);
	FUNC_PTR(_connect,		FUNC_PTR_UNKNOWN);
	FUNC_PTR(_select,		FUNC_PTR_UNKNOWN);
	FUNC_PTR(_recv,			FUNC_PTR_UNKNOWN);
	FUNC_PTR(_recvfrom,		FUNC_PTR_UNKNOWN);
	FUNC_PTR(_send,			FUNC_PTR_UNKNOWN);
	FUNC_PTR(_sendto,		FUNC_PTR_UNKNOWN);
	FUNC_PTR(_inet_addr,	FUNC_PTR_UNKNOWN);
	FUNC_PTR(_htons,		FUNC_PTR_UNKNOWN);
	FUNC_PTR(_ntohl,		FUNC_PTR_UNKNOWN);

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