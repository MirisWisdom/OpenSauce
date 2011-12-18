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

	//result.boolean = Networking::ChangeAdvertisedGameVersion(args->version_str, args->and_game_build);

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


static void InitializeMiscFunctions()
{
	InitializeScriptFunction(Enums::_hs_function_structure_bsp_lightmap_reset, 
		scripting_structure_bsp_lightmap_reset_evaluate);
	InitializeScriptFunctionWithParams(Enums::_hs_function_structure_bsp_lightmap_set_change, 
		scripting_structure_bsp_lightmap_set_change_evaluate);

	InitializeScriptFunctionWithParams(Enums::_hs_function_game_change_version_id, 
		scripting_game_change_version_id_evaluate);

	InitializeScriptFunctionWithParams(Enums::_hs_function_game_engine_data_get_integer, 
		scripting_game_engine_data_get_integer_evaluate);

	InitializeScriptFunction(Enums::_hs_function_machine_is_host, 
		scripting_machine_is_host);
	InitializeScriptFunction(Enums::_hs_function_machine_is_dedi, 
		scripting_machine_is_dedi);

	InitializeScriptFunctionWithParams(Enums::_hs_function_abs_integer, 
		scripting_abs_integer_evaluate);
	InitializeScriptFunctionWithParams(Enums::_hs_function_abs_real, 
		scripting_abs_real_evaluate);

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
#endif
}