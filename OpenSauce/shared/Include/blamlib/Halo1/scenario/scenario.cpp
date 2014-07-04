/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <blamlib/Halo1/scenario/scenario.hpp>

namespace Yelo
{
	namespace blam
	{
		TagGroups::s_game_globals* scenario_get_game_globals()
		{
			auto* global_game_globals = GameState::GlobalGameGlobals();
			YELO_ASSERT(global_game_globals);

			return global_game_globals;
		}
		TagGroups::scenario* global_scenario_get()
		{
			auto* global_scenario = Scenario::Scenario();
			YELO_ASSERT(global_scenario);

			return global_scenario;
		}
		TagGroups::scenario* global_scenario_try_and_get()
		{
			return Scenario::Scenario();
		}
		TagGroups::structure_bsp* global_structure_bsp_get()
		{
			auto* global_structure_bsp = Scenario::StructureBsp();
			YELO_ASSERT(global_structure_bsp);

			return global_structure_bsp;
		}
		TagGroups::collision_bsp* global_collision_bsp_get()
		{
			auto* global_collision_bsp = Scenario::CollisionBsp();
			YELO_ASSERT(global_collision_bsp);

			return global_collision_bsp;
		}
	};
};