/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

#include <blamlib/Halo1/tag_files/tag_groups.hpp>
#include <YeloLib/Halo1/shaders/shader_postprocess_definitions.hpp>

namespace Yelo { namespace Tool { namespace BuildCacheFileEx { namespace TagPreprocess {

namespace PostProcessing
{
	static void SetQuadToDefaults(datum_index tag_index, TagGroups::s_effect_postprocess_quad_definition& quad)
	{
		if (quad.tessellation.x == 0)
			quad.tessellation.x = 4;
		if (quad.tessellation.y == 0)
			quad.tessellation.y = 4;
		if (quad.x_bounds.upper == 0)
			quad.x_bounds.upper = 1;
		if (quad.y_bounds.upper == 0)
			quad.y_bounds.upper = 1;

		if(quad.x_bounds.lower > quad.x_bounds.upper || quad.y_bounds.lower > quad.y_bounds.upper)
			YELO_WARN("warning: quads lower bound is higher than its upper bound in '%s'", 
				blam::tag_get_name(tag_index));
	}

	template<typename TElement>
	static bool BlockContainsValueName(const TagBlock<TElement>& block, cstring test_string)
	{
		for (auto& element : block)
			if (strncmp(element.value_name, test_string, Enums::k_tag_string_length) == 0)
				return true;

		return false;
	}
	static bool shader_postprocess_generic_group_find_parameter(tag_string& test_string, datum_index tag_index)
	{
		auto* datum = blam::tag_get<TagGroups::s_shader_postprocess_generic>(tag_index);

		while(datum != nullptr)
		{
			TagGroups::s_shader_postprocess_implementation& impl = datum->implementation;

			if (BlockContainsValueName(impl.bitmaps, test_string) ||
				BlockContainsValueName(impl.bools, test_string) ||
				BlockContainsValueName(impl.integers, test_string) ||
				BlockContainsValueName(impl.colors, test_string) ||
				BlockContainsValueName(impl.floats, test_string) ||
				BlockContainsValueName(impl.float2s, test_string) ||
				BlockContainsValueName(impl.float3s, test_string) ||
				BlockContainsValueName(impl.float4s, test_string) ||
				// look through the parameter block in case the shader has already been processed
				BlockContainsValueName(datum->parameters, test_string))
				return true;

			if(!datum->base_shader.tag_index.IsNull())
				datum = blam::tag_get<TagGroups::s_shader_postprocess_generic>(datum->base_shader.tag_index);
			else
				datum = nullptr;
		}

		return false;
	}

	static bool effect_postprocess_generic_group_find_exposed_parameter(tag_string& test_string, datum_index tag_index)
	{
		auto* definition = blam::tag_get<TagGroups::s_effect_postprocess_generic>(tag_index);

		for(int i = 0; i < definition->exposed_parameters.Count; i++)
			if(strcmp(definition->exposed_parameters[i].exposed_name, test_string) == 0)
				return true;
		return false;
	}

	static void shader_postprocess_generic_remove_variables(TagGroups::s_shader_postprocess_generic* shader_tag)
	{
		auto& impl = shader_tag->implementation;
		TagGroups::tag_block_delete_all_elements(impl.bitmaps);
		TagGroups::tag_block_delete_all_elements(impl.bools);
		TagGroups::tag_block_delete_all_elements(impl.integers);
		TagGroups::tag_block_delete_all_elements(impl.floats);
		TagGroups::tag_block_delete_all_elements(impl.float2s);
		TagGroups::tag_block_delete_all_elements(impl.float3s);
		TagGroups::tag_block_delete_all_elements(impl.float4s);
		TagGroups::tag_block_delete_all_elements(impl.colors);
	}

	static bool shader_postprocess_generic_parameter_exists(const TagGroups::s_shader_postprocess_generic* shader_tag, 
		cstring parameter_name)
	{
		return BlockContainsValueName(shader_tag->parameters, parameter_name);
	}

	template<_enum variable_type, int16 var_field_count = 1, typename TElement>
	static void AddPostprocessParametersToImpl(
		TagGroups::s_shader_postprocess_generic* shader_tag,
		const TagBlock<TElement>& src_block)
	{
		auto& params = shader_tag->parameters;

		for (const auto& src_p : src_block)
		{
			if (shader_postprocess_generic_parameter_exists(shader_tag, src_p.value_name))
				continue;

			auto* p = blam::tag_block_add_and_get_element(params);
			p->SetParameter(&src_p);

			p->value_type.type = variable_type;
			p->value_type.count = var_field_count;
		}
	}
	static bool shader_postprocess_generic_preprocess(datum_index tag_index)
	{
		auto* shader_tag = blam::tag_get<TagGroups::s_shader_postprocess_generic>(tag_index);

		auto* current = shader_tag;
		while(current != nullptr)
		{
			auto& impl = current->implementation;

			// add parameters from this tag
			AddPostprocessParametersToImpl<Enums::_shader_variable_base_type_texture>(shader_tag,
				impl.bitmaps);
			AddPostprocessParametersToImpl<Enums::_shader_variable_base_type_boolean>(shader_tag,
				impl.bools);
			AddPostprocessParametersToImpl<Enums::_shader_variable_base_type_integer>(shader_tag,
				impl.integers);
			AddPostprocessParametersToImpl<Enums::_shader_variable_base_type_float>(shader_tag,
				impl.floats);
			AddPostprocessParametersToImpl<Enums::_shader_variable_base_type_float, 2>(shader_tag,
				impl.float2s);
			AddPostprocessParametersToImpl<Enums::_shader_variable_base_type_float, 3>(shader_tag,
				impl.float3s);
			AddPostprocessParametersToImpl<Enums::_shader_variable_base_type_float, 4>(shader_tag,
				impl.float4s);
			AddPostprocessParametersToImpl<Enums::_shader_variable_base_type_argb_color>(shader_tag,
				impl.colors);

			if(!current->base_shader.tag_index.IsNull())
				current = blam::tag_get<TagGroups::s_shader_postprocess_generic>(current->base_shader.tag_index);
			else
				current = nullptr;
		};
		shader_postprocess_generic_remove_variables(shader_tag);
		return true;
	}

	static bool effect_postprocess_generic_preprocess(datum_index tag_index)
	{
		auto* definition = blam::tag_get<TagGroups::s_effect_postprocess_generic>(tag_index);

		int i = 0;
		// set quad to defaults
		SetQuadToDefaults(tag_index, definition->quad_definition);

		// check the shaders block has at least one element with a shader referenced
		if(!definition->shaders.Count)
		{
			YELO_ERROR_CRITICAL(
				"error: an effect_postprocess_generic has no shaders referenced\ntag: %s", blam::tag_get_name(tag_index));
			return false;
		}

		for (auto& shader : definition->shaders)
			if(shader.tag_index.IsNull())
			{
				YELO_ERROR_CRITICAL(
					"error: an effect_postprocess_generic has a shader block that does not reference a shader\ntag: %s", 
					blam::tag_get_name(tag_index));
				return false;
			}

		// check the shader indices are valid
		for (auto index : definition->shader_indices)
			if (index >= definition->shaders.Count || index == NONE)
			{
				YELO_ERROR_CRITICAL(
					"error: an effect_postprocess_generic has a shader_index with an invalid value\ntag: %s", 
					blam::tag_get_name(tag_index));
				return false;
			}

		// check the exposed variables exist in the linked shader
		for (auto& parameter : definition->exposed_parameters)
		{
			if(parameter.shader_index == NONE)
			{
				YELO_ERROR_CRITICAL(
					"error: an effect_postprocess_generic has an exposed parameter with no shader referenced\ntag: %s", 
					blam::tag_get_name(tag_index));
				return false;
			}

			if(!shader_postprocess_generic_group_find_parameter(parameter.parameter_name, 
				definition->shaders[definition->shader_indices[parameter.shader_index]].tag_index))
			{
				YELO_ERROR_CRITICAL(
					"error: a effect_postprocess_generic is exposing a parameter that does not exist\ntag: %s", 
					blam::tag_get_name(tag_index));
				return false;
			}
		}
		return true;
	}

	static void effect_postprocess_collection_add_shader_block(TagGroups::s_effect_postprocess_collection* definition, tag_reference& reference)
	{
		for (auto& shader : definition->shaders)
			if (shader.tag_index == reference.tag_index)
				return;

		auto* shader = blam::tag_block_add_and_get_element(definition->shaders);
		shader->tag_index = reference.tag_index;
		blam::tag_reference_set(*shader, reference.group_tag, reference.name);
	}

	static bool effect_postprocess_collection_preprocess(datum_index tag_index)
	{
		auto* collection = blam::tag_get<TagGroups::s_effect_postprocess_collection>(tag_index);

		int i = 0;
		// add used shaders to the shaders block
		for (auto& effect : collection->effects)
		{
			// ensure each effect block references an effect
			if(effect.effect.tag_index.IsNull())
			{
				YELO_ERROR_CRITICAL(
					"error: an effect_postprocess_collection has an effect block with no effect referenced\ntag: %s", 
					blam::tag_get_name(tag_index));
				return false;
			}

			for (auto& variable : effect.script_variables)
			{
				if(!effect_postprocess_generic_group_find_exposed_parameter(variable.exposed_parameter_name, effect.effect.tag_index))
				{
					YELO_ERROR_CRITICAL(
						"error: an effect_postprocess_collection has a scripted variable without a valid exposed variable\ntag: %s", 
						blam::tag_get_name(tag_index));
					return false;
				}

				// the variable name is forced to lower case as Halo script is compiled that way
				std::string name(variable.script_variable_name);

				for( auto& c : name )
					c = tolower(c);
				strcpy_s(variable.script_variable_name, name.c_str());
			}

			auto* definition = blam::tag_get<TagGroups::s_effect_postprocess_generic>(effect.effect.tag_index);

			for (auto& shader : definition->shaders)
				effect_postprocess_collection_add_shader_block(collection, shader);
		}

		// verify exposed parameters exist in effects
		for (auto& instance : collection->effect_instances)
		{
			if (instance.effect_index >= collection->effects.Count || instance.effect_index == NONE)
			{
				YELO_ERROR_CRITICAL("error: effect instance \"%s\" has an invalid effect index",
					instance.name);
				return false;
			}

			// process the quads for default values
			SetQuadToDefaults(tag_index, instance.quad_definition);
		}

		return true;
	}
};

}; }; }; };