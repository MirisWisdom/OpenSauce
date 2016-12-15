/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/Halo1/math/periodic_functions.hpp>
#include <blamlib/Halo1/objects/object_definitions.hpp>

#include <YeloLib/tag_files/tag_groups_base_yelo.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum shader_type
		{
			_shader_type_screen,// sscr
			_shader_type_effect,
			_shader_type_decal,	// sdec

			_shader_type_environment,
			_shader_type_model,
			_shader_type_transparent_generic,
			_shader_type_transparent_chicago,
			_shader_type_transparent_chicago_extended,
			_shader_type_transparent_water,
			_shader_type_transparent_glass,
			_shader_type_transparent_meter,
			_shader_type_transparent_plasma,

			k_number_of_shader_types,
		};
		enum environment_shader_type : _enum
		{
			_environment_shader_type_normal,
			_environment_shader_type_blended,
			_environment_shader_type_blended_base_specular,

			_environment_shader_type,
		};
		enum detail_map_function : _enum
		{
			_detail_map_function_double_biased_multiply,
			_detail_map_function_multiply,
			_detail_map_function_double_biased_add,

			_detail_map_function,
		};
		enum reflection_type : _enum
		{
			_reflection_type_bumped_cube_map,
			_reflection_type_flat_cube_map,
			_reflection_type_bumped_radiosity,

			_reflection_type,
		};
		enum detail_mask : _enum
		{
			_detail_mask_none,
			_detail_mask_reflection_mask_inverse,
			_detail_mask_reflection_mask,
			_detail_mask_self_illumination_mask_inverse,
			_detail_mask_self_illumination_mask,
			_detail_mask_change_color_mask_inverse,
			_detail_mask_change_color_mask,
			_detail_mask_multipurpose_map_alpha_inverse,
			_detail_mask_multipurpose_map_alpha,

			_detail_mask,
		};
		enum first_map_type : _enum
		{
			_first_map_type__2d_map,
			_first_map_type_first_map_is_reflection_cube_map,
			_first_map_type_first_map_is_object_centered_cube_map,
			_first_map_type_first_map_is_viewer_centered_cube_map,

			_first_map_type,
		};
		enum shader_framebuffer_blend_function : _enum
		{
			_shader_framebuffer_blend_function_alpha_blend,
			_shader_framebuffer_blend_function_multiply,
			_shader_framebuffer_blend_function_double_multiply,
			_shader_framebuffer_blend_function_add,
			_shader_framebuffer_blend_function_subtract,
			_shader_framebuffer_blend_function_component_min,
			_shader_framebuffer_blend_function_component_max,
			_shader_framebuffer_blend_function_alpha_multiply_add,

			_shader_framebuffer_blend_function,
		};
		enum shader_framebuffer_fade_mode : _enum
		{
			_shader_framebuffer_fade_mode_none,
			_shader_framebuffer_fade_mode_fade_when_perpendicular,
			_shader_framebuffer_fade_mode_fade_when_parallel,

			_shader_framebuffer_fade_mode,
		};
		enum color_function : _enum
		{
			_color_function_current,
			_color_function_next_map,
			_color_function_multiply,
			_color_function_double_multiply,
			_color_function_add,
			_color_function_add_signed_current,
			_color_function_add_signed_next_map,
			_color_function_subtract_current,
			_color_function_subtract_next_map,
			_color_function_blend_current_alpha,
			_color_function_blend_current_alpha_inverse,
			_color_function_blend_next_map_alpha,
			_color_function_blend_next_map_alpha_inverse,

			_color_function,
		};
		enum color_input : _enum
		{
			_color_input_zero,
			_color_input_one,
			_color_input_one_half,
			_color_input_negative_one,
			_color_input_negative_one_half,
			_color_input_map_color_0,
			_color_input_map_color_1,
			_color_input_map_color_2,
			_color_input_map_color_3,
			_color_input_vertex_color_0__diffuse_light,
			_color_input_vertex_color_1__fade_perpendicular,
			_color_input_scratch_color_0,
			_color_input_scratch_color_1,
			_color_input_constant_color_0,
			_color_input_constant_color_1,
			_color_input_map_alpha_0,
			_color_input_map_alpha_1,
			_color_input_map_alpha_2,
			_color_input_map_alpha_3,
			_color_input_vertex_alpha_0__fade_none,
			_color_input_vertex_alpha_1__fade_perpendicular,
			_color_input_scratch_alpha_0,
			_color_input_scratch_alpha_1,
			_color_input_constant_alpha_0,
			_color_input_constant_alpha_1,

			_color_input,
		};
		enum color_input_mapping : _enum
		{
			_color_input_mapping_clamp_x,
			_color_input_mapping__1__clamp_x,
			_color_input_mapping__2,
			_color_input_mapping__1__2,
			_color_input_mapping_clamp_x__1_2,
			_color_input_mapping__1_2__clamp_x,
			_color_input_mapping_x,
			_color_input_mapping_x_1,

			_color_input_mapping,
		};
		enum color_output : _enum
		{
			_color_output_discard,
			_color_output_scratch_color_0__final_color,
			_color_output_scratch_color_1,
			_color_output_vertex_color_0,
			_color_output_vertex_color_1,
			_color_output_map_color_0,
			_color_output_map_color_1,
			_color_output_map_color_2,
			_color_output_map_color_3,

			_color_output,
		};
		enum color_output_function : _enum
		{
			_color_output_function_multiply,
			_color_output_function_dot_product,

			_color_output_function,
		};
		enum output_mapping : _enum
		{
			_output_mapping_identity,
			_output_mapping_scale_by_1_2,
			_output_mapping_scale_by_2,
			_output_mapping_scale_by_4,
			_output_mapping_bias_by_1_2,
			_output_mapping_expand_normal,

			_output_mapping,
		};
		enum alpha_input : _enum
		{
			_alpha_input_zero,
			_alpha_input_one,
			_alpha_input_one_half,
			_alpha_input_negative_one,
			_alpha_input_negative_one_half,
			_alpha_input_map_alpha_0,
			_alpha_input_map_alpha_1,
			_alpha_input_map_alpha_2,
			_alpha_input_map_alpha_3,
			_alpha_input_vertex_alpha_0__fade_none,
			_alpha_input_vertex_alpha_1__fade_perpendicular,
			_alpha_input_scratch_alpha_0,
			_alpha_input_scratch_alpha_1,
			_alpha_input_constant_alpha_0,
			_alpha_input_constant_alpha_1,
			_alpha_input_map_blue_0,
			_alpha_input_map_blue_1,
			_alpha_input_map_blue_2,
			_alpha_input_map_blue_3,
			_alpha_input_vertex_blue_0__blue_light,
			_alpha_input_vertex_blue_1__fade_parallel,
			_alpha_input_scratch_blue_0,
			_alpha_input_scratch_blue_1,
			_alpha_input_constant_blue_0,
			_alpha_input_constant_blue_1,

			_alpha_input,
		};
		enum alpha_output : _enum
		{
			_alpha_output_discard,
			_alpha_output_scratch_alpha_0__final_alpha,
			_alpha_output_scratch_alpha_1,
			_alpha_output_vertex_alpha_0__fog,
			_alpha_output_vertex_alpha_1,
			_alpha_output_map_alpha_0,
			_alpha_output_map_alpha_1,
			_alpha_output_map_alpha_2,
			_alpha_output_map_alpha_3,

			_alpha_output,
		};

		enum shader_effect_secondary_map_anchor : _enum
		{
			_shader_effect_secondary_map_anchor_with_primary,
			_shader_effect_secondary_map_anchor_with_screen_space,
			_shader_effect_secondary_map_anchor_zsprite,

			_shader_effect_secondary_map_anchor,
		};
	};

	namespace Flags
	{
		enum shader_extension_usage : word_flags
		{
			_shader_extension_usage_none						= 0,
			_shader_extension_usage_normal_map					= 1 << 0,
			_shader_extension_usage_detail_normal				= 1 << 1,
			_shader_extension_usage_specular_map				= 1 << 2,
			_shader_extension_usage_specular_lighting			= 1 << 3,
			_shader_extension_usage_directional_lightmaps_diff	= 1 << 4,
			_shader_extension_usage_directional_lightmaps_spec	= 1 << 5,
			_shader_extension_usage_depth_fade					= 1 << 6,

			_shader_extension_usage,
		};

		enum shader_extension_usage_bit : word_flags
		{
			_shader_extension_usage_bit_normal_map,
			_shader_extension_usage_bit_detail_normal,
			_shader_extension_usage_bit_specular_map,
			_shader_extension_usage_bit_specular_lighting,
			_shader_extension_usage_bit_directional_lightmaps_diff,
			_shader_extension_usage_bit_directional_lightmaps_spec,
			_shader_extension_usage_bit_depth_fade,

			_shader_extension_usage_bit,
		};
	};

	namespace TagGroups
	{
		struct s_shader_animation_function
		{
			TAG_ENUM(u_animation_source, Enums::object_function_reference);
			TAG_ENUM(u_animation_function, Enums::periodic_function);
			TAG_FIELD(real, animation_period, "seconds", "0 defaults to 1");
			TAG_FIELD(real, animation_phase);
			TAG_FIELD(real, animation_scale, "repeats", "0 defaults to 1");
		}; BOOST_STATIC_ASSERT( sizeof(s_shader_animation_function) == 16 );
		struct s_shader_animation
		{
			s_shader_animation_function anim_u, anim_v, anim_rot;
			TAG_FIELD(real_point2d, rotation_animation_center);
		}; BOOST_STATIC_ASSERT( sizeof(s_shader_animation) == 56 );
		struct s_shader_texture_animation_function
		{
			////////////////////////////////////////////////////////////////
			// texture scrolling animation
			// Scrolls all 2D maps simultaneously.
			TAG_ENUM(animation_function, Enums::periodic_function);
			PAD16;
			TAG_FIELD(real, animation_period, "seconds");
			TAG_FIELD(real, animation_scale, "base map repeats");
		}; BOOST_STATIC_ASSERT( sizeof(s_shader_texture_animation_function) == 12 );
		struct s_shader_scaled_map
		{
			TAG_FIELD(real, scale, "", "0 defaults to 1");
			TAG_FIELD(tag_reference, map, 'bitm');
		}; BOOST_STATIC_ASSERT( sizeof(s_shader_scaled_map) == 20 );
		struct s_shader_color_function
		{
			TAG_FIELD(real_rgb_color, on_color);
			TAG_FIELD(real_rgb_color, off_color);
			TAG_ENUM(animation_function, Enums::periodic_function);
			PAD16;
			TAG_FIELD(real, animation_period, "seconds", "0 defaults to 1");
			TAG_FIELD(real, animation_phase, "seconds");
			TAG_PAD(int32, 6);
		}; BOOST_STATIC_ASSERT( sizeof(s_shader_color_function) == 60 );
		struct s_shader_shader_framebuffer_function
		{
			TAG_ENUM(shader_framebuffer_blend_function, Enums::shader_framebuffer_blend_function);
			TAG_ENUM(shader_framebuffer_fade_mode, Enums::shader_framebuffer_fade_mode);
			TAG_ENUM(shader_framebuffer_fade_source, Enums::object_function_reference, "fade is multiplied by this external value");
			PAD16;
		}; BOOST_STATIC_ASSERT( sizeof(s_shader_shader_framebuffer_function) == 8 );
		struct s_shader_radiosity_properties
		{
			struct __flags
			{
				TAG_FLAG8(alpha_tested);
				TAG_FLAG8(decal);
				TAG_FLAG8(two_sided);
				TAG_FLAG8(first_map_is_in_screenspace);
				TAG_FLAG8(draw_before_water);
				TAG_FLAG8(ignore_effect);
				TAG_FLAG8(scale_first_map_with_distance);
				TAG_FLAG8(numeric);
			}; BOOST_STATIC_ASSERT( sizeof(__flags) == sizeof(byte_flags) );

			TAG_FIELD(byte, numeric_counter_limit, "[0,255]");
			TAG_FIELD(__flags, flags);
			TAG_ENUM(first_map_type, Enums::first_map_type);
			s_shader_shader_framebuffer_function shader_framebuffer_function;
		}; BOOST_STATIC_ASSERT( sizeof(s_shader_radiosity_properties) == 12 );
		//////////////////////////////////////////////////////////////////////////
		struct _shader_definition
		{
			struct __flags
			{
				TAG_FLAG16(simple_parameterization, "lightmap texture parametrization should correspond to diffuse texture parametrization");
				TAG_FLAG16(ignore_normals, "light independent of normals (trees)");
				TAG_FLAG16(transparent_lit);
			};

			struct __flags_1
			{
				TAG_FLAG16(unused);
			}; BOOST_STATIC_ASSERT( sizeof(__flags_1) == sizeof(word_flags) );

			struct {
				////////////////////////////////////////////////////////////////
				// radiosity properties
				TAG_FIELD(__flags, flags);
				TAG_ENUM(detail_level, Enums::detail_level, "affects the density of tesselation (high means slow).");
				TAG_FIELD(real, power, "", "power of emitted light from 0 to infinity");
				TAG_FIELD(real_rgb_color, color_of_emitted_light);
				TAG_FIELD(real_rgb_color, tint_color, "", "light passing through this surface (if it's transparent) will be tinted this color.");
			}radiosity;

			////////////////////////////////////////////////////////////////
			// physics properties
			TAG_FIELD(__flags_1, flags_1);
			TAG_ENUM(material_type, Enums::material_type);

			TAG_ENUM(shader_type, Enums::shader_type);
			TAG_FIELD(_enum, extension_usage);
		}; BOOST_STATIC_ASSERT( sizeof(_shader_definition) == 0x28 );
		
		struct s_shader_definition
		{
			enum { k_group_tag = 'shdr' };

			_shader_definition shader;
		}; BOOST_STATIC_ASSERT( sizeof(_shader_definition) == 0x28 );
		//////////////////////////////////////////////////////////////////////////
		struct s_shader_effect_extension
		{
			TAG_PAD(byte, 4);

			// Depth fade
			TAG_FIELD(real, depth_fade_distance);
			TAG_FIELD(real, camera_fade_distance);
			
			TAG_PAD(tag_block, 3);
		};
		struct _shader_effect_definition
		{
			struct _flags
			{
				TAG_FLAG16(sort_bias);
				TAG_FLAG16(nonlinear_tint);
				TAG_FLAG16(dont_overdraw_fp_weapon);
			}; BOOST_STATIC_ASSERT( sizeof(_flags) == sizeof(word_flags) );
			struct _map_flags
			{
				TAG_FLAG16(unfiltered);
			}; BOOST_STATIC_ASSERT( sizeof(_map_flags) == sizeof(word_flags) );

			TAG_FIELD(_flags, flags);
			TAG_ENUM(blend_function, Enums::shader_framebuffer_blend_function);
			TAG_ENUM(fade_mode, Enums::shader_framebuffer_fade_mode);
			TAG_FIELD(_map_flags, map_flags);
			TAG_TBLOCK(shader_extension, s_shader_effect_extension);
			PAD128;

			struct {
				TAG_FIELD(tag_reference, bitmap, 'bitm');
				TAG_ENUM(anchor, Enums::shader_effect_secondary_map_anchor);
				TAG_FIELD(_map_flags, flags);
				s_shader_animation animation;
			}secondary_map;
			PAD32;
			real z_sprite_radius_scale;
			PAD128; PAD32;
		}; BOOST_STATIC_ASSERT( sizeof(_shader_effect_definition) == 0x8C );
		
		// the following tags/blocks have inline instances of this:
		// contrail, lightning_shader_block, particle, particle_system_type_particle_states_block
		// not all of those field sets expose all the available tag fields...
		struct s_shader_effect : s_shader_definition
		{
			enum { k_group_tag = 'seff' };

			_shader_effect_definition effect;
		}; BOOST_STATIC_ASSERT( sizeof(s_shader_effect) == 0xB4 );

		//////////////////////////////////////////////////////////////////////////
		struct s_shader_environment_extension
		{
			struct _flags
			{
				TAG_FLAG16(do_not_use_dlms);
			}; BOOST_STATIC_ASSERT( sizeof(_flags) == sizeof(word_flags) );

			struct __specular_color_flags
			{
				TAG_FLAG16(alpha_as_exponent_mask);
			}; BOOST_STATIC_ASSERT( sizeof(__specular_color_flags) == sizeof(word_flags) );	

			TAG_FIELD(_flags, flags);
			TAG_PAD(byte, 2);
			TAG_FIELD(real, bump_amount);

			TAG_FIELD(tag_reference, specular_color_map);
			TAG_FIELD(real, specular_color_coefficient);
			TAG_FIELD(real, specular_color_exponent);
			TAG_FIELD(__specular_color_flags, specular_color_flags);
			TAG_PAD(byte, 2);
			
			TAG_FIELD(real_fraction, perpendicular_brightness, "[0,1]", "reflection brightness when viewed perpendicularly");
			TAG_FIELD(real_rgb_color, perpendicular_tint_color, "", "reflection tint color when viewed perpendicularly");
			TAG_FIELD(real_fraction, parallel_brightness, "[0,1]", "reflection brightness when viewed at a glancing angle");
			TAG_FIELD(real_rgb_color, parallel_tint_color, "", "reflection tint color when viewed at a glancing angle");

			TAG_FIELD(real, specular_lighting_exponent);
			TAG_FIELD(real, specular_lighting_coefficient);

			TAG_PAD(tag_block, 2);
		};
		struct _shader_environment_definition
		{
			struct _flags
			{
				TAG_FLAG16(alpha_tested);
				TAG_FLAG16(bump_map_is_specular_mask);
				TAG_FLAG16(true_atmospheric_fog);
			}; BOOST_STATIC_ASSERT( sizeof(_flags) == sizeof(word_flags) );

			////////////////////////////////////////////////////////////////
			// environment shader
			// Setting <true atmospheric fog> enables per-pixel atmospheric fog (for models) but disables point/spot lights, planar fog, and the ability to control the atmospheric fog density for this shader.
			TAG_FIELD(_flags, flags);

			////////////////////////////////////////////////////////////////
			// environment shader type
			// Controls how diffuse maps are combined:
			// 
			// NORMAL:
			// Secondary detail map alpha controls blend between primary and secondary detail map. Specular mask is alpha of blended primary and secondary detail map alpha multiplied by alpha of micro detail map.
			// 
			// BLENDED:
			// Base map alpha controls blend between primary and secondary detail map. Specular mask is alpha of blended primary and secondary detail map alpha multiplied by alpha of micro detail map.
			// 
			// BLENDED BASE SPECULAR:
			// Same as BLENDED, except specular mask is alpha is base map times alpha of micro detail map.
			TAG_ENUM(type, Enums::environment_shader_type);

			////////////////////////////////////////////////////////////////
			// lens flares
			struct {
				TAG_FIELD(real, spacing, "world units", "0 places a single lens flare");
				TAG_FIELD(tag_reference, reference, 'lens');
			}lens_flare;

			TAG_TBLOCK(shader_extension, s_shader_environment_extension);
			TAG_PAD(byte, 44 - sizeof(tag_block));

			////////////////////////////////////////////////////////////////
			// diffuse properties
			struct {
				struct _flags
				{
					TAG_FLAG16(rescale_detail_maps);
					TAG_FLAG16(rescale_bump_map);
				}; BOOST_STATIC_ASSERT(sizeof(_flags) == sizeof(word_flags));

				TAG_FIELD(_flags, flags);
				PAD16;
				TAG_PAD(int32, 6);
				TAG_FIELD(tag_reference, base_map, 'bitm');
				TAG_PAD(int32, 6);
				TAG_ENUM(detail_map_function, Enums::detail_map_function, "affects primary and secondary detail maps");
				PAD16;
				s_shader_scaled_map primary_detail_map;
				s_shader_scaled_map secondary_detail_map;
				TAG_PAD(int32, 6);
				TAG_ENUM(micro_detail_map_function, Enums::detail_map_function);
				PAD16;
				s_shader_scaled_map micro_detail_map;
				TAG_FIELD(real_rgb_color, material_color, "", "modulates incoming diffuse light, including lightmaps, but excluding self-illumination and specular effects");
				TAG_PAD(int32, 3);
			}diffuse;

			////////////////////////////////////////////////////////////////
			// bump properties
			// Perforated (alpha-tested) shaders use alpha in bump map.
			struct {
				s_shader_scaled_map bump_map;
				UNKNOWN_TYPE(real); // postprocessed
				UNKNOWN_TYPE(real); // postprocessed
				TAG_PAD(int32, 4);
			}bump;

			////////////////////////////////////////////////////////////////
			// texture scrolling animation
			// Scrolls all 2D maps simultaneously.
			s_shader_texture_animation_function texture_scrolling[2];
			TAG_PAD(int32, 6);

			////////////////////////////////////////////////////////////////
			// self-illumination properties
			// There are three self-illumination effects which are added together. Each effect has an <on color>, used when the shader is active, and an <off color>, used when the shader is not active. The self-illumination map is used as follows:
			// * RED: primary mask
			// * GREEN: secondary mask
			// * BLUE: plasma mask
			// * ALPHA: plasma animation reference
			// 
			// Each effect also has an animation <function>, <period> and <phase>, used when the shader is active. The primary and secondary effects simply modulate the <on color> by the animation value to produce an animation color, and then blend between the animation color and the <off color> based on the shader's activation level, and finally modulate by the mask.
			// 
			// The plasma shader compares the animation value with the alpha channel of the map (the plasma animation reference) and produces a high value when they are similar and a dark value when they are different. This value modulates the <plasma on color> to produce a plasma animation color, and the rest proceeds just like the primary and secondary effects.
			struct {
				struct _flags
				{
					TAG_FLAG16(unfiltered);
				}; BOOST_STATIC_ASSERT(sizeof(_flags) == sizeof(word_flags));

				TAG_FIELD(_flags, flags);
				PAD16;
				TAG_PAD(int32, 6);
				s_shader_color_function color_functions[3];
				s_shader_scaled_map map;
				TAG_PAD(int32, 6);
			}self_illumination;

			////////////////////////////////////////////////////////////////
			// specular properties
			// Controls dynamic specular highlights. The highlight is modulated by <brightness> as well as a blend between <perpendicular color> and <parallel color>.
			// 
			// Set <brightness> to zero to disable.
			struct {
				struct _flags
				{
					TAG_FLAG16(overbright);
					TAG_FLAG16(extra_shiny);
					TAG_FLAG16(lightmap_is_specular);
				}; BOOST_STATIC_ASSERT(sizeof(_flags) == sizeof(word_flags));

				TAG_FIELD(_flags, flags);
				PAD16;
				TAG_PAD(int32, 4);
				TAG_FIELD(real_fraction, brightness, "[0,1]", "0 is no specular hilights");
				TAG_PAD(int32, 5);
				TAG_FIELD(real_rgb_color, perpendicular_color, "", "hilight color when viewed perpendicularly");
				TAG_FIELD(real_rgb_color, parallel_color, "", "hilight color when viewed at a glancing angle");
				TAG_PAD(int32, 4);
			}specular;

			////////////////////////////////////////////////////////////////
			// reflection properties
			// Controls environment cube map reflections. The color of the cube map is "tinted" by a blend between <perpendicular color> and <parallel color> from the SPECULAR PROPERTIES above, and then modulated by a blend between <perpendicular brightness> and <parallel brightness>.
			// 
			// BUMPED CUBE MAP:
			// This type of reflection uses the shader's bump map (if it exists) to affect the reflection, as well as the perpendicular and parallel brightness (i.e. the "fresnel" effect).
			// 
			// FLAT CUBE MAP:
			// This is the fastest type of reflection. The bump map is used to attenuate the fresnel effect, but the reflection image itself is not bumped.
			// 
			// Clear <reflection cube map> or set both <perpendicular brightness> and <parallel brightness> to zero to disable.
			struct {
				struct _flags
				{
					TAG_FLAG16(dynamic_mirror);
				}; BOOST_STATIC_ASSERT(sizeof(_flags) == sizeof(word_flags));

				TAG_FIELD(_flags, flags);
				TAG_ENUM(type, Enums::reflection_type);
				TAG_FIELD(real_fraction, lightmap_brightness_scale, "[0,1]", "reflection brightness when lightmap brightness is 1");
				TAG_PAD(int32, 7);
				TAG_FIELD(real_fraction, perpendicular_brightness, "[0,1]", "brightness when viewed perpendicularly");
				TAG_FIELD(real_fraction, parallel_brightness, "[0,1]", "brightness when viewed at a glancing angle");
				TAG_PAD(int32, 4);
				TAG_PAD(int32, 2);
				TAG_PAD(int32, 4);
				TAG_FIELD(tag_reference, cube_map, "bitm");
				TAG_PAD(int32, 4);
			}reflection;
		}; BOOST_STATIC_ASSERT( sizeof(_shader_environment_definition) == 0x31C );
		
		struct s_shader_environment_definition : s_shader_definition
		{
			enum { k_group_tag = 'senv' };

			_shader_environment_definition environment;
		}; BOOST_STATIC_ASSERT( sizeof(s_shader_environment_definition) == 0x344 ); // max count: 1
		//////////////////////////////////////////////////////////////////////////
		struct s_shader_model_extension
		{
			struct __specular_color_flags
			{
				TAG_FLAG16(alpha_as_exponent_mask);
			}; BOOST_STATIC_ASSERT( sizeof(__specular_color_flags) == sizeof(word_flags) );

			struct __diffuse_lighting_flags
			{
				TAG_FLAG16(do_not_use_dlms_bsp);
			}; BOOST_STATIC_ASSERT( sizeof(__diffuse_lighting_flags) == sizeof(word_flags) );

			struct s_map{
				tag_reference  map;
				union{
					TAG_PAD(byte, 16);
					struct{
						real coefficient; //used by all
						union{
							struct{
								PAD32;
								real exponent;  //only used by specular color
								TAG_FIELD(__specular_color_flags, flags);
							};
							struct{
								real scale;  //only used by detail normals
								real v_scale;  //only used by detail normals
							};
						};
					};
				}modifiers;
			};

			s_map	specular_color;   //32
			s_map	base_normal;   //32
			s_map	detail_normals[2];  //64
			
			TAG_FIELD(real_fraction, perpendicular_brightness, "[0,1]", "reflection brightness when viewed perpendicularly");
			TAG_FIELD(real_rgb_color, perpendicular_tint_color, "", "reflection tint color when viewed perpendicularly");
			TAG_FIELD(real_fraction, parallel_brightness, "[0,1]", "reflection brightness when viewed at a glancing angle");
			TAG_FIELD(real_rgb_color, parallel_tint_color, "", "reflection tint color when viewed at a glancing angle");
			
			TAG_FIELD(__diffuse_lighting_flags, diffuse_lighting_flags);
			PAD16;
			PAD32;

			TAG_FIELD(real, specular_lighting_exponent);
			TAG_FIELD(real, specular_lighting_coefficient);

			TAG_PAD(byte, 16);
		};
		struct _shader_model_definition
		{
			struct __flags
			{
				TAG_FLAG16(detail_after_reflection);
				TAG_FLAG16(two_sided);
				TAG_FLAG16(not_alpha_tested);
				TAG_FLAG16(alpha_blended_decal);
				TAG_FLAG16(true_atmospheric_fog);
				TAG_FLAG16(disable_two_sided_culling);
			}; BOOST_STATIC_ASSERT( sizeof(__flags) == sizeof(word_flags) );

			struct __flags_1
			{
				TAG_FLAG16(no_random_phase);
			}; BOOST_STATIC_ASSERT( sizeof(__flags_1) == sizeof(word_flags) );

			////////////////////////////////////////////////////////////////
			// model shader
			// Setting <true atmospheric fog> enables per-pixel atmospheric fog but disables point/spot lights, planar fog, and the ability to control the atmospheric fog density for this shader.
			TAG_FIELD(__flags, flags);
			PAD16;
			TAG_PAD(int32, 3);
			TAG_FIELD(real_fraction, translucency, "", "amount of light that can illuminate the shader from behind");
			TAG_PAD(int32, 4);

			////////////////////////////////////////////////////////////////
			// change color
			// Change color is used to recolor the diffuse map, it affects pixels based on the BLUE channel of the multipurpose map.
			TAG_ENUM(change_color_source, Enums::object_change_color_reference);
			PAD16;
			TAG_PAD(int32, 7);

			struct {
				////////////////////////////////////////////////////////////////
				// self-illumination
				// Self-illumination adds diffuse light to pixels based on the GREEN channel of the multipurpose map. The external self-illumination color referenced by <color source> is modulated by the self-illumination animation.
				TAG_FIELD(__flags_1, flags_1);
				PAD16;
				TAG_ENUM(color_source, Enums::object_change_color_reference);
				TAG_ENUM(animation_function, Enums::periodic_function);
				TAG_FIELD(real, animation_period, "seconds");
				TAG_FIELD(real_rgb_color, animation_color_lower_bound);
				TAG_FIELD(real_rgb_color, animation_color_upper_bound);
				TAG_PAD(int32, 3);
			}self_illumination;

			struct { // 0x9C
				////////////////////////////////////////////////////////////////
				// maps
				// Base map alpha is used for alpha-testing.
				// 
				// Multipurpose map is used for the following:
				// * RED: specular reflection mask (modulates reflections)
				// * GREEN: self-illumination mask (adds to diffuse light)
				// * BLUE: primary change color mask (recolors diffuse map)
				// * ALPHA: auxiliary mask
				// 
				// Note that when DXT1 compressed color-key textures are used for the multipurpose map (as they should be normally), the alpha channel is 1-bit and any non-zero alpha pixels must have zero-color, therefore the secondary change color mask cannot affect pixels already affected by any of the other channels.
				// 
				// Detail map affects diffuse map, and optionally affects reflection if <detail after reflection> flag is set.
				TAG_FIELD(real, map_u_scale, "", "0 defaults to 1; scales all 2D maps simultaneously");
				TAG_FIELD(real, map_v_scale, "", "0 defaults to 1; scales all 2D maps simultaneously");
				TAG_FIELD(tag_reference, base_map, 'bitm');
				TAG_PAD(int32, 2);
				TAG_FIELD(tag_reference, multipurpose_map, 'bitm');
				TAG_PAD(int32, 2);
				TAG_ENUM(detail_function, Enums::detail_function, "controls how detail map is applied to diffuse map");
				TAG_ENUM(detail_mask, Enums::detail_mask, "controls how detail map is masked");
				s_shader_scaled_map detail_map;
				TAG_FIELD(real, detail_map_v_scale, "", "0 defaults to 1 (applied on top of detail map scale above)");
				TAG_TBLOCK(shader_extension, s_shader_model_extension);
				s_shader_animation animation;
				TAG_PAD(int32, 2);
			}maps;

			struct {
				////////////////////////////////////////////////////////////////
				// reflection properties
				TAG_FIELD(real, reflection_falloff_distance, "world units", "distance at which the reflection begins to fade out");
				TAG_FIELD(real, reflection_cutoff_distance, "world units", "distance at which the reflection fades out entirely (0 means no cutoff)");
				TAG_FIELD(real_fraction, perpendicular_brightness, "[0,1]", "reflection brightness when viewed perpendicularly");
				TAG_FIELD(real_rgb_color, perpendicular_tint_color, "", "reflection tint color when viewed perpendicularly");
				TAG_FIELD(real_fraction, parallel_brightness, "[0,1]", "reflection brightness when viewed at a glancing angle");
				TAG_FIELD(real_rgb_color, parallel_tint_color, "", "reflection tint color when viewed at a glancing angle");
				TAG_FIELD(tag_reference, reflection_cube_map, 'bitm');
			}reflection_properties;

			TAG_PAD(int32, 4);
			PAD32;
			TAG_PAD(int32, 4);
			TAG_PAD(int32, 8);			
		}; BOOST_STATIC_ASSERT( sizeof(_shader_model_definition) == 0x190 );
		
		struct s_shader_model_definition : s_shader_definition
		{
			enum { k_group_tag = 'soso' };

			_shader_model_definition model;
		}; BOOST_STATIC_ASSERT( sizeof(s_shader_model_definition) == 0x1B8 ); // max count: 1
		//////////////////////////////////////////////////////////////////////////
		struct s_shader_transparent_layer
		{
			TAG_FIELD(tag_reference, shader, 'shdr');
		}; BOOST_STATIC_ASSERT( sizeof(s_shader_transparent_layer) == 0x10 ); // max count: 4
		struct s_shader_transparent_chicago_map
		{
			struct __flags
			{
				TAG_FLAG16(unfiltered);
				TAG_FLAG16(alpha_replicate);
				TAG_FLAG16(u_clamped);
				TAG_FLAG16(v_clamped);
			}; BOOST_STATIC_ASSERT( sizeof(__flags) == sizeof(word_flags) );

			TAG_FIELD(__flags, flags);
			PAD16;
			TAG_PAD(int32, 10);
			TAG_ENUM(color_function, Enums::color_function, "ignored for last map");
			TAG_ENUM(alpha_function, Enums::color_function, "ignored for last map");
			TAG_PAD(int32, 9);
			TAG_FIELD(real, map_u_scale, "", "0 defaults to 1");
			TAG_FIELD(real, map_v_scale, "", "0 defaults to 1");
			TAG_FIELD(real, map_u_offset);
			TAG_FIELD(real, map_v_offset);
			TAG_FIELD(real, map_rotation, "degrees");
			TAG_FIELD(real_fraction, mipmap_bias, "[0,1]");
			TAG_FIELD(tag_reference, map, 'bitm');
			TAG_PAD(int32, 10);

			////////////////////////////////////////////////////////////////
			// 2D texture animation
			s_shader_animation map_animation;
		}; BOOST_STATIC_ASSERT( sizeof(s_shader_transparent_chicago_map) == 0xDC ); // max count: 4
		struct s_shader_transparent_chicago_base
		{
			struct __extra_flags
			{
				TAG_FLAG(don_t_fade_active_camouflage);
				TAG_FLAG(numeric_countdown_timer);
			}; BOOST_STATIC_ASSERT( sizeof(__extra_flags) == sizeof(long_flags) );

			////////////////////////////////////////////////////////////////
			// chicago shader
			s_shader_radiosity_properties radiosity_properties;

			////////////////////////////////////////////////////////////////
			// lens flares
			TAG_FIELD(real, lens_flare_spacing, "world units", "0 places a single lens flare");
			TAG_FIELD(tag_reference, lens_flare, 'lens');
		};
		//////////////////////////////////////////////////////////////////////////
		struct _shader_transparent_chicago_definition : s_shader_transparent_chicago_base
		{
			TAG_TBLOCK(extra_layers, s_shader_transparent_layer);
			TAG_TBLOCK(maps, s_shader_transparent_chicago_map);
			TAG_FIELD(__extra_flags, extra_flags);
			TAG_PAD(int32, 2);
		};
		
		struct s_shader_transparent_chicago_definition : s_shader_definition
		{
			enum { k_group_tag = 'schi' };

			_shader_transparent_chicago_definition transparent_chicago;
		}; BOOST_STATIC_ASSERT( sizeof(s_shader_transparent_chicago_definition) == 0x6C ); // max count: 1
		//////////////////////////////////////////////////////////////////////////
		struct _shader_transparent_chicago_extended_definition : s_shader_transparent_chicago_base
		{
			TAG_TBLOCK(extra_layers, s_shader_transparent_layer);
			TAG_TBLOCK(_4_stage_maps, s_shader_transparent_chicago_map);
			TAG_TBLOCK(_2_stage_maps, s_shader_transparent_chicago_map);
			TAG_FIELD(__extra_flags, extra_flags);
			TAG_PAD(int32, 2);
		};
		
		struct s_shader_transparent_chicago_extended_definition : s_shader_definition
		{
			enum { k_group_tag = 'scex' };

			_shader_transparent_chicago_extended_definition transparent_chicago_extended;
		}; BOOST_STATIC_ASSERT( sizeof(s_shader_transparent_chicago_extended_definition) == 0x78 ); // max count: 1
		//////////////////////////////////////////////////////////////////////////
		struct s_shader_transparent_generic_map
		{
			struct __flags
			{
				TAG_FLAG16(unfiltered);
				TAG_FLAG16(u_clamped);
				TAG_FLAG16(v_clamped);
			}; BOOST_STATIC_ASSERT( sizeof(__flags) == sizeof(word_flags) );

			TAG_FIELD(__flags, flags);
			PAD16;
			TAG_FIELD(real, map_u_scale, "", "0 defaults to 1");
			TAG_FIELD(real, map_v_scale, "", "0 defaults to 1");
			TAG_FIELD(real, map_u_offset);
			TAG_FIELD(real, map_v_offset);
			TAG_FIELD(real, map_rotation, "degrees");
			TAG_FIELD(real_fraction, mipmap_bias, "[0,1]");
			TAG_FIELD(tag_reference, map, 'bitm');

			////////////////////////////////////////////////////////////////
			// 2D texture animation
			s_shader_animation map_animation;
		}; BOOST_STATIC_ASSERT( sizeof(s_shader_transparent_generic_map) == 0x64 ); // max count: 4
		struct s_shader_transparent_generic_stage
		{
			struct __flags
			{
				TAG_FLAG16(color_mux);
				TAG_FLAG16(alpha_mux);
				TAG_FLAG16(a_out_controls_color0_animation);
			}; BOOST_STATIC_ASSERT( sizeof(__flags) == sizeof(word_flags) );

			TAG_FIELD(__flags, flags);
			PAD16;

			////////////////////////////////////////////////////////////////
			// constants and animation
			// Constant color 0 is animated in exactly the same way as the self-illumination color of the model shader, except that it has an alpha component in addition to the RGB components. Constant color 1 is just a constant.
			TAG_ENUM(color0_source, Enums::object_change_color_reference);
			TAG_ENUM(color0_animation_function, Enums::periodic_function);
			TAG_FIELD(real, color0_animation_period, "seconds", "0 defaults to 1");
			TAG_FIELD(real_argb_color, color0_animation_lower_bound);
			TAG_FIELD(real_argb_color, color0_animation_upper_bound);
			TAG_FIELD(real_argb_color, color1);

			////////////////////////////////////////////////////////////////
			// color inputs
			struct{
				TAG_ENUM(input, Enums::color_input);
				TAG_ENUM(input_mapping, Enums::color_input_mapping);
			}color_inputs[4];

			////////////////////////////////////////////////////////////////
			// color outputs
			struct{
				TAG_ENUM(output, Enums::color_output);
				TAG_ENUM(output_function, Enums::color_output_function);
			}color_outputs[2];
			TAG_ENUM(output_ab_cd_mux_sum, Enums::color_output);
			TAG_ENUM(output_mapping, Enums::output_mapping);

			////////////////////////////////////////////////////////////////
			// alpha inputs
			struct{
				TAG_ENUM(input_, Enums::alpha_input);
				TAG_ENUM(input_mapping, Enums::color_input_mapping);
			}alpha_inputs[4];

			////////////////////////////////////////////////////////////////
			// alpha outputs
			struct{
				TAG_ENUM(output, Enums::alpha_output);
			}alpha_outputs[2];
			TAG_ENUM(output_ab_cd_mux_sum_1, Enums::alpha_output);
			TAG_ENUM(output_mapping_1, Enums::output_mapping);
		}; BOOST_STATIC_ASSERT( sizeof(s_shader_transparent_generic_stage) == 0x70 ); // max count: 7
		struct _shader_transparent_generic_definition
		{
			////////////////////////////////////////////////////////////////
			// generic transparent shader
			s_shader_radiosity_properties radiosity_properties;

			////////////////////////////////////////////////////////////////
			// lens flares
			TAG_FIELD(real, lens_flare_spacing, "world units", "0 places a single lens flare");
			TAG_FIELD(tag_reference, lens_flare, 'lens');
			TAG_TBLOCK(extra_layers, s_shader_transparent_layer);
			TAG_TBLOCK(maps, s_shader_transparent_generic_map);
			TAG_TBLOCK(stages, s_shader_transparent_generic_stage);
		};
		
		struct s_shader_transparent_generic_definition : s_shader_definition
		{
			enum { k_group_tag = 'sotr' };

			_shader_transparent_generic_definition transparent_generic;
		}; BOOST_STATIC_ASSERT( sizeof(s_shader_transparent_generic_definition) == 0x6C ); // max count: 1
		//////////////////////////////////////////////////////////////////////////
		struct _shader_transparent_glass_definition
		{
			struct __flags
			{
				TAG_FLAG16(alpha_tested);
				TAG_FLAG16(decal);
				TAG_FLAG16(two_sided);
				TAG_FLAG16(bump_map_is_specular_mask);
			}; BOOST_STATIC_ASSERT( sizeof(__flags) == sizeof(word_flags) );

			////////////////////////////////////////////////////////////////
			// glass shader
			TAG_FIELD(__flags, flags);
			PAD16;

			////////////////////////////////////////////////////////////////
			// background tint properties
			// Background pixels are multiplied by the tint map and constant tint color.
			TAG_PAD(int32, 10);
			TAG_FIELD(real_rgb_color, background_tint_color);
			s_shader_scaled_map background_tint_map;
			TAG_PAD(int32, 5);

			////////////////////////////////////////////////////////////////
			// reflection properties
			// Reflection maps are multiplied by fresnel terms (glancing angles cause reflections to disappear) and then added to the background. The primary reflection map is textured normally, while the secondary reflection map is magnified.
			PAD16;
			TAG_ENUM(reflection_type, Enums::reflection_type);
			TAG_FIELD(real_fraction, perpendicular_brightness, "[0,1]");
			TAG_FIELD(real_rgb_color, perpendicular_tint_color);
			TAG_FIELD(real_fraction, parallel_brightness, "[0,1]");
			TAG_FIELD(real_rgb_color, parallel_tint_color);
			TAG_FIELD(tag_reference, reflection_map, "bitm");
			s_shader_scaled_map bump_map;
			TAG_PAD(int32, 32);

			////////////////////////////////////////////////////////////////
			// diffuse properties
			// Diffuse lights are accumulated in monochrome and then alpha-blended with diffuse map and diffuse detail map. The color is determined by double-multiplying both maps and multiplying with the accumulated light, the result being alpha-blended into the framebuffer. The opacity is determined by multiplying both map's alpha channels. Since this effect is alpha-blended, it covers up tinting and reflection on pixels with high opacity.
			PAD32;
			s_shader_scaled_map diffuse_map;
			s_shader_scaled_map diffuse_detail_map;
			TAG_PAD(int32, 7);

			////////////////////////////////////////////////////////////////
			// specular properties
			// Specular lights are accumulated in monochrome and then alpha-blended with diffuse map and diffuse detail map. The color is determined by double-multiplying both maps and multiplying with the accumulated light, the result being alpha-blended into the framebuffer. The opacity is determined by multiplying both map's alpha channels. Since this effect is alpha-blended, it covers up tinting, reflection and diffuse texture on pixels with high opacity.
			PAD32;
			s_shader_scaled_map specular_map;
			s_shader_scaled_map specular_detail_map;
			TAG_PAD(int32, 7);
		};
		
		struct s_shader_transparent_glass_definition : s_shader_definition
		{
			enum { k_group_tag = 'sgla' };

			_shader_transparent_glass_definition transparent_glass;
		}; BOOST_STATIC_ASSERT( sizeof(s_shader_transparent_glass_definition) == 0x1E0 ); // max count: 1
		//////////////////////////////////////////////////////////////////////////
		struct _shader_transparent_meter_definition
		{				
			struct __flags
			{
				TAG_FLAG16(decal);
				TAG_FLAG16(two_sided);
				TAG_FLAG16(flash_color_is_negative);
				TAG_FLAG16(tint_mode_2);
				TAG_FLAG16(unfiltered);
			}; BOOST_STATIC_ASSERT( sizeof(__flags) == sizeof(word_flags) );	

			////////////////////////////////////////////////////////////////
			// meter shader
			TAG_FIELD(__flags, flags);
			PAD16;
			TAG_PAD(int32, 8);
			TAG_FIELD(tag_reference, map, 'bitm');
			TAG_PAD(int32, 8);

			////////////////////////////////////////////////////////////////
			// colors
			TAG_FIELD(real_rgb_color, gradient_min_color);
			TAG_FIELD(real_rgb_color, gradient_max_color);
			TAG_FIELD(real_rgb_color, background_color);
			TAG_FIELD(real_rgb_color, flash_color);
			TAG_FIELD(real_rgb_color, tint_color, "", "modulates framebuffer color unless map alpha is zero");
			TAG_FIELD(real_fraction, meter_transparency, "[0,1]", "used only when 'tint mode-2' is set");
			TAG_FIELD(real_fraction, background_transparency, "[0,1]", "used only when 'tint mode-2' is set");
			TAG_PAD(int32, 6);

			////////////////////////////////////////////////////////////////
			// external function sources
			TAG_ENUM(meter_brightness_source, Enums::object_function_reference, "overall meter brightness (default is 1)");
			TAG_ENUM(flash_brightness_source, Enums::object_function_reference, "brightness of flash (default is 1)");
			TAG_ENUM(value_source, Enums::object_function_reference, "position of flash leading edge (default is 1)");
			TAG_ENUM(gradient_source, Enums::object_function_reference, "high color leading edge (default is 1)");
			TAG_ENUM(flash_extension_source, Enums::object_function_reference, "position of flash extension leading edge (default is 1)");
			PAD16;
			TAG_PAD(int32, 8);
		};
		
		struct s_shader_transparent_meter_definition : s_shader_definition
		{
			enum { k_group_tag = 'smet' };

			_shader_transparent_meter_definition transparent_meter;
		}; BOOST_STATIC_ASSERT( sizeof(s_shader_transparent_meter_definition) == 0x104 ); // max count: 1
		//////////////////////////////////////////////////////////////////////////
		struct _shader_transparent_plasma_definition
		{	
			////////////////////////////////////////////////////////////////
			// plasma shader
			PAD16;
			PAD16;

			////////////////////////////////////////////////////////////////
			// intensity
			// Controls how bright the plasma is.
			TAG_ENUM(intensity_source, Enums::object_function_reference);
			PAD16;
			TAG_FIELD(real, intensity_exponent, "", "0 defaults to 1");

			////////////////////////////////////////////////////////////////
			// offset
			// Controls how far the plasma energy extends from the model geometry.
			TAG_ENUM(offset_source, Enums::object_function_reference);
			PAD16;
			TAG_FIELD(real, offset_amount, "world units");
			TAG_FIELD(real, offset_exponent, "", "0 defaults to 1");
			TAG_PAD(int32, 8);

			////////////////////////////////////////////////////////////////
			// color
			// Controls the tint color and Fresnel brightness effects.
			TAG_FIELD(real_fraction, perpendicular_brightness, "[0,1]");
			TAG_FIELD(real_rgb_color, perpendicular_tint_color);
			TAG_FIELD(real_fraction, parallel_brightness, "[0,1]");
			TAG_FIELD(real_rgb_color, parallel_tint_color);
			TAG_ENUM(tint_color_source, Enums::object_change_color_reference, "modulates perpendicular and parallel colors above");
			PAD16;
			TAG_PAD(int32, 8);
			PAD16;
			PAD16;
			TAG_PAD(int32, 4);

			////////////////////////////////////////////////////////////////
			// primary noise map
			PAD32;
			PAD32;
			TAG_FIELD(real, primary_animation_period, "seconds");
			TAG_FIELD(real_vector3d, primary_animation_direction);
			s_shader_scaled_map primary_noise_map;
			TAG_PAD(int32, 8);

			////////////////////////////////////////////////////////////////
			// secondary noise map
			PAD32;
			TAG_FIELD(real, secondary_animation_period, "seconds");
			TAG_FIELD(real_vector3d, secondary_animation_direction);
			s_shader_scaled_map secondary_noise_map;
			TAG_PAD(int32, 8);
		};
		
		struct s_shader_transparent_plasma_definition : s_shader_definition
		{
			enum { k_group_tag = 'spla' };

			_shader_transparent_plasma_definition transparent_plasma;
		}; BOOST_STATIC_ASSERT( sizeof(s_shader_transparent_plasma_definition) == 0x14C ); // max count: 1
		//////////////////////////////////////////////////////////////////////////
		struct s_shader_transparent_water_ripple
		{
			PAD16;
			PAD16;
			TAG_FIELD(real_fraction, contribution_factor, "[0,1]", "0 defaults to 1");
			TAG_PAD(int32, 8);
			TAG_FIELD(angle, animation_angle, "[0,360]");
			TAG_FIELD(real, animation_velocity);
			TAG_FIELD(real_vector2d, map_offset);
			TAG_FIELD(int16, map_repeats, "", "0 defaults to 1");
			TAG_FIELD(int16, map_index, "", "index into ripple maps");
			TAG_PAD(int32, 4);
		}; BOOST_STATIC_ASSERT( sizeof(s_shader_transparent_water_ripple) == 0x4C ); // max count: 4
		struct _shader_transparent_water_definition
		{
			struct __flags
			{
				TAG_FLAG16(base_map_alpha_modulates_reflection);
				TAG_FLAG16(base_map_color_modulates_background);
				TAG_FLAG16(atmospheric_fog);
				TAG_FLAG16(draw_before_fog);
			}; BOOST_STATIC_ASSERT( sizeof(__flags) == sizeof(word_flags) );

			////////////////////////////////////////////////////////////////
			// water shader
			// Base map color modulates the background, while base map alpha modulates reflection brightness. Both of these effects can be independently enables and disabled. Note that if the <base map alpha modulates reflection> flag is not set, then the perpendicular/parallel brightness has no effect (but the perpendicular/parallel tint color DOES has an effect).
			TAG_FIELD(__flags, flags);
			PAD16;
			TAG_PAD(int32, 8);
			TAG_FIELD(tag_reference, base_map, 'bitm');
			TAG_PAD(int32, 4);
			TAG_FIELD(real_fraction, view_perpendicular_brightness, "[0,1]");
			TAG_FIELD(real_rgb_color, view_perpendicular_tint_color);
			TAG_FIELD(real_fraction, view_parallel_brightness, "[0,1]", "0 defaults to 1");
			TAG_FIELD(real_rgb_color, view_parallel_tint_color);
			TAG_PAD(int32, 4);
			TAG_FIELD(tag_reference, reflection_map, 'bitm');
			TAG_PAD(int32, 4);
			TAG_FIELD(angle, ripple_animation_angle, "[0,360]");
			TAG_FIELD(real, ripple_animation_velocity);
			TAG_FIELD(real, ripple_scale, "", "0 defaults to 1");
			TAG_FIELD(tag_reference, ripple_maps, 'bitm');
			TAG_FIELD(int16, ripple_mipmap_levels, "", "0 defaults to 1");
			PAD16;
			TAG_FIELD(real_fraction, ripple_mipmap_fade_factor, "[0,1]", "flatness of last mipmap");
			TAG_FIELD(real, ripple_mipmap_detail_bias);
			TAG_PAD(int32, 16);
			TAG_TBLOCK(ripples, s_shader_transparent_water_ripple);
			TAG_PAD(int32, 4);
		};
		
		struct s_shader_transparent_water_definition : s_shader_definition
		{
			enum { k_group_tag = 'swat' };

			_shader_transparent_water_definition transparent_water;
		}; BOOST_STATIC_ASSERT( sizeof(s_shader_transparent_water_definition) == 0x140 ); // max count: 1
		//////////////////////////////////////////////////////////////////////////
	};
};