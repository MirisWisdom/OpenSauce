/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace TagGroups
	{
		struct s_hud_absolute_placement;

		struct s_hud_element_number;
		struct s_hud_element_static;
		struct s_hud_element_meter;
	};

	namespace Flags
	{
		enum hud_draw_element_flags : long_flags
		{
			_hud_draw_element_0_bit,
			_hud_draw_element_1_bit,
			_hud_draw_element_2_bit, // set when the local player count is > 1
		};
	};

	namespace blam
	{
		void PLATFORM_API hud_draw_meter(int16 local_player_index, TagGroups::s_hud_absolute_placement* placement, TagGroups::s_hud_element_meter* element, 
			int32, int32, long_flags, real, real);
		void PLATFORM_API hud_draw_numbers(int16 local_player_index, TagGroups::s_hud_absolute_placement* placement, TagGroups::s_hud_element_number* element,
			int32 number, int32, long_flags, real, real);
		void PLATFORM_API hud_draw_static_element(int16 local_player_index, TagGroups::s_hud_absolute_placement* placement, TagGroups::s_hud_element_static* element, 
			long_flags flags, argb_color default_color_override);
	};
};