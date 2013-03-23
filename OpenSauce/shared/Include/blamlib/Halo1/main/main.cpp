/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <blamlib/Halo1/main/main.hpp>

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
	};
};