/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

static void* scripting_structure_bsp_lightmap_reset_evaluate()
{
	TagGroups::ScenarioStructureBspLightmapSetChange("default");

	return NULL;
}

static void* scripting_structure_bsp_lightmap_set_change_evaluate(void** arguments)
{
	struct s_arguments {
		cstring lightmap_set_name;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = NULL;

	result.boolean = TagGroups::ScenarioStructureBspLightmapSetChange(args->lightmap_set_name);

	return result.pointer;
}


void* scripting_game_change_version_id_evaluate(void** arguments)
{
	struct s_arguments {
		bool and_game_build;
		PAD24;
		cstring version_str;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = NULL;

	result.boolean = Networking::ChangeAdvertisedGameVersion(args->version_str, args->and_game_build);

	return result.pointer;
}


static void* scripting_game_engine_data_get_integer_evaluate(void** arguments)
{
	struct s_arguments {
		cstring data_name;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = NULL;

		 if( !strcmp(args->data_name,"type") )			result.int32 = GameEngine::Current() != NULL ? GameEngine::GlobalVariant()->game_engine_index : Enums::_game_engine_none;
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
	TypeHolder result; result.pointer = NULL;

	result.boolean = Networking::IsServer();

	return result.pointer;
}

static void* scripting_machine_is_dedi()
{
	TypeHolder result; result.pointer = NULL;

	Networking::s_network_game_server* game_server = Networking::NetworkGameServer();
	result.boolean = game_server != NULL && game_server->IsDedi();

	return result.pointer;
}


static void* scripting_abs_integer_evaluate(void** arguments)
{
	struct s_arguments {
		int32 value;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = NULL;

	result.int32 = abs(args->value);

	return result.pointer;
}
static void* scripting_abs_real_evaluate(void** arguments)
{
	struct s_arguments {
		real value;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = NULL;

	result.real = abs(args->value);

	return result.pointer;
}

static void* scripting_bitwise_and_evaluate(void** arguments)
{
	struct s_arguments {
		int32 value;
		int32 flags;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = NULL;

	result.uint32 = CAST(uint32,args->value) & CAST(uint32,args->flags);

	return result.pointer;
}
static void* scripting_bitwise_or_evaluate(void** arguments)
{
	struct s_arguments {
		int32 value;
		int32 flags;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = NULL;

	result.uint32 = CAST(uint32,args->value) | CAST(uint32,args->flags);

	return result.pointer;
}
static void* scripting_bitwise_xor_evaluate(void** arguments)
{
	struct s_arguments {
		int32 value;
		int32 flags;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = NULL;

	result.uint32 = CAST(uint32,args->value) ^ CAST(uint32,args->flags);

	return result.pointer;
}
static void* scripting_bitwise_lhs_evaluate(void** arguments)
{
	struct s_arguments {
		int32 value;
		int32 bit_count;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = NULL;

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
	TypeHolder result; result.pointer = NULL;

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
	TypeHolder result; result.pointer = NULL;

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
	TypeHolder result; result.pointer = NULL;

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
	TypeHolder result; result.pointer = NULL;

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
	TypeHolder result; result.pointer = NULL;

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
	TypeHolder result; result.pointer = NULL;

	sscanf_s(args->str, "%x", &result.uint32);

	return result.pointer;
}


static void* scripting_display_scripted_ui_widget_evaluate(void** arguments)
{
	struct s_arguments {
		int16 local_player_index;
		PAD16;
		cstring name;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = NULL;

#if !PLATFORM_IS_DEDI 
	result.boolean = UIWidget::DisplayScriptedWidget(args->local_player_index, args->name);
#endif

	return result.pointer;
}


static void InitializeMiscFunctions()
{
#if 0
	InitializeScriptFunction(Enums::_hs_function_structure_bsp_lightmap_reset, 
		scripting_structure_bsp_lightmap_reset_evaluate);
	InitializeScriptFunctionWithParams(Enums::_hs_function_structure_bsp_lightmap_set_change, 
		scripting_structure_bsp_lightmap_set_change_evaluate);
#endif

	InitializeScriptFunctionWithParams(Enums::_hs_function_game_change_version_id, 
		scripting_game_change_version_id_evaluate);

	InitializeScriptFunctionWithParams(Enums::_hs_function_game_engine_data_get_integer, 
		scripting_game_engine_data_get_integer_evaluate);

	InitializeScriptFunction(Enums::_hs_function_machine_is_host, 
		scripting_machine_is_host);
	InitializeScriptFunction(Enums::_hs_function_machine_is_dedi, 
		scripting_machine_is_dedi);

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

#if !PLATFORM_IS_DEDI
	InitializeScriptFunction(Enums::_hs_function_pp_load,
		Rasterizer::PostProcessing::Scripting::HS_Load);
	InitializeScriptFunction(Enums::_hs_function_pp_unload,
		Rasterizer::PostProcessing::Scripting::HS_Unload);
	InitializeScriptFunctionWithParams(Enums::_hs_function_pp_get_effect_instance_index_by_name,
		Rasterizer::PostProcessing::Scripting::Internal::HS_GetEffectInstanceIndexByName);
	InitializeScriptFunctionWithParams(Enums::_hs_function_pp_set_effect_instance_active,
		Rasterizer::PostProcessing::Scripting::Internal::HS_SetEffectInstanceActive);
	InitializeScriptFunctionWithParams(Enums::_hs_function_pp_set_effect_instance_fade,
		Rasterizer::PostProcessing::Scripting::Internal::HS_SetEffectInstanceFade);
	InitializeScriptFunctionWithParams(Enums::_hs_function_pp_get_effect_instance_current_fade,
		Rasterizer::PostProcessing::Scripting::Internal::HS_GetEffectInstanceCurrentFade);
	InitializeScriptFunctionWithParams(Enums::_hs_function_pp_get_effect_instance_fade_direction,
		Rasterizer::PostProcessing::Scripting::Internal::HS_GetEffectInstanceFadeDirection);
	InitializeScriptFunctionWithParams(Enums::_hs_function_pp_get_effect_index_by_name,
		Rasterizer::PostProcessing::Scripting::Internal::HS_GetEffectIndexByName);
	InitializeScriptFunctionWithParams(Enums::_hs_function_pp_get_effect_is_valid,
		Rasterizer::PostProcessing::Scripting::Internal::HS_GetEffectIsValid);
	InitializeScriptFunctionWithParams(Enums::_hs_function_pp_get_effect_shader_variable_index_by_name,
		Rasterizer::PostProcessing::Scripting::Internal::HS_GetEffectShaderVariableIndexByName);
	InitializeScriptFunctionWithParams(Enums::_hs_function_pp_set_effect_shader_variable_boolean,
		Rasterizer::PostProcessing::Scripting::Internal::HS_SetEffectShaderVariableBoolean);
	InitializeScriptFunctionWithParams(Enums::_hs_function_pp_set_effect_shader_variable_integer,
		Rasterizer::PostProcessing::Scripting::Internal::HS_SetEffectShaderVariableInteger);
	InitializeScriptFunctionWithParams(Enums::_hs_function_pp_set_effect_shader_variable_real,
		Rasterizer::PostProcessing::Scripting::Internal::HS_SetEffectShaderVariableReal);
	InitializeScriptFunctionWithParams(Enums::_hs_function_pp_set_effect_shader_instance_active, 
		Rasterizer::PostProcessing::Scripting::Internal::HS_SetEffectShaderInstanceActive);
	InitializeScriptFunctionWithParams(Enums::_hs_function_pp_bloom_set_size, 
		Rasterizer::PostProcessing::Scripting::Bloom::HS_BloomSetSize);
	InitializeScriptFunctionWithParams(Enums::_hs_function_pp_bloom_set_exposure, 
		Rasterizer::PostProcessing::Scripting::Bloom::HS_BloomSetExposure);
	InitializeScriptFunctionWithParams(Enums::_hs_function_pp_bloom_set_mix_amount, 
		Rasterizer::PostProcessing::Scripting::Bloom::HS_BloomSetMixAmount);
	InitializeScriptFunctionWithParams(Enums::_hs_function_pp_bloom_set_minimum_color, 
		Rasterizer::PostProcessing::Scripting::Bloom::HS_BloomSetMinimumColor);
	InitializeScriptFunctionWithParams(Enums::_hs_function_pp_bloom_set_maximum_color, 
		Rasterizer::PostProcessing::Scripting::Bloom::HS_BloomSetMaximumColor);

	InitializeScriptFunctionWithParams(Enums::_hs_function_display_scripted_ui_widget, 
		scripting_display_scripted_ui_widget_evaluate);

	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(sv_httpserver_log_enable) );
	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(sv_httpserver_set_connection_ban) );
	NullifyScriptFunction( GET_HS_FUNCTION(sv_httpserver_banlist) );
	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(sv_httpserver_banlist_file) );
	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(sv_httpserver_ban_ip) );
	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(sv_httpserver_unban_ip) );

	NullifyScriptFunction( GET_HS_FUNCTION(sv_mapdownload_start_server) );
	NullifyScriptFunction( GET_HS_FUNCTION(sv_mapdownload_stop_server) );
	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(sv_mapdownload_set_part_definitions_path) );
	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(sv_mapdownload_set_host) );
	NullifyScriptFunction( GET_HS_FUNCTION(sv_mapdownload_reload_map_part_definitions) );

#else
	NullifyScriptFunction( GET_HS_FUNCTION(pp_load) );
	NullifyScriptFunction( GET_HS_FUNCTION(pp_unload) );
	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(pp_get_effect_instance_index_by_name) );
	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(pp_set_effect_instance_active) );
	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(pp_set_effect_instance_fade) );
	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(pp_get_effect_instance_current_fade) );
	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(pp_get_effect_instance_fade_direction) );
	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(pp_get_effect_is_valid) );
	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(pp_get_effect_shader_variable_index_by_name) );
	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(pp_set_effect_shader_variable_boolean) );
	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(pp_set_effect_shader_variable_integer) );
	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(pp_set_effect_shader_variable_real) );
	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(pp_set_effect_shader_instance_active) );
	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(pp_bloom_set_size) );
	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(pp_bloom_set_exposure) );
	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(pp_bloom_set_mix_amount) );
	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(pp_bloom_set_minimum_color) );
	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(pp_bloom_set_maximum_color) );

	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(display_scripted_ui_widget) );

	InitializeScriptFunctionWithParams(Enums::_hs_function_sv_httpserver_log_enable,
		Networking::HTTP::Server::HTTPServerLogEnable);
	InitializeScriptFunctionWithParams(Enums::_hs_function_sv_httpserver_set_connection_ban,
		Networking::HTTP::Server::BanManager::HTTPServerSetConnectionBan);
	InitializeScriptFunction(Enums::_hs_function_sv_httpserver_banlist,
		Networking::HTTP::Server::BanManager::HTTPServerBanlist);
	InitializeScriptFunctionWithParams(Enums::_hs_function_sv_httpserver_banlist_file,
		Networking::HTTP::Server::BanManager::HTTPServerBanlistFile);
	InitializeScriptFunctionWithParams(Enums::_hs_function_sv_httpserver_ban_ip,
		Networking::HTTP::Server::BanManager::HTTPServerBanIP);
	InitializeScriptFunctionWithParams(Enums::_hs_function_sv_httpserver_unban_ip,
		Networking::HTTP::Server::BanManager::HTTPServerUnbanIP);

	InitializeScriptFunction(Enums::_hs_function_sv_mapdownload_start_server,
		Networking::HTTP::Server::MapDownload::MapDownloadStartServer);
	InitializeScriptFunction(Enums::_hs_function_sv_mapdownload_stop_server,
		Networking::HTTP::Server::MapDownload::MapDownloadStopServer);
	InitializeScriptFunctionWithParams(Enums::_hs_function_sv_mapdownload_set_part_definitions_path,
		Networking::HTTP::Server::MapDownload::MapDownloadSetPartDefinitionsPath);
	InitializeScriptFunctionWithParams(Enums::_hs_function_sv_mapdownload_set_host,
		Networking::HTTP::Server::MapDownload::MapDownloadSetHost);
	InitializeScriptFunction(Enums::_hs_function_sv_mapdownload_reload_map_part_definitions,
		Networking::HTTP::Server::MapDownload::MapDownloadReloadMapPartDefinitions);
#endif
}