--	Yelo: Open Sauce SDK
--		Halo 1 (CE) Edition

--	See license\OpenSauce\Halo1_CE for specific license information

HaloCEServer_Game_EngineFunctions =
{
	["K_GATHER_EXCEPTION_DATA"] =                        0x626524,
	["K_GATHER_EXCEPTION"] =                             0x52AEB0,
	["K_ANIMATION_CHOOSE_RANDOM_PERMUTATION_INTERNAL"] = 0x4C1CF0,
	["K_GSMD5DIGEST"] =                                  0x52F7E0,
	["K_SHELL_GET_COMMAND_LINE_ARGUMENT"] =              0x4FF900,

--////////////////////////////////////////////////////////////////////////
-- AI
	["K_ACTOR_DELETE"] =                                 0x427E70,
	["K_ACTOR_UPDATE"] =                                 0x429170,
	["K_ACTOR_CUSTOMIZE_UNIT"] =                         0x426D00,
	["K_ACTOR_SET_ACTIVE"] =                             0x4277D0,
	["K_ACTOR_SET_DORMANT"] =                            0x427870,
	["K_ACTOR_DELETE_PROPS"] =                           0x427E10,
	["K_ACTOR_FREEZE"] =                                 0x429010,
	["K_ACTOR_CREATE_FOR_UNIT"] =                        0x426AD0,
	["K_ACTOR_ACTION_CHANGE"] =                          0x40D8E0,
	["K_AI_SCRIPTING_ATTACH_FREE"] =                     0x435430,

--////////////////////////////////////////////////////////////////////////
-- Cache
	["K_CACHE_FILE_READ_REQUEST"] =                      0x4440A0 + 0x10,

--////////////////////////////////////////////////////////////////////////
-- Console
	["K_CONSOLE_PROCESS_COMMAND"] =                      0x4B3990,
	["K_CONSOLE_PRINTF"] =                               0x48ED70,
	["K_CONSOLE_RESPONSE_PRINTF"] =                      0x4B3790,
	["K_CONSOLE_WARNING"] =                              0x4B38F0,

--////////////////////////////////////////////////////////////////////////
-- Cheats
	["K_CHEAT_ALL_WEAPONS"] =                            0x457B10,
	["K_CHEAT_SPAWN_WARTHOG"] =                          0x457BA0,
	["K_CHEAT_TELEPORT_TO_CAMERA"] =                     0x457C10,
	["K_CHEAT_ACTIVE_CAMOFLAGE"] =                       0x457CA0,
	["K_CHEAT_ACTIVE_CAMOFLAGE_LOCAL_PLAYER"] =          0x457D00,
	["K_CHEAT_PLAYER_INDEX"] =                           0x457D80,

--////////////////////////////////////////////////////////////////////////
-- Effects
	["K_EFFECT_NEW_ON_OBJECT_MARKER"] =                  0x482F20,

--////////////////////////////////////////////////////////////////////////
-- Game
	["K_SCENARIO_SWITCH_STRUCTURE_BSP"] =                0x4FD320,
	["K_GAME_TEAM_IS_ENEMY"] =                           0x459280,
	["K_GAME_ENGINE_PLAY_MULTIPLAYER_SOUND"] =           0x4665D0,

--////////////////////////////////////////////////////////////////////////
-- HS
	["K_OBJECT_LIST_ADD"] =                              0x4858E0,

--////////////////////////////////////////////////////////////////////////
-- Interface
	["K_HUD_PRINT_MESSAGE"] =                            0x4A0240,
	["K_HUD_GET_ITEM_MESSAGE"] =                         0x49F340,

--////////////////////////////////////////////////////////////////////////
-- Items
	["K_WEAPON_PREVENTS_MELEE_ATTACK"] =                 0x4AFF40,
	["K_WEAPON_PREVENTS_GRENADE_THROWING"] =             0x4AFF90,
	["K_WEAPON_STOP_RELOAD"] =                           0x4B1BB0,
	["K_FIRST_PERSON_WEAPON_MESSAGE_FROM_UNIT"] =        0x48C330,
	["K_WEAPON_GET_FIRST_PERSON_ANIMATION_TIME"] =       0x4AFFE0,

--////////////////////////////////////////////////////////////////////////
-- Math
	["K_PERIODIC_FUNCTION_EVALUATE"] =                   0x4B8F60,
	["K_TRANSITION_FUNCTION_EVALUATE"] =                 0x4B9070,

--////////////////////////////////////////////////////////////////////////
-- Memory
	["K_DATA_NEW"] =                                     0x4BC7F0 + 0x30,
	["K_DATUM_NEW_AT_INDEX"] =                           0x4BC880,
	["K_DATUM_NEW"] =                                    0x4BC930,
	["K_DATUM_DELETE"] =                                 0x4BC9C0,
	["K_DATA_DELETE_ALL"] =                              0x4BCA00 + 0x30,
	["K_DATA_ITERATOR_NEXT"] =                           0x4BCA80,
	["K_DATA_NEXT_INDEX"] =                              0x4BCAE0,
	["K_DATUM_TRY_AND_GET"] =                            0x4BCB30,
	["K_DATUM_INITIALIZE"] =                             0x4BCB70,

--////////////////////////////////////////////////////////////////////////
-- Models
	["K_MODEL_FIND_MARKER"] =                            0x4C2950,

--////////////////////////////////////////////////////////////////////////
-- Networking
	["K_HUD_CHAT_TO_NETWORK"] =                          0x49F740,
	["K_INDEX_RESOLUTION_TABLE_TRANSLATE"] =             0x4DD4D0,
	["K_MAIN_CONNECT"] =                                 0x4B4D50 + 0x30,

--////////////////////////////////////////////////////////////////////////
-- Objects
	["K_HS_OBJECT_ORIENT"] =                             0x482580,
	["K_OBJECT_HEADER_BLOCK_ALLOCATE"] =                 0x4E2A30,
	["K_OBJECT_ITERATOR_NEXT"] =                         0x4E1AD0 + 0x30,
	["K_OBJECT_PLACEMENT_DATA_NEW"] =                    0x4E0020,
	["K_OBJECT_NEW"] =                                   0x4E00E0,
	["K_OBJECT_NEW_WITH_ROLE"] =                         0x4E0130,
	["K_OBJECT_DELETE_TO_NETWORK"] =                     0x4E0720,
	["K_OBJECT_DELETE"] =                                0x4E0850,
	["K_OBJECT_ATTACH_TO_MARKER"] =                      0x4E0D60,
	["K_OBJECT_DETACH"] =                                0x4E11F0,
	["K_OBJECTS_FIND_IN_SPHERE"] =                       0x4E1BC0,
	["K_OBJECT_START_INTERPOLATION"] =                   0x4E1750,
	["K_OBJECT_RESET"] =                                 0x4DFDE0,
	["K_OBJECT_RECONNECT_TO_MAP"] =                      0x4E08B0,
	["K_OBJECT_DISCONNECT_FROM_MAP"] =                   0x4E0A60,
	["K_OBJECT_GET_ORIGIN"] =                            0x4E14E0,
	["K_OBJECT_GET_ORIENTATION"] =                       0x4E1550,
	["K_OBJECT_GET_LOCATION"] =                          0x4E16F0,
	["K_OBJECT_SET_POSITION"] =                          0x4DFE40,
	["K_OBJECT_SET_POSITION_NETWORK"] =                  0x4DFF40,
	["K_OBJECT_RESTORE_BODY"] =                          0x4DA8C0,
	["K_OBJECT_DEPLETE_BODY"] =                          0x4DA910,
	["K_OBJECT_DEPLETE_SHIELD"] =                        0x4DAA00,
	["K_OBJECT_DOUBLE_CHARGE_SHIELD"] =                  0x4DAA90,
	["K_OBJECT_CAUSE_DAMAGE"] =                          0x4DB4D0,
	["K_OBJECT_DEFINITION_PREDICT"] =                    0x4E26B0,
	["K_OBJECT_SET_SCALE"] =                             0x4E4280,
	["K_OBJECTS_UPDATE"] =                               0x4DFB10,
	["K_OBJECT_UPDATE"] =                                0x4FBB80,
	["K_OBJECT_GET_MARKER_BY_NAME"] =                    0x4E0C60,
	["K_OBJECT_DESTROY"] =                               0x4DAB70,

--////////////////////////////////////////////////////////////////////////
-- Units
	["K_UNIT_UPDATE"] =                                  0x514A40,
	["K_UNIT_SET_ANIMATION"] =                           0x521010,
	["K_UNIT_ANIMATION_START_ACTION"] =                  0x518290,
	["K_UNIT_GET_CUSTOM_ANIMATION_TIME"] =               0x522600,
	["K_UNIT_CAN_ENTER_SEAT"] =                          0x518CD0,
	["K_UNIT_ENTER_SEAT"] =                              0x518E00,
	["K_UNIT_EXIT_VEHICLE"] =                            0x51DA80,
	["K_UNIT_TRY_AND_EXIT_SEAT"] =                       0x51E900,
	["K_UNIT_OPEN"] =                                    0x51CFA0,
	["K_UNIT_CLOSE"] =                                   0x51CFC0,
	["K_UNIT_FIND_NEARBY_SEAT"] =                        0x5189F0,
	["K_UNIT_EXIT_SEAT_END"] =                           0x51EAD0,
	["K_UNIT_CAN_SEE_POINT"] =                           0x51E590,
	["K_UNIT_DETACH_FROM_PARENT"] =                      0x51EED0,
	["K_UNIT_START_USER_ANIMATION"] =                    0x5226F0,
	["K_UNIT_DROP_CURRENT_WEAPON"] =                     0x520350,
	["K_UNIT_DAMAGE_AFTERMATH"] =                        0x519930,
	["K_UNIT_SCRIPTING_SET_CURRENT_VITALITY"] =          0x514010,
	["K_UNIT_ANIMATION_SET_STATE"] =                     0x518420,
	["K_UNIT_INVENTORY_GET_WEAPON"] =                    0x51BE00,
	["K_UNIT_THROW_GRENADE_RELEASE"] =                   0x520880,
	["K_UNIT_CAUSE_PLAYER_MELEE_DAMAGE"] =               0x521990,
	["K_UNIT_SET_ACTIVELY_CONTROLLED"] =                 0x51C080,
	["K_UNIT_READY_DESIRED_WEAPON"] =                    0x51FB70,

--////////////////////////////////////////////////////////////////////////
-- Physics
	["K_COLLISION_TEST_VECTOR"] =                        0x4EF5D0 + 0x30,

--////////////////////////////////////////////////////////////////////////
-- Players
	["K_PLAYER_INDEX_FROM_UNIT_INDEX"] =                 0x46F670,
	["K_PLAYER_TELEPORT"] =                              0x470510,
	["K_PLAYER_EXAMINE_NEARBY_VEHICLE"] =                0x472E20,
	["K_PLAYER_SET_ACTION_RESULT"] =                     0x473620,
	["K_PLAYER_SET_ACTION_RESULT_TO_NETWORK"] =          0x473810,

--////////////////////////////////////////////////////////////////////////
-- Scenario
	["K_SCENARIO_TRIGGER_VOLUME_TEST_POINT"] =           0x4FD490,

--////////////////////////////////////////////////////////////////////////
-- TagGroups
	["K_TAG_LOADED"] =                                   0x443530,
	["K_TAG_ITERATOR_NEXT"] =                            0x4435B0,
	["K_UNICODE_STRING_LIST_GET_STRING"] =               0x509DE0,
}

HaloCEServer_Game_EngineFunctions_Index =
{
	[1] = "K_GATHER_EXCEPTION_DATA",
	[2] = "K_GATHER_EXCEPTION",
	[3] = "K_ANIMATION_CHOOSE_RANDOM_PERMUTATION_INTERNAL",
	[4] = "K_GSMD5DIGEST",
	[5] = "K_SHELL_GET_COMMAND_LINE_ARGUMENT",
	[6] = "K_ACTOR_DELETE",
	[7] = "K_ACTOR_UPDATE",
	[8] = "K_ACTOR_CUSTOMIZE_UNIT",
	[9] = "K_ACTOR_SET_ACTIVE",
	[10] = "K_ACTOR_SET_DORMANT",
	[11] = "K_ACTOR_DELETE_PROPS",
	[12] = "K_ACTOR_FREEZE",
	[13] = "K_ACTOR_CREATE_FOR_UNIT",
	[14] = "K_ACTOR_ACTION_CHANGE",
	[15] = "K_AI_SCRIPTING_ATTACH_FREE",
	[16] = "K_CACHE_FILE_READ_REQUEST",
	[17] = "K_CONSOLE_PROCESS_COMMAND",
	[18] = "K_CONSOLE_PRINTF",
	[19] = "K_CONSOLE_RESPONSE_PRINTF",
	[20] = "K_CONSOLE_WARNING",
	[21] = "K_CHEAT_ALL_WEAPONS",
	[22] = "K_CHEAT_SPAWN_WARTHOG",
	[23] = "K_CHEAT_TELEPORT_TO_CAMERA",
	[24] = "K_CHEAT_ACTIVE_CAMOFLAGE",
	[25] = "K_CHEAT_ACTIVE_CAMOFLAGE_LOCAL_PLAYER",
	[26] = "K_CHEAT_PLAYER_INDEX",
	[27] = "K_EFFECT_NEW_ON_OBJECT_MARKER",
	[28] = "K_SCENARIO_SWITCH_STRUCTURE_BSP",
	[29] = "K_GAME_TEAM_IS_ENEMY",
	[30] = "K_GAME_ENGINE_PLAY_MULTIPLAYER_SOUND",
	[31] = "K_OBJECT_LIST_ADD",
	[32] = "K_HUD_PRINT_MESSAGE",
	[33] = "K_HUD_GET_ITEM_MESSAGE",
	[34] = "K_WEAPON_PREVENTS_MELEE_ATTACK",
	[35] = "K_WEAPON_PREVENTS_GRENADE_THROWING",
	[36] = "K_WEAPON_STOP_RELOAD",
	[37] = "K_FIRST_PERSON_WEAPON_MESSAGE_FROM_UNIT",
	[38] = "K_WEAPON_GET_FIRST_PERSON_ANIMATION_TIME",
	[39] = "K_PERIODIC_FUNCTION_EVALUATE",
	[40] = "K_TRANSITION_FUNCTION_EVALUATE",
	[41] = "K_DATA_NEW",
	[42] = "K_DATUM_NEW_AT_INDEX",
	[43] = "K_DATUM_NEW",
	[44] = "K_DATUM_DELETE",
	[45] = "K_DATA_DELETE_ALL",
	[46] = "K_DATA_ITERATOR_NEXT",
	[47] = "K_DATA_NEXT_INDEX",
	[48] = "K_DATUM_TRY_AND_GET",
	[49] = "K_DATUM_INITIALIZE",
	[50] = "K_MODEL_FIND_MARKER",
	[51] = "K_HUD_CHAT_TO_NETWORK",
	[52] = "K_INDEX_RESOLUTION_TABLE_TRANSLATE",
	[53] = "K_MAIN_CONNECT",
	[54] = "K_HS_OBJECT_ORIENT",
	[55] = "K_OBJECT_HEADER_BLOCK_ALLOCATE",
	[56] = "K_OBJECT_ITERATOR_NEXT",
	[57] = "K_OBJECT_PLACEMENT_DATA_NEW",
	[58] = "K_OBJECT_NEW",
	[59] = "K_OBJECT_NEW_WITH_ROLE",
	[60] = "K_OBJECT_DELETE_TO_NETWORK",
	[61] = "K_OBJECT_DELETE",
	[62] = "K_OBJECT_ATTACH_TO_MARKER",
	[63] = "K_OBJECT_DETACH",
	[64] = "K_OBJECTS_FIND_IN_SPHERE",
	[65] = "K_OBJECT_START_INTERPOLATION",
	[66] = "K_OBJECT_RESET",
	[67] = "K_OBJECT_RECONNECT_TO_MAP",
	[68] = "K_OBJECT_DISCONNECT_FROM_MAP",
	[69] = "K_OBJECT_GET_ORIGIN",
	[70] = "K_OBJECT_GET_ORIENTATION",
	[71] = "K_OBJECT_GET_LOCATION",
	[72] = "K_OBJECT_SET_POSITION",
	[73] = "K_OBJECT_SET_POSITION_NETWORK",
	[74] = "K_OBJECT_RESTORE_BODY",
	[75] = "K_OBJECT_DEPLETE_BODY",
	[76] = "K_OBJECT_DEPLETE_SHIELD",
	[77] = "K_OBJECT_DOUBLE_CHARGE_SHIELD",
	[78] = "K_OBJECT_CAUSE_DAMAGE",
	[79] = "K_OBJECT_DEFINITION_PREDICT",
	[80] = "K_OBJECT_SET_SCALE",
	[81] = "K_OBJECTS_UPDATE",
	[82] = "K_OBJECT_UPDATE",
	[83] = "K_OBJECT_GET_MARKER_BY_NAME",
	[84] = "K_OBJECT_DESTROY",
	[85] = "K_UNIT_UPDATE",
	[86] = "K_UNIT_SET_ANIMATION",
	[87] = "K_UNIT_ANIMATION_START_ACTION",
	[88] = "K_UNIT_GET_CUSTOM_ANIMATION_TIME",
	[89] = "K_UNIT_CAN_ENTER_SEAT",
	[90] = "K_UNIT_ENTER_SEAT",
	[91] = "K_UNIT_EXIT_VEHICLE",
	[92] = "K_UNIT_TRY_AND_EXIT_SEAT",
	[93] = "K_UNIT_OPEN",
	[94] = "K_UNIT_CLOSE",
	[95] = "K_UNIT_FIND_NEARBY_SEAT",
	[96] = "K_UNIT_EXIT_SEAT_END",
	[97] = "K_UNIT_CAN_SEE_POINT",
	[98] = "K_UNIT_DETACH_FROM_PARENT",
	[99] = "K_UNIT_START_USER_ANIMATION",
	[100] = "K_UNIT_DROP_CURRENT_WEAPON",
	[101] = "K_UNIT_DAMAGE_AFTERMATH",
	[102] = "K_UNIT_SCRIPTING_SET_CURRENT_VITALITY",
	[103] = "K_UNIT_ANIMATION_SET_STATE",
	[104] = "K_UNIT_INVENTORY_GET_WEAPON",
	[105] = "K_UNIT_THROW_GRENADE_RELEASE",
	[106] = "K_UNIT_CAUSE_PLAYER_MELEE_DAMAGE",
	[107] = "K_UNIT_SET_ACTIVELY_CONTROLLED",
	[108] = "K_UNIT_READY_DESIRED_WEAPON",
	[109] = "K_COLLISION_TEST_VECTOR",
	[110] = "K_PLAYER_INDEX_FROM_UNIT_INDEX",
	[111] = "K_PLAYER_TELEPORT",
	[112] = "K_PLAYER_EXAMINE_NEARBY_VEHICLE",
	[113] = "K_PLAYER_SET_ACTION_RESULT",
	[114] = "K_PLAYER_SET_ACTION_RESULT_TO_NETWORK",
	[115] = "K_SCENARIO_TRIGGER_VOLUME_TEST_POINT",
	[116] = "K_TAG_LOADED",
	[117] = "K_TAG_ITERATOR_NEXT",
	[118] = "K_UNICODE_STRING_LIST_GET_STRING",
}