/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/tag_files/tag_groups.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		struct s_unit_boarding_seat
		{
			struct __flags
			{
				TAG_FLAG16(boarding_ejects_target_seat);
				TAG_FLAG16(boarding_enters_target_seat);
				TAG_FLAG16(controls_open_and_close);
			}; BOOST_STATIC_ASSERT( sizeof(__flags) == sizeof(word_flags) );

			TAG_FIELD(__flags, flags);
			PAD16;
			TAG_FIELD(int16, seat_index);
			TAG_FIELD(int16, target_seat_index);
			TAG_FIELD(tag_string, seat_label);
			TAG_FIELD(tag_string, target_seat_label);
			TAG_FIELD(tag_reference, boarding_damage, "jpt!");
			TAG_PAD(int32, 8);
		}; BOOST_STATIC_ASSERT( sizeof(s_unit_boarding_seat) == 0x78 ); // max count: 32

		struct s_unit_external_upgrades
		{
			TAG_FIELD(tag_reference, unit, "unit");
			TAG_TBLOCK(boarding_seats, s_unit_boarding_seat);
			TAG_PAD(int32, 9);
		}; BOOST_STATIC_ASSERT( sizeof(s_unit_external_upgrades) == 0x40 ); // max count: 64

		//////////////////////////////////////////////////////////////////////////
		// Currently unused
		struct s_object_damage_region_permutation_extension
		{
			TAG_FIELD(tag_string, name);
			TAG_FIELD(real, damage_threshold);

			TAG_FIELD(tag_string, effect_marker_name);
			TAG_FIELD(tag_reference, transition_effect, 'effe');
			TAG_PAD(int32, 8);

			TAG_FIELD(tag_string, damage_effect_marker_name);
			TAG_FIELD(tag_reference, transition_damage_effect, 'jpt!');
			TAG_PAD(int32, 8);
		}; BOOST_STATIC_ASSERT( sizeof(s_object_damage_region_permutation_extension) == 0xC4 );
		struct s_object_damage_region_extension
		{
			TAG_FIELD(tag_string, name);
			TAG_TBLOCK(permutations, s_object_damage_region_permutation_extension); // 32
		}; BOOST_STATIC_ASSERT( sizeof(s_object_damage_region_extension) == 0x2C );
		struct s_object_damage_extension
		{
			TAG_FIELD(tag_reference, object, 'obje');
			TAG_TBLOCK(regions, s_object_damage_region_extension); // 32
		}; BOOST_STATIC_ASSERT( sizeof(s_object_damage_extension) == 0x1C );
		//////////////////////////////////////////////////////////////////////////

		// yelo for globals
		struct project_yellow_globals_cv
		{
			/* !-- ChokingVictim globals --! */
			int16 version;	enum { k_version = 1, k_group_tag = 'gelc' };
			PAD16;
			
			TAG_TBLOCK(unit_external_upgrades, s_unit_external_upgrades); // 64
			//TAG_TBLOCK(dual_wielding, s_dual_wield_weapon_definition); // 256
			//TAG_TBLOCK(object_damage_extensions, s_object_damage_extension); // 512
			
			TAG_PAD(int32, 32);
		}
	};
};