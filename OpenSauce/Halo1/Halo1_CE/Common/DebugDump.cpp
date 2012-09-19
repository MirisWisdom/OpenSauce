/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Common/DebugDump.hpp"

#include <Common/FileIO.hpp>
#include <Common/YeloSettings.hpp>

#include "dbghelp.h"
#include "Memory/MemoryInterface.hpp"

namespace Yelo
{
	namespace DebugDump
	{
#define __EL_INCLUDE_ID			__EL_INCLUDE_OPEN_SAUCE
#define __EL_INCLUDE_FILE_ID	__EL_COMMON_DEBUG_DUMP
#include "Memory/_EngineLayout.inl"

		void CreateMiniDump()
		{
			HANDLE process = GetCurrentProcess();
			DWORD process_id = GetProcessId(process);

			char dump_name[MAX_PATH] = "";
			char dump_path[MAX_PATH] = "";

			tag_string date_time;
			Settings::GetTimeStampStringForFile(date_time);
			if(-1 == sprintf_s(dump_name, MAX_PATH, "haloce-%s.dmp", date_time))
				return;

			// the dump is saved to the opensauce reports path
			if(!FileIO::PathBuild(dump_path, false, 2, Settings::ReportsPath(), dump_name))
				return;

			// open the dump and use the files handle when calling MiniDumpWriteDump
			FileIO::s_file_info file_info;
			if(Enums::_file_io_open_error_none != FileIO::OpenFile(file_info, dump_path,
				Enums::_file_io_open_access_type_write,
				Enums::_file_io_open_create_option_new))
				return;

			BOOL success = MiniDumpWriteDump(process, process_id, file_info.file_handle, MiniDumpNormal, NULL, NULL, NULL);

			if(success)
			{
				// writing the dump was successful so tell the user
				char message_box_text[MAX_PATH + 180] = "";

				sprintf_s(message_box_text, sizeof(message_box_text),
					"A minidump for this exception has been saved to your OpenSauce Reports directory:\n%s\nAttach this file if you report this issue.",
						dump_path);

				MessageBox(NULL, message_box_text, "Minidump Saved", MB_OK);
			}

			FileIO::CloseFile(file_info);
		}

		API_FUNC_NAKED void PreDrWatson_Hook()
		{
			static const int32 JMP_ADDRESS = GET_FUNC_PTR(PRE_DR_WATSON_LAUNCH_RETN);

			_asm pushad;

			CreateMiniDump();

			_asm {
				popad
				mov		ecx, 11h
				xor		eax, eax
				jmp		JMP_ADDRESS
			};
		}

		void Initialize()
		{
			Memory::WriteRelativeJmp(PreDrWatson_Hook, GET_FUNC_VPTR(PRE_DR_WATSON_LAUNCH_HOOK), true);
		}

		void Dispose()
		{}
	};
};