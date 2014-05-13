/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/

#include <blamlib/Halo1/tag_files/tag_groups.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_global_definitions.hpp>

#include "Memory/MemoryInterface.hpp"

namespace Yelo { namespace Tool {

namespace BuildCacheFileEx
{
	namespace CullTags
	{
		static void* BUILD_CACHE_FILE_CULL_TAGS_CALL = CAST_PTR(void*, 0x455541);
		static bool PLATFORM_API cull_custom_tags()
		{
			bool result = build_cache_file_for_scenario_internals.build_cache_file_cull_tags();

			// call custom tag's data culling stuff in this loop
			for(auto tag_index : TagGroups::c_tag_iterator::all())
			{
				switch( blam::tag_get_group_tag(tag_index) )
				{
				case TagGroups::project_yellow_globals::k_group_tag:
					{
						blam::tag_get<TagGroups::project_yellow_globals>(tag_index)->Cull();
					} break;
				}
			}

			return result;
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