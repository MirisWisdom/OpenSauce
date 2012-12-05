/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/tag_files/tag_groups_base.hpp>
#include <YeloLib/tag_files/tag_groups_yelo.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_maximum_tags_per_tag_chain = 4,
			k_maximum_children_per_tag = 16,

			k_maximum_field_byte_swap_codes = 1024,

			k_protected_group_tag = 'prot', // HEK+: this overrides the scenario's group tag in the tag index.
		};
	};

	namespace TagGroups
	{
		struct tag_iterator {
			PAD32;
			int16 current_index;
			PAD16;
			datum_index current_datum;
			PAD32;
			tag group_tag_filter;
		}; BOOST_STATIC_ASSERT( sizeof(tag_iterator) == 0x14 );
	};
};