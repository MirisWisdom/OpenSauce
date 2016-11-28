/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once
#if PLATFORM_TYPE == PLATFORM_TOOL

namespace Yelo { namespace Tool
{
	bool IsBuildingYeloMap();

	void PLATFORM_API build_cache_file_for_scenario_extended(char* arguments[]);
}; };
#endif