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
		enum shader_type
		{
			_shader_type_screen,
			_shader_type_effect,
			_shader_type_decal,

			_shader_type_environment,
			_shader_type_model,
			_shader_type_transparent_generic,
			_shader_type_transparent_chicago,
			_shader_type_transparent_chicago_extended,
			_shader_type_transparent_water,
			_shader_type_transparent_glass,
			_shader_type_transparent_meter,
			_shader_type_transparent_plasma,

			_shader_type,
		};
	};

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

			TAG_ENUM(shader_type, Enums::shader_type);
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
			struct __flags
			{
				TAG_FLAG16(alpha_tested);
				TAG_FLAG16(bump_map_is_specular_mask);
				TAG_FLAG16(true_atmospheric_fog);
			}; BOOST_STATIC_ASSERT( sizeof(__flags) == sizeof(word_flags) );

			struct __flags_1
			{
				TAG_FLAG16(rescale_detail_maps);
				TAG_FLAG16(rescale_bump_map);
			}; BOOST_STATIC_ASSERT( sizeof(__flags_1) == sizeof(word_flags) );

			struct __flags_2
			{
				TAG_FLAG16(unfiltered);
			}; BOOST_STATIC_ASSERT( sizeof(__flags_2) == sizeof(word_flags) );

			struct __flags_3
			{
				TAG_FLAG16(overbright);
				TAG_FLAG16(extra_shiny);
				TAG_FLAG16(lightmap_is_specular);
			}; BOOST_STATIC_ASSERT( sizeof(__flags_3) == sizeof(word_flags) );

			struct __flags_4
			{
				TAG_FLAG16(dynamic_mirror);
			}; BOOST_STATIC_ASSERT( sizeof(__flags_4) == sizeof(word_flags) );

			////////////////////////////////////////////////////////////////
			// environment shader
			// Setting <true atmospheric fog> enables per-pixel atmospheric fog (for models) but disables point/spot lights, planar fog, and the ability to control the atmospheric fog density for this shader.
			TAG_FIELD(__flags, flags);

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
			TAG_ENUM(type);

			////////////////////////////////////////////////////////////////
			// lens flares
			TAG_FIELD(real, lens_flare_spacing, "world units", "0 places a single lens flare");
			TAG_FIELD(tag_reference, lens_flare, "lens");
			TAG_PAD(int32, 11);

			////////////////////////////////////////////////////////////////
			// diffuse properties
			TAG_FIELD(__flags_1, flags_1);
			PAD16;
			TAG_PAD(int32, 6);
			TAG_FIELD(tag_reference, base_map, "bitm");
			TAG_PAD(int32, 6);
			TAG_ENUM(detail_map_function);
			PAD16;
			TAG_FIELD(real, primary_detail_map_scale, "", "0 defaults to 1");
			TAG_FIELD(tag_reference, primary_detail_map, "bitm");
			TAG_FIELD(real, secondary_detail_map_scale, "", "0 defaults to 1");
			TAG_FIELD(tag_reference, secondary_detail_map, "bitm");
			TAG_PAD(int32, 6);
			TAG_ENUM(micro_detail_map_function);
			PAD16;
			TAG_FIELD(real, micro_detail_map_scale, "", "0 defaults to 1");
			TAG_FIELD(tag_reference, micro_detail_map, "bitm");
			TAG_FIELD(real_rgb_color, material_color, "", "modulates incoming diffuse light, including lightmaps, but excluding self-illumination and specular effects");
			TAG_PAD(int32, 3);

			////////////////////////////////////////////////////////////////
			// bump properties
			// Perforated (alpha-tested) shaders use alpha in bump map.
			TAG_FIELD(real, bump_map_scale);
			TAG_FIELD(tag_reference, bump_map, "bitm");
			TAG_PAD(int32, 2);
			TAG_PAD(int32, 4);

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
			TAG_FIELD(__flags_2, flags_2);
			PAD16;
			TAG_PAD(int32, 6);
			s_shader_color_function color_functions[3];
			TAG_FIELD(real, map_scale, "", "0 defaults to 1");
			TAG_FIELD(tag_reference, map, "bitm");
			TAG_PAD(int32, 6);

			////////////////////////////////////////////////////////////////
			// specular properties
			// Controls dynamic specular highlights. The highlight is modulated by <brightness> as well as a blend between <perpendicular color> and <parallel color>.
			// 
			// Set <brightness> to zero to disable.
			TAG_FIELD(__flags_3, flags_3);
			PAD16;
			TAG_PAD(int32, 4);
			TAG_FIELD(real_fraction, brightness, "[0,1]", "0 is no specular hilights");
			TAG_PAD(int32, 5);
			TAG_FIELD(real_rgb_color, perpendicular_color, "", "hilight color when viewed perpendicularly");
			TAG_FIELD(real_rgb_color, parallel_color, "", "hilight color when viewed at a glancing angle");
			TAG_PAD(int32, 4);

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
			TAG_FIELD(__flags_4, flags_4);
			TAG_ENUM(type_1);
			TAG_FIELD(real_fraction, lightmap_brightness_scale, "[0,1]", "reflection brightness when lightmap brightness is 1");
			TAG_PAD(int32, 7);
			TAG_FIELD(real_fraction, perpendicular_brightness, "[0,1]", "brightness when viewed perpendicularly");
			TAG_FIELD(real_fraction, parallel_brightness, "[0,1]", "brightness when viewed at a glancing angle");
			TAG_PAD(int32, 4);
			TAG_PAD(int32, 2);
			TAG_PAD(int32, 4);
			TAG_FIELD(tag_reference, reflection_cube_map, "bitm");
			TAG_PAD(int32, 4);
		};

		struct s_shader_environment_definition : s_shader_definition
		{
			enum { k_group_tag = 'senv' };

			_shader_environment_definition environment;
		}; BOOST_STATIC_ASSERT( sizeof(s_shader_environment_definition) == 0x344 ); // max count: 1

		//////////////////////////////////////////////////////////////////////////
		struct s_shader_model_extension
		{
			struct s_map{
				tag_reference  map;
				union{
					TAG_PAD(byte, 16);
					struct{
						real multiplier; //used by all
						union{
							struct{
								PAD32;
								real power;  //only used by specular color
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
			TAG_PAD(byte, 32);    //32
		};

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
				TAG_TBLOCK_(shader_extension, s_shader_model_extension);
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
			enum { k_group_tag = 'soso' };

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