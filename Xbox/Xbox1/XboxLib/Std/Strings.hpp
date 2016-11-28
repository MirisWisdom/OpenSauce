/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
#pragma once

// ******************************************************************
// * Strings
// ******************************************************************
	XBOX_STDPROC(size_t, strlen, (
				 const char* str
				 ));
	#define Std_strlen XboxStd.strlen

	XBOX_STDPROC(int, _snprintf, (
				 char* buffer,
				 size_t count,
				 const char* format,
				 ...
				 ));
	#define Std_snprintf XboxStd._snprintf

	XBOX_STDPROC(char*, strncpy, (
				 char* strDest,
				 const char* strSrc,
				 size_t count
				 ));
	#define Std_strncpy XboxStd.strncpy

	XBOX_STDPROC(int, _strnicmp, (
				 const char* s1,
				 const char* s2,
				 size_t count
				 ));
	#define Std_strnicmp XboxStd.strnicmp

	XBOX_STDPROC(int, _stricmp, (
				 const char* s1,
				 const char* s2
				 ));
	#define Std_stricmp XboxStd.stricmp

	XBOX_STDPROC(int, _snwprintf, (
				 wchar_t* buffer,
				 size_t count,
				 const wchar_t* format,
				 ...
				 ));
	#define Std_snwprintf XboxStd._snwprintf

	XBOX_STDPROC(int, sprintf, (
				 char* buffer,
				 const char* format,
				 ...
				 ));
	#define Std_sprintf XboxStd.sprintf

	XBOX_STDPROC(wchar_t*, wcsncpy, (
				 wchar_t* strDest,
				 const wchar_t* strSrc,
				 size_t count
				 ));
	#define Std_wcsncpy XboxStd.wcsncpy

	XBOX_STDPROC(wchar_t*, wcscat, (
				 wchar_t* strDest,
				 const wchar_t* strSrc
				 ));
	#define Std_wcscat XboxStd.wcscat

	XBOX_STDPROC(wchar_t*, wcscpy, (
				 wchar_t* strDest,
				 const wchar_t* strSrc
				 ));
	#define Std_wcscpy XboxStd.wcscpy

	XBOX_STDPROC(size_t, wcslen, (
				 wchar_t* str
				 ));
	#define Std_wcslen XboxStd.wcslen

	XBOX_STDPROC(void*, memcpy, (
				 void* dest,
				 const void* src,
				 size_t count
				 ));
	#define Std_memcpy XboxStd.memcpy

	XBOX_STDPROC(int, atexit, (
				 void (__cdecl *func )( void )
				 ));
	#define Std_atexit XboxStd.atexit

	XBOX_STDPROC(int, strncmp, (
				 const char* string1,
				 const char* string2,
				 size_t count
				 ));
	#define Std_strncmp XboxStd.strncmp

	XBOX_STDPROC(void, qsort, (
				 void* base,
				 size_t num,
				 size_t width,
				 int (__cdecl *compare )(const void *, const void *)
				 ));
	#define Std_qsort XboxStd.qsort

	XBOX_STDPROC(long, atol, (
				 const char *str
				 ));
	#define Std_atol XboxStd.atol

	XBOX_STDPROC(__int64, _atoi64, (
				 const char *str
				 ));
	#define Std_atoi64 XboxStd.atoi64

	XBOX_STDPROC(char*, strstr, (
				 const char* str,
				 const char* strSearch
				 ));
	#define Std_strstr XboxStd.strstr

	XBOX_STDPROC(wchar_t*, wcschr, (
				 const wchar_t* str,
				 wchar_t c
				 ));
	#define Std_wcschr XboxStd.wcschr

	XBOX_STDPROC(int, wcsncmp, (
				 const wchar_t* string1,
				 const wchar_t* string2,
				 size_t count
				 ));
	#define Std_wcsncmp XboxStd.wcsncmp

	XBOX_STDPROC(wchar_t*, wcsstr, (
				 const wchar_t* str,
				 const wchar_t* strSearch
				 ));
	#define Std_wcsstr XboxStd.wcsstr

	XBOX_STDPROC(int, _wcsnicmp, (
				 const wchar_t* string1,
				 const wchar_t* string2,
				 size_t count
				 ));
	#define Std_wcsnicmp XboxStd.wcsnicmp

	XBOX_STDPROC(char*, strrchr, (
				 const char* str,
				 int c
				 ));
	#define Std_strrchr XboxStd.strrchr

	XBOX_STDPROC(wchar_t*, wcsncat, (
				 wchar_t* strDest,
				 const wchar_t* strSource,
				 size_t count
				 ));
	#define Std_wcsncat XboxStd.wcsncat

	XBOX_STDPROC(int, vsprintf, (
				 char* bufer,
				 const char* format,
				 va_list argptr
				 ));
	#define Std_vsprintf XboxStd.vsprintf