/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

#include <blamlib/Halo1/tag_files/tag_groups_structures_macros.hpp>
#include <YeloLib/Halo1/tag_files/tag_fieldset_replacement_builder.hpp>
#include <blamlib/Halo1/bitmaps/bitmap_group.hpp>

#include "TagGroups/TagFieldReplacements/Shared.hpp"

namespace Yelo
{
	namespace TagGroups { namespace TagFieldReplacements { namespace Bitmap
	{
		static void BitmapGroupUpdateFlags(tag_block_definition* bitmap_block)
		{
			auto* flags_list = Shared::FindFlagsField<Enums::_field_word_flags>(bitmap_block);
			assert(flags_list->count==Flags::k_number_of_bitmap_group_flags);

			static cstring bitmap_group_flags_yelo_strings[Flags::k_number_of_bitmap_group_flags_yelo];
			for (int x = 0; x < Flags::k_number_of_bitmap_group_flags; x++)
				bitmap_group_flags_yelo_strings[x] = flags_list->strings[x];

			Shared::PopulateWithReservedString<Flags::_bitmap_group_reserved4_bit, Flags::_bitmap_group_reserved12_bit>(
				bitmap_group_flags_yelo_strings);

			bitmap_group_flags_yelo_strings[Flags::_bitmap_group_never_share_resources_yelo_bit] =
				Shared::k_never_share_resources_flag_string;

			flags_list->strings = bitmap_group_flags_yelo_strings;
			flags_list->count = NUMBEROF(bitmap_group_flags_yelo_strings);
		}

		static void Initialize()
		{
			tag_group* bitmap = blam::tag_group_get<s_bitmap_group>();
			assert(bitmap);

			BitmapGroupUpdateFlags(bitmap->header_block_definition);
		}
	};};};
};