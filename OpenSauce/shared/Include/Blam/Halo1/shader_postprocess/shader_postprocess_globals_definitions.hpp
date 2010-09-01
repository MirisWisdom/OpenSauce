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

#include <Blam/Halo1/shader_postprocess_definitions.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		struct s_shader_postprocess_globals_bloom
		{
			struct _flags {
				TAG_FLAG16(is_enabled);
				TAG_FLAG16(apply_after_hud);
			}flags;
			PAD16;
			TAG_FIELD(real,							size);
			TAG_FIELD(real,							exposure);
			TAG_FIELD(real_fraction,				mix_amount);

			TAG_FIELD(real_rgb_color,				minimum_color);
			TAG_FIELD(real_rgb_color,				maximum_color);
		}; BOOST_STATIC_ASSERT( sizeof(s_shader_postprocess_globals_bloom) == 0x28 );
		struct s_shader_postprocess_globals
		{
			enum { k_group_tag = 'sppg' };

			PAD16;
			PAD16;
			union {
				TAG_PAD(tag_block, 20); // Up to to 20 blocks for subsystem globals

				struct {
					TAG_TBLOCK_(bloom_globals, s_shader_postprocess_globals_bloom);
				};
			};
		}; BOOST_STATIC_ASSERT( sizeof(s_shader_postprocess_globals) == 0xF4 );
	};
};