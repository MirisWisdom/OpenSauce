/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

#if !PLATFORM_IS_DEDI
#include "Memory/1.10/Pointers/HaloCE_110_Runtime.Game.inl"
#include "Memory/1.10/Pointers/HaloCE_110_Runtime_Manual.Game.inl"
#else
#include "Memory/1.10/Pointers/HaloCE_110_Dedi.Game.inl"
#include "Memory/1.10/Pointers/HaloCE_110_Dedi_Manual.Game.inl"
#endif

//////////////////////////////////////////////////////////////////////////
// AI.cpp
#if __EL_INCLUDE_FILE_ID == __EL_GAME_AI
	ENGINE_DPTR(s_ai_globals_data, ai_globals,								DUO_PTR(K_AI_GLOBALS));
	ENGINE_DPTR(actor_data_t, actors,										DUO_PTR(K_ACTORS));
	ENGINE_DPTR(swarm_data_t, swarms,										DUO_PTR(K_SWARMS));
	ENGINE_DPTR(swarm_component_data_t, swarm_components,					DUO_PTR(K_SWARM_COMPONENTS));
	ENGINE_DPTR(prop_data_t, props,											DUO_PTR(K_PROPS));
	ENGINE_DPTR(encounter_data_t, encounters,								DUO_PTR(K_ENCOUNTERS));
	ENGINE_DPTR(squads_data_t, squads,										DUO_PTR(K_SQUADS));
	ENGINE_DPTR(platoons_data_t, platoons,									DUO_PTR(K_PLATOONS));
	ENGINE_DPTR(ai_pursuit_data_t, ai_pursuits,								DUO_PTR(K_AI_PURSUITS));
	//ai_communication_dialogue_events_t
	ENGINE_DPTR(ai_communication_reply_events_t, ai_communication_replies,	DUO_PTR(K_AI_COMMUNICATION_REPLIES));
	ENGINE_DPTR(ai_conversation_data_t, ai_conversations,					DUO_PTR(K_AI_CONVERSATIONS));

	FUNC_PTR(AI_UPDATE_HOOK,												DUO_PTR(K_AI_UPDATE_HOOK));
	FUNC_PTR(ACTOR_ACTION_HANDLE_VEHICLE_EXIT_HOOK,							DUO_PTR(K_ACTOR_ACTION_HANDLE_VEHICLE_EXIT_HOOK));
	FUNC_PTR(ACTOR_ACTION_HANDLE_VEHICLE_EXIT_RETN,							DUO_PTR(K_ACTOR_ACTION_HANDLE_VEHICLE_EXIT_RETN));
	FUNC_PTR(PROP_STATUS_REFRESH_HOOK,										DUO_PTR(K_PROP_STATUS_REFRESH_HOOK));
	FUNC_PTR(PROP_STATUS_REFRESH_RETN,										DUO_PTR(K_PROP_STATUS_REFRESH_RETN));
	FUNC_PTR(ACTOR_INPUT_UPDATE_HOOK,										DUO_PTR(K_ACTOR_INPUT_UPDATE_HOOK));
	FUNC_PTR(ACTOR_INPUT_UPDATE_RETN,										DUO_PTR(K_ACTOR_INPUT_UPDATE_RETN));


//////////////////////////////////////////////////////////////////////////
// Camera.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_CAMERA
namespace Camera
{
	ENGINE_PTR(s_observer, observers,								K_OBSERVERS, PTR_NULL);
	ENGINE_PTR(s_camera_script_globals_data, camera_script_globals,	K_CAMERA_SCRIPT_GLOBALS, PTR_NULL);
	ENGINE_DPTR(s_director_scripting_data, director_scripting,		K_DIRECTOR_SCRIPTING, PTR_NULL);
	ENGINE_DPTR(s_cinematic_globals_data, cinematic_globals,		K_CINEMATIC_GLOBALS, PTR_NULL);
	ENGINE_PTR(s_director_data, global_director,					K_GLOBAL_DIRECTOR, PTR_NULL);

	FUNC_PTR(DIRECTOR_CHOOSE_GAME_PERSPECTIVE_HOOK,					K_DIRECTOR_CHOOSE_GAME_PERSPECTIVE_HOOK, PTR_NULL);

	#pragma region old third person shit
	#if 0 // enable for old 3rd person camera hack
		FUNC_PTR(OBSERVER_UPDATE_COMMAND_HOOK,						K_OBSERVER_UPDATE_COMMAND_HOOK, FUNC_PTR_NULL);

		FUNC_PTR(DIRECTOR_GET_PERSPECTIVE_DEFAULT_SET,				K_DIRECTOR_GET_PERSPECTIVE_DEFAULT_SET, FUNC_PTR_NULL);
		FUNC_PTR(HUD_INITIALIZE_FOR_NEW_MAP_DEFAULT_SET_SHOW_HUD,	K_HUD_INITIALIZE_FOR_NEW_MAP_DEFAULT_SET_SHOW_HUD, FUNC_PTR_NULL);
		FUNC_PTR(HUD_ADD_ITEM_MESSAGE_DEFAULT_SET_SHOW_MSG,			K_HUD_ADD_ITEM_MESSAGE_DEFAULT_SET_SHOW_MSG, FUNC_PTR_NULL);
	#endif
	#pragma endregion
};

namespace Fov
{
	FUNC_PTR(OBSERVER_UPDATE_COMMAND_HOOK,							K_OBSERVER_UPDATE_COMMAND_HOOK, FUNC_PTR_NULL);

	#pragma region OBSERVER_UPDATE_POSITIONS
	FUNC_PTR(OBSERVER_UPDATE_POSITIONS,								K_OBSERVER_UPDATE_POSITIONS, FUNC_PTR_NULL);
	ENGINE_PTR(bool, OBSERVER_UPDATE_POSITIONS_no_scope_blur,		K_OBSERVER_UPDATE_POSITIONS_NO_SCOPE_BLUR, PTR_NULL);
	// Address of the call opcodes to [OBSERVER_UPDATE_POSITIONS] inside the function
	// known as [OBSERVER_TICK]
	FUNC_PTR(OBSERVER_TICK_CALL_HOOK_OBSERVER_UPDATE_POSITIONS,		K_OBSERVER_TICK_CALL_HOOK_OBSERVER_UPDATE_POSITIONS, FUNC_PTR_NULL);
	#pragma endregion

	ENGINE_PTR(byte, MAX_FOV_CHECK_JMP,								K_MAX_FOV_CHECK_JMP, PTR_NULL);
};


//////////////////////////////////////////////////////////////////////////
// Campaign.cpp. CLIENT BUILDS ONLY
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_CAMPAIGN
#if YELO_CAMPAIGN_UPGRADES_ENABLED
	#pragma region Scenario Paths
	static cstring** ScenarioPathsReferences[] = {
		CAST_PTR(cstring**, K_SCENARIO_PATHS_REFERENCE_0),
		CAST_PTR(cstring**, K_SCENARIO_PATHS_REFERENCE_1),
		CAST_PTR(cstring**, K_SCENARIO_PATHS_REFERENCE_2),
		CAST_PTR(cstring**, K_SCENARIO_PATHS_REFERENCE_3),
		CAST_PTR(cstring**, K_SCENARIO_PATHS_REFERENCE_4),
		//CAST_PTR(cstring**, K_SCENARIO_PATHS_REFERENCE_5),
	};

	static cstring** ScenarioPathsReferences2[] = {
		CAST_PTR(cstring**, K_SCENARIO_PATHS_REFERENCE2_0),
		CAST_PTR(cstring**, K_SCENARIO_PATHS_REFERENCE2_1),
		CAST_PTR(cstring**, K_SCENARIO_PATHS_REFERENCE2_2),
		CAST_PTR(cstring**, K_SCENARIO_PATHS_REFERENCE2_3),
	};
	#pragma endregion

	#pragma region Scenario Help
	FUNC_PTR(DISPLAY_SCENARIO_HELP_HOOK,		K_DISPLAY_SCENARIO_HELP_HOOK, FUNC_PTR_NULL);
		FUNC_PTR(DISPLAY_SCENARIO_HELP_SUCCESS,	K_DISPLAY_SCENARIO_HELP_SUCCESS, FUNC_PTR_NULL);
		FUNC_PTR(DISPLAY_SCENARIO_HELP_FAIL,	K_DISPLAY_SCENARIO_HELP_FAIL, FUNC_PTR_NULL);
	#pragma endregion

	#pragma region Campaign Level Data
	static void** CampaignLevelDataReferences[] = {
		CAST_PTR(void**, K_CAMPAIGN_LEVEL_DATA_REFERENCE_0),
		CAST_PTR(void**, K_CAMPAIGN_LEVEL_DATA_REFERENCE_1),
		CAST_PTR(void**, K_CAMPAIGN_LEVEL_DATA_REFERENCE_2),
		CAST_PTR(void**, K_CAMPAIGN_LEVEL_DATA_REFERENCE_3),
		//CAST_PTR(void**, K_CAMPAIGN_LEVEL_DATA_REFERENCE_4),
		//CAST_PTR(void**, K_CAMPAIGN_LEVEL_DATA_REFERENCE_5),
		//CAST_PTR(void**, K_CAMPAIGN_LEVEL_DATA_REFERENCE_6),
	};

	static size_t* CampaignLevelDataSizeReferences[] = {
		CAST_PTR(size_t*, K_CAMPAIGN_LEVEL_DATA_SIZE_REFERENCE_0),
		CAST_PTR(size_t*, K_CAMPAIGN_LEVEL_DATA_SIZE_REFERENCE_1),
		CAST_PTR(size_t*, K_CAMPAIGN_LEVEL_DATA_SIZE_REFERENCE_2),
	};
	#pragma endregion

	#pragma region Campaign Level Count
	static byte* CampaignLevelCountReferences_8bit[] = {
		CAST_PTR(byte*, K_CAMPAIGN_LEVEL_COUNT_REFERENCE_8BIT_0),
		//CAST_PTR(byte*, K_CAMPAIGN_LEVEL_COUNT_REFERENCE_8BIT_1),
	};

	static int16* CampaignLevelCountReferences_16bit[] = {
		CAST_PTR(int16*, K_CAMPAIGN_LEVEL_COUNT_REFERENCE_16BIT_0),
	};

	static int32* CampaignLevelCountReferences_32bit[] = {
		CAST_PTR(int32*, K_CAMPAIGN_LEVEL_COUNT_REFERENCE_32BIT_0),
		//CAST_PTR(int32*, K_CAMPAIGN_LEVEL_COUNT_REFERENCE_32BIT_1),
	};

	//////////////////////////////////////////////////////////////////////////

	static byte* CampaignLevelCountReferences2_8bit[] = {
		CAST_PTR(byte*, K_CAMPAIGN_LEVEL_COUNT_REFERENCE2_8BIT_0),
		CAST_PTR(byte*, K_CAMPAIGN_LEVEL_COUNT_REFERENCE2_8BIT_1),
		CAST_PTR(byte*, K_CAMPAIGN_LEVEL_COUNT_REFERENCE2_8BIT_2),
		CAST_PTR(byte*, K_CAMPAIGN_LEVEL_COUNT_REFERENCE2_8BIT_3),
		CAST_PTR(byte*, K_CAMPAIGN_LEVEL_COUNT_REFERENCE2_8BIT_4),
		CAST_PTR(byte*, K_CAMPAIGN_LEVEL_COUNT_REFERENCE2_8BIT_5),
		//CAST_PTR(byte*, K_CAMPAIGN_LEVEL_COUNT_REFERENCE2_8BIT_6),
	};

	static int16* CampaignLevelCountReferences2_16bit[] = {
		CAST_PTR(int16*, K_CAMPAIGN_LEVEL_COUNT_REFERENCE2_16BIT_0),
		CAST_PTR(int16*, K_CAMPAIGN_LEVEL_COUNT_REFERENCE2_16BIT_1),
		//CAST_PTR(int16*, K_CAMPAIGN_LEVEL_COUNT_REFERENCE2_16BIT_2),
	};
	#pragma endregion
#endif


//////////////////////////////////////////////////////////////////////////
// Console.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_CONSOLE
	ENGINE_DPTR(s_terminal_globals, terminal_globals,		DUO_PTR(K_TERMINAL_GLOBALS));
	ENGINE_PTR(s_console_globals, console_globals,			DUO_PTR(K_CONSOLE_GLOBALS));

	FUNC_PTR(CONSOLE_UPDATE_HOOK,							DUO_PTR(K_CONSOLE_UPDATE_HOOK));


//////////////////////////////////////////////////////////////////////////
// Effects.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_EFFECTS
	ENGINE_DPTR(contrail_data_t, contrails,									DUO_PTR(K_CONTRAILS));
	ENGINE_DPTR(contrail_point_data_t, contrail_points,						DUO_PTR(K_CONTRAIL_POINTS));
	ENGINE_DPTR(particle_data_t, particles,									DUO_PTR(K_PARTICLES));
	ENGINE_DPTR(effect_data_t, effects,										DUO_PTR(K_EFFECTS));
	ENGINE_DPTR(effect_location_data_t, effect_locations,					DUO_PTR(K_EFFECT_LOCATIONS));
	ENGINE_DPTR(particle_systems_data_t, particle_systems,					DUO_PTR(K_PARTICLE_SYSTEMS));
	ENGINE_DPTR(particle_system_particles_data_t, particle_system_particles,DUO_PTR(K_PARTICLE_SYSTEM_PARTICLES));
	ENGINE_DPTR(s_structure_detail_objects_data, structure_detail_objects,	DUO_PTR(K_STRUCTURE_DETAIL_OBJECTS));
	ENGINE_DPTR(s_structure_decals_data, structure_decals,					DUO_PTR(K_STRUCTURE_DECALS));
	ENGINE_DPTR(s_breakable_surface_globals_data, breakable_surface_globals,DUO_PTR(K_BREAKABLE_SURFACE_GLOBALS));
	ENGINE_DPTR(decals_data_t, decals,										DUO_PTR(K_DECALS));
	ENGINE_DPTR(s_decal_globals_data, decal_globals,						DUO_PTR(K_DECAL_GLOBALS));
	ENGINE_DPTR(s_decal_vertex_cache_data, decal_vertex_cache,				K_DECAL_VERTEX_CACHE, PTR_NULL);

	FUNC_PTR(EFFECTS_UPDATE_HOOK,											DUO_PTR(K_EFFECTS_UPDATE_HOOK));

	#if PLATFORM_IS_USER
		static void* K_RENDER_PARTICLES_RENDERED_PARTICLES_ARRAY_ADDRESS_LIST[] = {
			CAST_PTR(void*, K_RENDER_PARTICLES_RENDERED_PARTICLES_ARRAY_ADDRESS_0),
			CAST_PTR(void*, K_RENDER_PARTICLES_RENDERED_PARTICLES_ARRAY_ADDRESS_1),
			CAST_PTR(void*, K_RENDER_PARTICLES_RENDERED_PARTICLES_ARRAY_ADDRESS_2),
			CAST_PTR(void*, K_RENDER_PARTICLES_RENDERED_PARTICLES_ARRAY_ADDRESS_3),
			CAST_PTR(void*, K_RENDER_PARTICLES_RENDERED_PARTICLES_ARRAY_ADDRESS_4),
		};

		FUNC_PTR(GAME_INITIALIZE_MOD_PER_MAP_UPGRADE_PARTICLES,					K_GAME_INITIALIZE_MOD_PER_MAP_UPGRADE_PARTICLES, FUNC_PTR_NULL);
		FUNC_PTR(GAME_INITIALIZE_MOD_PER_MAP_UPGRADE_EFFECTS,					K_GAME_INITIALIZE_MOD_PER_MAP_UPGRADE_EFFECTS, FUNC_PTR_NULL);
		FUNC_PTR(GAME_INITIALIZE_MOD_PER_MAP_UPGRADE_EFFECT_LOCATIONS,			K_GAME_INITIALIZE_MOD_PER_MAP_UPGRADE_EFFECT_LOCATIONS, FUNC_PTR_NULL);
		FUNC_PTR(GAME_INITIALIZE_MOD_PER_MAP_UPGRADE_PARTICLE_SYSTEM_PARTICLES,	K_GAME_INITIALIZE_MOD_PER_MAP_UPGRADE_PARTICLE_SYSTEM_PARTICLES, FUNC_PTR_NULL);
	#endif


//////////////////////////////////////////////////////////////////////////
// EngineFunctions.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_ENGINE_FUNCTIONS
	#include __EL_INCLUDE_FILE_MACRO(_EngineLayout.Game.EngineFunctions.inl)


//////////////////////////////////////////////////////////////////////////
// GameBuildNumber.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_BUILD_NUMBER
	ENGINE_PTR(char, game_build_version,								DUO_PTR(K_GAME_BUILD_VERSION));
	ENGINE_PTR(char, game_build_version_gamespy,						DUO_PTR(K_GAME_BUILD_VERSION_GAMESPY));

	ENGINE_PTR(Enums::network_game_protocol_id, network_version_id1,	DUO_PTR(K_NETWORK_VERSION_ID1));
	ENGINE_PTR(Enums::network_game_protocol_id, network_version_id2,	DUO_PTR(K_NETWORK_VERSION_ID2));
	ENGINE_PTR(Enums::network_game_protocol_id, network_version_id3,	DUO_PTR(K_NETWORK_VERSION_ID3));

	FUNC_PTR(GAME_STATE_HEADER_IS_VALID_HOOK,							DUO_PTR(K_GAME_STATE_HEADER_IS_VALID_HOOK));
	FUNC_PTR(GAME_STATE_HEADER_IS_VALID_HOOK_RET,						DUO_PTR(K_GAME_STATE_HEADER_IS_VALID_HOOK_RET));
	FUNC_PTR(GAME_STATE_HEADER_TRY_AND_LOAD_HOOK,						DUO_PTR(K_GAME_STATE_HEADER_TRY_AND_LOAD_HOOK));
	FUNC_PTR(GAME_STATE_HEADER_TRY_AND_LOAD_HOOK_RET_TRUE,				DUO_PTR(K_GAME_STATE_HEADER_TRY_AND_LOAD_HOOK_RET_TRUE));
	FUNC_PTR(GAME_STATE_HEADER_TRY_AND_LOAD_HOOK_RET_FALSE,				DUO_PTR(K_GAME_STATE_HEADER_TRY_AND_LOAD_HOOK_RET_FALSE));


//////////////////////////////////////////////////////////////////////////
// GameEngine.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_GAME_ENGINE
	ENGINE_PTR(s_ctf_globals, ctf_globals,				DUO_PTR(K_CTF_GLOBALS));
	ENGINE_PTR(s_slayer_globals, slayer_globals,		DUO_PTR(K_SLAYER_GLOBALS));
	ENGINE_PTR(s_oddball_globals, oddball_globals,		DUO_PTR(K_ODDBALL_GLOBALS));
	ENGINE_PTR(s_king_globals, king_globals,			DUO_PTR(K_KING_GLOBALS));
	ENGINE_PTR(s_race_globals, race_globals,			DUO_PTR(K_RACE_GLOBALS));

	ENGINE_PTR(s_game_variant, global_variant,			DUO_PTR(K_GLOBAL_VARIANT));

	ENGINE_DPTR(game_engine_definition, game_engines,	DUO_PTR(K_GAME_ENGINES));
	ENGINE_DPTR(game_engine_definition, game_engine,	DUO_PTR(K_GAME_ENGINE));

	#pragma region New Engines functions
	//////////////////////////////////////////////////////////////////////////
	// FixHudRenderUnitInterface()
	FUNC_PTR(HUD_RENDER_UNIT_INTERFACE_JMP_TABLE,			K_HUD_RENDER_UNIT_INTERFACE_JMP_TABLE, FUNC_PTR_NULL);
	ENGINE_PTR(void**, hud_render_unit_interface_jmp_ptr,	K_HUD_RENDER_UNIT_INTERFACE_JMP_PTR, PTR_NULL);

	//////////////////////////////////////////////////////////////////////////
	// FixFunc4F7440()
	FUNC_PTR(Func4F7440JmpTable,							DUO_PTR(K_FUNC4F7440JMPTABLE));
	ENGINE_PTR(void**, Func4F7440JmpPtr,					DUO_PTR(K_FUNC4F7440JMPPTR));

	//////////////////////////////////////////////////////////////////////////
	// FixFunc4F7580()
	FUNC_PTR(Func4F7580JmpTable,							DUO_PTR(K_FUNC4F7580JMPTABLE));
	ENGINE_PTR(void**, Func4F7580JmpPtr,					DUO_PTR(K_FUNC4F7580JMPPTR));

	//////////////////////////////////////////////////////////////////////////
	// Initialize()
	ENGINE_PTR(game_engine_definition**, game_engine_init_mod, DUO_PTR(K_GAME_ENGINE_INIT_MOD));
	#pragma endregion

	FUNC_PTR(GAME_ENGINE_INITIALIZE_FOR_NEW_MAP_HOOK,	DUO_PTR(K_GAME_ENGINE_INITIALIZE_FOR_NEW_MAP_HOOK));
	FUNC_PTR(GAME_ENGINE_UPDATE_HOOK,					DUO_PTR(K_GAME_ENGINE_UPDATE_HOOK));


//////////////////////////////////////////////////////////////////////////
// GameState.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_GAME_STATE
	static uintptr_t K_GAME_STATE_BEFORE_SAVE_PROCS_CALLS[] = {
		K_GAME_STATE_BEFORE_SAVE_PROCS_CALL_0,
		K_GAME_STATE_BEFORE_SAVE_PROCS_CALL_1,
	};
	static uintptr_t K_GAME_STATE_BEFORE_LOAD_PROCS_CALLS[] = {	
		K_GAME_STATE_BEFORE_LOAD_PROCS_CALL_0,
		K_GAME_STATE_BEFORE_LOAD_PROCS_CALL_1,
		K_GAME_STATE_BEFORE_LOAD_PROCS_CALL_2,
	};
	static uintptr_t K_GAME_STATE_AFTER_LOAD_PROCS_REFS[] = {
		K_GAME_STATE_AFTER_LOAD_PROCS_REF_0,
		K_GAME_STATE_AFTER_LOAD_PROCS_REF_1,
		K_GAME_STATE_AFTER_LOAD_PROCS_REF_2,
	};
	static uintptr_t K_GAME_STATE_AFTER_LOAD_PROCS_COUNTS[] = {
		K_GAME_STATE_AFTER_LOAD_PROCS_COUNT_0,
		K_GAME_STATE_AFTER_LOAD_PROCS_COUNT_1,
		K_GAME_STATE_AFTER_LOAD_PROCS_COUNT_2,
	};

	ENGINE_PTR(s_main_globals, main_globals,							DUO_PTR(K_MAIN_GLOBALS));

	ENGINE_PTR(s_physical_memory_map_globals, physical_memory_globals,	DUO_PTR(K_PHYSICAL_MEMORY_GLOBALS));
	FUNC_PTR(PHYSICAL_MEMORY_INITIALIZE,								DUO_PTR(K_PHYSICAL_MEMORY_INITIALIZE));
	DATA_PTR(PHYSICAL_MEMORY_ALLOCATION_SIZE,							DUO_PTR(K_PHYSICAL_MEMORY_ALLOCATION_SIZE));

	ENGINE_PTR(s_game_state_globals, game_state_globals,				DUO_PTR(K_GAME_STATE_GLOBALS));
	ENGINE_DPTR(s_game_globals, game_globals,							DUO_PTR(K_GAME_GLOBALS));
	ENGINE_DPTR(s_game_time_globals, game_time_globals,					DUO_PTR(K_GAME_TIME_GLOBALS));
	ENGINE_DPTR(s_game_allegiance_globals, game_allegiance_globals,		DUO_PTR(K_GAME_ALLEGIANCE_GLOBALS));

	ENGINE_PTR(TagGroups::s_game_globals*, global_game_globals,			DUO_PTR(K_GLOBAL_GAME_GLOBALS));
	ENGINE_PTR(s_physics_globals, global_physics,						DUO_PTR(K_GLOBAL_PHYSICS));
	ENGINE_PTR(s_point_physics_globals, global_point_physics,			DUO_PTR(K_GLOBAL_POINT_PHYSICS));

	ENGINE_PTR(byte, developer_mode,									DUO_PTR(K_DEVELOPER_MODE));
	ENGINE_PTR(bool, transport_dumping,									DUO_PTR(K_TRANSPORT_DUMPING));
	ENGINE_PTR(char, init_txt_filename,									K_INIT_TXT_FILENAME, PTR_NULL);
	ENGINE_PTR(bool, devmode_enabled,									DUO_PTR(K_DEVMODE_ENABLED));

	FUNC_PTR(GAME_INITIALIZE_HOOK,										DUO_PTR(K_GAME_INITIALIZE_HOOK));
	FUNC_PTR(GAME_INITIALIZE_FOR_NEW_MAP_HOOK,							DUO_PTR(K_GAME_INITIALIZE_FOR_NEW_MAP_HOOK));
	FUNC_PTR(GAME_DISPOSE_FROM_OLD_MAP_HOOK,							DUO_PTR(K_GAME_DISPOSE_FROM_OLD_MAP_HOOK));
	static void** K_GAME_SCENARIO_SWITCH_BSP_CALLS[] =
	{
		CAST_PTR(void**, K_GAME_SCENARIO_SWITCH_BSP_CALL_0),
		CAST_PTR(void**, K_GAME_SCENARIO_SWITCH_BSP_CALL_1),
		CAST_PTR(void**, K_GAME_SCENARIO_SWITCH_BSP_CALL_2),
		CAST_PTR(void**, K_GAME_SCENARIO_SWITCH_BSP_CALL_3),
		CAST_PTR(void**, K_GAME_SCENARIO_SWITCH_BSP_CALL_4),
	};
	FUNC_PTR(GAME_DISPOSE_FROM_OLD_BSP_HOOK,							DUO_PTR(K_GAME_DISPOSE_FROM_OLD_BSP_HOOK));

	FUNC_PTR(GAME_SERVER_QR2_STRING_MATCHES_GAMEVER_CALL,				K_GAME_SERVER_QR2_STRING_MATCHES_GAMEVER_CALL, FUNC_PTR_NULL);

	FUNC_PTR(GAME_CREATE_NETWORK_SERVER,								DUO_PTR(K_GAME_CREATE_NETWORK_SERVER));
	FUNC_PTR(GAME_CREATE_NETWORK_CLIENT,								DUO_PTR(K_GAME_CREATE_NETWORK_CLIENT));
	FUNC_PTR(GAME_CREATE_NETWORK_SERVER_CALL,							K_GAME_CREATE_NETWORK_SERVER_CALL, FUNC_PTR_NULL);

#if PLATFORM_IS_USER
	static void** K_GAME_CREATE_NETWORK_CLIENT_CALL[] =
	{
		CAST_PTR(void**, K_GAME_CREATE_NETWORK_CLIENT_CALL_0),
		CAST_PTR(void**, K_GAME_CREATE_NETWORK_CLIENT_CALL_1),
		CAST_PTR(void**, K_GAME_CREATE_NETWORK_CLIENT_CALL_2),
		CAST_PTR(void**, K_GAME_CREATE_NETWORK_CLIENT_CALL_3),
		CAST_PTR(void**, K_GAME_CREATE_NETWORK_CLIENT_CALL_4),
		CAST_PTR(void**, K_GAME_CREATE_NETWORK_CLIENT_CALL_5),
		CAST_PTR(void**, K_GAME_CREATE_NETWORK_CLIENT_CALL_6),
	};
#endif

//////////////////////////////////////////////////////////////////////////
// Lights.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_LIGHTS
	ENGINE_DPTR(lights_data_t, lights,										DUO_PTR(K_LIGHTS));
	ENGINE_DPTR(s_lights_globals_data, light_game_globals,					DUO_PTR(K_LIGHT_GAME_GLOBALS));
	ENGINE_DPTR(s_light_cluster_data, light_cluster,						DUO_PTR(K_LIGHT_CLUSTER));
	ENGINE_DPTR(cluster_light_reference_data_t, cluster_light_reference,	DUO_PTR(K_CLUSTER_LIGHT_REFERENCE));
	ENGINE_DPTR(light_cluster_reference_data_t, light_cluster_reference,	DUO_PTR(K_LIGHT_CLUSTER_REFERENCE));


//////////////////////////////////////////////////////////////////////////
// Players.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_GAME_PLAYERS
	ENGINE_DPTR(players_data_t, players,								DUO_PTR(K_PLAYERS));
	ENGINE_DPTR(teams_data_t, teams,									DUO_PTR(K_TEAMS));
	ENGINE_DPTR(s_players_globals_data, players_globals,				DUO_PTR(K_PLAYERS_GLOBALS));
	ENGINE_DPTR(s_player_control_globals_data, player_control_globals,	DUO_PTR(K_PLAYER_CONTROL_GLOBALS));
	ENGINE_DPTR(s_player_effects_data, player_effects,					DUO_PTR(K_PLAYER_EFFECTS));
	ENGINE_PTR(datum_index, machine_to_player_table,					DUO_PTR(K_MACHINE_TO_PLAYER_TABLE));

	FUNC_PTR(PLAYER_SPAWN__CREATE_UNIT_MP_HOOK,			DUO_PTR(K_PLAYER_SPAWN__CREATE_UNIT_MP_HOOK));
	FUNC_PTR(PLAYER_SPAWN__CREATE_UNIT_MP_HOOK_RETN,	DUO_PTR(K_PLAYER_SPAWN__CREATE_UNIT_MP_HOOK_RETN));

	FUNC_PTR(PLAYERS_UPDATE_BEFORE_GAME_SERVER_HOOK,	DUO_PTR(K_PLAYERS_UPDATE_BEFORE_GAME_SERVER_HOOK));
	FUNC_PTR(PLAYERS_UPDATE_BEFORE_GAME_CLIENT_HOOK,	DUO_PTR(K_PLAYERS_UPDATE_BEFORE_GAME_CLIENT_HOOK));
	FUNC_PTR(PLAYERS_UPDATE_AFTER_GAME_HOOK,			DUO_PTR(K_PLAYERS_UPDATE_AFTER_GAME_HOOK));
	
	//////////////////////////////////////////////////////////////////////////
	// Players.NearbyObjects
	FUNC_PTR(PLAYER_EXAMINE_NEARBY_OBJECTS_SERVER_JMP_TABLE_RETN,				DUO_PTR(K_PLAYER_EXAMINE_NEARBY_OBJECTS_SERVER_JMP_TABLE_RETN));
	ENGINE_PTR(void**, player_examine_nearby_objects_server_jmp_ptr,			DUO_PTR(K_PLAYER_EXAMINE_NEARBY_OBJECTS_SERVER_JMP_PTR));
	ENGINE_PTR(byte, PLAYER_EXAMINE_NEARBY_OBJECTS_SERVER_LAST_ENTRY_TYPE,		DUO_PTR(K_PLAYER_EXAMINE_NEARBY_OBJECTS_SERVER_LAST_ENTRY_TYPE));
	ENGINE_PTR(void**, PLAYER_EXAMINE_NEARBY_OBJECTS_SERVER_JMP_TABLE,			DUO_PTR(K_PLAYER_EXAMINE_NEARBY_OBJECTS_SERVER_JMP_TABLE));

	FUNC_PTR(PLAYER_EXAMINE_NEARBY_OBJECTS_CLIENT_JMP_TABLE_RETN,				DUO_PTR(K_PLAYER_EXAMINE_NEARBY_OBJECTS_CLIENT_JMP_TABLE_RETN));
	ENGINE_PTR(void**, player_examine_nearby_objects_client_jmp_ptr,			DUO_PTR(K_PLAYER_EXAMINE_NEARBY_OBJECTS_CLIENT_JMP_PTR));
	ENGINE_PTR(byte, PLAYER_EXAMINE_NEARBY_OBJECTS_CLIENT_LAST_ENTRY_TYPE,		DUO_PTR(K_PLAYER_EXAMINE_NEARBY_OBJECTS_CLIENT_LAST_ENTRY_TYPE));
	ENGINE_PTR(void**, PLAYER_EXAMINE_NEARBY_OBJECTS_CLIENT_JMP_TABLE,			DUO_PTR(K_PLAYER_EXAMINE_NEARBY_OBJECTS_CLIENT_JMP_TABLE));
	
	//////////////////////////////////////////////////////////////////////////
	// Players.ActionResults
	FUNC_PTR(PLAYER_HANDLE_ACTION_JMP_TABLE_ADJUST_SIZE,		DUO_PTR(K_PLAYER_HANDLE_ACTION_JMP_TABLE_ADJUST_SIZE));
	FUNC_PTR(PLAYER_HANDLE_ACTION_JMP_TABLE_RETN,				DUO_PTR(K_PLAYER_HANDLE_ACTION_JMP_TABLE_RETN));
	ENGINE_PTR(byte, PLAYER_HANDLE_ACTION_LAST_ENTRY_TYPE,		DUO_PTR(K_PLAYER_HANDLE_ACTION_LAST_ENTRY_TYPE));
	ENGINE_PTR(void**, player_handle_action_jmp_ptr,			DUO_PTR(K_PLAYER_HANDLE_ACTION_JMP_PTR));
	ENGINE_PTR(void**, PLAYER_HANDLE_ACTION_JMP_TABLE,			DUO_PTR(K_PLAYER_HANDLE_ACTION_JMP_TABLE));

	FUNC_PTR(FOLLOWING_CAMERA_GET_UNIT_CAMERA_INFO_CALL,		K_FOLLOWING_CAMERA_GET_UNIT_CAMERA_INFO_CALL, FUNC_PTR_NULL);

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