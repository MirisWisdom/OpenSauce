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
	namespace Enums
	{
		enum bitmap_group_type : _enum
		{
			_bitmap_group_type_2d_textures,
			_bitmap_group_type_3d_textures,
			_bitmap_group_type_cube_maps,
			_bitmap_group_type_sprites,
			_bitmap_group_type_interface_bitmaps,

			_bitmap_group_type
		};

		enum bitmap_group_format : _enum
		{
			_bitmap_group_format_compressed_color_key_transparency,
			_bitmap_group_format_compressed_explicit_alpha,
			_bitmap_group_format_compressed_interpolated_alpha,
			_bitmap_group_format_16bit_color,
			_bitmap_group_format_32bit_color,
			_bitmap_group_format_monochrome,

			_bitmap_group_format
		};

		enum bitmap_group_usage : _enum
		{
			_bitmap_group_usage_alpha_blend,
			_bitmap_group_usage_default,
			_bitmap_group_usage_height_map,
			_bitmap_group_usage_detail_map,
			_bitmap_group_usage_light_map,
			_bitmap_group_usage_vector_map,

			_bitmap_group_usage
		};
	};

	namespace TagGroups
	{
		// struct bitmap_group_sprite
		// struct bitmap_group_sequence

		struct bitmap_data
		{
			TAG_FIELD(tag, signature);
			TAG_FIELD(int16, width);
			TAG_FIELD(int16, height);
			TAG_FIELD(int16, depth);
			TAG_FIELD(_enum, type);
			TAG_FIELD(_enum, format);
			TAG_FIELD(word_flags, flags);
			TAG_FIELD(point2d, registration_point);
			TAG_FIELD(int16, mipmap_count);
			PAD16;
			TAG_FIELD(int32, pixels_offset);

			UNKNOWN_TYPE(int32);
			datum_index owner_tag_index;
			datum_index texture_cache_index;
			void* hardware_format; // IDirect3DBaseTexture9*
			void* base_address;
		}; BOOST_STATIC_ASSERT( sizeof(bitmap_data) == 0x30 );

		struct bitmap_group
		{
			enum { k_group_tag = 'bitm' };

			TAG_FIELD(_enum, type);
			TAG_FIELD(_enum, format);
			TAG_FIELD(_enum, usage);
			TAG_FIELD(word_flags, flags);

			TAG_FIELD(real, detail_fade_factor);
			TAG_FIELD(real, sharpen_amount);
			TAG_FIELD(real, bump_height);

			TAG_FIELD(_enum, sprite_budget_size);
			TAG_FIELD(int16, sprite_budget_count);

			TAG_FIELD(int16, color_plate_width);
			TAG_FIELD(int16, color_plate_height);
			TAG_FIELD(tag_data, compressed_color_plate_data);
			TAG_FIELD(tag_data, processed_pixel_data);

			TAG_FIELD(real, blur_filter_size);
			TAG_FIELD(real, alpha_bias);
			TAG_FIELD(int16, mipmap_count);

			TAG_FIELD(_enum, sprite_usage);
			TAG_FIELD(int16, sprite_spacing);
			PAD16;
			TAG_FIELD(tag_block, sequences);

			TAG_TBLOCK_(bitmaps, bitmap_data);
		}; BOOST_STATIC_ASSERT( sizeof(bitmap_group) == 0x6C );
	};
};