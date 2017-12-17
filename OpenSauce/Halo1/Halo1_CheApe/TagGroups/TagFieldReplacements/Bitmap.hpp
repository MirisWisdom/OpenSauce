/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

#include <blamlib/Halo1/tag_files/tag_groups_structures_macros.hpp>
#include <YeloLib/Halo1/tag_files/tag_fieldset_replacement_builder.hpp>
#include <blamlib/Halo1/bitmaps/bitmap_definition.hpp>

#include "TagGroups/TagFieldReplacements/Shared.hpp"

namespace Yelo
{
	namespace TagGroups { namespace TagFieldReplacements { namespace Bitmap
	{
		static void BitmapGroupUpdateFlags(tag_block_definition* bitmap_block)
		{
			auto* flags_list = Shared::FindFlagsField<Enums::_field_word_flags>(bitmap_block);
			assert(flags_list->count==e_bitmap_group_flags::k_count);

			static cstring bitmap_group_flags_yelo_strings[e_bitmap_group_flags::k_count_yelo];
			for (int x = 0; x < e_bitmap_group_flags::k_count; x++)
				bitmap_group_flags_yelo_strings[x] = flags_list->strings[x];

			Shared::PopulateWithReservedString<e_bitmap_group_flags::reserved4_bit, e_bitmap_group_flags::reserved12_bit>(
				bitmap_group_flags_yelo_strings);

			bitmap_group_flags_yelo_strings[e_bitmap_group_flags::never_share_resources_yelo_bit] =
				Shared::k_never_share_resources_flag_string;

			flags_list->strings = bitmap_group_flags_yelo_strings;
			flags_list->count = NUMBEROF(bitmap_group_flags_yelo_strings);
		}

		static void Initialize()
		{
			tag_group* bitmap = blam::tag_group_get<s_bitmap_definition>();
			assert(bitmap);

			BitmapGroupUpdateFlags(bitmap->header_block_definition);
		}
	};};};
};