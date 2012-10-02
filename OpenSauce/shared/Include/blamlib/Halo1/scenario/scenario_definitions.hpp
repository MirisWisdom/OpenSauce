/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <TagGroups/Halo1/TagGroupDefinitions.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_maximum_skies_per_scenario = 32,
			k_maximum_structure_bsps_per_scenario = 16,

			// How much OS-upgrades [k_maximum_structure_bsps_per_scenario]
			k_maximum_structure_bsps_per_scenario_upgrade = k_maximum_structure_bsps_per_scenario * 2,
		};
	};

	namespace TagGroups
	{
	};
};