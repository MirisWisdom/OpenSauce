--	Yelo: Open Sauce SDK
--		Halo 1 (CE) Edition

--	See license\OpenSauce\Halo1_CE for specific license information

HaloCEClient_Game_HS =
{
	--////////////////////////////////////////////////////////////////////////
	-- Scripting.cpp
	["K_RECORDED_ANIMATIONS"] =                                     0x64B960,
	["K_HS_SYNTAX"] =                                               0x8155B4,
	["K_OBJECT_LIST_HEADER"] =                                      0x8155A4,
	["K_LIST_OBJECT_REFERENCE"] =                                   0x8155A8,
	["K_HS_GLOBALS"] =                                              0x8155AC,
	["K_HS_THREADS"] =                                              0x8155B0,

	["K_HS_UPDATE_HOOK"] =                                          0x48CF98,

	["K_MAX_HS_SYNTAX_NODES_PER_SCENARIO_UPGRADE_ADDRESS_LIST_0"] = 0x485E9B,

	["K_TOTAL_SCENARIO_HS_SYNTAX_DATA_UPGRADE_ADDRESS_LIST_0"] =    0x485EEA,

	["K_ADDRESS_OF_SCENARIO_HS_SYNTAX_DATA_SIZE_CHECK"] =           0x485E97,


	--////////////////////////////////////////////////////////////////////////
	-- ScriptLibrary.cpp
	["K_HS_MACRO_FUNCTION_PARSE"] =                                 0x48A220,
	["K_HS_COMPILE_AND_EVALUATE"] =                                 0x487150,
	["K_HS_NULL_EVALUATE"] =                                        0x4824E0,
	["K_HS_NULL_WITH_PARAMS_EVALUATE"] =                            0x480110,

	--////////////////////////////////////////////////////////////////////////
	-- script functions related
	["K_HS_FUNCTION_TABLE_COUNT_PTR"] =                             0x5F9C10,
	["K_HS_FUNCTION_TABLE"] =                                       0x624118,

	["K_HS_FUNCTION_TABLE_COUNT_REFERENCE_16BIT_0"] =               0x486301,
	["K_HS_FUNCTION_TABLE_COUNT_REFERENCE_16BIT_1"] =               0x487034,

	["K_HS_FUNCTION_TABLE_COUNT_REFERENCE_32BIT_0"] =               0x48661A,

	--////////////////////////////////////////////////////////////////////////
	-- script globals related
	["K_HS_EXTERNAL_GLOBALS_COUNT_PTR"] =                           0x5F9D0C,
	["K_HS_EXTERNAL_GLOBALS"] =                                     0x626988,

	["K_HS_EXTERNAL_GLOBALS_COUNT_REFERENCE_16BIT_0"] =             0x486211,

	["K_HS_EXTERNAL_GLOBALS_COUNT_REFERENCE_32BIT_0"] =             0x4866CA,
	["K_HS_EXTERNAL_GLOBALS_COUNT_REFERENCE_32BIT_1"] =             0x48BDFA,
	["K_HS_EXTERNAL_GLOBALS_COUNT_REFERENCE_32BIT_2"] =             0x48CC1B,
	["K_HS_EXTERNAL_GLOBALS_COUNT_REFERENCE_32BIT_3"] =             0x48CD2F,
	["K_HS_EXTERNAL_GLOBALS_COUNT_REFERENCE_32BIT_4"] =             0x48CD8D,
	["K_HS_EXTERNAL_GLOBALS_COUNT_REFERENCE_32BIT_5"] =             0x48CE90,
	["K_HS_EXTERNAL_GLOBALS_COUNT_REFERENCE_32BIT_6"] =             0x48D4AA,

	["K_HS_EXTERNAL_GLOBALS_REFERENCE_0"] =                         0x48619C,
	["K_HS_EXTERNAL_GLOBALS_REFERENCE_1"] =                         0x4861CC,
	["K_HS_EXTERNAL_GLOBALS_REFERENCE_2"] =                         0x4861F9,
	["K_HS_EXTERNAL_GLOBALS_REFERENCE_3"] =                         0x486530,
	["K_HS_EXTERNAL_GLOBALS_REFERENCE_4"] =                         0x4866C5,
	["K_HS_EXTERNAL_GLOBALS_REFERENCE_5"] =                         0x489392,
	["K_HS_EXTERNAL_GLOBALS_REFERENCE_6"] =                         0x48BD8E,
	["K_HS_EXTERNAL_GLOBALS_REFERENCE_7"] =                         0x48D306,
	["K_HS_EXTERNAL_GLOBALS_REFERENCE_8"] =                         0x48DC3B,
	["K_HS_EXTERNAL_GLOBALS_REFERENCE_9"] =                         0x48DDA7,

	["K_HS_VALID_ACCESS_FLAGS"] =                                   0x486340,

	--////////////////////////////////////////////////////////////////////////
	-- InitializeCreateScriptFunction()
	["K_HS_ARGUMENTS_EVALUATE"] =                                   0x48D5A0,
	["K_HS_RETURN"] =                                               0x48D390
}

HaloCEClient_Game_HS_Index =
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
	[15] = "K_HS_FUNCTION_TABLE_COUNT_PTR",
	[16] = "K_HS_FUNCTION_TABLE",
	[17] = "K_HS_FUNCTION_TABLE_COUNT_REFERENCE_16BIT_0",
	[18] = "K_HS_FUNCTION_TABLE_COUNT_REFERENCE_16BIT_1",
	[19] = "K_HS_FUNCTION_TABLE_COUNT_REFERENCE_32BIT_0",
	[20] = "K_HS_EXTERNAL_GLOBALS_COUNT_PTR",
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