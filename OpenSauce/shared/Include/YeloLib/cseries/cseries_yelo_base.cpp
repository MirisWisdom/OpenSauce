/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"

#include <blamlib/Halo1/cseries/cseries.hpp>

#if PLATFORM_TARGET != PLATFORM_TARGET_XBOX
	#include <crtdbg.h>
	#include <YeloLib/cseries/errors_yelo.hpp>
	#include <YeloLib/cseries/pc_crashreport.hpp>

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
		if( !WIN32_FUNC(WideCharToMultiByte)(CP_ACP, 0, wide, wide_length, string, string_length, nullptr, nullptr) )
			return nullptr;
		else
			return string;
	}
	char* wstring_to_string_lazy(char* string, int32 string_length, wcstring wide)
	{
		assert(string_length > 0);

		string[--string_length] = '\0';
		for(int32 x = 0; string_length--; x++)
		{
			string[x] = CAST(char, wide[x]);
			if(wide[x] == L'\0') break;
		}

		return string;
	}

	wstring string_to_wstring(wstring wide, int32 wide_length, cstring string, int32 string_length)
	{
		if( !WIN32_FUNC(MultiByteToWideChar)(CP_ACP, 0, string, string_length, wide, wide_length) )
			return nullptr;
		else
			return wide;
	}
	wstring string_to_wstring_lazy(wstring string, int32 string_length, cstring ascii)
	{
		assert(string_length > 0);

		string[--string_length] = L'\0';
		for(int32 x = 0; string_length--; x++)
		{
			string[x] = CAST(wchar_t, ascii[x]);
			if(ascii[x] == '\0') break;
		}

		return string;
	}

	bool EndsWith(const std::string& str, const std::string& suffix)
	{
		// based on http://stackoverflow.com/a/20447331/444977

		if (suffix.length() > str.length())
			return false;

		auto pos = str.length() - suffix.length();
		auto pos_a = &str[pos];
		auto pos_b = &suffix[0];
		while (*pos_a != '\0')
			if (*pos_a++ != *pos_b++)
				return false;

		return true;
	}

	void winapi_handle_closer::operator()(HANDLE h) const
	{
		YELO_ASSERT_DISPLAY(h != INVALID_HANDLE_VALUE, "tried to close an INVALID handle");
		if(h != nullptr)
			WIN32_FUNC(CloseHandle)(h);
	}

#if PLATFORM_TARGET != PLATFORM_TARGET_XBOX
	void winapi_local_deleter::operator()(HLOCAL h) const
	{
		YELO_ASSERT_DISPLAY(h != INVALID_HANDLE_VALUE, "tried to close an INVALID handle");
		if (h != nullptr)
			LocalFree(h);
	}

	void crt_file_closer::operator()(FILE* h) const
	{
		if(h != nullptr)
		{
			int result = fclose(h);
			YELO_ASSERT_DISPLAY(result == 0, "failed to fclose");
		}
	}
#endif

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
		time_str[25] = '\0'; // remove the \n character that asctime adds (http://msdn.microsoft.com/en-us/library/b6htak9c.aspx)
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
		if(text == nullptr) text = "(null)";

		MessageBox(nullptr, text, "Prepare to Drop!", MB_OK | MB_ICONEXCLAMATION);
	}

	#if YELO_ASSERT_ENABLED
	void Assert(cstring assertion, cstring message, cstring file, const int line, cstring function, bool halt)
	{
		YELO_DEBUG_FORMAT("Assertion: %s", assertion);
		// Yes, we want pointer equality as some assert calls just use the assertion as the message
		if (message != nullptr && message != assertion)
			YELO_DEBUG_FORMAT("Message: %s", message);
		YELO_DEBUG_FORMAT("%s,#%d in %s", file, line, function);

#ifdef _DEBUG
		// based on _ASSERT_EXPR's implementation
		_CrtDbgReport(halt ? _CRT_ASSERT : _CRT_WARN,
			file, line, nullptr, assertion);
#else // TODO: figure out a better alternative
		if (halt)
		{
			// TODO: AddPropertyToCrashReport returns true on success. do we -really- need to check it at this point?

			Debug::AddPropertyToCrashReport("Assert", assertion);

			if (message != assertion)
				Debug::AddPropertyToCrashReport("Assert-Msg", message);

			Debug::AddPropertyToCrashReport("Assert-File", file);

			tag_string line_string = { 0 };
			_itoa_s(line, line_string, 10);
			Debug::AddPropertyToCrashReport("Assert-Line", line_string);

			Debug::AddPropertyToCrashReport("Assert-Func", function);
		}
#endif

		if (halt)
			throw;
	}

	namespace blam
	{
		char g_display_assert_buffer[512];
	};
	#endif
#endif
};