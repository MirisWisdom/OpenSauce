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
	static bool shader_model_extension_process(datum_index tag_index)
	{
		TagGroups::s_shader_model_definition* shader_tag = Yelo::tag_get<TagGroups::s_shader_model_definition>(tag_index);

		if(shader_tag->model.maps.shader_extension.Count != 1)
			return true;

		TagGroups::s_shader_model_extension& extension = shader_tag->model.maps.shader_extension[0];

		shader_tag->shader.extension_usage = Enums::_shader_extension_usage_none;

		if(!extension.base_normal.map.tag_index.IsNull())
			shader_tag->shader.extension_usage |= Enums::_shader_extension_usage_normal_map;
		if(	extension.base_normal.modifiers.coefficient == 0.0f)
			extension.base_normal.modifiers.coefficient =  1.0f;

		for(int32 x = 0; x < NUMBEROF(extension.detail_normals); x++)
		{
			if(!extension.detail_normals[x].map.tag_index.IsNull())
				shader_tag->shader.extension_usage |= Enums::_shader_extension_usage_detail_normal;
			if(	extension.detail_normals[x].modifiers.coefficient == 0.0f)
				extension.detail_normals[x].modifiers.coefficient =  1.0f;
			if(	extension.detail_normals[x].modifiers.scale == 0.0f)
				extension.detail_normals[x].modifiers.scale =  1.0f;
			if(	extension.detail_normals[x].modifiers.v_scale == 0.0f)
				extension.detail_normals[x].modifiers.v_scale =  1.0f;
		}

		if(!extension.specular_color.map.tag_index.IsNull())
			shader_tag->shader.extension_usage |= Enums::_shader_extension_usage_specular_map;
		if(	extension.specular_color.modifiers.coefficient == 0.0f)
			extension.specular_color.modifiers.coefficient =  1.0f;
		if(	extension.specular_color.modifiers.exponent == 0.0f)
			extension.specular_color.modifiers.exponent =  1.0f;

		//if(	extension.specular_reflection_exponent == 0.0f)
		//	extension.specular_reflection_exponent =  1.0f;
		//if(	extension.specular_reflection_coefficient == 0.0f)
		//	extension.specular_reflection_coefficient =  1.0f;

		if(	extension.specular_lighting_exponent != 0.0f)
			shader_tag->shader.extension_usage |= Enums::_shader_extension_usage_specular_lighting;
		if(	extension.specular_lighting_coefficient == 0.0f)
			extension.specular_lighting_coefficient =  1.0f;

		return true;
	}

	static bool shader_environment_extension_process(datum_index tag_index)
	{
		TagGroups::s_shader_environment_definition* shader_tag = Yelo::tag_get<TagGroups::s_shader_environment_definition>(tag_index);

		shader_tag->shader.extension_usage = Enums::_shader_extension_usage_none;

		if(!shader_tag->environment.bump_map.map.tag_index.IsNull())
			shader_tag->shader.extension_usage |= Enums::_shader_extension_usage_normal_map;

		return true;
	}
};