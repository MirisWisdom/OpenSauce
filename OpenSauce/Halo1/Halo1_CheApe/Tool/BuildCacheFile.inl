/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/

#include "Tool/BuildCacheFile/DataFiles.inl"
#include "Tool/BuildCacheFile/BuildGlobals.inl"
#include "Tool/BuildCacheFile/CullTags.inl"
#include "Tool/BuildCacheFile/PredictedResources.inl"
#include "Tool/BuildCacheFile/TagPreprocess.inl"

void build_cache_file_end_preprocess(Cache::s_cache_header* header, Cache::s_cache_header_yelo& ych);
void build_cache_file_begin_preprocess(cstring scenario_name);
#include "Tool/BuildCacheFile/MemoryUpgrades.inl"
/*!
 * \brief
 * Code to execute before we commit the yelo header and the tool's code finishes building the cache.
 * 
 * This function is called before we commit the yelo header to the resulting cache file's header. The implementing code which 
 * calls this (MemoryUpgrades.inl, InterceptorEnd) calls the tool's build_cache_file_end function after this function completes.
 * 
 */
static void build_cache_file_end_preprocess(Cache::s_cache_header* header, Cache::s_cache_header_yelo& ych)
{
	s_build_cache_file_for_scenario& bcffs = build_cache_file_for_scenario_internals;

	//////////////////////////////////////////////////////////////////////////
	// Setup the core tag versions
	ych.tag_versioning.project_yellow = Yelo::TagGroups::project_yellow::k_version;
	ych.tag_versioning.project_yellow_globals = Yelo::TagGroups::project_yellow_globals::k_version;
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Add the definitions for debugging purposes
	void* buffer;
	bool result = CheApe::GetCacheFileResourceBuffer(buffer, 
		ych.cheape_definitions.decompressed_size, ych.cheape_definitions.size);

	if(result)
	{
		bcffs.build_cache_file_add_resource(buffer, ych.cheape_definitions.size, 
			&ych.cheape_definitions.offset, false);

		delete buffer;
	}
	//////////////////////////////////////////////////////////////////////////

	// Add the build date of the OS HEK tools
	CheApeApi_GetPchBuildDateA(sizeof(ych.cheape_definitions.build_string), ych.cheape_definitions.build_string);

	printf_s("Checking tags for OS-modified game-state requirements...\n");
	bool uses_game_state_upgrades = TagGroups::YeloToolCheckTagsForGameStateUpgradeRequirements();
	if(ych.flags.uses_game_state_upgrades = uses_game_state_upgrades)
		printf_s("\tFound tag(s) which require OS-modified game-states to (fully) function\n");
}

/*!
 * \brief
 * Code to execute before the tool's code for building the cache begins.
 * 
 * \param scenario_name
 * Tag name of the scenario which is being built into a cache.
 * 
 * Since this code is executed before any real cache building code begins, we can do things like create stub tags, in the event the user 
 * doesn't define them. Or anything else we need to do/change in tags (e.g., rename) before they're committed to cache memory.
 *
 */
static void build_cache_file_begin_preprocess(cstring scenario_name)
{
	datum_index scenario_index = tag_load<TagGroups::scenario>(build_cache_file_for_scenario_internals.scenario_path, 0);
	if(!scenario_index.IsNull())
	{
		TagGroups::scenario* scenario = tag_get<TagGroups::scenario>(scenario_index);
		const tag_reference& yelo_reference = scenario->GetYeloReferenceHack();

		if(!yelo_reference.tag_index.IsNull())
		{
			TagGroups::project_yellow* yelo = tag_get<TagGroups::project_yellow>(yelo_reference.tag_index);

			// If the scenario's yelo specifies build info, update the yelo header with that info, else use the defaults
			if(yelo->build_info.Count > 0)
			{
				const TagGroups::s_project_yellow_scenario_build_info& build_info = yelo->build_info[0];

				BuildCacheFileEx::MemoryUpgrades::InitializeHeaderGlobalsBuildInfo(build_info);
			}
			else
				BuildCacheFileEx::MemoryUpgrades::InitializeHeaderGlobalsBuildInfo();

			// if there is a globals tag override, rename it to "globals/globals" so 
			// we avoid further hacks in the runtime engine code as it also explicitly 
			// looks for "globals/globals"
			if(yelo->game_globals.name_length > 0)
			{
				yelo->game_globals.tag_index = 
					tag_load(yelo->game_globals.group_tag, yelo->game_globals.name, 0);

				if(!yelo->game_globals.tag_index.IsNull())
					tag_rename(yelo->game_globals.tag_index, "globals\\globals");

				// re-set the reference just for good measure
				tag_reference_set(&yelo->game_globals, 
					yelo->game_globals.group_tag, "globals\\globals");
			}
		}

		BuildCacheFileEx::TagPreprocess::preprocess_tags_for_build();
	}
}

namespace BuildCacheFileEx
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
};

static void PLATFORM_API build_cache_file_for_scenario_extended(void** arguments)
{
	//////////////////////////////////////////////////////////////////////////
	// Initialize arguments
	struct s_arguments {
		char* mod_name;
		cstring copy_data_files_first_str;
		cstring store_resources_str;
		cstring use_memory_upgrades_str;
		char* scenario_name;
	}* args = CAST_PTR(s_arguments*, arguments);

	bool copy_data_files_first, store_resources, use_memory_upgrades;
	copy_data_files_first = Settings::ParseBoolean(args->copy_data_files_first_str);
	store_resources = Settings::ParseBoolean(args->store_resources_str);
	use_memory_upgrades = Settings::ParseBoolean(args->use_memory_upgrades_str);
	//////////////////////////////////////////////////////////////////////////

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

	c_data_files data_files;

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

/*!
 * \brief
 * We replace the stock build_cache_file_for_scenario implementation with out own to turn off non-stock-compliant things like custom script definitions
 */
static void PLATFORM_API build_cache_file_for_scenario_stock_override(void** arguments)
{
	//////////////////////////////////////////////////////////////////////////
	// Initialize arguments
	struct s_arguments {
		char* scenario_name;
	}* args = CAST_PTR(s_arguments*, arguments);
	//////////////////////////////////////////////////////////////////////////

	printf_s("CheApe: warning: build-cache-file doesn't support custom script definitions...\n");
	BuildCacheFileEx::MemoryUpgrades::PrintScriptUpgradesUsage();

	// Don't allow script node upgrades when building stock cache files
	Scripting::DisposeScriptNodeUpgrades();

	s_build_cache_file_for_scenario& bcffs = build_cache_file_for_scenario_internals;
	bcffs.build_cache_file_for_scenario_command(arguments);

	Scripting::InitializeScriptNodeUpgrades();
}