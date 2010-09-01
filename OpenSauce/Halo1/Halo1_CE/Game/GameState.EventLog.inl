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

//////////////////////////////////////////////////////////////////////////
// EventLog
static struct s_event_log_globals {
	cstring filename;
	FILE* file_handle;
}event_log_globals;

static void EventLogInitialize()
{
	event_log_globals.filename = "Game.Log.txt";
	event_log_globals.file_handle = Settings::CreateReport(event_log_globals.filename, true, true);
}

static void EventLogDispose()
{
	if(event_log_globals.file_handle != NULL)
		fclose(event_log_globals.file_handle);
}

static void WriteEventFirstLine()
{
	static const char format[] = "\n%s\n";

	tm* newtime;
	time_t aclock;	time( &aclock ); // Get time in seconds
	newtime = localtime( &aclock ); // Convert time to struct tm form

	fprintf(event_log_globals.file_handle, format, asctime(newtime));
	fflush(event_log_globals.file_handle);
}

void WriteEvent(cstring str, bool write_time_stamp)
{
	static const char format[] = "%s%s%c";

	if(event_log_globals.file_handle == NULL) return;

	static bool first_line = true;
	if(first_line)
	{
		first_line = false;
		WriteEventFirstLine();
	}

	char* time_str;
	if(write_time_stamp)
	{
		tm* newtime;
		time_t aclock;	time( &aclock ); // Get time in seconds
		newtime = localtime( &aclock ); // Convert time to struct tm form
		time_str = asctime(newtime);
	}

	char time_buffer[16];
	memset(time_buffer, 0, sizeof(time_buffer));

	if(write_time_stamp)
		strncpy(time_buffer, time_str+11, 9); // copy the time only, leave out the date and year

	if(write_time_stamp)
		fprintf(event_log_globals.file_handle, format, time_buffer, str, '\n');
	else
		fprintf(event_log_globals.file_handle, format+2, str, '\n');
	fflush(event_log_globals.file_handle);
}
//////////////////////////////////////////////////////////////////////////