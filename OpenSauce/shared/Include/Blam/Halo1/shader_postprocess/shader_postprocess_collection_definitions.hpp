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
		struct s_shader_postprocess_collection_script_variable
		{
			TAG_FIELD(tag_string, name);
			TAG_FIELD(tag_string, shader_variable_name);
			TAG_FIELD(shader_variable_type, value_type);
			TAG_FIELD(int32, shader_index);

			struct _runtime {
				ID3DXEffect* effect_reference;

				union {		
					t_shader_variable_bool		boolean;
					t_shader_variable_int		integer32;
					t_shader_variable_real		real32;
					t_shader_variable_real2d	vector2d;
					t_shader_variable_real3d	vector3d;
					t_shader_variable_real4d	vector4d;
					t_shader_variable_real4d	color4d;
				}handle;

				union {		
					bool			boolean;
					int32			integer32;
					real			real32;
					real_vector2d	vector2d;
					real_vector3d	vector3d;
					real_quaternion vector4d;
					real_argb_color color4d;
				}value;
			}runtime;

		}; BOOST_STATIC_ASSERT( sizeof(s_shader_postprocess_collection_script_variable) == 0x60);

		struct s_shader_postprocess_collection_shader
		{
			TAG_FIELD(tag_reference, shader, s_shader_postprocess_generic::k_group_tag);

			struct _runtime {
				s_shader_postprocess_generic* shader_reference;
			}runtime;
		}; BOOST_STATIC_ASSERT( sizeof(s_shader_postprocess_collection_shader) == 0x14);

		struct s_shader_postprocess_collection_shader_index
		{
			TAG_FIELD(int32, shader_index);
			PAD32;
		}; BOOST_STATIC_ASSERT( sizeof(s_shader_postprocess_collection_shader_index) == 0x8);

		struct s_shader_postprocess_collection_effect : s_shader_postprocess_effect_generic
		{
			TAG_TBLOCK_(shader_indices, s_shader_postprocess_collection_shader_index);
			TAG_TBLOCK_(script_variables, s_shader_postprocess_collection_script_variable);
		}; BOOST_STATIC_ASSERT( sizeof(s_shader_postprocess_collection_effect) == 0x18 + sizeof(s_shader_postprocess_effect_generic) );


		struct s_shader_postprocess_collection
		{
			enum { k_group_tag = 'shpc' };
			PAD16;
			PAD16;

			TAG_TBLOCK_(shaders, s_shader_postprocess_collection_shader);
			TAG_TBLOCK_(effects, s_shader_postprocess_collection_effect);
		}; BOOST_STATIC_ASSERT( sizeof(s_shader_postprocess_collection) == 0x1C);
	};
};