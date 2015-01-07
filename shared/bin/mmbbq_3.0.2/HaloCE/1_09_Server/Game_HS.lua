--	Yelo: Open Sauce SDK
--		Halo 1 (CE) Edition

--	See license\OpenSauce\Halo1_CE for specific license information

HaloCEServer_Game_HS =
{
--////////////////////////////////////////////////////////////////////////
-- Scripting.cpp
	["K_RECORDED_ANIMATIONS"] =                                     0x5BD740,
	["K_HS_SYNTAX"] =                                               0x6E1474,
	["K_OBJECT_LIST_HEADER"] =                                      0x6E1464,
	["K_LIST_OBJECT_REFERENCE"] =                                   0x6E1468,
	["K_HS_GLOBALS"] =                                              0x6E146C,
	["K_HS_THREADS"] =                                              0x6E1470,

	["K_HS_UPDATE_HOOK"] =                                          0x484878,
	
	["K_MAX_HS_SYNTAX_NODES_PER_SCENARIO_UPGRADE_ADDRESS_LIST_0"] = 0x47D78B,
	
	["K_TOTAL_SCENARIO_HS_SYNTAX_DATA_UPGRADE_ADDRESS_LIST_0"] =    0x47D7DA,

	["K_ADDRESS_OF_SCENARIO_HS_SYNTAX_DATA_SIZE_CHECK"] =           0x47D787,
	
--////////////////////////////////////////////////////////////////////////
-- ScriptLibrary.cpp
	["K_HS_MACRO_FUNCTION_PARSE"] =                                 0x481A70,
	["K_HS_COMPILE_AND_EVALUATE"] =                                 0x47EA30,
	["K_HS_NULL_EVALUATE"] =                                        0x476140,
	["K_HS_NULL_WITH_PARAMS_EVALUATE"] =                            0x47B8B0,
	
--////////////////////////////////////////////////////////////////////////
-- script functions related
	["K_HS_FUNCTION_TABLE_COUNT"] =                                 0x555690,
	["K_HS_FUNCTION_TABLE"] =                                       0x599AD8,
	
	["K_HS_FUNCTION_TABLE_COUNT_REFERENCE_16BIT_0"] =               0x47DBF1,
	["K_HS_FUNCTION_TABLE_COUNT_REFERENCE_16BIT_1"] =               0x47E914,
	
	["K_HS_FUNCTION_TABLE_COUNT_REFERENCE_32BIT_0"] =               0x47DEFA,
	
--////////////////////////////////////////////////////////////////////////
-- script globals related
	["K_HS_EXTERNAL_GLOBALS_COUNT"] =                               0x55578C,
	["K_HS_EXTERNAL_GLOBALS"] =                                     0x59C328,
	
	["K_HS_EXTERNAL_GLOBALS_COUNT_REFERENCE_16BIT_0"] =             0x47DB01,
	
	["K_HS_EXTERNAL_GLOBALS_COUNT_REFERENCE_32BIT_0"] =             0x47DFAA,
	["K_HS_EXTERNAL_GLOBALS_COUNT_REFERENCE_32BIT_1"] =             0x4836DA,
	["K_HS_EXTERNAL_GLOBALS_COUNT_REFERENCE_32BIT_2"] =             0x4844FB,
	["K_HS_EXTERNAL_GLOBALS_COUNT_REFERENCE_32BIT_3"] =             0x48460F,
	["K_HS_EXTERNAL_GLOBALS_COUNT_REFERENCE_32BIT_4"] =             0x48466D,
	["K_HS_EXTERNAL_GLOBALS_COUNT_REFERENCE_32BIT_5"] =             0x484770,
	["K_HS_EXTERNAL_GLOBALS_COUNT_REFERENCE_32BIT_6"] =             0x484D8A,
	
	["K_HS_EXTERNAL_GLOBALS_REFERENCE_0"] =                         0x47DA8C,
	["K_HS_EXTERNAL_GLOBALS_REFERENCE_1"] =                         0x47DABC, 
	["K_HS_EXTERNAL_GLOBALS_REFERENCE_2"] =                         0x47DAE9,
	["K_HS_EXTERNAL_GLOBALS_REFERENCE_3"] =                         0x47DE20, 
	["K_HS_EXTERNAL_GLOBALS_REFERENCE_4"] =                         0x47DFA5,
	["K_HS_EXTERNAL_GLOBALS_REFERENCE_5"] =                         0x480BE2, 
	["K_HS_EXTERNAL_GLOBALS_REFERENCE_6"] =                         0x48366E,
	["K_HS_EXTERNAL_GLOBALS_REFERENCE_7"] =                         0x484BE6, 
	["K_HS_EXTERNAL_GLOBALS_REFERENCE_8"] =                         0x48551B,
	["K_HS_EXTERNAL_GLOBALS_REFERENCE_9"] =                         0x485687,
	
	["K_HS_VALID_ACCESS_FLAGS"] =                                   0x47DC30,
	
--////////////////////////////////////////////////////////////////////////
-- InitializeCreateScriptFunction()
	["K_HS_ARGUMENTS_EVALUATE"] =                                   0x484E80,
	["K_HS_RETURN"] =                                               0x484C70,
}

HaloCEServer_Game_HS_Index =
{
	[1] = "K_RECORDED_ANIMATIONS",
	[2] = "K_HS_SYNTAX",
	[3] = "K_OBJECT_LIST_HEADER",
	[4] = "K_LIST_OBJECT_REFERENCE",
	[5] = "K_HS_GLOBALS",
	[6] = "K_HS_THREADS",
	[7] = "K_HS_UPDATE_HOOK",
	[8] = "K_MAX_HS_SYNTAX_NODES_PER_SCENARIO_UPGRADE_ADDRESS_LIST_0",
	[9] = "K_TOTAL_SCENARIO_HS_SYNTAX_DATA_UPGRADE_ADDRESS_LIST_0",
	[10] = "K_ADDRESS_OF_SCENARIO_HS_SYNTAX_DATA_SIZE_CHECK",
	[11] = "K_HS_MACRO_FUNCTION_PARSE",
	[12] = "K_HS_COMPILE_AND_EVALUATE",
	[13] = "K_HS_NULL_EVALUATE",
	[14] = "K_HS_NULL_WITH_PARAMS_EVALUATE",
	[15] = "K_HS_FUNCTION_TABLE_COUNT",
	[16] = "K_HS_FUNCTION_TABLE",
	[17] = "K_HS_FUNCTION_TABLE_COUNT_REFERENCE_16BIT_0",
	[18] = "K_HS_FUNCTION_TABLE_COUNT_REFERENCE_16BIT_1",
	[19] = "K_HS_FUNCTION_TABLE_COUNT_REFERENCE_32BIT_0",
	[20] = "K_HS_EXTERNAL_GLOBALS_COUNT",
	[21] = "K_HS_EXTERNAL_GLOBALS",
	[22] = "K_HS_EXTERNAL_GLOBALS_COUNT_REFERENCE_16BIT_0",
	[23] = "K_HS_EXTERNAL_GLOBALS_COUNT_REFERENCE_32BIT_0",
	[24] = "K_HS_EXTERNAL_GLOBALS_COUNT_REFERENCE_32BIT_1",
	[25] = "K_HS_EXTERNAL_GLOBALS_COUNT_REFERENCE_32BIT_2",
	[26] = "K_HS_EXTERNAL_GLOBALS_COUNT_REFERENCE_32BIT_3",
	[27] = "K_HS_EXTERNAL_GLOBALS_COUNT_REFERENCE_32BIT_4",
	[28] = "K_HS_EXTERNAL_GLOBALS_COUNT_REFERENCE_32BIT_5",
	[29] = "K_HS_EXTERNAL_GLOBALS_COUNT_REFERENCE_32BIT_6",
	[30] = "K_HS_EXTERNAL_GLOBALS_REFERENCE_0",
	[31] = "K_HS_EXTERNAL_GLOBALS_REFERENCE_1",
	[32] = "K_HS_EXTERNAL_GLOBALS_REFERENCE_2",
	[33] = "K_HS_EXTERNAL_GLOBALS_REFERENCE_3",
	[34] = "K_HS_EXTERNAL_GLOBALS_REFERENCE_4",
	[35] = "K_HS_EXTERNAL_GLOBALS_REFERENCE_5",
	[36] = "K_HS_EXTERNAL_GLOBALS_REFERENCE_6",
	[37] = "K_HS_EXTERNAL_GLOBALS_REFERENCE_7",
	[38] = "K_HS_EXTERNAL_GLOBALS_REFERENCE_8",
	[39] = "K_HS_EXTERNAL_GLOBALS_REFERENCE_9",
	[40] = "K_HS_VALID_ACCESS_FLAGS",
	[41] = "K_HS_ARGUMENTS_EVALUATE",
	[42] = "K_HS_RETURN",
}