/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Common/DebugDump.hpp"

#include <dbghelp.h>

#include <YeloLib/cseries/pc_crashreport.hpp>
#include <YeloLib/Halo1/shell/shell_windows_command_line.hpp>
#include <YeloLib/files/files.hpp>

#include "Settings/Settings.hpp"
#include "Settings/SettingsTool.hpp"
#include "Memory/MemoryInterface.hpp"
#include "Engine/EngineFunctions.hpp"

namespace Yelo
{
	namespace Debug
	{
#define __EL_INCLUDE_ID			__EL_INCLUDE_OPEN_SAUCE
#define __EL_INCLUDE_FILE_ID	__EL_COMMON_DEBUG_DUMP
#include "Memory/_EngineLayout.inl"

		static char g_reports_path[_MAX_PATH];

		static void ReportComplete(const char* report_directory)
		{
			// writing the report was successful so tell the user
			cstring message = "An exception has occurred.\n"
				"A crash report for this exception has been saved to your OpenSauce Reports directory (OpenSauce\\Reports\\CrashRpt).\n"
				"Attach the zip archive if you report this issue.";

			MessageBox(nullptr, message, "Exception!", MB_OK);
		}

		static void SetupExceptionHandler();

#if PLATFORM_TYPE != PLATFORM_GUERILLA

		static int PLATFORM_API ExceptionFilter(uint32 code, PEXCEPTION_POINTERS ptrs)
		{
			int result;
			bool disable_exception_handling;

#if PLATFORM_TYPE == PLATFORM_TOOL
			disable_exception_handling = Settings::c_settings_tool::Instance()->m_disable_exception_handling;
#else
			disable_exception_handling = CMDLINE_GET_PARAM(disable_exception_handling).ParameterSet();
#endif

			if (
#ifdef API_DEBUG
				IsDebuggerPresent() ||
#endif
				disable_exception_handling
				)
			{
				result = EXCEPTION_CONTINUE_SEARCH;
			}
			else
			{
				Debug::SEHExceptionFilter(ptrs);

				// call stock filter so that debug.txt and such can be written to
				typedef int (PLATFORM_API *exception_filter_t)(uint32, PEXCEPTION_POINTERS);
				result = CAST_PTR(exception_filter_t, GET_FUNC_VPTR(GENERIC_EXCEPTION_FILTER))(code, ptrs);
			}

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
			// not actually __stdcall, but __thiscall (first param is 'this'!)
			// stdcall gives us the desired stack behavior, without having to explicitly pass a 'this' pointer (ecx)
			typedef int (__stdcall* winapp_run_t)(void);
			// preserve the return value
			int result = CAST_PTR(winapp_run_t, GET_FUNC_VPTR(CWINAPP_RUN))();

			// uninstall the exception handler
			Debug::UninstallExceptionHandler();

			return result;
		}

#endif

		static void SetupExceptionHandler()
		{
			bool do_full_dump = false;
#if PLATFORM_TYPE == PLATFORM_TOOL
			do_full_dump = Settings::c_settings_tool::Instance()->m_do_full_crashdump;
#else
			do_full_dump = CMDLINE_GET_PARAM(full_dump).ParameterSet();
#endif

			s_crash_report_options crashreport_options;
			Debug::InitDefaultOptions(crashreport_options);

			// save reports locally and do not show the crashrpt gui
			if (CMDLINE_GET_PARAM(full_dump).ParameterSet())
				crashreport_options.m_flags = (Flags::crashreport_option_flags)(crashreport_options.m_flags | Flags::_crashreport_option_full_dump_flag);
			crashreport_options.m_report_complete_callback = &ReportComplete;
			crashreport_options.m_application_name = "OpenSauce HEK";
			crashreport_options.m_reports_directory = g_reports_path;

			if (Debug::InstallExceptionHandler(crashreport_options))
			{
#if PLATFORM_TYPE != PLATFORM_GUERILLA
				Memory::WriteRelativeCall(&ExceptionFilter, GET_FUNC_VPTR(GENERIC_EXCEPTION_FILTER_CALL), true);
#endif
				// add custom properties and files to the report
				Debug::AddPropertyToCrashReport("CommandLine", GetCommandLine());
				Debug::AddScreenshotToCrashReport();
				Debug::AddFileToCrashReport(Settings::K_EDITOR_FILENAME_XML, Settings::K_EDITOR_FILENAME_XML, "Editor settings file");
			}
		}

#ifdef API_DEBUG
	#pragma check_stack(off) // We NEVER want stack checking enabled for this function
	#pragma runtime_checks( "", off )
		static void PLATFORM_API BlamErrorHook(
			void* error_return_address, // the return address after blam::error() finishes

			Enums::error_message_priority priority, cstring format, /*...*/
			// these are only valid when error() was called in display_assert()
			cstring assert_kind, cstring file, int32 line, cstring reason)
		{
			if (priority != Enums::_error_message_priority_warning && format != nullptr && strstr(format, "EXCEPTION") != nullptr)
				DebugBreak();
		}
	#pragma runtime_checks( "", restore )
	#pragma check_stack

		static API_FUNC_NAKED void BlamErrorHook_Trampoline(Enums::error_message_priority priority, cstring format, ...)
		{
			API_FUNC_NAKED_START_()
				add		esp, PLATFORM_VALUE(404h, 408h, 408h)
				// NOTE: we don't push error()'s arguments to the stack for BlamErrorHook
				// so _ReturnAddress() will actually be its 'first' parameter
				call	BlamErrorHook
			API_FUNC_NAKED_END_()
		}
	
		static char g_error_buffer[1024] = "";
		void PLATFORM_API error_debug_output(const Enums::error_message_priority priority, cstring format, ...)
		{
			va_list params;
			va_start(params,format);

			vsprintf_s(g_error_buffer, format, params);
			OutputDebugString(g_error_buffer);
			OutputDebugString("\r\n");

			blam::error(priority, format, *params);
			va_end(params);
		}
#endif
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
				FileIO::DirectoryDelete(g_reports_path, true, true);
				if(0 == CreateDirectory(g_reports_path, nullptr))
					return;
			}

#if PLATFORM_TYPE == PLATFORM_GUERILLA
			// override the stock CWinApp::Run vfunction
			GET_PTR(CWINAPP_RUN_HOOK) = &CWinApp_Run_Hook;
#else
			SetupExceptionHandler();
#endif

#ifdef API_DEBUG
			if (IsDebuggerPresent())
			{
				// put a hook at the end of error(). hook will not handle priority==0 cases
				byte* error_proc_eof = CAST_PTR(byte*, blam::error) + PLATFORM_VALUE(0x290, 0x2A8, 0x29A);
				Memory::WriteRelativeJmp(BlamErrorHook_Trampoline, error_proc_eof, true);

				// Hook the error jmp to print all errors to the output console
#if PLATFORM_TYPE != PLATFORM_TOOL
				Memory::WriteRelativeJmp(&error_debug_output, CAST_PTR(void*, PLATFORM_VALUE(0x401FE6, PTR_NULL, 0x40716C)), true);
#endif
			}
#endif
		}

		void DumpDispose()
		{
#if PLATFORM_TYPE != PLATFORM_GUERILLA
			Debug::UninstallExceptionHandler();
#endif
		}
	};
};