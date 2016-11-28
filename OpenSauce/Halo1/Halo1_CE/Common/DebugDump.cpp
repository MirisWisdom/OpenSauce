/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Common/DebugDump.hpp"

#include <ErrorRep.h>
#pragma comment (lib, "Faultrep.lib")
#include <YeloLib/Halo1/shell/shell_windows_command_line.hpp>

#include "Memory/MemoryInterface.hpp"

#if PLATFORM_IS_USER
#include <blamlib/Halo1/main/console.hpp>
#include <blamlib/Halo1/saved_games/game_state_structures.hpp>
#include <blamlib/Halo1/game/game_globals.hpp>
#include <blamlib/Halo1/game/game_globals_structures.hpp>

#include <YeloLib/cseries/pc_crashreport.hpp>

#include "Common/FileIO.hpp"
#include "Settings/Settings.hpp"
#include "Interface/Keystone.hpp"
#include "Rasterizer/DX9/DX9.hpp"
#endif

namespace Yelo
{
	namespace Debug
	{
#define __EL_INCLUDE_ID			__EL_INCLUDE_OPEN_SAUCE
#define __EL_INCLUDE_FILE_ID	__EL_COMMON_DEBUG_DUMP
#include "Memory/_EngineLayout.inl"

#if PLATFORM_IS_USER
		static char g_reports_path[MAX_PATH];
		struct s_freeze_dump_globals
		{
			std::thread m_freeze_thread;
			DWORD m_main_thread_id;

			volatile bool m_thread_running;
			volatile bool m_end_thread;
			volatile bool m_dump_initiated;
			PAD8;

			struct
			{
				real time;
				real current;
				bool elapsed;
				PAD24;
			}m_delay;

			struct
			{
				real time;
				volatile real current;
				bool elapsed;
				PAD24;
			}m_wait;
		};
		static s_freeze_dump_globals g_freeze_dump_globals;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Reports to the user that a crash report has been created. </summary>
		///
		/// <param name="report_directory">	Pathname of the report directory. </param>
		static void ReportComplete(const char* report_directory)
		{
			// writing the report was successful so tell the user
			cstring message = "An exception has occurred.\nA crash report for this exception has been saved to your OpenSauce Reports directory (OpenSauce\\Reports\\CrashRpt).\nAttach the zip archive if you report this issue.";

#if PLATFORM_IS_USER
			// do not display a message box if this is a freeze dump as the dialog is unusable
			if(g_freeze_dump_globals.m_dump_initiated)
			{
				return;
			}
			MessageBox(nullptr, message, "Crash Report Saved", MB_OK);
#else
			blam::console_response_printf(message);
#endif
		}

		/// <summary>	Adds a copy of the game state to the crash report archive. </summary>
		static void AddGameStateDump()
		{
			if(FileIO::PathExists(g_reports_path))
			{
				char dump_file[MAX_PATH];
				if(FileIO::PathBuild(dump_file, false, 2, g_reports_path, "core.bin"))
				{
					FileIO::s_file_info dump;
					if(Enums::_file_io_open_error_none == FileIO::OpenFile(dump, dump_file, Enums::_file_io_open_access_type_write, Enums::_file_io_open_create_option_new))
					{
						// write the entire game state to file, which can be loaded again using core_load
						FileIO::WriteToFile(dump, GameState::GameStateGlobals()->base_address, Enums::k_game_state_allocation_size);
						FileIO::CloseFile(dump);

						Debug::AddFileToCrashReport(dump_file, "core.bin", "Gamestate dump");
					}
				}
			}
		}

		/// <summary>	Adds a backbuffer frame dump to the report. </summary>
		static void AddFrameDump()
		{
			if(FileIO::PathExists(g_reports_path))
			{
				char frame_file[MAX_PATH];
				if(FileIO::PathBuild(frame_file, false, 2, g_reports_path, "framedump.jpg"))
				{
					auto* device = DX9::Direct3DDevice();

					c_auto_release<IDirect3DSurface9> surface(nullptr);
					if(device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &surface) != S_OK)
					{
						return;
					}

					if(D3DXSaveSurfaceToFile(frame_file, D3DXIFF_JPG, surface, nullptr, nullptr) == S_OK)
					{
						Debug::AddFileToCrashReport(frame_file, "framedump.jpg", "Frame dump");
					}
				}
			}
		}

		struct s_cpu_feature
		{
			uint32 value;
			cstring string;
		};

		static const s_cpu_feature g_cpu_extensions[] =
		{
			{ PF_FLOATING_POINT_PRECISION_ERRATA,	"FLOATING_POINT_PRECISION_ERRATA" },
			{ PF_FLOATING_POINT_EMULATED,			"FLOATING_POINT_EMULATED" },
			{ PF_COMPARE_EXCHANGE_DOUBLE,			"COMPARE_EXCHANGE" },
			{ PF_MMX_INSTRUCTIONS_AVAILABLE,		"MMX" },
			{ PF_XMMI_INSTRUCTIONS_AVAILABLE,		"SSE" },
			{ PF_3DNOW_INSTRUCTIONS_AVAILABLE,		"3DNOW" },
			{ PF_RDTSC_INSTRUCTION_AVAILABLE,		"RDTSC" },
			{ PF_PAE_ENABLED,						"PAE" },
			{ PF_XMMI64_INSTRUCTIONS_AVAILABLE,		"SSE2" },
			{ PF_NX_ENABLED,						"DEP" },
			{ PF_SSE3_INSTRUCTIONS_AVAILABLE,		"SSE3" },
			{ PF_COMPARE_EXCHANGE128,				"COMPARE_EXCHANGE128" },
			{ PF_COMPARE64_EXCHANGE128,				"COMPARE64_EXCHANGE128" },
			{ PF_CHANNELS_ENABLED,					"CHANNELS" },
		};

		/// <summary>	Adds details about the users CPU to the crash report. </summary>
		static void AddCPUInfo()
		{
			// get system information
			SYSTEM_INFO system_info;
			GetSystemInfo(&system_info);

			char buffer[64];
			switch(system_info.wProcessorArchitecture)
			{
			case PROCESSOR_ARCHITECTURE_AMD64:
				Debug::AddPropertyToCrashReport("CPUArchitecture", "AMD64");
				break;
			case PROCESSOR_ARCHITECTURE_ARM:
				Debug::AddPropertyToCrashReport("CPUArchitecture", "ARM");
				break;
			case PROCESSOR_ARCHITECTURE_IA64:
				Debug::AddPropertyToCrashReport("CPUArchitecture", "IA64");
				break;
			case PROCESSOR_ARCHITECTURE_INTEL:
				Debug::AddPropertyToCrashReport("CPUArchitecture", "INTEL");
				break;
			case PROCESSOR_ARCHITECTURE_UNKNOWN:
				Debug::AddPropertyToCrashReport("CPUArchitecture", "UNKNOWN");
				break;
			}

			sprintf_s(buffer, "%u", system_info.dwNumberOfProcessors);
			Debug::AddPropertyToCrashReport("CPUNumberOfProcessors", buffer);

			// add processor features to the report
			for (const auto& cpu_ext : g_cpu_extensions)
			{
				BOOL supported = IsProcessorFeaturePresent(cpu_ext.value);
				sprintf_s(buffer, "CPUFeat_%s", cpu_ext.string);

				Debug::AddPropertyToCrashReport(buffer, (supported ? "true" : "false"));
			}
		}

		/// <summary>	Adds map information. </summary>
		static void AddMapInfo()
		{
			Debug::AddPropertyToCrashReport("MapName", GameState::GameGlobals()->options.map_name);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// 	Adds data to the crash report that should be done before the dump itself is taken.
		/// </summary>
		static void PreDump()
		{
			// Unlock the page for read and write
			VirtualProtect(GET_PTR(VIRTUALPROTECT_LOCK), 1, PAGE_READWRITE, GET_PTR2(VIRTUALPROTECT_OLD_PROTECT));
			*GET_PTR(VIRTUALPROTECT_LOCK) = nullptr;

			// Add a gamestate dump to the report
			AddGameStateDump();

			// Add a frame dump to the report
			AddFrameDump();

			// Add CPU information to the report
			AddCPUInfo();

			// Add the current map info
			AddMapInfo();
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// 	Exception filter used to override the stock handling and create a crash report.
		/// </summary>
		///
		/// <param name="ptrs">	The exception ptrs. </param>
		///
		/// <returns>	Whether the exception was handled by this filter or not. </returns>
		static int WINAPI ExceptionFilter(PEXCEPTION_POINTERS ptrs)
		{
			typedef void (PLATFORM_API* proc_simple_function)(void);

			PreDump();

			int result;
			bool disable_exception_handling = CMDLINE_GET_PARAM(disable_exception_handling).ParameterSet();

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
				result = Debug::SEHExceptionFilter(ptrs);
			}

			Keystone::ReleaseKeystone();

			// save gamma to registry
			CAST_PTR(proc_simple_function, GET_FUNC_VPTR(RASTERIZER_DX9_SAVE_GAMMA))();
			// present final frame
			CAST_PTR(proc_simple_function, GET_FUNC_VPTR(RASTERIZER_WINDOWS_PRESENT_FRAME))();

			// kill all sounds
			CAST_PTR(proc_simple_function, GET_FUNC_VPTR(SOUND_STOP_ALL))();

			ShowCursor(TRUE);

			// report the exception to windows
			ReportFault(ptrs, 0);

			return result;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// 	Runs on a separate thread and creates a crash report if the main thread becomes
		/// 	unresponsive.
		/// </summary>
		static void FreezeDumpCount()
		{
			g_freeze_dump_globals.m_thread_running = true;

			g_freeze_dump_globals.m_delay.current = 0.0f;
			g_freeze_dump_globals.m_wait.current = 0.0f;

			while(!g_freeze_dump_globals.m_end_thread)
			{
				Sleep(100);

				// count until the delay time has elapsed, then wait until the wait time has elapsed before dumping
				if(g_freeze_dump_globals.m_delay.elapsed && !g_freeze_dump_globals.m_wait.elapsed)
				{
					if(g_freeze_dump_globals.m_wait.current > g_freeze_dump_globals.m_wait.time)
					{
						g_freeze_dump_globals.m_wait.elapsed = true;
					}
					else
					{
						g_freeze_dump_globals.m_wait.current += 0.1f;
					}
				}
				else
				{
					if(g_freeze_dump_globals.m_delay.current > g_freeze_dump_globals.m_delay.time)
					{
						g_freeze_dump_globals.m_delay.elapsed = true;
					}
					else
					{
						g_freeze_dump_globals.m_delay.current += 0.1f;
					}
				}

				// once the wait time has elapsed force a crash report
				if(g_freeze_dump_globals.m_wait.elapsed)
				{
					g_freeze_dump_globals.m_end_thread = true;
					g_freeze_dump_globals.m_dump_initiated = true;

					PreDump();

					// get a handle to the main thread and suspend it
					HANDLE main_thread = OpenThread(THREAD_GET_CONTEXT | THREAD_SET_CONTEXT | THREAD_SUSPEND_RESUME, FALSE, g_freeze_dump_globals.m_main_thread_id);
					SuspendThread(main_thread);

					// force a crash dump, recording from the main thread
					Debug::ForceCrashReport(main_thread);

					// resume the main thread and release the handle on it
					ResumeThread(main_thread);
					CloseHandle(main_thread);
				}
			}

			g_freeze_dump_globals.m_thread_running = false;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Updates the crash handler to reset the freeze dump timer if needed. </summary>
		///
		/// <param name="delta">	The time that has passed since the last update. </param>
		void Update(real delta)
		{
			if(!g_freeze_dump_globals.m_thread_running)
			{
				return;
			}

			// reset the freeze dump counter
			g_freeze_dump_globals.m_wait.current = 0.0f;
		}

		/// <summary>	Initializes the crash handler. </summary>
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
				if(CreateDirectory(g_reports_path, nullptr) == FALSE)
				{
					return;
				}
			}

			// set up the freeze detection thread if requested
			if(CMDLINE_GET_PARAM(freeze_dump).ParameterSet())
			{
				memset(&g_freeze_dump_globals, 0, sizeof(g_freeze_dump_globals));

				g_freeze_dump_globals.m_wait.time = CMDLINE_GET_PARAM(freeze_dump).GetValue();
				if(g_freeze_dump_globals.m_wait.time > 0.0f)
				{
					// if a delay has not been set, set the delay to elapsed
					g_freeze_dump_globals.m_delay.time = CMDLINE_GET_PARAM(freeze_dump_delay).GetValue();
					if(!(g_freeze_dump_globals.m_delay.time > 0.0f))
					{
						g_freeze_dump_globals.m_delay.elapsed = true;
					}

					g_freeze_dump_globals.m_main_thread_id = GetCurrentThreadId();
					g_freeze_dump_globals.m_freeze_thread = std::thread(FreezeDumpCount);
				}
			}

			// install the CrashRpt exception reporter
			s_crash_report_options crashreport_options;
			Debug::InitDefaultOptions(crashreport_options);

			// save reports locally and do not show the crashrpt gui
			if(CMDLINE_GET_PARAM(full_dump).ParameterSet())
			{
				crashreport_options.m_flags = (Flags::crashreport_option_flags)(crashreport_options.m_flags | Flags::_crashreport_option_full_dump_flag);
			}
			crashreport_options.m_report_complete_callback = &ReportComplete;
			crashreport_options.m_application_name = "OpenSauce Halo CE";
			crashreport_options.m_reports_directory = g_reports_path;

			if(Debug::InstallExceptionHandler(crashreport_options))
			{
				// override the WinMain catch all exception filter
				Memory::WriteRelativeCall(ExceptionFilter, GET_FUNC_VPTR(WINMAIN_EXCEPTION_FILTER_CALL), true);

				// add custom properties to the report
				Debug::AddPropertyToCrashReport("CommandLine", GetCommandLine());

				// add the build date to the report
				Debug::AddPropertyToCrashReport("BuildDate", K_OPENSAUCE_BUILD_DATE_STR);

				// add settings files to the report
				char file_path[MAX_PATH];
				if(Settings::GetSettingsFilePath(Settings::K_USER_FILENAME_XML, file_path))
				{
					Debug::AddFileToCrashReport(file_path, Settings::K_USER_FILENAME_XML, "User settings file");
				}
			}
		}

		/// <summary>	Clean up the crash handler and uninstall crasrpt. </summary>
		void DumpDispose()
		{
			if(g_freeze_dump_globals.m_thread_running)
			{
				// stop the freeze dump thread
				g_freeze_dump_globals.m_end_thread = true;

				g_freeze_dump_globals.m_freeze_thread.join();
			}

			// uninstall the crashrpt exception handler
			Debug::UninstallExceptionHandler();
		}
#else
		static int WINAPI ExceptionFilter(PEXCEPTION_POINTERS ptrs)
		{
			typedef void (PLATFORM_API* proc_simple_function)(void);

			int result;
			bool disable_exception_handling = CMDLINE_GET_PARAM(disable_exception_handling).ParameterSet();

			if (
#ifdef API_DEBUG
				IsDebuggerPresent() ||
#endif
				disable_exception_handling
				)
			{
				result = EXCEPTION_CONTINUE_SEARCH;

				// kill all sounds
				CAST_PTR(proc_simple_function, GET_FUNC_VPTR(SOUND_STOP_ALL))();

				// report the exception to windows
				ReportFault(ptrs, 0);
			}
			else
			{
				result = CAST_PTR(int (WINAPI*)(PEXCEPTION_POINTERS), GET_FUNC_VPTR(WINMAIN_EXCEPTION_FILTER))(ptrs);
			}

			return result;
		}

		void DumpInitialize()
		{
			Memory::WriteRelativeCall(ExceptionFilter, GET_FUNC_VPTR(WINMAIN_EXCEPTION_FILTER_CALL), true);
		}

		void DumpDispose() { }

		void Update(real delta) { }
#endif
	};
};