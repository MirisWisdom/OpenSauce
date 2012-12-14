/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/tag_files/tag_groups_base.hpp>
#include <YeloLib/tag_files/tag_groups_yelo.hpp>

#if PLATFORM_IS_EDITOR
	#include <blamlib/Halo1/tag_files/tag_groups_structures.hpp>
#endif

namespace Yelo
{
	namespace Enums
	{
		enum {
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