/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Common/DebugDump.hpp"

#include <dbghelp.h>

#include <YeloLib/cseries/pc_minidump_yelo.hpp>
#include <YeloLib/Halo1/shell/shell_windows_command_line.hpp>

#include "Memory/MemoryInterface.hpp"

namespace Yelo
{
	namespace Debug
	{
#define __EL_INCLUDE_ID			__EL_INCLUDE_OPEN_SAUCE
#define __EL_INCLUDE_FILE_ID	__EL_COMMON_DEBUG_DUMP
#include "Memory/_EngineLayout.inl"

		API_FUNC_NAKED void PreDrWatson_Hook()
		{
			static const int32 JMP_ADDRESS = GET_FUNC_PTR(PRE_DR_WATSON_LAUNCH_RETN);
			static cstring k_process_name = PLATFORM_VALUE("haloce", "haloceded");

			_asm pushad;

			if(CMDLINE_GET_PARAM(full_dump).ParameterSet())
				Debug::OutputExceptionData(k_process_name, Settings::ReportsPath(), true);
			else
				Debug::OutputExceptionData(k_process_name, Settings::ReportsPath(), false);

			_asm {
				popad
				mov		ecx, 11h
				xor		eax, eax
				jmp		JMP_ADDRESS
			};
		}

		void DumpInitialize()
		{
			Memory::WriteRelativeJmp(PreDrWatson_Hook, GET_FUNC_VPTR(PRE_DR_WATSON_LAUNCH_HOOK), true);
		}

		void DumpDispose()
		{}
	};
};