/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <blamlib/Halo1/scenario/scenario.hpp>

#include <blamlib/Halo1/cache/cache_files.hpp>
#include <blamlib/Halo1/cache/sound_cache.hpp>
#include <blamlib/Halo1/cache/texture_cache.hpp>
#include <blamlib/Halo1/game/game_globals_definitions.hpp>
#include <blamlib/Halo1/interface/ui_widget_group.hpp>
#include <blamlib/Halo1/scenario/scenario_definitions.hpp>
#include <blamlib/Halo1/scenario/scenario_structures.hpp>
#include <blamlib/Halo1/tag_files/tag_group_loading.hpp>
#include <blamlib/Halo1/tag_files/tag_groups.hpp>
#include <Yelolib/Halo1/open_sauce/project_yellow_scenario.hpp>

namespace Yelo
{
	namespace GameState
	{
		// defined by the implementing shell
		TagGroups::s_game_globals** GlobalGameGlobalsReference();

#define blam_global_game_globals		*(GameState::GlobalGameGlobalsReference())
	};

	namespace Scenario
	{
		cstring K_GAME_GLOBALS_TAG_NAME = "globals\\globals";

		// defined by the implementing shell
		datum_index* GlobalScenarioIndexReference();
		TagGroups::scenario** GlobalScenarioReference();
		int16* GlobalStructureBspIndexReference();
		TagGroups::structure_bsp** GlobalStructureBspReference();
		TagGroups::collision_bsp** GlobalBspReference();
		TagGroups::collision_bsp** GlobalCollisionBspReference();

#define blam_global_scenario_index		*(Scenario::GlobalScenarioIndexReference())
#define blam_global_scenario			*(Scenario::GlobalScenarioReference())
#define blam_global_structure_bsp_index	*(Scenario::GlobalStructureBspIndexReference())
#define blam_global_structure_bsp		*(Scenario::GlobalStructureBspReference())
#define blam_global_bsp3d				*(Scenario::GlobalBspReference())
#define blam_global_collision_bsp		*(Scenario::GlobalCollisionBspReference())
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


		datum_index PLATFORM_API scenario_tags_load_impl(cstring scenario_name); // temp setup
		// TODO: once this is in use, remove the scenario_yelo_load related code in CheApe
		bool PLATFORM_API scenario_load_impl(cstring scenario_name)
		{
			tag_load_error_string_clear();

			datum_index scenario_index = scenario_tags_load_impl(scenario_name);
			blam_global_scenario_index = scenario_index;
			
			if (scenario_index.IsNull())
			{
				tag_load_error_string_print();
				return false;
			}

			auto* scenario = TagGetForModify<TagGroups::scenario>(scenario_index);
			blam_global_scenario = scenario;

			if (scenario->structure_bsps.Count == 0)
			{
				YELO_WARN("scenario doesn't have a structure bsp");
				return false;
			}

			datum_index game_globals_index = tag_loaded<s_game_globals>(Scenario::K_GAME_GLOBALS_TAG_NAME);
			// NOTE: engine doesn't actually do this
			if (game_globals_index.IsNull())
			{
				YELO_WARN("failed to load game globals for scenario");
				return false;
			}

			auto* game_globals = TagGetForModify<s_game_globals>(game_globals_index);
			blam_global_game_globals = game_globals;

			// NOTE: engine doesn't print an error message
			if (!scenario_switch_structure_bsp(0))
			{
				YELO_WARN("failed to switch to the scenario's first bsp");
				return false;
			}

			for (scenario_netgame_equipment& equipment : scenario->netgame_equipment)
				equipment.runtime_object_index = datum_index::null;

			return true;
		}

		void PLATFORM_API scenario_tags_unload_impl(); // temp setup
		void PLATFORM_API scenario_unload_impl()
		{
			//YELO_ASSERT(!bink_playback_active());
			scenario_tags_unload_impl();
			blam_global_scenario_index = datum_index::null;
			blam_global_structure_bsp_index = NONE;
			Scenario::ScenarioGlobals()->current_structure_bsp_index = NONE;
			blam_global_scenario = nullptr;
			blam_global_structure_bsp = nullptr;
			blam_global_collision_bsp = nullptr;
			blam_global_bsp3d = nullptr;
			blam_global_game_globals = nullptr;
		}

		datum_index PLATFORM_API scenario_tags_load_impl(cstring scenario_name)
		{
			datum_index scenario_index = datum_index::null;

			texture_cache_open();
			sound_cache_open();
#if PLATFORM_USES_CACHE_FILES
			scenario_index = cache_file_tags_load(scenario_name);
#else
			scenario_index = tag_load<TagGroups::scenario>(scenario_name, 0);
			if (scenario_index.IsNull())
				return datum_index::null;

			datum_index game_globals_index = tag_load<TagGroups::s_game_globals>(Scenario::K_GAME_GLOBALS_TAG_NAME, 0);
			if (game_globals_index.IsNull())
			{
				YELO_WARN("couldn't load game globals (get new tags)");
				return datum_index::null;
			}

			blam_global_scenario = tag_get<TagGroups::scenario>(scenario_index);
			blam_global_game_globals = tag_get<TagGroups::s_game_globals>(game_globals_index);
			ui_load_tags_for_scenario(scenario_index);
#endif

			Scenario::ProjectYellowInitializeForNewMap();

			return scenario_index;
		}

		void PLATFORM_API scenario_tags_unload_impl()
		{
			texture_cache_close();
			sound_cache_close();

			Scenario::ProjectYellowDisposeFromOldMap();

#if PLATFORM_USES_CACHE_FILES
			cache_file_tags_unload();
#else
			if (!Scenario::ScenarioIndex().IsNull())
			{
				tag_groups_dispose_from_old_map();
			}
#endif
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