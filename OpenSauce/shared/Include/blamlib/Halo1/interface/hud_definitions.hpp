/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <TagGroups/Halo1/TagGroupDefinitions.hpp>

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

			_hud_anchor,
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
	};
};