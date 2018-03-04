/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/scenario/scenario_definitions.hpp>
#include <blamlib/scenario/scenario_structures.hpp>
#include <blamlib/structures/structure_bsp_definitions.hpp>

namespace Yelo
{
	namespace Scenario
	{
		s_scenario_globals* ScenarioGlobals();

		TagGroups::scenario* Scenario();
		TagGroups::collision_bsp* Bsp3d();
		TagGroups::collision_bsp* CollisionBsp();
		TagGroups::structure_bsp* StructureBsp();

		datum_index ScenarioIndex();
		int16 StructureBspIndex();

		void Initialize();
		void Dispose();
	};
};