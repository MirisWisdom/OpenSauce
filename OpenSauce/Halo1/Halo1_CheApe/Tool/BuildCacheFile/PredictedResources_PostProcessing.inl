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
	static bool shader_postprocess_generic_group_find_variable(TagGroups::shader_variable_type& type_out, tag_string& test_string, Yelo::datum_index tag_index)
	{
		TagGroups::s_shader_postprocess_generic* datum = Yelo::tag_get<TagGroups::s_shader_postprocess_generic>(tag_index);

		while(datum != NULL)
		{
			int32 i = 0;
			type_out.count = 1;

			TagGroups::s_shader_postprocess_implementation& impl = datum->implementation;

			type_out.type = Enums::_shader_variable_base_type_texture;
			for(i = 0; i < impl.bitmaps.Count; i++)
				if(strncmp(impl.bitmaps[i].value_name, test_string, Enums::k_tag_string_length) == 0)
					return true;

			type_out.type = Enums::_shader_variable_base_type_boolean;
			for(i = 0; i < impl.bools.Count; i++)
				if(strncmp(impl.bools[i].value_name, test_string, Enums::k_tag_string_length) == 0)
					return true;

			type_out.type = Enums::_shader_variable_base_type_integer;
			for(i = 0; i < impl.integers.Count; i++)
				if(strncmp(impl.integers[i].value_name, test_string, Enums::k_tag_string_length) == 0)
					return true;

			type_out.type = Enums::_shader_variable_base_type_argb_color;
			for(i = 0; i < impl.colors.Count; i++)
				if(strncmp(impl.colors[i].value_name, test_string, Enums::k_tag_string_length) == 0)
					return true;

			type_out.type = Enums::_shader_variable_base_type_float;
			for(i = 0; i < impl.floats.Count; i++)
				if(strncmp(impl.floats[i].value_name, test_string, Enums::k_tag_string_length) == 0)
					return true;

			type_out.count++;
			for(i = 0; i < impl.float2s.Count; i++)
				if(strncmp(impl.float2s[i].value_name, test_string, Enums::k_tag_string_length) == 0)
					return true;

			type_out.count++;
			for(i = 0; i < impl.float3s.Count; i++)
				if(strncmp(impl.float3s[i].value_name, test_string, Enums::k_tag_string_length) == 0)
					return true;

			type_out.count++;
			for(i = 0; i < impl.float4s.Count; i++)
				if(strncmp(impl.float4s[i].value_name, test_string, Enums::k_tag_string_length) == 0)
					return true;

			if(!datum->base_shader.tag_index.IsNull())
				datum = Yelo::tag_get<TagGroups::s_shader_postprocess_generic>(datum->base_shader.tag_index);
			else
				datum = NULL;
		}

		return false;
	}

	static bool shader_postprocess_collection_process_collection(datum_index tag_index)
	{
		TagGroups::s_shader_postprocess_collection* collection_tag = Yelo::tag_get<TagGroups::s_shader_postprocess_collection>(tag_index);

		for(int32 i = 0; i < collection_tag->effects.Count; i++)
		{
			TagGroups::s_shader_postprocess_collection_effect& collection_effect = collection_tag->effects[i];

			for(int32 j = 0; j < collection_effect.script_variables.Count; j++)
			{
				TagGroups::s_shader_postprocess_collection_shader* shader = 
					&collection_tag->shaders[
						collection_effect.shader_indices[
							collection_effect.script_variables[j].shader_index
						].shader_index
					];

				bool result = shader_postprocess_generic_group_find_variable(
					collection_effect.script_variables[j].value_type,
					collection_effect.script_variables[j].shader_variable_name,
					shader->shader.tag_index);

				if(!result)
				{
					s_tag_instance* tag_instance = (*TagGroups::TagInstances())[shader->shader.tag_index];

					YELO_ERROR(_error_message_priority_warning,
						"\nCheApe: shader_postprocess_collection validity failed on '%s'\n"
						"error: unable to find matching variable for scripted variable \"%s\"", 
						tag_instance->filename,
						collection_effect.script_variables[j].name);
					return false;
				}

				for(int k = 0; k < collection_effect.shader_indices.Count; k++)
				{
					if((collection_effect.shader_indices[k].shader_index == NONE) || 
						(collection_effect.shader_indices[k].shader_index >= collection_tag->shaders.Count))
					{
						s_tag_instance* tag_instance = (*TagGroups::TagInstances())[shader->shader.tag_index];

						YELO_ERROR(_error_message_priority_warning,
							"\nCheApe: shader_postprocess_collection validity failed on '%s'\n"
							"error: invalid shader index %i", 
							tag_instance->filename,
							collection_effect.shader_indices[k].shader_index);
						return false;											
					}
				}
			}
		}
		return true;
	}

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
			if(shader_tag->parameters[i].value_type.type != Enums::_shader_variable_base_type_texture)
				continue;

			predicted_resources_add_resource(shader_tag->predicted_resources, 
				Enums::_predicted_resource_bitmap,
				shader_tag->parameters[i].bitmap_value.bitmap.tag_index,
				shader_tag->parameters[i].value.bitmap.bitmap_index);
		}
		return true;
	}
	
	static void shader_postprocess_generic_remove_variables(TagGroups::s_shader_postprocess_generic* shader_tag)
	{
		while(shader_tag->implementation.bitmaps.Count > 0)
			Yelo::tag_block_delete_element(&shader_tag->implementation.bitmaps, 0);
		while(shader_tag->implementation.bools.Count > 0)
			Yelo::tag_block_delete_element(&shader_tag->implementation.bools, 0);
		while(shader_tag->implementation.integers.Count > 0)
			Yelo::tag_block_delete_element(&shader_tag->implementation.integers, 0);
		while(shader_tag->implementation.floats.Count > 0)
			Yelo::tag_block_delete_element(&shader_tag->implementation.floats, 0);
		while(shader_tag->implementation.float2s.Count > 0)
			Yelo::tag_block_delete_element(&shader_tag->implementation.float2s, 0);
		while(shader_tag->implementation.float3s.Count > 0)
			Yelo::tag_block_delete_element(&shader_tag->implementation.float3s, 0);
		while(shader_tag->implementation.float4s.Count > 0)
			Yelo::tag_block_delete_element(&shader_tag->implementation.float4s, 0);
		while(shader_tag->implementation.colors.Count > 0)
			Yelo::tag_block_delete_element(&shader_tag->implementation.colors, 0);
	}
	static bool shader_postprocess_generic_parameter_exists(TagGroups::s_shader_postprocess_generic* shader_tag, cstring parameter_name)
	{
		for(int j = 0; j < shader_tag->parameters.Count; j++)
		{
			if(strncmp(parameter_name, shader_tag->parameters[j].value_name, Enums::k_tag_string_length) == 0)
				return true;
		}
		return false;
	}
	static bool shader_postprocess_generic_setup_parameters(datum_index tag_index)
	{
		TagGroups::s_shader_postprocess_generic* shader_tag = Yelo::tag_get<TagGroups::s_shader_postprocess_generic>(tag_index);

		TagGroups::s_shader_postprocess_generic* current = shader_tag;
		while(current != NULL)
		{
			// add parameters from this tag
			for(int32 i = 0; i < current->implementation.bitmaps.Count; i++)
			{
				if(shader_postprocess_generic_parameter_exists(shader_tag, current->implementation.bitmaps[i].value_name))
					continue;

				int32 index = Yelo::tag_block_add_element<TagGroups::s_shader_postprocess_parameter>(shader_tag->parameters);
				memset(&shader_tag->parameters[index], 0, sizeof(TagGroups::s_shader_postprocess_parameter));
				shader_tag->parameters[index].SetParameter(&current->implementation.bitmaps[i]);

				shader_tag->parameters[index].value_type.type = Enums::_shader_variable_base_type_texture;
				shader_tag->parameters[index].value_type.count = 1;
			}
			for(int32 i = 0; i < current->implementation.bools.Count; i++)
			{
				if(shader_postprocess_generic_parameter_exists(shader_tag, current->implementation.bools[i].value_name))
					continue;

				int32 index = Yelo::tag_block_add_element<TagGroups::s_shader_postprocess_parameter>(shader_tag->parameters);
				memset(&shader_tag->parameters[index], 0, sizeof(TagGroups::s_shader_postprocess_parameter));
				shader_tag->parameters[index].SetParameter(&current->implementation.bools[i]);

				shader_tag->parameters[index].value_type.type = Enums::_shader_variable_base_type_boolean;
				shader_tag->parameters[index].value_type.count = 1;
			}
			for(int32 i = 0; i < current->implementation.integers.Count; i++)
			{
				if(shader_postprocess_generic_parameter_exists(shader_tag, current->implementation.integers[i].value_name))
					continue;

				int32 index = Yelo::tag_block_add_element<TagGroups::s_shader_postprocess_parameter>(shader_tag->parameters);
				memset(&shader_tag->parameters[index], 0, sizeof(TagGroups::s_shader_postprocess_parameter));
				shader_tag->parameters[index].SetParameter(&current->implementation.integers[i]);

				shader_tag->parameters[index].value_type.type = Enums::_shader_variable_base_type_integer;
				shader_tag->parameters[index].value_type.count = 1;
			}
			for(int32 i = 0; i < current->implementation.floats.Count; i++)
			{
				if(shader_postprocess_generic_parameter_exists(shader_tag, current->implementation.floats[i].value_name))
					continue;

				int32 index = Yelo::tag_block_add_element<TagGroups::s_shader_postprocess_parameter>(shader_tag->parameters);
				memset(&shader_tag->parameters[index], 0, sizeof(TagGroups::s_shader_postprocess_parameter));
				shader_tag->parameters[index].SetParameter(&current->implementation.floats[i]);

				shader_tag->parameters[index].value_type.type = Enums::_shader_variable_base_type_float;
				shader_tag->parameters[index].value_type.count = 1;
			}
			for(int32 i = 0; i < current->implementation.float2s.Count; i++)
			{
				if(shader_postprocess_generic_parameter_exists(shader_tag, current->implementation.float2s[i].value_name))
					continue;

				int32 index = Yelo::tag_block_add_element<TagGroups::s_shader_postprocess_parameter>(shader_tag->parameters);
				memset(&shader_tag->parameters[index], 0, sizeof(TagGroups::s_shader_postprocess_parameter));
				shader_tag->parameters[index].SetParameter(&current->implementation.float2s[i]);

				shader_tag->parameters[index].value_type.type = Enums::_shader_variable_base_type_float;
				shader_tag->parameters[index].value_type.count = 2;
			}
			for(int32 i = 0; i < current->implementation.float3s.Count; i++)
			{
				if(shader_postprocess_generic_parameter_exists(shader_tag, current->implementation.float3s[i].value_name))
					continue;

				int32 index = Yelo::tag_block_add_element<TagGroups::s_shader_postprocess_parameter>(shader_tag->parameters);
				memset(&shader_tag->parameters[index], 0, sizeof(TagGroups::s_shader_postprocess_parameter));
				shader_tag->parameters[index].SetParameter(&current->implementation.float3s[i]);

				shader_tag->parameters[index].value_type.type = Enums::_shader_variable_base_type_float;
				shader_tag->parameters[index].value_type.count = 3;
			}
			for(int32 i = 0; i < current->implementation.float4s.Count; i++)
			{
				if(shader_postprocess_generic_parameter_exists(shader_tag, current->implementation.float4s[i].value_name))
					continue;

				int32 index = Yelo::tag_block_add_element<TagGroups::s_shader_postprocess_parameter>(shader_tag->parameters);
				memset(&shader_tag->parameters[index], 0, sizeof(TagGroups::s_shader_postprocess_parameter));
				shader_tag->parameters[index].SetParameter(&current->implementation.float4s[i]);

				shader_tag->parameters[index].value_type.type = Enums::_shader_variable_base_type_float;
				shader_tag->parameters[index].value_type.count = 4;
			}
			for(int32 i = 0; i < current->implementation.colors.Count; i++)
			{
				if(shader_postprocess_generic_parameter_exists(shader_tag, current->implementation.colors[i].value_name))
					continue;

				int32 index = Yelo::tag_block_add_element<TagGroups::s_shader_postprocess_parameter>(shader_tag->parameters);
				memset(&shader_tag->parameters[index], 0, sizeof(TagGroups::s_shader_postprocess_parameter));
				shader_tag->parameters[index].SetParameter(&current->implementation.colors[i]);

				shader_tag->parameters[index].value_type.type = Enums::_shader_variable_base_type_argb_color;
				shader_tag->parameters[index].value_type.count = 1;
			}

			if(!current->base_shader.tag_index.IsNull())
				current = Yelo::tag_get<TagGroups::s_shader_postprocess_generic>(current->base_shader.tag_index);
			else
				current = NULL;
		};
		shader_postprocess_generic_remove_variables(shader_tag);
		return true;
	}
};