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
		union s_shader_postprocess_value_union
		{
			static const size_t k_sizeof = 36;
			TAG_PAD(byte, k_sizeof);

			struct s_bitmap {
				t_shader_variable_texture handle;

				TAG_FIELD(uint16, bitmap_index);
				PAD16;
			}bitmap;

			struct s_bool {
				t_shader_variable_bool handle;

				TAG_FIELD(bool, enabled); // byte_flags
				bool inverse; // inverse value of 'enabled
				PAD16;
			}boolean;

			struct s_integer32 {
				t_shader_variable_int handle;

				TAG_FIELD(int32, lower_bound);
				TAG_FIELD(int32, upper_bound);
			}integer32;

			struct s_real32 {
				t_shader_variable_real handle;

				TAG_FIELD(real, lower_bound);
				TAG_FIELD(real, upper_bound);
			}real32;

			struct s_real_vector2d {
				t_shader_variable_real2d handle;

				TAG_FIELD(real_vector2d, lower_bound);
				TAG_FIELD(real_vector2d, upper_bound);
			}vector2d;

			struct s_real_vector3d {
				t_shader_variable_real3d handle;

				TAG_FIELD(real_vector3d, lower_bound);
				TAG_FIELD(real_vector3d, upper_bound);
			}vector3d;

			struct s_real_vector4d {
				t_shader_variable_real4d handle;

				TAG_FIELD(real_quaternion, lower_bound);
				TAG_FIELD(real_quaternion, upper_bound);
			}vector4d;

			struct s_real_color4d {
				t_shader_variable_color handle;

				TAG_FIELD(real_argb_color, lower_bound);
				TAG_FIELD(real_argb_color, upper_bound);
			}color4d;

		}; BOOST_STATIC_ASSERT( sizeof(s_shader_postprocess_value_union) == s_shader_postprocess_value_union::k_sizeof );

		struct s_shader_postprocess_value_animation_function
		{
			TAG_ENUM(function, Enums::periodic_function);
			struct _flags {
				TAG_FLAG8(inverted);
				TAG_FLAG8(multichannel_noise);
				TAG_FLAG8(ignore_alpha);
			}flags;	BOOST_STATIC_ASSERT( sizeof(_flags) == sizeof(byte_flags) );
			PAD8;

			TAG_FIELD(real, animation_duration);
			TAG_FIELD(real, animation_rate);
		}; BOOST_STATIC_ASSERT( sizeof(s_shader_postprocess_value_animation_function) == 0xC );
		struct s_shader_postprocess_value_base
		{
			TAG_FIELD(tag_string, value_name);

			// Set by tag post-processing code
			shader_variable_type value_type;

			s_shader_postprocess_value_union value;
			s_shader_postprocess_value_animation_function animation_function;

			void GetBoundingValues(void*& lower_ref, void*& upper_ref);
			void SetVariableInterp(LPD3DXEFFECT* effect, const void* lower_data, const void* upper_data, const real* interp_values);

			// [fixup_argb_color_hack] - internal use only, use default when calling externally
			void SetVariable(LPD3DXEFFECT* effect, void* data, const bool fixup_argb_color_hack = true);

			// Sets the memory at [dst] to the default variable value (the 'upper' bound)
			void CopyDefaultVariable(void* dst);
		};

		struct s_shader_postprocess_bitmap : s_shader_postprocess_value_base
		{
			union {
				struct {
					TagGroups::bitmap_data* bitmap;
					PAD32;
				}_internal;	// We use a '_' prefix so intelli-sense doesn't get retarded

				struct {
					cstring source;
					IDirect3DTexture9* texture_2d;
				}external;
			}runtime;

			struct {
				TAG_FLAG16(is_loaded);
				TAG_FLAG16(is_external);
			}flags; PAD16;
			TAG_FIELD(tag_reference, bitmap, 'bitm');

#if !PLATFORM_IS_EDITOR && !PLATFORM_IS_DEDI
			void				SetSource(TagGroups::bitmap_data* source_bitmap);
			void				SetSource(cstring source_bitmap);

			HRESULT				LoadCacheBitmap(IDirect3DDevice9* pDevice);
			void				ReleaseBitmap();

			IDirect3DTexture9*	GetTexture();
#endif
		}; BOOST_STATIC_ASSERT( sizeof(s_shader_postprocess_bitmap) == 0x1C + sizeof(s_shader_postprocess_value_base) );


		struct s_shader_postprocess_implementation
		{
			TAG_TBLOCK_(additional_bitmaps, s_shader_postprocess_bitmap);
			TAG_TBLOCK_(bools, s_shader_postprocess_value_base);
			TAG_TBLOCK_(integers, s_shader_postprocess_value_base);
			TAG_TBLOCK_(floats, s_shader_postprocess_value_base);
			TAG_TBLOCK_(float2s, s_shader_postprocess_value_base);
			TAG_TBLOCK_(float3s, s_shader_postprocess_value_base);
			TAG_TBLOCK_(float4s, s_shader_postprocess_value_base);
			TAG_TBLOCK_(colors, s_shader_postprocess_value_base);
		}; BOOST_STATIC_ASSERT( sizeof(s_shader_postprocess_implementation) == 0x60);

		struct s_shader_postprocess_generic : s_shader_postprocess_definition
		{
			enum { k_group_tag = 'shpg' };
			PAD16;
			PAD16;

			TAG_FIELD(tag_reference, base_shader, s_shader_postprocess_generic::k_group_tag);
			TAG_FIELD(s_shader_postprocess_implementation, implementation);

#if !PLATFORM_IS_EDITOR // for externally defined shaders
			s_shader_postprocess_generic()	{}
#endif
		}; BOOST_STATIC_ASSERT( sizeof(s_shader_postprocess_generic) == 0x14 + sizeof(s_shader_postprocess_definition) + sizeof(s_shader_postprocess_implementation) );
	};
};