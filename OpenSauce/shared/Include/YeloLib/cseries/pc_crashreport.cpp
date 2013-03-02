/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/cseries/pc_crashreport.hpp>

// have to set the packing value to 8 for CrashRpt as that is what the was built as
#pragma pack(push)
#pragma pack(8)
#include <CrashRpt/CrashRpt.h>
#pragma pack(pop)
#pragma comment (lib, "CrashRpt1401.lib")

#include <YeloLib/files/files.hpp>

//#define TEST_CRASH_REPORT

namespace Yelo
{
	namespace Debug
	{
		static int CrashCallback(CR_CRASH_CALLBACK_INFO* pInfo)
		{
			if((pInfo->nStage == CR_CB_STAGE_FINISH) && pInfo->pUserParam)
			{
				t_report_callback callback = CAST_PTR(t_report_callback, pInfo->pUserParam);

				// send the error folder to the client callback
				callback(pInfo->pszErrorReportFolder);

				pInfo->bContinueExecution = false;
			}
			return CR_CB_NOTIFY_NEXT_STAGE;
		}

		void InitDefaultOptions(s_crash_report_options& options)
		{
			// save reports locally and do not show the crashrpt gui
			int flags = Flags::_crashreport_option_hide_gui_bit | Flags::_crashreport_option_save_local_bit;
			options.m_flags = (Flags::crashreport_option_flags)flags;

			options.m_report_complete_callback = NULL;

			options.m_application_name = NULL;
			options.m_application_version = BOOST_STRINGIZE(K_OPENSAUCE_VERSION_BUILD_MAJ) "." BOOST_STRINGIZE(K_OPENSAUCE_VERSION_BUILD_MIN) "." BOOST_STRINGIZE(K_OPENSAUCE_VERSION_BUILD);

			options.m_reports_directory = NULL;
			options.m_dependency_path = NULL;

			options.m_report_server_url = NULL;
			options.m_privacy_policy_url = NULL;
		}

		bool InstallExceptionHandler(s_crash_report_options& crashreport_options)
		{
			// install crash reporting
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
			send_to_server  = (crashreport_options.m_flags & Flags::_crashreport_option_save_local_bit) == 0;
			send_to_server &= (crashreport_options.m_report_server_url != NULL);

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
			info.pszEmailTo = NULL;
			info.pszEmailSubject = NULL;
			info.pszEmailText = NULL;
			info.pszSmtpProxy = NULL;
			info.pszSmtpLogin = NULL;
			info.pszSmtpPassword = NULL;

			// report flags
			info.dwFlags |= CR_INST_ALL_POSSIBLE_HANDLERS;
			
			if(!send_to_server)
			{
				info.dwFlags |= CR_INST_DONT_SEND_REPORT;
				info.dwFlags |= CR_INST_STORE_ZIP_ARCHIVES;

				if(Flags::_crashreport_option_hide_gui_bit == (crashreport_options.m_flags & Flags::_crashreport_option_hide_gui_bit))
					info.dwFlags |= CR_INST_NO_GUI;
			}

			// dump type
			if(Flags::_crashreport_option_full_dump_bit == (crashreport_options.m_flags & Flags::_crashreport_option_full_dump_bit))
				info.uMiniDumpType = MiniDumpWithFullMemory;
			else
				info.uMiniDumpType = MiniDumpNormal;

			int result = crInstall(&info);
			if(result!=0)
			{
				if(Flags::_crashreport_option_hide_gui_bit == (crashreport_options.m_flags & Flags::_crashreport_option_hide_gui_bit))
					return false;

				TCHAR error_msg[256];
				crGetLastErrorMsg(error_msg, 256);
				MessageBox(NULL, error_msg, "CrashRpt Install Error", MB_OK);
				return false;
			}

			// define the crash callback
			crSetCrashCallbackA(CAST_PTR(PFNCRASHCALLBACKA, &CrashCallback), crashreport_options.m_report_complete_callback);

		#ifdef TEST_CRASH_REPORT
			crEmulateCrash(0);
		#endif

			return true;
		}

		void UninstallExceptionHandler()
		{
			crUninstall();
		}

		bool AddFileToCrashReport(LPCSTR path, LPCSTR name, LPCSTR description)
		{
			return (0 == crAddFile2(path, name, description, CR_AF_MISSING_FILE_OK | CR_AF_MAKE_FILE_COPY | CR_AF_ALLOW_DELETE));
		}

		bool AddPropertyToCrashReport(LPCSTR name, LPCSTR value)
		{
			return (0 == crAddProperty(name, value));
		}

		bool AddRegistryKeyToCrashReport(LPCSTR key, LPCSTR value)
		{
			return (0 == crAddRegKey(key, "registry.xml", 0));
		}

		bool AddScreenshotToCrashReport()
		{
			return (0 == crAddScreenshot2(CR_AS_MAIN_WINDOW | CR_AS_USE_JPEG_FORMAT | CR_AS_ALLOW_DELETE, 95));
		}

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

			CR_EXCEPTION_INFO ei;
			memset(&ei, 0, sizeof(CR_EXCEPTION_INFO));
			ei.cb = sizeof(ei);
			ei.exctype = CR_SEH_EXCEPTION;
			ei.code = EXCEPTION_BREAKPOINT;
			ei.pexcptrs = (thread != INVALID_HANDLE_VALUE ? &ptrs : NULL);
			ei.bManual = FALSE;

			crGenerateErrorReport(&ei);
		}

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
				return EXCEPTION_CONTINUE_SEARCH;

			return EXCEPTION_EXECUTE_HANDLER;
		}
	};
};