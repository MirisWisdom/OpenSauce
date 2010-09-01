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
		struct s_shader_animation_function
		{
			_enum source;
			_enum function;
			real period, phase, scale;
		}; BOOST_STATIC_ASSERT( sizeof(s_shader_animation_function) == 16 );
		struct s_shader_animation
		{
			s_shader_animation_function anim_u, anim_v, anim_rot;
			real_point2d rotation_center;
		}; BOOST_STATIC_ASSERT( sizeof(s_shader_animation) == 56 );

		struct s_shader_texture_animation_function
		{
			_enum function;
			PAD16;
			real period, scale;
		}; BOOST_STATIC_ASSERT( sizeof(s_shader_texture_animation_function) == 12 );

		struct s_shader_map
		{
			PAD128; PAD128; PAD64;

			word_flags shader_flags;
			_enum framebuffer_blend_function, framebuffer_fade_mode;
			word_flags shader_map_flags;

			PAD128; PAD64; PAD32;

			tag_reference map_bitmap;
			_enum map_anchor;
			word_flags map_flags;
			s_shader_animation map_animation;

			PAD32;
			real z_sprite_radius_scale;
			PAD128; PAD32;
		}; BOOST_STATIC_ASSERT( sizeof(s_shader_map) == 180 );

		struct s_shader_color_function
		{
			real_rgb_color on_color, off_color;
			_enum anim_function;
			PAD16;
			real anim_period, anim_phase;
			PAD128; PAD64;
		}; BOOST_STATIC_ASSERT( sizeof(s_shader_color_function) == 60 );

		struct s_shader_framebuffer_function
		{
			_enum blend_function, 
				blend_fade_mode, 
				fade_source;
			PAD16;
		}; BOOST_STATIC_ASSERT( sizeof(s_shader_framebuffer_function) == 8 );
		struct s_shader_radiosity_properties
		{
			byte numeric_counter_limit;
			byte_flags flags;
			_enum first_map_type;
			s_shader_framebuffer_function framebuffer_function;
		}; BOOST_STATIC_ASSERT( sizeof(s_shader_radiosity_properties) == 12 );


		struct _shader_definition
		{
			struct {
				word_flags flags;
				_enum detail_level;
				real power;
				real_rgb_color emitted_light_color, tint_color;
			}radiosity;

			word_flags physics_flags;
			_enum material_type;

			_enum shader_type;
			PAD16;
		}; BOOST_STATIC_ASSERT( sizeof(_shader_definition) == 0x28 );

		struct s_shader_definition
		{
			enum { k_group_tag = 'shdr' };

			_shader_definition shader;
		};

		//////////////////////////////////////////////////////////////////////////

		struct _shader_effect_definition
		{
			byte_flags flags;
			PAD8;
			s_shader_framebuffer_function framebuffer_function;
			PAD128; PAD128;
			tag_reference bitmap;
			_enum permutation_function;
			PAD16;
			s_shader_animation animation;
		};

		struct s_shader_effect : s_shader_definition
		{
			_shader_effect_definition effect;
		};

		//////////////////////////////////////////////////////////////////////////

		struct _shader_environment_definition
		{
			word_flags flags;
			_enum type;

			// TODO
		};

		struct s_shader_environment : s_shader_definition
		{
			_shader_environment_definition environment;
		};

		//////////////////////////////////////////////////////////////////////////

		struct _shader_model_definition
		{
			word_flags flags;
			PAD16;
			TAG_PAD(int32, 3);
			real_fraction translucency;
			PAD128;
			_enum change_color_source;
			PAD16;
			TAG_PAD(int32, 7);

			struct {
				word_flags flags;
				PAD16;
				_enum color_source;
				_enum animation_function;
				real animation_period;
				real_rgb_color animation_color_lower, animation_color_upper;
				TAG_PAD(int32, 3);
			}self_illumination;

			struct { // 0x9C
				real u_scale, v_scale;
				tag_reference base_map;
				PAD64;
				tag_reference multipurpose_map;
				PAD64;

				_enum detail_function;
				_enum detail_mask;
				real detail_map_scale;
				tag_reference detail_map;
				real detail_map_v_scale;
				TAG_PAD(int32, 3);
				s_shader_animation animation;
				PAD64;
			}maps;

			struct {
				real falloff_distance;
				real cutoff_distance;
				real_fraction perpendicular_brightness;
				real_rgb_color perpendicular_tint_color;
				real_fraction parallel_brightness;
				real_rgb_color parallel_tint_color;

				tag_reference cubemap;
			}reflection_properties;

			TAG_PAD(byte, 16 + 4 + 16 + 32);
			
		}; BOOST_STATIC_ASSERT( sizeof(_shader_model_definition) == 0x190 );

		struct s_shader_model : s_shader_definition
		{
			_shader_model_definition model;
		};

		//////////////////////////////////////////////////////////////////////////

		struct shader_transparent_chicago_map_block
		{
		};

		struct _shader_transparent_chicago_definition
		{
		};

		struct s_shader_transparent_chicago : s_shader_definition
		{
			_shader_transparent_chicago_definition transparent;
		};

		//////////////////////////////////////////////////////////////////////////

		struct _shader_transparent_chicago_extended_definition
		{
		};

		struct s_shader_transparent_chicago_extended : s_shader_definition
		{
			_shader_transparent_chicago_extended_definition transparent;
		};

		//////////////////////////////////////////////////////////////////////////

		struct shader_transparent_generic_map_block
		{
		};

		struct _shader_transparent_generic_definition
		{
		};

		struct s_shader_transparent_generic : s_shader_definition
		{
			_shader_transparent_generic_definition transparent;
		};

		//////////////////////////////////////////////////////////////////////////

		struct _shader_transparent_glass_definition
		{
		};

		struct s_shader_transparent_glass : s_shader_definition
		{
			_shader_transparent_glass_definition transparent;
		};

		//////////////////////////////////////////////////////////////////////////

		struct _shader_transparent_meter_definition
		{
		};

		struct s_shader_transparent_meter : s_shader_definition
		{
			_shader_transparent_meter_definition transparent;
		};

		//////////////////////////////////////////////////////////////////////////

		struct _shader_transparent_plasma_definition
		{
		};

		struct s_shader_transparent_plasma : s_shader_definition
		{
			_shader_transparent_plasma_definition transparent;
		};

		//////////////////////////////////////////////////////////////////////////

		struct shader_transparent_water_ripple_block
		{
		};

		struct _shader_transparent_water_definition
		{
		};

		struct s_shader_transparent_water : s_shader_definition
		{
			_shader_transparent_water_definition transparent;
		};

		//////////////////////////////////////////////////////////////////////////
	};
};