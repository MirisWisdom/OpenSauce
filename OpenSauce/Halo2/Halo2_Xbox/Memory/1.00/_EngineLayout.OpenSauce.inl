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

	// UTF8ToUnicode	331D50
	// UnicodeToUTF8	331EA6
	// IsBadReadPtr	3320E1
	// IsBadWritePtr	332146
	// IsBadCodePtr	3321AF

	// ******************************************************************
	// * Directory I\O
	// ******************************************************************
	FUNC_PTR(_CreateDirectoryA,			0x331A35);
	FUNC_PTR(_RemoveDirectoryA,			0x331AA1);

	// ******************************************************************
	// * File I\O
	// ******************************************************************
	FUNC_PTR(_GetFileSize,				0x2D2715);
	FUNC_PTR(_GetFileSizeEx,			0x2D26D7);
	FUNC_PTR(_WriteFile,				0x2D232E);
	FUNC_PTR(_ReadFile,					0x2D2241);
	FUNC_PTR(_FlushFileBuffers,			0x2D26B0);
	FUNC_PTR(_SetEndOfFile,				FUNC_PTR_NULL); // I haven't seen this defined in 1.00
	FUNC_PTR(_SetFilePointer,			0x2D2488);
	FUNC_PTR(_SetFilePointerEx,			0x2D2582);
	FUNC_PTR(_GetFileTime,				0x2D264A);
	FUNC_PTR(_CreateFileA,				0x2D2750);
	FUNC_PTR(_DeleteFileA,				0x2D6B6B);
	FUNC_PTR(_FindFirstFileA,			0x331B9B);
	FUNC_PTR(_FindNextFileA,			0x331CA8);

	// ******************************************************************
	// * Misc
	// ******************************************************************
	FUNC_PTR(_GetSystemTimeAsFileTime,	0x3303F3);
	FUNC_PTR(_CloseHandle,				0x2D1F3A);
	FUNC_PTR(_GetLastError,				0x2D1D3E);
	FUNC_PTR(_MultiByteToWideChar,		0x331F5E);
	FUNC_PTR(_WideCharToMultiByte,		0x332019);

	// ******************************************************************
	// * Networking
	// ******************************************************************
	FUNC_PTR(_XNetGetTitleXnAddr,		0x3CD17D);

	// ******************************************************************
	// * Threading
	// ******************************************************************
	FUNC_PTR(_CreateThread,				0x2D21C6);
	FUNC_PTR(_GetCurrentThreadId,		0x2D2234);
	FUNC_PTR(_CreateEventA,				0x2D616A);
	FUNC_PTR(_CreateSemaphoreA,			0x2D61EB);

	// ******************************************************************
	// * Signature
	// ******************************************************************
	FUNC_PTR(_XCalculateSignatureBegin,		0x2D6765);
	FUNC_PTR(_XCalculateSignatureBeginEx,	0x2D6724);
	FUNC_PTR(_XCalculateSignatureEnd,		0x2D67C7);
	FUNC_PTR(_XCalculateSignatureGetSize,	0x2D6703);
	FUNC_PTR(_XCalculateSignatureUpdate,	0x2D6779);

	// ******************************************************************
	// * Virtual Memory
	// ******************************************************************
	FUNC_PTR(_VirtualAlloc,				0x3312CC);
	FUNC_PTR(_VirtualFree,				0x3312FA);
	FUNC_PTR(_VirtualProtect,			FUNC_PTR_NULL); // I haven't found this in 1.00 yet

	FUNC_PTR(_XMemAlloc,				0x12280);
	FUNC_PTR(_XMemFree,					0x122C0);


//////////////////////////////////////////////////////////////////////////
// XBOX_STD_VTABLE
	// ******************************************************************
	// * File I\O
	// ******************************************************************
	FUNC_PTR(__fsopen,		0x321AE6);
	FUNC_PTR(_fclose,		0x321C71);
	FUNC_PTR(_fflush,		0x322907);
	FUNC_PTR(_fopen,		0x321B42);
	FUNC_PTR(_fgets,		0x321B9F);
	FUNC_PTR(_fseek,		0x3225B8);
	FUNC_PTR(_ftell,		0x322763);

	// ******************************************************************
	// * Math
	// ******************************************************************
	FUNC_PTR(_srand,		0x321F68);
	FUNC_PTR(_rand,			0x321F75);

	// ******************************************************************
	// * Memory
	// ******************************************************************
	FUNC_PTR(_malloc,		FUNC_PTR_NULL); // I haven't found this in 1.00 yet

	// ******************************************************************
	// * Networking
	// ******************************************************************
	FUNC_PTR(_socket,		0x3CD188);
	FUNC_PTR(_closesocket,	0x3CD193);
	FUNC_PTR(_shutdown,		0x3CD19E);
	FUNC_PTR(_ioctlsocket,	0x3CD1A9);
	FUNC_PTR(_setsockopt,	0x3CD1B4);
	FUNC_PTR(_getsockopt,	0x3CD1C3);
	FUNC_PTR(_bind,			0x3CD1D2);
	FUNC_PTR(_connect,		0x3CD1DD);
	FUNC_PTR(_select,		0x3CD1E8);
	FUNC_PTR(_recv,			0x3CD211);
	FUNC_PTR(_recvfrom,		0x3CD22B);
	FUNC_PTR(_send,			0x3CD23A);
	FUNC_PTR(_sendto,		0x3CD254);
	FUNC_PTR(_inet_addr,	0x3CE01E);
	FUNC_PTR(_htons,		0x3CE17B);
	FUNC_PTR(_ntohl,		0x3CE184);

	// ******************************************************************
	// * Strings
	// ******************************************************************
	FUNC_PTR(_strlen,		0x2D1469);
	FUNC_PTR(__snprintf,	0x32037F);
	FUNC_PTR(_strncpy,		0x3203E0);
	FUNC_PTR(__strnicmp,	0x320504);
	FUNC_PTR(__stricmp,		0x3205A1);
	FUNC_PTR(__snwprintf,	0x320725);
	FUNC_PTR(_sprintf,		0x320795);
	FUNC_PTR(_wcsncpy,		0x3207ED);
	FUNC_PTR(_wcscat,		0x32082A);
	FUNC_PTR(_wcscpy,		0x320854);
	FUNC_PTR(_wcslen,		0x320870);
	FUNC_PTR(_memcpy,		0x320890);
	FUNC_PTR(_atexit,		0x320FB4);
	FUNC_PTR(_strncmp,		0x321340);
	FUNC_PTR(_qsort,		0x321460);
	FUNC_PTR(_atol,			0x3216E3);
	FUNC_PTR(__atoi64,		0x321740);
	FUNC_PTR(_strstr,		0x3217C0);
	FUNC_PTR(_wcschr,		0x321CC2);
	FUNC_PTR(_wcsncmp,		0x321CE4);
	FUNC_PTR(_wcsstr,		0x321D19);
	FUNC_PTR(__wcsnicmp,	0x321D77);
	FUNC_PTR(_strrchr,		0x321FA0);
	FUNC_PTR(_wcsncat,		0x321FCD);
	FUNC_PTR(_vsprintf,		0x322B81);


//////////////////////////////////////////////////////////////////////////
// LIB_API_VTABLE
	FUNC_PTR(_new,			0x3202BB);
	FUNC_PTR(_delete,		0x324C91);
	FUNC_PTR(_new_vector,	0x2CE780);
	FUNC_PTR(_delete_vector,0x2CE7A0);
};

#else
	#error Undefined engine layout include for: __EL_INCLUDE_OPEN_SAUCE
#endif