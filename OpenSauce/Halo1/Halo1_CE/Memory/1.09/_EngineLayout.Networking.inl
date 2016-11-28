/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

//////////////////////////////////////////////////////////////////////////
// MDP.hpp
#if __EL_INCLUDE_FILE_ID == __EL_NETWORKING_MDP
	FUNC_PTR(MDP_FIELD_ENCODE_ARRAY, 0x4ECD00, 0x4D5F90);
	FUNC_PTR(MDP_FIELD_DECODE_ARRAY, 0x4ECD40, 0x4D5FD0);

	ENGINE_PTR(field_properties_definition, message_delta_field_integer_small,							0x6229B8, 0x598370);
	ENGINE_PTR(field_properties_definition, message_delta_field_integer_medium,							0x622A28, 0x5983E0);
	ENGINE_PTR(field_properties_definition, message_delta_field_integer_large,							0x622600, 0x597FB8);
	ENGINE_PTR(field_properties_definition, message_delta_field_real,									0x622770, 0x598128);
	ENGINE_PTR(field_properties_definition, message_delta_field_boolean,								0x622950, 0x598308);
	ENGINE_PTR(field_properties_definition, message_delta_field_ascii_character,						0x635E20, 0x5AB7F8);
	ENGINE_PTR(field_properties_definition, message_delta_field_time,									0x635F68, 0x5AB940);
	ENGINE_PTR(field_properties_definition, message_delta_field_grenade_counts,							0x635C58, 0x5AB630);
	ENGINE_PTR(field_properties_definition, message_delta_field_locality_reference_position,			0x6314B0, 0x5A6E50);
	ENGINE_PTR(field_properties_definition, message_delta_field_digital_throttle,						0x633660, 0x5A8FF8);
	ENGINE_PTR(field_properties_definition, message_delta_field_fixed_width_weapon_index,				0x633738, 0x5A90D0);
	ENGINE_PTR(field_properties_definition, message_delta_field_fixed_width_grenade_index,				0x6337A0, 0x5A9138);
	ENGINE_PTR(field_properties_definition, message_delta_field_item_placement_position,				0x635FD0, 0x5AB9A8);

	ENGINE_PTR(field_properties_definition, message_delta_field_local_player_update_sequence_number,	0x634EC8, 0x5AA8A0);
	ENGINE_PTR(field_properties_definition, message_delta_field_remote_player_update_sequence_number,	0x634FA8, 0x5AA980);
	ENGINE_PTR(field_properties_definition, message_delta_field_remote_player_action_update_baseline_id,0x634F38, 0x5AA910);
	ENGINE_PTR(field_properties_definition, message_delta_field_update_tick_count,						0x633568, 0x5A8F00);
	ENGINE_PTR(field_properties_definition, message_delta_field_update_id,								0x6334F8, 0x5A8E90);
	ENGINE_PTR(field_properties_definition, message_delta_field_fixed_width_3bits,						0x635CC8, 0x5AB6A0);
	ENGINE_PTR(field_properties_definition, message_delta_field_fixed_width_6bits,						0x631740, 0x5A70E0);
	ENGINE_PTR(field_properties_definition, message_delta_field_fixed_width_1bit,						0x6336D0, 0x5A9068);
	ENGINE_PTR(field_properties_definition, message_delta_field_fixed_width_normal_4bit,				0x635D40, 0x5AB718);
	ENGINE_PTR(field_properties_definition, message_delta_field_fixed_width_normal_8bit,				0x630A68, 0x5A6408);
	ENGINE_PTR(field_properties_definition, message_delta_field_fixed_width_normal_16bit,				0x635DB8, 0x5AB790);
	ENGINE_PTR(field_properties_definition, message_delta_field_object_index,							0x622698, 0x598050);
	ENGINE_PTR(field_properties_definition, message_delta_field_player_index,							0x622AC0, 0x598478);
	ENGINE_PTR(field_properties_definition, message_delta_field_definition_index,						0x62E4D0, 0x5A3E70);
	ENGINE_PTR(field_properties_definition, message_delta_field_point2d,								0x635E90, 0x5AB868);
	ENGINE_PTR(field_properties_definition, message_delta_field_point3d,								0x630AD8, 0x5A6478);
	ENGINE_PTR(field_properties_definition, message_delta_field_vector2d,								0x635F00, 0x5AB8D8);
	ENGINE_PTR(field_properties_definition, message_delta_field_vector3d,								0x622708, 0x5980C0);
	ENGINE_PTR(field_properties_definition, message_delta_field_control_flags,							0x6335F8, 0x5A8F90);
	ENGINE_PTR(field_properties_definition, message_delta_field_angular_velocity,						0x6311E8, 0x5A6B88);
	ENGINE_PTR(field_properties_definition, message_delta_field_translational_velocity,					0x630E60, 0x5A6800);
	ENGINE_PTR(field_properties_definition, message_delta_field_map_name,								0x6339E8, 0x5A9380);
	ENGINE_PTR(field_properties_definition, message_delta_field_network_map,							0x633A78, 0x5A9410);
	ENGINE_PTR(field_properties_definition, message_delta_field_network_player_name,					0x633AE8, 0x5A9480);
	ENGINE_PTR(field_properties_definition, message_delta_field_network_player,							0x633BC8, 0x5A9560);
	ENGINE_PTR(field_properties_definition, message_delta_field_network_game_name,						0x633C38, 0x5A95D0);
	ENGINE_PTR(field_properties_definition, message_delta_field_network_game_players,					0x633CB0, 0x5A9648);
	ENGINE_PTR(field_properties_definition, message_delta_field_game_variant,							0x622F78, 0x598930);
	ENGINE_PTR(field_properties_definition, message_delta_field_game_variant_human_readable_description,0x622E28, 0x5987E0);
	ENGINE_PTR(field_properties_definition, message_delta_field_universal_variant,						0x622D40, 0x5986F8);
	ENGINE_PTR(field_properties_definition, message_delta_field_universal_variant_flags,				0x622B50, 0x598508);
	ENGINE_PTR(field_properties_definition, message_delta_field_game_engine_variant,					0x622DB8, 0x598770);
	ENGINE_PTR(field_properties_definition, message_delta_field_game_variant_flags,						0x622EB8, 0x598870);
	ENGINE_PTR(field_properties_definition, message_delta_field_parameters_protocol_array,				0x636808, 0x5AC1E0);
	ENGINE_PTR(field_properties_definition, message_delta_field_hud_chat_message,						0x62E388, 0x5A3D28);
	ENGINE_PTR(field_properties_definition, message_delta_field_hud_chat_message_ptr,					0x62E3F8, 0x5A3D98);
	ENGINE_PTR(field_properties_definition, message_delta_field_object_change_colors,					0x636530, 0x5ABF08);
	ENGINE_PTR(field_properties_definition, message_delta_field_ctf_score_array,						0x6233A0, 0x598D60);
	ENGINE_PTR(field_properties_definition, message_delta_field_race_score_array,						0x623AC8, 0x599488);
	ENGINE_PTR(field_properties_definition, message_delta_field_king_score_array,						0x623548, 0x598F08);
	ENGINE_PTR(field_properties_definition, message_delta_field_oddball_score_array,					0x623970, 0x599330);
	ENGINE_PTR(field_properties_definition, message_delta_field_oddball_owner_array,					0x6239E8, 0x5993A8);
	ENGINE_PTR(field_properties_definition, message_delta_field_slayer_score_array,						0x623BC8, 0x599588);
	ENGINE_PTR(field_properties_definition, message_delta_field_damage_data_flags,						0x622810, 0x5981C8);
	ENGINE_PTR(field_properties_definition, message_delta_field_motd_text,								0x634E10, 0x5AA7E8);
	ENGINE_PTR(field_properties_definition, message_delta_field_rcon_response,							0x634D58, 0x5AA6F0);
	ENGINE_PTR(field_properties_definition, message_delta_field_rcon_password,							0x634C20, 0x5AA5B8);
	ENGINE_PTR(field_properties_definition, message_delta_field_rcon_command,							0x634C90, 0x5AA628);
	//ENGINE_PTR(field_properties_definition, message_delta_field_, 0, PTR_UNKNOWN);


	ENGINE_PTR(field_reference_set, message_delta_empty_field_set,								0x6225E0, 0x597F98);

	//ENGINE_PTR(field_reference_set, message_delta_guaranteed_object_header_field_set,				0x0, 0x0);
	ENGINE_PTR(field_reference_set, message_delta_game_object_header_field_set,					0x631250, 0x5A6BF0);
	ENGINE_PTR(field_reference_set, message_delta_game_object_timestamped_header_field_set,		0x636060, 0x5ABA38);
	ENGINE_PTR(field_reference_set, message_delta_biped_update_header_field_set,				0x636440, 0x5ABE18);
	ENGINE_PTR(field_reference_set, message_delta_ctf_update_header_field_set,					0x623408, 0x598DC8);
	ENGINE_PTR(field_reference_set, message_delta_client_game_update_header_field_set,			0x6338EC, 0x5A9284);
	ENGINE_PTR(field_reference_set, message_delta_player_score_update_header_field_set,			0x6230DC, 0x598A9C);
	ENGINE_PTR(field_reference_set, message_delta_remote_player_action_update_header_field_set,	0x635130, 0x5AAB08);
	ENGINE_PTR(field_reference_set, message_delta_remote_player_position_update_header_field_set,0x6352C8, 0x5AACA0);
	ENGINE_PTR(field_reference_set, message_delta_remote_player_total_update_header_field_set,	0x6353F0, 0x5AADC8);
	//ENGINE_PTR(field_reference_set, message_delta__header_field_set, 0, PTR_UNKNOWN);


//////////////////////////////////////////////////////////////////////////
// MessageDefinitions.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_NETWORKING_MESSAGE_DEFINITIONS

	// Currently there is nothing to add here


//////////////////////////////////////////////////////////////////////////
// MessageDeltas.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_NETWORKING_MESSAGE_DELTAS
	ENGINE_PTR(s_message_delta_parameters, message_delta_parameters,				0x635994, 0x5AB36C);
	ENGINE_PTR(field_type_definition, message_delta_global_field_type_list,			0x6359B8, 0x5AB390);
	ENGINE_PTR(Enums::message_delta_encoding_class, g_message_delta_encoding_class,	0x636A18, 0x5AC3F0);
	ENGINE_PTR(exposed_parameters_t, g_exposed_params,								0x653610, 0x5C54F8);
	ENGINE_PTR(s_parameters_protocol_globals, mdp_parameters_protocol_globals,		0x6B8260, 0x620A30);

	ENGINE_PTR(byte, mdp_packet_buffer_received_data,			0x7FCE20, 0x6C8D20);
	ENGINE_PTR(byte, mdp_packet_buffer_sent_data,				0x80CE20, 0x6D8D20);

	ENGINE_DPTR(message_delta_definition, message_delta_packets,0x5FBA40, 0x5573C0);

	#ifndef YELO_NO_NETWORK
		#pragma region MessageDeltaPointerReferences
		static void** MessageDeltaPointerReferences[] = {
			CAST_PTR(void**, PLATFORM_VALUE(0x4EFFA3,0x4D9203)), CAST_PTR(void**, PLATFORM_VALUE(0x4F0024,0x4D9284)), CAST_PTR(void**, PLATFORM_VALUE(0x4F01BD,0x4D941D)),
			CAST_PTR(void**, PLATFORM_VALUE(0x4F03E3,0x4D9643)), CAST_PTR(void**, PLATFORM_VALUE(0x4F0470,0x4D96D0)), CAST_PTR(void**, PLATFORM_VALUE(0x4F0600,0x4D9860)),
			CAST_PTR(void**, PLATFORM_VALUE(0x4F079C,0x4D99FC)), CAST_PTR(void**, PLATFORM_VALUE(0x4F0809,0x4D9A69)), CAST_PTR(void**, PLATFORM_VALUE(0x4F089E,0x4D9AFE)),
			CAST_PTR(void**, PLATFORM_VALUE(0x4F08C8,0x4D9B28)), CAST_PTR(void**, PLATFORM_VALUE(0x4F0A7F,0x4D9CDF)), CAST_PTR(void**, PLATFORM_VALUE(0x4F0C59,0x4D9EB9)),
			CAST_PTR(void**, PLATFORM_VALUE(0x4F0D11,0x4D9F71)), CAST_PTR(void**, PLATFORM_VALUE(0x4F0DD9,0x4DA039)), CAST_PTR(void**, PLATFORM_VALUE(0x4F0E69,0x4DA0C9)),
			CAST_PTR(void**, PLATFORM_VALUE(0x4F0EB7,0x4DA117)), CAST_PTR(void**, PLATFORM_VALUE(0x4F0F25,0x4DA185)), CAST_PTR(void**, PLATFORM_VALUE(0x4F0F43,0x4DA1A3)),
		};
		#pragma endregion

		#pragma region MessageDeltaPointerEndChecks
		static void** MessageDeltaPointerEndChecks[] = {
			CAST_PTR(void**, PLATFORM_VALUE(0x4EFFB3,0x4D9213)), CAST_PTR(void**, PLATFORM_VALUE(0x4F0047,0x4D92A7)), CAST_PTR(void**, PLATFORM_VALUE(0x4F0407,0x4D9667)),
		};

		static byte* MessageDeltaTypeCountChecks8bit[] = {
			CAST_PTR(byte*, PLATFORM_VALUE(0x4F0BCD,0x4D9E2D)),
		};
		#pragma endregion

		FUNC_PTR(NETWORK_GAME_CLIENT_GAME_SETTINGS_UPDATED,			0x4DCD60+0x30, 0x4C42D0+0x30);
		FUNC_PTR(NETWORK_GAME_CLIENT_GAME_SETTINGS_UPDATED_CALL1,	0x4DF4C0+0x30, 0x4C69C1+0x30);
		FUNC_PTR(NETWORK_GAME_CLIENT_GAME_SETTINGS_UPDATED_CALL2,	0x4E1FB1+0x30, 0x4C9491+0x30);

		FUNC_PTR(NETWORK_GAME_CLIENT_HANDLE_MESSAGE_DELTA_MESSAGE_BODY_CALL,0x4DEEFA, 0x4C63FA);
			DATA_PTR(DONT_SEND_OBJECT_NEW_MSG,								0x6B7370, 0x61FB38);
			FUNC_PTR(NETWORK_GAME_CLIENT_HANDLE_MESSAGE_DELTA_MESSAGE_BODY,	0x4DD8E0, 0x4C4DE0);


		ENGINE_PTR(Networking::s_network_game_server, global_network_game_server_data, 0x7FC340, 0x6C7980);
		ENGINE_PTR(Networking::s_network_game_client, global_network_game_client_data, 0x80DE20, 0x6D9D20);

		FUNC_PTR(MDPI_ENCODE,										0x4F05D0, 0x4D9830);
		FUNC_PTR(MDP_DECODE_STATELESS_ITERATED,						0x4F0220, 0x4D9480);
		FUNC_PTR(MDP_DECODE_INCREMENTAL_ITERATED,					0x4F0260+0x30, 0x4D94C0+0x30);
		FUNC_PTR(MDP_DISCARD_ITERATION_BODY,						0x4F0300, 0x4D9560);
	#endif


//////////////////////////////////////////////////////////////////////////
// Networking.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_NETWORKING_NETWORKING
	ENGINE_PTR(s_update_client_globals, update_client_globals,		0x692EB8, 0x5FB6E8);
	ENGINE_PTR(s_update_server_globals, update_server_globals,		0x68CDA8, 0x5F55D8);
	ENGINE_PTR(network_update_globals, network_update_globals,		0x624A68, 0x59A408);
	ENGINE_DPTR(s_network_game_server, global_network_game_server,	0x6B7384, 0x61FB4C);
	ENGINE_DPTR(s_network_game_client, global_network_game_client,	0x6B7388, 0x61FB50);

	FUNC_PTR(NETWORK_GAME_ADD_PLAYER,									0x4E1AF0, 0x4C8F70);
	FUNC_PTR(NETWORK_GAME_SERVER_ADD_PLAYER_TO_GAME__HOOK_ADD_PLAYER,	0x4E2EDA, 0x4CA44A);
	FUNC_PTR(NETWORK_GAME_REMOVE_PLAYER,								0x4E1C50, 0x4C90D0);
	// just a few bytes after this gets called is another call to ngrp, but its for the s_network_client's game member
	FUNC_PTR(CLIENT_MACHINE_CLEANUP__HOOK_REMOVE_PLAYER,				0x4E4813, 0x4CBE13);

	FUNC_PTR(NETWORK_CONNECTION_WRITE,									0x4E0440, 0x4C7920);
	FUNC_PTR(NETWORK_GAME_SERVER_SEND_REJECTION_MESSAGE,				0x4E4250+0x30, 0x4CB810+0x30);
	FUNC_PTR(NETWORK_GAME_SERVER_HOLDUP_NEW_CLIENT,						0x4E4330+0x30, 0x4CB900+0x30);
	FUNC_PTR(NETWORK_GAME_SERVER_SEND_MESSAGE_TO_MACHINE,				0x4E50E0, 0x4CC700);
	FUNC_PTR(NETWORK_GAME_SERVER_SEND_MESSAGE_TO_ALL_MACHINES,			0x4E5140+0x30, 0x4CC760+0x30);
	FUNC_PTR(NETWORK_GAME_SERVER_SEND_MESSAGE_TO_ALL_MACHINES_INGAME,	0x4E5200+0x30, 0x4CC820+0x30);


//////////////////////////////////////////////////////////////////////////
// Server.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_NETWORKING_SERVER
	namespace Networking
	{
		ENGINE_PTR(wchar_t, sv_password,								0x6B73A4, 0x61FB6C);
		ENGINE_PTR(char, sv_rcon_password,								0x6B74C0, 0x61FC8C);
		ENGINE_PTR(int32, sv_timelimit,									0x634C18, 0x5AA5B0);
		ENGINE_PTR(int32, halo_argv_port,								0x633808, 0x5A91A0);
		ENGINE_PTR(int32, halo_argv_client_port,						0x63380C, 0x5A91A4);
	};

	//////////////////////////////////////////////////////////////////////////
	// Server
	namespace Server
	{
		ENGINE_PTR(s_network_sv_file_globals, network_sv_file_globals,			0x6534EC, 0x5C529C);
		ENGINE_PTR(s_network_sv_mapcycle_globals, network_sv_mapcycle_globals,	0x6230CC, 0x598A8C);
		ENGINE_PTR(s_network_sv_globals, network_sv_globals,					0x6B72B4, 0x61FB64);

	#if PLATFORM_IS_DEDI
		ENGINE_PTR(int32, server_instance,			PTR_NULL, 0x625880);

		FUNC_PTR(SV_EVENT_LOG_WITH_TIME,			FUNC_PTR_NULL, 0x4D17E0);
		FUNC_PTR(SV_EVENT_ECHO,						FUNC_PTR_NULL, 0x4D18F0);

		ENGINE_PTR(bool, event_type_logging,		PTR_NULL, 0x5C53CC);
		ENGINE_PTR(bool, event_type_echoing,		PTR_NULL, 0x5C53DC);

		ENGINE_PTR(wcstring*, log_entry_titles,		PTR_NULL, 0x5AA7A0);
		ENGINE_PTR(wcstring*, log_entry_titles_ref,PTR_NULL, 0x4D186D);

		static byte** K_EVENT_ENABLE_LOGGING_0_REFS[] =
		{ CAST_PTR(byte**, 0x4D0808), CAST_PTR(byte**, 0x4D180A) };

		ENGINE_PTR(byte**, event_enable_logging_4_ref, PTR_NULL, 0x4D0812);
		ENGINE_PTR(byte**, event_enable_logging_8_ref, PTR_NULL, 0x4D0823);
		ENGINE_PTR(byte**, event_enable_logging_12_ref, PTR_NULL, 0x4D083E);
		
		static byte** K_EVENT_ENABLE_ECHOING_0_REFS[] =
		{ CAST_PTR(byte**, 0x4D07F9), CAST_PTR(byte**, 0x4D084A), CAST_PTR(byte**, 0x4D1908) };

		ENGINE_PTR(byte**, event_enable_echoing_2_ref, PTR_NULL, 0x4D0856);
		ENGINE_PTR(byte**, event_enable_echoing_3_ref, PTR_NULL, 0x4D0850);
		ENGINE_PTR(byte**, event_enable_echoing_4_ref, PTR_NULL, 0x4D0818);
		ENGINE_PTR(byte**, event_enable_echoing_8_ref, PTR_NULL, 0x4D0829);
		ENGINE_PTR(byte**, event_enable_echoing_12_ref, PTR_NULL, 0x4D0844);
	#endif
	};


//////////////////////////////////////////////////////////////////////////
// GameSpyApi.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_NETWORKING_GAMESPY_API
	namespace Networking
	{
		ENGINE_DPTR(s_gamespy_socket, gs_Socket,									0x68C4A0, 0x5F4CD0);
		ENGINE_DPTR(s_gamespy_socket, gs_LoopbackSocket,							0x68C4A4, 0x5F4CD4);
		ENGINE_PTR(s_gamespy_config, gamespy_config,								0x63AD04, 0x5AED34);
		ENGINE_PTR(s_gamespy_globals, gamespy_globals,								0x6BDA88, 0x626100);
		ENGINE_PTR(s_gamespy_server_browser_globals, gamespy_server_browser_globals,0x6B4500, 0x61CD2C);
		ENGINE_DPTR(s_gamespy_qr2, gamespy_qr2,										0x6BDC28+0xE8, 0x6262F0+0x98);
		ENGINE_PTR(bool, g_gamespy_patch_check_for_updates,							0x62E101, PTR_NULL);

		ENGINE_PTR(s_gamespy_product, gamespy_products_list,						0x6BE4E8, 0x626968); // [4]

		DATA_PTR(compiler_null_string,			0x5F363C, 0x54F2FC); // TODO: not the best place for this...
		FUNC_PTR(GAMESPY_GET_CLIENT_KEY_HASH,	0x579CD0, 0x5283D0);
		DATA_PTR(GAMESPY_PATCH_SPAWN_CHECK_FOR_UPDATES_THREAD_CALL,					0x4A7A30+0x30, DATA_PTR_NULL);

		namespace GameSpy
		{
			ENGINE_PTR(cstring, Matchup1Hostname,									0x61EDF0, 0x571058);
			ENGINE_PTR(cstring, Matchup2Hostname,									0x61EDF4, 0x57105C);
			ENGINE_PTR(cstring, MASTER_ADDR_REFERENCE,								0x5BB82B+0x140+1, 0x52D40B+0xB0+1);
			ENGINE_PTR(cstring, MASTER_ADDR_SB_REFERENCE,							0x5C3D65+0x140+1, 0x535935+0xB0+1);
			ENGINE_PTR(cstring, PTA_DEFAULT_VERCHECK_URL_REFERENCE,					0x5C15A4+0x140+1, 0x5312A4+0xB0+1);

			FUNC_PTR(CREATE_GAMESPY_QR2_HOOK,	0x57B5EA+0x30, 0x529D0A+0x30);
			FUNC_PTR(QR2_REGISTER_KEY,			0x5C0E40+0x40, 0x530B40+0xB0);

			FUNC_PTR(SBSERVER_GET_BOOL_VALUE,	0x5BC910, FUNC_PTR_NULL);
			FUNC_PTR(SBSERVER_GET_INT_VALUE,	0x5BD050, FUNC_PTR_NULL);
			FUNC_PTR(SBSERVER_GET_STRING_VALUE,	0x5BC8D0, FUNC_PTR_NULL);
		};
	};


#else
	#error Undefined engine layout include for: __EL_INCLUDE_NETWORKING
#endif