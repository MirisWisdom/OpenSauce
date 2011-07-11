/*
    Yelo: Open Sauce SDK
		Halo 1 (CE) Edition
    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "Common/Precompile.hpp"
#include "Common/DebugFile.hpp"

#ifdef API_DEBUG
#include "Common/YeloSettings.hpp"

namespace Yelo
{
	namespace DebugFile
	{
		static FILE* debug_file = NULL;

		void Initialize()
		{
			debug_file = Settings::CreateReport("Game.debug.log", true, true);
		}

		void Dispose()
		{
			if(debug_file != NULL)
			{
				//fwrite("\n", sizeof(char), 1, debug_file);
				fclose(debug_file);
			}
		}

		static void GetTimeStampString(_Out_ tag_string time_str)
		{
			const size_t k_time_str_sizeof = sizeof(tag_string);

			memset(time_str, 0, k_time_str_sizeof);

			tm* newtime;
			time_t aclock;	time( &aclock ); // Get time in seconds
			localtime_s( newtime, &aclock ); // Convert time to struct tm form
			asctime_s( time_str, k_time_str_sizeof, newtime );
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
			if(debug_file == NULL) return;

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