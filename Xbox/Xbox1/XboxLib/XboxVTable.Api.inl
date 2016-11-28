#ifndef VTABLE_ENTRY
	#define VTABLE_ENTRY(function) _##function function ;
	#define VTABLE_ENTRYA(function) _##function##A function ;
#endif


#ifdef CreateDirectory
	#undef CreateDirectory
#endif

#ifdef RemoveDirectory
	#undef RemoveDirectory
#endif
// ******************************************************************
// * Directory I\O
// ******************************************************************
VTABLE_ENTRYA(CreateDirectory)
VTABLE_ENTRYA(RemoveDirectory)


#ifdef CreateFile
	#undef CreateFile
#endif

#ifdef DeleteFile
	#undef DeleteFile
#endif

#ifdef FindFirstFile
	#undef FindFirstFile
#endif

#ifdef FindNextFile
	#undef FindNextFile
#endif
// ******************************************************************
// * File I\O
// ******************************************************************
VTABLE_ENTRY(GetFileSize)
VTABLE_ENTRY(GetFileSizeEx)
VTABLE_ENTRY(WriteFile)
VTABLE_ENTRY(ReadFile)
VTABLE_ENTRY(FlushFileBuffers)
VTABLE_ENTRY(SetEndOfFile)
VTABLE_ENTRY(SetFilePointer)
VTABLE_ENTRY(SetFilePointerEx)
VTABLE_ENTRY(GetFileTime)
VTABLE_ENTRYA(CreateFile)
VTABLE_ENTRYA(DeleteFile)
VTABLE_ENTRYA(FindFirstFile)
VTABLE_ENTRYA(FindNextFile)

// ******************************************************************
// * Misc
// ******************************************************************
VTABLE_ENTRY(GetSystemTimeAsFileTime)
VTABLE_ENTRY(CloseHandle)
VTABLE_ENTRY(GetLastError)
VTABLE_ENTRY(MultiByteToWideChar)
VTABLE_ENTRY(WideCharToMultiByte)

// ******************************************************************
// * Networking
// ******************************************************************
VTABLE_ENTRY(XNetGetTitleXnAddr)


#ifdef CreateEvent
	#undef CreateEvent
#endif

#ifdef CreateSemaphore
	#undef CreateSemaphore
#endif
// ******************************************************************
// * Threading
// ******************************************************************
VTABLE_ENTRY(CreateThread)
VTABLE_ENTRY(GetCurrentThreadId)
VTABLE_ENTRYA(CreateEvent)
VTABLE_ENTRYA(CreateSemaphore)

// ******************************************************************
// * Signature
// ******************************************************************
VTABLE_ENTRY(XCalculateSignatureBegin)
VTABLE_ENTRY(XCalculateSignatureBeginEx)
VTABLE_ENTRY(XCalculateSignatureEnd)
VTABLE_ENTRY(XCalculateSignatureGetSize)
VTABLE_ENTRY(XCalculateSignatureUpdate)

// ******************************************************************
// * Virtual Memory
// ******************************************************************
VTABLE_ENTRY(VirtualAlloc)
VTABLE_ENTRY(VirtualFree)
VTABLE_ENTRY(VirtualProtect)

VTABLE_ENTRY(XMemAlloc)
VTABLE_ENTRY(XMemFree)


#undef VTABLE_ENTRY
#undef VTABLE_ENTRYA