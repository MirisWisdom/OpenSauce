/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Common/DebugDump.hpp"

#include "ErrorRep.h"
#pragma comment (lib, "Faultrep.lib")

#include <YeloLib/cseries/pc_crashreport.hpp>
#include <YeloLib/Halo1/shell/shell_windows_command_line.hpp>

#include "Common/FileIO.hpp"
#include "Common/YeloSettings.hpp"
#include "Memory/MemoryInterface.hpp"
#include "Game/EngineFunctions.hpp"
#include "Game/Gamestate.hpp"
#include "Interface/Keystone.hpp"

//#define DISABLE_EXCEPTION_HANDLING

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

#if PLATFORM_IS_USER
			MessageBox(NULL, message, "Crash Report Saved", MB_OK);
#else
			Engine::Console::TerminalPrint(message);
#endif
		}

		static void AddGameStateDump()
		{
			if(FileIO::PathExists(g_reports_path))
			{
				char dump_file[255];
				if(FileIO::PathBuild(dump_file, false, 2, g_reports_path, "core.bin"))
				{
					FileIO::s_file_info dump;
					if(Enums::_file_io_open_error_none == FileIO::OpenFile(dump, dump_file, Enums::_file_io_open_access_type_write, Enums::_file_io_open_create_option_new))
					{
						FileIO::WriteToFile(dump, GameState::GameStateGlobals()->base_address, Enums::k_game_state_allocation_size);
						FileIO::CloseFile(dump);

						Debug::AddFileToCrashReport(dump_file, "core.bin", "Gamestate dump");
					}
				}
			}
		}

		static int WINAPI ExceptionFilter(PEXCEPTION_POINTERS ptrs)
		{
			typedef void (*t_function)(void);

			// unlock the page for read and write
			VirtualProtect(GET_PTR(VIRTUALPROTECT_LOCK), 1, PAGE_READWRITE, GET_PTR2(VIRTUALPROTECT_OLD_PROTECT));
			*GET_PTR(VIRTUALPROTECT_LOCK) = NULL;

			// add a gamestate dump to the report
			AddGameStateDump();

			int result;
#ifdef DISABLE_EXCEPTION_HANDLING
			result = EXCEPTION_CONTINUE_SEARCH;
#else
			result = Debug::SEHExceptionFilter(ptrs);
#endif

#if PLATFORM_IS_USER
			Keystone::ReleaseKeystone();

			// save gamma to registry
			CAST_PTR(t_function, GET_FUNC_VPTR(RASTERIZER_DX9_SAVE_GAMMA))();
			// present final frame
			CAST_PTR(t_function, GET_FUNC_VPTR(RASTERIZER_WINDOWS_PRESENT_FRAME))();
#endif
			// kill all sounds
			CAST_PTR(t_function, GET_FUNC_VPTR(SOUND_STOP_ALL))();

			ShowCursor(TRUE);

			// report the exception to windows
			ReportFault(ptrs, 0);

			return result;
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

			// install the CrashRpt exception reporter
			s_crash_report_options crashreport_options;
			Debug::InitDefaultOptions(crashreport_options);

			// save reports locally and do not show the crashrpt gui
			if(CMDLINE_GET_PARAM(full_dump).ParameterSet())
				crashreport_options.m_flags = (Flags::crashreport_option_flags)(crashreport_options.m_flags | Flags::_crashreport_option_full_dump_bit);
			crashreport_options.m_report_complete_callback = &ReportComplete;
			crashreport_options.m_application_name = "OpenSauce Halo CE";
			crashreport_options.m_reports_directory = g_reports_path;

			if(Debug::InstallExceptionHandler(crashreport_options))
			{
				// override the WinMain catch all exception filter
				Memory::WriteRelativeCall(ExceptionFilter, GET_FUNC_VPTR(WINMAIN_EXCEPTION_FILTER_CALL), true);

#if PLATFORM_IS_USER
				// add a screenshot to the report
				Debug::AddScreenshotToCrashReport();
#endif

				// add custom properties to the report
				Debug::AddPropertyToCrashReport("CommandLine", GetCommandLine());
				
				// add settings files to the report
				char file_path[MAX_PATH];
				if(Settings::GetSettingsFilePath(Settings::K_USER_FILENAME_XML, file_path))
					Debug::AddFileToCrashReport(file_path, Settings::K_USER_FILENAME_XML, "User settings file");
				if(Settings::GetSettingsFilePath(Settings::K_SERVER_FILENAME_XML, file_path))
					Debug::AddFileToCrashReport(file_path, Settings::K_SERVER_FILENAME_XML, "Server settings file");
			}
		}

		void DumpDispose()
		{
			Debug::UninstallExceptionHandler();
		}
	};
};