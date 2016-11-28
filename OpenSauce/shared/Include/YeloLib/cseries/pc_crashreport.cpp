/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/cseries/pc_crashreport.hpp>

// have to set the packing value to 8 for CrashRpt as that is what the binary was built as
#pragma pack(push)
#pragma pack(8)
#include <CrashRpt/CrashRpt.h>
#pragma pack(pop)
#pragma comment (lib, "../../shared/lib/x86/CrashRpt1401.lib") // TODO: setup proper shared lib directories in vcxproj

#include <YeloLib/files/files.hpp>
#include <YeloLib/Halo1/open_sauce/yelo_version.hpp>

//#define TEST_CRASH_REPORT

namespace Yelo
{
	namespace Debug
	{
		// TODO: remove duplicated XML docs that already appear in the .hpp

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Callback that's called when the crash reporter progresses a stage. </summary>
		/// <param name="pInfo">	[in] Pointer to information about the crash report state. </param>
		/// <returns>	Returns CR_CB_NOTIFY_NEXT_STAGE. </returns>
		///-------------------------------------------------------------------------------------------------
		static int CrashCallback(CR_CRASH_CALLBACK_INFO* pInfo)
		{
			// if the report has been completed, call the custom callback to display a message to the user
			if((pInfo->nStage == CR_CB_STAGE_FINISH) && pInfo->pUserParam)
			{
				auto callback = CAST_PTR(proc_report_callback, pInfo->pUserParam);

				// send the error folder to the client callback
				callback(pInfo->pszErrorReportFolder);

				pInfo->bContinueExecution = false;
			}
			return CR_CB_NOTIFY_NEXT_STAGE;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Initialises the default crash report options. </summary>
		/// <param name="options">	[in,out] Options for controlling the crash report. </param>
		///-------------------------------------------------------------------------------------------------
		void InitDefaultOptions(s_crash_report_options& options)
		{
			// save reports locally and do not show the crashrpt gui
			int flags = Flags::_crashreport_option_hide_gui_flag | Flags::_crashreport_option_save_local_flag;
			options.m_flags = (Flags::crashreport_option_flags)flags;

			options.m_report_complete_callback = nullptr;

			options.m_application_name = nullptr;
			options.m_application_version = BOOST_STRINGIZE(K_OPENSAUCE_VERSION_BUILD_MAJ) "." BOOST_STRINGIZE(K_OPENSAUCE_VERSION_BUILD_MIN) "." BOOST_STRINGIZE(K_OPENSAUCE_VERSION_BUILD);

			options.m_reports_directory = nullptr;
			options.m_dependency_path = nullptr;

			options.m_report_server_url = nullptr;
			options.m_privacy_policy_url = nullptr;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>
		/// 	Installs the crashrpt exception handler using the supplied crash report options.
		/// </summary>
		/// <param name="crashreport_options">	[in] Options for controlling the crash report. </param>
		/// <returns>	true if it succeeds, false if it fails. </returns>
		///-------------------------------------------------------------------------------------------------
		bool InstallExceptionHandler(s_crash_report_options& crashreport_options)
		{
			// clear the installation info
			CR_INSTALL_INFO info;
			memset(&info, 0, sizeof(CR_INSTALL_INFO));

			info.cb = sizeof(info);

			// application details
			info.pszAppName = crashreport_options.m_application_name;
			info.pszAppVersion = crashreport_options.m_application_version;

			info.pszErrorReportSaveDir = crashreport_options.m_reports_directory;

			// dependencies (when null, looks in the working directory)
			info.pszCrashSenderPath = crashreport_options.m_dependency_path;
			info.pszDebugHelpDLL = crashreport_options.m_dependency_path;
			info.pszLangFilePath = crashreport_options.m_dependency_path;

			bool send_to_server = false;
			send_to_server  = (crashreport_options.m_flags & Flags::_crashreport_option_save_local_flag) == 0;
			send_to_server &= (crashreport_options.m_report_server_url != nullptr);

			// transfer type priorities
			if(send_to_server)
				info.uPriorities[CR_HTTP] = 1;
			else
				info.uPriorities[CR_HTTP] = CR_NEGATIVE_PRIORITY;
			info.uPriorities[CR_SMTP] = CR_NEGATIVE_PRIORITY;
			info.uPriorities[CR_SMAPI] = CR_NEGATIVE_PRIORITY;

			// server url
			if(send_to_server)
				info.pszUrl = crashreport_options.m_report_server_url;

			// privacy policy
			info.pszPrivacyPolicyURL = crashreport_options.m_privacy_policy_url;

			// email details
			info.pszEmailTo = nullptr;
			info.pszEmailSubject = nullptr;
			info.pszEmailText = nullptr;
			info.pszSmtpProxy = nullptr;
			info.pszSmtpLogin = nullptr;
			info.pszSmtpPassword = nullptr;

			// report flags
			info.dwFlags |= CR_INST_ALL_POSSIBLE_HANDLERS;

			// stop crashrpt from catching Ctrl+C
			info.dwFlags ^= CR_INST_SIGINT_HANDLER;
			
			if(!send_to_server)
			{
				info.dwFlags |= CR_INST_DONT_SEND_REPORT;
				info.dwFlags |= CR_INST_STORE_ZIP_ARCHIVES;

				if (Flags::_crashreport_option_hide_gui_flag == (crashreport_options.m_flags & Flags::_crashreport_option_hide_gui_flag))
					info.dwFlags |= CR_INST_NO_GUI;
			}

			// dump type
			if (Flags::_crashreport_option_full_dump_flag == (crashreport_options.m_flags & Flags::_crashreport_option_full_dump_flag))
				info.uMiniDumpType = MiniDumpWithFullMemory;
			else
				info.uMiniDumpType = MiniDumpNormal;

			// install the crashrpt handler with the generated info
			int result = crInstall(&info);
			if(result!=0)
			{
				// if the installation failed, and the gui is enabled, show an error message otherwise just return false
				if (Flags::_crashreport_option_hide_gui_flag == (crashreport_options.m_flags & Flags::_crashreport_option_hide_gui_flag))
					return false;

				TCHAR error_msg[256];
				crGetLastErrorMsg(error_msg, NUMBEROF(error_msg));
				MessageBox(nullptr, error_msg, "CrashRpt Install Error", MB_OK);
				return false;
			}

			// define the crash callback
			crSetCrashCallbackA(CAST_PTR(PFNCRASHCALLBACKA, &CrashCallback), crashreport_options.m_report_complete_callback);

		#ifdef TEST_CRASH_REPORT
			crEmulateCrash(0);
		#endif

			return true;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Uninstalls the crashrpt exception handler. </summary>
		///-------------------------------------------------------------------------------------------------
		void UninstallExceptionHandler()
		{
			crUninstall();
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Adds a file to the crash report. </summary>
		/// <param name="path">		  	Full pathname of the file. </param>
		/// <param name="name">		  	The name for the file. </param>
		/// <param name="description">	The description of the file. </param>
		/// <returns>	true if it succeeds, false if it fails. </returns>
		///-------------------------------------------------------------------------------------------------
		bool AddFileToCrashReport(LPCSTR path, LPCSTR name, LPCSTR description)
		{
			return (0 == crAddFile2(path, name, description, CR_AF_MISSING_FILE_OK | CR_AF_MAKE_FILE_COPY | CR_AF_ALLOW_DELETE));
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Adds a property to the crash report. </summary>
		/// <param name="name"> 	The name for the property. </param>
		/// <param name="value">	The property's value. </param>
		/// <returns>	true if it succeeds, false if it fails. </returns>
		///-------------------------------------------------------------------------------------------------
		bool AddPropertyToCrashReport(LPCSTR name, LPCSTR value)
		{
			return (0 == crAddProperty(name, value));
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Adds a registry key to the crash report. </summary>
		/// <param name="key">  	The registry key to add. </param>
		/// <returns>	true if it succeeds, false if it fails. </returns>
		///-------------------------------------------------------------------------------------------------
		bool AddRegistryKeyToCrashReport(LPCSTR key)
		{
			return (0 == crAddRegKey(key, "registry.xml", 0));
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Adds screenshot to the crash report. </summary>
		/// <returns>	true if it succeeds, false if it fails. </returns>
		///-------------------------------------------------------------------------------------------------
		bool AddScreenshotToCrashReport()
		{
			return (0 == crAddScreenshot2(CR_AS_MAIN_WINDOW | CR_AS_USE_JPEG_FORMAT | CR_AS_ALLOW_DELETE, 95));
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Forces a crash report to be generated for the supplied thread. </summary>
		/// <param name="thread">	Handle of the thread. </param>
		///-------------------------------------------------------------------------------------------------
		void ForceCrashReport(HANDLE thread)
		{
			EXCEPTION_POINTERS ptrs;
			EXCEPTION_RECORD exception;
			CONTEXT context;

			// if a thread handle is provided, gather exception data from it
			if(thread != INVALID_HANDLE_VALUE)
			{
				memset(&exception, 0, sizeof(exception));
				memset(&context, 0, sizeof(context));

				// get the threads current context
				context.ContextFlags = CONTEXT_FULL;
				GetThreadContext(thread, &context);

				exception.ExceptionCode = EXCEPTION_BREAKPOINT;
				exception.ExceptionFlags = EXCEPTION_NONCONTINUABLE;
				exception.ExceptionAddress = (void*)context.Eip;

				ptrs.ExceptionRecord = &exception;
				ptrs.ContextRecord = &context;
			}

			// set up the crashrpt exception info then generate a report
			CR_EXCEPTION_INFO ei;
			memset(&ei, 0, sizeof(CR_EXCEPTION_INFO));
			ei.cb = sizeof(ei);
			ei.exctype = CR_SEH_EXCEPTION;
			ei.code = EXCEPTION_BREAKPOINT;
			ei.pexcptrs = (thread != INVALID_HANDLE_VALUE ? &ptrs : nullptr);
			ei.bManual = FALSE;

			crGenerateErrorReport(&ei);
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	An exception filter that generates a crash report. </summary>
		/// <param name="ptrs">	The exception ptrs. </param>
		/// <returns>
		/// 	Returns EXCEPTION_CONTINUE_SEARCH if the exception was not handled, otherwise
		/// 	EXCEPTION_EXECUTE_HANDLER.
		/// </returns>
		///-------------------------------------------------------------------------------------------------
		int WINAPI SEHExceptionFilter(PEXCEPTION_POINTERS ptrs)
		{
			CR_EXCEPTION_INFO ei;
			memset(&ei, 0, sizeof(CR_EXCEPTION_INFO));
			ei.cb = sizeof(ei);
			ei.exctype = CR_SEH_EXCEPTION;
			ei.code = ptrs->ExceptionRecord->ExceptionCode;
			ei.pexcptrs = ptrs;
			ei.bManual = FALSE;

			int result = crGenerateErrorReport(&ei);
			if(result != 0)
			{
				return EXCEPTION_CONTINUE_SEARCH;
			}

			return EXCEPTION_EXECUTE_HANDLER;
		}
	};
};