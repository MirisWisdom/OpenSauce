/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"

#if PLATFORM_TARGET != PLATFORM_TARGET_XBOX
	#include <crtdbg.h>

	#define WIN32_FUNC(func) func
#else
	#define WIN32_FUNC(func) BOOST_PP_CAT(Api, func )
#endif

const errno_t k_errnone = 0;

namespace Yelo
{
	const sbyte K_SBYTE_MIN	=	CAST(sbyte, -128);
	const sbyte K_SBYTE_MAX	=	CAST(sbyte, 127);
	const byte K_BYTE_MIN =		CAST(byte, 0);
	const byte K_BYTE_MAX =		CAST(byte, 255);
	const int16 K_INT16_MIN	=	CAST(int16, -32768);
	const int16 K_INT16_MAX	=	CAST(int16, 32767);
	const uint16 K_UINT16_MIN =	CAST(uint16, 0);
	const uint16 K_UINT16_MAX =	CAST(uint16, 65535);
	const int32 K_INT32_MIN	=	CAST(int32, -2147483646);
	const int32 K_INT32_MAX	=	CAST(int32, 2147483647);
	const uint32 K_UINT32_MIN =	CAST(uint32, 0);
	const uint32 K_UINT32_MAX =	CAST(uint32, 4294967295);
	const int64 K_INT64_MIN	=	CAST(int64, -9223372036854775806);
	const int64 K_INT64_MAX	=	CAST(int64, 9223372036854775807);
	const uint64 K_UINT64_MIN =	CAST(uint64, 0);
	const uint64 K_UINT64_MAX =	CAST(uint64, 18446744073709551615);
	const real K_REAL_MIN	=	CAST(real, 1.175494351e-38F);
	const real K_REAL_MAX	=	CAST(real, 3.402823466e+38F);

	char* wstring_to_string(char* string, int32 string_length, wcstring wide, int32 wide_length)
	{
		if( !WIN32_FUNC(WideCharToMultiByte)(CP_ACP, 0, wide, wide_length, string, string_length, NULL, NULL) )
			return NULL;
		else
			return string;
	}
	char* wstring_to_string_lazy(char* string, int32 string_length, wcstring wide)
	{
		for(int32 x = 0; wide[x] && string_length--; x++)
			string[x] = (char)wide[x];

		return string;
	}

	wstring string_to_wstring(wstring wide, int32 wide_length, cstring string, int32 string_length)
	{
		if( !WIN32_FUNC(MultiByteToWideChar)(CP_ACP, 0, string, string_length, wide, wide_length) )
			return NULL;
		else
			return wide;
	}
	wstring string_to_wstring_lazy(wstring string, int32 string_length, cstring ascii)
	{
		for(int32 x = 0; ascii[x] && string_length--; x++)
			string[x] = (wchar_t)ascii[x];

		return string;
	}

	cstring BooleanToString(bool value)
	{
		return value ? "true" : "false";
	}

#if PLATFORM_TARGET != PLATFORM_TARGET_XBOX
	void GetTimeStampString(_Out_ tag_string time_str)
	{
		const size_t k_time_str_sizeof = sizeof(tag_string);

		memset(time_str, 0, k_time_str_sizeof);

		tm newtime;
		time_t aclock;	time( &aclock ); // Get time in seconds
		localtime_s( &newtime, &aclock ); // Convert time to struct tm form
		asctime_s( time_str, k_time_str_sizeof, &newtime );
	}

	void GetTimeStampStringForFile(_Out_ tag_string time_str)
	{
		const size_t k_time_str_sizeof = sizeof(tag_string);

		memset(time_str, 0, k_time_str_sizeof);

		tm newtime;
		time_t aclock;	time( &aclock ); // Get time in seconds
		localtime_s( &newtime, &aclock ); // Convert time to struct tm form

		sprintf_s(time_str, k_time_str_sizeof, "%04d_%02d_%02d_%02d_%02d_%02d", 
			newtime.tm_year + 1900,
			newtime.tm_mon + 1,
			newtime.tm_mday,
			newtime.tm_hour,
			newtime.tm_min,
			newtime.tm_sec);
	}

	void PrepareToDropError(cstring text)
	{
		if(text == NULL) text = "(null)";

		MessageBox(NULL, text, "Prepare to Drop!", MB_OK | MB_ICONEXCLAMATION);
	}

	#if defined(API_DEBUG) && defined(ASSERTS_ENABLED)
	void Assert(cstring assertion, cstring message, cstring file, const int line, cstring function)
	{
		YELO_DEBUG_FORMAT("Assertion: %s", assertion);
		YELO_DEBUG_FORMAT("Message: %s", message);
		YELO_DEBUG_FORMAT("File: %s", file);
		YELO_DEBUG_FORMAT("Line: %i", line);
		YELO_DEBUG_FORMAT("Function: %s", function);

		// based on _ASSERT_EXPR's implementation
		_CrtDbgReport(_CRT_ASSERT, file, line, NULL, assertion);

		throw;
	}
	#endif
#endif
};