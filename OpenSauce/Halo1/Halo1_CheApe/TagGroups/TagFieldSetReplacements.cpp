/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "TagGroups/TagGroups.hpp"

#include <YeloLib/Halo1/tag_files/tag_fieldset_replacement_builder.hpp>

#include <blamlib/Halo1/shaders/shader_definitions.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		void InitializeFieldSetReplacementsForShaderEnvironment()
		{
			tag_group* senv = blam::tag_group_get<s_shader_environment_definition>();

			static tag_field shader_environment_extension_fields[] = {
				{ Enums::_field_terminator, }
			};
			static tag_block_definition shader_environment_extension_block = {
				"shader_environment_extension_block", 0, 1,
				0,//sizeof(s_shader_environment_extension),
				nullptr,
				shader_environment_extension_fields
			};
			FIELDSET_REPLACEMENT_BEGIN(_shader_environment_definition, 92+2, senv->header_block_definition)
				FIELDSET_SEEK_AFTER_WITH_COPY("lens flare")
				FIELDSET_INSERT_BEGIN()
					FIELDSET_INSERT(shader_extension, Enums::_field_explanation, "replacement test", "")
					FIELDSET_INSERT_BLOCK(shader_extension, "yelo extension" "*", // TODO: readonly until the block actually has fields
						&shader_environment_extension_block)
				FIELDSET_INSERT_END()
			FIELDSET_REPLACEMENT_END();
		}
		void InitializeFieldSetReplacements()
		{
			// NOTE: call tag_field_set_replacement_builder's here

			InitializeFieldSetReplacementsForShaderEnvironment();
		}
	};
};