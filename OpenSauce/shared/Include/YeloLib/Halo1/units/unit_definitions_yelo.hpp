/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <YeloLib/tag_files/tag_groups_base_yelo.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		struct unit_seat_boarding
		{
			PAD16;
			int16 target_seat_index;
			TAG_FIELD(tag_reference, boarding_damage, "jpt!");
			TAG_PAD(int32, 3); // 12
		}; BOOST_STATIC_ASSERT( sizeof(unit_seat_boarding) == 32 );

		struct unit_seat_yelo_extensions
		{
			unit_seat_boarding boarding;
			TAG_PAD(int32, 20); // 80
		}; BOOST_STATIC_ASSERT( sizeof(unit_seat_yelo_extensions) == 112 );
	};
};