#pragma once

#include <blamlib/cseries/cseries_base.hpp>
#include <blamlib/memory/data_base.hpp>
#include <blamlib/memory/datum_index.hpp>
#include <blamlib/tag_files/tag_block.h>
#include <blamlib/tag_files/tag_groups_base.hpp>

namespace Yelo
{
	struct s_tag_instance : Memory::s_datum_base_aligned
	{
		char filename[Enums::k_max_tag_name_length + 1]; // 0x4
		tag group_tag; // 0x104
		tag parent_group_tags[2]; // 0x108
		bool is_verified; // 0x110 was loaded with Flags::_tag_load_for_editor_bit
		bool is_read_only; // 0x111
		bool is_orphan; // 0x112
		bool is_reload; // 0x113 true if this instance is the one used for another tag during tag_reload
		datum_index reload_index; // 0x114 index of the instance used to reload -this- tag's definition
		uint32 file_checksum; // 0x118
		tag_block root_block; // 0x11C
	};

	ASSERT_SIZE(s_tag_instance, 0x128);
}
