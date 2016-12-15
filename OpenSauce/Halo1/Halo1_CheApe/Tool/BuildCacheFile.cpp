/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Tool/BuildCacheFile.hpp"
#if PLATFORM_TYPE == PLATFORM_TOOL
#include <blamlib/Halo1/cseries/errors.hpp>
#include <blamlib/Halo1/cache/cache_file_builder.hpp>
#include <blamlib/Halo1/scenario/scenario_definitions.hpp>
#include <blamlib/Halo1/bitmaps/bitmap_group.hpp>
#include <blamlib/Halo1/structures/structure_bsp_definitions.hpp>

#include <YeloLib/Halo1/open_sauce/project_yellow_global_definitions.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_scenario.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_scenario_definitions.hpp>
#include <YeloLib/cseries/value_conversion.hpp>

#include "Engine/EngineFunctions.hpp"
#include "Engine/GrenadeTypesUpgrade.hpp"
#include "Engine/Scripting.hpp"
#include "TagGroups/yelo_definitions.hpp"

#include "Tool/BuildCacheFile/BuildCacheFileGlobals.hpp"
#include "Tool/BuildCacheFile/BuildCacheFileMemoryUpgrades.hpp"

#include "Tool/BuildCacheFile/TagPreprocess.inl"

namespace Yelo { namespace Tool {

	struct s_build_version
	{
		byte major;
		byte minor;
		uint16 build;
	};
	static s_build_version g_minimum_build_version;
	static bool g_is_building_cache_file = false;

	bool& is_building_cache_file()
	{
		return g_is_building_cache_file;
	}

	void build_cache_file_set_minimum_os_build(const byte major, const byte minor, const uint16 build)
	{
		if((g_minimum_build_version.major <= major)
			&& (g_minimum_build_version.minor <= minor)
			&& (g_minimum_build_version.build <= build))
		{
			g_minimum_build_version.major = major;
			g_minimum_build_version.minor = minor;
			g_minimum_build_version.build = build;
		}
	}

	void build_cache_file_end_preprocess(Cache::s_cache_header* header, Cache::s_cache_header_yelo& ych)
	{
		// NOTE: when build_cache_file_write_header_and_compress calls build_cache_file_end, it performs a printf
		// without a newline, so we do one here
		printf_s("\n");

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

		if (result)
		{
			bcffs.build_cache_file_add_resource(buffer, ych.cheape_definitions.size,
				&ych.cheape_definitions.offset, false);

			delete buffer;
		}
		//////////////////////////////////////////////////////////////////////////

		// Add the build date of the OS HEK tools
		CheApeApi_GetPchBuildDateA(_countof(ych.cheape_definitions.build_string), ych.cheape_definitions.build_string);

		printf_s("Checking tags for OS-modified game-state requirements...\n");
		bool uses_game_state_upgrades = TagGroups::YeloToolCheckTagsForGameStateUpgradeRequirements();
		if (ych.flags.uses_game_state_upgrades = uses_game_state_upgrades)
			printf_s("\tFound tag(s) which require OS-modified game-states to (fully) function\n");
	}

	static void BuildCacheFileInitializeForNewProjectYellow()
	{
		// If the scenario's yelo specifies build info, update the yelo header with that info, else use the defaults
		if (Scenario::GetYelo()->build_info.Count > 0)
		{
			const TagGroups::s_project_yellow_scenario_build_info& build_info = Scenario::GetYelo()->build_info[0];

			BuildCacheFileEx::MemoryUpgrades::InitializeHeaderGlobalsBuildInfo(build_info);
		}
		else
			BuildCacheFileEx::MemoryUpgrades::InitializeHeaderGlobalsBuildInfo();

		// default definitions won't specify a game_globals override
		if (auto* yelo = Scenario::TryGetYeloForModification())
			yelo->LoadGameGlobalsOverride();
	}

	void build_cache_file_begin_preprocess(cstring scenario_name)
	{
		BuildCacheFileInitializeForNewProjectYellow();

		BuildCacheFileEx::TagPreprocess::preprocess_tags_for_build();

		BuildCacheFileEx::MemoryUpgrades::InitializeHeaderGlobalsMinimumBuild(g_minimum_build_version.major, g_minimum_build_version.minor, g_minimum_build_version.build);
	}

	void PLATFORM_API build_cache_file_for_scenario_stock_override(char* arguments[])
	{
		//////////////////////////////////////////////////////////////////////////
		// Initialize arguments
		struct s_arguments {
			char* scenario_name;
		}*args = CAST_PTR(s_arguments*, arguments);
		//////////////////////////////////////////////////////////////////////////

		is_building_cache_file() = true;

		printf_s("CheApe: reminder: stock build-cache-file doesn't support\n");
		printf_s("\t* "	"OS script definitions\n");
		printf_s("\t* "	"additional grenade types\n");
		BuildCacheFileEx::MemoryUpgrades::PrintScriptUpgradesUsage();

		// Don't allow script node upgrades when building stock cache files
		Scripting::DisposeScriptNodeUpgrades();
		Objects::Items::GrenadeTypesUpgrade(false);

		s_build_cache_file_for_scenario& bcffs = build_cache_file_for_scenario_internals;
		bcffs.build_cache_file_for_scenario_command(arguments);

		Scripting::InitializeScriptNodeUpgrades();
	}

	void PLATFORM_API build_cache_file_for_scenario_new(char* arguments[])
	{
		//////////////////////////////////////////////////////////////////////////
		// Initialize arguments
		struct s_arguments {
			cstring copy_data_files_first_str;
			cstring store_resources_str;
			cstring use_memory_upgrades_str;
			char* scenario_name;
		}* args = CAST_PTR(s_arguments*, arguments);

		is_building_cache_file() = true;

		bool copy_data_files_first, store_resources, use_memory_upgrades;
		ValueConversion::FromString(args->copy_data_files_first_str, copy_data_files_first);
		ValueConversion::FromString(args->store_resources_str, store_resources);
		ValueConversion::FromString(args->use_memory_upgrades_str, use_memory_upgrades);
		//////////////////////////////////////////////////////////////////////////
		
		if (!use_memory_upgrades)
		{
			bool abort_build = false;

			if (abort_build = store_resources)
				printf_s("CheApe: store-resources was set, but use-memory-upgrades wasn't! Aborting build\n");

			if (!abort_build && copy_data_files_first)
			{
				printf_s("CheApe: ignoring meaningless create-anew argument when no memory upgrades are active\n");
				copy_data_files_first = false;
			}

			if (abort_build)
				return;
		}

		if (copy_data_files_first == true && store_resources == false)
			printf_s("CheApe: creating mod-set files, but not storing anything in them (is this intentional?)\n");

		long_flags begin_flags = 0;
		// TODO: we're just gonna assume that use-memory-upgrades is our .yelo switch, right?
		SET_FLAG(begin_flags, Flags::_build_cache_file_begin_building_yelo_bit, use_memory_upgrades);
		SET_FLAG(begin_flags, Flags::_build_cache_file_begin_mod_sets_create_anew_bit, copy_data_files_first);
		SET_FLAG(begin_flags, Flags::_build_cache_file_begin_mod_sets_store_scenario_resources_bit, store_resources);
		SET_FLAG(begin_flags, Flags::_build_cache_file_begin_use_memory_upgrades_bit, use_memory_upgrades);

		blam::build_cache_file_for_scenario(args->scenario_name, CAST(byte_flags, begin_flags));
	}
}; };

#endif