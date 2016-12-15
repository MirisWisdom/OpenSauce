/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

void* scripting_game_change_version_id_evaluate(void** arguments)
{
	struct s_arguments {
		bool and_game_build;
		PAD24;
		cstring version_str;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = nullptr;

	result.boolean = BuildNumber::ChangeAdvertisedVersion(args->version_str, args->and_game_build);

	return result.pointer;
}


static void* scripting_game_engine_data_get_integer_evaluate(void** arguments)
{
	struct s_arguments {
		cstring data_name;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = nullptr;

		 if( !strcmp(args->data_name,"type") )			result.int32 = GameEngine::Current() != nullptr ? GameEngine::GlobalVariant()->game_engine_index : Enums::_game_engine_none;
	else if( !strcmp(args->data_name,"is_teams") )		result.int32 = CAST(int32, GameEngine::GlobalVariant()->universal_variant.teams);
	else if( !strcmp(args->data_name,"is_odd_man_out") )result.int32 = CAST(int32, GameEngine::GlobalVariant()->universal_variant.odd_man_out);
	else if( !strcmp(args->data_name,"lives") )			result.int32 = GameEngine::GlobalVariant()->universal_variant.lives;
	else if( !strcmp(args->data_name,"score_to_win") )	result.int32 = GameEngine::GlobalVariant()->universal_variant.score_to_win;

	// CTF
//	else if( !strcmp(args->data_name,"ctf:") )			result.int32 = GameEngine::GlobalVariant();

	// Oddball
	else if( !strcmp(args->data_name,"oddball:ball_count") )	result.int32 = GameEngine::GlobalVariant()->game_engine_variant.oddball.ball_count;
//	else if( !strcmp(args->data_name,"oddball:") )		result.int32 = GameEngine::GlobalVariant();

	else												result.int32 = NONE;

	return result.pointer;
}


static void* scripting_machine_is_host()
{
	TypeHolder result; result.pointer = nullptr;

	result.boolean = GameState::IsServer();

	return result.pointer;
}

static void* scripting_machine_is_dedi()
{
	TypeHolder result; result.pointer = nullptr;

	Networking::s_network_game_server* game_server = Networking::NetworkGameServer();
	result.boolean = game_server != nullptr && game_server->IsDedi();

	return result.pointer;
}


static void* scripting_abs_integer_evaluate(void** arguments)
{
	struct s_arguments {
		int32 value;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = nullptr;

	result.int32 = abs(args->value);

	return result.pointer;
}
static void* scripting_abs_real_evaluate(void** arguments)
{
	struct s_arguments {
		real value;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = nullptr;

	result.real = abs(args->value);

	return result.pointer;
}

static void* scripting_bitwise_and_evaluate(void** arguments)
{
	struct s_arguments {
		int32 value;
		int32 flags;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = nullptr;

	result.uint32 = CAST(uint32,args->value) & CAST(uint32,args->flags);

	return result.pointer;
}
static void* scripting_bitwise_or_evaluate(void** arguments)
{
	struct s_arguments {
		int32 value;
		int32 flags;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = nullptr;

	result.uint32 = CAST(uint32,args->value) | CAST(uint32,args->flags);

	return result.pointer;
}
static void* scripting_bitwise_xor_evaluate(void** arguments)
{
	struct s_arguments {
		int32 value;
		int32 flags;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = nullptr;

	result.uint32 = CAST(uint32,args->value) ^ CAST(uint32,args->flags);

	return result.pointer;
}
static void* scripting_bitwise_lhs_evaluate(void** arguments)
{
	struct s_arguments {
		int32 value;
		int32 bit_count;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = nullptr;

	if(args->bit_count >= 0 && args->bit_count < BIT_COUNT(int32))
		result.uint32 = CAST(uint32,args->value) << args->bit_count;

	return result.pointer;
}
static void* scripting_bitwise_rhs_evaluate(void** arguments)
{
	struct s_arguments {
		int32 value;
		int32 bit_count;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = nullptr;

	if(args->bit_count >= 0 && args->bit_count < BIT_COUNT(int32))
		result.uint32 = CAST(uint32,args->value) >> args->bit_count;

	return result.pointer;
}
static void* scripting_bit_test_evaluate(void** arguments)
{
	struct s_arguments {
		int32 value;
		int16 bit_index;
		PAD16;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = nullptr;

	if(args->bit_index >= 0 && args->bit_index < BIT_COUNT(int32))
		result.boolean = TEST_FLAG(CAST(uint32,args->value), args->bit_index);

	return result.pointer;
}
static void* scripting_bit_toggle_evaluate(void** arguments)
{
	struct s_arguments {
		int32 value;
		int16 bit_index;
		PAD16;
		bool add_or_remove;
		PAD24;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = nullptr;

	if(args->bit_index >= 0 && args->bit_index < BIT_COUNT(int32))
	{
		uint32 value = CAST(uint32,args->value);
		result.uint32 = SET_FLAG(value, args->bit_index, args->add_or_remove);
	}
	else result.uint32 = CAST(uint32,args->value);

	return result.pointer;
}
static void* scripting_bit_flags_test_evaluate(void** arguments)
{
	struct s_arguments {
		int32 value;
		int32 flags;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = nullptr;

	result.boolean = (CAST(uint32,args->value) & CAST(uint32,args->flags)) != 0;

	return result.pointer;
}
static void* scripting_bit_flags_toggle_evaluate(void** arguments)
{
	struct s_arguments {
		int32 value;
		int32 flags;
		bool add_or_remove;
		PAD24;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = nullptr;

	{
		uint32 value = CAST(uint32,args->value);
		uint32 flags = CAST(uint32,args->flags);
		result.uint32 = args->add_or_remove ? value | flags : value & ~flags;
	}

	return result.pointer;
}
static void* scripting_hex_string_to_long_evaluate(void** arguments)
{
	struct s_arguments {
		cstring str;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = nullptr;

	sscanf_s(args->str, "%x", &result.uint32);

	return result.pointer;
}


static void* scripting_display_scripted_ui_widget_evaluate(void** arguments)
{
	struct s_arguments {
		cstring name;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = nullptr;

#if !PLATFORM_IS_DEDI
	result.boolean = UIWidgets::DisplayScriptedWidget(args->name);
#endif

	return result.pointer;
}

static void* scripting_play_bink_movie_evaluate(void** arguments)
{
	struct s_arguments {
		cstring name;
	}* args = CAST_PTR(s_arguments*, arguments);

#if !PLATFORM_IS_DEDI
	if(GameState::IsLocal())
		blam::bink_playback_start(args->name);
#endif

	return nullptr;
}


static void InitializeMiscFunctions()
{
	InitializeScriptFunctionWithParams(Enums::_hs_function_game_change_version_id, 
		scripting_game_change_version_id_evaluate);

	InitializeScriptFunctionWithParams(Enums::_hs_function_game_engine_data_get_integer, 
		scripting_game_engine_data_get_integer_evaluate);

	InitializeScriptFunction(Enums::_hs_function_machine_is_host, 
		scripting_machine_is_host);
	InitializeScriptFunction(Enums::_hs_function_machine_is_dedi, 
		scripting_machine_is_dedi);


	YELO_INIT_SCRIPT_FUNCTION_USER(Enums::_hs_function_pp_load,
		Rasterizer::PostProcessing::Scripting::HS_Load);
	YELO_INIT_SCRIPT_FUNCTION_USER(Enums::_hs_function_pp_unload,
		Rasterizer::PostProcessing::Scripting::HS_Unload);
	YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_USER(Enums::_hs_function_pp_get_effect_instance_index_by_name,
		Rasterizer::PostProcessing::Scripting::Internal::HS_GetEffectInstanceIndexByName);
	YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_USER(Enums::_hs_function_pp_set_effect_instance_active,
		Rasterizer::PostProcessing::Scripting::Internal::HS_SetEffectInstanceActive);
	YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_USER(Enums::_hs_function_pp_set_effect_instance_fade,
		Rasterizer::PostProcessing::Scripting::Internal::HS_SetEffectInstanceFade);
	YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_USER(Enums::_hs_function_pp_get_effect_instance_current_fade,
		Rasterizer::PostProcessing::Scripting::Internal::HS_GetEffectInstanceCurrentFade);
	YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_USER(Enums::_hs_function_pp_get_effect_instance_fade_direction,
		Rasterizer::PostProcessing::Scripting::Internal::HS_GetEffectInstanceFadeDirection);
	YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_USER(Enums::_hs_function_pp_get_effect_index_by_name,
		Rasterizer::PostProcessing::Scripting::Internal::HS_GetEffectIndexByName);
	YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_USER(Enums::_hs_function_pp_get_effect_is_valid,
		Rasterizer::PostProcessing::Scripting::Internal::HS_GetEffectIsValid);
	YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_USER(Enums::_hs_function_pp_get_effect_shader_variable_index_by_name,
		Rasterizer::PostProcessing::Scripting::Internal::HS_GetEffectShaderVariableIndexByName);
	YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_USER(Enums::_hs_function_pp_set_effect_shader_variable_boolean,
		Rasterizer::PostProcessing::Scripting::Internal::HS_SetEffectShaderVariableBoolean);
	YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_USER(Enums::_hs_function_pp_set_effect_shader_variable_integer,
		Rasterizer::PostProcessing::Scripting::Internal::HS_SetEffectShaderVariableInteger);
	YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_USER(Enums::_hs_function_pp_set_effect_shader_variable_real,
		Rasterizer::PostProcessing::Scripting::Internal::HS_SetEffectShaderVariableReal);
	YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_USER(Enums::_hs_function_pp_set_effect_shader_instance_active, 
		Rasterizer::PostProcessing::Scripting::Internal::HS_SetEffectShaderInstanceActive);
	YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_USER(Enums::_hs_function_pp_bloom_set_size, 
		Rasterizer::PostProcessing::Scripting::Bloom::HS_BloomSetSize);
	YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_USER(Enums::_hs_function_pp_bloom_set_exposure, 
		Rasterizer::PostProcessing::Scripting::Bloom::HS_BloomSetExposure);
	YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_USER(Enums::_hs_function_pp_bloom_set_mix_amount, 
		Rasterizer::PostProcessing::Scripting::Bloom::HS_BloomSetMixAmount);
	YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_USER(Enums::_hs_function_pp_bloom_set_minimum_color, 
		Rasterizer::PostProcessing::Scripting::Bloom::HS_BloomSetMinimumColor);
	YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_USER(Enums::_hs_function_pp_bloom_set_maximum_color, 
		Rasterizer::PostProcessing::Scripting::Bloom::HS_BloomSetMaximumColor);
	YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_USER(Enums::_hs_function_ai_transform_actor, 
		AI::Transform::HS_AITransformActor);
	YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_USER(Enums::_hs_function_ai_transform_actors, 
		AI::Transform::HS_AITransformActors);
	YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_USER(Enums::_hs_function_ai_transform_actors_by_type, 
		AI::Transform::HS_AITransformActorsByType);
	YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_USER(Enums::_hs_function_ai_actor_is_transforming, 
		AI::Transform::HS_AIActorIsTransforming);

	//////////////////////////////////////////////////////////////////////////
	// Numbers
	InitializeScriptFunctionWithParams(Enums::_hs_function_abs_integer, 
		scripting_abs_integer_evaluate);
	InitializeScriptFunctionWithParams(Enums::_hs_function_abs_real, 
		scripting_abs_real_evaluate);
	InitializeScriptFunctionWithParams(Enums::_hs_function_bitwise_and, 
		scripting_bitwise_and_evaluate);
	InitializeScriptFunctionWithParams(Enums::_hs_function_bitwise_or, 
		scripting_bitwise_or_evaluate);
	InitializeScriptFunctionWithParams(Enums::_hs_function_bitwise_xor, 
		scripting_bitwise_xor_evaluate);
	InitializeScriptFunctionWithParams(Enums::_hs_function_bitwise_lhs, 
		scripting_bitwise_lhs_evaluate);
	InitializeScriptFunctionWithParams(Enums::_hs_function_bitwise_rhs, 
		scripting_bitwise_rhs_evaluate);
	InitializeScriptFunctionWithParams(Enums::_hs_function_bit_test, 
		scripting_bit_test_evaluate);
	InitializeScriptFunctionWithParams(Enums::_hs_function_bit_toggle, 
		scripting_bit_toggle_evaluate);
	InitializeScriptFunctionWithParams(Enums::_hs_function_bit_flags_test, 
		scripting_bit_flags_test_evaluate);
	InitializeScriptFunctionWithParams(Enums::_hs_function_bit_flags_toggle, 
		scripting_bit_flags_toggle_evaluate);
	InitializeScriptFunctionWithParams(Enums::_hs_function_hex_string_to_long, 
		scripting_hex_string_to_long_evaluate);
	//////////////////////////////////////////////////////////////////////////

	YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_USER(Enums::_hs_function_display_scripted_ui_widget, 
		scripting_display_scripted_ui_widget_evaluate);
	YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_USER(Enums::_hs_function_play_bink_movie, 
		scripting_play_bink_movie_evaluate);


	YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_DEDI(Enums::_hs_function_sv_httpserver_set_thread_count,
		Networking::HTTP::Server::HTTPServerSetThreadCount);
	YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_DEDI(Enums::_hs_function_sv_httpserver_set_root,
		Networking::HTTP::Server::HTTPServerSetRoot);
	YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_DEDI(Enums::_hs_function_sv_httpserver_set_throttle,
		Networking::HTTP::Server::HTTPServerSetThrottle);
	YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_DEDI(Enums::_hs_function_sv_httpserver_set_ports,
		Networking::HTTP::Server::HTTPServerSetPorts);
	YELO_INIT_SCRIPT_FUNCTION_DEDI(Enums::_hs_function_sv_httpserver_show_config,
		Networking::HTTP::Server::HTTPServerShowConfig);
	YELO_INIT_SCRIPT_FUNCTION_DEDI(Enums::_hs_function_sv_httpserver_start,
		Networking::HTTP::Server::HTTPServerStart);
	YELO_INIT_SCRIPT_FUNCTION_DEDI(Enums::_hs_function_sv_httpserver_stop,
		Networking::HTTP::Server::HTTPServerStop);

	YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_DEDI(Enums::_hs_function_sv_httpserver_log_enable,
		Networking::HTTP::Server::HTTPServerLogEnable);
	YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_DEDI(Enums::_hs_function_sv_httpserver_set_connection_ban,
		Networking::HTTP::Server::BanManager::HTTPServerSetConnectionBan);
	YELO_INIT_SCRIPT_FUNCTION_DEDI(Enums::_hs_function_sv_httpserver_banlist,
		Networking::HTTP::Server::BanManager::HTTPServerBanlist);
	YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_DEDI(Enums::_hs_function_sv_httpserver_banlist_file,
		Networking::HTTP::Server::BanManager::HTTPServerBanlistFile);
	YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_DEDI(Enums::_hs_function_sv_httpserver_ban_ip,
		Networking::HTTP::Server::BanManager::HTTPServerBanIP);
	YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_DEDI(Enums::_hs_function_sv_httpserver_unban_ip,
		Networking::HTTP::Server::BanManager::HTTPServerUnbanIP);

	YELO_INIT_SCRIPT_FUNCTION_DEDI(Enums::_hs_function_sv_mapdownload_start_server,
		Networking::HTTP::Server::MapDownload::MapDownloadStartServer);
	YELO_INIT_SCRIPT_FUNCTION_DEDI(Enums::_hs_function_sv_mapdownload_stop_server,
		Networking::HTTP::Server::MapDownload::MapDownloadStopServer);
	YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_DEDI(Enums::_hs_function_sv_mapdownload_set_part_definitions_path,
		Networking::HTTP::Server::MapDownload::MapDownloadSetPartDefinitionsPath);
	YELO_INIT_SCRIPT_FUNCTION_WITH_PARAMS_DEDI(Enums::_hs_function_sv_mapdownload_set_host,
		Networking::HTTP::Server::MapDownload::MapDownloadSetHost);
	YELO_INIT_SCRIPT_FUNCTION_DEDI(Enums::_hs_function_sv_mapdownload_reload_map_part_definitions,
		Networking::HTTP::Server::MapDownload::MapDownloadReloadMapPartDefinitions);

	// Depreceated
	Scripting::NullifyScriptFunctionWithParams(Enums::_hs_function_scenario_faux_zones_reset);
	Scripting::NullifyScriptFunctionWithParams(Enums::_hs_function_scenario_faux_zone_current_switch_variant);
	Scripting::NullifyScriptFunctionWithParams(Enums::_hs_function_scenario_faux_zone_switch_variant);
	Scripting::NullifyScriptFunctionWithParams(Enums::_hs_function_scenario_faux_zone_switch_sky);	
}