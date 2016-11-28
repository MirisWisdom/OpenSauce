/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/cseries/errors_yelo.hpp>

#if PLATFORM_IS_EDITOR || defined(API_DEBUG)
#include <YeloLib/Halo1/open_sauce/settings/yelo_shared_settings.hpp>

namespace Yelo
{
	namespace Debug
	{
		static FILE* debug_file = nullptr;

		void FileInitialize()
		{
			debug_file = Settings::CreateReport(Settings::K_DEBUG_FILENAME, true, true);
		}

		void FileDispose()
		{
			if(debug_file != nullptr)
				fclose(debug_file);
		}

		static void WriteFirstLine()
		{
			static char format[] = "\n%s\n";

			tag_string time_str;
			GetTimeStampString(time_str);

			fprintf_s(debug_file, format, time_str);
			fflush(debug_file);
		}

		void Write(cstring str, bool write_time_stamp)
		{
			if(debug_file == nullptr) return;

			static char format[] = "%s%s%c";

			static bool first_line = true;
			if(first_line)
			{
				first_line = false;
				WriteFirstLine();
			}

			tag_string time_str;
			if(write_time_stamp)
				GetTimeStampString(time_str);

			char time_buffer[16];
			memset(time_buffer, 0, sizeof(time_buffer));

			if(write_time_stamp)
				strncpy_s(time_buffer, time_str+11, 9); // copy the time only, leave out the date and year

			if(IsDebuggerPresent())
			{
				OutputDebugString("DEBUG: ");
				OutputDebugString(time_buffer);
				OutputDebugString(str);
				OutputDebugString("\r\n");
			}

			if(write_time_stamp)
				fprintf_s(debug_file, format, time_buffer, str, '\n');
			else
				fprintf_s(debug_file, format+2, str, '\n');
			fflush(debug_file);
		}

		void WriteFormat(cstring format, ...)
		{
			char buffer[0x800];
			va_list argptr;
			va_start(argptr, format);
			vsprintf_s(buffer, format, argptr);
			va_end(argptr);

			Write(buffer, true);
		}
	};
};
#endif

#if !PLATFORM_IS_EDITOR
#include <blamlib/Halo1/cseries/errors.hpp>

namespace Yelo
{
	namespace Debug
	{
		static void PLATFORM_API Error(Enums::error_message_priority priority, cstring format, ...)
		{
		}
	};

	namespace blam
	{
		proc_error error = Yelo::Debug::Error;
	};
};
#endif