--	Yelo: Open Sauce SDK
--		Halo 1 (CE) Edition

--	See license\OpenSauce\Halo1_CE for specific license information

HaloCEClient_OpenSauce =
{
--////////////////////////////////////////////////////////////////////////
-- Memory/FunctionInterface.cpp
	["K_RENDER_SKY"] =                                              0x5148F0,
	["K_RENDER_WINDOW_CALL_HOOK_RENDER_SKY"] =                      0x50FD32,

	["K_RENDER_OBJECTS"] =                                          0x5125D0,
	["K_RENDER_WINDOW_CALL_HOOK_RENDER_OBJECTS"] =                  0x50FD41,

	["K_STRUCTURE_RENDER_LIGHTMAPS"] =                              0x556330,
	["K_RENDER_WINDOW_CALL_HOOK_STRUCTURE_RENDER_LIGHTMAPS"] =      0x50FD4B,

	["K_WEATHER_PARTICLE_SYSTEMS_RENDER"] =                         0x459620,
	["K_RENDER_WINDOW_CALL_HOOK_WEATHER_PARTICLE_SYSTEMS_RENDER"] = 0x5100CC,

	["K_RENDER_UI_WIDGETS"] =                                       0x49B620,
	["K_RENDER_WINDOW_CALL_HOOK_RENDER_UI_WIDGETS"] =               0x5101FD,

	["K_RENDER_UI_CURSOR"] =                                        0x49A4B0,
	["K_CALL_HOOK_RENDER_UI_CURSOR"] =                              0x49B6F0,

	["K_INTERFACE_DRAW_SCREEN"] =                                   0x4976C0,
	["K_RENDER_WINDOW_CALL_HOOK_INTERFACE_DRAW_SCREEN"] =           0x5101F1,

	["K_UPDATE_UI_WIDGETS"] =                                       0x49AFE0,
	["K_CALL_HOOK_UPDATE_UI_WIDGETS"] =                             0x4CB0D7,
	
--////////////////////////////////////////////////////////////////////////
-- Common/GameSystems.cpp
	["K_QUERY_EXITFLAG_REG_CALL"] =                                 0x5450A1,
	["K_QUERY_EXITFLAG_REG"] =                                      0x582560,
	["K_RELEASE_RESOURCES_ON_EXIT_CALL"] =                          0x5452C0,
	["K_RELEASE_RESOURCES_ON_EXIT"] =                               0x5447C0,
	
--////////////////////////////////////////////////////////////////////////
-- Common/DebugDump.cpp
	["K_WINMAIN_EXCEPTION_FILTER"] =                                0x546920,
	["K_WINMAIN_EXCEPTION_FILTER_CALL"] =                           0x545341,

	["K_RASTERIZER_DX9_SAVE_GAMMA"] =                               0x525E00,
	["K_RASTERIZER_WINDOWS_PRESENT_FRAME"] =                        0x51BDC0,
	["K_SOUND_STOP_ALL"] =                                          0x54E7D0,
	["K_VIRTUALPROTECT_LOCK"] =                                     0x6BD1F8,
	["K_VIRTUALPROTECT_OLD_PROTECT"] =                              0x6BD1FC
}

HaloCEClient_OpenSauce_Index =
{
	[1] = "K_RENDER_SKY",
	[2] = "K_RENDER_WINDOW_CALL_HOOK_RENDER_SKY",
	[3] = "K_RENDER_OBJECTS",
	[4] = "K_RENDER_WINDOW_CALL_HOOK_RENDER_OBJECTS",
	[5] = "K_STRUCTURE_RENDER_LIGHTMAPS",
	[6] = "K_RENDER_WINDOW_CALL_HOOK_STRUCTURE_RENDER_LIGHTMAPS",
	[7] = "K_WEATHER_PARTICLE_SYSTEMS_RENDER",
	[8] = "K_RENDER_WINDOW_CALL_HOOK_WEATHER_PARTICLE_SYSTEMS_RENDER",
	[9] = "K_RENDER_UI_WIDGETS",
	[10] = "K_RENDER_WINDOW_CALL_HOOK_RENDER_UI_WIDGETS",
	[11] = "K_RENDER_UI_CURSOR",
	[12] = "K_CALL_HOOK_RENDER_UI_CURSOR",
	[13] = "K_INTERFACE_DRAW_SCREEN",
	[14] = "K_RENDER_WINDOW_CALL_HOOK_INTERFACE_DRAW_SCREEN",
	[15] = "K_UPDATE_UI_WIDGETS",
	[16] = "K_CALL_HOOK_UPDATE_UI_WIDGETS",
	[17] = "K_QUERY_EXITFLAG_REG_CALL",
	[18] = "K_QUERY_EXITFLAG_REG",
	[19] = "K_RELEASE_RESOURCES_ON_EXIT_CALL",
	[20] = "K_RELEASE_RESOURCES_ON_EXIT",
	[21] = "K_WINMAIN_EXCEPTION_FILTER",
	[22] = "K_WINMAIN_EXCEPTION_FILTER_CALL",
	[23] = "K_RASTERIZER_DX9_SAVE_GAMMA",
	[24] = "K_RASTERIZER_WINDOWS_PRESENT_FRAME",
	[25] = "K_SOUND_STOP_ALL",
	[26] = "K_VIRTUALPROTECT_LOCK",
	[27] = "K_VIRTUALPROTECT_OLD_PROTECT",
}