/*
    Yelo: Open Sauce SDK
		Halo 2 Edition

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum {
			// Maximum number of users per machine
			k_number_of_users = 
#if PLATFORM_TARGET == PLATFORM_TARGET_PC
			1,
#else
			4,
#endif

			k_no_controller = NONE,
			k_number_of_controllers = k_number_of_users,

			k_maximum_machines = 16,

			k_network_maximum_players_per_session = k_maximum_machines,

			k_maximum_players = k_maximum_machines,
			k_maximum_teams = k_maximum_players,
		};
	};
};