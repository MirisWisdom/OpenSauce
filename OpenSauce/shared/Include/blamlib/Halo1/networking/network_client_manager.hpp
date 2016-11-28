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
		enum network_game_client_state : _enum
		{
			_network_game_client_state_searching,
			_network_game_client_state_joining,
			_network_game_client_state_pregame,
			_network_game_client_state_ingame,
			_network_game_client_state_postgame,

			_network_game_client_state,
		};
	};

	namespace Networking
	{		
		struct s_network_game_client;
		struct s_network_game_client_yelo;


		// Gets the network game client pointer
		s_network_game_client* NetworkGameClient();
	};
};