/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <TagGroups/Halo1/TagGroupDefinitions.hpp>
#include <TagGroups/Halo1/global_definitions.hpp>
#include <TagGroups/Halo1/shader_definitions.hpp>

#include <blamlib/Halo1/interface/hud_definitions.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum hud_multitexture_overlay_effector_type : _enum
		{
			_hud_multitexture_overlay_effector_type_tint,
			_hud_multitexture_overlay_effector_type_horizontal_offset,
			_hud_multitexture_overlay_effector_type_vertical_offset,
			_hud_multitexture_overlay_effector_type_fade,

			_hud_multitexture_overlay_effector_type,
		};
		enum hud_multitexture_overlay_effector_destination : _enum
		{
			_hud_multitexture_overlay_effector_destination_geometry_offset,
			_hud_multitexture_overlay_effector_destination_primary_map,
			_hud_multitexture_overlay_effector_destination_secondary_map,
			_hud_multitexture_overlay_effector_destination_tertiary_map,

			_hud_multitexture_overlay_effector_destination,
		};
		enum hud_multitexture_overlay_effector_source : _enum
		{
			_hud_multitexture_overlay_effector_source_player_pitch,
			_hud_multitexture_overlay_effector_source_player_pitch_tangent,
			_hud_multitexture_overlay_effector_source_player_yaw,
			_hud_multitexture_overlay_effector_source_weapon_ammo_total,
			_hud_multitexture_overlay_effector_source_weapon_ammo_loaded,
			_hud_multitexture_overlay_effector_source_weapon_heat,
			_hud_multitexture_overlay_effector_source_explicit_uses_low_bound,
			_hud_multitexture_overlay_effector_source_weapon_zoom_level,

			_hud_multitexture_overlay_effector_source,
		};

		enum hud_multitexture_overlay_anchor : _enum
		{
			_hud_multitexture_overlay_anchor_texture,
			_hud_multitexture_overlay_anchor_screen,

			_hud_multitexture_overlay_anchor,
		};
		enum hud_multitexture_overlay_blend_mode : _enum
		{
			_hud_multitexture_overlay_blend_mode_add,
			_hud_multitexture_overlay_blend_mode_subtract,
			_hud_multitexture_overlay_blend_mode_multiply,
			_hud_multitexture_overlay_blend_mode_multiply2x,
			_hud_multitexture_overlay_blend_mode_dot,

			_hud_multitexture_overlay_blend_mode,
		};
		enum hud_multitexture_overlay_address : _enum
		{
			_hud_multitexture_overlay_address_clamp,
			_hud_multitexture_overlay_address_wrap,

			_hud_multitexture_overlay_address,
		};

		enum auxilary_overlay_type : _enum
		{
			_auxilary_overlay_type_team_icon,

			_auxilary_overlay_type,
		};
		enum hud_auxilary_meter_type : _enum
		{
			_hud_auxilary_meter_type_integrated_light,

			_hud_auxilary_meter_type,
		};


		enum weapon_hud_state
		{
			_weapon_hud_state_total_ammo,
			_weapon_hud_state_loaded_ammo,
			_weapon_hud_state_heat,
			_weapon_hud_state_age,
			_weapon_hud_state_secondary_weapon_total_ammo,
			_weapon_hud_state_secondary_weapon_loaded_ammo,
			_weapon_hud_state_dist_to_target,
			_weapon_hud_state_elevation_to_target,
			_weapon_hud_state,
		};
	};

	namespace Flags
	{
		enum hud_flash_flags : word_flags
		{
			_hud_flash_reverse_colors_bit, // reverse default/flashing
		};

		enum hud_meter_flags : byte_flags
		{
			_hud_meter_use_min_max_for_state_changes_bit,
			_hud_meter_interpolate_between_min_max_flash_colors_as_state_changes_bit,
			_hud_meter_interpolate_color_along_hsv_space_bit,
			_hud_meter_more_colors_for_hsv_interpolation_bit,
			_hud_meter_invert_interpolation_bit,
		};

		enum hud_auxilary_overlay_flags : word_flags
		{
			_hud_auxilary_overlay_use_team_color_bit,
		};
		enum hud_auxilary_panel_flags : long_flags
		{
			_hud_auxilary_panel_show_only_when_active_bit,
			_hud_auxilary_panel_flash_once_if_activated_while_disabled_bit,
		};

		enum hud_sound_flags : long_flags
		{
			_hud_sound_shield_recharging_bit,
			_hud_sound_shield_damaged_bit,
			_hud_sound_shield_low_bit,
			_hud_sound_shield_empty_bit,
			_hud_sound_health_low_bit,
			_hud_sound_health_empty_bit,
			_hud_sound_health_minor_damage_bit,
			_hud_sound_health_major_damage_bit,
		};
	};

	namespace TagGroups
	{
		struct s_hud_color_flash // aka global_hud_color
		{
			TAG_FIELD(argb_color, default_color);
			TAG_FIELD(argb_color, flashing_color);
			TAG_FIELD(real, flash_period);
			TAG_FIELD(real, flash_delay);
			TAG_FIELD(int16, number_of_flashes);
			TAG_FIELD(Flags::hud_flash_flags, flash_flags);
			TAG_FIELD(real, flash_length);
			TAG_FIELD(argb_color, disabled_color);
			PAD32;
		}; BOOST_STATIC_ASSERT( sizeof(s_hud_color_flash) == 0x20 );

		//////////////////////////////////////////////////////////////////////////
		// generic hud elements
		struct s_hud_element_number : public s_hud_element // aka global_hud_element
		{
			s_hud_color_flash color;
		}; BOOST_STATIC_ASSERT( sizeof(s_hud_element_number) == 0x44 );

		struct s_hud_element_static : public s_hud_element // aka global_hud_interface_element
		{
			TAG_FIELD(tag_reference, interface_bitmap, 'bitm');
			s_hud_color_flash color;
		}; BOOST_STATIC_ASSERT( sizeof(s_hud_element_static) == 0x54 );

		struct s_hud_element_meter : public s_hud_element // aka global_hud_meter
		{
			TAG_FIELD(tag_reference, interface_bitmap, 'bitm');
			TAG_FIELD(rgb_color, color_at_meter_minimum);
			TAG_FIELD(rgb_color, color_at_meter_maximum);
			TAG_FIELD(rgb_color, flash_color);
			TAG_FIELD(argb_color, empty_color);
			TAG_FIELD(Flags::hud_meter_flags, flags);
			TAG_FIELD(byte, minumum_meter_value);
			TAG_FIELD(int16, sequence_index);
			TAG_FIELD(byte, alpha_multiplier);
			TAG_FIELD(byte, alpha_bias);
			TAG_FIELD(int16, value_scale, "", "used for non-integral values, i.e. health and shields");
			TAG_FIELD(real, opacity);
			TAG_FIELD(real, translucency);
			TAG_FIELD(argb_color, disabled_color);
			TAG_PAD(tag_block, 1);
			TAG_PAD(int32, 1);
		}; BOOST_STATIC_ASSERT( sizeof(s_hud_element_meter) == 0x68 );


		//////////////////////////////////////////////////////////////////////////
		// hud overlays
		struct multitexture_overlay_hud_element_effector_definition // aka global_hud_multitexture_overlay_effector_definition
		{
			TAG_PAD(int32, 16);

			////////////////////////////////////////////////////////////////
			// source/destination
			// These describe the relationship that causes the effect.
			// * destination type is the type of variable you want to be effected
			// * destination tells which texture map (or geometry offset) to apply it to
			// * source says which value to look at when computing the effect
			TAG_ENUM(destination_type, Enums::hud_multitexture_overlay_effector_type);
			TAG_ENUM(destination, Enums::hud_multitexture_overlay_effector_destination);
			TAG_ENUM(source, Enums::hud_multitexture_overlay_effector_source);
			PAD16;

			////////////////////////////////////////////////////////////////
			// in/out bounds
			// When the source is at the lower inbound, the destination ends up the lower outbound and vice-versa applies for the upper values.
			TAG_FIELD(real_bounds, in_bounds, "source units");
			TAG_FIELD(real_bounds, out_bounds, "pixels");
			TAG_PAD(int32, 16);

			////////////////////////////////////////////////////////////////
			// tint color bounds
			// If destination is tint, these values are used instead of the out bounds.
			TAG_FIELD(real_rgb_color, tint_color_lower_bound);
			TAG_FIELD(real_rgb_color, tint_color_upper_bound);

			////////////////////////////////////////////////////////////////
			// periodic functions
			// If you use a periodic function as the source, this lets you tweak it.
			TAG_ENUM(periodic_function, Enums::periodic_function);
			PAD16;
			TAG_FIELD(real, function_period, "seconds");
			TAG_FIELD(real, function_phase, "seconds");
			TAG_PAD(int32, 8);
		}; BOOST_STATIC_ASSERT( sizeof(multitexture_overlay_hud_element_effector_definition) == 0xDC ); // max count: 30
		struct multitexture_overlay_hud_element_definition // aka 'global_hud_multitexture_overlay_definition'
		{
			PAD16;
			TAG_FIELD(int16, type);
			TAG_ENUM(framebuffer_blend_func, Enums::shader_framebuffer_blend_function);
			PAD16;
			TAG_PAD(int32, 8);

			////////////////////////////////////////////////////////////////
			// anchors
			// where you want the origin of the texture.
			// *"texture" uses the texture coordinates supplied
			// *"screen" uses the origin of the screen as the origin of the texture
			TAG_ENUM(primary_anchor, Enums::hud_multitexture_overlay_anchor);
			TAG_ENUM(secondary_anchor, Enums::hud_multitexture_overlay_anchor);
			TAG_ENUM(tertiary_anchor, Enums::hud_multitexture_overlay_anchor);

			////////////////////////////////////////////////////////////////
			// blending function
			// how to blend the textures together
			TAG_ENUM(_0_to_1_blend_func, Enums::hud_multitexture_overlay_blend_mode);
			TAG_ENUM(_1_to_2_blend_func, Enums::hud_multitexture_overlay_blend_mode);
			PAD16;

			////////////////////////////////////////////////////////////////
			// map scales
			// how much to scale the textures
			TAG_FIELD(real_point2d, primary_scale);
			TAG_FIELD(real_point2d, secondary_scale);
			TAG_FIELD(real_point2d, tertiary_scale);

			////////////////////////////////////////////////////////////////
			// map offsets
			// how much to offset the origin of the texture
			TAG_FIELD(real_point2d, primary_offset);
			TAG_FIELD(real_point2d, secondary_offset);
			TAG_FIELD(real_point2d, tertiary_offset);

			////////////////////////////////////////////////////////////////
			// map
			// which maps to use
			TAG_FIELD(tag_reference, primary, 'bitm');
			TAG_FIELD(tag_reference, secondary, 'bitm');
			TAG_FIELD(tag_reference, tertiary, 'bitm');
			TAG_ENUM(primary_wrap_mode, Enums::hud_multitexture_overlay_address);
			TAG_ENUM(secondary_wrap_mode, Enums::hud_multitexture_overlay_address);
			TAG_ENUM(tertiary_wrap_mode, Enums::hud_multitexture_overlay_address);
			PAD16;
			TAG_PAD(int32, 46);
			TAG_TBLOCK(effectors, multitexture_overlay_hud_element_effector_definition);
			TAG_PAD(int32, 32);
		}; BOOST_STATIC_ASSERT( sizeof(multitexture_overlay_hud_element_definition) == 0x1E0 ); // max count: 30

		struct s_hud_element_overlay : public s_hud_element_static
		{
			TAG_FIELD(int16, sequence_index);
			PAD16;
			TAG_TBLOCK(multitex_overlay, multitexture_overlay_hud_element_definition);
			PAD32;
		}; BOOST_STATIC_ASSERT( sizeof(s_hud_element_overlay) == 0x68 );


		//////////////////////////////////////////////////////////////////////////
		// unit_hud_interface
		struct sound_hud_element_definition
		{
			TAG_FIELD(tag_reference, sound, 'snd!', 'lsnd');
			TAG_FIELD(Flags::hud_sound_flags, latched_to);
			TAG_FIELD(real, scale);
			TAG_PAD(int32, 8);
		}; BOOST_STATIC_ASSERT( sizeof(sound_hud_element_definition) == 0x38 );

		struct auxilary_overlay_definition : public s_hud_element_overlay
		{
			TAG_ENUM(type, Enums::auxilary_overlay_type);
			TAG_FIELD(Flags::hud_auxilary_overlay_flags, flags);
			TAG_PAD(int32, 6);
		}; BOOST_STATIC_ASSERT( sizeof(auxilary_overlay_definition) == 0x84 ); // max count: 16

		struct auxilary_meter_definition
		{
			TAG_ENUM(type, Enums::hud_auxilary_meter_type);
			PAD16;
			TAG_PAD(int32, 4);

			s_hud_element_overlay background;
			s_hud_element_meter meter;
			TAG_FIELD(real, minimum_fraction_cutoff);
			TAG_FIELD(Flags::hud_auxilary_panel_flags, flags);
			TAG_PAD(int32, 6);
			TAG_PAD(int32, 16);
		}; BOOST_STATIC_ASSERT( sizeof(auxilary_meter_definition) == 0x144 ); // max count: 16

		struct unit_hud_interface_definition
		{
			enum { k_group_tag = 'unhi' };

			s_hud_absolute_placement placement;

			s_hud_element_overlay hud_background;

			struct {
				s_hud_element_overlay background;

				struct {
					s_hud_element_meter element;
					TAG_FIELD(rgb_color, overcharge_minimum_color);
					TAG_FIELD(rgb_color, overcharge_maximum_color);
					TAG_FIELD(rgb_color, overcharge_flash_color);
					TAG_FIELD(rgb_color, overcharge_empty_color);
					TAG_PAD(int32, 4);
				}meter;
			}shield_panel;

			struct {
				s_hud_element_overlay background;

				struct {
					s_hud_element_meter element;
					TAG_FIELD(rgb_color, medium_health_left_color);
					TAG_FIELD(real, max_color_health_fraction_cutoff);
					TAG_FIELD(real, min_color_health_fraction_cutoff);
					TAG_PAD(int32, 5);
				}meter; // same size as shield_panel.meter, so this may be union'd
			}health_panel;

			struct {
				s_hud_element_overlay background;
				s_hud_element_overlay forground;
				TAG_PAD(int32, 8);

				s_hud_element center; // The blips use this as a reference point
			}motion_sensor;

			struct {
				s_hud_absolute_placement placement;

				TAG_TBLOCK(overlays, auxilary_overlay_definition);
			}auxilary_overlays;

			TAG_PAD(int32, 4);

			TAG_TBLOCK(sounds, sound_hud_element_definition);

			TAG_TBLOCK(meters, auxilary_meter_definition);

			TAG_PAD(int32, 89);
			TAG_PAD(int32, 12);
		}; BOOST_STATIC_ASSERT( sizeof(unit_hud_interface_definition) == 0x56C );


		//////////////////////////////////////////////////////////////////////////
		// weapon hud interface

		struct weapon_hud_interface_definition
		{
			enum { k_group_tag = 'wphi' };

			TAG_FIELD(tag_reference, child_hud, "wphi");

		}; //BOOST_STATIC_ASSERT( sizeof(weapon_hud_interface_definition) == 0x17C );
	};
};