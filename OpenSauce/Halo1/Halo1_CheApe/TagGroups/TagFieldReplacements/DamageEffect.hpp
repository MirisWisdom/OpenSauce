/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

#include <blamlib/Halo1/tag_files/tag_groups_structures_macros.hpp>
#include <YeloLib/Halo1/tag_files/tag_fieldset_replacement_builder.hpp>
#include <blamlib/Halo1/effects/damage_effect_definitions.hpp>

#include "TagGroups/TagFieldReplacements/Shared.hpp"

namespace Yelo
{
	namespace TagGroups { namespace TagFieldReplacements { namespace DamageEffect
	{
		static void InstantaneousAccelerationTo3d(tag_block_definition* damage_header_block)
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

		static void Initialize()
		{
			tag_group* jpt_ = blam::tag_group_get<s_damage_effect_definition>();
			tag_group* cdmg = blam::tag_group_get<s_continuous_damage_effect_definition>();
			assert(jpt_ && cdmg);

			InstantaneousAccelerationTo3d(jpt_->header_block_definition);
			InstantaneousAccelerationTo3d(cdmg->header_block_definition);

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
					TAG_FIELD_MARKUP_PREFIX_HELP
					"factor in the J and K components instead of only I";

			flags_list->strings = damage_flags_yelo_strings;
			flags_list->count = NUMBEROF(damage_flags_yelo_strings);
		}
	};};};
};