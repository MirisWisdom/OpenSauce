/*
    Yelo: Open Sauce SDK
		Halo 1 (CE) Edition
    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
			Memory::WriteRelativeCall(&Console::Update, GET_FUNC_VPTR(CONSOLE_UPDATE_HOOK));
		}

		void Dispose()
		{
		}

		API_FUNC_NAKED void PLATFORM_API Update(uint32 access_flags)
		{
			static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(CONSOLE_PROCESS_COMMAND);

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