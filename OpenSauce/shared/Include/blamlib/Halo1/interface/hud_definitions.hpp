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

		struct hud_globals_definition
		{
			hud_messaging_definition hud_messaging;
		}; //BOOST_STATIC_ASSERT( sizeof(hud_globals_definition) == 0x );
	};
};