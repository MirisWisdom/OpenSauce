--	Yelo: Open Sauce SDK
--		Halo 1 (CE) Edition

--	See license\OpenSauce\Halo1_CE for specific license information

HaloCEServer_OpenSauce =
{
--////////////////////////////////////////////////////////////////////////
-- Common/GameSystems.cpp
	["K_QUERY_EXITFLAG_REG_CALL"] =        0x4FF3D6,
	["K_QUERY_EXITFLAG_REG"] =             0x52ACF0,
	["K_RELEASE_RESOURCES_ON_EXIT_CALL"] = 0x4FF5B7,
	["K_RELEASE_RESOURCES_ON_EXIT"] =      0x4FF100,
	
--////////////////////////////////////////////////////////////////////////
-- Common/DebugDump.cpp
	["K_WINMAIN_EXCEPTION_FILTER"] =       0x4FFEA0,
	["K_WINMAIN_EXCEPTION_FILTER_CALL"] =  0x4FF60F,
	
	["K_SOUND_STOP_ALL"] =                 0x503BC0,
	["K_VIRTUALPROTECT_LOCK"] =            0x625884,
	["K_VIRTUALPROTECT_OLD_PROTECT"] =     0x625888,
}

HaloCEServer_OpenSauce_Index =
{
	[1] = "K_QUERY_EXITFLAG_REG_CALL",
	[2] = "K_QUERY_EXITFLAG_REG",
	[3] = "K_RELEASE_RESOURCES_ON_EXIT_CALL",
	[4] = "K_RELEASE_RESOURCES_ON_EXIT",
	[5] = "K_WINMAIN_EXCEPTION_FILTER",
	[6] = "K_WINMAIN_EXCEPTION_FILTER_CALL",
	[7] = "K_SOUND_STOP_ALL",
	[8] = "K_VIRTUALPROTECT_LOCK",
	[9] = "K_VIRTUALPROTECT_OLD_PROTECT",
}