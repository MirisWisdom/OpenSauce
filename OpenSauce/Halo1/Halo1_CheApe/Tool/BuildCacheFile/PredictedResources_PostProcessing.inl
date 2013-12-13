/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/

namespace PostProcessing
{
	static bool shader_postprocess_generic_add_predicted_resources(datum_index tag_index)
	{
		auto* shader_tag = blam::tag_get<TagGroups::s_shader_postprocess_generic>(tag_index);
		
		if(shader_tag->shader_code_binary.size == 0 && shader_tag->base_shader.tag_index.IsNull())
		{
			YELO_WARN(
				"\nCheApe: shader_postprocess_generic validity failed on '%s'\n"
				"error: postprocess shader with no binary data also has no base shader defined",
				blam::tag_get_name(tag_index));
			return false;
		}

		for (auto& parameter : shader_tag->parameters)
		{
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