--	Yelo: Open Sauce SDK
--		Halo 1 (CE) Edition

--	See license\OpenSauce\Halo1_CE for specific license information

HaloCEClient_Game_EngineFunctions =
{
	["K_GATHER_EXCEPTION_DATA"] =                        0x6BDEAC,
	["K_GATHER_EXCEPTION"] =                             0x582780,
	["K_RASTERIZER_SET_TEXTURE_BITMAP_DATA"] =           0x51C370,
	["K_RESOLUTION_LIST_ADD_RESOLUTION"] =               0x4BE350,
	["K_ANIMATION_CHOOSE_RANDOM_PERMUTATION_INTERNAL"] = 0x4D97C0,
	["K_GSMD5DIGEST"] =                                  0x5BFB70,
	["K_SHELL_GET_COMMAND_LINE_ARGUMENT"] =              0x5460E0,

--////////////////////////////////////////////////////////////////////////
-- AI
	["K_ACTOR_DELETE"] =                                 0x427E60,
	["K_AI_SCRIPTING_ATTACH_FREE"] =                     0x435420,

--////////////////////////////////////////////////////////////////////////
-- Cache
	["K_CACHE_FILE_READ_REQUEST"] =                      0x443580 + 0x10,
	["K_SOUND_CACHE_SOUND_REQUEST"] =                    0x444940,
	["K_TEXTURE_CACHE_BITMAP_GET_HARDWARE_FORMAT"] =     0x445080,
	["K_PHYSICAL_MEMORY_MAP_PREDICT_RESOURCES"] =        0x445520,

--////////////////////////////////////////////////////////////////////////
-- Console
	["K_CONSOLE_PROCESS_COMMAND"] =                      0x4C9F40,
	["K_CONSOLE_PRINTF"] =                               0x4C9D40,
	["K_CONSOLE_RESPONSE_PRINTF"] =                      0x4C9DE0,
	["K_CONSOLE_WARNING"] =                              0x4C9EA0,

--////////////////////////////////////////////////////////////////////////
-- Cheats
	["K_CHEAT_ALL_WEAPONS"] =                            0x45B0B0,
	["K_CHEAT_SPAWN_WARTHOG"] =                          0x45B140,
	["K_CHEAT_TELEPORT_TO_CAMERA"] =                     0x45B1B0,
	["K_CHEAT_ACTIVE_CAMOFLAGE"] =                       0x45B240,
	["K_CHEAT_ACTIVE_CAMOFLAGE_LOCAL_PLAYER"] =          0x45B2A0,
	["K_CHEAT_PLAYER_INDEX"] =                           0x45B320,
	["K_DIRECTOR_SAVE_CAMERA"] =                         0x4463B0,
	["K_DIRECTOR_LOAD_CAMERA"] =                         0x446470,

--////////////////////////////////////////////////////////////////////////
-- Effects
	["K_EFFECT_NEW_ON_OBJECT_MARKER"] =                  0x48B640,

--////////////////////////////////////////////////////////////////////////
-- Game
	["K_SCENARIO_SWITCH_STRUCTURE_BSP"] =                0x542610,
	["K_BINK_PLAYBACK_START"] =                          0x43ED20,
	["K_GAME_ENGINE_RASTERIZE_MESSAGE"] =                0x462C90,
	["K_GAME_TEAM_IS_ENEMY"] =                           0x45C8D0,
	["K_GAME_ENGINE_PLAY_MULTIPLAYER_SOUND"] =           0x46EC70,

--////////////////////////////////////////////////////////////////////////
-- HS
	["K_OBJECT_LIST_ADD"] =                              0x48E000,

--////////////////////////////////////////////////////////////////////////
-- Input
	["K_INPUT_KEY_IS_DOWN"] =                            0x493AD0,

--////////////////////////////////////////////////////////////////////////
-- Interface
	["K_KEYSTONE_CHAT_LOG_ADD_STRING"] =                 0x4AEAD0,
	["K_HUD_PRINT_MESSAGE"] =                            0x4B1790,
	["K_HUD_GET_ITEM_MESSAGE"] =                         0x4AD630,
	["K_HUD_DRAW_METER"] =                               0x4AF1B0 + 0x30,
	["K_HUD_DRAW_NUMBERS"] =                             0x4AF6A0 + 0x30,
	["K_HUD_DRAW_STATIC_ELEMENT"] =                      0x4AFCE0 + 0x30,
	["K_HUD_DRAW_BITMAP"] =                              0x4B00C0 + 0x30,
	["K_MAINMENU_LOAD"] =                                0x4CBF00,

--////////////////////////////////////////////////////////////////////////
-- Math
	["K_PERIODIC_FUNCTION_EVALUATE"] =                   0x4CFEF0,
	["K_TRANSITION_FUNCTION_EVALUATE"] =                 0x4D0000,

--////////////////////////////////////////////////////////////////////////
-- Memory
	["K_DATA_NEW"] =                                     0x4D3880 + 0x30,
	["K_DATUM_NEW_AT_INDEX"] =                           0x4D3910,
	["K_DATUM_NEW"] =                                    0x4D39C0,
	["K_DATUM_DELETE"] =                                 0x4D3A50,
	["K_DATA_DELETE_ALL"] =                              0x4D3A90 + 0x30,
	["K_DATA_ITERATOR_NEXT"] =                           0x4D3B10,
	["K_DATA_NEXT_INDEX"] =                              0x4D3B70,
	["K_DATUM_TRY_AND_GET"] =                            0x4D3BC0,
	["K_DATUM_INITIALIZE"] =                             0x4D3C00,

--////////////////////////////////////////////////////////////////////////
-- Networking
	["K_HUD_CHAT_TO_NETWORK"] =                          0x4ADF70,
	["K_INDEX_RESOLUTION_TABLE_TRANSLATE"] =             0x4F4270,
	["K_MAIN_CONNECT"] =                                 0x4CB950 + 0x30,

--////////////////////////////////////////////////////////////////////////
-- Objects
	["K_HS_OBJECT_ORIENT"] =                             0x48ACA0,

	["K_OBJECT_HEADER_BLOCK_ALLOCATE"] =                 0x4FBAE0,
	["K_OBJECT_ITERATOR_NEXT"] =                         0x4FAB80 + 0x30,

	["K_OBJECT_PLACEMENT_DATA_NEW"] =                    0x4F9030,
	["K_OBJECT_NEW"] =                                   0x4F90F0,
	["K_OBJECT_NEW_WITH_ROLE"] =                         0x4F9140,
	["K_OBJECT_DELETE_TO_NETWORK"] =                     0x4F9730,
	["K_OBJECT_DELETE"] =                                0x4F9860,
	["K_OBJECT_ATTACH_TO_MARKER"] =                      0x4F9E10,
	["K_OBJECT_DETACH"] =                                0x4FA2A0,
	["K_OBJECTS_FIND_IN_SPHERE"] =                       0x4FAC70,
	["K_OBJECT_START_INTERPOLATION"] =                   0x4FA800,
	["K_OBJECT_RESET"] =                                 0x4F8DF0,
	["K_OBJECT_RECONNECT_TO_MAP"] =                      0x4F98C0,
	["K_OBJECT_DISCONNECT_FROM_MAP"] =                   0x4F9A70,
	["K_OBJECT_GET_ORIGIN"] =                            0x4FA590,
	["K_OBJECT_GET_ORIENTATION"] =                       0x4FA600,
	["K_OBJECT_GET_LOCATION"] =                          0x4FA7A0,
	["K_OBJECT_SET_POSITION"] =                          0x4F8E50,
	["K_OBJECT_SET_POSITION_NETWORK"] =                  0x4F8F50,
	["K_OBJECT_RESTORE_BODY"] =                          0x4F1660,
	["K_OBJECT_DEPLETE_BODY"] =                          0x4F16B0,
	["K_OBJECT_DEPLETE_SHIELD"] =                        0x4F17A0,
	["K_OBJECT_DOUBLE_CHARGE_SHIELD"] =                  0x4F1830,
	["K_OBJECT_CAUSE_DAMAGE"] =                          0x4F2270,
	["K_OBJECT_DEFINITION_PREDICT"] =                    0x4FB760,
	["K_OBJECT_SET_SCALE"] =                             0x4FD330,
	["K_OBJECT_GET_LEVEL_OF_DETAIL_PIXELS"] =            0x5133E0,
	["K_OBJECT_RENDER_STATE_REFRESH"] =                  0x512F10,
	["K_UNIT_GET_CAMERA_POSITION"] =                     0x56CA80,
	["K_UNIT_SET_ANIMATION"] =                           0x572910,
	["K_UNIT_GET_CUSTOM_ANIMATION_TIME"] =               0x573EF0,
	["K_UNIT_CAN_ENTER_SEAT"] =                          0x56A340,
	["K_UNIT_ENTER_SEAT"] =                              0x56A470,
	["K_UNIT_EXIT_VEHICLE"] =                            0x56F330,
	["K_UNIT_OPEN"] =                                    0x56E850,
	["K_UNIT_CLOSE"] =                                   0x56E870,
	["K_UNIT_FIND_NEARBY_SEAT"] =                        0x56A060,
	["K_UNIT_EXIT_SEAT_END"] =                           0x570380,

--////////////////////////////////////////////////////////////////////////
-- Physics
	["K_COLLISION_TEST_VECTOR"] =                        0x5094F0 + 0x30,

--////////////////////////////////////////////////////////////////////////
-- Players
	["K_PLAYER_INDEX_FROM_UNIT_INDEX"] =                 0x477BF0,
	["K_PLAYER_TELEPORT"] =                              0x478A90,
	["K_PLAYER_OVER_SHIELD_SCREEN_EFFECT"] =             0x47C4B0,
	["K_PLAYER_CAMO_SCREEN_EFFECT"] =                    0x47C570,
	["K_PLAYER_HEALTH_PACK_SCREEN_EFFECT"] =             0x47C630,
	["K_FIND_CLOSEST_PLAYER_INDEX"] =                    0x45EEE0,
	["K_PLAYER_EXAMINE_NEARBY_VEHICLE"] =                0x47B3A0,
	["K_PLAYER_SET_ACTION_RESULT"] =                     0x47BBA0,
	["K_PLAYER_SET_ACTION_RESULT_TO_NETWORK"] =          0x47BD90,

--////////////////////////////////////////////////////////////////////////
-- Render
	["K_RENDER_FRAME"] =                                 0x50FB40,
	["K_RENDER_WINDOW"] =                                0x50FC50,

--////////////////////////////////////////////////////////////////////////
-- Scenario
	["K_SCENARIO_TRIGGER_VOLUME_TEST_POINT"] =           0x542780,

--////////////////////////////////////////////////////////////////////////
-- TagGroups
	["K_TAG_LOADED"] =                                   0x4429A0,
	["K_TAG_ITERATOR_NEXT"] =                            0x442A20,
	["K_UNICODE_STRING_LIST_GET_STRING"] =               0x55B390
}

HaloCEClient_Game_EngineFunctions_Index =
{
	[1] = "K_GATHER_EXCEPTION_DATA",
	[2] = "K_GATHER_EXCEPTION",
	[3] = "K_RASTERIZER_SET_TEXTURE_BITMAP_DATA",
	[4] = "K_RESOLUTION_LIST_ADD_RESOLUTION",
	[5] = "K_ANIMATION_CHOOSE_RANDOM_PERMUTATION_INTERNAL",
	[6] = "K_GSMD5DIGEST",
	[7] = "K_SHELL_GET_COMMAND_LINE_ARGUMENT",
	[8] = "K_ACTOR_DELETE",
	[9] = "K_AI_SCRIPTING_ATTACH_FREE",
	[10] = "K_CACHE_FILE_READ_REQUEST",
	[11] = "K_SOUND_CACHE_SOUND_REQUEST",
	[12] = "K_TEXTURE_CACHE_BITMAP_GET_HARDWARE_FORMAT",
	[13] = "K_PHYSICAL_MEMORY_MAP_PREDICT_RESOURCES",
	[14] = "K_CONSOLE_PROCESS_COMMAND",
	[15] = "K_CONSOLE_PRINTF",
	[16] = "K_CONSOLE_RESPONSE_PRINTF",
	[17] = "K_CONSOLE_WARNING",
	[18] = "K_CHEAT_ALL_WEAPONS",
	[19] = "K_CHEAT_SPAWN_WARTHOG",
	[20] = "K_CHEAT_TELEPORT_TO_CAMERA",
	[21] = "K_CHEAT_ACTIVE_CAMOFLAGE",
	[22] = "K_CHEAT_ACTIVE_CAMOFLAGE_LOCAL_PLAYER",
	[23] = "K_CHEAT_PLAYER_INDEX",
	[24] = "K_DIRECTOR_SAVE_CAMERA",
	[25] = "K_DIRECTOR_LOAD_CAMERA",
	[26] = "K_EFFECT_NEW_ON_OBJECT_MARKER",
	[27] = "K_SCENARIO_SWITCH_STRUCTURE_BSP",
	[28] = "K_BINK_PLAYBACK_START",
	[29] = "K_GAME_ENGINE_RASTERIZE_MESSAGE",
	[30] = "K_GAME_TEAM_IS_ENEMY",
	[31] = "K_GAME_ENGINE_PLAY_MULTIPLAYER_SOUND",
	[32] = "K_OBJECT_LIST_ADD",
	[33] = "K_INPUT_KEY_IS_DOWN",
	[34] = "K_KEYSTONE_CHAT_LOG_ADD_STRING",
	[35] = "K_HUD_PRINT_MESSAGE",
	[36] = "K_HUD_GET_ITEM_MESSAGE",
	[37] = "K_HUD_DRAW_METER",
	[38] = "K_HUD_DRAW_NUMBERS",
	[39] = "K_HUD_DRAW_STATIC_ELEMENT",
	[40] = "K_HUD_DRAW_BITMAP",
	[41] = "K_MAINMENU_LOAD",
	[42] = "K_PERIODIC_FUNCTION_EVALUATE",
	[43] = "K_TRANSITION_FUNCTION_EVALUATE",
	[44] = "K_DATA_NEW",
	[45] = "K_DATUM_NEW_AT_INDEX",
	[46] = "K_DATUM_NEW",
	[47] = "K_DATUM_DELETE",
	[48] = "K_DATA_DELETE_ALL",
	[49] = "K_DATA_ITERATOR_NEXT",
	[50] = "K_DATA_NEXT_INDEX",
	[51] = "K_DATUM_TRY_AND_GET",
	[52] = "K_DATUM_INITIALIZE",
	[53] = "K_HUD_CHAT_TO_NETWORK",
	[54] = "K_INDEX_RESOLUTION_TABLE_TRANSLATE",
	[55] = "K_MAIN_CONNECT",
	[56] = "K_HS_OBJECT_ORIENT",
	[57] = "K_OBJECT_HEADER_BLOCK_ALLOCATE",
	[58] = "K_OBJECT_ITERATOR_NEXT",
	[59] = "K_OBJECT_PLACEMENT_DATA_NEW",
	[60] = "K_OBJECT_NEW",
	[61] = "K_OBJECT_NEW_WITH_ROLE",
	[62] = "K_OBJECT_DELETE_TO_NETWORK",
	[63] = "K_OBJECT_DELETE",
	[64] = "K_OBJECT_ATTACH_TO_MARKER",
	[65] = "K_OBJECT_DETACH",
	[66] = "K_OBJECTS_FIND_IN_SPHERE",
	[67] = "K_OBJECT_START_INTERPOLATION",
	[68] = "K_OBJECT_RESET",
	[69] = "K_OBJECT_RECONNECT_TO_MAP",
	[70] = "K_OBJECT_DISCONNECT_FROM_MAP",
	[71] = "K_OBJECT_GET_ORIGIN",
	[72] = "K_OBJECT_GET_ORIENTATION",
	[73] = "K_OBJECT_GET_LOCATION",
	[74] = "K_OBJECT_SET_POSITION",
	[75] = "K_OBJECT_SET_POSITION_NETWORK",
	[76] = "K_OBJECT_RESTORE_BODY",
	[77] = "K_OBJECT_DEPLETE_BODY",
	[78] = "K_OBJECT_DEPLETE_SHIELD",
	[79] = "K_OBJECT_DOUBLE_CHARGE_SHIELD",
	[80] = "K_OBJECT_CAUSE_DAMAGE",
	[81] = "K_OBJECT_DEFINITION_PREDICT",
	[82] = "K_OBJECT_SET_SCALE",
	[83] = "K_OBJECT_GET_LEVEL_OF_DETAIL_PIXELS",
	[84] = "K_OBJECT_RENDER_STATE_REFRESH",
	[85] = "K_UNIT_GET_CAMERA_POSITION",
	[86] = "K_UNIT_SET_ANIMATION",
	[87] = "K_UNIT_GET_CUSTOM_ANIMATION_TIME",
	[88] = "K_UNIT_CAN_ENTER_SEAT",
	[89] = "K_UNIT_ENTER_SEAT",
	[90] = "K_UNIT_EXIT_VEHICLE",
	[91] = "K_UNIT_OPEN",
	[92] = "K_UNIT_CLOSE",
	[93] = "K_UNIT_FIND_NEARBY_SEAT",
	[94] = "K_UNIT_EXIT_SEAT_END",
	[95] = "K_COLLISION_TEST_VECTOR",
	[96] = "K_PLAYER_INDEX_FROM_UNIT_INDEX",
	[97] = "K_PLAYER_TELEPORT",
	[98] = "K_PLAYER_OVER_SHIELD_SCREEN_EFFECT",
	[99] = "K_PLAYER_CAMO_SCREEN_EFFECT",
	[100] = "K_PLAYER_HEALTH_PACK_SCREEN_EFFECT",
	[101] = "K_FIND_CLOSEST_PLAYER_INDEX",
	[102] = "K_PLAYER_EXAMINE_NEARBY_VEHICLE",
	[103] = "K_PLAYER_SET_ACTION_RESULT",
	[104] = "K_PLAYER_SET_ACTION_RESULT_TO_NETWORK",
	[105] = "K_RENDER_FRAME",
	[106] = "K_RENDER_WINDOW",
	[107] = "K_SCENARIO_TRIGGER_VOLUME_TEST_POINT",
	[108] = "K_TAG_LOADED",
	[109] = "K_TAG_ITERATOR_NEXT",
	[110] = "K_UNICODE_STRING_LIST_GET_STRING",
}