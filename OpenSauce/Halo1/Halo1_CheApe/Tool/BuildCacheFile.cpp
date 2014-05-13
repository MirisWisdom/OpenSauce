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
#include <YeloLib/Halo1/open_sauce/project_yellow_scenario_definitions.hpp>

#include "Engine/EngineFunctions.hpp"
#include "Engine/GrenadeTypesUpgrade.hpp"
#include "Engine/Scripting.hpp"
#include "TagGroups/yelo_definitions.hpp"

#include "Tool/BuildCacheFile/BuildCacheFileGlobals.hpp"
#include "Tool/BuildCacheFile/BuildCacheFileMemoryUpgrades.hpp"

#include "Tool/BuildCacheFile/TagPreprocess.inl"

namespace Yelo { namespace Tool {
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

	void build_cache_file_begin_preprocess(cstring scenario_name)
	{
		static cstring k_stock_globals_tag_name = "globals\\globals";

		datum_index scenario_index = blam::tag_load<TagGroups::scenario>(build_cache_file_for_scenario_internals.scenario_path, 0);
		if (!scenario_index.IsNull())
		{
			auto* scenario = blam::tag_get<TagGroups::scenario>(scenario_index);
			const tag_reference& yelo_reference = scenario->GetYeloReferenceHack();

			if (!yelo_reference.tag_index.IsNull())
			{
				auto* yelo = blam::tag_get<TagGroups::project_yellow>(yelo_reference.tag_index);

				// If the scenario's yelo specifies build info, update the yelo header with that info, else use the defaults
				if (yelo->build_info.Count > 0)
				{
					const TagGroups::s_project_yellow_scenario_build_info& build_info = yelo->build_info[0];

					BuildCacheFileEx::MemoryUpgrades::InitializeHeaderGlobalsBuildInfo(build_info);
				}
				else
					BuildCacheFileEx::MemoryUpgrades::InitializeHeaderGlobalsBuildInfo();

				// if there is a globals tag override, rename it to "globals/globals" so 
				// we avoid further hacks in the runtime engine code as it also explicitly 
				// looks for "globals/globals"
				if (yelo->game_globals.name_length > 0)
				{
					yelo->game_globals.tag_index =
						blam::tag_load(yelo->game_globals.group_tag, yelo->game_globals.name, 0);

					if (!yelo->game_globals.tag_index.IsNull())
						blam::tag_rename(yelo->game_globals.tag_index, k_stock_globals_tag_name);

					// re-set the reference just for good measure
					blam::tag_reference_set(yelo->game_globals,
						yelo->game_globals.group_tag, k_stock_globals_tag_name);
				}
			}

			BuildCacheFileEx::TagPreprocess::preprocess_tags_for_build();
		}
	}

	void PLATFORM_API build_cache_file_for_scenario_stock_override(char* arguments[])
	{
		//////////////////////////////////////////////////////////////////////////
		// Initialize arguments
		struct s_arguments {
			char* scenario_name;
		}*args = CAST_PTR(s_arguments*, arguments);
		//////////////////////////////////////////////////////////////////////////

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
}; };

#endif