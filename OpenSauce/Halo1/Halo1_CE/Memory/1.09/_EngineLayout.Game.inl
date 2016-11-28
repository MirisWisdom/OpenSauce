/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

//////////////////////////////////////////////////////////////////////////
// AI.cpp
#if __EL_INCLUDE_FILE_ID == __EL_GAME_AI
	ENGINE_DPTR(s_ai_globals_data, ai_globals,								0x81B534, 0x6E73F4);
	ENGINE_DPTR(actor_data_t, actors,										0x81B540, 0x6E7400);
	ENGINE_DPTR(swarm_data_t, swarms,										0x81B53C, 0x6E73FC);
	ENGINE_DPTR(swarm_component_data_t, swarm_components,					0x81B538, 0x6E73F8);
	ENGINE_DPTR(prop_data_t, props,											0x81B4A0, 0x6E7360);
	ENGINE_DPTR(encounter_data_t, encounters,								0x81B4A8, 0x6E7368);
	ENGINE_DPTR(squads_data_t, squads,										0x81B4AC, 0x6E736C);
	ENGINE_DPTR(platoons_data_t, platoons,									0x81B4A4, 0x6E7364);
	ENGINE_DPTR(ai_pursuit_data_t, ai_pursuits,								0x81B4B0, 0x6E7370);
	//ai_communication_dialogue_events_t
	ENGINE_DPTR(ai_communication_reply_events_t, ai_communication_replies,	0x68BC7C, 0x5F4474);
	ENGINE_DPTR(ai_conversation_data_t, ai_conversations,					0x81B4B4, 0x6E7374);

	FUNC_PTR(AI_UPDATE_HOOK,												0x42A949, 0x42A959);
	FUNC_PTR(ACTOR_ACTION_HANDLE_VEHICLE_EXIT_HOOK,							0x40B105, 0x40B115);
	FUNC_PTR(ACTOR_ACTION_HANDLE_VEHICLE_EXIT_RETN,							0x40B10A, 0x40B11A);
	FUNC_PTR(PROP_STATUS_REFRESH_HOOK,										0x41C9CE, 0x41C9DE);
	FUNC_PTR(PROP_STATUS_REFRESH_RETN,										0x41C9D4, 0x41C9E4);
	FUNC_PTR(ACTOR_INPUT_UPDATE_HOOK,										0x429CA7, 0x429CB7);
	FUNC_PTR(ACTOR_INPUT_UPDATE_RETN,										0x429CAE, 0x429CBE);


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

	FUNC_PTR(DIRECTOR_CHOOSE_GAME_PERSPECTIVE_HOOK,				0x446755, PTR_NULL);

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
	FUNC_PTR(OBSERVER_UPDATE_COMMAND_HOOK,							0x4486DA, FUNC_PTR_NULL);

	#pragma region OBSERVER_UPDATE_POSITIONS
	FUNC_PTR(OBSERVER_UPDATE_POSITIONS,								0x448DA0, FUNC_PTR_NULL);
	ENGINE_PTR(bool, OBSERVER_UPDATE_POSITIONS_no_scope_blur,		0x49754D+2, PTR_NULL);
	// Address of the call opcodes to [OBSERVER_UPDATE_POSITIONS] inside the function
	// known as [OBSERVER_TICK]
	FUNC_PTR(OBSERVER_TICK_CALL_HOOK_OBSERVER_UPDATE_POSITIONS,		0x448729, FUNC_PTR_NULL);
	#pragma endregion

	ENGINE_PTR(byte, MAX_FOV_CHECK_JMP,								0x44952D, PTR_NULL);
};


//////////////////////////////////////////////////////////////////////////
// Campaign.cpp. CLIENT BUILDS ONLY
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_CAMPAIGN
#if YELO_CAMPAIGN_UPGRADES_ENABLED
	#pragma region Scenario Paths
	static cstring** ScenarioPathsReferences[] = {
		CAST_PTR(cstring**, 0x49FBB3), CAST_PTR(cstring**, 0x49FD13), /*CAST_PTR(cstring**, 0x49FE6C),*/
		CAST_PTR(cstring**, 0x4A00BB), CAST_PTR(cstring**, 0x4A0190), CAST_PTR(cstring**, 0x4A4C1B),
	};

	static cstring** ScenarioPathsReferences2[] = {
		CAST_PTR(cstring**, 0x4A73F5), CAST_PTR(cstring**, 0x4CD10C), CAST_PTR(cstring**, 0x53C8CB),
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
		CAST_PTR(byte*, 0x49FCD4), CAST_PTR(byte*, 0x49FD2C), CAST_PTR(byte*, 0x49FD64),
		/*CAST_PTR(byte*, 0x49FED6),*/ CAST_PTR(byte*, 0x4CD0C7), CAST_PTR(byte*, 0x4CD106),
		CAST_PTR(byte*, 0x53C8C2),
	};

	static int16* CampaignLevelCountReferences2_16bit[] = { // 0xA
		CAST_PTR(int16*, 0x49FCF6), /*CAST_PTR(int16*, 0x49FEE7),*/ CAST_PTR(int16*, 0x53C98A),
	};
	#pragma endregion
#endif


//////////////////////////////////////////////////////////////////////////
// Console.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_CONSOLE
	ENGINE_DPTR(s_terminal_globals, terminal_globals,		0x64DE4C, 0x5BFC2C);
	ENGINE_PTR(s_console_globals, console_globals,			0x651F70, 0x5C3D30);

	FUNC_PTR(CONSOLE_UPDATE_HOOK,		0x4C9BE3, 0x4B3637);


//////////////////////////////////////////////////////////////////////////
// Effects.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_EFFECTS
	ENGINE_DPTR(contrail_data_t, contrails,									0x815DAC, 0x6E1C6C);
	ENGINE_DPTR(contrail_point_data_t, contrail_points,						0x815DA8, 0x6E1C68);
	ENGINE_DPTR(particle_data_t, particles,									0x815D90, 0x6E1C50);
	ENGINE_DPTR(effect_data_t, effects,										0x815D9C, 0x6E1C5C);
	ENGINE_DPTR(effect_location_data_t, effect_locations,					0x815DA0, 0x6E1C60);
	ENGINE_DPTR(particle_systems_data_t, particle_systems,					0x815D98, 0x6E1C54);
	ENGINE_DPTR(particle_system_particles_data_t, particle_system_particles,0x815D94, 0x6E1C58);

	ENGINE_DPTR(s_structure_detail_objects_data, structure_detail_objects,	0x6BDA6C, 0x6260F4);
	ENGINE_DPTR(s_structure_decals_data, structure_decals,					0x6BDA7C, 0x6260F4);
	ENGINE_DPTR(s_breakable_surface_globals_data, breakable_surface_globals,0x653CC8, 0x5C5BB0);
	ENGINE_DPTR(decals_data_t, decals,										0x815DA4, 0x6E1C64);
	ENGINE_DPTR(s_decal_globals_data, decal_globals,						0x64BA28, 0x5BD808);
	ENGINE_DPTR(s_decal_vertex_cache_data, decal_vertex_cache,				0x6B8478, PTR_NULL);

	FUNC_PTR(EFFECTS_UPDATE_HOOK, 0x451697, 0x4500C7);

	#if PLATFORM_IS_USER
		static void* K_RENDER_PARTICLES_RENDERED_PARTICLES_ARRAY_ADDRESS_LIST[] = {
			CAST_PTR(void*, 0x513ABA+0x30), CAST_PTR(void*, 0x513B4B+0x30), CAST_PTR(void*, 0x513B52+0x30), 
			CAST_PTR(void*, 0x513B79+0x30), // add4
			CAST_PTR(void*, 0x513BCB+0x30),
		};

		FUNC_PTR(GAME_INITIALIZE_MOD_PER_MAP_UPGRADE_PARTICLES,					0x45B6B7+0x10, FUNC_PTR_NULL);
		FUNC_PTR(GAME_INITIALIZE_MOD_PER_MAP_UPGRADE_EFFECTS,					0x45B6CB+0x10, FUNC_PTR_NULL);
		FUNC_PTR(GAME_INITIALIZE_MOD_PER_MAP_UPGRADE_EFFECT_LOCATIONS,			0x45B6E4+0x10, FUNC_PTR_NULL);
		FUNC_PTR(GAME_INITIALIZE_MOD_PER_MAP_UPGRADE_PARTICLE_SYSTEM_PARTICLES,	0x45B72C+0x10, FUNC_PTR_NULL);
	#endif


//////////////////////////////////////////////////////////////////////////
// EngineFunctions.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_ENGINE_FUNCTIONS
	#include __EL_INCLUDE_FILE_MACRO(_EngineLayout.Game.EngineFunctions.inl)


//////////////////////////////////////////////////////////////////////////
// GameBuildNumber.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_BUILD_NUMBER
	ENGINE_PTR(char, game_build_version,							0x60A3C8, 0x564B34);
	ENGINE_PTR(char, game_build_version_gamespy,					0x6BDD18, 0x626398);

	ENGINE_PTR(Enums::network_game_protocol_id, network_version_id1,0x4DC2D6, 0x4C3866);
	ENGINE_PTR(Enums::network_game_protocol_id, network_version_id2,0x4DC4B3, 0x4C3A43);
	ENGINE_PTR(Enums::network_game_protocol_id, network_version_id3,0x4E3FF3, 0x4CB583);

	FUNC_PTR(GAME_STATE_HEADER_IS_VALID_HOOK,						0x53BBB7+0x30, 0x4F7187+0x30);
	FUNC_PTR(GAME_STATE_HEADER_IS_VALID_HOOK_RET,					0x53BC0B+0x30, 0x4F71DB+0x30);
	FUNC_PTR(GAME_STATE_HEADER_TRY_AND_LOAD_HOOK,					0x53B96E+0x30, 0x4F6F3E+0x30);
	FUNC_PTR(GAME_STATE_HEADER_TRY_AND_LOAD_HOOK_RET_TRUE,			0x53B9CE+0x30, 0x4F6F9E+0x30);
	FUNC_PTR(GAME_STATE_HEADER_TRY_AND_LOAD_HOOK_RET_FALSE,			0x53BA89+0x30, 0x4F7059+0x30);


//////////////////////////////////////////////////////////////////////////
// GameEngine.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_GAME_ENGINE
	ENGINE_PTR(s_ctf_globals, ctf_globals,				0x64BDD8, 0x5BDBB8);
	ENGINE_PTR(s_slayer_globals, slayer_globals,		0x64C328, 0x5BE108);
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
	static uintptr_t K_GAME_STATE_BEFORE_SAVE_PROCS_CALLS[] = {
		PLATFORM_VALUE(0x4CAD3A, 0x4B4091)+0x30, PLATFORM_VALUE(0x53B870, 0x4F6E40)+0x30, 
	};
	static uintptr_t K_GAME_STATE_BEFORE_LOAD_PROCS_CALLS[] = {
		PLATFORM_VALUE(0x53B90D, 0x4F6EDD)+0x30, PLATFORM_VALUE(0x53BA53, 0x4F7023)+0x30, PLATFORM_VALUE(0x53BB45, 0x4F7115)+0x30, 
	};
	static uintptr_t K_GAME_STATE_AFTER_LOAD_PROCS_REFS[] = {
		PLATFORM_VALUE(0x53B622, 0x4F6C02)+1+0x30, PLATFORM_VALUE(0x53B918, 0x4F6EE8)+1+0x30, PLATFORM_VALUE(0x53BB6E, 0x4F713E)+1+0x30,
	};
	static uintptr_t K_GAME_STATE_AFTER_LOAD_PROCS_COUNTS[] = {
		PLATFORM_VALUE(0x53B627, 0x4F6C07)+1+0x30, PLATFORM_VALUE(0x53B91D, 0x4F6EED)+1+0x30, PLATFORM_VALUE(0x53BB73, 0x4F7143)+1+0x30,
	};

	ENGINE_PTR(s_main_globals, main_globals,	0x6B47B8, 0x61CFE0);

	ENGINE_PTR(s_physical_memory_map_globals, physical_memory_globals,	0x647478, 0x5B9278);
	FUNC_PTR(PHYSICAL_MEMORY_INITIALIZE,		0x445400, 0x4450C0);
	DATA_PTR(PHYSICAL_MEMORY_ALLOCATION_SIZE,	0x445428, 0x4450E2);

	ENGINE_PTR(s_game_state_globals, game_state_globals,			0x67DDA8, 0x5E66A4);
	ENGINE_DPTR(s_game_globals, game_globals,						0x64BAD0, 0x5BD8B0);
	ENGINE_DPTR(s_game_time_globals, game_time_globals,				0x68CD90, 0x5F55BC);
	ENGINE_DPTR(s_game_allegiance_globals, game_allegiance_globals,	0x64BAD4, 0x5BD8B4);

	ENGINE_PTR(TagGroups::s_game_globals*, global_game_globals,			0x6E2280, 0x629460);
	ENGINE_PTR(s_physics_globals, global_physics,						0x637BF4, 0x5AD5CC);
	ENGINE_PTR(s_point_physics_globals, global_point_physics,			0x653CCC, 0x5C5BB4);

	ENGINE_PTR(byte, developer_mode,									0x815DC6, 0x6E1C86);
	ENGINE_PTR(bool, transport_dumping,									0x621F60, 0x597918);
	ENGINE_PTR(char, init_txt_filename,									0x60AAF4, PTR_NULL);
	ENGINE_PTR(bool, devmode_enabled,									0x6BD17E, 0x62585E);

	FUNC_PTR(GAME_INITIALIZE_HOOK,										0x45B84B, 0x4582B0);
	FUNC_PTR(GAME_INITIALIZE_FOR_NEW_MAP_HOOK,							0x45BEEB, 0x4588B5);
	FUNC_PTR(GAME_DISPOSE_FROM_OLD_MAP_HOOK,							0x45C060, 0x458A18);
	static void** K_GAME_SCENARIO_SWITCH_BSP_CALLS[] =
	{
		PLATFORM_PTR(void**, 0x482440, 0x479F80), PLATFORM_PTR(void**, 0x4CD037, 0x4B6547), PLATFORM_PTR(void**, 0x4CD161, 0x4B6671),
		PLATFORM_PTR(void**, 0x541E68, 0x4FCE1C), PLATFORM_PTR(void**, 0x542779, 0x4FD485)
	};
	FUNC_PTR(GAME_DISPOSE_FROM_OLD_BSP_HOOK,							0x442996, 0x443526);

	FUNC_PTR(GAME_SERVER_QR2_STRING_MATCHES_GAMEVER_CALL,				0x4BA7CB, FUNC_PTR_NULL);

	FUNC_PTR(GAME_CREATE_NETWORK_SERVER,								0x4E21E0, 0x4C96C0);
	FUNC_PTR(GAME_CREATE_NETWORK_CLIENT,								0x4DBFC0, 0x4C3550);
	FUNC_PTR(GAME_CREATE_NETWORK_SERVER_CALL,							0x4E1340, FUNC_PTR_NULL);

#if PLATFORM_IS_USER
	static void** K_GAME_CREATE_NETWORK_CLIENT_CALL[] =
	{
		CAST_PTR(void**, 0x498C94), CAST_PTR(void**, 0x4A0343), CAST_PTR(void**, 0x4A03E5), 
		CAST_PTR(void**, 0x4A062E), CAST_PTR(void**, 0x4DFEF2), CAST_PTR(void**, 0x4E1450), 
		CAST_PTR(void**, 0x4E7B3E)
	};
#endif

//////////////////////////////////////////////////////////////////////////
// Lights.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_LIGHTS
	ENGINE_DPTR(lights_data_t, lights,									0x7FBB14, 0x6C7150);
	ENGINE_DPTR(s_lights_globals_data, light_game_globals,				0x6B8270, 0x620A40);
	ENGINE_DPTR(s_light_cluster_data, light_cluster,					0x7FBB20, 0x6C7160);
	ENGINE_DPTR(cluster_light_reference_data_t, cluster_light_reference,0x7FBB24, 0x6C7164);
	ENGINE_DPTR(light_cluster_reference_data_t, light_cluster_reference,0x7FBB28, 0x6C7168);


//////////////////////////////////////////////////////////////////////////
// Players.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_PLAYERS
	ENGINE_DPTR(players_data_t, players,								0x8155C0, 0x6E1480);
	ENGINE_DPTR(teams_data_t, teams,									0x8155BC, 0x6E147C);
	ENGINE_DPTR(s_players_globals_data, players_globals,				0x8155B8, 0x6E1478);
	ENGINE_DPTR(s_player_control_globals_data, player_control_globals,	0x64C3AC, 0x5BE18C);
	ENGINE_DPTR(s_player_effects_data, player_effects,					0x68C864, 0x5F5094);
	ENGINE_PTR(datum_index, machine_to_player_table,					0x64C3B0, 0x5BE190);

	FUNC_PTR(PLAYER_SPAWN__CREATE_UNIT_MP_HOOK,			0x47AD21, 0x4727A1);
	FUNC_PTR(PLAYER_SPAWN__CREATE_UNIT_MP_HOOK_RETN,	0x47AE40, 0x4728C0);

	FUNC_PTR(PLAYERS_UPDATE_BEFORE_GAME_SERVER_HOOK,	0x4773B7, 0x46EE37);
	FUNC_PTR(PLAYERS_UPDATE_BEFORE_GAME_CLIENT_HOOK,	0x4777EB, 0x46F26B);
	FUNC_PTR(PLAYERS_UPDATE_AFTER_GAME_HOOK,			0x477B7B, 0x46F5FB);
	
	//////////////////////////////////////////////////////////////////////////
	// Players.NearbyObjects
	FUNC_PTR(PLAYER_EXAMINE_NEARBY_OBJECTS_SERVER_JMP_TABLE_RETN,			0x47B268, 0x472CE8);
	ENGINE_PTR(void**, player_examine_nearby_objects_server_jmp_ptr,		0x47B248, 0x472CC8);
	ENGINE_PTR(byte, PLAYER_EXAMINE_NEARBY_OBJECTS_SERVER_LAST_ENTRY_TYPE,	0x47B242, 0x472CC2);
	ENGINE_PTR(void**, PLAYER_EXAMINE_NEARBY_OBJECTS_SERVER_JMP_TABLE,		0x47B274, 0x472CF4);

	FUNC_PTR(PLAYER_EXAMINE_NEARBY_OBJECTS_CLIENT_JMP_TABLE_RETN,			0x47B368, 0x472DE8);
	ENGINE_PTR(void**, player_examine_nearby_objects_client_jmp_ptr,		0x47B348, 0x472DC8);
	ENGINE_PTR(byte, PLAYER_EXAMINE_NEARBY_OBJECTS_CLIENT_LAST_ENTRY_TYPE,0x47B342, 0x472DC2);
	ENGINE_PTR(void**, PLAYER_EXAMINE_NEARBY_OBJECTS_CLIENT_JMP_TABLE,		0x47B374, 0x472DF4);
	
	//////////////////////////////////////////////////////////////////////////
	// Players.ActionResults
	FUNC_PTR(PLAYER_HANDLE_ACTION_JMP_TABLE_ADJUST_SIZE,	0x47C183, 0x473C03);
	FUNC_PTR(PLAYER_HANDLE_ACTION_JMP_TABLE_RETN,			0x47C47B, 0x473EFB);
	ENGINE_PTR(byte, PLAYER_HANDLE_ACTION_LAST_ENTRY_TYPE,	0x47C188, 0x473C08);
	ENGINE_PTR(void**, player_handle_action_jmp_ptr,		0x47C199, 0x473C19);
	ENGINE_PTR(void**, PLAYER_HANDLE_ACTION_JMP_TABLE,		0x47C488, 0x473F08);

	FUNC_PTR(FOLLOWING_CAMERA_GET_UNIT_CAMERA_INFO_CALL,	0x447EB2, FUNC_PTR_NULL);


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