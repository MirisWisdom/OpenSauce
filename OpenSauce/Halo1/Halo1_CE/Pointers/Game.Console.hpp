/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include "Pointers/1.10/HaloCE_110_Runtime.Game.inl"
#include "Pointers/1.10/HaloCE_110_Runtime_Manual.Game.inl"
#else
#include "Pointers/1.10/HaloCE_110_Dedi.Game.inl"
#include "Pointers/1.10/HaloCE_110_Dedi_Manual.Game.inl"
#endif

#include <blamlib/interface/terminal.hpp>
#include <blamlib/main/console.hpp>
#include <Memory/MemoryInterface.hpp>
#include <yelolib/memory/memory_interface_base.hpp>

namespace Yelo
{
	namespace Console
	{
		ENGINE_DPTR(s_terminal_globals, terminal_globals, DUO_PTR(K_TERMINAL_GLOBALS));
		ENGINE_PTR(s_console_globals, console_globals, DUO_PTR(K_CONSOLE_GLOBALS));

		FUNC_PTR(CONSOLE_UPDATE_HOOK, DUO_PTR(K_CONSOLE_UPDATE_HOOK));
	}
}
