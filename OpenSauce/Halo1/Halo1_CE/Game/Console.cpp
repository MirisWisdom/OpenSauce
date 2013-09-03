/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Game/Console.hpp"

#include "Memory/MemoryInterface.hpp"

namespace Yelo
{
	namespace Console
	{
#define __EL_INCLUDE_ID			__EL_INCLUDE_GAME
#define __EL_INCLUDE_FILE_ID	__EL_GAME_CONSOLE
#include "Memory/_EngineLayout.inl"

		s_terminal_globals* TerminalGlobals()				DPTR_IMP_GET(terminal_globals);
		s_console_globals* Globals()						PTR_IMP_GET2(console_globals);

		void Initialize()
		{
#if !PLATFORM_DISABLE_UNUSED_CODE
			Memory::WriteRelativeCall(&Console::Update, GET_FUNC_VPTR(CONSOLE_UPDATE_HOOK));
#endif
		}

		void Dispose()
		{
		}

		API_FUNC_NAKED void PLATFORM_API Update(uint32 access_flags)
		{
			static const uintptr_t TEMP_CALL_ADDR = GET_FUNC_PTR(CONSOLE_PROCESS_COMMAND);

			__asm {
				mov		eax, access_flags
				push	eax
				//mov		edi, edi // the command string buffer is given to us in edi
				call	TEMP_CALL_ADDR
				add		esp, 4
				retn
			}
		}
	};
};