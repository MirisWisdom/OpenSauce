/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

namespace PostProcessing
{
	static void SetQuadToDefaults(TagGroups::s_effect_postprocess_quad_definition& quad)
	{
		if (quad.tessellation.x == 0)
			quad.tessellation.x = 4;
		if (quad.tessellation.y == 0)
			quad.tessellation.y = 4;
		if (quad.x_bounds.upper == 0)
			quad.x_bounds.upper = 1;
		if (quad.y_bounds.upper == 0)
			quad.y_bounds.upper = 1;

		if((quad.x_bounds.lower > quad.x_bounds.upper) || (quad.y_bounds.lower > quad.y_bounds.upper))
			YELO_ERROR(_error_message_priority_warning, "warning: a post processing quads lower bound is higher than its upper bound");
	}

	static bool shader_postprocess_generic_group_find_parameter(tag_string& test_string, Yelo::datum_index tag_index)
	{
		TagGroups::s_shader_postprocess_generic* datum = Yelo::tag_get<TagGroups::s_shader_postprocess_generic>(tag_index);

		while(datum != NULL)
		{
			int32 i = 0;

			TagGroups::s_shader_postprocess_implementation& impl = datum->implementation;

			for(i = 0; i < impl.bitmaps.Count; i++)
				if(strncmp(impl.bitmaps[i].value_name, test_string, Enums::k_tag_string_length) == 0)
					return true;

			for(i = 0; i < impl.bools.Count; i++)
				if(strncmp(impl.bools[i].value_name, test_string, Enums::k_tag_string_length) == 0)
					return true;

			for(i = 0; i < impl.integers.Count; i++)
				if(strncmp(impl.integers[i].value_name, test_string, Enums::k_tag_string_length) == 0)
					return true;

			for(i = 0; i < impl.colors.Count; i++)
				if(strncmp(impl.colors[i].value_name, test_string, Enums::k_tag_string_length) == 0)
					return true;

			for(i = 0; i < impl.floats.Count; i++)
				if(strncmp(impl.floats[i].value_name, test_string, Enums::k_tag_string_length) == 0)
					return true;

			for(i = 0; i < impl.float2s.Count; i++)
				if(strncmp(impl.float2s[i].value_name, test_string, Enums::k_tag_string_length) == 0)
					return true;

			for(i = 0; i < impl.float3s.Count; i++)
				if(strncmp(impl.float3s[i].value_name, test_string, Enums::k_tag_string_length) == 0)
					return true;

			for(i = 0; i < impl.float4s.Count; i++)
				if(strncmp(impl.float4s[i].value_name, test_string, Enums::k_tag_string_length) == 0)
					return true;

			// look through the parameter block in case the shader has already been processed
			for(i = 0; i < datum->parameters.Count; i++)
				if(strncmp(datum->parameters[i].value_name, test_string, Enums::k_tag_string_length) == 0)
					return true;

			if(!datum->base_shader.tag_index.IsNull())
				datum = Yelo::tag_get<TagGroups::s_shader_postprocess_generic>(datum->base_shader.tag_index);
			else
				datum = NULL;
		}

		return false;
	}

	static bool effect_postprocess_generic_group_find_exposed_parameter(tag_string& test_string, Yelo::datum_index tag_index)
	{
		TagGroups::s_effect_postprocess_generic* definition = Yelo::tag_get<TagGroups::s_effect_postprocess_generic>(tag_index);

		for(int i = 0; i < definition->exposed_parameters.Count; i++)
			if(strcmp(definition->exposed_parameters[i].exposed_name, test_string) == 0)
				return true;
		return false;
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

	static bool shader_postprocess_generic_preprocess(datum_index tag_index)
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
				shader_tag->parameters[index].SetParameter(&current->implementation.bitmaps[i]);

				shader_tag->parameters[index].value_type.type = Enums::_shader_variable_base_type_texture;
				shader_tag->parameters[index].value_type.count = 1;
			}
			for(int32 i = 0; i < current->implementation.bools.Count; i++)
			{
				if(shader_postprocess_generic_parameter_exists(shader_tag, current->implementation.bools[i].value_name))
					continue;

				int32 index = Yelo::tag_block_add_element<TagGroups::s_shader_postprocess_parameter>(shader_tag->parameters);
				shader_tag->parameters[index].SetParameter(&current->implementation.bools[i]);

				shader_tag->parameters[index].value_type.type = Enums::_shader_variable_base_type_boolean;
				shader_tag->parameters[index].value_type.count = 1;
			}
			for(int32 i = 0; i < current->implementation.integers.Count; i++)
			{
				if(shader_postprocess_generic_parameter_exists(shader_tag, current->implementation.integers[i].value_name))
					continue;

				int32 index = Yelo::tag_block_add_element<TagGroups::s_shader_postprocess_parameter>(shader_tag->parameters);
				shader_tag->parameters[index].SetParameter(&current->implementation.integers[i]);

				shader_tag->parameters[index].value_type.type = Enums::_shader_variable_base_type_integer;
				shader_tag->parameters[index].value_type.count = 1;
			}
			for(int32 i = 0; i < current->implementation.floats.Count; i++)
			{
				if(shader_postprocess_generic_parameter_exists(shader_tag, current->implementation.floats[i].value_name))
					continue;

				int32 index = Yelo::tag_block_add_element<TagGroups::s_shader_postprocess_parameter>(shader_tag->parameters);
				shader_tag->parameters[index].SetParameter(&current->implementation.floats[i]);

				shader_tag->parameters[index].value_type.type = Enums::_shader_variable_base_type_float;
				shader_tag->parameters[index].value_type.count = 1;
			}
			for(int32 i = 0; i < current->implementation.float2s.Count; i++)
			{
				if(shader_postprocess_generic_parameter_exists(shader_tag, current->implementation.float2s[i].value_name))
					continue;

				int32 index = Yelo::tag_block_add_element<TagGroups::s_shader_postprocess_parameter>(shader_tag->parameters);
				shader_tag->parameters[index].SetParameter(&current->implementation.float2s[i]);

				shader_tag->parameters[index].value_type.type = Enums::_shader_variable_base_type_float;
				shader_tag->parameters[index].value_type.count = 2;
			}
			for(int32 i = 0; i < current->implementation.float3s.Count; i++)
			{
				if(shader_postprocess_generic_parameter_exists(shader_tag, current->implementation.float3s[i].value_name))
					continue;

				int32 index = Yelo::tag_block_add_element<TagGroups::s_shader_postprocess_parameter>(shader_tag->parameters);
				shader_tag->parameters[index].SetParameter(&current->implementation.float3s[i]);

				shader_tag->parameters[index].value_type.type = Enums::_shader_variable_base_type_float;
				shader_tag->parameters[index].value_type.count = 3;
			}
			for(int32 i = 0; i < current->implementation.float4s.Count; i++)
			{
				if(shader_postprocess_generic_parameter_exists(shader_tag, current->implementation.float4s[i].value_name))
					continue;

				int32 index = Yelo::tag_block_add_element<TagGroups::s_shader_postprocess_parameter>(shader_tag->parameters);
				shader_tag->parameters[index].SetParameter(&current->implementation.float4s[i]);

				shader_tag->parameters[index].value_type.type = Enums::_shader_variable_base_type_float;
				shader_tag->parameters[index].value_type.count = 4;
			}
			for(int32 i = 0; i < current->implementation.colors.Count; i++)
			{
				if(shader_postprocess_generic_parameter_exists(shader_tag, current->implementation.colors[i].value_name))
					continue;

				int32 index = Yelo::tag_block_add_element<TagGroups::s_shader_postprocess_parameter>(shader_tag->parameters);
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

	static bool effect_postprocess_generic_preprocess(datum_index tag_index)
	{
		TagGroups::s_effect_postprocess_generic* definition = Yelo::tag_get<TagGroups::s_effect_postprocess_generic>(tag_index);

		int i = 0;
		// set quad to defaults
		SetQuadToDefaults(definition->quad_definition);

		// check the shaders block has at least one element with a shader referenced
		if(!definition->shaders.Count)
		{
			YELO_ERROR(_error_message_priority_critical,
				"error: an effect_postprocess_generic has no shaders referenced\ntag: %s", (*TagGroups::TagInstances())[tag_index]->filename);
			return false;
		}

		for(i = 0; i < definition->shaders.Count; i++)
			if(definition->shaders[i].tag_index.IsNull())
			{
				YELO_ERROR(_error_message_priority_critical,
					"error: an effect_postprocess_generic has a shader block that does not reference a shader\ntag: %s", (*TagGroups::TagInstances())[tag_index]->filename);
				return false;
			}

		// check the shader indices are valid
		for(i = 0; i < definition->shader_indices.Count; i++)
			if(definition->shader_indices[i] >= definition->shaders.Count)
			{
				YELO_ERROR(_error_message_priority_critical,
					"error: an effect_postprocess_generic has a shader_index with an invalid value\ntag: %s", (*TagGroups::TagInstances())[tag_index]->filename);
				return false;
			}

		// check the exposed variables exist in the linked shader
		for(i = 0; i < definition->exposed_parameters.Count; i++)
		{
			TagGroups::s_effect_postprocess_generic_exposed_parameter& parameter = definition->exposed_parameters[i];
			if(!shader_postprocess_generic_group_find_parameter(parameter.parameter_name, definition->shaders[definition->shader_indices[parameter.shader_index]].tag_index))
			{
				YELO_ERROR(_error_message_priority_critical,
					"error: a effect_postprocess_generic is exposing a parameter that does not exist\ntag: %s", (*TagGroups::TagInstances())[tag_index]->filename);
				return false;
			}
		}
		return true;
	}

	static void effect_postprocess_collection_add_shader_block(TagGroups::s_effect_postprocess_collection* definition, tag_reference& reference)
	{
		for(int i = 0; i < definition->shaders.Count; i++)
			if(definition->shaders[i].tag_index == reference.tag_index)
				return;

		int32 block = Yelo::tag_block_add_element(definition->shaders);
		definition->shaders[block].tag_index = reference.tag_index;
		Yelo::tag_reference_set(&definition->shaders[block], reference.group_tag, reference.name);
	}

	static bool effect_postprocess_collection_preprocess(datum_index tag_index)
	{
		TagGroups::s_effect_postprocess_collection* collection = Yelo::tag_get<TagGroups::s_effect_postprocess_collection>(tag_index);

		int i = 0;
		// add used shaders to the shaders block
		for(i = 0; i < collection->effects.Count; i++)
		{
			TagGroups::s_effect_postprocess_collection_effect& effect = collection->effects[i];

			int j = 0;
			// ensure each effect block references an effect
			if(effect.effect.tag_index.IsNull())
			{
				YELO_ERROR(_error_message_priority_critical,
					"error: an effect_postprocess_collection has an effect block with no effect referenced\ntag: %s", (*TagGroups::TagInstances())[tag_index]->filename);
				return false;
			}

			for(j = 0; j < effect.script_variables.Count; j++)
				if(!effect_postprocess_generic_group_find_exposed_parameter(effect.script_variables[j].exposed_parameter_name, effect.effect.tag_index))
				{
					YELO_ERROR(_error_message_priority_critical,
					"error: an effect_postprocess_collection has a scripted variable without a valid exposed variable\ntag: %s", (*TagGroups::TagInstances())[tag_index]->filename);
					return false;
				}

			TagGroups::s_effect_postprocess_generic* definition = Yelo::tag_get<TagGroups::s_effect_postprocess_generic>(effect.effect.tag_index);

			for(j = 0; j < definition->shaders.Count; j++)
				effect_postprocess_collection_add_shader_block(collection, definition->shaders[j]);
		}

		// verify exposed parameters exist in effects
		for(int i = 0; i < collection->effect_instances.Count; i++)
		{
			TagGroups::s_effect_postprocess_generic_effect_instance& instance = collection->effect_instances[i];

			// check the effect index is valid
			if(instance.effect_index >= collection->effects.Count)
			{
				YELO_ERROR(_error_message_priority_critical, "error: effect instance \"%s\" has an invalid effect index", instance.name);
				return false;
			}

			// process the quads for default values
			SetQuadToDefaults(instance.quad_definition);
		}
		return true;
	}
};