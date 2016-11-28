/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/

#include <blamlib/Halo1/tag_files/tag_groups.hpp>
#include <YeloLib/Halo1/cache/cache_file_builder_yelo.hpp>

#include "Memory/MemoryInterface.hpp"

namespace Yelo { namespace Tool {

namespace BuildCacheFileEx
{
	namespace CullTags
	{
		static void* BUILD_CACHE_FILE_CULL_TAGS_CALL = CAST_PTR(void*, 0x455541);
		static bool PLATFORM_API cull_custom_tags()
		{
			return 
				build_cache_file_for_scenario_internals.build_cache_file_cull_tags() &&
				Cache::BuildCacheFileCullTagsYelo();
		}

		void Initialize()
		{
			Memory::WriteRelativeCall(&cull_custom_tags, BUILD_CACHE_FILE_CULL_TAGS_CALL);
		}

		void Dispose()
		{
		}
	};
};

}; };