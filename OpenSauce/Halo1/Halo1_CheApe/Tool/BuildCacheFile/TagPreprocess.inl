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
				switch( (*TagGroups::TagInstances())[tag_index]->group_tag )
				{
				case TagGroups::s_shader_postprocess_generic::k_group_tag:
					PostProcessing::shader_postprocess_generic_setup_parameters(tag_index);
					break;
				case TagGroups::s_shader_postprocess_collection::k_group_tag:
					PostProcessing::shader_postprocess_collection_process_collection(tag_index);
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