/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <YeloLib/tag_files/tag_groups_base_yelo.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_maximum_number_of_waypoints = 16,
		};

		enum hud_anchor : _enum
		{
			_hud_anchor_top_left,
			_hud_anchor_top_right,
			_hud_anchor_bottom_left,
			_hud_anchor_bottom_right,
			_hud_anchor_center,

			k_number_of_hud_anchors,
		};


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
	};

	namespace Flags
	{
		enum hud_scaling_flags : word_flags
		{
			_hud_scaling_dont_scale_offset_bit,
			_hud_scaling_dont_scale_size_bit,
			_hud_scaling_use_high_res_scale_bit,
		};

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
	};

	namespace TagGroups
	{
		struct s_hud_absolute_placement
		{
			TAG_ENUM(anchor, Enums::hud_anchor);
			PAD16;
			TAG_PAD(int32, 8);
		}; BOOST_STATIC_ASSERT( sizeof(s_hud_absolute_placement) == 0x24 );

		struct s_hud_element // same size as s_hud_absolute_placement...I think this may be union'd
		{
			TAG_FIELD(point2d, anchor_offset);
			TAG_FIELD(real, width_scale);
			TAG_FIELD(real, height_scale);
			TAG_FIELD(Flags::hud_scaling_flags, scaling_flags);
			PAD16;
			TAG_PAD(int32, 5);
		}; BOOST_STATIC_ASSERT( sizeof(s_hud_element) == 0x24 );

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
			//////////////////////////////////////////////////////////////////////////
			// only seen objective colors expose these fields
			TAG_FIELD(int16, uptime_ticks);
			TAG_FIELD(int16, fade_ticks);
		}; BOOST_STATIC_ASSERT( sizeof(s_hud_color_flash) == 0x20 );

		struct icon_hud_element_definition
		{
			TAG_FIELD(int16, sequence_index);
			TAG_FIELD(int16, width_offset);
			TAG_FIELD(point2d, offset_from_reference_corner);
			TAG_FIELD(argb_color, override_icon_color);
			TAG_FIELD(sbyte, frame_rate);
			TAG_FIELD(byte_flags, flags);
			TAG_FIELD(int16, text_index);
		}; BOOST_STATIC_ASSERT( sizeof(icon_hud_element_definition) == 0x10 );


		struct hud_messaging_definition
		{
			s_hud_absolute_placement placement;
			s_hud_element element;
			TAG_FIELD(tag_reference, single_player_font, 'font');
			TAG_FIELD(tag_reference, multiplayer_font, 'font');
			TAG_FIELD(real, up_time);
			TAG_FIELD(real, fade_time);
			TAG_FIELD(real_argb_color, icon_color);
			TAG_FIELD(real_argb_color, text_color);
			TAG_FIELD(real, text_spacing);
			TAG_FIELD(tag_reference, item_message_text, 'ustr');
			TAG_FIELD(tag_reference, icon_bitmap, 'bitm');
			TAG_FIELD(tag_reference, alternate_icon_text, 'ustr');
			TAG_TBLOCK(button_icons, icon_hud_element_definition);
		}; BOOST_STATIC_ASSERT( sizeof(hud_messaging_definition) == 0xD0 );

		struct hud_waypoint_arrow
		{
			TAG_FIELD(tag_string, name);
			PAD64;
			TAG_FIELD(rgb_color, color);
			TAG_FIELD(real, opacity);
			TAG_FIELD(real, translucency);
			TAG_FIELD(int16, on_screen_sequence_index);
			TAG_FIELD(int16, off_screen_sequence_index);
			TAG_FIELD(int16, occluded_sequence_index);
			PAD16;
			TAG_PAD(int32, 4);
			TAG_FIELD(long_flags, flags);
			TAG_PAD(int32, 6);
		}; BOOST_STATIC_ASSERT( sizeof(hud_waypoint_arrow) == 0x68 );

		struct hud_globals_definition
		{
			hud_messaging_definition hud_messaging;
			s_hud_color_flash hud_help_text_color;
			TAG_FIELD(tag_reference, hud_messages, 'hmt ');
			s_hud_color_flash objective_colors;
			struct {
				TAG_FIELD(real_rectangle2d, offset);
				TAG_PAD(tag_reference, 2);
				TAG_FIELD(tag_reference, arrow_bitmap, 'bitm');
				TAG_TBLOCK(arrows, hud_waypoint_arrow);
				TAG_PAD(int32, 20);
			}waypoint;
			struct {
				TAG_FIELD(real, hud_scale);
				TAG_PAD(tag_reference, 16);
			}multiplayer;
			struct {
				TAG_FIELD(tag_reference, weapon_hud, 'wphi');
				TAG_FIELD(real, motion_sensor_range);
				TAG_FIELD(real, motion_sensor_velocity_sensitivity);
				TAG_FIELD(real, motion_sensor_scale);
				TAG_FIELD(rectangle2d, chapter_title_bounds);
				TAG_PAD(int32, 11);
			}defaults;
			struct {
				struct { // short_rectangle2d
					int16 x0, x1;
					int16 y0, y1;
				}offset;
				TAG_PAD(tag_reference, 2);
				TAG_FIELD(tag_reference, bitmap, 'bitm');
				TAG_FIELD(int16, sequence_index);
				TAG_FIELD(int16, multiplayer_sequence_index);
				TAG_FIELD(argb_color, color);
				TAG_PAD(tag_reference, 1);
			}damage_indicators;
			struct {
				s_hud_color_flash not_much_time_left;
				s_hud_color_flash out_of_time;
				TAG_PAD(int32, 10);
			}hud_timer;
			TAG_FIELD(tag_reference, carnage_report_bitmap, 'bitm');
			struct {
				struct {
					TAG_FIELD(int16, begin_text);
					TAG_FIELD(int16, end_text);
				}loading_text,
				 checkpoint_text;
				TAG_FIELD(tag_reference, checkpoint_sound, 'snd!');
				TAG_PAD(int32, 24);
			}misc;
		}; BOOST_STATIC_ASSERT( sizeof(hud_globals_definition) == 0x450 );


		//////////////////////////////////////////////////////////////////////////
		// generic hud elements
		struct s_hud_element_number : public s_hud_element // aka global_hud_element
		{
			s_hud_color_flash color;
			TAG_FIELD(sbyte, maximum_number_of_digits);
			TAG_FIELD(byte_flags, flags);
			TAG_FIELD(sbyte, number_of_fractional_digits);
			PAD8;
			TAG_PAD(tag_block, 1);
		}; BOOST_STATIC_ASSERT( sizeof(s_hud_element_number) == 0x54 );

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
			TAG_FIELD(int16, value_scale, "used for non-integral values, i.e. health and shields");
			TAG_FIELD(real, opacity);
			TAG_FIELD(real, translucency);
			TAG_FIELD(argb_color, disabled_color);
			TAG_PAD(tag_block, 1);
			PAD32;
		}; BOOST_STATIC_ASSERT( sizeof(s_hud_element_meter) == 0x68 );

		struct hud_screen_effect_definition
		{
			PAD32;
			struct {
				TAG_FIELD(word_flags, flags);
				PAD16;
				TAG_PAD(tag_reference, 1);
				TAG_FIELD(tag_reference, fullscreen_bitmap, 'bitm');
				TAG_FIELD(tag_reference, splitscreen_bitmap, 'bitm');
				TAG_PAD(int32, 2);
			}mask;
			struct {
				TAG_FIELD(word_flags, flags);
				PAD16;
				TAG_FIELD(angle_bounds, fov_in_bounds);
				TAG_FIELD(real_bounds, radius_out_bounds);
				TAG_PAD(int32, 6);
			}convolution;
			struct {
				TAG_FIELD(word_flags, flags);
				TAG_FIELD(int16, script_source);
				TAG_FIELD(real, intensity);
				TAG_PAD(int32, 6);
			}night_vision;
			struct {
				TAG_FIELD(word_flags, flags);
				TAG_FIELD(int16, script_source);
				TAG_FIELD(real, intensity);
				TAG_FIELD(real_rgb_color, tint);
				TAG_PAD(int32, 6);
			}desaturation;
		}; BOOST_STATIC_ASSERT( sizeof(hud_screen_effect_definition) == 0xB8 );

		struct sound_hud_element_definition
		{
			TAG_FIELD(tag_reference, sound, 'snd!', 'lsnd');
			TAG_FIELD(long_flags, latched_to); // unit_hud_sound_flags, grenade_hud_sound_flags
			TAG_FIELD(real, scale);
			TAG_PAD(int32, 8);
		}; BOOST_STATIC_ASSERT( sizeof(sound_hud_element_definition) == 0x38 );


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
	};

	namespace blam
	{
		extern cstring g_hud_anchor_names[Enums::k_number_of_hud_anchors];
	};
};