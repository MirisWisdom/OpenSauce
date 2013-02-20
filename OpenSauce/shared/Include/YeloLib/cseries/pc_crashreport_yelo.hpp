/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum crashreport_option_flags : int
		{
			_crashreport_options_save_local	= 1,
			_crashreport_options_full_dump	= 2 | _crashreport_options_save_local, // don't send full dumps to the report server
			_crashreport_options_hide_gui	= 4,

			_crashreport_options
		};
	};

	namespace Debug
	{
		typedef void (*t_report_callback)(const char* /*report_directory*/);

		struct s_crash_report_options
		{
			Enums::crashreport_option_flags m_flags;
			t_report_callback m_report_complete_callback;

			cstring m_application_name;
			cstring m_application_version;

			cstring m_reports_directory;
			cstring m_dependency_path;

			cstring m_report_server_url;
			cstring m_privacy_policy_url;
		};

		bool InstallExceptionHandler(s_crash_report_options& crashreport_options);
		void UninstallExceptionHandler();

		bool AddFileToCrashReport(LPCSTR path, LPCSTR name, LPCSTR description);
		bool AddPropertyToCrashReport(LPCSTR name, LPCSTR value);
		bool AddRegistryKeyToCrashReport(LPCSTR key, LPCSTR value);
		bool AddScreenshotToCrashReport();

		void ForceCrashReport();

		int WINAPI SEHExceptionFilter(PEXCEPTION_POINTERS ptrs);
	};
};