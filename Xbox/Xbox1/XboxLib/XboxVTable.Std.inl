#ifndef VTABLE_ENTRY
	#define VTABLE_ENTRY(function) _##function function ;
	#define VTABLE_ENTRY_(function) __##function function ;
#endif


// ******************************************************************
// * File I\O
// ******************************************************************
VTABLE_ENTRY_(fsopen)
VTABLE_ENTRY(fclose)
VTABLE_ENTRY(fflush)
VTABLE_ENTRY(fopen)
VTABLE_ENTRY(fgets)
VTABLE_ENTRY(fseek)
VTABLE_ENTRY(ftell)

// ******************************************************************
// * Math
// ******************************************************************
VTABLE_ENTRY(srand)
VTABLE_ENTRY(rand)

// ******************************************************************
// * Memory
// ******************************************************************
VTABLE_ENTRY(malloc)

// ******************************************************************
// * Networking
// ******************************************************************
VTABLE_ENTRY(socket)
VTABLE_ENTRY(closesocket)
VTABLE_ENTRY(shutdown)
VTABLE_ENTRY(ioctlsocket)
VTABLE_ENTRY(setsockopt)
VTABLE_ENTRY(getsockopt)
VTABLE_ENTRY(bind)
VTABLE_ENTRY(connect)
VTABLE_ENTRY(select)
VTABLE_ENTRY(recv)
VTABLE_ENTRY(recvfrom)
VTABLE_ENTRY(send)
VTABLE_ENTRY(sendto)
VTABLE_ENTRY(inet_addr)
VTABLE_ENTRY(htons)
VTABLE_ENTRY(ntohl)

// ******************************************************************
// * Strings
// ******************************************************************
VTABLE_ENTRY(strlen)
VTABLE_ENTRY_(snprintf)
VTABLE_ENTRY(strncpy)
VTABLE_ENTRY_(strnicmp)
VTABLE_ENTRY_(stricmp)
VTABLE_ENTRY_(snwprintf)
VTABLE_ENTRY(sprintf)
VTABLE_ENTRY(wcsncpy)
VTABLE_ENTRY(wcscat)
VTABLE_ENTRY(wcscpy)
VTABLE_ENTRY(wcslen)
VTABLE_ENTRY(memcpy)
VTABLE_ENTRY(atexit)
VTABLE_ENTRY(strncmp)
VTABLE_ENTRY(qsort)
VTABLE_ENTRY(atol)
VTABLE_ENTRY_(atoi64)
VTABLE_ENTRY(strstr)
VTABLE_ENTRY(wcschr)
VTABLE_ENTRY(wcsncmp)
VTABLE_ENTRY(wcsstr)
VTABLE_ENTRY_(wcsnicmp)
VTABLE_ENTRY(strrchr)
VTABLE_ENTRY(wcsncat)
VTABLE_ENTRY(vsprintf)


#undef VTABLE_ENTRY
#undef VTABLE_ENTRY_