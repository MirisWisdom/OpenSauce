/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

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
	};

	namespace Players
	{
		struct s_player_action
		{
			uint32 update_id;
			real_euler_angles2d desired_facing;
			real_vector2d throttle;
			real primary_trigger;
			int16 weapon_index, grenade_index;
			int16 zoom_index;
			PAD16;
		}; BOOST_STATIC_ASSERT( sizeof(s_player_action) == 0x20 );
	};
};