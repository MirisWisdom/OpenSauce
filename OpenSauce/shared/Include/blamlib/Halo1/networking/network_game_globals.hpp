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
			k_network_server_password_length = 8,

			k_network_game_join_token_size = 16,
			k_network_game_challenge_length = 7,
		};
	};

	namespace Networking
	{
		struct network_update_globals
		{
			struct _local_player {
				int32 update_rate;						// 15
				int32 vehicle_update_rate;				// 15
			}local_player;

			struct _remote_player {
				int32 action_baseline_update_rate;		// 180
				int32 position_update_rate;				// 30
				int32 position_baseline_update_rate;	// 90
				int32 vehicle_update_rate;				// 15
				int32 vehicle_baseline_update_rate;		// 90, not used
			}remote_player;

			bool use_super_remote_players_action_update;// true
			bool use_new_vehicle_update_scheme;			// true
		};


		// Network settings dealing with the rate of updating certain data
		network_update_globals* UpdateSettings();
	};
};