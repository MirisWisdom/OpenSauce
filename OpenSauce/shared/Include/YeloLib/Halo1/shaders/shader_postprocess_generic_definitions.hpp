/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/tag_files/tag_groups.hpp>
#include <YeloLib/Halo1/shaders/shader_postprocess_definitions.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum parameter_runtime_value_bool : _enum
		{
			_parameter_runtime_value_bool_none,
			//_parameter_runtime_value_bool_target_is_enemy, //not yet implemented
			//_parameter_runtime_value_bool_weapon_zoomed, //not yet implemented
			_parameter_runtime_value_bool
		};
		enum parameter_runtime_value_int : _enum
		{
			_parameter_runtime_value_int_none,
			_parameter_runtime_value_int
		};
		enum parameter_runtime_value_float : _enum
		{
			_parameter_runtime_value_float_none,
			//_parameter_runtime_value_float_weapon_zoom_amount, //not yet implemented
			_parameter_runtime_value_float
		};
		enum parameter_runtime_value_float2 : _enum
		{
			_parameter_runtime_value_float2_none,
			_parameter_runtime_value_float2
		};
		enum parameter_runtime_value_float3 : _enum
		{
			_parameter_runtime_value_float3_none,
			_parameter_runtime_value_float3
		};
		enum parameter_runtime_value_float4 : _enum
		{
			_parameter_runtime_value_float4_none,
			_parameter_runtime_value_float4
		};
		enum parameter_runtime_value_color : _enum
		{
			_parameter_runtime_value_color_none,
			_parameter_runtime_value_color_player_team_color,
			_parameter_runtime_value_color
		};
	};

	namespace TagGroups
	{
		union s_shader_postprocess_value_union
		{
			static const size_t k_sizeof = 36;
			TAG_PAD(byte, k_sizeof);

			struct s_base {
				t_shader_variable_base handle;
			}base;

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
				PAD(byte, 12);
				TAG_FIELD(real, upper_bound);
			}real32;

			struct s_real_vector2d {
				t_shader_variable_real2d handle;

				TAG_FIELD(real_vector2d, lower_bound);
				PAD(byte, 8);
				TAG_FIELD(real_vector2d, upper_bound);
			}vector2d;

			struct s_real_vector3d {
				t_shader_variable_real3d handle;

				TAG_FIELD(real_vector3d, lower_bound);
				PAD(byte, 4);
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

		struct s_shader_postprocess_value_runtime_override
		{
			TAG_FIELD(_enum, value);
			struct _flags {
				TAG_FLAG16(invert);
			}flags;
		};

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

			// set by tag post-processing code
			shader_variable_type value_type;

			s_shader_postprocess_value_union value;
			s_shader_postprocess_value_runtime_override runtime_value;
			s_shader_postprocess_value_animation_function animation_function;
		};

		struct s_shader_postprocess_bitmap : s_shader_postprocess_value_base
		{
			TAG_PAD(byte, 12);
			TAG_FIELD(tag_reference, bitmap, 'bitm');
		}; BOOST_STATIC_ASSERT( sizeof(s_shader_postprocess_bitmap) == 0x1C + sizeof(s_shader_postprocess_value_base) );

		struct s_shader_postprocess_parameter : s_shader_postprocess_value_base
		{
			struct
			{
				TAG_FIELD(tag_reference, bitmap);
				
				union {
					struct {
						TagGroups::s_bitmap_data* bitmap;
						PAD32;
					}_internal;	// We use a '_' prefix so intelli-sense doesn't get retarded

					struct {
						char* source;
						IDirect3DTexture9* texture_2d;
					}external;
				}runtime;

				struct {
					TAG_FLAG16(is_loaded);
					TAG_FLAG16(is_external);
				}flags; PAD16;
			}bitmap_value;

			void				GetBoundingValues(void*& lower_ref, void*& upper_ref);
			void				SetVariableInterp(LPD3DXEFFECT effect, const void* lower_data, const void* upper_data, const real* interp_values);

			// [fixup_argb_color_hack] - internal use only, use default when calling externally
			void				SetVariable(LPD3DXEFFECT effect, void* data, const bool fixup_argb_color_hack = true);

			//bitmap only functions
			HRESULT				LoadBitmap(IDirect3DDevice9* pDevice);
			void				ReleaseBitmap();

			IDirect3DTexture9*	GetTexture();

#if PLATFORM_IS_EDITOR && !PLATFORM_IS_DEDI
			void SetParameter(const s_shader_postprocess_value_base* value);
			void SetParameter(const s_shader_postprocess_bitmap* value);
#endif
		};

		struct s_shader_postprocess_implementation
		{
			TAG_TBLOCK_(bitmaps, s_shader_postprocess_bitmap);
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
			TAG_TBLOCK_(parameters, s_shader_postprocess_parameter);
			TAG_FIELD(s_shader_postprocess_implementation, implementation);
			TAG_PAD(byte, 36);

#if !PLATFORM_IS_EDITOR // for externally defined shaders
			s_shader_postprocess_generic()	{}
#endif
		}; BOOST_STATIC_ASSERT( sizeof(s_shader_postprocess_generic) == 0x44 + sizeof(s_shader_postprocess_definition) + sizeof(s_shader_postprocess_implementation) );
	};
};