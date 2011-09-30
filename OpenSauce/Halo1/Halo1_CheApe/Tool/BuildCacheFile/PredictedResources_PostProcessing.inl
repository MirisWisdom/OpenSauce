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

namespace PostProcessing
{
	static bool shader_postprocess_generic_add_predicted_resources(datum_index tag_index)
	{
		TagGroups::s_shader_postprocess_generic* shader_tag = Yelo::tag_get<TagGroups::s_shader_postprocess_generic>(tag_index);
		
		if(shader_tag->shader_code_binary.size == 0 && shader_tag->base_shader.tag_index.IsNull())
		{
			s_tag_instance* tag_instance = (*TagGroups::TagInstances())[tag_index];

			YELO_ERROR(_error_message_priority_warning,
				"\nCheApe: shader_postprocess_generic validity failed on '%s'\n"
				"error: postprocess shader with no binary data also has no base shader defined",
				tag_instance->filename);
			return false;
		}

		for(int32 i = 0; i < shader_tag->parameters.Count; i++)
		{
			const TagGroups::s_shader_postprocess_parameter& parameter = shader_tag->parameters[i];

			if(parameter.value_type.type != Enums::_shader_variable_base_type_texture)
				continue;

			predicted_resources_add_resource(shader_tag->predicted_resources, 
				Enums::_predicted_resource_bitmap,
				parameter.bitmap_value.bitmap.tag_index,
				parameter.value.bitmap.bitmap_index);
		}
		return true;
	}
};