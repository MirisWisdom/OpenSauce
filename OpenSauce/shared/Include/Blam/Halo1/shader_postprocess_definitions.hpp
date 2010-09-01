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

#include <d3dx9.h>
#include <TagGroups/Halo1/TagGroupDefinitions.hpp>
#include <TagGroups/Halo1/bitmap_definitions.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum postprocess_render_stage : _enum
		{
			_postprocess_render_stage_pre_blur,
			_postprocess_render_stage_pre_alpha_blended,	
			_postprocess_render_stage_pre_hud,
			_postprocess_render_stage_pre_ui,
			_postprocess_render_stage_post_ui,

			_postprocess_render_stage
		};

		enum shader_variable_base_type : _enum
		{
			_shader_variable_base_type_boolean,
			_shader_variable_base_type_integer,	
			_shader_variable_base_type_float,

			_shader_variable_base_type_argb_color,
			_shader_variable_base_type_matrix,
			_shader_variable_base_type_texture,

			_shader_variable_base_type
		};
	};

	namespace TagGroups
	{
		struct shader_variable_type
		{
			_enum type;		// Enums::shader_variable_base_type
			int16 count;	// # of [shader_variable_base_type]s
		};

		template<Enums::shader_variable_base_type TType, size_t TCount = 1>
		struct s_shader_postprocess_shader_variable
		{
			struct {
				D3DXHANDLE dx_handle;
			}runtime;

#if !PLATFORM_IS_EDITOR && !PLATFORM_IS_DEDI
			bool			IsUsed()		{ return runtime.dx_handle != NULL; }

			void			ClearHandles()	{ runtime.dx_handle = NULL; }

			void			Initialize(LPD3DXEFFECT* effect, const char* id, void* data, bool semantic = false);


			template<typename TValueType>
			void			GetValues(TValueType* values_out, const TValueType* values1, const TValueType* values2, const size_t count, const real* interp_values);
			void			GetValues(BOOL* values_out, const bool* values1, const bool* values2, const size_t count, const real* interp_values);
			void			GetValues(D3DXCOLOR* values_out, const FLOAT* values1, const FLOAT* values2, const size_t count, const real* interp_values);

			void			SetVariableInterp(LPD3DXEFFECT* effect, const void* data1, const void* data2, const real* interp_values);

			// [fixup_argb_color_hack] - internal use only, use default when calling externally
			void			SetVariable(LPD3DXEFFECT* effect, void* data, const bool fixup_argb_color_hack = true);
#endif
		};
		typedef s_shader_postprocess_shader_variable<Enums::_shader_variable_base_type_boolean>		t_shader_variable_bool;
		typedef s_shader_postprocess_shader_variable<Enums::_shader_variable_base_type_integer>		t_shader_variable_int;
		typedef s_shader_postprocess_shader_variable<Enums::_shader_variable_base_type_float>		t_shader_variable_real;
		typedef s_shader_postprocess_shader_variable<Enums::_shader_variable_base_type_float,2>		t_shader_variable_real2d;
		typedef s_shader_postprocess_shader_variable<Enums::_shader_variable_base_type_float,3>		t_shader_variable_real3d;
		typedef s_shader_postprocess_shader_variable<Enums::_shader_variable_base_type_float,4>		t_shader_variable_real4d;
		typedef s_shader_postprocess_shader_variable<Enums::_shader_variable_base_type_argb_color>	t_shader_variable_color;
		typedef s_shader_postprocess_shader_variable<Enums::_shader_variable_base_type_matrix>		t_shader_variable_matrix;
		typedef s_shader_postprocess_shader_variable<Enums::_shader_variable_base_type_texture>		t_shader_variable_texture;


		//////////////////////////////////////////////////////////////////////////
		// shader_postprocess base
		struct s_shader_postprocess_definition
		{
			enum { k_group_tag = 'shpp' };
			PAD16;
			struct _flags {
				TAG_FLAG16(shader_is_binary);
			}flags;

			TAG_FIELD(tag_data, shader_code_text);
			TAG_FIELD(tag_data, shader_code_binary);

			t_shader_variable_matrix	ortho_wvp_matrix;	// Orthographic projection matrix handle
			t_shader_variable_matrix	wv_matrix;			// Current world view matrix handle
			t_shader_variable_matrix	wvt_matrix;			// Current world view transpose matrix handle
			t_shader_variable_real2d	scene_size;			// Handle to the scene size float2 value
			t_shader_variable_real2d	hud_scale;			// Handle to the hud scale float2 value
			t_shader_variable_real2d	pixel_size;			// Handle to the pixel size float2 value
			t_shader_variable_real		near_clip_dist;		// Handle to the near clip float value
			t_shader_variable_real		far_clip_dist;		// Handle to the far clip float value
			t_shader_variable_real		frame_time;			// Handle to the frame time float value
			t_shader_variable_texture	tex_source;			// Handle to the post-process source texture
			t_shader_variable_texture	tex_scene;			// Handle to the saved scene texture
			t_shader_variable_texture	tex_depth;			// Handle to the depth texture
			t_shader_variable_texture	tex_velocity;		// Handle to the velocity texture
			t_shader_variable_texture	tex_normals;		// Handle to the normals texture
			t_shader_variable_texture	tex_index;			// Handle to the normals texture

			TAG_PAD(int32, 4);

			TAG_TBLOCK_(predicted_resources, predicted_resource);

			struct _runtime {
				struct _postprocess_flags{
					TAG_FLAG16(valid_shader);
					TAG_FLAG16(uses_gbuffer);
				}flags; PAD16;	BOOST_STATIC_ASSERT( sizeof(_postprocess_flags) == sizeof(word_flags) );

				TAG_FIELD(LPD3DXEFFECT, dx_effect);
				TAG_FIELD(D3DXHANDLE, postprocess_handle);
			}runtime;

#if !PLATFORM_IS_EDITOR // for externally defined shaders
			s_shader_postprocess_definition()	{}
#endif
		}; BOOST_STATIC_ASSERT( sizeof(s_shader_postprocess_definition) == 0x90 );


		//////////////////////////////////////////////////////////////////////////
		// shader_postprocess_effect base
		struct s_shader_postprocess_effect_render_quad
		{
			struct _runtime {
				TAG_FIELD(IDirect3DVertexBuffer9*, vertex_buffer); 
				TAG_FIELD(IDirect3DIndexBuffer9*, index_buffer); 
			}runtime;

			TAG_FIELD(int32, quad_count, "How many quads this object has");  
			TAG_FIELD(int32, vertex_count, "How many verticies this object has"); 
			TAG_FIELD(int32, primitive_count, "Number of primitives (triangles) this quad has"); 
			TAG_FIELD(int32, x_segs, "How many quads on the X axis"); 
			TAG_FIELD(int32, y_segs, "How many quads on the Y axis");
		}; BOOST_STATIC_ASSERT( sizeof(s_shader_postprocess_effect_render_quad) == 0x1C );

		struct s_shader_postprocess_effect_definition
		{
			enum { k_group_tag = 'shpe' };
			PAD16;
			PAD16;

			TAG_FIELD(tag_string, name);

			TAG_FIELD(point2d, quad_tesselation);

			TAG_ENUM(render_stage, Enums::postprocess_render_stage);
			PAD16;

			struct _runtime {
				struct _postprocess_flags{
					TAG_FLAG16(valid_effect);
					TAG_FLAG16(is_active);
				}flags; PAD16;

				s_shader_postprocess_effect_render_quad quad;
			}runtime;

		}; BOOST_STATIC_ASSERT( sizeof(s_shader_postprocess_effect_definition) == 0x4C );
	};
};

#include <Blam/Halo1/shader_postprocess_definitions.inl>

#include <Blam/Halo1/shader_postprocess/shader_postprocess_globals_definitions.hpp>
#include <Blam/Halo1/shader_postprocess/shader_postprocess_generic_definitions.hpp>
#include <Blam/Halo1/shader_postprocess/shader_postprocess_effect_generic_definitions.hpp>
#include <Blam/Halo1/shader_postprocess/shader_postprocess_collection_definitions.hpp>