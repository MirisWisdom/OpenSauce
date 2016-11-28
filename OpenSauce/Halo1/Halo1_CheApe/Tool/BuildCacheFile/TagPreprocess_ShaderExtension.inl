/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/

#include <blamlib/Halo1/tag_files/tag_groups.hpp>
#include <blamlib/Halo1/shaders/shader_definitions.hpp>
#include <blamlib/Halo1/effects/weather_particle_system_definitions.hpp>

namespace Yelo { namespace Tool { namespace BuildCacheFileEx { namespace TagPreprocess {

namespace ShaderExtension
{
	static bool shader_model_extension_process(datum_index tag_index)
	{
		auto* shader_tag = blam::tag_get<TagGroups::s_shader_model_definition>(tag_index);
		
		// Default to using DLM's
		shader_tag->shader.extension_usage = Flags::_shader_extension_usage_directional_lightmaps_diff;

		if(shader_tag->model.maps.shader_extension.Count != 1)
			return true;

		TagGroups::s_shader_model_extension& extension = shader_tag->model.maps.shader_extension[0];

		// Use normal maps if one is available, and default 0 bump to full bump
		if(!extension.base_normal.map.tag_index.IsNull())
			shader_tag->shader.extension_usage |= Flags::_shader_extension_usage_normal_map;
		if(	extension.base_normal.modifiers.coefficient == 0.0f)
			extension.base_normal.modifiers.coefficient =  1.0f;

		// Use detail normal maps if set and default 0 values to 1
		for (auto& detail_normal : extension.detail_normals)
		{
			if(!detail_normal.map.tag_index.IsNull())
				shader_tag->shader.extension_usage |= Flags::_shader_extension_usage_detail_normal;
			if(	detail_normal.modifiers.coefficient == 0.0f)
				detail_normal.modifiers.coefficient =  1.0f;
			if(	detail_normal.modifiers.scale == 0.0f)
				detail_normal.modifiers.scale =  1.0f;
			if(	detail_normal.modifiers.v_scale == 0.0f)
				detail_normal.modifiers.v_scale =  1.0f;
		}
		
		// Use specular map if set and default 0 values to 1
		if(!extension.specular_color.map.tag_index.IsNull())
			shader_tag->shader.extension_usage |= Flags::_shader_extension_usage_specular_map;
		if(	extension.specular_color.modifiers.coefficient == 0.0f)
			extension.specular_color.modifiers.coefficient =  1.0f;
		if(	extension.specular_color.modifiers.exponent == 0.0f)
			extension.specular_color.modifiers.exponent =  1.0f;
		
		// Use specular lighting if exponent set and default 0 coefficient to 1
		if(	extension.specular_lighting_exponent != 0.0f)
			shader_tag->shader.extension_usage |= Flags::_shader_extension_usage_specular_lighting | Flags::_shader_extension_usage_directional_lightmaps_spec;
		if(	extension.specular_lighting_coefficient == 0.0f)
			extension.specular_lighting_coefficient =  1.0f;
		
		// Disable DLM's for this shader if set
		if( extension.diffuse_lighting_flags.do_not_use_dlms_bsp_bit)
		{
			shader_tag->shader.extension_usage &= ~Flags::_shader_extension_usage_directional_lightmaps_diff;
			shader_tag->shader.extension_usage &= ~Flags::_shader_extension_usage_directional_lightmaps_spec;
		}
		return true;
	}

	static bool shader_environment_extension_process(datum_index tag_index)
	{
		auto* shader_tag = blam::tag_get<TagGroups::s_shader_environment_definition>(tag_index);

		// Default to using DLM's
		shader_tag->shader.extension_usage = Flags::_shader_extension_usage_directional_lightmaps_diff;
		
		// If the shader has a normal map use it in the shader extension
		if(!shader_tag->environment.bump.bump_map.map.tag_index.IsNull())
		{
			shader_tag->shader.extension_usage |= Flags::_shader_extension_usage_normal_map;
		}

		if(shader_tag->environment.shader_extension.Count != 1)
			return true;

		auto& extension = shader_tag->environment.shader_extension[0];

		// Default 0 bump to full bump
		if(extension.bump_amount == 0.0f)
			extension.bump_amount = 1.0f;
		
		// Use specular map if set and default 0 values to 1
		if(!extension.specular_color_map.tag_index.IsNull())
			shader_tag->shader.extension_usage |= Flags::_shader_extension_usage_specular_map;
		if(	extension.specular_color_coefficient == 0.0f)
			extension.specular_color_coefficient =  1.0f;
		if(	extension.specular_color_exponent == 0.0f)
			extension.specular_color_exponent =  1.0f;

		// Use specular lighting if exponent set and default 0 coefficient to 1
		if(extension.specular_lighting_exponent != 0.0f)
			shader_tag->shader.extension_usage |= Flags::_shader_extension_usage_directional_lightmaps_spec;
		if(extension.specular_lighting_coefficient == 0.0f)
			extension.specular_lighting_coefficient =  1.0f;

		// Disable DLM use if set
		if(extension.flags.do_not_use_dlms_bit)
		{
			shader_tag->shader.extension_usage &= ~Flags::_shader_extension_usage_directional_lightmaps_diff;
			shader_tag->shader.extension_usage &= ~Flags::_shader_extension_usage_directional_lightmaps_spec;
		}
		return true;
	}

	static bool shader_effect_extension_process(TagGroups::s_shader_effect& shader)
	{
		if(shader.effect.shader_extension.Count != 1)
		{
			return true;
		}

		auto& extension = shader.effect.shader_extension[0];
		auto& base_shader = shader.shader;

		// If using camera distance fade or depth fade set the depth fade flag
		if((extension.depth_fade_distance > 0) || (extension.camera_fade_distance > 0))
		{
			base_shader.extension_usage |= Flags::_shader_extension_usage_depth_fade;
		}

		return true;
	}
	
	template<typename T>
	static bool shader_effect_extension_process(datum_index tag_index)
	{
		auto* container_tag = blam::tag_get<T>(tag_index);

		return shader_effect_extension_process(container_tag->shader_effect);
	}
	
	template<>
	static bool shader_effect_extension_process<TagGroups::s_weather_particle_system_definition>(datum_index tag_index)
	{
		auto* container_tag = blam::tag_get<TagGroups::s_weather_particle_system_definition>(tag_index);

		bool success = false;
		for(auto& particle : container_tag->particle_types)
		{
			success &= shader_effect_extension_process(particle.shader_effect);
		}
		return success;
	}
	
	template<>
	static bool shader_effect_extension_process<TagGroups::s_particle_system_definition>(datum_index tag_index)
	{
		auto* container_tag = blam::tag_get<TagGroups::s_particle_system_definition>(tag_index);

		bool success = false;
		for(auto& particle_type : container_tag->particle_types)
		{
			for(auto& particle_state : particle_type.particle_states)
			{
				success &= shader_effect_extension_process(particle_state.shader_effect);
			}
		}
		return success;
	}
};
}; }; }; };