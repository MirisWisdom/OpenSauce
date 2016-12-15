/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

#include <blamlib/Halo1/tag_files/tag_groups.hpp>
#include <blamlib/Halo1/shaders/shader_definitions.hpp>
#include <YeloLib/Halo1/shaders/shader_postprocess_definitions.hpp>
#include <blamlib/Halo1/effects/contrail_definitions.hpp>
#include <blamlib/Halo1/effects/particle_definitions.hpp>
#include <blamlib/Halo1/effects/weather_particle_system_definitions.hpp>
#include <blamlib/Halo1/effects/particle_system_definitions.hpp>

#include "Tool/BuildCacheFile/TagPreprocess_PostProcessing.inl"
#include "Tool/BuildCacheFile/TagPreprocess_ShaderExtension.inl"
#include "Tool/BuildCacheFile/TagPreprocess_Scenario.inl"

namespace Yelo { namespace Tool {

namespace BuildCacheFileEx
{
	namespace TagPreprocess
	{
		void preprocess_tags_for_build()
		{
			printf_s("pre-processing custom tag instances...\n");

			for (auto tag_index : TagGroups::c_tag_iterator::all())
 			{
				switch( blam::tag_get_group_tag(tag_index) )
				{
				case TagGroups::s_shader_postprocess_generic::k_group_tag:
					PostProcessing::shader_postprocess_generic_preprocess(tag_index);
					break;
				case TagGroups::s_effect_postprocess_generic::k_group_tag:
					PostProcessing::effect_postprocess_generic_preprocess(tag_index);
					break;
				case TagGroups::s_effect_postprocess_collection::k_group_tag:
					PostProcessing::effect_postprocess_collection_preprocess(tag_index);
					break;
				case TagGroups::s_shader_model_definition::k_group_tag:
					ShaderExtension::shader_model_extension_process(tag_index);
					break;
				case TagGroups::s_shader_environment_definition::k_group_tag:
					ShaderExtension::shader_environment_extension_process(tag_index);
					break;
				case TagGroups::s_contrail_definition::k_group_tag:
					ShaderExtension::shader_effect_extension_process<TagGroups::s_contrail_definition>(tag_index);
					break;
				case TagGroups::s_particle_definition::k_group_tag:
					ShaderExtension::shader_effect_extension_process<TagGroups::s_particle_definition>(tag_index);
					break;
				case TagGroups::s_weather_particle_system_definition::k_group_tag:
					ShaderExtension::shader_effect_extension_process<TagGroups::s_weather_particle_system_definition>(tag_index);
					break;
				case TagGroups::s_particle_system_definition::k_group_tag:
					ShaderExtension::shader_effect_extension_process<TagGroups::s_particle_system_definition>(tag_index);
					break;
				case TagGroups::scenario::k_group_tag:
					Scenario::scenario_preprocess(tag_index);
					break;
				}
			}
			puts("done");
		}
	};
};

}; };