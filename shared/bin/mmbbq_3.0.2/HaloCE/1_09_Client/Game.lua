--	Yelo: Open Sauce SDK
--		Halo 1 (CE) Edition

--	See license\OpenSauce\Halo1_CE for specific license information

HaloCEClient_Game =
{
--////////////////////////////////////////////////////////////////////////
-- AI.cpp
	["K_AI_GLOBALS"] =                                                    0x81B534,
	["K_ACTORS"] =                                                        0x81B540,
	["K_SWARMS"] =                                                        0x81B53C,
	["K_SWARM_COMPONENTS"] =                                              0x81B538,
	["K_PROPS"] =                                                         0x81B4A0,
	["K_ENCOUNTERS"] =                                                    0x81B4A8,
	["K_SQUADS"] =                                                        0x81B4AC,
	["K_PLATOONS"] =                                                      0x81B4A4,
	["K_AI_PURSUITS"] =                                                   0x81B4B0,
	
	["K_AI_COMMUNICATION_REPLIES"] =                                      0x68BC7C,
	["K_AI_CONVERSATIONS"] =                                              0x81B4B4,

	["K_AI_UPDATE_HOOK"] =                                                0x42A949,
	["K_ACTOR_ACTION_HANDLE_VEHICLE_EXIT_HOOK"] =                         0x40B105,
	["K_ACTOR_ACTION_HANDLE_VEHICLE_EXIT_RETN"] =                         0x40B10A,
	
--////////////////////////////////////////////////////////////////////////
-- Camera.cpp
	["K_OBSERVERS"] =                                                     0x6475AC,
	["K_CAMERA_SCRIPT_GLOBALS"] =                                         0x621F90,
	["K_DIRECTOR_SCRIPTING"] =                                            0x816DDC,
	["K_CINEMATIC_GLOBALS"] =                                             0x68C85C,
	["K_GLOBAL_DIRECTOR"] =                                               0x6474B0,

	["K_DIRECTOR_GET_PERSPECTIVE_DEFAULT_SET"] =                          0x44661F,
	["K_HUD_INITIALIZE_FOR_NEW_MAP_DEFAULT_SET_SHOW_HUD"] =               0x4ACAEC,
	["K_HUD_ADD_ITEM_MESSAGE_DEFAULT_SET_SHOW_MSG"] =                     0x4B1A6F,
	
	["K_OBSERVER_UPDATE_COMMAND_HOOK"] =                                  0x4486DA,
	
	["K_OBSERVER_UPDATE_POSITIONS"] =                                     0x448DA0,
	["K_OBSERVER_UPDATE_POSITIONS_NO_SCOPE_BLUR"] =                       0x49754D + 2,
	
	["K_OBSERVER_TICK_CALL_HOOK_OBSERVER_UPDATE_POSITIONS"] =             0x448729,
	
	["K_MAX_FOV_CHECK_JMP"] =                                             0x44952D,
	
--////////////////////////////////////////////////////////////////////////
-- Campaign.cpp
	["K_SCENARIO_PATHS_REFERENCE_0"] =                                    0x49FBB3,
	["K_SCENARIO_PATHS_REFERENCE_1"] =                                    0x49FD13,
	["K_SCENARIO_PATHS_REFERENCE_2"] =                                    0x4A00BB,
	["K_SCENARIO_PATHS_REFERENCE_3"] =                                    0x4A0190,
	["K_SCENARIO_PATHS_REFERENCE_4"] =                                    0x4A4C1B,
	--["K_SCENARIO_PATHS_REFERENCE_5"] =                                  0x49FE6C,

	["K_SCENARIO_PATHS_REFERENCE2_0"] =                                   0x4A73F5,
	["K_SCENARIO_PATHS_REFERENCE2_1"] =                                   0x4CD10C,
	["K_SCENARIO_PATHS_REFERENCE2_2"] =                                   0x53C8CB,
	["K_SCENARIO_PATHS_REFERENCE2_3"] =                                   0x53C994,

	["K_DISPLAY_SCENARIO_HELP_HOOK"] =                                    0x49C3BA,
	["K_DISPLAY_SCENARIO_HELP_SUCCESS"] =                                 0x49C4E6,
	["K_DISPLAY_SCENARIO_HELP_FAIL"] =                                    0x49C539,
		
	["K_CAMPAIGN_LEVEL_DATA_REFERENCE_0"] =                               0x49FABB,
	["K_CAMPAIGN_LEVEL_DATA_REFERENCE_1"] =                               0x49FBBA,
	["K_CAMPAIGN_LEVEL_DATA_REFERENCE_2"] =                               0x49FCEE,
	["K_CAMPAIGN_LEVEL_DATA_REFERENCE_3"] =                               0x49FF39,
	--["K_CAMPAIGN_LEVEL_DATA_REFERENCE_4"] =                             0x49FDFB,
	--["K_CAMPAIGN_LEVEL_DATA_REFERENCE_5"] =                             0x49FE73,
	--["K_CAMPAIGN_LEVEL_DATA_REFERENCE_6"] =                             0x49FEDF,

	["K_CAMPAIGN_LEVEL_DATA_SIZE_REFERENCE_0"] =                          0x49FAB6,
	["K_CAMPAIGN_LEVEL_DATA_SIZE_REFERENCE_1"] =                          0x49FDF6,
	["K_CAMPAIGN_LEVEL_DATA_SIZE_REFERENCE_2"] =                          0x49FF34,
	
	["K_CAMPAIGN_LEVEL_COUNT_REFERENCE_8BIT_0"] =                         0x4CD0F9,
	--["K_CAMPAIGN_LEVEL_COUNT_REFERENCE_8BIT_1"] =                       0x49FF05,

	["K_CAMPAIGN_LEVEL_COUNT_REFERENCE_16BIT_0"] =                        0x49FB81,

	["K_CAMPAIGN_LEVEL_COUNT_REFERENCE_32BIT_0"] =                        0x49FB89,
	--["K_CAMPAIGN_LEVEL_COUNT_REFERENCE_32BIT_1"] =                      0x49FF09,
	
	["K_CAMPAIGN_LEVEL_COUNT_REFERENCE2_8BIT_0"] =                        0x49FCD4,
	["K_CAMPAIGN_LEVEL_COUNT_REFERENCE2_8BIT_1"] =                        0x49FD2C,
	["K_CAMPAIGN_LEVEL_COUNT_REFERENCE2_8BIT_2"] =                        0x49FD64,
	["K_CAMPAIGN_LEVEL_COUNT_REFERENCE2_8BIT_3"] =                        0x4CD0C7,
	["K_CAMPAIGN_LEVEL_COUNT_REFERENCE2_8BIT_4"] =                        0x4CD106,
	["K_CAMPAIGN_LEVEL_COUNT_REFERENCE2_8BIT_5"] =                        0x53C8C2,
	--["K_CAMPAIGN_LEVEL_COUNT_REFERENCE2_8BIT_6"] =                      0x49FED6,
	
	["K_CAMPAIGN_LEVEL_COUNT_REFERENCE2_16BIT_0"] =                       0x49FCF6,
	["K_CAMPAIGN_LEVEL_COUNT_REFERENCE2_16BIT_1"] =                       0x53C98A,
	--["K_CAMPAIGN_LEVEL_COUNT_REFERENCE2_16BIT_2"] =                     0x49FEE7,
	
--////////////////////////////////////////////////////////////////////////
-- Console.cpp
	["K_TERMINAL_GLOBALS"] =                                              0x64DE4C,
	["K_CONSOLE_GLOBALS"] =                                               0x651F70,

	["K_CONSOLE_UPDATE_HOOK"] =                                           0x4C9BE3,
	
--////////////////////////////////////////////////////////////////////////
-- Effects.cpp
	["K_CONTRAILS"] =                                                     0x815DAC,
	["K_CONTRAIL_POINTS"] =                                               0x815DA8,
	["K_PARTICLES"] =                                                     0x815D90,
	["K_EFFECTS"] =                                                       0x815D9C,
	["K_EFFECT_LOCATIONS"] =                                              0x815DA0,
	["K_PARTICLE_SYSTEMS"] =                                              0x815D98,
	["K_PARTICLE_SYSTEM_PARTICLES"] =                                     0x815D94,
	["K_STRUCTURE_DETAIL_OBJECTS"] =                                      0x6BDA6C,
	["K_STRUCTURE_DECALS"] =                                              0x6BDA7C,
	["K_BREAKABLE_SURFACE_GLOBALS"] =                                     0x653CC8,
	["K_DECALS"] =                                                        0x815DA4,
	["K_DECAL_GLOBALS"] =                                                 0x64BA28,
	["K_DECAL_VERTEX_CACHE"] =                                            0x6B8478,

	["K_EFFECTS_UPDATE_HOOK"] =                                           0x451697,
	
	["K_RENDER_PARTICLES_RENDERED_PARTICLES_ARRAY_ADDRESS_0"] =           0x513ABA + 0x30,
	["K_RENDER_PARTICLES_RENDERED_PARTICLES_ARRAY_ADDRESS_1"] =           0x513B4B + 0x30,
	["K_RENDER_PARTICLES_RENDERED_PARTICLES_ARRAY_ADDRESS_2"] =           0x513B52 + 0x30,
	["K_RENDER_PARTICLES_RENDERED_PARTICLES_ARRAY_ADDRESS_3"] =           0x513B79 + 0x30,
	["K_RENDER_PARTICLES_RENDERED_PARTICLES_ARRAY_ADDRESS_4"] =           0x513BCB + 0x30,

	["K_GAME_INITIALIZE_MOD_PER_MAP_UPGRADE_PARTICLES"] =                 0x45B6B7 + 0x10,
	["K_GAME_INITIALIZE_MOD_PER_MAP_UPGRADE_EFFECTS"] =                   0x45B6CB + 0x10,
	["K_GAME_INITIALIZE_MOD_PER_MAP_UPGRADE_EFFECT_LOCATIONS"] =          0x45B6E4 + 0x10,
	["K_GAME_INITIALIZE_MOD_PER_MAP_UPGRADE_PARTICLE_SYSTEM_PARTICLES"] = 0x45B72C + 0x10,
		
		
--////////////////////////////////////////////////////////////////////////
-- GAMEBUILDNUMBER.CPP
	["K_GAME_BUILD_VERSION"] =                                            0x60A3C8,
	["K_GAME_BUILD_VERSION_GAMESPY"] =                                    0x6BDD18,
	["K_NETWORK_VERSION_ID1"] =                                           0x4DC2D6,
	["K_NETWORK_VERSION_ID2"] =                                           0x4DC4B3,
	["K_NETWORK_VERSION_ID3"] =                                           0x4E3FF3,

	["K_GAME_STATE_HEADER_IS_VALID_HOOK"] =                               0x53BBB7 + 0x30,
	["K_GAME_STATE_HEADER_IS_VALID_HOOK_RET"] =                           0x53BC0B + 0x30,
	["K_GAME_STATE_HEADER_TRY_AND_LOAD_HOOK"] =                           0x53B96E + 0x30,
	["K_GAME_STATE_HEADER_TRY_AND_LOAD_HOOK_RET_TRUE"] =                  0x53B9CE + 0x30,
	["K_GAME_STATE_HEADER_TRY_AND_LOAD_HOOK_RET_FALSE"] =                 0x53BA89 + 0x30,
	
--////////////////////////////////////////////////////////////////////////
-- GameEngine.cpp
	["K_CTF_GLOBALS"] =                                                   0x64BDD8,
	["K_SLAYER_GLOBALS"] =                                                0x64C328,
	["K_ODDBALL_GLOBALS"] =                                               0x64C098,
	["K_KING_GLOBALS"] =                                                  0x64BE10,
	["K_RACE_GLOBALS"] =                                                  0x64C1E0,
	["K_GLOBAL_VARIANT"] =                                                0x68CC68,
	["K_GAME_ENGINES"] =                                                  0x6238C8,
	["K_GAME_ENGINE"] =                                                   0x68CD44,
	
--////////////////////////////////////////////////////////////////////////
-- FixHudRenderUnitInterface()
	["K_HUD_RENDER_UNIT_INTERFACE_JMP_TABLE"] =                           0x4B4A84,
	["K_HUD_RENDER_UNIT_INTERFACE_JMP_PTR"] =                             0x4B485C,
	
--////////////////////////////////////////////////////////////////////////
-- FixFunc4F7440()
	["K_FUNC4F7440JMPTABLE"] =                                            0x4F764C,
	["K_FUNC4F7440JMPPTR"] =                                              0x4F7565,
	
--////////////////////////////////////////////////////////////////////////
-- FixFunc4F7580()
	["K_FUNC4F7580JMPTABLE"] =                                            0x4F774C,
	["K_FUNC4F7580JMPPTR"] =                                              0x4F76C3,
	
--////////////////////////////////////////////////////////////////////////
-- Initialize()
	["K_GAME_ENGINE_INIT_MOD"] =                                          0x45CEA4,

	["K_GAME_ENGINE_INITIALIZE_FOR_NEW_MAP_HOOK"] =                       0x45CFD8,
	["K_GAME_ENGINE_UPDATE_HOOK"] =                                       0x460F61,
	
--////////////////////////////////////////////////////////////////////////
-- GameState.cpp
	["K_GAME_STATE_BEFORE_SAVE_PROCS_CALL_0"] =                           0x4CAD3A + 0x30,
	["K_GAME_STATE_BEFORE_SAVE_PROCS_CALL_1"] =                           0x53B870 + 0x30,
	
	["K_GAME_STATE_BEFORE_LOAD_PROCS_CALL_0"] =                           0x53B90D + 0x30,
	["K_GAME_STATE_BEFORE_LOAD_PROCS_CALL_1"] =                           0x53BA53 + 0x30,
	["K_GAME_STATE_BEFORE_LOAD_PROCS_CALL_2"] =                           0x53BB45 + 0x30, 
	
	["K_GAME_STATE_AFTER_LOAD_PROCS_REF_0"] =                             0x53B622 + 1 + 0x30,
	["K_GAME_STATE_AFTER_LOAD_PROCS_REF_1"] =                             0x53B918 + 1 + 0x30,
	["K_GAME_STATE_AFTER_LOAD_PROCS_REF_2"] =                             0x53BB6E + 1 + 0x30,
	
	["K_GAME_STATE_AFTER_LOAD_PROCS_COUNT_0"] =                           0x53B627 + 1 + 0x30,
	["K_GAME_STATE_AFTER_LOAD_PROCS_COUNT_1"] =                           0x53B91D + 1 + 0x30,
	["K_GAME_STATE_AFTER_LOAD_PROCS_COUNT_2"] =                           0x53BB73 + 1 + 0x30,
		
	["K_MAIN_GLOBALS"] =                                                  0x6B47B8,
	["K_PHYSICAL_MEMORY_GLOBALS"] =                                       0x647478,
	["K_PHYSICAL_MEMORY_INITIALIZE"] =                                    0x445400,
	["K_PHYSICAL_MEMORY_ALLOCATION_SIZE"] =                               0x445428,
	["K_GAME_STATE_GLOBALS"] =                                            0x67DDA8,
	["K_GAME_GLOBALS"] =                                                  0x64BAD0,
	["K_GAME_TIME_GLOBALS"] =                                             0x68CD90,
	["K_GAME_ALLEGIANCE_GLOBALS"] =                                       0x64BAD4,
	["K_GLOBAL_GAME_GLOBALS"] =                                           0x6E2280,
	["K_GLOBAL_PHYSICS"] =                                                0x637BF4,
	["K_GLOBAL_POINT_PHYSICS"] =                                          0x653CCC,
	["K_DEVELOPER_MODE"] =                                                0x815DC6,
	["K_TRANSPORT_DUMPING"] =                                             0x621F60,
	["K_INIT_TXT_FILENAME"] =                                             0x60AAF4,
	["K_DEVMODE_ENABLED"] =                                               0x6BD17E,

	["K_GAME_INITIALIZE_HOOK"] =                                          0x45B84B,
	["K_GAME_INITIALIZE_FOR_NEW_MAP_HOOK"] =                              0x45BEEB,
	["K_GAME_DISPOSE_FROM_OLD_MAP_HOOK"] =                                0x45C060,
	
	["K_GAME_SCENARIO_SWITCH_BSP_CALL_0"] =                               0x482440,
	["K_GAME_SCENARIO_SWITCH_BSP_CALL_1"] =                               0x4CD037,
	["K_GAME_SCENARIO_SWITCH_BSP_CALL_2"] =                               0x4CD161,
	["K_GAME_SCENARIO_SWITCH_BSP_CALL_3"] =                               0x541E68,
	["K_GAME_SCENARIO_SWITCH_BSP_CALL_4"] =                               0x542779,
		
	["K_GAME_DISPOSE_FROM_OLD_BSP_HOOK"] =                                0x442996,

	["K_GAME_SERVER_QR2_STRING_MATCHES_GAMEVER_CALL"] =                   0x4BA7CB,

	["K_GAME_CREATE_NETWORK_SERVER"] =                                    0x4E21E0,
	["K_GAME_CREATE_NETWORK_CLIENT"] =                                    0x4DBFC0,
	["K_GAME_CREATE_NETWORK_SERVER_CALL"] =                               0x4E1340,
	
	["K_GAME_CREATE_NETWORK_CLIENT_CALL_0"] =                             0x498C94,
	["K_GAME_CREATE_NETWORK_CLIENT_CALL_1"] =                             0x4A0343,
	["K_GAME_CREATE_NETWORK_CLIENT_CALL_2"] =                             0x4A03E5,
	["K_GAME_CREATE_NETWORK_CLIENT_CALL_3"] =                             0x4A062E,
	["K_GAME_CREATE_NETWORK_CLIENT_CALL_4"] =                             0x4DFEF2,
	["K_GAME_CREATE_NETWORK_CLIENT_CALL_5"] =                             0x4E1450,
	["K_GAME_CREATE_NETWORK_CLIENT_CALL_6"] =                             0x4E7B3E,
		
--////////////////////////////////////////////////////////////////////////
-- Lights.cpp
	["K_LIGHTS"] =                                                        0x7FBB14,
	["K_LIGHT_GAME_GLOBALS"] =                                            0x6B8270,
	["K_LIGHT_CLUSTER"] =                                                 0x7FBB20,
	["K_CLUSTER_LIGHT_REFERENCE"] =                                       0x7FBB24,
	["K_LIGHT_CLUSTER_REFERENCE"] =                                       0x7FBB28,
	
--////////////////////////////////////////////////////////////////////////
-- Players.cpp
	["K_PLAYERS"] =                                                       0x8155C0,
	["K_TEAMS"] =                                                         0x8155BC,
	["K_PLAYERS_GLOBALS"] =                                               0x8155B8,
	["K_PLAYER_CONTROL_GLOBALS"] =                                        0x64C3AC,
	["K_PLAYER_EFFECTS"] =                                                0x68C864,
	["K_MACHINE_TO_PLAYER_TABLE"] =                                       0x64C3B0,

	["K_PLAYER_SPAWN__CREATE_UNIT_MP_HOOK"] =                             0x47AD21,
	["K_PLAYER_SPAWN__CREATE_UNIT_MP_HOOK_RETN"] =                        0x47AE40,

	["K_PLAYERS_UPDATE_BEFORE_GAME_SERVER_HOOK"] =                        0x4773B7,
	["K_PLAYERS_UPDATE_BEFORE_GAME_CLIENT_HOOK"] =                        0x4777EB,
	["K_PLAYERS_UPDATE_AFTER_GAME_HOOK"] =                                0x477B7B,
	
--////////////////////////////////////////////////////////////////////////
-- Players.NearbyObjects
	["K_PLAYER_EXAMINE_NEARBY_OBJECTS_SERVER_JMP_TABLE_RETN"] =           0x47B268,
	["K_PLAYER_EXAMINE_NEARBY_OBJECTS_SERVER_JMP_PTR"] =                  0x47B248,
	["K_PLAYER_EXAMINE_NEARBY_OBJECTS_SERVER_LAST_ENTRY_TYPE"] =          0x47B242,
	["K_PLAYER_EXAMINE_NEARBY_OBJECTS_SERVER_JMP_TABLE"] =                0x47B274,

	["K_PLAYER_EXAMINE_NEARBY_OBJECTS_CLIENT_JMP_TABLE_RETN"] =           0x47B368,
	["K_PLAYER_EXAMINE_NEARBY_OBJECTS_CLIENT_JMP_PTR"] =                  0x47B348,
	["K_PLAYER_EXAMINE_NEARBY_OBJECTS_CLIENT_LAST_ENTRY_TYPE"] =          0x47B342,
	["K_PLAYER_EXAMINE_NEARBY_OBJECTS_CLIENT_JMP_TABLE"] =                0x47B374,
	
--////////////////////////////////////////////////////////////////////////
-- Players.ActionResults
	["K_PLAYER_HANDLE_ACTION_JMP_TABLE_ADJUST_SIZE"] =                    0x47C183,
	["K_PLAYER_HANDLE_ACTION_JMP_TABLE_RETN"] =                           0x47C47B,
	["K_PLAYER_HANDLE_ACTION_LAST_ENTRY_TYPE"] =                          0x47C188,
	["K_PLAYER_HANDLE_ACTION_JMP_PTR"] =                                  0x47C199,
	["K_PLAYER_HANDLE_ACTION_JMP_TABLE"] =                                0x47C488,
}

HaloCEClient_Game_Index =
{
	[1] = "K_AI_GLOBALS",
	[2] = "K_ACTORS",
	[3] = "K_SWARMS",
	[4] = "K_SWARM_COMPONENTS",
	[5] = "K_PROPS",
	[6] = "K_ENCOUNTERS",
	[7] = "K_SQUADS",
	[8] = "K_PLATOONS",
	[9] = "K_AI_PURSUITS",
	[10] = "K_AI_COMMUNICATION_REPLIES",
	[11] = "K_AI_CONVERSATIONS",
	[12] = "K_AI_UPDATE_HOOK",
	[13] = "K_ACTOR_ACTION_HANDLE_VEHICLE_EXIT_HOOK",
	[14] = "K_ACTOR_ACTION_HANDLE_VEHICLE_EXIT_RETN",
	[15] = "K_OBSERVERS",
	[16] = "K_CAMERA_SCRIPT_GLOBALS",
	[17] = "K_DIRECTOR_SCRIPTING",
	[18] = "K_CINEMATIC_GLOBALS",
	[19] = "K_GLOBAL_DIRECTOR",
	[20] = "K_DIRECTOR_GET_PERSPECTIVE_DEFAULT_SET",
	[21] = "K_HUD_INITIALIZE_FOR_NEW_MAP_DEFAULT_SET_SHOW_HUD",
	[22] = "K_HUD_ADD_ITEM_MESSAGE_DEFAULT_SET_SHOW_MSG",
	[23] = "K_OBSERVER_UPDATE_COMMAND_HOOK",
	[24] = "K_OBSERVER_UPDATE_POSITIONS",
	[25] = "K_OBSERVER_UPDATE_POSITIONS_NO_SCOPE_BLUR",
	[26] = "K_OBSERVER_TICK_CALL_HOOK_OBSERVER_UPDATE_POSITIONS",
	[27] = "K_MAX_FOV_CHECK_JMP",
	[28] = "K_SCENARIO_PATHS_REFERENCE_0",
	[29] = "K_SCENARIO_PATHS_REFERENCE_1",
	[30] = "K_SCENARIO_PATHS_REFERENCE_2",
	[31] = "K_SCENARIO_PATHS_REFERENCE_3",
	[32] = "K_SCENARIO_PATHS_REFERENCE_4",
	[33] = "K_SCENARIO_PATHS_REFERENCE2_0",
	[34] = "K_SCENARIO_PATHS_REFERENCE2_1",
	[35] = "K_SCENARIO_PATHS_REFERENCE2_2",
	[36] = "K_SCENARIO_PATHS_REFERENCE2_3",
	[37] = "K_DISPLAY_SCENARIO_HELP_HOOK",
	[38] = "K_DISPLAY_SCENARIO_HELP_SUCCESS",
	[39] = "K_DISPLAY_SCENARIO_HELP_FAIL",
	[40] = "K_CAMPAIGN_LEVEL_DATA_REFERENCE_0",
	[41] = "K_CAMPAIGN_LEVEL_DATA_REFERENCE_1",
	[42] = "K_CAMPAIGN_LEVEL_DATA_REFERENCE_2",
	[43] = "K_CAMPAIGN_LEVEL_DATA_REFERENCE_3",
	[44] = "K_CAMPAIGN_LEVEL_DATA_SIZE_REFERENCE_0",
	[45] = "K_CAMPAIGN_LEVEL_DATA_SIZE_REFERENCE_1",
	[46] = "K_CAMPAIGN_LEVEL_DATA_SIZE_REFERENCE_2",
	[47] = "K_CAMPAIGN_LEVEL_COUNT_REFERENCE_8BIT_0",
	[48] = "K_CAMPAIGN_LEVEL_COUNT_REFERENCE_16BIT_0",
	[49] = "K_CAMPAIGN_LEVEL_COUNT_REFERENCE_32BIT_0",
	[50] = "K_CAMPAIGN_LEVEL_COUNT_REFERENCE2_8BIT_0",
	[51] = "K_CAMPAIGN_LEVEL_COUNT_REFERENCE2_8BIT_1",
	[52] = "K_CAMPAIGN_LEVEL_COUNT_REFERENCE2_8BIT_2",
	[53] = "K_CAMPAIGN_LEVEL_COUNT_REFERENCE2_8BIT_3",
	[54] = "K_CAMPAIGN_LEVEL_COUNT_REFERENCE2_8BIT_4",
	[55] = "K_CAMPAIGN_LEVEL_COUNT_REFERENCE2_8BIT_5",
	[56] = "K_CAMPAIGN_LEVEL_COUNT_REFERENCE2_16BIT_0",
	[57] = "K_CAMPAIGN_LEVEL_COUNT_REFERENCE2_16BIT_1",
	[58] = "K_TERMINAL_GLOBALS",
	[59] = "K_CONSOLE_GLOBALS",
	[60] = "K_CONSOLE_UPDATE_HOOK",
	[61] = "K_CONTRAILS",
	[62] = "K_CONTRAIL_POINTS",
	[63] = "K_PARTICLES",
	[64] = "K_EFFECTS",
	[65] = "K_EFFECT_LOCATIONS",
	[66] = "K_PARTICLE_SYSTEMS",
	[67] = "K_PARTICLE_SYSTEM_PARTICLES",
	[68] = "K_STRUCTURE_DETAIL_OBJECTS",
	[69] = "K_STRUCTURE_DECALS",
	[70] = "K_BREAKABLE_SURFACE_GLOBALS",
	[71] = "K_DECALS",
	[72] = "K_DECAL_GLOBALS",
	[73] = "K_DECAL_VERTEX_CACHE",
	[74] = "K_EFFECTS_UPDATE_HOOK",
	[75] = "K_RENDER_PARTICLES_RENDERED_PARTICLES_ARRAY_ADDRESS_0",
	[76] = "K_RENDER_PARTICLES_RENDERED_PARTICLES_ARRAY_ADDRESS_1",
	[77] = "K_RENDER_PARTICLES_RENDERED_PARTICLES_ARRAY_ADDRESS_2",
	[78] = "K_RENDER_PARTICLES_RENDERED_PARTICLES_ARRAY_ADDRESS_3",
	[79] = "K_RENDER_PARTICLES_RENDERED_PARTICLES_ARRAY_ADDRESS_4",
	[80] = "K_GAME_INITIALIZE_MOD_PER_MAP_UPGRADE_PARTICLES",
	[81] = "K_GAME_INITIALIZE_MOD_PER_MAP_UPGRADE_EFFECTS",
	[82] = "K_GAME_INITIALIZE_MOD_PER_MAP_UPGRADE_EFFECT_LOCATIONS",
	[83] = "K_GAME_INITIALIZE_MOD_PER_MAP_UPGRADE_PARTICLE_SYSTEM_PARTICLES",
	[84] = "K_GAME_BUILD_VERSION",
	[85] = "K_GAME_BUILD_VERSION_GAMESPY",
	[86] = "K_NETWORK_VERSION_ID1",
	[87] = "K_NETWORK_VERSION_ID2",
	[88] = "K_NETWORK_VERSION_ID3",
	[89] = "K_GAME_STATE_HEADER_IS_VALID_HOOK",
	[90] = "K_GAME_STATE_HEADER_IS_VALID_HOOK_RET",
	[91] = "K_GAME_STATE_HEADER_TRY_AND_LOAD_HOOK",
	[92] = "K_GAME_STATE_HEADER_TRY_AND_LOAD_HOOK_RET_TRUE",
	[93] = "K_GAME_STATE_HEADER_TRY_AND_LOAD_HOOK_RET_FALSE",
	[94] = "K_CTF_GLOBALS",
	[95] = "K_SLAYER_GLOBALS",
	[96] = "K_ODDBALL_GLOBALS",
	[97] = "K_KING_GLOBALS",
	[98] = "K_RACE_GLOBALS",
	[99] = "K_GLOBAL_VARIANT",
	[100] = "K_GAME_ENGINES",
	[101] = "K_GAME_ENGINE",
	[102] = "K_HUD_RENDER_UNIT_INTERFACE_JMP_TABLE",
	[103] = "K_HUD_RENDER_UNIT_INTERFACE_JMP_PTR",
	[104] = "K_FUNC4F7440JMPTABLE",
	[105] = "K_FUNC4F7440JMPPTR",
	[106] = "K_FUNC4F7580JMPTABLE",
	[107] = "K_FUNC4F7580JMPPTR",
	[108] = "K_GAME_ENGINE_INIT_MOD",
	[109] = "K_GAME_ENGINE_INITIALIZE_FOR_NEW_MAP_HOOK",
	[110] = "K_GAME_ENGINE_UPDATE_HOOK",
	[111] = "K_GAME_STATE_BEFORE_SAVE_PROCS_CALL_0",
	[112] = "K_GAME_STATE_BEFORE_SAVE_PROCS_CALL_1",
	[113] = "K_GAME_STATE_BEFORE_LOAD_PROCS_CALL_0",
	[114] = "K_GAME_STATE_BEFORE_LOAD_PROCS_CALL_1",
	[115] = "K_GAME_STATE_BEFORE_LOAD_PROCS_CALL_2",
	[116] = "K_GAME_STATE_AFTER_LOAD_PROCS_REF_0",
	[117] = "K_GAME_STATE_AFTER_LOAD_PROCS_REF_1",
	[118] = "K_GAME_STATE_AFTER_LOAD_PROCS_REF_2",
	[119] = "K_GAME_STATE_AFTER_LOAD_PROCS_COUNT_0",
	[120] = "K_GAME_STATE_AFTER_LOAD_PROCS_COUNT_1",
	[121] = "K_GAME_STATE_AFTER_LOAD_PROCS_COUNT_2",
	[122] = "K_MAIN_GLOBALS",
	[123] = "K_PHYSICAL_MEMORY_GLOBALS",
	[124] = "K_PHYSICAL_MEMORY_INITIALIZE",
	[125] = "K_PHYSICAL_MEMORY_ALLOCATION_SIZE",
	[126] = "K_GAME_STATE_GLOBALS",
	[127] = "K_GAME_GLOBALS",
	[128] = "K_GAME_TIME_GLOBALS",
	[129] = "K_GAME_ALLEGIANCE_GLOBALS",
	[130] = "K_GLOBAL_GAME_GLOBALS",
	[131] = "K_GLOBAL_PHYSICS",
	[132] = "K_GLOBAL_POINT_PHYSICS",
	[133] = "K_DEVELOPER_MODE",
	[134] = "K_TRANSPORT_DUMPING",
	[135] = "K_INIT_TXT_FILENAME",
	[136] = "K_DEVMODE_ENABLED",
	[137] = "K_GAME_INITIALIZE_HOOK",
	[138] = "K_GAME_INITIALIZE_FOR_NEW_MAP_HOOK",
	[139] = "K_GAME_DISPOSE_FROM_OLD_MAP_HOOK",
	[140] = "K_GAME_SCENARIO_SWITCH_BSP_CALL_0",
	[141] = "K_GAME_SCENARIO_SWITCH_BSP_CALL_1",
	[142] = "K_GAME_SCENARIO_SWITCH_BSP_CALL_2",
	[143] = "K_GAME_SCENARIO_SWITCH_BSP_CALL_3",
	[144] = "K_GAME_SCENARIO_SWITCH_BSP_CALL_4",
	[145] = "K_GAME_DISPOSE_FROM_OLD_BSP_HOOK",
	[146] = "K_GAME_SERVER_QR2_STRING_MATCHES_GAMEVER_CALL",
	[147] = "K_GAME_CREATE_NETWORK_SERVER",
	[148] = "K_GAME_CREATE_NETWORK_CLIENT",
	[149] = "K_GAME_CREATE_NETWORK_SERVER_CALL",
	[150] = "K_GAME_CREATE_NETWORK_CLIENT_CALL_0",
	[151] = "K_GAME_CREATE_NETWORK_CLIENT_CALL_1",
	[152] = "K_GAME_CREATE_NETWORK_CLIENT_CALL_2",
	[153] = "K_GAME_CREATE_NETWORK_CLIENT_CALL_3",
	[154] = "K_GAME_CREATE_NETWORK_CLIENT_CALL_4",
	[155] = "K_GAME_CREATE_NETWORK_CLIENT_CALL_5",
	[156] = "K_GAME_CREATE_NETWORK_CLIENT_CALL_6",
	[157] = "K_LIGHTS",
	[158] = "K_LIGHT_GAME_GLOBALS",
	[159] = "K_LIGHT_CLUSTER",
	[160] = "K_CLUSTER_LIGHT_REFERENCE",
	[161] = "K_LIGHT_CLUSTER_REFERENCE",
	[162] = "K_PLAYERS",
	[163] = "K_TEAMS",
	[164] = "K_PLAYERS_GLOBALS",
	[165] = "K_PLAYER_CONTROL_GLOBALS",
	[166] = "K_PLAYER_EFFECTS",
	[167] = "K_MACHINE_TO_PLAYER_TABLE",
	[168] = "K_PLAYER_SPAWN__CREATE_UNIT_MP_HOOK",
	[169] = "K_PLAYER_SPAWN__CREATE_UNIT_MP_HOOK_RETN",
	[170] = "K_PLAYERS_UPDATE_BEFORE_GAME_SERVER_HOOK",
	[171] = "K_PLAYERS_UPDATE_BEFORE_GAME_CLIENT_HOOK",
	[172] = "K_PLAYERS_UPDATE_AFTER_GAME_HOOK",
	[173] = "K_PLAYER_EXAMINE_NEARBY_OBJECTS_SERVER_JMP_TABLE_RETN",
	[174] = "K_PLAYER_EXAMINE_NEARBY_OBJECTS_SERVER_JMP_PTR",
	[175] = "K_PLAYER_EXAMINE_NEARBY_OBJECTS_SERVER_LAST_ENTRY_TYPE",
	[176] = "K_PLAYER_EXAMINE_NEARBY_OBJECTS_SERVER_JMP_TABLE",
	[177] = "K_PLAYER_EXAMINE_NEARBY_OBJECTS_CLIENT_JMP_TABLE_RETN",
	[178] = "K_PLAYER_EXAMINE_NEARBY_OBJECTS_CLIENT_JMP_PTR",
	[179] = "K_PLAYER_EXAMINE_NEARBY_OBJECTS_CLIENT_LAST_ENTRY_TYPE",
	[180] = "K_PLAYER_EXAMINE_NEARBY_OBJECTS_CLIENT_JMP_TABLE",
	[181] = "K_PLAYER_HANDLE_ACTION_JMP_TABLE_ADJUST_SIZE",
	[182] = "K_PLAYER_HANDLE_ACTION_JMP_TABLE_RETN",
	[183] = "K_PLAYER_HANDLE_ACTION_LAST_ENTRY_TYPE",
	[184] = "K_PLAYER_HANDLE_ACTION_JMP_PTR",
	[185] = "K_PLAYER_HANDLE_ACTION_JMP_TABLE",
}