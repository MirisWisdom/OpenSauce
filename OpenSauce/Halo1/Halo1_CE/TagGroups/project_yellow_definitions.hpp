/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <YeloLib/Halo1/open_sauce/project_yellow_global_definitions.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_global_cv_definitions.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_scenario_definitions.hpp>

#include "TagGroups/TagGroups.hpp"

namespace Yelo
{
	namespace TagGroups
	{
		// Note: actually defined in TagGroups.cpp
		extern const project_yellow_globals* _global_yelo_globals;
		// Note: actually defined in TagGroups.cpp
		extern const project_yellow* _global_yelo;

		const project_yellow_globals_cv* CvGlobals();
	};
};