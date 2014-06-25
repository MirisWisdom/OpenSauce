namespace ProjectYellow
{
	void project_yellow_preprocess_bsp_existance(const TagGroups::s_project_yellow_scenario_information& scenario_info)
	{
		const auto& scenario = *Scenario::Scenario();

		// All BSP's referenced in the project yellow must be referenced in the scenario
		for(auto bsp : scenario_info.bsps)
		{
			// Bsp entries must have a bsp defined
			YELO_ERROR_IF(bsp.bsp.tag_index.IsNull(),
				_error_message_priority_critical, "error: a project_yellow bsp entry has no bsp defined");

			bool was_found = false;
			for(auto scenario_bsp : scenario.structure_bsps)
			{
				if(was_found = (scenario_bsp.structure_bsp.tag_index == bsp.bsp.tag_index))
				{
					break;
				}
			}

			YELO_ERROR_IF(!was_found,
				_error_message_priority_critical, "error: the project_yellow tag references a bsp that is not in the scenario\ntag: %s", blam::tag_get_name(bsp.bsp.tag_index));
		}
	}

	void project_yellow_preprocess_bsp_lightmap_sets(const TagGroups::s_project_yellow_scenario_information& scenario_info)
	{
		for(auto bsp : scenario_info.bsps)
		{
			const auto& bsp_tag = *blam::tag_get<TagGroups::structure_bsp>(bsp.bsp.tag_index);
			const int bsp_lightmap_count = bsp_tag.lightmaps.Count - 1; // -1 as there is a lightmap entry for transparents

			// Cannot use lightmap sets if the bsp does not have a lightmap itself
			YELO_ERROR_IF(bsp_tag.lightmap_bitmaps.tag_index.IsNull() && (bsp.lightmap_sets.Count > 0), 
				_error_message_priority_critical, "error: attempting to use bsp lightmap sets when the bsp tag does not define a lightmap");

			for(auto& lightmap_set : bsp.lightmap_sets)
			{
				auto check_bitmap_count_func =
					[bsp_lightmap_count](datum_index bitmap_index)
					{
						const auto& bitmap = *blam::tag_get<TagGroups::s_bitmap_group>(bitmap_index);

						YELO_ERROR_IF(bitmap.bitmaps.Count != bsp_lightmap_count,
							_error_message_priority_critical, "error: a lightmap bitmap referenced in the project_yellow tag does not have the same bitmap count as the bsp has lightmap meshes");
					};

				// Every lightmap set must have a standard lightmap
				YELO_ERROR_IF(lightmap_set.standard_lightmap.tag_index.IsNull(),
					_error_message_priority_critical, "error: a lightmap set in the project_yellow tag has no standard lightmap");
				
				// All lightmap bitmaps must have the same bitmap count as the bsp
				check_bitmap_count_func(lightmap_set.standard_lightmap.tag_index);
				
				// All 3 directional lightmaps must be present if any one is used
				bool has_dlm1 = !lightmap_set.directional_lightmap_1.tag_index.IsNull();
				bool has_dlm2 = !lightmap_set.directional_lightmap_2.tag_index.IsNull();
				bool has_dlm3 = !lightmap_set.directional_lightmap_3.tag_index.IsNull();

				if(has_dlm1 || has_dlm2 || has_dlm3)
				{
					YELO_ERROR_IF(!(has_dlm1 && has_dlm2 && has_dlm3),
						_error_message_priority_critical, "error: a lightmap set in the project_yellow tag has a missing directional lightmap");
					
					// All lightmap bitmaps must have the same bitmap count as the bsp
					check_bitmap_count_func(lightmap_set.directional_lightmap_1.tag_index);
					check_bitmap_count_func(lightmap_set.directional_lightmap_2.tag_index);
					check_bitmap_count_func(lightmap_set.directional_lightmap_3.tag_index);
				}
			}
		}
	}

	void project_yellow_preprocess_bsp_sky_sets(const TagGroups::s_project_yellow_scenario_information& scenario_info)
	{
		const auto& scenario = *Scenario::Scenario();

		const int scenario_sky_count = scenario.skies.Count;

		for(auto bsp : scenario_info.bsps)
		{
			YELO_ERROR_IF((bsp.sky_sets.Count > 0) && (scenario_sky_count == 0),
				_error_message_priority_critical, "error: a scenario info bsp has sky sets, when the scenario has no skies");

			for(auto sky_set : bsp.sky_sets)
			{
				YELO_ERROR_IF(sky_set.skies.Count == 0,
					_error_message_priority_critical, "error: a scenario info bsp sky set has no skies defined");

				for(auto sky : sky_set.skies)
				{
					// The scenario sky index must be valid
					YELO_ERROR_IF(sky.sky_index >= scenario_sky_count,
					_error_message_priority_critical, "error: a scenario info bsp sky set has a sky with an invalid scenario sky index");
				}
			}
		}
	}

	void project_yellow_preprocess(datum_index tag_index)
	{
		const auto& project_yellow = *blam::tag_get<TagGroups::project_yellow>(tag_index);

		if(project_yellow.scenario.Count != 1)
		{
			return;
		}

		const auto& scenario_info = project_yellow.scenario[0];

		project_yellow_preprocess_bsp_existance(scenario_info);
		project_yellow_preprocess_bsp_lightmap_sets(scenario_info);
		project_yellow_preprocess_bsp_sky_sets(scenario_info);
	}
}