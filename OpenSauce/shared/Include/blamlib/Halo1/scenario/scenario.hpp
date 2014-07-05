/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace TagGroups
	{
		struct scenario;

		struct collision_bsp;
		struct structure_bsp;

		struct s_game_globals;
	};

	namespace Enums
	{
		enum {
			k_maximum_object_names_per_scenario = 512,
		};

		enum scenario_type : _enum
		{
			_scenario_type_campaign,
			_scenario_type_multiplayer,
			_scenario_type_main_menu,
		};
	};

	namespace GameState
	{
		// Globals tag data
		TagGroups::s_game_globals*	GlobalGameGlobals();
	};

	namespace Scenario
	{
		struct s_scenario_globals;


		s_scenario_globals*	ScenarioGlobals();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the currently loaded scenario. </summary>
		TagGroups::scenario*		Scenario();
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Get the currently loaded SBPS's bsp3d block. </summary>
		TagGroups::collision_bsp*	Bsp3d();
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Get the currently loaded SBPS's collision bsp block. </summary>
		TagGroups::collision_bsp*	CollisionBsp();
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Get the currently loaded SBPS's. </summary>
		TagGroups::structure_bsp*	StructureBsp();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Tag index of the currently loaded scenario. </summary>
		datum_index	ScenarioIndex();
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Scenario structure bsp reference index of the currently loaded SBSP. </summary>
		int16		StructureBspIndex();
	};

	namespace blam
	{
		TagGroups::s_game_globals* scenario_get_game_globals();
		TagGroups::scenario* global_scenario_get();
		TagGroups::scenario* global_scenario_try_and_get();
		TagGroups::structure_bsp* global_structure_bsp_get();
		TagGroups::collision_bsp* global_collision_bsp_get();

		// Switches the bsp being used to the bsp at [index] in the scenario structure bsps block
		bool PLATFORM_API scenario_switch_structure_bsp(int16 bsp_index);

		// Tests to see if [point] exists inside [trigger_volume_index] (index to a definition in the current scenario)
		bool PLATFORM_API scenario_trigger_volume_test_point(int32 trigger_volume_index, const real_point3d& point);

		// Tests to see if [object_index] is inside [trigger_volume_index] (index to a definition in the current scenario)
		bool PLATFORM_API scenario_trigger_volume_test_object(int32 trigger_volume_index, datum_index object_index);
	};
};