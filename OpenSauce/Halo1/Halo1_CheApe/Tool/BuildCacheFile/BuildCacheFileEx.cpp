/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Tool/BuildCacheFile/BuildCacheFileEx.hpp"
#if PLATFORM_TYPE == PLATFORM_TOOL

#include "Tool/BuildCacheFile.hpp"

#include <YeloLib/Halo1/cache/cache_files_structures_yelo.hpp>
#include <YeloLib/Halo1/cache/data_file_yelo.hpp>
#include <YeloLib/cseries/value_conversion.hpp>

#include "Tool/BuildCacheFile/BuildCacheFileGlobals.hpp"
#include "Tool/BuildCacheFile/BuildCacheFileMemoryUpgrades.hpp"

#include "Tool/BuildCacheFile/CullTags.inl"
#include "Tool/BuildCacheFile/PredictedResources.inl"

namespace Yelo { namespace Tool { namespace BuildCacheFileEx
{
	void Initialize(bool only_using_data_file_hacks)
	{
		BuildCacheFileEx::CullTags::Initialize();
		BuildCacheFileEx::PredictedResources::Initialize();
		BuildCacheFileEx::MemoryUpgrades::Initialize(only_using_data_file_hacks);
	}

	void Dispose(bool only_using_data_file_hacks)
	{
		BuildCacheFileEx::MemoryUpgrades::Dispose(only_using_data_file_hacks);
		BuildCacheFileEx::PredictedResources::Dispose();
		BuildCacheFileEx::CullTags::Dispose();
	}
}; }; };

namespace Yelo { namespace Tool {

bool g_is_building_yelo = false;
bool IsBuildingYeloMap()
{
	return g_is_building_yelo;
}

void PLATFORM_API build_cache_file_for_scenario_extended(char* arguments[])
{
	//////////////////////////////////////////////////////////////////////////
	// Initialize arguments
	struct s_arguments {
		char* mod_name; // NOTE: we only modify this argument to null-terminate at a smaller length
		cstring copy_data_files_first_str;
		cstring store_resources_str;
		cstring use_memory_upgrades_str;
		cstring scenario_name;
	}* args = CAST_PTR(s_arguments*, arguments);

	is_building_cache_file() = true;

	bool copy_data_files_first, store_resources, use_memory_upgrades;
	ValueConversion::FromString(args->copy_data_files_first_str, copy_data_files_first);
	ValueConversion::FromString(args->store_resources_str, store_resources);
	ValueConversion::FromString(args->use_memory_upgrades_str, use_memory_upgrades);

	//////////////////////////////////////////////////////////////////////////

	g_is_building_yelo = use_memory_upgrades;

	if(!use_memory_upgrades)
	{
		bool abort_build = false;

		if(abort_build = store_resources)
			printf_s("CheApe: store-resources was set, but use-memory-upgrades wasn't! Aborting build\n");

		if(!abort_build && copy_data_files_first)
		{
			printf_s("CheApe: ignoring meaningless create-anew argument when no memory upgrades are active\n");
			copy_data_files_first = false;
		}

		if(abort_build)
			return;
	}

	if(copy_data_files_first == true && store_resources == false)
		printf_s("CheApe: creating mod-set files, but not storing anything in them (is this intentional?)\n");


	//////////////////////////////////////////////////////////////////////////
	// Initialize locals and globals
	build_cache_file_for_scenario_internals.build_failed = false;
	build_cache_file_for_scenario_internals.scenario_path = args->scenario_name;

	BuildCacheFileEx::MemoryUpgrades::yelo_cache_header_globals.InitializeForNewMap();
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	// validate and fix the mod name if it is too long for a tag_string
	const size_t k_max_mod_set_name_length = Enums::k_tag_string_length - sizeof("-bitmaps");

	size_t mod_set_name_length = strlen(args->mod_name);

	if(mod_set_name_length > k_max_mod_set_name_length)
		args->mod_name[k_max_mod_set_name_length+1] = '\0';
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	// setup the data files
	bool using_mod_sets = use_memory_upgrades && mod_set_name_length > 0;

	Cache::c_data_files data_files;

	s_build_cache_file_for_scenario::InitializeFileSystem(data_files, args->mod_name, using_mod_sets);

	if(using_mod_sets)
	{
		printf_s("Engine Mod: cache will use mod-set '%s'\n", args->mod_name);

		// check if the user wants to copy the base data files
		if(copy_data_files_first)
			data_files.CopyStock();
	}
	//////////////////////////////////////////////////////////////////////////


	BuildCacheFileEx::MemoryUpgrades::InitializeHeaderGlobals(using_mod_sets, args->mod_name, use_memory_upgrades);

	// open the data files for referencing and write
	s_build_cache_file_for_scenario& bcffs = build_cache_file_for_scenario_internals;
	bcffs.DataFilesOpen(data_files, store_resources);

	// If we're using OS's memory upgrades, force tool to use our file naming convention
	if(use_memory_upgrades) bcffs.InitializeBuildCacheFileEndSprintfOverride();

	BuildCacheFileEx::Initialize(use_memory_upgrades == false);

	// build cache file
		bcffs._build_cache_file_for_scenario(args->scenario_name);

	BuildCacheFileEx::Dispose(use_memory_upgrades == false);

	bcffs.BuildPostprocess(args->mod_name, using_mod_sets);

	bcffs.DataFilesClose(store_resources);
}

}; };
#endif