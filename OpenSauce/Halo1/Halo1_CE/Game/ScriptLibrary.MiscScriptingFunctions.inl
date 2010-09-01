/*
    Yelo: Open Sauce SDK
		Halo 1 (CE) Edition
    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
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

	result.boolean = GameState::ChangeAdvertisedGameVersion(args->version_str, args->and_game_build);

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

#if !PLATFORM_IS_DEDI
	InitializeScriptFunction(Enums::_hs_function_pp_load, 
		Postprocessing::HS_Load);
	InitializeScriptFunction(Enums::_hs_function_pp_unload, 
		Postprocessing::HS_Unload);
	InitializeScriptFunctionWithParams(Enums::_hs_function_pp_set_effect_active, 
		Postprocessing::HS_SetEffectActive);
	InitializeScriptFunctionWithParams(Enums::_hs_function_pp_set_effect_fade, 
		Postprocessing::HS_SetEffectFade);
	InitializeScriptFunctionWithParams(Enums::_hs_function_pp_get_effect_index_by_name, 
		Postprocessing::HS_GetEffectIndexByName);
	InitializeScriptFunctionWithParams(Enums::_hs_function_pp_get_effect_current_fade, 
		Postprocessing::HS_GetEffectCurrentFade);
	InitializeScriptFunctionWithParams(Enums::_hs_function_pp_get_effect_is_valid, 
		Postprocessing::HS_GetEffectIsValid);
	InitializeScriptFunctionWithParams(Enums::_hs_function_pp_get_effect_shader_variable_index_by_name, 
		Postprocessing::HS_GetEffectShaderVariableIndexByName);
	InitializeScriptFunctionWithParams(Enums::_hs_function_pp_set_effect_shader_variable_boolean, 
		Postprocessing::HS_SetEffectShaderVariableBoolean);
	InitializeScriptFunctionWithParams(Enums::_hs_function_pp_set_effect_shader_variable_integer, 
		Postprocessing::HS_SetEffectShaderVariableInteger);
	InitializeScriptFunctionWithParams(Enums::_hs_function_pp_set_effect_shader_variable_real, 
		Postprocessing::HS_SetEffectShaderVariableReal);
#else
	NullifyScriptFunction( GET_HS_FUNCTION(pp_load) );
	NullifyScriptFunction( GET_HS_FUNCTION(pp_unload) );
	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(pp_set_effect_active) );
	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(pp_set_effect_fade) );
	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(pp_get_effect_index_by_name) );
	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(pp_get_effect_current_fade) );
	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(pp_get_effect_is_valid) );
	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(pp_get_effect_shader_variable_index_by_name) );
	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(pp_set_effect_shader_variable_boolean) );
	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(pp_set_effect_shader_variable_integer) );
	NullifyScriptFunctionWithParams( GET_HS_FUNCTION(pp_set_effect_shader_variable_real) );
#endif
}