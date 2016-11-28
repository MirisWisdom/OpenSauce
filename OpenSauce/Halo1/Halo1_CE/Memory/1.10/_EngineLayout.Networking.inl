/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

#if !PLATFORM_IS_DEDI
#include "Memory/1.10/Pointers/HaloCE_110_Runtime.Networking.inl"
#include "Memory/1.10/Pointers/HaloCE_110_Runtime_Manual.Networking.inl"
#else
#include "Memory/1.10/Pointers/HaloCE_110_Dedi.Networking.inl"
#include "Memory/1.10/Pointers/HaloCE_110_Dedi_Manual.Networking.inl"
#endif

//////////////////////////////////////////////////////////////////////////
// MDP.hpp
#if __EL_INCLUDE_FILE_ID == __EL_NETWORKING_MDP
	FUNC_PTR(MDP_FIELD_ENCODE_ARRAY,	DUO_PTR(K_MDP_FIELD_ENCODE_ARRAY));
	FUNC_PTR(MDP_FIELD_DECODE_ARRAY,	DUO_PTR(K_MDP_FIELD_DECODE_ARRAY));

	ENGINE_PTR(field_properties_definition, message_delta_field_integer_small,							DUO_PTR(K_MESSAGE_DELTA_FIELD_INTEGER_SMALL));
	ENGINE_PTR(field_properties_definition, message_delta_field_integer_medium,							DUO_PTR(K_MESSAGE_DELTA_FIELD_INTEGER_MEDIUM));
	ENGINE_PTR(field_properties_definition, message_delta_field_integer_large,							DUO_PTR(K_MESSAGE_DELTA_FIELD_INTEGER_LARGE));
	ENGINE_PTR(field_properties_definition, message_delta_field_real,									DUO_PTR(K_MESSAGE_DELTA_FIELD_REAL));
	ENGINE_PTR(field_properties_definition, message_delta_field_boolean,								DUO_PTR(K_MESSAGE_DELTA_FIELD_BOOLEAN));
	ENGINE_PTR(field_properties_definition, message_delta_field_ascii_character,						DUO_PTR(K_MESSAGE_DELTA_FIELD_ASCII_CHARACTER));
	ENGINE_PTR(field_properties_definition, message_delta_field_time,									DUO_PTR(K_MESSAGE_DELTA_FIELD_TIME));
	ENGINE_PTR(field_properties_definition, message_delta_field_grenade_counts,							DUO_PTR(K_MESSAGE_DELTA_FIELD_GRENADE_COUNTS));
	ENGINE_PTR(field_properties_definition, message_delta_field_locality_reference_position,			DUO_PTR(K_MESSAGE_DELTA_FIELD_LOCALITY_REFERENCE_POSITION));
	ENGINE_PTR(field_properties_definition, message_delta_field_digital_throttle,						DUO_PTR(K_MESSAGE_DELTA_FIELD_DIGITAL_THROTTLE));
	ENGINE_PTR(field_properties_definition, message_delta_field_fixed_width_weapon_index,				DUO_PTR(K_MESSAGE_DELTA_FIELD_FIXED_WIDTH_WEAPON_INDEX));
	ENGINE_PTR(field_properties_definition, message_delta_field_fixed_width_grenade_index,				DUO_PTR(K_MESSAGE_DELTA_FIELD_FIXED_WIDTH_GRENADE_INDEX));
	ENGINE_PTR(field_properties_definition, message_delta_field_item_placement_position,				DUO_PTR(K_MESSAGE_DELTA_FIELD_ITEM_PLACEMENT_POSITION));

	ENGINE_PTR(field_properties_definition, message_delta_field_local_player_update_sequence_number,	DUO_PTR(K_MESSAGE_DELTA_FIELD_LOCAL_PLAYER_UPDATE_SEQUENCE_NUMBER));
	ENGINE_PTR(field_properties_definition, message_delta_field_remote_player_update_sequence_number,	DUO_PTR(K_MESSAGE_DELTA_FIELD_REMOTE_PLAYER_UPDATE_SEQUENCE_NUMBER));
	ENGINE_PTR(field_properties_definition, message_delta_field_remote_player_action_update_baseline_id,DUO_PTR(K_MESSAGE_DELTA_FIELD_REMOTE_PLAYER_ACTION_UPDATE_BASELINE_ID));
	ENGINE_PTR(field_properties_definition, message_delta_field_update_tick_count,						DUO_PTR(K_MESSAGE_DELTA_FIELD_UPDATE_TICK_COUNT));
	ENGINE_PTR(field_properties_definition, message_delta_field_update_id,								DUO_PTR(K_MESSAGE_DELTA_FIELD_UPDATE_ID));
	ENGINE_PTR(field_properties_definition, message_delta_field_fixed_width_3bits,						DUO_PTR(K_MESSAGE_DELTA_FIELD_FIXED_WIDTH_3BITS));
	ENGINE_PTR(field_properties_definition, message_delta_field_fixed_width_6bits,						DUO_PTR(K_MESSAGE_DELTA_FIELD_FIXED_WIDTH_6BITS));
	ENGINE_PTR(field_properties_definition, message_delta_field_fixed_width_1bit,						DUO_PTR(K_MESSAGE_DELTA_FIELD_FIXED_WIDTH_1BIT));
	ENGINE_PTR(field_properties_definition, message_delta_field_fixed_width_normal_4bit,				DUO_PTR(K_MESSAGE_DELTA_FIELD_FIXED_WIDTH_NORMAL_4BIT));
	ENGINE_PTR(field_properties_definition, message_delta_field_fixed_width_normal_8bit,				DUO_PTR(K_MESSAGE_DELTA_FIELD_FIXED_WIDTH_NORMAL_8BIT));
	ENGINE_PTR(field_properties_definition, message_delta_field_fixed_width_normal_16bit,				DUO_PTR(K_MESSAGE_DELTA_FIELD_FIXED_WIDTH_NORMAL_16BIT));
	ENGINE_PTR(field_properties_definition, message_delta_field_object_index,							DUO_PTR(K_MESSAGE_DELTA_FIELD_OBJECT_INDEX));
	ENGINE_PTR(field_properties_definition, message_delta_field_player_index,							DUO_PTR(K_MESSAGE_DELTA_FIELD_PLAYER_INDEX));
	ENGINE_PTR(field_properties_definition, message_delta_field_definition_index,						DUO_PTR(K_MESSAGE_DELTA_FIELD_DEFINITION_INDEX));
	ENGINE_PTR(field_properties_definition, message_delta_field_point2d,								DUO_PTR(K_MESSAGE_DELTA_FIELD_POINT2D));
	ENGINE_PTR(field_properties_definition, message_delta_field_point3d,								DUO_PTR(K_MESSAGE_DELTA_FIELD_POINT3D));
	ENGINE_PTR(field_properties_definition, message_delta_field_vector2d,								DUO_PTR(K_MESSAGE_DELTA_FIELD_VECTOR2D));
	ENGINE_PTR(field_properties_definition, message_delta_field_vector3d,								DUO_PTR(K_MESSAGE_DELTA_FIELD_VECTOR3D));
	ENGINE_PTR(field_properties_definition, message_delta_field_control_flags,							DUO_PTR(K_MESSAGE_DELTA_FIELD_CONTROL_FLAGS));
	ENGINE_PTR(field_properties_definition, message_delta_field_angular_velocity,						DUO_PTR(K_MESSAGE_DELTA_FIELD_ANGULAR_VELOCITY));
	ENGINE_PTR(field_properties_definition, message_delta_field_translational_velocity,					DUO_PTR(K_MESSAGE_DELTA_FIELD_TRANSLATIONAL_VELOCITY));
	ENGINE_PTR(field_properties_definition, message_delta_field_map_name,								DUO_PTR(K_MESSAGE_DELTA_FIELD_MAP_NAME));
	ENGINE_PTR(field_properties_definition, message_delta_field_network_map,							DUO_PTR(K_MESSAGE_DELTA_FIELD_NETWORK_MAP));
	ENGINE_PTR(field_properties_definition, message_delta_field_network_player_name,					DUO_PTR(K_MESSAGE_DELTA_FIELD_NETWORK_PLAYER_NAME));
	ENGINE_PTR(field_properties_definition, message_delta_field_network_player,							DUO_PTR(K_MESSAGE_DELTA_FIELD_NETWORK_PLAYER));
	ENGINE_PTR(field_properties_definition, message_delta_field_network_game_name,						DUO_PTR(K_MESSAGE_DELTA_FIELD_NETWORK_GAME_NAME));
	ENGINE_PTR(field_properties_definition, message_delta_field_network_game_players,					DUO_PTR(K_MESSAGE_DELTA_FIELD_NETWORK_GAME_PLAYERS));
	ENGINE_PTR(field_properties_definition, message_delta_field_game_variant,							DUO_PTR(K_MESSAGE_DELTA_FIELD_GAME_VARIANT));
	ENGINE_PTR(field_properties_definition, message_delta_field_game_variant_human_readable_description,DUO_PTR(K_MESSAGE_DELTA_FIELD_GAME_VARIANT_HUMAN_READABLE_DESCRIPTION));
	ENGINE_PTR(field_properties_definition, message_delta_field_universal_variant,						DUO_PTR(K_MESSAGE_DELTA_FIELD_UNIVERSAL_VARIANT));
	ENGINE_PTR(field_properties_definition, message_delta_field_universal_variant_flags,				DUO_PTR(K_MESSAGE_DELTA_FIELD_UNIVERSAL_VARIANT_FLAGS));
	ENGINE_PTR(field_properties_definition, message_delta_field_game_engine_variant,					DUO_PTR(K_MESSAGE_DELTA_FIELD_GAME_ENGINE_VARIANT));
	ENGINE_PTR(field_properties_definition, message_delta_field_game_variant_flags,						DUO_PTR(K_MESSAGE_DELTA_FIELD_GAME_VARIANT_FLAGS));
	ENGINE_PTR(field_properties_definition, message_delta_field_parameters_protocol_array,				DUO_PTR(K_MESSAGE_DELTA_FIELD_PARAMETERS_PROTOCOL_ARRAY));
	ENGINE_PTR(field_properties_definition, message_delta_field_hud_chat_message,						DUO_PTR(K_MESSAGE_DELTA_FIELD_HUD_CHAT_MESSAGE));
	ENGINE_PTR(field_properties_definition, message_delta_field_hud_chat_message_ptr,					DUO_PTR(K_MESSAGE_DELTA_FIELD_HUD_CHAT_MESSAGE_PTR));
	ENGINE_PTR(field_properties_definition, message_delta_field_object_change_colors,					DUO_PTR(K_MESSAGE_DELTA_FIELD_OBJECT_CHANGE_COLORS));
	ENGINE_PTR(field_properties_definition, message_delta_field_ctf_score_array,						DUO_PTR(K_MESSAGE_DELTA_FIELD_CTF_SCORE_ARRAY));
	ENGINE_PTR(field_properties_definition, message_delta_field_race_score_array,						DUO_PTR(K_MESSAGE_DELTA_FIELD_RACE_SCORE_ARRAY));
	ENGINE_PTR(field_properties_definition, message_delta_field_king_score_array,						DUO_PTR(K_MESSAGE_DELTA_FIELD_KING_SCORE_ARRAY));
	ENGINE_PTR(field_properties_definition, message_delta_field_oddball_score_array,					DUO_PTR(K_MESSAGE_DELTA_FIELD_ODDBALL_SCORE_ARRAY));
	ENGINE_PTR(field_properties_definition, message_delta_field_oddball_owner_array,					DUO_PTR(K_MESSAGE_DELTA_FIELD_ODDBALL_OWNER_ARRAY));
	ENGINE_PTR(field_properties_definition, message_delta_field_slayer_score_array,						DUO_PTR(K_MESSAGE_DELTA_FIELD_SLAYER_SCORE_ARRAY));
	ENGINE_PTR(field_properties_definition, message_delta_field_damage_data_flags,						DUO_PTR(K_MESSAGE_DELTA_FIELD_DAMAGE_DATA_FLAGS));
	ENGINE_PTR(field_properties_definition, message_delta_field_motd_text,								DUO_PTR(K_MESSAGE_DELTA_FIELD_MOTD_TEXT));
	ENGINE_PTR(field_properties_definition, message_delta_field_rcon_response,							DUO_PTR(K_MESSAGE_DELTA_FIELD_RCON_RESPONSE));
	ENGINE_PTR(field_properties_definition, message_delta_field_rcon_password,							DUO_PTR(K_MESSAGE_DELTA_FIELD_RCON_PASSWORD));
	ENGINE_PTR(field_properties_definition, message_delta_field_rcon_command,							DUO_PTR(K_MESSAGE_DELTA_FIELD_RCON_COMMAND));


	ENGINE_PTR(field_reference_set, message_delta_empty_field_set,										DUO_PTR(K_MESSAGE_DELTA_EMPTY_FIELD_SET));

	//ENGINE_PTR(field_reference_set, message_delta_guaranteed_object_header_field_set,					0x0, 0x0);
	ENGINE_PTR(field_reference_set, message_delta_game_object_header_field_set,							DUO_PTR(K_MESSAGE_DELTA_GAME_OBJECT_HEADER_FIELD_SET));
	ENGINE_PTR(field_reference_set, message_delta_game_object_timestamped_header_field_set,				DUO_PTR(K_MESSAGE_DELTA_GAME_OBJECT_TIMESTAMPED_HEADER_FIELD_SET));
	ENGINE_PTR(field_reference_set, message_delta_biped_update_header_field_set,						DUO_PTR(K_MESSAGE_DELTA_BIPED_UPDATE_HEADER_FIELD_SET));
	ENGINE_PTR(field_reference_set, message_delta_ctf_update_header_field_set,							DUO_PTR(K_MESSAGE_DELTA_CTF_UPDATE_HEADER_FIELD_SET));
	ENGINE_PTR(field_reference_set, message_delta_client_game_update_header_field_set,					DUO_PTR(K_MESSAGE_DELTA_CLIENT_GAME_UPDATE_HEADER_FIELD_SET));
	ENGINE_PTR(field_reference_set, message_delta_player_score_update_header_field_set,					DUO_PTR(K_MESSAGE_DELTA_PLAYER_SCORE_UPDATE_HEADER_FIELD_SET));
	ENGINE_PTR(field_reference_set, message_delta_remote_player_action_update_header_field_set,			DUO_PTR(K_MESSAGE_DELTA_REMOTE_PLAYER_ACTION_UPDATE_HEADER_FIELD_SET));
	ENGINE_PTR(field_reference_set, message_delta_remote_player_position_update_header_field_set,		DUO_PTR(K_MESSAGE_DELTA_REMOTE_PLAYER_POSITION_UPDATE_HEADER_FIELD_SET));
	ENGINE_PTR(field_reference_set, message_delta_remote_player_total_update_header_field_set,			DUO_PTR(K_MESSAGE_DELTA_REMOTE_PLAYER_TOTAL_UPDATE_HEADER_FIELD_SET));
	//ENGINE_PTR(field_reference_set, message_delta__header_field_set, 0, PTR_UNKNOWN);


//////////////////////////////////////////////////////////////////////////
// MessageDefinitions.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_NETWORKING_MESSAGE_DEFINITIONS

	// Currently there is nothing to add here


//////////////////////////////////////////////////////////////////////////
// MessageDeltas.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_NETWORKING_MESSAGE_DELTAS
	ENGINE_PTR(s_message_delta_parameters, message_delta_parameters,				DUO_PTR(K_MESSAGE_DELTA_PARAMETERS));
	ENGINE_PTR(field_type_definition, message_delta_global_field_type_list,			DUO_PTR(K_MESSAGE_DELTA_GLOBAL_FIELD_TYPE_LIST));
	ENGINE_PTR(Enums::message_delta_encoding_class, g_message_delta_encoding_class,	DUO_PTR(K_G_MESSAGE_DELTA_ENCODING_CLASS));
	ENGINE_PTR(exposed_parameters_t, g_exposed_params,								DUO_PTR(K_G_EXPOSED_PARAMS));
	ENGINE_PTR(s_parameters_protocol_globals, mdp_parameters_protocol_globals,		DUO_PTR(K_MDP_PARAMETERS_PROTOCOL_GLOBALS));

	ENGINE_PTR(byte, mdp_packet_buffer_received_data,								DUO_PTR(K_MDP_PACKET_BUFFER_RECEIVED_DATA));
	ENGINE_PTR(byte, mdp_packet_buffer_sent_data,									DUO_PTR(K_MDP_PACKET_BUFFER_SENT_DATA));

	ENGINE_DPTR(message_delta_definition, message_delta_packets,					DUO_PTR(K_MESSAGE_DELTA_PACKETS));

	#ifndef YELO_NO_NETWORK
		#pragma region MessageDeltaPointerReferences
		static void** MessageDeltaPointerReferences[] = {
			CAST_PTR(void**, K_MESSAGE_DELTA_POINTER_REFERENCE_0),
			CAST_PTR(void**, K_MESSAGE_DELTA_POINTER_REFERENCE_1),
			CAST_PTR(void**, K_MESSAGE_DELTA_POINTER_REFERENCE_2),
			CAST_PTR(void**, K_MESSAGE_DELTA_POINTER_REFERENCE_3),
			CAST_PTR(void**, K_MESSAGE_DELTA_POINTER_REFERENCE_4),
			CAST_PTR(void**, K_MESSAGE_DELTA_POINTER_REFERENCE_5),
			CAST_PTR(void**, K_MESSAGE_DELTA_POINTER_REFERENCE_6),
			CAST_PTR(void**, K_MESSAGE_DELTA_POINTER_REFERENCE_7),
			CAST_PTR(void**, K_MESSAGE_DELTA_POINTER_REFERENCE_8),
			CAST_PTR(void**, K_MESSAGE_DELTA_POINTER_REFERENCE_9),
			CAST_PTR(void**, K_MESSAGE_DELTA_POINTER_REFERENCE_10),
			CAST_PTR(void**, K_MESSAGE_DELTA_POINTER_REFERENCE_11),
			CAST_PTR(void**, K_MESSAGE_DELTA_POINTER_REFERENCE_12),
			CAST_PTR(void**, K_MESSAGE_DELTA_POINTER_REFERENCE_13),
			CAST_PTR(void**, K_MESSAGE_DELTA_POINTER_REFERENCE_14),
			CAST_PTR(void**, K_MESSAGE_DELTA_POINTER_REFERENCE_15),
			CAST_PTR(void**, K_MESSAGE_DELTA_POINTER_REFERENCE_16),
			CAST_PTR(void**, K_MESSAGE_DELTA_POINTER_REFERENCE_17),
		};
		#pragma endregion

		#pragma region MessageDeltaPointerEndChecks
		static void** MessageDeltaPointerEndChecks[] = {
			CAST_PTR(void**, K_MESSAGE_DELTA_POINTER_END_CHECK_0),
			CAST_PTR(void**, K_MESSAGE_DELTA_POINTER_END_CHECK_1),
			CAST_PTR(void**, K_MESSAGE_DELTA_POINTER_END_CHECK_2),
		};

		static byte* MessageDeltaTypeCountChecks8bit[] = {
			CAST_PTR(byte*, K_MESSAGE_DELTA_TYPE_COUNT_CHECK_8BIT_0),
		};
		#pragma endregion

		FUNC_PTR(NETWORK_GAME_CLIENT_GAME_SETTINGS_UPDATED,			DUO_PTR(K_NETWORK_GAME_CLIENT_GAME_SETTINGS_UPDATED));
		FUNC_PTR(NETWORK_GAME_CLIENT_GAME_SETTINGS_UPDATED_CALL1,	DUO_PTR(K_NETWORK_GAME_CLIENT_GAME_SETTINGS_UPDATED_CALL1));
		FUNC_PTR(NETWORK_GAME_CLIENT_GAME_SETTINGS_UPDATED_CALL2,	DUO_PTR(K_NETWORK_GAME_CLIENT_GAME_SETTINGS_UPDATED_CALL2));

		FUNC_PTR(NETWORK_GAME_CLIENT_HANDLE_MESSAGE_DELTA_MESSAGE_BODY_CALL,DUO_PTR(K_NETWORK_GAME_CLIENT_HANDLE_MESSAGE_DELTA_MESSAGE_BODY_CALL));
			DATA_PTR(DONT_SEND_OBJECT_NEW_MSG,								DUO_PTR(K_DONT_SEND_OBJECT_NEW_MSG));
			FUNC_PTR(NETWORK_GAME_CLIENT_HANDLE_MESSAGE_DELTA_MESSAGE_BODY,	DUO_PTR(K_NETWORK_GAME_CLIENT_HANDLE_MESSAGE_DELTA_MESSAGE_BODY));


		ENGINE_PTR(Networking::s_network_game_server, global_network_game_server_data, DUO_PTR(K_GLOBAL_NETWORK_GAME_SERVER_DATA));
		ENGINE_PTR(Networking::s_network_game_client, global_network_game_client_data, DUO_PTR(K_GLOBAL_NETWORK_GAME_CLIENT_DATA));

		FUNC_PTR(MDPI_ENCODE,										DUO_PTR(K_MDPI_ENCODE));
		FUNC_PTR(MDP_DECODE_STATELESS_ITERATED,						DUO_PTR(K_MDP_DECODE_STATELESS_ITERATED));
		FUNC_PTR(MDP_DECODE_INCREMENTAL_ITERATED,					DUO_PTR(K_MDP_DECODE_INCREMENTAL_ITERATED));
		FUNC_PTR(MDP_DISCARD_ITERATION_BODY,						DUO_PTR(K_MDP_DISCARD_ITERATION_BODY));
	#endif


//////////////////////////////////////////////////////////////////////////
// Networking.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_NETWORKING_NETWORKING
	ENGINE_PTR(s_update_client_globals, update_client_globals,		DUO_PTR(K_UPDATE_CLIENT_GLOBALS));
	ENGINE_PTR(s_update_server_globals, update_server_globals,		DUO_PTR(K_UPDATE_SERVER_GLOBALS));
	ENGINE_PTR(network_update_globals, network_update_globals,		DUO_PTR(K_NETWORK_UPDATE_GLOBALS));
	ENGINE_DPTR(s_network_game_server, global_network_game_server,	DUO_PTR(K_GLOBAL_NETWORK_GAME_SERVER));
	ENGINE_DPTR(s_network_game_client, global_network_game_client,	DUO_PTR(K_GLOBAL_NETWORK_GAME_CLIENT));

	FUNC_PTR(NETWORK_GAME_ADD_PLAYER,									DUO_PTR(K_NETWORK_GAME_ADD_PLAYER));
	FUNC_PTR(NETWORK_GAME_SERVER_ADD_PLAYER_TO_GAME__HOOK_ADD_PLAYER,	DUO_PTR(K_NETWORK_GAME_SERVER_ADD_PLAYER_TO_GAME__HOOK_ADD_PLAYER));
	FUNC_PTR(NETWORK_GAME_REMOVE_PLAYER,								DUO_PTR(K_NETWORK_GAME_REMOVE_PLAYER));
	// just a few bytes after this gets called is another call to ngrp, but its for the s_network_client's game member
	FUNC_PTR(CLIENT_MACHINE_CLEANUP__HOOK_REMOVE_PLAYER,				DUO_PTR(K_CLIENT_MACHINE_CLEANUP__HOOK_REMOVE_PLAYER));

	FUNC_PTR(NETWORK_CONNECTION_WRITE,									DUO_PTR(K_NETWORK_CONNECTION_WRITE));
	FUNC_PTR(NETWORK_GAME_SERVER_SEND_REJECTION_MESSAGE,				DUO_PTR(K_NETWORK_GAME_SERVER_SEND_REJECTION_MESSAGE));
	FUNC_PTR(NETWORK_GAME_SERVER_HOLDUP_NEW_CLIENT,						DUO_PTR(K_NETWORK_GAME_SERVER_HOLDUP_NEW_CLIENT));
	FUNC_PTR(NETWORK_GAME_SERVER_SEND_MESSAGE_TO_MACHINE,				DUO_PTR(K_NETWORK_GAME_SERVER_SEND_MESSAGE_TO_MACHINE));
	FUNC_PTR(NETWORK_GAME_SERVER_SEND_MESSAGE_TO_ALL_MACHINES,			DUO_PTR(K_NETWORK_GAME_SERVER_SEND_MESSAGE_TO_ALL_MACHINES));
	FUNC_PTR(NETWORK_GAME_SERVER_SEND_MESSAGE_TO_ALL_MACHINES_INGAME,	DUO_PTR(K_NETWORK_GAME_SERVER_SEND_MESSAGE_TO_ALL_MACHINES_INGAME));


//////////////////////////////////////////////////////////////////////////
// Server.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_NETWORKING_SERVER
	namespace Networking
	{
		ENGINE_PTR(wchar_t, sv_password,								DUO_PTR(K_SV_PASSWORD));
		ENGINE_PTR(char, sv_rcon_password,								DUO_PTR(K_SV_RCON_PASSWORD));
		ENGINE_PTR(int32, sv_timelimit,									DUO_PTR(K_SV_TIMELIMIT));
		ENGINE_PTR(int32, halo_argv_port,								DUO_PTR(K_HALO_ARGV_PORT));
		ENGINE_PTR(int32, halo_argv_client_port,						DUO_PTR(K_HALO_ARGV_CLIENT_PORT));
	};

	//////////////////////////////////////////////////////////////////////////
	// Server
	namespace Server
	{
		ENGINE_PTR(s_network_sv_file_globals, network_sv_file_globals,			DUO_PTR(K_NETWORK_SV_FILE_GLOBALS));
		ENGINE_PTR(s_network_sv_mapcycle_globals, network_sv_mapcycle_globals,	DUO_PTR(K_NETWORK_SV_MAPCYCLE_GLOBALS));
		ENGINE_PTR(s_network_sv_globals, network_sv_globals,					DUO_PTR(K_NETWORK_SV_GLOBALS));

	#if PLATFORM_IS_DEDI
		ENGINE_PTR(int32, server_instance,			PTR_NULL, K_SERVER_INSTANCE);

		FUNC_PTR(SV_EVENT_LOG_WITH_TIME,			FUNC_PTR_NULL, K_SV_EVENT_LOG_WITH_TIME);
		FUNC_PTR(SV_EVENT_ECHO,						FUNC_PTR_NULL, K_SV_EVENT_ECHO);

		ENGINE_PTR(bool, event_type_logging,		PTR_NULL, K_EVENT_TYPE_LOGGING);
		ENGINE_PTR(bool, event_type_echoing,		PTR_NULL, K_EVENT_TYPE_ECHOING);

		ENGINE_PTR(wcstring*, log_entry_titles,		PTR_NULL, K_LOG_ENTRY_TITLES);
		ENGINE_PTR(wcstring*, log_entry_titles_ref,	PTR_NULL, K_LOG_ENTRY_TITLES_REF);

		static byte** K_EVENT_ENABLE_LOGGING_0_REFS[] =
		{
			CAST_PTR(byte**, K_EVENT_ENABLE_LOGGING_0_REF_0),
			CAST_PTR(byte**, K_EVENT_ENABLE_LOGGING_0_REF_1)
		};

		ENGINE_PTR(byte**, event_enable_logging_4_ref, PTR_NULL, K_EVENT_ENABLE_LOGGING_4_REF);
		ENGINE_PTR(byte**, event_enable_logging_8_ref, PTR_NULL, K_EVENT_ENABLE_LOGGING_8_REF);
		ENGINE_PTR(byte**, event_enable_logging_12_ref, PTR_NULL, K_EVENT_ENABLE_LOGGING_12_REF);
		
		static byte** K_EVENT_ENABLE_ECHOING_0_REFS[] =
		{
			CAST_PTR(byte**, K_EVENT_ENABLE_ECHOING_0_REF_0),
			CAST_PTR(byte**, K_EVENT_ENABLE_ECHOING_0_REF_1),
			CAST_PTR(byte**, K_EVENT_ENABLE_ECHOING_0_REF_2)
		};

		ENGINE_PTR(byte**, event_enable_echoing_2_ref, PTR_NULL, K_EVENT_ENABLE_ECHOING_2_REF);
		ENGINE_PTR(byte**, event_enable_echoing_3_ref, PTR_NULL, K_EVENT_ENABLE_ECHOING_3_REF);
		ENGINE_PTR(byte**, event_enable_echoing_4_ref, PTR_NULL, K_EVENT_ENABLE_ECHOING_4_REF);
		ENGINE_PTR(byte**, event_enable_echoing_8_ref, PTR_NULL, K_EVENT_ENABLE_ECHOING_8_REF);
		ENGINE_PTR(byte**, event_enable_echoing_12_ref, PTR_NULL, K_EVENT_ENABLE_ECHOING_12_REF);
	#endif
	};


//////////////////////////////////////////////////////////////////////////
// GameSpyApi.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_NETWORKING_GAMESPY_API
	namespace Networking
	{
		ENGINE_DPTR(s_gamespy_socket, gs_Socket,									DUO_PTR(K_GS_SOCKET));
		ENGINE_DPTR(s_gamespy_socket, gs_LoopbackSocket,							DUO_PTR(K_GS_LOOPBACKSOCKET));
		ENGINE_PTR(s_gamespy_config, gamespy_config,								DUO_PTR(K_GAMESPY_CONFIG));
		ENGINE_PTR(s_gamespy_globals, gamespy_globals,								DUO_PTR(K_GAMESPY_GLOBALS));
		ENGINE_PTR(s_gamespy_server_browser_globals, gamespy_server_browser_globals,DUO_PTR(K_GAMESPY_SERVER_BROWSER_GLOBALS));
		ENGINE_DPTR(s_gamespy_qr2, gamespy_qr2,										DUO_PTR(K_GAMESPY_QR2));
		ENGINE_PTR(bool, g_gamespy_patch_check_for_updates,							K_G_GAMESPY_PATCH_CHECK_FOR_UPDATES, PTR_NULL);

		ENGINE_PTR(s_gamespy_product, gamespy_products_list,						DUO_PTR(K_GAMESPY_PRODUCTS_LIST)); // [4]

		DATA_PTR(compiler_null_string,												DUO_PTR(K_COMPILER_NULL_STRING)); // TODO: not the best place for this...
		FUNC_PTR(GAMESPY_GET_CLIENT_KEY_HASH,										DUO_PTR(K_GAMESPY_GET_CLIENT_KEY_HASH));
		DATA_PTR(GAMESPY_PATCH_SPAWN_CHECK_FOR_UPDATES_THREAD_CALL,					K_GAMESPY_PATCH_SPAWN_CHECK_FOR_UPDATES_THREAD_CALL, DATA_PTR_NULL);

		namespace GameSpy
		{
			ENGINE_PTR(cstring, Matchup1Hostname,									DUO_PTR(K_MATCHUP1_HOSTNAME));
			ENGINE_PTR(cstring, Matchup2Hostname,									DUO_PTR(K_MATCHUP2_HOSTNAME));
			ENGINE_PTR(cstring, MASTER_ADDR_REFERENCE,								DUO_PTR(K_MASTER_ADDR_REFERENCE));
			ENGINE_PTR(cstring, MASTER_ADDR_SB_REFERENCE,							DUO_PTR(K_MASTER_ADDR_SB_REFERENCE));
			ENGINE_PTR(cstring, PTA_DEFAULT_VERCHECK_URL_REFERENCE,					DUO_PTR(K_PTA_DEFAULT_VERCHECK_URL_REFERENCE));

			FUNC_PTR(CREATE_GAMESPY_QR2_HOOK,										DUO_PTR(K_CREATE_GAMESPY_QR2_HOOK));
			FUNC_PTR(QR2_REGISTER_KEY,												DUO_PTR(K_QR2_REGISTER_KEY));

			FUNC_PTR(SBSERVER_GET_BOOL_VALUE,										K_SBSERVER_GET_BOOL_VALUE, FUNC_PTR_NULL);
			FUNC_PTR(SBSERVER_GET_INT_VALUE,										K_SBSERVER_GET_INT_VALUE, FUNC_PTR_NULL);
			FUNC_PTR(SBSERVER_GET_STRING_VALUE,										K_SBSERVER_GET_STRING_VALUE, FUNC_PTR_NULL);
		};
	};


#else
	#error Undefined engine layout include for: __EL_INCLUDE_NETWORKING
#endif