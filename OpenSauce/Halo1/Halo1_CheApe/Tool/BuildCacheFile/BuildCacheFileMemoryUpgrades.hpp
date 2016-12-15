/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Cache
	{
		struct s_cache_header_yelo;
	};

	namespace TagGroups
	{
		struct s_project_yellow_scenario_build_info;
	};
};

namespace Yelo { namespace Tool { namespace BuildCacheFileEx { namespace MemoryUpgrades {

	extern Cache::s_cache_header_yelo yelo_cache_header_globals;

	void InitializeHeaderGlobals(bool using_mod_sets, cstring mod_name, bool use_memory_upgrades);

	// Initializes the yelo header with the default build info settings
	void InitializeHeaderGlobalsBuildInfo();

	// Initializes the yelo header with a scenario's yelo build info
	void InitializeHeaderGlobalsBuildInfo(const TagGroups::s_project_yellow_scenario_build_info& build_info);
	
	void InitializeHeaderGlobalsMinimumBuild(const byte major, const byte minor, const uint16 build);

	void PrintScriptUpgradesUsage();

	void Initialize(bool only_using_data_file_hacks);
	void Dispose(bool only_using_data_file_hacks);

}; }; }; };