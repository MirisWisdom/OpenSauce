/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

//////////////////////////////////////////////////////////////////////////
// EventLog
static struct s_event_log_globals {
	cstring filename;
	FILE* file_handle;
}event_log_globals;

static void EventLogInitialize()
{
	event_log_globals.filename = Settings::K_EVENT_LOG_FILENAME;
	event_log_globals.file_handle = Settings::CreateReport(event_log_globals.filename, true, true);
}

static void EventLogDispose()
{
	if(event_log_globals.file_handle != nullptr)
		fclose(event_log_globals.file_handle);
}

static void WriteEventFirstLine()
{
	static const char format[] = "\n%s\n";

	tag_string time_str;
	GetTimeStampString(time_str);

	fprintf(event_log_globals.file_handle, format, time_str);
	fflush(event_log_globals.file_handle);
}

void WriteEvent(cstring str, bool write_time_stamp)
{
	static const char format[] = "%s%s%c";

	if(event_log_globals.file_handle == nullptr) return;

	static bool first_line = true;
	if(first_line)
	{
		first_line = false;
		WriteEventFirstLine();
	}

	tag_string time_str;
	if(write_time_stamp)
		GetTimeStampString(time_str);

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