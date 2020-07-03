/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

//////////////////////////////////////////////////////////////////////////
// AI.cpp
#if __EL_INCLUDE_FILE_ID == __EL_GAME_AI
	ENGINE_DPTR(s_ai_globals_data, ai_globals,								0x81B454, 0x6E7354);
	ENGINE_DPTR(actor_data_t, actors,										0x81B460, 0x6E7360);
	ENGINE_DPTR(swarm_data_t, swarms,										0x81B45C, 0x6E735C);
	ENGINE_DPTR(swarm_component_data_t, swarm_components,					0x81B458, 0x6E7358);
	ENGINE_DPTR(prop_data_t, props,											0x81B3C0, 0x6E72C0);
	ENGINE_DPTR(encounter_data_t, encounters,								0x81B3C8, 0x6E72C8);
	ENGINE_DPTR(squads_data_t, squads,										0x81B3CC, 0x6E72CC);
	ENGINE_DPTR(platoons_data_t, platoons,									0x81B3C4, 0x6E72C4);
	ENGINE_DPTR(ai_pursuit_data_t, ai_pursuits,								0x81B3D0, 0x6E72D0);
	//ai_communication_dialogue_events_t
	ENGINE_DPTR(ai_communication_reply_events_t, ai_communication_replies,	0x68BB94, 0x5F43DC);
	ENGINE_DPTR(ai_conversation_data_t, ai_conversations,					0x81B3D4, 0x6E72D4);

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
	ENGINE_PTR(s_observer, observers,								0x6474C4, PTR_NULL);
	ENGINE_PTR(s_camera_script_globals_data, camera_script_globals,	0x621F90, PTR_NULL);
	ENGINE_DPTR(s_director_scripting_data, director_scripting,		0x816CFC, PTR_NULL);
	ENGINE_DPTR(s_cinematic_globals_data, cinematic_globals,		0x68C774, PTR_NULL);
	ENGINE_PTR(s_director_data, global_director,					0x6473C8, PTR_NULL);

	FUNC_PTR(DIRECTOR_CHOOSE_GAME_PERSPECTIVE_HOOK,					0x446745, PTR_NULL);

	#pragma region old third person shit
	#if 0 // enable for old 3rd person camera hack
		FUNC_PTR(OBSERVER_UPDATE_COMMAND_HOOK,						0x4486CA, FUNC_PTR_NULL);

		FUNC_PTR(DIRECTOR_GET_PERSPECTIVE_DEFAULT_SET,				0x44660F, FUNC_PTR_NULL);
		FUNC_PTR(HUD_INITIALIZE_FOR_NEW_MAP_DEFAULT_SET_SHOW_HUD,	0x4ACABC, FUNC_PTR_NULL);
		FUNC_PTR(HUD_ADD_ITEM_MESSAGE_DEFAULT_SET_SHOW_MSG,			0x4B1A3F, FUNC_PTR_NULL);
	#endif
	#pragma endregion
};

namespace Fov
{
	FUNC_PTR(OBSERVER_UPDATE_COMMAND_HOOK,							0x4486CA, FUNC_PTR_NULL);

	#pragma region OBSERVER_UPDATE_POSITIONS
	FUNC_PTR(OBSERVER_UPDATE_POSITIONS,								0x448D90, FUNC_PTR_NULL);
	ENGINE_PTR(bool, OBSERVER_UPDATE_POSITIONS_no_scope_blur,		0x49753D+2, PTR_NULL);
	// Address of the call opcodes to [OBSERVER_UPDATE_POSITIONS] inside the function
	// known as [OBSERVER_TICK]
	FUNC_PTR(OBSERVER_TICK_CALL_HOOK_OBSERVER_UPDATE_POSITIONS,		0x448719, FUNC_PTR_NULL);
	#pragma endregion

	ENGINE_PTR(byte, MAX_FOV_CHECK_JMP,								0x44951D, PTR_NULL);
};


//////////////////////////////////////////////////////////////////////////
// Campaign.cpp. CLIENT BUILDS ONLY
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_CAMPAIGN
#if YELO_CAMPAIGN_UPGRADES_ENABLED
	#pragma region Scenario Paths
	static cstring** ScenarioPathsReferences[] = {
		CAST_PTR(cstring**, 0x49FB83), CAST_PTR(cstring**, 0x49FCE3), /*CAST_PTR(cstring**, 0x49FE3C),*/
		CAST_PTR(cstring**, 0x4A008B), CAST_PTR(cstring**, 0x4A0160), CAST_PTR(cstring**, 0x4A4BEB),
	};

	static cstring** ScenarioPathsReferences2[] = {
		CAST_PTR(cstring**, 0x4A73C5), CAST_PTR(cstring**, 0x4CD0DC), CAST_PTR(cstring**, 0x53C89B),
		CAST_PTR(cstring**, 0x53C964),
	};
	#pragma endregion

	#pragma region Scenario Help
	FUNC_PTR(DISPLAY_SCENARIO_HELP_HOOK,		0x49C38A, FUNC_PTR_NULL);
		FUNC_PTR(DISPLAY_SCENARIO_HELP_SUCCESS,	0x49C4B6, FUNC_PTR_NULL);
		FUNC_PTR(DISPLAY_SCENARIO_HELP_FAIL,	0x49C509, FUNC_PTR_NULL);
	#pragma endregion

	#pragma region Campaign Level Data
	static void** CampaignLevelDataReferences[] = {
		CAST_PTR(void**, 0x49FA8B), CAST_PTR(void**, 0x49FB8A), CAST_PTR(void**, 0x49FCBE),
		/*CAST_PTR(void**, 0x49FDCB),*/ /*CAST_PTR(void**, 0x49FE43),*/ /*CAST_PTR(void**, 0x49FEAF),*/
		CAST_PTR(void**, 0x49FF09),
	};

	static size_t* CampaignLevelDataSizeReferences[] = { // 0x14
		CAST_PTR(size_t*, 0x49FA86), CAST_PTR(size_t*, 0x49FDC6), CAST_PTR(size_t*, 0x49FF04),
	};
	#pragma endregion

	#pragma region Campaign Level Count
	static byte* CampaignLevelCountReferences_8bit[] = { // 0x9
		/*CAST_PTR(byte*, 0x49FED5),*/ CAST_PTR(byte*, 0x4CD0C9),
	};

	static int16* CampaignLevelCountReferences_16bit[] = { // 0x9
		CAST_PTR(int16*, 0x49FB51),
	};

	static int32* CampaignLevelCountReferences_32bit[] = { // 0x9
		CAST_PTR(int32*, 0x49FB59), /*CAST_PTR(int32*, 0x49FED9),*/
	};

	//////////////////////////////////////////////////////////////////////////

	static byte* CampaignLevelCountReferences2_8bit[] = { // 0xA
		CAST_PTR(byte*, 0x49FCA4), CAST_PTR(byte*, 0x49FCFC), CAST_PTR(byte*, 0x49FD34),
		/*CAST_PTR(byte*, 0x49FEA6),*/ CAST_PTR(byte*, 0x4CD097), CAST_PTR(byte*, 0x4CD0D6),
		CAST_PTR(byte*, 0x53C892),
	};

	static int16* CampaignLevelCountReferences2_16bit[] = { // 0xA
		CAST_PTR(int16*, 0x49FCC6), /*CAST_PTR(int16*, 0x49FEB7),*/ CAST_PTR(int16*, 0x53C95A),
	};
	#pragma endregion
#endif


//////////////////////////////////////////////////////////////////////////
// Console.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_CONSOLE
	ENGINE_DPTR(s_terminal_globals, terminal_globals,		0x64DD64, 0x5BFB94);
	ENGINE_PTR(s_console_globals, console_globals,			0x651E88, 0x5C3C98);

	FUNC_PTR(CONSOLE_UPDATE_HOOK,		0x4C9BB3, 0x4B3607);


//////////////////////////////////////////////////////////////////////////
// Effects.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_EFFECTS
	ENGINE_DPTR(contrail_data_t, contrails,									0x815CCC, 0x6E1BCC);
	ENGINE_DPTR(contrail_point_data_t, contrail_points,						0x815CC8, 0x6E1BC8);
	ENGINE_DPTR(particle_data_t, particles,									0x815CB0, 0x6E1BB0);
	ENGINE_DPTR(effect_data_t, effects,										0x815CBC, 0x6E1BBC);
	ENGINE_DPTR(effect_location_data_t, effect_locations,					0x815CC0, 0x6E1BC0);
	ENGINE_DPTR(particle_systems_data_t, particle_systems,					0x815CB8, 0x6E1BB4);
	ENGINE_DPTR(particle_system_particles_data_t, particle_system_particles,0x815CB4, 0x6E1BB8);

	ENGINE_DPTR(s_structure_detail_objects_data, structure_detail_objects,	0x6BD984, 0x62604C);
	ENGINE_DPTR(s_structure_decals_data, structure_decals,					0x6BD994, 0x62605C);
	ENGINE_DPTR(s_breakable_surface_globals_data, breakable_surface_globals,0x653BE0, 0x5C5B18);
	ENGINE_DPTR(decals_data_t, decals,										0x815CC4, 0x6E1BC4);
	ENGINE_DPTR(s_decal_globals_data, decal_globals,						0x64B940, 0x5BD770);
	ENGINE_DPTR(s_decal_vertex_cache_data, decal_vertex_cache,				0x6B8390, PTR_NULL);

	FUNC_PTR(EFFECTS_UPDATE_HOOK, 0x451687, 0x4500B7);

	#if PLATFORM_IS_USER
		static void* K_RENDER_PARTICLES_RENDERED_PARTICLES_ARRAY_ADDRESS_LIST[] = {
			CAST_PTR(void*, 0x513ABA), CAST_PTR(void*, 0x513B4B), CAST_PTR(void*, 0x513B52), 
			CAST_PTR(void*, 0x513B79), // add4
			CAST_PTR(void*, 0x513BCB),
		};

		FUNC_PTR(GAME_INITIALIZE_MOD_PER_MAP_UPGRADE_PARTICLES,					0x45B6B7, FUNC_PTR_NULL);
		FUNC_PTR(GAME_INITIALIZE_MOD_PER_MAP_UPGRADE_EFFECTS,					0x45B6CB, FUNC_PTR_NULL);
		FUNC_PTR(GAME_INITIALIZE_MOD_PER_MAP_UPGRADE_EFFECT_LOCATIONS,			0x45B6E4, FUNC_PTR_NULL);
		FUNC_PTR(GAME_INITIALIZE_MOD_PER_MAP_UPGRADE_PARTICLE_SYSTEM_PARTICLES,	0x45B72C, FUNC_PTR_NULL);
	#endif


//////////////////////////////////////////////////////////////////////////
// EngineFunctions.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_ENGINE_FUNCTIONS
	#include __EL_INCLUDE_FILE_MACRO(_EngineLayout.Game.EngineFunctions.inl)


//////////////////////////////////////////////////////////////////////////
// GameBuildNumber.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_BUILD_NUMBER
	ENGINE_PTR(char, game_build_version,							0x60A3E8, 0x564B74);
	ENGINE_PTR(char, game_build_version_gamespy,					0x6BDC30, 0x6262F8);

	ENGINE_PTR(Enums::network_game_protocol_id, network_version_id1,0x4DC2A6, 0x4C3836);
	ENGINE_PTR(Enums::network_game_protocol_id, network_version_id2,0x4DC483, 0x4C3A13);
	ENGINE_PTR(Enums::network_game_protocol_id, network_version_id3,0x4E3FC3, 0x4CB553);

	FUNC_PTR(GAME_STATE_HEADER_IS_VALID_HOOK,						0x53BBB7, 0x4F7187);
	FUNC_PTR(GAME_STATE_HEADER_IS_VALID_HOOK_RET,					0x53BC0B, 0x4F71DB);
	FUNC_PTR(GAME_STATE_HEADER_TRY_AND_LOAD_HOOK,					0x53B96E, 0x4F6F3E);
	FUNC_PTR(GAME_STATE_HEADER_TRY_AND_LOAD_HOOK_RET_TRUE,			0x53B9CE, 0x4F6F9E);
	FUNC_PTR(GAME_STATE_HEADER_TRY_AND_LOAD_HOOK_RET_FALSE,			0x53BA89, 0x4F7059);


//////////////////////////////////////////////////////////////////////////
// GameEngine.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_GAME_ENGINE
	ENGINE_PTR(s_ctf_globals, ctf_globals,				0x64BCF0, 0x5BDB20);
	ENGINE_PTR(s_slayer_globals, slayer_globals,		0x64C240, 0x5BE070);
	ENGINE_PTR(s_oddball_globals, oddball_globals,		0x64BFB0, 0x5BDDE0);
	ENGINE_PTR(s_king_globals, king_globals,			0x64BD28, 0x5BDB58);
	ENGINE_PTR(s_race_globals, race_globals,			0x64C0F8, 0x5BDF28);

	ENGINE_PTR(s_game_variant, global_variant,			0x68CB80, 0x5F5400);

	ENGINE_DPTR(game_engine_definition, game_engines,	0x6238C8, 0x599288);
	ENGINE_DPTR(game_engine_definition, game_engine,	0x68CC5C, 0x5F54DC);

	#pragma region New Engines functions
	//////////////////////////////////////////////////////////////////////////
	// FixHudRenderUnitInterface()
	FUNC_PTR(HUD_RENDER_UNIT_INTERFACE_JMP_TABLE,			0x4B4A54, FUNC_PTR_NULL);
	ENGINE_PTR(void**, hud_render_unit_interface_jmp_ptr,	0x4B482C, PTR_NULL);

	//////////////////////////////////////////////////////////////////////////
	// FixFunc4F7440()
	FUNC_PTR(Func4F7440JmpTable,							0x4F761C, 0x4DE67C);
	ENGINE_PTR(void**, Func4F7440JmpPtr,					0x4F7535, 0x4DE595);

	//////////////////////////////////////////////////////////////////////////
	// FixFunc4F7580()
	FUNC_PTR(Func4F7580JmpTable,							0x4F771C, 0x4DE77C);
	ENGINE_PTR(void**, Func4F7580JmpPtr,					0x4F7693, 0x4DE6F3);

	//////////////////////////////////////////////////////////////////////////
	// Initialize()
	ENGINE_PTR(game_engine_definition**, game_engine_init_mod, 0x45CE94, 0x459844);
	#pragma endregion

	FUNC_PTR(GAME_ENGINE_INITIALIZE_FOR_NEW_MAP_HOOK,	0x45CFC8, 0x459978);
	FUNC_PTR(GAME_ENGINE_UPDATE_HOOK,					0x460F51, 0x45BDBB);


//////////////////////////////////////////////////////////////////////////
// GameState.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_GAME_STATE
	static uintptr_t K_GAME_STATE_BEFORE_SAVE_PROCS_CALLS[] = {
		PLATFORM_VALUE(0x4CAD3A, 0x4B4091), PLATFORM_VALUE(0x53B870, 0x4F6E40), 
	};
	static uintptr_t K_GAME_STATE_BEFORE_LOAD_PROCS_CALLS[] = {
		PLATFORM_VALUE(0x53B90D, 0x4F6EDD), PLATFORM_VALUE(0x53BA53, 0x4F7023), PLATFORM_VALUE(0x53BB45, 0x4F7115), 
	};
	static uintptr_t K_GAME_STATE_AFTER_LOAD_PROCS_REFS[] = {
		PLATFORM_VALUE(0x53B622, 0x4F6C02)+1, PLATFORM_VALUE(0x53B918, 0x4F6EE8)+1, PLATFORM_VALUE(0x53BB6E, 0x4F713E)+1,
	};
	static uintptr_t K_GAME_STATE_AFTER_LOAD_PROCS_COUNTS[] = {
		PLATFORM_VALUE(0x53B627, 0x4F6C07)+1, PLATFORM_VALUE(0x53B91D, 0x4F6EED)+1, PLATFORM_VALUE(0x53BB73, 0x4F7143)+1,
	};

	ENGINE_PTR(s_main_globals, main_globals,	0x6B46D0, 0x61CF48);

	ENGINE_PTR(s_physical_memory_map_globals, physical_memory_globals,	0x647390, 0x5B91E0);
	FUNC_PTR(PHYSICAL_MEMORY_INITIALIZE,		0x4453F0, 0x4450B0);
	DATA_PTR(PHYSICAL_MEMORY_ALLOCATION_SIZE,	0x445418, 0x4450D2);

	ENGINE_PTR(s_game_state_globals, game_state_globals,			0x67DCC0, 0x5E660C);
	ENGINE_DPTR(s_game_globals, game_globals,						0x64B9E8, 0x5BD818);
	ENGINE_DPTR(s_game_time_globals, game_time_globals,				0x68CCA8, 0x5F5524);
	ENGINE_DPTR(s_game_allegiance_globals, game_allegiance_globals,	0x64B9EC, 0x5BD81C);

	ENGINE_PTR(TagGroups::s_game_globals*, global_game_globals,			0x6E21A0, 0x6293C0);
	ENGINE_PTR(s_physics_globals, global_physics,						0x637BF4, 0x5AD5CC);
	ENGINE_PTR(s_point_physics_globals, global_point_physics,			0x653BE4, 0x5C5B1C);

	ENGINE_PTR(byte, developer_mode,									0x815CE6, 0x6E1BE6);
	ENGINE_PTR(bool, transport_dumping,									0x621F60, 0x597918);
	ENGINE_PTR(char, init_txt_filename,									0x60AB14, PTR_NULL);
	ENGINE_PTR(bool, devmode_enabled,									0x6BD096, 0x6257C6);

	FUNC_PTR(GAME_INITIALIZE_HOOK,										0x45B83B, 0x4582A0);
	FUNC_PTR(GAME_INITIALIZE_FOR_NEW_MAP_HOOK,							0x45BEDB, 0x4588A5);
	FUNC_PTR(GAME_DISPOSE_FROM_OLD_MAP_HOOK,							0x45C050, 0x458A08);
	static void** K_GAME_SCENARIO_SWITCH_BSP_CALLS[] =
	{
		PLATFORM_PTR(void**, 0x482430, 0x479F70), PLATFORM_PTR(void**, 0x4CD007, 0x4B6517), PLATFORM_PTR(void**, 0x4CD131, 0x4B6641),
		PLATFORM_PTR(void**, 0x541E38, 0x4FCDEC), PLATFORM_PTR(void**, 0x542749, 0x4FD455)
	};
	FUNC_PTR(GAME_DISPOSE_FROM_OLD_BSP_HOOK,							0x442996 - 0x10, 0x443526 - 0x10);

	FUNC_PTR(GAME_SERVER_QR2_STRING_MATCHES_GAMEVER_CALL,				0x4BA79B, FUNC_PTR_NULL);

	FUNC_PTR(GAME_CREATE_NETWORK_SERVER,								0x4E21B0, 0x4C9690);
	FUNC_PTR(GAME_CREATE_NETWORK_CLIENT,								0x4DBF90, 0x4C3520);
	FUNC_PTR(GAME_CREATE_NETWORK_SERVER_CALL,							0x4E1310, FUNC_PTR_NULL);

#if PLATFORM_IS_USER
	static void** K_GAME_CREATE_NETWORK_CLIENT_CALL[] =
	{
		CAST_PTR(void**, 0x498C64), CAST_PTR(void**, 0x4A0313), CAST_PTR(void**, 0x4A03B5), 
		CAST_PTR(void**, 0x4A05FE), CAST_PTR(void**, 0x4DFEC2), CAST_PTR(void**, 0x4E1420), 
		CAST_PTR(void**, 0x4E7B0E)
	};
#endif


//////////////////////////////////////////////////////////////////////////
// Lights.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_LIGHTS
	ENGINE_DPTR(lights_data_t, lights,									0x7FBA34, 0x6C70B0);
	ENGINE_DPTR(s_lights_globals_data, light_game_globals,				0x6B8188, 0x6209A8);
	ENGINE_DPTR(s_light_cluster_data, light_cluster,					0x7FBA40, 0x6C70C0);
	ENGINE_DPTR(cluster_light_reference_data_t, cluster_light_reference,0x7FBA44, 0x6C70C4);
	ENGINE_DPTR(light_cluster_reference_data_t, light_cluster_reference,0x7FBA48, 0x6C70C8);


//////////////////////////////////////////////////////////////////////////
// Players.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_PLAYERS
	ENGINE_DPTR(players_data_t, players,								0x8154E0, 0x6E13E0);
	ENGINE_DPTR(teams_data_t, teams,									0x8154DC, 0x6E13DC);
	ENGINE_DPTR(s_players_globals_data, players_globals,				0x8154D8, 0x6E13D8);
	ENGINE_DPTR(s_player_control_globals_data, player_control_globals,	0x64C2C4, 0x5BE0F4);
	ENGINE_DPTR(s_player_effects_data, player_effects,					0x68C77C, 0x5F4FFC);
	ENGINE_PTR(datum_index, machine_to_player_table,					0x64C2C8, 0x5BE0F8);

	FUNC_PTR(PLAYER_SPAWN__CREATE_UNIT_MP_HOOK,			0x47AD11, 0x472791);
	FUNC_PTR(PLAYER_SPAWN__CREATE_UNIT_MP_HOOK_RETN,	0x47AE30, 0x4728B0);

	FUNC_PTR(PLAYERS_UPDATE_BEFORE_GAME_SERVER_HOOK,	0x4773A7, 0x46EE27);
	FUNC_PTR(PLAYERS_UPDATE_BEFORE_GAME_CLIENT_HOOK,	0x4777DB, 0x46F25B);
	FUNC_PTR(PLAYERS_UPDATE_AFTER_GAME_HOOK,			0x477B6B, 0x46F5EB);

	//////////////////////////////////////////////////////////////////////////
	// Players.NearbyObjects
	FUNC_PTR(PLAYER_EXAMINE_NEARBY_OBJECTS_SERVER_JMP_TABLE_RETN,			0x47B258, 0x472CD8);
	ENGINE_PTR(void**, player_examine_nearby_objects_server_jmp_ptr,		0x47B238, 0x472CB8);
	ENGINE_PTR(byte, PLAYER_EXAMINE_NEARBY_OBJECTS_SERVER_LAST_ENTRY_TYPE,	0x47B232, 0x472CB2);
	ENGINE_PTR(void**, PLAYER_EXAMINE_NEARBY_OBJECTS_SERVER_JMP_TABLE,		0x47B264, 0x472CE4);

	FUNC_PTR(PLAYER_EXAMINE_NEARBY_OBJECTS_CLIENT_JMP_TABLE_RETN,			0x47B358, 0x472DD8);
	ENGINE_PTR(void**, player_examine_nearby_objects_client_jmp_ptr,		0x47B338, 0x472DB8);
	ENGINE_PTR(byte, PLAYER_EXAMINE_NEARBY_OBJECTS_CLIENT_LAST_ENTRY_TYPE,0x47B332, 0x472DB2);
	ENGINE_PTR(void**, PLAYER_EXAMINE_NEARBY_OBJECTS_CLIENT_JMP_TABLE,		0x47B364, 0x472DE4);

	//////////////////////////////////////////////////////////////////////////
	// Players.ActionResults
	FUNC_PTR(PLAYER_HANDLE_ACTION_JMP_TABLE_ADJUST_SIZE,	0x47C173, 0x473BF3);
	FUNC_PTR(PLAYER_HANDLE_ACTION_JMP_TABLE_RETN,			0x47C46B, 0x473EEB);
	ENGINE_PTR(byte, PLAYER_HANDLE_ACTION_LAST_ENTRY_TYPE,	0x47C178, 0x473BF8);
	ENGINE_PTR(void**, player_handle_action_jmp_ptr,		0x47C189, 0x473C09);
	ENGINE_PTR(void**, PLAYER_HANDLE_ACTION_JMP_TABLE,		0x47C478, 0x473EF8);

	FUNC_PTR(FOLLOWING_CAMERA_GET_UNIT_CAMERA_INFO_CALL,	0x447EA2, FUNC_PTR_NULL);


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