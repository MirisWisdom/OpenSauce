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
			_player_powerup,
		};
	};

	namespace Players
	{
		union u_player_datum_scores
		{
			PAD64;

			struct s_ctf {
				int16 flag_grabs;
				int16 flag_returns;
				int16 flag_scores;
			}ctf;

			struct s_slayer {
			}slayer;

			struct s_oddball {
				UNKNOWN_TYPE(int16);
				int16 target_kills;
				int16 kills;
			}oddball;

			struct s_king {
				int16 hill_score;
			}king;

			struct s_race {
				int16 time;
				int16 laps;
				int16 best_time;
			}race;
		}; BOOST_STATIC_ASSERT( sizeof(u_player_datum_scores) == 8 );
	};
};