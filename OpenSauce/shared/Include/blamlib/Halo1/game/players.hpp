/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/game/player_action.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_maximum_number_of_local_players = 1,

			k_multiplayer_maximum_players = 16,

			k_player_name_length = 11,
		};

		enum multiplayer_team
		{
			_multiplayer_team_red,
			_multiplayer_team_blue,

			k_number_of_multiplayer_teams,

			// for team_data (which has a maximum of 16 datums) and such
			k_multiplayer_maximum_teams = k_multiplayer_maximum_players,
		};

		enum player_powerup
		{
			_player_powerup_active_camo,
			_player_powerup_full_spectrum_vision,
			
			k_number_of_player_powerups,
		};
	};

	namespace Players
	{
	};
};