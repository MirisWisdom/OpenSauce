/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
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
#if 0
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
#endif

			return false;
		}
	};
};