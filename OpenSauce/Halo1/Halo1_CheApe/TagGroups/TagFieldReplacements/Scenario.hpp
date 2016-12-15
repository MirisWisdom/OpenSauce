/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

#include <blamlib/Halo1/tag_files/tag_groups_structures_macros.hpp>
#include <YeloLib/Halo1/tag_files/tag_fieldset_replacement_builder.hpp>

#include <blamlib/Halo1/scenario/scenario_definitions.hpp>
#include <blamlib/Halo1/bitmaps/bitmap_group.hpp>
#include <blamlib/Halo1/scenario/sky_definitions.hpp>

#include "TagGroups/TagFieldReplacements/Shared.hpp"

namespace Yelo
{
	namespace TagGroups { namespace TagFieldReplacements { namespace Scenario
	{
		TAG_GROUP_BLOCK_FIELDS_DEFINE(scenario_bsp_lightmap_set) =
		{
			TAG_FIELD_ENTRY(_field_string, "name" TAG_FIELD_MARKUP_IS_BLOCK_NAME),
			TAG_FIELD_ENTRY_PAD(sizeof(int32)),
			TAG_FIELD_ENTRY(_field_explanation, "standard lightmap", "The standard lightmap is the same type of lightmaps as used by stock Halo.\r\nIt is used if directional lightmaps are disabled." ),
			TAG_FIELD_ENTRY(_field_tag_reference, "std. lightmap", &Shared::TAG_GROUP_REFERENCE_GET(bitmap) ),
			TAG_FIELD_ENTRY(_field_explanation, "directional lightmap", "Directional lightmaps stores light accumulated in 3 directions in 3 lightmaps.\r\nThey are used to create normal mapped static lighting." ),
			TAG_FIELD_ENTRY(_field_tag_reference, "dir. lightmap 1", &Shared::TAG_GROUP_REFERENCE_GET(bitmap) ),
			TAG_FIELD_ENTRY(_field_tag_reference, "dir. lightmap 2", &Shared::TAG_GROUP_REFERENCE_GET(bitmap) ),
			TAG_FIELD_ENTRY(_field_tag_reference, "dir. lightmap 3", &Shared::TAG_GROUP_REFERENCE_GET(bitmap) ),
			TAG_FIELD_ENTRY_PAD(sizeof(tag_block) * 2),
			TAG_FIELD_ENTRY_END()
		};
		TAG_GROUP_BLOCK_DEFINE(scenario_bsp_lightmap_set, 0, 64,
			sizeof(s_scenario_bsp_lightmap_set)
		);


		TAG_GROUP_BLOCK_FIELDS_DEFINE(scenario_bsp_sky_set_sky) =
		{
			TAG_FIELD_ENTRY_PAD(sizeof(int16)),
			TAG_FIELD_ENTRY(_field_short_block_index, "sky index", nullptr), // NOTE: initialized later
			TAG_FIELD_ENTRY(_field_tag_reference, "sky", &Shared::TAG_GROUP_REFERENCE_GET(sky) ),
			TAG_FIELD_ENTRY_END()
		};
		TAG_GROUP_BLOCK_DEFINE(scenario_bsp_sky_set_sky, 0, 8,
			sizeof(s_scenario_bsp_sky_set_sky)
		);


		TAG_GROUP_BLOCK_FIELDS_DEFINE(scenario_bsp_sky_set) =
		{
			TAG_FIELD_ENTRY(_field_string, "name" TAG_FIELD_MARKUP_IS_BLOCK_NAME),
			TAG_FIELD_ENTRY(_field_block, "skies", &TAG_GROUP_BLOCK_GET(scenario_bsp_sky_set_sky)),
			TAG_FIELD_ENTRY_END()
		};
		TAG_GROUP_BLOCK_DEFINE(scenario_bsp_sky_set, 0, 64,
			sizeof(s_scenario_bsp_sky_set)
		);


		TAG_GROUP_BLOCK_FIELDS_DEFINE(scenario_bsp_modifier) =
		{
			TAG_FIELD_ENTRY_PAD(sizeof(int16)),
			TAG_FIELD_ENTRY(_field_short_block_index, "bsp index", nullptr), // NOTE: initialized later
			TAG_FIELD_ENTRY(_field_block, "lightmap sets", &TAG_GROUP_BLOCK_GET(scenario_bsp_lightmap_set)),
			TAG_FIELD_ENTRY(_field_block, "sky sets", &TAG_GROUP_BLOCK_GET(scenario_bsp_sky_set)),
			TAG_FIELD_ENTRY_PAD(sizeof(tag_block) * 3),
			TAG_FIELD_ENTRY_END()
		};
		TAG_GROUP_BLOCK_DEFINE(scenario_bsp_modifier, 0, 32,
			sizeof(s_scenario_bsp_modifier)
		);


		static void Initialize()
		{
			tag_group* scnr = blam::tag_group_get<scenario>();
			auto* header_block = scnr->header_block_definition;

			Shared::LinkBlockIndex<Enums::_field_short_block_index>(*scnr->header_block_definition, "skies", TAG_GROUP_BLOCK_GET(scenario_bsp_sky_set_sky), "sky index");
			Shared::LinkBlockIndex<Enums::_field_short_block_index>(*scnr->header_block_definition, "structure bsps", TAG_GROUP_BLOCK_GET(scenario_bsp_modifier), "bsp index");

			FIELDSET_REPLACEMENT_BEGIN(scenario, 71 + 1, header_block)
				FIELDSET_SEEK_AFTER_WITH_COPY("cutscene titles")
				FIELDSET_INSERT_BEGIN()
					FIELDSET_INSERT_BLOCK(bsp_modifiers, "bsp modifiers", &TAG_GROUP_BLOCK_GET(scenario_bsp_modifier))
				FIELDSET_INSERT_END()
			FIELDSET_REPLACEMENT_END();
		}
	};};};
};