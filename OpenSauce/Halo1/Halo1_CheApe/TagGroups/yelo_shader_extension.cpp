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
#include <Common/Precompile.hpp>
#include "TagGroups/yelo_shader_extension.hpp"

#if PLATFORM_ID == PLATFORM_TOOL
#include <TagGroups/Halo1/object_definitions.hpp>
#include <TagGroups/Halo1/model_definitions.hpp>
#include <TagGroups/Halo1/shader_definitions.hpp>

#include "Engine/EngineFunctions.hpp"
#include "TagGroups/TagGroups.hpp"

namespace Yelo
{
	namespace ShaderExtension
	{
		static void shader_model_extension_add_to_predicted_resources(
			const TagGroups::s_shader_model_extension& extension, 
			TagBlock<TagGroups::predicted_resource>& predicted_resources)
		{
			if(!extension.specular_color.map.tag_index.IsNull())
			{
				int32 element_index = tag_block_add_element(predicted_resources);
				TagGroups::predicted_resource* predicted_resource_element = tag_block_get_element(predicted_resources, 
					element_index);

				predicted_resource_element->type = Enums::_predicted_resource_bitmap;
				predicted_resource_element->resource_index = 0;
				predicted_resource_element->tag_index = extension.specular_color.map.tag_index;
			}
			if(!extension.base_normal.map.tag_index.IsNull())
			{
				int32 element_index = tag_block_add_element(predicted_resources);
				TagGroups::predicted_resource* predicted_resource_element = tag_block_get_element(predicted_resources, 
					element_index);

				predicted_resource_element->type = Enums::_predicted_resource_bitmap;
				predicted_resource_element->resource_index = 0;
				predicted_resource_element->tag_index = extension.base_normal.map.tag_index;
			}
			if(!extension.detail_normals[0].map.tag_index.IsNull())
			{
				int32 element_index = tag_block_add_element(predicted_resources);
				TagGroups::predicted_resource* predicted_resource_element = tag_block_get_element(predicted_resources, 
					element_index);

				predicted_resource_element->type = Enums::_predicted_resource_bitmap;
				predicted_resource_element->resource_index = 0;
				predicted_resource_element->tag_index = extension.detail_normals[0].map.tag_index;
			}
			if(!extension.detail_normals[1].map.tag_index.IsNull())	
			{
				int32 element_index = tag_block_add_element(predicted_resources);
				TagGroups::predicted_resource* predicted_resource_element = tag_block_get_element(predicted_resources, 
					element_index);

				predicted_resource_element->type = Enums::_predicted_resource_bitmap;
				predicted_resource_element->resource_index = 0;
				predicted_resource_element->tag_index = extension.detail_normals[1].map.tag_index;
			}
		}

		// TODO: Should we move the following code into a process which can be called by the Tool\BuildCacheFile\PredictedResource.inl code instead?
		static proc_tag_group_postprocess g_object_postproc;
		bool PLATFORM_API object_group_postprocess(datum_index tag_index, bool verify_data)
		{
			bool base_bool = (*g_object_postproc)(tag_index, verify_data);

			TagGroups::s_object_definition* object_tag = Yelo::tag_get<TagGroups::s_object_definition>(tag_index);
			
			if(object_tag->object.references.render_model.tag_index.IsNull())
				return base_bool;

			TagGroups::s_gbxmodel_definition* gbxmodel_tag = Yelo::tag_get<TagGroups::s_gbxmodel_definition>(object_tag->object.references.render_model.tag_index);
			
			for(int32 i = 0; i < gbxmodel_tag->shaders.Count; i++)
			{
				const tag_reference& shader = gbxmodel_tag->shaders[i].shader;

				if((shader.group_tag != TagGroups::s_shader_model::k_group_tag) ||
					shader.tag_index.IsNull())
					continue;

				TagGroups::s_shader_model* shader_model_tag = Yelo::tag_get<TagGroups::s_shader_model>(shader.tag_index);

				if(shader_model_tag->model.maps.shader_extension.Count != 1)
					continue;

				const TagGroups::s_shader_model_extension& extension = shader_model_tag->model.maps.shader_extension[0];

				shader_model_extension_add_to_predicted_resources(extension, object_tag->object.predicted_resources);
			}
			return base_bool;
		}
		
		
		void Initialize()
		{
			Yelo::tag_group_definition* obje_definition = Yelo::tag_group_get<TagGroups::s_object_definition>();
			if(obje_definition)
			{
				g_object_postproc = obje_definition->postprocess_proc;
				obje_definition->postprocess_proc = &object_group_postprocess;
			}
		}
		void Dispose()
		{
		}
	};
};
#else
namespace Yelo
{
	namespace ShaderExtension
	{
		void Initialize() {}
		void Dispose() {}
	};
};
#endif