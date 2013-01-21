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
		enum network_messsage_type
		{
			_network_messsage_type_message, // 'old' packets
			_network_messsage_type_message_delta,

			_network_messsage_type,
		};
		// Old halo packets
		enum message_type
		{
			_message_client_broadcast_game_search,
			_message_client_ping,
			_message_server_game_advertise,
			_message_server_pong,
			_message_server_new_client_challenge, // new
			_message_server_machine_accepted,
			_message_server_machine_rejected,
			// Makes the client set its state to _network_game_client_state_postgame
			_message_server_game_is_ending_holdup, // new
			_message_server_game_settings_update,
			_message_server_pregame_countdown,
			// always written to local connection
			_message_server_begin_game,
			_message_server_graceful_game_exit_pregame,
			_message_server_pregame_keep_alive,
			_message_server_postgame_keep_alive,
			_message_client_join_game_request,
			_message_client_add_player_request_pregame,
			_message_client_remove_player_request_pregame,
			_message_client_settings_request,
			_message_client_player_settings_request,
			_message_client_game_start_request,
			_message_client_graceful_game_exit_pregame,
			_message_client_map_is_precached_pregame,
			_message_server_game_update,
			_message_server_add_player_ingame,
			_message_server_remove_player_ingame,
			_message_server_game_over,
			_message_client_loaded,
			_message_client_game_update,
			_message_client_add_player_request_ingame,
			_message_client_remove_player_request_ingame,
			_message_client_graceful_game_exit_ingame, // new
			_message_client_host_crashed_cry_for_help,
			_message_client_join_new_host,
			_message_server_reconnect, // new
			_message_server_graceful_game_exit,
			_message_client_remove_player_request_postgame,
			_message_client_switch_to_pregame,
			_message_client_graceful_game_exit_postgame,
			_message_type,
		};
	};

	namespace Networking
	{
	};
};