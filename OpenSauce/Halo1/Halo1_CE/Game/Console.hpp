/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/interface/terminal.hpp>
#include <blamlib/Halo1/main/console.hpp>
#include <blamlib/Halo1/memory/data.hpp>

namespace Yelo
{
	namespace Console
	{
		s_terminal_globals* TerminalGlobals();
		s_console_globals* Globals();


		void Initialize();
		void Dispose();
		void PLATFORM_API Update(long_flags access_flags);
	};
};