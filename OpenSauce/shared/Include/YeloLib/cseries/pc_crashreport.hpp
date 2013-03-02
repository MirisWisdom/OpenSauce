/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Flags
	{
		enum crashreport_option_flags : int
		{
			_crashreport_option_save_local_bit	= 1,
			_crashreport_option_full_dump_bit	= 2 | _crashreport_option_save_local_bit, // don't send full dumps to the report server
			_crashreport_option_hide_gui_bit	= 4,

			k_number_of_crashreport_option_flags
		};
	};

	namespace Debug
	{
		typedef void (*t_report_callback)(const char* /*report_directory*/);

		struct s_crash_report_options
		{
			Flags::crashreport_option_flags m_flags;
			t_report_callback m_report_complete_callback;

			cstring m_application_name;
			cstring m_application_version;

			cstring m_reports_directory;
			cstring m_dependency_path;

			cstring m_report_server_url;
			cstring m_privacy_policy_url;
		};

		void InitDefaultOptions(s_crash_report_options& options);

		bool InstallExceptionHandler(s_crash_report_options& crashreport_options);
		void UninstallExceptionHandler();

		bool AddFileToCrashReport(LPCSTR path, LPCSTR name, LPCSTR description);
		bool AddPropertyToCrashReport(LPCSTR name, LPCSTR value);
		bool AddRegistryKeyToCrashReport(LPCSTR key, LPCSTR value);
		bool AddScreenshotToCrashReport();

		void ForceCrashReport(HANDLE thread = INVALID_HANDLE_VALUE);

		int WINAPI SEHExceptionFilter(PEXCEPTION_POINTERS ptrs);
	};
};