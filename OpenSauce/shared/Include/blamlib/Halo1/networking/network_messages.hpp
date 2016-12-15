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


		enum message_delta
		{
			_message_delta_object_deletion,
			_message_delta_projectile_update,
			_message_delta_equipment_update,
			_message_delta_weapon_update,
			_message_delta_biped_update,
			_message_delta_vehicle_update,
			// always written to local connection
			_message_delta_hud_add_item,
			_message_delta_player_create,
			// always written to local connection
			_message_delta_player_spawn,
			_message_delta_player_exit_vehicle,
			_message_delta_player_set_action_result,
			// always written to local connection
			_message_delta_player_effect_start,
			_message_delta_unit_kill,
			_message_delta_client_game_update,
			_message_delta_player_handle_powerup,
			// always written to local connection
			_message_delta_hud_chat,
			_message_delta_slayer_update,
			_message_delta_ctf_update,
			_message_delta_oddball_update,
			_message_delta_king_update,
			_message_delta_race_update,
			_message_delta_player_score_update,
			_message_delta_game_engine_change_mode,
			_message_delta_game_engine_map_reset,
			_message_delta_multiplayer_hud_message,
			_message_delta_multiplayer_sound,
			_message_delta_team_change,
			_message_delta_unit_drop_current_weapon,
			_message_delta_vehicle_new,
			_message_delta_biped_new,
			_message_delta_projectile_new,
			_message_delta_equipment_new,
			_message_delta_weapon_new,
			_message_delta_game_settings_update,
			_message_delta_parameters_protocol,
			_message_delta_local_player_update,
			_message_delta_local_player_vehicle_update,
			_message_delta_remote_player_action_update,
			_message_delta_super_remote_players_action_update,
			_message_delta_remote_player_position_update,
			_message_delta_remote_player_vehicle_update,
			_message_delta_remote_player_total_update_biped,
			_message_delta_remote_player_total_update_vehicle,
			_message_delta_weapon_start_reload,
			_message_delta_weapon_ammo_pickup_mid_reload,
			_message_delta_weapon_finish_reload,
			_message_delta_weapon_cancel_reload,
			_message_delta_netgame_equipment_new,
			_message_delta_projectile_detonate,
			_message_delta_item_accelerate,
			_message_delta_damage_dealt,
			_message_delta_projectile_attach,
			_message_delta_client_to_server_pong,
			_message_delta_super_ping_update,
			_message_delta_sv_motd,
			_message_delta_rcon_request,
			_message_delta_rcon_response,

			k_message_deltas_count
		};
	};
};