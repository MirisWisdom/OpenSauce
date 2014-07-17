/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <blamlib/Halo1/scenario/scenario.hpp>

#include <blamlib/Halo1/cache/cache_files.hpp>
#include <blamlib/Halo1/scenario/scenario_definitions.hpp>
#include <blamlib/Halo1/tag_files/tag_groups.hpp>

namespace Yelo
{
	namespace Scenario
	{
		cstring K_GAME_GLOBALS_TAG_NAME = "globals\\globals";
	};

	namespace blam
	{
		using namespace TagGroups;

		s_game_globals* scenario_get_game_globals()
		{
			auto* global_game_globals = GameState::GlobalGameGlobals();
			YELO_ASSERT(global_game_globals);

			return global_game_globals;
		}
		scenario* global_scenario_get()
		{
			auto* global_scenario = Scenario::Scenario();
			YELO_ASSERT(global_scenario);

			return global_scenario;
		}
		scenario* global_scenario_try_and_get()
		{
			return Scenario::Scenario();
		}
		structure_bsp* global_structure_bsp_get()
		{
			auto* global_structure_bsp = Scenario::StructureBsp();
			YELO_ASSERT(global_structure_bsp);

			return global_structure_bsp;
		}
		collision_bsp* global_collision_bsp_get()
		{
			auto* global_collision_bsp = Scenario::CollisionBsp();
			YELO_ASSERT(global_collision_bsp);

			return global_collision_bsp;
		}


		void scenario_structure_bsp_unload(scenario_structure_bsp_reference* reference)
		{
#if PLATFORM_USES_CACHE_FILES
			cache_file_structure_bsp_unload(reference);
#else
			tag_unload(reference->structure_bsp.tag_index);
#endif
			reference->structure_bsp.tag_index = datum_index::null;
		}

		bool scenario_structure_bsp_load(scenario_structure_bsp_reference* reference)
		{
			bool loaded = false;

#if PLATFORM_USES_CACHE_FILES
			loaded = cache_file_structure_bsp_load(reference);
#else
			loaded = tag_reference_resolve(&reference->structure_bsp);
#endif
			return loaded;
		}
	};
};