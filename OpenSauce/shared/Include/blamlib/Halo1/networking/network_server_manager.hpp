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
		enum network_game_server_state : _enum
		{
			_network_game_server_state_pregame,
			_network_game_server_state_ingame,
			_network_game_server_state_postgame,

			_network_game_server_state,
		};
	};

	namespace Networking
	{
		struct s_network_game_server;

		// Gets the network game server pointer
		s_network_game_server* NetworkGameServer();
	};
};