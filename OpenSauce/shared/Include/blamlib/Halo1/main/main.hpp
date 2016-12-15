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
		enum game_connection : _enum
		{
			_game_connection_local,
			_game_connection_network_client,
			_game_connection_network_server,
			_game_connection_film_playback,
		};
	};

	namespace GameState
	{
		struct s_main_globals;

		s_main_globals* MainGlobals();

		// Is the game *only* running the simulation locally? (ie, campaign or splitscreen)
		bool IsLocal();
		// Is this machine a server?
		bool IsServer();
		// Is this machine a client?
		bool IsClient();
	};

	namespace Main
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Get the HaloCE executable path as it is defined in the registry ("EXE Path"). </summary>
		///
		/// <returns>	The EXE Path value with an ending '\', or an empty string if the registry query failed </returns>
		const std::string& RegistryGetGameExePath();
	};

	namespace blam
	{
		// Gets the current connection type
		Enums::game_connection game_connection();

		// Gets the name of the current map. Actually the scenario path usually.
		// If map_name was specified via the console, it of course won't be the scenario path.
		cstring main_get_map_name();

		// Connect to a multiplayer server using it's ip:port and password
		bool PLATFORM_API main_connect(cstring address, cstring password);

		void PLATFORM_API main_menu_load();
	};	
};