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

	namespace blam
	{
		// Gets the current connection type
		Enums::game_connection game_connection();

		// Connect to a multiplayer server using it's ip:port and password
		bool PLATFORM_API main_connect(cstring address, cstring password);

		void PLATFORM_API main_menu_load();
	};

	// actually belongs in shell.hpp (shell_windows.c)
	namespace blam
	{
		// NOTE: command line arguments are not populated in guerilla nor tool.exe by default
		// So basically only use this in sapien builds, unless we've fixed this limitation
		bool PLATFORM_API shell_get_command_line_argument(cstring param, _Out_opt_ cstring* value = nullptr);
	};
};