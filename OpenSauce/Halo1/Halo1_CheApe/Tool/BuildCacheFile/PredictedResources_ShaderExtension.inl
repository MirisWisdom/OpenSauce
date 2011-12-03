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

namespace ShaderExtension
{
	static void shader_model_extension_add_to_predicted_resources(
		TagGroups::s_shader_model_extension& extension, 
		TagBlock<TagGroups::predicted_resource>& predicted_resources)
	{
		if(!extension.specular_color.map.tag_index.IsNull())
			predicted_resources_add_resource(predicted_resources,
				Enums::_predicted_resource_bitmap,
				extension.specular_color.map.tag_index,
				0);

		if(!extension.base_normal.map.tag_index.IsNull())
			predicted_resources_add_resource(predicted_resources, 
				Enums::_predicted_resource_bitmap,
				extension.base_normal.map.tag_index,
				0);

		for(int32 x = 0; x < NUMBEROF(extension.detail_normals); x++)
			if(!extension.detail_normals[x].map.tag_index.IsNull())
				predicted_resources_add_resource(predicted_resources, 
					Enums::_predicted_resource_bitmap,
					extension.detail_normals[x].map.tag_index,
					0);

	}

	static bool object_add_to_predicted_resources(datum_index tag_index)
	{
		TagGroups::s_object_definition* object_tag = Yelo::tag_get<TagGroups::s_object_definition>(tag_index);
		
		if(object_tag->object.references.render_model.tag_index.IsNull())
			return true;

		TagGroups::gbxmodel_definition* gbxmodel_tag = Yelo::tag_get<TagGroups::gbxmodel_definition>(object_tag->object.references.render_model.tag_index);
		
		for(int32 i = 0; i < gbxmodel_tag->shaders.Count; i++)
		{
			const tag_reference& shader = gbxmodel_tag->shaders[i].shader;

			if(	shader.group_tag != TagGroups::s_shader_model_definition::k_group_tag ||
				shader.tag_index.IsNull())
				continue;

			TagGroups::s_shader_model_definition* shader_model_tag = Yelo::tag_get<TagGroups::s_shader_model_definition>(shader.tag_index);

			if(shader_model_tag->model.maps.shader_extension.Count != 1)
				continue;

			TagGroups::s_shader_model_extension& extension = shader_model_tag->model.maps.shader_extension[0];
			shader_model_extension_add_to_predicted_resources(extension, object_tag->object.predicted_resources);
		}
		return true;
	}
};