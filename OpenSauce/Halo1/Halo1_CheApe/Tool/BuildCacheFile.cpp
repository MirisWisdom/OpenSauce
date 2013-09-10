/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Tool/BuildCacheFile.hpp"
#if PLATFORM_ID == PLATFORM_TOOL
#include <blamlib/Halo1/cache/cache_file_builder.hpp>
#include <blamlib/Halo1/cache/cache_files.hpp>
#include <blamlib/Halo1/models/model_definitions.hpp>
#include <blamlib/Halo1/objects/object_definitions.hpp>
#include <blamlib/Halo1/scenario/scenario_definitions.hpp>
#include <blamlib/Halo1/shaders/shader_definitions.hpp>
#include <blamlib/Halo1/tool/tool.hpp>

#include <YeloLib/Halo1/cache/data_file_yelo.hpp>
#include <YeloLib/Halo1/open_sauce/blam_memory_upgrades.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_global_definitions.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_scenario_definitions.hpp>
#include <YeloLib/Halo1/shaders/shader_postprocess_definitions.hpp>

#include "Engine/EngineFunctions.hpp"
#include "Engine/GrenadeTypesUpgrade.hpp"
#include "Engine/Scripting.hpp"
#include "TagGroups/yelo_definitions.hpp"
#include "TagGroups/yelo_scenario_definitions.hpp"

namespace Yelo
{
	namespace Tool
	{
		#include "Tool/BuildCacheFile/BuildGlobals.inl"
		#include "Tool/BuildCacheFile/CullTags.inl"
		#include "Tool/BuildCacheFile/PredictedResources.inl"
		#include "Tool/BuildCacheFile/TagPreprocess.inl"

		void build_cache_file_end_preprocess(Cache::s_cache_header* header, Cache::s_cache_header_yelo& ych);
		void build_cache_file_begin_preprocess(cstring scenario_name);
		#include "Tool/BuildCacheFile/MemoryUpgrades.inl"

		#include "Tool/BuildCacheFile.inl"
	};
};
#endif