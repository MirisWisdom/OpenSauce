/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

#include <blamlib/Halo1/tag_files/tag_groups_structures_macros.hpp>
#include <YeloLib/Halo1/tag_files/tag_fieldset_replacement_builder.hpp>
#include <blamlib/Halo1/shaders/shader_definitions.hpp>

namespace Yelo
{
	namespace TagGroups { namespace TagFieldReplacements { namespace Shader
	{
		static void InitializeShaderEnvironment()
		{
			tag_group* senv = blam::tag_group_get<s_shader_environment_definition>();

			TAG_GROUP_STRING_TABLE_DEFINE(shader_environment_extension_flags, 1, "do not use dlms");

			TAG_GROUP_BLOCK_FIELDS_DEFINE(shader_environment_extension) =
			{
				TAG_FIELD_ENTRY(_field_explanation, "directional lightmaps", "" ),
				TAG_FIELD_ENTRY(_field_word_flags, "flags", &shader_environment_extension_flags),
				TAG_FIELD_ENTRY_PAD(sizeof(int16) * 1),
				TAG_FIELD_ENTRY(_field_real, "bump amount", nullptr),
				TAG_FIELD_ENTRY_PAD(sizeof(tag_block) * 4),
				TAG_FIELD_ENTRY_END()
			};

			TAG_GROUP_BLOCK_DEFINE(shader_environment_extension, 0, 1, sizeof(s_shader_environment_extension));

			FIELDSET_REPLACEMENT_BEGIN(_shader_environment_definition, 92+2, senv->header_block_definition)
				FIELDSET_SEEK_AFTER_WITH_COPY("lens flare")
				FIELDSET_INSERT_BEGIN()
					FIELDSET_INSERT_BLOCK(shader_extension, "shader extension", &TAG_GROUP_BLOCK_GET(shader_environment_extension))
				FIELDSET_INSERT_END()
			FIELDSET_REPLACEMENT_END();
		}
		
		static void Initialize()
		{
			InitializeShaderEnvironment();
		}
	};};};
};