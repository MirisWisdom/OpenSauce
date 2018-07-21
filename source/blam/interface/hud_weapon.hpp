/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blam/game/game_configuration.hpp>
#include <blam/interface/weapon_hud_interface_definition.hpp>

namespace Yelo
{
	namespace Flags
	{
		enum weapon_interface_show
		{
			_weapon_interface_show_crosshair_bit,
		};
	};

	namespace GameUI
	{
		struct s_hud_weapon_interface_weapon_state
		{
			game_ticks_t first_render_times[Enums::k_number_of_weapon_hud_states];
			datum_index weapon_index;
			game_ticks_t grenades_first_render_time;
		}; ASSERT_SIZE(s_hud_weapon_interface_weapon_state, 0x28);

		struct s_hud_weapon_interface_crosshairs_state
		{
			game_ticks_t first_render_times[Enums::k_number_of_weapon_crosshair_types];
			long_flags render_types_mask[BIT_VECTOR_SIZE_IN_DWORDS(Enums::k_number_of_weapon_crosshair_types)];
		}; ASSERT_SIZE(s_hud_weapon_interface_crosshairs_state, 0x50);

		struct s_hud_weapon_interface
		{
			struct s_local_player
			{
				s_hud_weapon_interface_weapon_state weapon;
				s_hud_weapon_interface_crosshairs_state crosshairs;
			}; ASSERT_SIZE(s_local_player, 0x78);

			s_local_player local_players[Enums::k_maximum_number_of_local_players];
			long_flags show_flags;
		}; ASSERT_SIZE(s_hud_weapon_interface, 0x7C);
		s_hud_weapon_interface*		HudWeaponInterface();
	};
};