/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

#include <blamlib/Halo1/tag_files/tag_groups.hpp>
#include <blamlib/Halo1/shaders/shader_definitions.hpp>
#include <YeloLib/Halo1/shaders/shader_postprocess_definitions.hpp>

#include "Tool/BuildCacheFile/TagPreprocess_PostProcessing.inl"
#include "Tool/BuildCacheFile/TagPreprocess_ShaderExtension.inl"

namespace Yelo { namespace Tool {

namespace BuildCacheFileEx
{
	namespace TagPreprocess
	{
#include "Tool/BuildCacheFile/TagPreprocess_ProjectYellow.inl"
		static void PLATFORM_API preprocess_tags_for_build()
		{
			TagGroups::s_tag_iterator tag_iter;
			blam::tag_iterator_new(tag_iter);

			printf_s("pre-processing custom tag instances...\n");

 			datum_index tag_index;
 			while( !(tag_index = blam::tag_iterator_next(tag_iter)).IsNull() )
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
				case TagGroups::project_yellow::k_group_tag:
					ProjectYellow::project_yellow_preprocess(tag_index);
					break;
				}
			}
			puts("done");
		}
	};
};

}; };