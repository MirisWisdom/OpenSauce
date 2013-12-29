/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

//#include <blamlib/Halo1/bungie_net/common/message_header.hpp>
#include <blamlib/Halo1/game/game_configuration.hpp>
#include <blamlib/Halo1/game/player_action.hpp>
#include <blamlib/Halo1/game/players.hpp>
#include <blamlib/Halo1/networking/network_game_globals.hpp>
#include <blamlib/Halo1/networking/network_game_manager.hpp>

namespace Yelo
{
	namespace Enums
	{
		// engine writes a bit after the message in the bitstream:
		// 0 means it's an 'old' message
		// 1 means it's a message delta
		// We use an enum just to be clear
		enum network_messsage_type
		{
			_network_messsage_type_message, // 'old' packets
			_network_messsage_type_message_delta,
		};
		// Old halo packets
		enum
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
#if FALSE
			_message_server_object_creation_packet,
			_message_server_object_deletion_packet,
			_message_server_projectile_update_packet,
			_message_server_equipment_update_packet,
			_message_server_weapon_update_packet,
			_message_server_player_index_set_packet,
			_message_server_biped_update_packet,
			_message_server_vehicle_update_packet,
			_message_server_hud_add_item_packet,
			_message_server_player_effect_start_packet,
			_message_server_player_set_action_result_packet,
			_message_server_player_exit_vehicle_packet,
#endif
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
#if FALSE
			_message_server_game_object_update_packet,
#endif
			
			k_number_of_message_packet_types,
		};
	};

	namespace Networking
	{
		struct message_server_new_client_challenge
		{
			char challenge[Enums::k_network_game_challenge_length+1];
			bool sv_public;
			PAD24;
			int16 machine_index;
			PAD16;
			s_network_game_map map;
		}; BOOST_STATIC_ASSERT( sizeof(message_server_new_client_challenge) == 0x94 );

		struct message_client_join_game_request
		{
			char token[Enums::k_network_game_join_token_size];
			wchar_t password[Enums::k_network_server_password_length+1];
			char unused[73];
			byte_enum connection_class;										// 0x6B Enums::network_connection_class
			PAD16;
			s_network_game_player player;									// 0x6E
			PAD16;
			uint32 map_crc_xor_challenge;									// 0x90
		}; BOOST_STATIC_ASSERT( sizeof(message_client_join_game_request) == 0x94 );

		struct message_server_game_update
		{
			int32 update_id;
			int32 random_seed;

			game_ticks_t ticks_to_apply_update_to;
			UNKNOWN_TYPE(int16); // unused?
			int16 action_count;
			Players::s_player_action actions[Enums::k_multiplayer_maximum_players];
		}; BOOST_STATIC_ASSERT( sizeof(message_server_game_update) == 0x210 );

		struct message_client_game_update
		{
			game_ticks_t ticks_to_apply_update_to;
			UNKNOWN_TYPE(int16); // unused?
			int16 action_count; // should be equal to local_player_count
			Players::s_player_action actions[Enums::k_maximum_number_of_local_players];
		}; BOOST_STATIC_ASSERT( sizeof(message_client_game_update) == 0x28 );
	};
};