/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#if !PLATFORM_IS_EDITOR
#include <blamlib/Halo1/main/main.hpp>

#include <blamlib/Halo1/main/main_structures.hpp>

namespace Yelo
{
	namespace GameState
	{
		void s_main_globals::QuitToMainMenu()
		{
			map.switch_to_structure_bsp = NONE;
			map.save_map = false;
			map.main_menu_scenario_load = true;
		}

		bool IsLocal()	{ return MainGlobals()->game_connection == Enums::_game_connection_local; }
		bool IsServer() { return MainGlobals()->game_connection == Enums::_game_connection_network_server; }
		bool IsClient() { return MainGlobals()->game_connection == Enums::_game_connection_network_client; }
	};

	namespace blam
	{
		Enums::game_connection game_connection()	{ return GameState::MainGlobals()->game_connection; }
	};
};

#endif