/*
    Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition
    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

namespace BuildCacheFileEx
{
	namespace PredictedResources
	{
		static void predicted_resources_add_resource(TagBlock<TagGroups::predicted_resource>& predicted_resources, 
			long_enum resource_type, datum_index tag_index, int32 resource_index = NONE)
		{
			static void* PREDICTED_RESOURCES_ADD_RESOURCE = CAST_PTR(void*, 0x4B94E0);

			typedef void (PLATFORM_API* call_proc)(TagBlock<TagGroups::predicted_resource>&, long_enum, datum_index, int32);
			static call_proc add_predicted_resource = CAST_PTR(call_proc, PREDICTED_RESOURCES_ADD_RESOURCE);

			add_predicted_resource(predicted_resources, resource_type, tag_index, resource_index);
		}

#include "Tool/BuildCacheFile/PredictedResources_ShaderExtension.inl"
#include "Tool/BuildCacheFile/PredictedResources_PostProcessing.inl"

		static void* PREDICTED_RESOURCES_ADD_RESOURCES_FROM_SHADER_TAG_HOOK = CAST_PTR(void*, 0x4B97AB);
		// Hook placed in tool's code so we can add extra resource info
		static void PLATFORM_API predicted_resources_add_resources_from_shader_tag_ex(TagBlock<TagGroups::predicted_resource>& predicted_resources,
			datum_index tag_index, int16 permutation)
		{
		}

		static void* BUILD_STRUCTURE_BSP_PREDICTED_RESOURCES_CALL = CAST_PTR(void*, 0x45557A);
		static bool PLATFORM_API build_custom_predicted_resources()
		{
			static void* BUILD_STRUCTURE_BSP_PREDICTED_RESOURCES = CAST_PTR(void*, 0x453860);

			typedef bool (PLATFORM_API* call_proc)();
			static const call_proc build_structure_bsp_predicted_resources = CAST_PTR(call_proc, BUILD_STRUCTURE_BSP_PREDICTED_RESOURCES);

			bool result = build_structure_bsp_predicted_resources();

			TagGroups::tag_iterator tag_iter;
			tag_iterator_new(tag_iter, NULL_HANDLE);

			printf("processing custom tag instances...");
			// call custom tag's predicted resource stuff in this loop
 			datum_index tag_index;
 			while( !(tag_index = tag_iterator_next(tag_iter)).IsNull() )
 			{
				switch((*TagGroups::TagInstances())[tag_index.index].parent_group_tags[1])
				{
				case TagGroups::s_object_definition::k_group_tag:
					ShaderExtension::object_add_to_predicted_resources(tag_index);
					break;
				}

				switch((*TagGroups::TagInstances())[tag_index.index].parent_group_tags[0])
				{
				case TagGroups::s_object_definition::k_group_tag:
					ShaderExtension::object_add_to_predicted_resources(tag_index);
					break;
				}

				switch((*TagGroups::TagInstances())[tag_index.index].group_tag)
				{
				case TagGroups::s_shader_postprocess_generic::k_group_tag:
					result &= PostProcessing::shader_postprocess_generic_add_predicted_resources(tag_index);
					break;
				case TagGroups::s_shader_postprocess_collection::k_group_tag:
					result &= PostProcessing::shader_postprocess_collection_process_collection(tag_index);
					break;
				}

				if(!result) break;
			}
			if(result)
				puts("done");

			return result;
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