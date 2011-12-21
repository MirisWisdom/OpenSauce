/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include "TagGroups/TagGroups.hpp"
#include <Blam/Halo1/project_yellow_shared_definitions.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		// Note: actually defined in TagGroups.cpp
		extern const project_yellow_globals* _global_yelo_globals;
		// Note: actually defined in TagGroups.cpp
		extern const project_yellow* _global_yelo;

		// Changes the current structure_bsp's lightmaps to the lightmap-set named 
		// [lightmap_set_name] in the [_global_yelo] definition's bsp-information block.
		// NOTE: Uses the current structure_bsp when finding the bsp-set to use.
		bool ScenarioStructureBspLightmapSetChange(cstring lightmap_set_name);
	};
};