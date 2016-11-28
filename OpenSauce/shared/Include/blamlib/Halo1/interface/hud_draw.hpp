/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/game/game_configuration.hpp>

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
			_hud_draw_element_blink_bit,
			_hud_draw_element_flash_bit,
			_hud_draw_element_splitscreen_bit,
		};
	};

	namespace blam
	{
		void PLATFORM_API hud_draw_meter(int16 local_player_index, TagGroups::s_hud_absolute_placement* placement, TagGroups::s_hud_element_meter* element, 
			byte, byte, long_flags flags, real, real);
		void PLATFORM_API hud_draw_numbers(int16 local_player_index, TagGroups::s_hud_absolute_placement* placement, TagGroups::s_hud_element_number* element,
			int32 number, int16, long_flags flags, game_ticks_t first_render_time, real);
		void PLATFORM_API hud_draw_static_element(int16 local_player_index, TagGroups::s_hud_absolute_placement* placement, TagGroups::s_hud_element_static* element, 
			long_flags flags, game_ticks_t first_render_time);
	};
};