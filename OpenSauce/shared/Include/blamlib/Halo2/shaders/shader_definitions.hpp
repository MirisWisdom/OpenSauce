/*
    Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/Halo2/tag_files/tag_groups.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		struct s_shader_definition
		{
			enum { k_group_tag = 'shad' };

			TAG_FIELD(tag_reference, shader_template, 'stem');
			TAG_FIELD(string_id, material_name);
			TAG_BLOCK(runtime_properties, s_shader_properties);
			word_flags internal_flags;
			TAG_FIELD(word_flags, flags);
			TAG_BLOCK(parameters, s_shader_parameter);
			TAG_BLOCK(postprocess_definition, s_shader_postprocess_definition_new);
			TAG_PAD(int32, 1);
			//TAG_PAD(int32, 3); // useless padding
			TAG_BLOCK(predicted_resources, predicted_resource);
			TAG_FIELD(tag_reference, light_response, 'slit');
			TAG_ENUM(shader_lod_bias);
			TAG_ENUM(specular_type);
			TAG_ENUM(lightmap_type);
			PAD16;
			TAG_FIELD(real, lightmap_specular_brightness);
			TAG_FIELD(real, lightmap_ambient_bias);
			TAG_BLOCK(postprocess_properties, int32);
			TAG_FIELD(real, added_depth_bias_offset);
			TAG_FIELD(real, added_depth_bias_slope_scale);
		}; BOOST_STATIC_ASSERT( sizeof(s_shader_definition) == 0x80 );

		struct s_shader_pass_definition
		{
			enum { k_group_tag = 'spas' };

			TAG_FIELD(tag_data, documentation);
			TAG_BLOCK(parameters, s_shader_pass_parameter);
			PAD16;
			PAD16;
			TAG_BLOCK(implementations, s_shader_pass_implementation);
			TAG_BLOCK(postprocess_definition, s_shader_pass_postprocess_definition_new);
		}; BOOST_STATIC_ASSERT( sizeof(s_shader_pass_definition) == 0x3C );

		struct s_shader_template_definition
		{
			enum { k_group_tag = 'stem' };

			TAG_FIELD(tag_data, documentation);
			TAG_FIELD(string_id, default_material_name);
			word_flags internal_flags;
			TAG_FIELD(word_flags, flags);
			TAG_BLOCK(properties, s_shader_template_property);
			TAG_BLOCK(categories, s_shader_template_category);
			TAG_FIELD(tag_reference, light_response, 'slit');
			TAG_BLOCK(lods, s_shader_template_level_of_detail);
			TAG_PAD(tag_block, 2);
			struct {
				TAG_FIELD(tag_reference, shader, 'shad');
				TAG_ENUM(layer);
				PAD16;
			}aux_shaders[2];
			TAG_BLOCK(postprocess_definition, s_shader_template_postprocess_definition_new);
		}; BOOST_STATIC_ASSERT( sizeof(s_shader_template_definition) == 0x9C );
	};
};