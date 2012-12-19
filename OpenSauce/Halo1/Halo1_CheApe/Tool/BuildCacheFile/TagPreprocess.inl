/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

namespace BuildCacheFileEx
{
	namespace TagPreprocess
	{
#include "Tool/BuildCacheFile/TagPreprocess_PostProcessing.inl"
#include "Tool/BuildCacheFile/TagPreprocess_ShaderExtension.inl"

		static void PLATFORM_API preprocess_tags_for_build()
		{
			TagGroups::tag_iterator tag_iter;
			tag_iterator_new(tag_iter);

			printf_s("pre-processing custom tag instances...\n");

 			datum_index tag_index;
 			while( !(tag_index = tag_iterator_next(tag_iter)).IsNull() )
 			{
				switch( tag_get_group_tag(tag_index) )
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
				}
			}
			puts("done");
		}
	};
};