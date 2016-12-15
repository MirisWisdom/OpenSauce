/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <d3dx9.h>

#include <blamlib/Halo1/bitmaps/bitmap_group.hpp>
#include <blamlib/Halo1/cache/predicted_resources.hpp>
#include <YeloLib/tag_files/tag_groups_base_yelo.hpp>

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
			_postprocess_render_stage_blur,

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

		enum effect_activation_state : _enum
		{
			_effect_activation_state_initially_active,
			_effect_activation_state_is_in_cutscene,
			_effect_activation_state_player_is_zoomed,
			_effect_activation_state_player_using_a_vehicle,

			_effect_activation_control_state
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

			void			Initialize(LPD3DXEFFECT effect, const char* id, bool semantic = false);


			template<typename TValueType>
			void			GetValues(TValueType* values_out, const TValueType* values1, const TValueType* values2, const size_t count, const real* interp_values);
			void			GetValues(BOOL* values_out, const bool* values1, const bool* values2, const size_t count, const real* interp_values);
			void			GetValues(D3DXCOLOR* values_out, const FLOAT* values1, const FLOAT* values2, const size_t count, const real* interp_values);

			void			SetVariableInterp(LPD3DXEFFECT effect, const void* data1, const void* data2, const real* interp_values);

			// [fixup_argb_color_hack] - internal use only, use default when calling externally
			void			SetVariable(LPD3DXEFFECT effect, void* data, const bool fixup_argb_color_hack = true);
#endif
		};
		typedef s_shader_postprocess_shader_variable<Enums::_shader_variable_base_type>				t_shader_variable_base;
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
		struct s_pass_definition
		{
			TAG_FIELD(tag_string, name);

			struct _flags {
				TAG_FLAG16(clear_target);
				TAG_FLAG16(copy_scene_to_target);
				TAG_FLAG16(clear_buffer_texture);
			}flags;	BOOST_STATIC_ASSERT( sizeof(_flags) == sizeof(word_flags) );
			TAG_FIELD(_enum, render_chain);

			TAG_PAD(int32, 3);
		}; BOOST_STATIC_ASSERT( sizeof(s_pass_definition) == 0x30 );

		struct s_technique_definition
		{
			TAG_FIELD(tag_string, name);

			struct _flags {
				TAG_FLAG16(sm_1_0);
				TAG_FLAG16(sm_2_0);
				TAG_FLAG16(sm_3_0);
			}shader_model;	BOOST_STATIC_ASSERT( sizeof(_flags) == sizeof(word_flags) );

			PAD16;

			TAG_PAD(int32, 4);

			TAG_TBLOCK_(passes, s_pass_definition);
		}; BOOST_STATIC_ASSERT( sizeof(s_technique_definition) == 0x40);

		struct s_shader_postprocess_definition
		{
			enum { k_group_tag = 'shpp' };
			PAD16;
			struct _flags {
				TAG_FLAG16(shader_is_binary);
			}flags;

#if !PLATFORM_IS_EDITOR
			TAG_FIELD(tag_data, shader_code_text);
#else
			TAG_PAD(tag_data, 1);
#endif
			TAG_FIELD(tag_data, shader_code_binary);

			t_shader_variable_matrix	ortho_wvp_matrix;	// Orthographic projection matrix handle
			t_shader_variable_real2d	scene_size;			// Handle to the scene size float2 value
			t_shader_variable_real2d	screen_fov;			// Handle to the screen fov float2 value
			t_shader_variable_real2d	hud_scale;			// Handle to the hud scale float2 value
			t_shader_variable_real2d	pixel_size;			// Handle to the pixel size float2 value
			t_shader_variable_real		near_clip_dist;		// Handle to the near clip float value
			t_shader_variable_real		far_clip_dist;		// Handle to the far clip float value
			t_shader_variable_real		frame_time;			// Handle to the frame time float value
			t_shader_variable_texture	tex_source;			// Handle to the post-process source texture
			t_shader_variable_texture	tex_scene;			// Handle to the saved scene texture
			t_shader_variable_texture	tex_buffer;			// Handle to the secondary buffer

			TAG_PAD(int32, 5);

			TAG_TBLOCK_(techniques, s_technique_definition);
			TAG_TBLOCK_(predicted_resources, predicted_resource);

			struct _runtime {
				struct _postprocess_flags{
					TAG_FLAG16(valid_shader);
					TAG_FLAG16(uses_gbuffer);
				}flags; PAD16;	BOOST_STATIC_ASSERT( sizeof(_postprocess_flags) == sizeof(word_flags) );

				TAG_FIELD(LPD3DXEFFECT, dx_effect);
				TAG_FIELD(D3DXHANDLE, postprocess_handle);
				TAG_FIELD(s_technique_definition*, active_technique);
				TAG_PAD(int32, 4);
			}runtime;

#if !PLATFORM_IS_EDITOR // for externally defined shaders
			s_shader_postprocess_definition()	{}
#endif
		}; BOOST_STATIC_ASSERT( sizeof(s_shader_postprocess_definition) == 0xA4 );


		//////////////////////////////////////////////////////////////////////////
		// effect_postprocess base
		struct s_effect_postprocess_custom_vertex_data
		{
			TAG_FIELD(real_bounds, x_data);
			TAG_FIELD(real_bounds, y_data);
			TAG_FIELD(real_bounds, z_data);
			TAG_FIELD(real_bounds, w_data);
		}; BOOST_STATIC_ASSERT( sizeof(s_effect_postprocess_custom_vertex_data) == 0x20);

		struct s_effect_postprocess_quad_definition
		{
			TAG_FIELD(point2d, tessellation);
			TAG_FIELD(real_bounds, x_bounds);
			TAG_FIELD(real_bounds, y_bounds);
			TAG_TBLOCK_(custom_vertex_data, s_effect_postprocess_custom_vertex_data); // not implemented
		};

		struct s_effect_postprocess_definition
		{
			enum { k_group_tag = 'shpe' };
			TAG_PAD(byte, 12);

			TAG_FIELD(s_effect_postprocess_quad_definition, quad_definition);

			struct _runtime {
				struct _postprocess_flags{
					TAG_FLAG16(valid_effect);
					TAG_FLAG16(is_active);
					TAG_FLAG16(uses_gbuffer);
				}flags; PAD16;

				TAG_PAD(byte, 12);
			}runtime;

		}; BOOST_STATIC_ASSERT( sizeof(s_effect_postprocess_definition) == 0x3C );
	};
};

#include <YeloLib/Halo1/shaders/shader_postprocess_runtime.inl>

#include <YeloLib/Halo1/shaders/shader_postprocess_globals_definitions.hpp>
#include <YeloLib/Halo1/shaders/shader_postprocess_generic_definitions.hpp>
#include <YeloLib/Halo1/shaders/effect_postprocess_generic_definitions.hpp>
#include <YeloLib/Halo1/shaders/effect_postprocess_collection_definitions.hpp>