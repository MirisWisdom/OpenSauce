/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "TagGroups/TagGroups.hpp"

#include <YeloLib/Halo1/tag_files/tag_fieldset_replacement_builder.hpp>

#include <blamlib/Halo1/effects/damage_effect_definitions.hpp>
#include <blamlib/Halo1/shaders/shader_definitions.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		static void InitializeFieldSetReplacementsForShaderEnvironment()
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
					FIELDSET_ADD_EXPLANATION("replacement test", "")
					FIELDSET_INSERT_BLOCK(shader_extension, "yelo extension" "*", // TODO: readonly until the block actually has fields
						&shader_environment_extension_block)
				FIELDSET_INSERT_END()
			FIELDSET_REPLACEMENT_END();
		}
		static void DamageEffectGroupChangeInstantaneouAccelerationTo3d(tag_block_definition* damage_header_block)
		{
			tag_field* fields = damage_header_block->fields;
			int field_index = damage_header_block->FindFieldIndex(Enums::_field_real, "instantaneous acceleration");
			assert(field_index != NONE);

			fields[field_index++].type = Enums::_field_real_vector_3d;

			// "null" the old padding. It's an error to define zero length padding, so we pretend to be custom
			for (int x = 0; x < 2; x++)
			{
				assert(fields[field_index].type == Enums::_field_pad);
				fields[field_index].type = Enums::_field_custom;
				fields[field_index++].definition = 0;
			}
		}
		static void InitializeFieldSetReplacementsForDamageEffects()
		{
			tag_group* jpt_ = blam::tag_group_get<s_damage_effect_definition>();
			tag_group* cdmg = blam::tag_group_get<s_continuous_damage_effect_definition>();
			assert(jpt_ && cdmg);

			DamageEffectGroupChangeInstantaneouAccelerationTo3d(jpt_->header_block_definition);
			DamageEffectGroupChangeInstantaneouAccelerationTo3d(cdmg->header_block_definition);

			// find the damage effect flags, then the actual damage flags (both have the same name)
			int flags_field_index = jpt_->header_block_definition->FindFieldIndex(Enums::_field_long_flags, "flags");
			assert(flags_field_index != NONE);
			flags_field_index = jpt_->header_block_definition->FindFieldIndex(Enums::_field_long_flags, "flags", flags_field_index+1);
			assert(flags_field_index != NONE);

			static cstring damage_flags_yelo_strings[Flags::k_number_of_damage_flags_yelo];
			auto flags_list = jpt_->header_block_definition->fields[flags_field_index].Definition<string_list>();
			assert(flags_list->count==Flags::k_number_of_damage_flags);

			for (int x = 0; x < Flags::k_number_of_damage_flags; x++)
				damage_flags_yelo_strings[x] = flags_list->strings[x];

			damage_flags_yelo_strings[Flags::_damage_use_3d_acceleration_yelo_bit] = 
				"YELO; 3D inst. accel."
				"#factor in the J and K components instead of only I";

			flags_list->strings = damage_flags_yelo_strings;
			flags_list->count = NUMBEROF(damage_flags_yelo_strings);
		}
		void InitializeFieldSetReplacements()
		{
			// NOTE: call tag_field_set_replacement_builder's here

			InitializeFieldSetReplacementsForShaderEnvironment();
			InitializeFieldSetReplacementsForDamageEffects();
		}
	};
};