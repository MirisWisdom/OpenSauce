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
#include "Common/Precompile.hpp"
#include "TagGroups/project_yellow_definitions.hpp"

#include <TagGroups/Halo1/scenario_definitions.hpp>
#include <TagGroups/Halo1/structure_bsp_definitions.hpp>

#include "Game/GameState.hpp"
#include "Rasterizer/Rasterizer.hpp"

namespace Yelo
{
	namespace TagGroups
	{
		static bool StructureBspLightmapSetChange(structure_bsp* sbsp,
			const TAG_TBLOCK(& lightmap_sets, s_bsp_information_lightmap_set),
			cstring lightmap_set_name)
		{
			const bool use_default = !strcmp(lightmap_set_name, "default");

			for(int32 x = 0; x < lightmap_sets.Count; x++)
			{
				if( !strcmp(lightmap_sets[x].name, lightmap_set_name) || (use_default && lightmap_sets[x].flags.is_default_bit) )
				{
					const s_bsp_information_lightmap_set& lm_set = lightmap_sets[x];

					if(!lm_set.definition.tag_index.IsNull())
						sbsp->lightmap_bitmaps.tag_index = lm_set.definition;		// change the current lightmap

					return true;
				}
			}

			return false;
		}

		bool ScenarioStructureBspLightmapSetChange(cstring lightmap_set_name)
		{
			if(_global_yelo->bsp_information.Count == 1)
			{
				structure_bsp* current_sbsp = GameState::StructureBsp();
				const TAG_TBLOCK(& bsp_sets, s_bsp_information_bsp_set) = _global_yelo->bsp_information[0].bsp_sets;

				for(int32 x = 0; x < bsp_sets.Count; x++)
				{
					datum_index bsp_tag_index = bsp_sets[x].structure_bsp;

					if(Instances()[bsp_tag_index.index].definition == current_sbsp)
						return StructureBspLightmapSetChange(current_sbsp, bsp_sets[x].lightmap_sets, 
							lightmap_set_name);
				}
			}

			return false;
		}
	};
};