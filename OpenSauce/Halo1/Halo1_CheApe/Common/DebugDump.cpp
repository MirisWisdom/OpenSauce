/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Common/DebugDump.hpp"

#include <dbghelp.h>

#include <YeloLib/cseries/pc_crashreport_yelo.hpp>
#include <YeloLib/Halo1/shell/shell_windows_command_line.hpp>
#include <YeloLib/files/files.hpp>

#include "Common/YeloSettings.hpp"
#include "Memory/MemoryInterface.hpp"
#include "Engine/EngineFunctions.hpp"

namespace Yelo
{
	namespace Debug
	{
#define __EL_INCLUDE_ID			__EL_INCLUDE_OPEN_SAUCE
#define __EL_INCLUDE_FILE_ID	__EL_COMMON_DEBUG_DUMP
#include "Memory/_EngineLayout.inl"

		static char g_reports_path[255];

		static void ReportComplete(const char* report_directory)
		{
			// writing the report was successful so tell the user
			cstring message = "An exception has occurred.\nA crash report for this exception has been saved to your OpenSauce Reports directory (OpenSauce\\Reports\\CrashRpt).\nAttach the zip archive if you report this issue.";

			MessageBox(NULL, message, "Exception!", MB_OK);
		}

		static void SetupExceptionHandler();

#if PLATFORM_ID != PLATFORM_GUERILLA

		static int PLATFORM_API ExceptionFilter(uint32 code, PEXCEPTION_POINTERS ptrs)
		{
			int result;

#ifdef DISABLE_EXCEPTION_HANDLING
			result = EXCEPTION_CONTINUE_SEARCH;
#else
			Debug::SEHExceptionFilter(ptrs);

			// call stock filter so that debug.txt and such can be written to
			typedef int (PLATFORM_API *t_exception_filter)(uint32, PEXCEPTION_POINTERS);
			result = CAST_PTR(t_exception_filter, PLATFORM_VALUE(PTR_NULL, 0x437090, 0x4F81E0))(code, ptrs);
#endif
			return result;
		}

#else

		static int CWinApp_Run_Hook()
		{
			// install the exception handler
			_asm	push ecx;
			SetupExceptionHandler();
			_asm	pop ecx;

			// call the stock CWinApp::Run()
			CAST_PTR(void (*)(void), 0x84EBE1)();

			// preserve the return value
			int result;
			_asm	mov result, eax;

			// uninstall the exception handler
			Debug::UninstallExceptionHandler();

			return result;
		}

#endif

		static void SetupExceptionHandler()
		{
			bool do_full_dump = false;
#if PLATFORM_ID == PLATFORM_TOOL
			do_full_dump = Yelo::Settings::Get().active_profile.tool.do_full_crashdump;
#else
			do_full_dump = CMDLINE_GET_PARAM(full_dump).ParameterSet();
#endif

			s_crash_report_options crashreport_options;

			// save reports locally and do not show the crashrpt gui
			int flags;
			flags  = Enums::_crashreport_options_hide_gui;
			flags |= Enums::_crashreport_options_save_local;
			if(do_full_dump)
				flags |= Enums::_crashreport_options_full_dump;
			crashreport_options.m_flags = (Enums::crashreport_option_flags)flags;

			crashreport_options.m_report_complete_callback = &ReportComplete;

			crashreport_options.m_application_name = "OpenSauce HEK";
			crashreport_options.m_application_version = BOOST_STRINGIZE(K_OPENSAUCE_VERSION_BUILD_MAJ) "." BOOST_STRINGIZE(K_OPENSAUCE_VERSION_BUILD_MIN) "." BOOST_STRINGIZE(K_OPENSAUCE_VERSION_BUILD);

			crashreport_options.m_reports_directory = g_reports_path;
			crashreport_options.m_dependency_path = NULL;

			crashreport_options.m_report_server_url = NULL;
			crashreport_options.m_privacy_policy_url = NULL;

			if(Debug::InstallExceptionHandler(crashreport_options))
			{
#if PLATFORM_ID != PLATFORM_GUERILLA
				Memory::WriteRelativeCall(&ExceptionFilter, CAST_PTR(void*, PLATFORM_VALUE(PTR_NULL, 0x415456, 0x4A8278)), true);
#endif
				// add custom properties and files to the report
				Debug::AddPropertyToCrashReport("CommandLine", GetCommandLine());
				Debug::AddScreenshotToCrashReport();
				Debug::AddFileToCrashReport(Settings::K_EDITOR_FILENAME_XML, Settings::K_EDITOR_FILENAME_XML, "Editor settings file");
			}
		}

		void DumpInitialize()
		{
			// output path
			if(!FileIO::PathBuild(g_reports_path, false, 2, Settings::ReportsPath(), "CrashRpt"))
			{
				return;
			}
			else
			{
				// delete the reports directory
				FileIO::Delete_Directory(g_reports_path, true, true);
				if(0 == CreateDirectory(g_reports_path, NULL))
					return;
			}

#if PLATFORM_ID == PLATFORM_GUERILLA
			// override the stock CWinApp::Run vfunction
			*CAST_PTR(void**, 0x89AD54) = &CWinApp_Run_Hook;
#else
			SetupExceptionHandler();
#endif
		}

		void DumpDispose()
		{
#if PLATFORM_ID != PLATFORM_GUERILLA
			Debug::UninstallExceptionHandler();
#endif
		}
	};
};