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

//////////////////////////////////////////////////////////////////////////
// AI.cpp
#if __EL_INCLUDE_FILE_ID == __EL_GAME_AI
	ENGINE_DPTR(s_ai_globals_data, ai_globals,								0x81B534, 0x6E73F4);
	ENGINE_DPTR(t_actor_data, actors,										0x81B540, 0x6E7400);
	ENGINE_DPTR(t_swarm_data, swarms,										0x81B53C, 0x6E73FC);
	ENGINE_DPTR(t_swarm_component_data, swarm_components,					0x81B538, 0x6E73F8);
	ENGINE_DPTR(t_prop_data, props,											0x81B4A0, 0x6E7360);
	ENGINE_DPTR(t_encounter_data, encounters,								0x81B4A8, 0x6E7368);
	ENGINE_DPTR(s_squad_data, squads,										0x81B4AC, 0x6E736C);
	ENGINE_DPTR(s_platoon_data, platoons,									0x81B4A4, 0x6E7364);
	ENGINE_DPTR(t_ai_pursuit_data, ai_pursuits,								0x81B4B0, 0x6E7370);
	//ai_communication_dialogue_data
	ENGINE_DPTR(s_ai_communication_replies_data, ai_communication_replies,	0x68BC7C, 0x5F4474);
	ENGINE_DPTR(t_ai_conversation_data, ai_conversations,					0x81B4B4, 0x6E7374);

	FUNC_PTR(AI_UPDATE_HOOK, 0x42A949, 0x42A959);


//////////////////////////////////////////////////////////////////////////
// Camera.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_CAMERA
namespace Camera
{
	ENGINE_PTR(s_observer, observers,								0x6475AC, PTR_NULL);
	ENGINE_PTR(s_camera_script_globals_data, camera_script_globals,	0x621F90, PTR_NULL);
	ENGINE_DPTR(s_director_scripting_data, director_scripting,		0x816DDC, PTR_NULL);
	ENGINE_DPTR(s_cinematic_globals_data, cinematic_globals,		0x68C85C, PTR_NULL);
	ENGINE_PTR(s_director_data, global_director,					0x6474B0, PTR_NULL);

	#pragma region old third person shit
	#if 0 // enable for old 3rd person camera hack
		FUNC_PTR(OBSERVER_UPDATE_COMMAND_HOOK,						0x4486DA, FUNC_PTR_NULL);

		FUNC_PTR(DIRECTOR_GET_PERSPECTIVE_DEFAULT_SET,				0x44661F, FUNC_PTR_NULL);
		FUNC_PTR(HUD_INITIALIZE_FOR_NEW_MAP_DEFAULT_SET_SHOW_HUD,	0x4ACAEC, FUNC_PTR_NULL);
		FUNC_PTR(HUD_ADD_ITEM_MESSAGE_DEFAULT_SET_SHOW_MSG,			0x4B1A6F, FUNC_PTR_NULL);
	#endif
	#pragma endregion
};

namespace Fov
{
	#pragma region OBSERVER_UPDATE_COMMAND
	FUNC_PTR(OBSERVER_UPDATE_COMMAND, 0x448640, FUNC_PTR_NULL);
		// Address of the call opcodes to [OBSERVER_UPDATE_COMMAND] inside the function
		// known as [OBSERVER_UPDATE]
		FUNC_PTR(OBSERVER_UPDATE_CALL_HOOK_OBSERVER_UPDATE_COMMAND, 0x448413, FUNC_PTR_NULL);
	#pragma endregion

	#pragma region OBSERVER_UPDATE_POSITIONS
	FUNC_PTR(OBSERVER_UPDATE_POSITIONS, 0x448DA0, FUNC_PTR_NULL);
		ENGINE_PTR(bool, OBSERVER_UPDATE_POSITIONS_no_scope_blur, 0x49754F, PTR_NULL);
		// Address of the call opcodes to [OBSERVER_UPDATE_POSITIONS] inside the function
		// known as [OBSERVER_TICK]
		FUNC_PTR(OBSERVER_TICK_CALL_HOOK_OBSERVER_UPDATE_POSITIONS, 0x448729, FUNC_PTR_NULL);
	#pragma endregion

	ENGINE_PTR(real*, MAX, 0x449524, PTR_NULL);
};


//////////////////////////////////////////////////////////////////////////
// Campaign.cpp. CLIENT BUILDS ONLY
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_CAMPAIGN

	#pragma region Scenario Paths
	static cstring** ScenarioPathsReferences[] = {
		CAST_PTR(cstring**, 0x49FBB3), CAST_PTR(cstring**, 0x49FD13), /*CAST_PTR(cstring**, 0x49FE6C),*/
		CAST_PTR(cstring**, 0x4A00BB), CAST_PTR(cstring**, 0x4A0190), CAST_PTR(cstring**, 0x4A4C1B),
	};

	static cstring** ScenarioPathsReferences2[] = {
		CAST_PTR(cstring**, 0x4A73F5), CAST_PTR(cstring**, 0x4CD10A), CAST_PTR(cstring**, 0x53C8CB),
		CAST_PTR(cstring**, 0x53C994),
	};
	#pragma endregion

	#pragma region Scenario Help
	FUNC_PTR(DISPLAY_SCENARIO_HELP_HOOK,		0x49C3BA, FUNC_PTR_NULL);
		FUNC_PTR(DISPLAY_SCENARIO_HELP_SUCCESS,	0x49C4E6, FUNC_PTR_NULL);
		FUNC_PTR(DISPLAY_SCENARIO_HELP_FAIL,	0x49C539, FUNC_PTR_NULL);
	#pragma endregion

	#pragma region Campaign Level Data
	static void** CampaignLevelDataReferences[] = {
		CAST_PTR(void**, 0x49FABB), CAST_PTR(void**, 0x49FBBA), CAST_PTR(void**, 0x49FCEE),
		/*CAST_PTR(void**, 0x49FDFB),*/ /*CAST_PTR(void**, 0x49FE73),*/ /*CAST_PTR(void**, 0x49FEDF),*/
		CAST_PTR(void**, 0x49FF39),
	};

	static size_t* CampaignLevelDataSizeReferences[] = { // 0x14
		CAST_PTR(size_t*, 0x49FAB6), CAST_PTR(size_t*, 0x49FDF6), CAST_PTR(size_t*, 0x49FF34),
	};
	#pragma endregion

	#pragma region Campaign Level Count
	static byte* CampaignLevelCountReferences_8bit[] = { // 0x9
		/*CAST_PTR(byte*, 0x49FF05),*/ CAST_PTR(byte*, 0x4CD0F9),
	};

	static int16* CampaignLevelCountReferences_16bit[] = { // 0x9
		CAST_PTR(int16*, 0x49FB81),
	};

	static int32* CampaignLevelCountReferences_32bit[] = { // 0x9
		CAST_PTR(int32*, 0x49FB89), /*CAST_PTR(int32*, 0x49FF09),*/
	};

	//////////////////////////////////////////////////////////////////////////

	static byte* CampaignLevelCountReferences2_8bit[] = { // 0xA
		CAST_PTR(byte*, 0x49FCD8), CAST_PTR(byte*, 0x49FD2C), CAST_PTR(byte*, 0x49FD64),
		/*CAST_PTR(byte*, 0x49FED6),*/ CAST_PTR(byte*, 0x4CD0C9), CAST_PTR(byte*, 0x4CD106),
		CAST_PTR(byte*, 0x53C8C2),
	};

	static int16* CampaignLevelCountReferences2_16bit[] = { // 0xA
		CAST_PTR(int16*, 0x49FCF6), /*CAST_PTR(int16*, 0x49FEE7),*/ CAST_PTR(int16*, 0x53C98A),
	};
	#pragma endregion


//////////////////////////////////////////////////////////////////////////
// Console.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_CONSOLE
	ENGINE_DPTR(s_terminal_globals, terminal_globals,	0x64DE4C, 0x5BFC2C);
	ENGINE_PTR(s_console_globals, console_globals,		0x651F70, 0x5C3D30);

	FUNC_PTR(CONSOLE_UPDATE_HOOK,		0x4C9BE3, 0x4B3637);
	FUNC_PTR(CONSOLE_PROCESS_COMMAND,	0x4C9F40, 0x4B3990); // also in EngineFunctions.cpp


//////////////////////////////////////////////////////////////////////////
// Effects.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_EFFECTS
	ENGINE_DPTR(t_contrail_data, contrails,									0x815DAC, 0x6E1C6C);
	ENGINE_DPTR(t_contrail_point_data, contrail_points,						0x815DA8, 0x6E1C68);
	ENGINE_DPTR(t_particle_data, particles,									0x815D90, 0x6E1C50);
	ENGINE_DPTR(t_effect_data, effects,										0x815D9C, 0x6E1C5C);
	ENGINE_DPTR(t_effect_location_data, effect_locations,					0x815DA0, 0x6E1C60);
	ENGINE_DPTR(t_particle_systems_data, particle_systems,					0x815D98, 0x6E1C54);
	ENGINE_DPTR(t_particle_system_particles_data, particle_system_particles,0x815D94, 0x6E1C58);

	ENGINE_DPTR(s_structure_detail_objects_data, structure_detail_objects,	0x6BDA6C, 0x6260F4);
	ENGINE_DPTR(s_structure_decals_data, structure_decals,					0x6BDA7C, 0x626104);
	ENGINE_DPTR(s_breakable_surface_globals_data, breakable_surface_globals,0x653CC8, 0x5C5BB0);
	ENGINE_DPTR(t_decals_data, decals,										0x815DA4, 0x6E1C64);
	ENGINE_DPTR(s_decal_globals_data, decal_globals,						0x64BA28, 0x5BD808);
	ENGINE_DPTR(s_decal_vertex_cache_data, decal_vertex_cache,				0x6B8478, PTR_NULL);

	FUNC_PTR(EFFECTS_UPDATE_HOOK, 0x451697, 0x4500C7);


//////////////////////////////////////////////////////////////////////////
// EngineFunctions.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_ENGINE_FUNCTIONS
	#include __EL_INCLUDE_FILE_MACRO(_EngineLayout.Game.EngineFunctions.inl)


//////////////////////////////////////////////////////////////////////////
// GameEngine.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_GAME_ENGINE
	ENGINE_PTR(s_ctf_globals, ctf_globals,				0x64BDD8, 0x5BDBB8);
	ENGINE_PTR(s_slayer_globals, slayer_globals,		0x64C2E8, 0x5BE108);
	ENGINE_PTR(s_oddball_globals, oddball_globals,		0x64C098, 0x5BDE78);
	ENGINE_PTR(s_king_globals, king_globals,			0x64BE10, 0x5BDBF0);
	ENGINE_PTR(s_race_globals, race_globals,			0x64C1E0, 0x5BDFC0);

	ENGINE_PTR(s_game_variant, global_variant,			0x68CC68, 0x5F5498);

	ENGINE_DPTR(game_engine_definition, game_engines,	0x6238C8, 0x599288);
	ENGINE_DPTR(game_engine_definition, game_engine,	0x68CD44, 0x5F5574);

	#pragma region New Engines functions
	//////////////////////////////////////////////////////////////////////////
	// FixHudRenderUnitInterface()
	FUNC_PTR(HUD_RENDER_UNIT_INTERFACE_JMP_TABLE,			0x4B4A84, FUNC_PTR_NULL);
	ENGINE_PTR(void**, hud_render_unit_interface_jmp_ptr,	0x4B485C, PTR_NULL);

	//////////////////////////////////////////////////////////////////////////
	// FixFunc4F7440()
	FUNC_PTR(Func4F7440JmpTable,							0x4F764C, 0x4DE6AC);
	ENGINE_PTR(void**, Func4F7440JmpPtr,					0x4F7565, 0x4DE5C5);

	//////////////////////////////////////////////////////////////////////////
	// FixFunc4F7580()
	FUNC_PTR(Func4F7580JmpTable,							0x4F774C, 0x4DE7AC);
	ENGINE_PTR(void**, Func4F7580JmpPtr,					0x4F76C3, 0x4DE723);

	//////////////////////////////////////////////////////////////////////////
	// Initialize()
	ENGINE_PTR(game_engine_definition**, game_engine_init_mod, 0x45CEA4, 0x459854);
	#pragma endregion

	FUNC_PTR(GAME_ENGINE_INITIALIZE_FOR_NEW_MAP_HOOK,	0x45CFD8, 0x459988);
	FUNC_PTR(GAME_ENGINE_UPDATE_HOOK,					0x460F61, 0x45BDCB);


//////////////////////////////////////////////////////////////////////////
// GameState.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_GAME_STATE
	ENGINE_PTR(s_main_globals, main_globals,	0x6B47B8, 0x61CFE0);

	ENGINE_PTR(s_physical_memory_map_globals, physical_memory_globals,	0x647478, 0x5B9278);
	FUNC_PTR(PHYSICAL_MEMORY_INITIALIZE,		0x445400, 0x4450C0);
	DATA_PTR(PHYSICAL_MEMORY_ALLOCATION_SIZE,	0x445428, 0x4450E2);

	ENGINE_PTR(s_game_state_globals, game_state_globals,			0x67DDA8, 0x5E66A4);
	ENGINE_DPTR(s_game_globals, game_globals,						0x64BAD0, 0x5BD8B0);
	ENGINE_DPTR(s_game_time_globals, game_time_globals,				0x68CD90, 0x5F55BC);
	ENGINE_DPTR(s_game_allegiance_globals, game_allegiance_globals,	0x64BAD4, 0x5BD8B4);
	ENGINE_DPTR(s_scenario_globals, scenario_globals,				0x6E2274, 0x629454);

	ENGINE_PTR(TagGroups::scenario*, global_scenario,					0x6E226C, 0x62944C);
	ENGINE_PTR(void*, global_game_globals,								0x6E2280, 0x629460);
	ENGINE_PTR(void*, global_bsp3d,										0x6E2270, 0x629450);
	ENGINE_PTR(void*, global_collision_bsp,								0x6E2278, 0x629458);
	ENGINE_PTR(TagGroups::structure_bsp*, global_structure_bsp,			0x6E227C, 0x62945C);
	ENGINE_PTR(real, global_gravity,									0x637BF4, 0x5AD5CC);

	ENGINE_PTR(datum_index, global_scenario_index,						0x6397DC, 0x5AD85C);
	ENGINE_PTR(int16, structure_bsp_index,								0x6397E0, 0x5AD860);
	ENGINE_PTR(byte, developer_mode,									0x815DC6, 0x6E1C86);
	ENGINE_PTR(char, game_build_version,								0x60A3C8, 0x564B34);
	ENGINE_PTR(char, game_build_version_gamespy,						0x6BDD18, 0x626398);
	ENGINE_PTR(bool, transport_dumping,									0x621F60, 0x597918);
	ENGINE_PTR(char, init_txt_filename,									0x60AAF4, PTR_NULL);
	ENGINE_PTR(bool, devmode_enabled,									0x6BD17E, 0x62585E);

	FUNC_PTR(GAME_INITIALIZE_HOOK,										0x45B84B, 0x4582B0);
	FUNC_PTR(GAME_INITIALIZE_FOR_NEW_MAP_HOOK,							0x45BEEB, 0x4588B5);
	FUNC_PTR(GAME_DISPOSE_FROM_OLD_MAP_HOOK,							0x45C060, 0x458A18);

	ENGINE_PTR(long_enum, game_version_id1,								0x4DC2D6, 0x4C3866);
	ENGINE_PTR(long_enum, game_version_id2,								0x4DC4B3, 0x4C3A43);
	ENGINE_PTR(long_enum, game_version_id3,								0x4E3FF3, 0x4CB583);

	DATA_PTR(PE_DATA_SIZE, 0x215000, 0x171000);


//////////////////////////////////////////////////////////////////////////
// Lights.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_LIGHTS
	ENGINE_DPTR(t_lights_data, lights,									0x7FBB14, 0x6C7150);
	ENGINE_DPTR(s_lights_globals_data, light_game_globals,				0x6B8270, 0x620A40);
	ENGINE_DPTR(s_light_cluster_data, light_cluster,					0x7FBB20, 0x6C7160);
	ENGINE_DPTR(t_cluster_light_reference_data, cluster_light_reference,0x7FBB24, 0x6C7164);
	ENGINE_DPTR(t_light_cluster_reference_data, light_cluster_reference,0x7FBB28, 0x6C7168);


//////////////////////////////////////////////////////////////////////////
// Players.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_PLAYERS
	ENGINE_DPTR(t_players_data, players,								0x8155C0, 0x6E1480);
	ENGINE_DPTR(t_teams_data, teams,									0x8155BC, 0x6E147C);
	ENGINE_DPTR(s_players_globals_data, players_globals,				0x8155B8, 0x6E1478);
	ENGINE_DPTR(s_player_control_globals_data, player_control_globals,	0x64C3AC, 0x5BE18C);
	ENGINE_DPTR(s_player_effects_data, player_effects,					0x68C864, 0x5F5094);
	ENGINE_PTR(datum_index, machine_to_player_table,					0x64C3B0, 0x5BE190);

	FUNC_PTR(PLAYERS_UPDATE_BEFORE_GAME_SERVER_HOOK,	0x4773B7, 0x46EE37);
	FUNC_PTR(PLAYERS_UPDATE_BEFORE_GAME_CLIENT_HOOK,	0x4777EB, 0x46F26B);
	FUNC_PTR(PLAYERS_UPDATE_AFTER_GAME_HOOK,			0x477B7B, 0x46F5FB);


//////////////////////////////////////////////////////////////////////////
// Scripting.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_SCRIPTING
	#include __EL_INCLUDE_FILE_MACRO(_EngineLayout.Game.HS.inl)
/////////////////////////////////////////////////////////////////////////
// ScriptLibrary.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_SCRIPT_LIBRARY
	#include __EL_INCLUDE_FILE_MACRO(_EngineLayout.Game.HS.inl)

#else
	#error Undefined engine layout include for: __EL_INCLUDE_GAME
#endif