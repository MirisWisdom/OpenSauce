/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

#include <blamlib/Halo1/tag_files/tag_groups.hpp>
#include <blamlib/Halo1/scenario/scenario_definitions.hpp>

namespace Yelo { namespace Tool { namespace BuildCacheFileEx { namespace TagPreprocess {
	
namespace Scenario
{
	void scenario_preprocess_bsp_existance(const TagGroups::scenario& scenario)
	{
		// All BSP's referenced in the project yellow must be referenced in the scenario
		for(const auto& modifier : scenario.bsp_modifiers)
		{
			// Bsp entries must have a bsp defined
			YELO_ASSERT_DISPLAY(modifier.bsp_index > NONE,
				"A bsp modifier block has an invalid bsp index");
		}
	}

	void scenario_preprocess_bsp_lightmap_sets(const TagGroups::scenario& scenario)
	{
		for(const auto& modifier : scenario.bsp_modifiers)
		{
			const auto& bsp_tag_reference = scenario.structure_bsps[modifier.bsp_index].structure_bsp;
			const auto& bsp_tag = *blam::tag_get<TagGroups::structure_bsp>(bsp_tag_reference.tag_index);
			const int bsp_lightmap_count = bsp_tag.lightmaps.Count - 1; // -1 as there is a lightmap entry for transparents

			// Cannot use lightmap sets if the bsp does not have a lightmap itself
			YELO_ASSERT_DISPLAY(!(bsp_tag.lightmap_bitmaps.tag_index.IsNull() && (modifier.lightmap_sets.Count > 0)),
				"error: attempting to use bsp lightmap sets when the bsp tag does not define a lightmap");

			for(const auto& lightmap_set : modifier.lightmap_sets)
			{
				auto check_bitmap_count_func =
					[bsp_lightmap_count](datum_index bitmap_index)
					{
						const auto& bitmap = *blam::tag_get<TagGroups::s_bitmap_group>(bitmap_index);

						YELO_ASSERT_DISPLAY(bitmap.bitmaps.Count == bsp_lightmap_count,
							"error: a lightmap bitmap referenced in a bsp modifier does not have the same bitmap count as the bsp has lightmap meshes");
					};

				// Every lightmap set must have a name
				YELO_ASSERT_DISPLAY(!is_null_or_empty(lightmap_set.name),
					"error: a lightmap set in the scenario tag has no name");

				// Every lightmap set must have a standard lightmap
				YELO_ASSERT_DISPLAY(!lightmap_set.standard_lightmap.tag_index.IsNull(),
					"error: a lightmap set in the scenario tag has no standard lightmap");
				
				// All lightmap bitmaps must have the same bitmap count as the bsp
				check_bitmap_count_func(lightmap_set.standard_lightmap.tag_index);
				
				// All 3 directional lightmaps must be present if any one is used
				bool has_dlm1 = !lightmap_set.directional_lightmap_1.tag_index.IsNull();
				bool has_dlm2 = !lightmap_set.directional_lightmap_2.tag_index.IsNull();
				bool has_dlm3 = !lightmap_set.directional_lightmap_3.tag_index.IsNull();

				if(has_dlm1 || has_dlm2 || has_dlm3)
				{
					YELO_ASSERT_DISPLAY(has_dlm1 && has_dlm2 && has_dlm3,
						"error: a lightmap set in the scenario tag has a missing directional lightmap");
					
					// All lightmap bitmaps must have the same bitmap count as the bsp
					check_bitmap_count_func(lightmap_set.directional_lightmap_1.tag_index);
					check_bitmap_count_func(lightmap_set.directional_lightmap_2.tag_index);
					check_bitmap_count_func(lightmap_set.directional_lightmap_3.tag_index);
				}
			}
		}
	}

	void scenario_preprocess_bsp_sky_sets(const TagGroups::scenario& scenario)
	{
		const int scenario_sky_count = scenario.skies.Count;

		for(const auto& modifier : scenario.bsp_modifiers)
		{
			YELO_ASSERT_DISPLAY(!((modifier.sky_sets.Count > 0) && (scenario_sky_count == 0)),
				"error: a scenario bsp modifier has sky sets, when the scenario has no skies");

			for(const auto& sky_set : modifier.sky_sets)
			{
				YELO_ASSERT_DISPLAY(!is_null_or_empty(sky_set.name),
					"error: a bsp modifier sky set in the scenario tag has no name");

				YELO_ASSERT_DISPLAY(sky_set.skies.Count != 0,
					"error: a bsp modifier sky set has no skies defined");

				for(const auto& sky : sky_set.skies)
				{
					// The scenario sky index must be valid
					YELO_ASSERT_DISPLAY(sky.sky_index > NONE,
						"error: a bsp modifier sky set has a sky with an invalid scenario sky index");

					YELO_ASSERT_DISPLAY(!sky.sky.tag_index.IsNull(),
						"error: a bsp modifier sky set has a sky no sky tag set");
				}
			}
		}
	}

	void scenario_preprocess(datum_index tag_index)
	{
		const auto& scenario = *blam::tag_get<TagGroups::scenario>(tag_index);

		scenario_preprocess_bsp_existance(scenario);
		scenario_preprocess_bsp_lightmap_sets(scenario);
		scenario_preprocess_bsp_sky_sets(scenario);
	}
};

};};};};