/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "TagGroups/TagFieldReplacements/Sound.hpp"

#include <blamlib/Halo1/tag_files/tag_groups_structures_macros.hpp>
#include <YeloLib/Halo1/tag_files/tag_fieldset_replacement_builder.hpp>
#include <blamlib/Halo1/sound/sound_definitions.hpp>

#include "TagGroups/TagFieldReplacements/Shared.hpp"

namespace Yelo
{
	namespace TagGroups { namespace TagFieldReplacements { namespace Sound
	{
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

		void Initialize()
		{
			tag_group* sound = blam::tag_group_get<sound_definition>();
			assert(sound);

			SoundUpdateFlags(sound->header_block_definition);
		}
	};};}
};