/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/scenario/scenario_definitions.hpp>
#include <blamlib/Halo1/scenario/scenario_structures.hpp>
#include <blamlib/Halo1/structures/structure_bsp_definitions.hpp>

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

#if !PLATFORM_IS_DEDI
		// Sky should be rendered flag
		bool		RenderSky();
		// Index of the current render sky in the scenarios skies block
		int16		SkyIndex();
#endif
		void Initialize();
		void Dispose();
	};
};