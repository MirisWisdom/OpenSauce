/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/scenario/scenario.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		struct scenario;

		struct collision_bsp;
		struct structure_bsp;
	};
};

namespace Yelo
{
	namespace Scenario
	{
		s_scenario_globals*	ScenarioGlobals();

		// Scenario Tag data
		TagGroups::scenario*		Scenario();
		// Pointer to the current SBPS's bsp3d block
		TagGroups::collision_bsp*	Bsp3d();
		// Pointer to the current SBPS's collision bsp block
		TagGroups::collision_bsp*	CollisionBsp();
		// Pointer to the current SBSP's definition
		TagGroups::structure_bsp*	StructureBsp();

		// Index of the Scenario definition in the tag table
		datum_index	ScenarioIndex();
		// Index of the current SBSP in the Scenario's structure bsps block
		int16		StructureBspIndex();

		void Initialize();
		void Dispose();
	};
};