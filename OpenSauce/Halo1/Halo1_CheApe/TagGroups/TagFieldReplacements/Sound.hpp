/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

#include <blamlib/Halo1/tag_files/tag_groups_structures_macros.hpp>
#include <YeloLib/Halo1/tag_files/tag_fieldset_replacement_builder.hpp>
#include <blamlib/Halo1/sound/sound_definitions.hpp>

#include "TagGroups/TagFieldReplacements/Shared.hpp"

namespace Yelo
{
	namespace TagGroups { namespace TagFieldReplacements { namespace Sound
	{
		static void SoundPermutationsBlockFixActualFields(tag_block_definition* sound_permutations_block)
		{
			assert(0==strcmp("sound_permutations_block", sound_permutations_block->name));

			// s_sound_permutation->decompressed_buffer_size (actually 'unused0[0]' in engine code) is a postprocessed
			// field, but GBX left the tag definitions as 'pad' instead of changing it to 'skip'. We correct this here.
			// +2 as we begin the initial padding, then a new (correct) skip, then the END() logic will finish the existing pad field
			FIELDSET_REPLACEMENT_BEGIN(s_sound_permutation, 10+2, sound_permutations_block)
				FIELDSET_SEEK_AFTER_WITH_COPY("next permutation index")
				FIELDSET_INSERT_BEGIN()
					FIELDSET_INSERT_PAD(next_permutation_index, 0,
						sizeof(datum_index) +	// cache_block_index
						sizeof(void*) +			// cache_base_address
						sizeof(datum_index)		// cache_tag_index
						)
					FIELDSET_INSERT_SKIP(decompressed_buffer_size)
				FIELDSET_INSERT_END()
			FIELDSET_REPLACEMENT_END();
		}

		static void SoundUpdateFlags(tag_block_definition* sound_block)
		{
			auto* flags_list = Shared::FindFlagsField<Enums::_field_long_flags>(sound_block);
			assert(flags_list->count==Flags::k_number_of_sound_definition_flags);

			static cstring sound_definition_flags_yelo_strings[Flags::k_number_of_sound_definition_flags_yelo];
			for (int x = 0; x < Flags::k_number_of_sound_definition_flags; x++)
				sound_definition_flags_yelo_strings[x] = flags_list->strings[x];

			Shared::PopulateWithReservedString<Flags::_sound_definition_reserved2_bit, Flags::_sound_definition_reserved9_bit>(
				sound_definition_flags_yelo_strings);

			sound_definition_flags_yelo_strings[Flags::_sound_definition_never_share_resources_yelo_bit] =
				Shared::k_never_share_resources_flag_string;

			flags_list->strings = sound_definition_flags_yelo_strings;
			flags_list->count = NUMBEROF(sound_definition_flags_yelo_strings);
		}

		static void Initialize()
		{
			tag_group* sound = blam::tag_group_get<sound_definition>();
			assert(sound);

			SoundUpdateFlags(sound->header_block_definition);

			auto* block = sound->header_block_definition;
			// find sound->pitch_ranges
			int field_index = block->FindFieldIndex(Enums::_field_block, "pitch ranges");
			assert(field_index != NONE);

			block = block->fields[field_index].Definition<tag_block_definition>();
			// find sound_pitch_range->permutations
			field_index = block->FindFieldIndex(Enums::_field_block, "permutations");
			assert(field_index != NONE);

			block = block->fields[field_index].Definition<tag_block_definition>();

			SoundPermutationsBlockFixActualFields(block);
		}
	};};}
};