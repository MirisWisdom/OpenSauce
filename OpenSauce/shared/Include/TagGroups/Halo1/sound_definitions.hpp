/*
    Yelo: Open Sauce SDK

    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include <TagGroups/Halo1/TagGroupDefinitions.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		struct s_sound_permutation
		{
			TAG_FIELD(tag_string, name);
			TAG_FIELD(real, skip_fraction);
			TAG_FIELD(real, gain);
			TAG_FIELD(_enum, compression);
			TAG_FIELD(int16, next_permutation_index);

			UNKNOWN_TYPE(int32);				// 0x30, idk
			UNKNOWN_TYPE(int32);				// 0x34, idk
			UNKNOWN_TYPE(uint32);				// 0x38
			datum_index owner_tag_index;		// 0x3C
			datum_index lruv_desired_page_count;// 0x40

			TAG_FIELD(tag_data, samples);
			TAG_FIELD(tag_data, mouth_data);
			TAG_FIELD(tag_data, subtitle_data);
		}; BOOST_STATIC_ASSERT( sizeof(s_sound_permutation) == 0x7C );

		struct s_sound_pitch_range
		{
			TAG_FIELD(tag_string, name);
			TAG_FIELD(real, natural_pitch);
			TAG_FIELD(real_bounds, bend_bounds);
			TAG_FIELD(int16, actual_permutation_count);
			PAD16;
			TAG_PAD(tag_block, 1);
			TAG_TBLOCK_(permutations, s_sound_permutation);
		}; BOOST_STATIC_ASSERT( sizeof(s_sound_pitch_range) == 0x48 );

		struct s_sound_definition_playback_parameters
		{
			TAG_FIELD(real_bounds, audible_distance);
			TAG_FIELD(real, skip_fraction);
			TAG_FIELD(real_bounds, random_pitch_bounds);
			TAG_FIELD(angle_bounds, cone_angles); // inner, outer
			TAG_FIELD(real, outer_cone_gain);
			TAG_FIELD(real, gain_modifier);
			TAG_FIELD(real, max_bend_per_second);
		};
		struct s_sound_definition_scale_modifiers
		{
			TAG_FIELD(real, skip_fraction_modifier);
			TAG_FIELD(real, gain_modifier);
			TAG_FIELD(real, pitch_modifier);
		};
		struct s_sound_promotion_parameters
		{
			TAG_FIELD(tag_reference, sound, "snd!");
			TAG_FIELD(int16, count);
			PAD16;
		};
		struct sound_definition
		{
			enum { k_group_tag = 'snd!' };

			TAG_FIELD(long_flags, flags);
			TAG_FIELD(_enum, sound_class);
			TAG_FIELD(_enum, sample_rate);

			TAG_FIELD(s_sound_definition_playback_parameters, playback_parameters);
			// unused, when-scale-is-ZERO, unused, when-scale-is-ONE, unused
			TAG_FIELD(s_sound_definition_scale_modifiers, scale_modifiers)[5];

			TAG_FIELD(_enum, encoding);
			TAG_FIELD(_enum, compression);

			TAG_FIELD(s_sound_promotion_parameters, promotion_parameters);

			TAG_PAD(tag_data, 1);
			TAG_TBLOCK(pitch_ranges, s_sound_pitch_range);
		}; BOOST_STATIC_ASSERT( sizeof(sound_definition) == 0xA4 );
	};
};