/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#include "Common/Precompile.hpp"

namespace Yelo
{
#define __EL_INCLUDE_ID			__EL_INCLUDE_OPEN_SAUCE
#define __EL_INCLUDE_FILE_ID	__EL_COMMON_PRECOMPILE
#include "Memory/_EngineLayout.inl"

#define XBOX_LIB_PTR(type)	CAST_PTR(type, CAST_PTR(void*, Yelo::Common::GET_FUNC_PTR(type)) )
};

#if defined(__cplusplus)
extern "C" {
#endif

DWORD k_module_title_id =		PLATFORM_VALUE(0x4D530064, 0x4D530064, 0x4D530082); // Halo 2 TitleId
DWORD k_module_title_version =	PLATFORM_VALUE(0x00000003, 0x00000803, 0x00000001);

#pragma region XboxLib declares
XBOX_API_VTABLE XboxApi = {
// ******************************************************************
// * Directory I\O
// ******************************************************************
	XBOX_LIB_PTR(_CreateDirectoryA),
	XBOX_LIB_PTR(_RemoveDirectoryA),

// ******************************************************************
// * File I\O
// ******************************************************************
	XBOX_LIB_PTR(_GetFileSize),
	XBOX_LIB_PTR(_GetFileSizeEx),
	XBOX_LIB_PTR(_WriteFile),
	XBOX_LIB_PTR(_ReadFile),
	XBOX_LIB_PTR(_FlushFileBuffers),
	XBOX_LIB_PTR(_SetEndOfFile),
	XBOX_LIB_PTR(_SetFilePointer),
	XBOX_LIB_PTR(_SetFilePointerEx),
	XBOX_LIB_PTR(_GetFileTime),
	XBOX_LIB_PTR(_CreateFileA),
	XBOX_LIB_PTR(_DeleteFileA),
	XBOX_LIB_PTR(_FindFirstFileA),
	XBOX_LIB_PTR(_FindNextFileA),

// ******************************************************************
// * Misc
// ******************************************************************
	XBOX_LIB_PTR(_GetSystemTimeAsFileTime),
	XBOX_LIB_PTR(_CloseHandle),
	XBOX_LIB_PTR(_GetLastError),
	XBOX_LIB_PTR(_MultiByteToWideChar),
	XBOX_LIB_PTR(_WideCharToMultiByte),

// ******************************************************************
// * Networking
// ******************************************************************
	XBOX_LIB_PTR(_XNetGetTitleXnAddr),

// ******************************************************************
// * Threading
// ******************************************************************
	XBOX_LIB_PTR(_CreateThread),
	XBOX_LIB_PTR(_GetCurrentThreadId),
	XBOX_LIB_PTR(_CreateEventA),
	XBOX_LIB_PTR(_CreateSemaphoreA),

// ******************************************************************
// * Signature
// ******************************************************************
	XBOX_LIB_PTR(_XCalculateSignatureBegin),
	XBOX_LIB_PTR(_XCalculateSignatureBeginEx),
	XBOX_LIB_PTR(_XCalculateSignatureEnd),
	XBOX_LIB_PTR(_XCalculateSignatureGetSize),
	XBOX_LIB_PTR(_XCalculateSignatureUpdate),

// ******************************************************************
// * Virtual Memory
// ******************************************************************
	XBOX_LIB_PTR(_VirtualAlloc),
	XBOX_LIB_PTR(_VirtualFree),
	XBOX_LIB_PTR(_VirtualProtect),

	XBOX_LIB_PTR(_XMemAlloc),
	XBOX_LIB_PTR(_XMemFree),
};


XBOX_STD_VTABLE XboxStd = {
// ******************************************************************
// * File I\O
// ******************************************************************
	XBOX_LIB_PTR(__fsopen),
	XBOX_LIB_PTR(_fclose),
	XBOX_LIB_PTR(_fflush),
	XBOX_LIB_PTR(_fopen),
	XBOX_LIB_PTR(_fgets),
	XBOX_LIB_PTR(_fseek),
	XBOX_LIB_PTR(_ftell),

// ******************************************************************
// * Math
// ******************************************************************
	XBOX_LIB_PTR(_srand),
	XBOX_LIB_PTR(_rand),

// ******************************************************************
// * Memory
// ******************************************************************
	XBOX_LIB_PTR(_malloc),

// ******************************************************************
// * Networking
// ******************************************************************
	XBOX_LIB_PTR(_socket),
	XBOX_LIB_PTR(_closesocket),
	XBOX_LIB_PTR(_shutdown),
	XBOX_LIB_PTR(_ioctlsocket),
	XBOX_LIB_PTR(_setsockopt),
	XBOX_LIB_PTR(_getsockopt),
	XBOX_LIB_PTR(_bind),
	XBOX_LIB_PTR(_connect),
	XBOX_LIB_PTR(_select),
	XBOX_LIB_PTR(_recv),
	XBOX_LIB_PTR(_recvfrom),
	XBOX_LIB_PTR(_send),
	XBOX_LIB_PTR(_sendto),
	XBOX_LIB_PTR(_inet_addr),
	XBOX_LIB_PTR(_htons),
	XBOX_LIB_PTR(_ntohl),

// ******************************************************************
// * Strings
// ******************************************************************
	XBOX_LIB_PTR(_strlen),
	XBOX_LIB_PTR(__snprintf),
	XBOX_LIB_PTR(_strncpy),
	XBOX_LIB_PTR(__strnicmp),
	XBOX_LIB_PTR(__stricmp),
	XBOX_LIB_PTR(__snwprintf),
	XBOX_LIB_PTR(_sprintf),
	XBOX_LIB_PTR(_wcsncpy),
	XBOX_LIB_PTR(_wcscat),
	XBOX_LIB_PTR(_wcscpy),
	XBOX_LIB_PTR(_wcslen),
	XBOX_LIB_PTR(_memcpy),
	XBOX_LIB_PTR(_atexit),
	XBOX_LIB_PTR(_strncmp),
	XBOX_LIB_PTR(_qsort),
	XBOX_LIB_PTR(_atol),
	XBOX_LIB_PTR(__atoi64),
	XBOX_LIB_PTR(_strstr),
	XBOX_LIB_PTR(_wcschr),
	XBOX_LIB_PTR(_wcsncmp),
	XBOX_LIB_PTR(_wcsstr),
	XBOX_LIB_PTR(__wcsnicmp),
	XBOX_LIB_PTR(_strrchr),
	XBOX_LIB_PTR(_wcsncat),
	XBOX_LIB_PTR(_vsprintf),
};
#pragma endregion

#if defined(__cplusplus)
};
#endif

#pragma region new/delete hacks
static LIB_API_VTABLE LibApi = {
	XBOX_LIB_PTR(_new),
	XBOX_LIB_PTR(_delete),
	XBOX_LIB_PTR(_new_vector),
	XBOX_LIB_PTR(_delete_vector),
};
LIB_API_VTABLE* gLIB_API_VTABLE = &LibApi;
#pragma endregion