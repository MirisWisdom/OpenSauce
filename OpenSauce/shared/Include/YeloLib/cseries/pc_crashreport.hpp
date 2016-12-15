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
			_crashreport_option_save_local_flag	= 1,
			_crashreport_option_full_dump_flag	= 2 | _crashreport_option_save_local_flag, // don't send full dumps to the report server
			_crashreport_option_hide_gui_flag	= 4,
		};
	};

	namespace Debug
	{
		typedef void (*proc_report_callback)(const char* /*report_directory*/);

		struct s_crash_report_options
		{
			Flags::crashreport_option_flags m_flags;
			proc_report_callback m_report_complete_callback;

			cstring m_application_name;
			cstring m_application_version;

			cstring m_reports_directory;
			cstring m_dependency_path;

			cstring m_report_server_url;
			cstring m_privacy_policy_url;
		};

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Initialises the default crash report options. </summary>
		/// <param name="options">	[in,out] Options for controlling the crash report. </param>
		///-------------------------------------------------------------------------------------------------
		void InitDefaultOptions(s_crash_report_options& options);

		///-------------------------------------------------------------------------------------------------
		/// <summary>
		/// 	Installs the crashrpt exception handler using the supplied crash report options.
		/// </summary>
		/// <param name="crashreport_options">	[in] Options for controlling the crash report. </param>
		/// <returns>	true if it succeeds, false if it fails. </returns>
		///-------------------------------------------------------------------------------------------------
		bool InstallExceptionHandler(s_crash_report_options& crashreport_options);
		
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Uninstalls the crashrpt exception handler. </summary>
		///-------------------------------------------------------------------------------------------------
		void UninstallExceptionHandler();

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Adds a file to the crash report. </summary>
		/// <param name="path">		  	Full pathname of the file. </param>
		/// <param name="name">		  	The name for the file. </param>
		/// <param name="description">	The description of the file. </param>
		/// <returns>	true if it succeeds, false if it fails. </returns>
		///-------------------------------------------------------------------------------------------------
		bool AddFileToCrashReport(LPCSTR path, LPCSTR name, LPCSTR description);
		
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Adds a property to the crash report. </summary>
		/// <param name="name"> 	The name for the property. </param>
		/// <param name="value">	The property's value. </param>
		/// <returns>	true if it succeeds, false if it fails. </returns>
		///-------------------------------------------------------------------------------------------------
		bool AddPropertyToCrashReport(LPCSTR name, LPCSTR value);
		
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Adds a registry key to the crash report. </summary>
		/// <param name="key">  	The registry key to add. </param>
		/// <returns>	true if it succeeds, false if it fails. </returns>
		///-------------------------------------------------------------------------------------------------
		bool AddRegistryKeyToCrashReport(LPCSTR key);
		
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Adds screenshot to the crash report. </summary>
		/// <returns>	true if it succeeds, false if it fails. </returns>
		///-------------------------------------------------------------------------------------------------
		bool AddScreenshotToCrashReport();

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Forces a crash report to be generated for the supplied thread. </summary>
		/// <param name="thread">	Handle of the thread. </param>
		///-------------------------------------------------------------------------------------------------
		void ForceCrashReport(HANDLE thread = INVALID_HANDLE_VALUE);

		///-------------------------------------------------------------------------------------------------
		/// <summary>	An exception filter that generates a crash report. </summary>
		/// <param name="ptrs">	The exception ptrs. </param>
		/// <returns>
		/// 	Returns EXCEPTION_CONTINUE_SEARCH if the exception was not handled, otherwise
		/// 	EXCEPTION_EXECUTE_HANDLER.
		/// </returns>
		///-------------------------------------------------------------------------------------------------
		int WINAPI SEHExceptionFilter(PEXCEPTION_POINTERS ptrs);
	};
};