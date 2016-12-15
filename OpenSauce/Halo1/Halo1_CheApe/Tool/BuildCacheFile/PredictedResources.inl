/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/

#include <blamlib/Halo1/cache/predicted_resources.hpp>
#include <blamlib/Halo1/tag_files/tag_groups.hpp>
#include <YeloLib/Halo1/cache/cache_file_builder_yelo.hpp>

namespace Yelo { namespace Tool {

namespace BuildCacheFileEx
{
	namespace PredictedResources
	{
		static void* PREDICTED_RESOURCES_ADD_RESOURCES_FROM_SHADER_TAG_HOOK = CAST_PTR(void*, 0x4B97AB);
		// Hook placed in tool's code so we can add extra resource info
		static void PLATFORM_API predicted_resources_add_resources_from_shader_tag_ex(TagBlock<TagGroups::predicted_resource>& predicted_resources,
			datum_index tag_index, int16 permutation)
		{
		}

		static void* BUILD_STRUCTURE_BSP_PREDICTED_RESOURCES_CALL = CAST_PTR(void*, 0x45557A);
		static bool PLATFORM_API build_custom_predicted_resources()
		{
			return
				build_cache_file_for_scenario_internals.build_structure_bsp_predicted_resources() &&
				Cache::BuildCacheFilePredicatedResourcesYelo();
		}

		void Initialize()
		{
			// doesn't do anything yet
			//Memory::WriteRelativeCall(&predicted_resources_add_resources_from_shader_tag_ex, 
			//	PREDICTED_RESOURCES_ADD_RESOURCES_FROM_SHADER_TAG_HOOK);
			Memory::WriteRelativeCall(&build_custom_predicted_resources, 
				BUILD_STRUCTURE_BSP_PREDICTED_RESOURCES_CALL);
		}

		void Dispose()
		{
		}
	};
};

}; };