/*
    Yelo: Open Sauce SDK
		Halo 1 (CE) Edition
    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
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