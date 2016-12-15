/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Tool/BuildCppDefinition.hpp"
#if PLATFORM_TYPE == PLATFORM_TOOL
#include <YeloLib/Halo1/open_sauce/settings/yelo_shared_settings.hpp>
#include <YeloLib/cseries/value_conversion.hpp>

#include "Common/StringEditing.hpp"
#include "Engine/EngineFunctions.hpp"
#include "TagGroups/TagGroups.hpp"

namespace Yelo
{
	namespace Tool
	{
#include "Tool/BuildCppDefinition.inl"

		void PLATFORM_API build_cpp_definition(char* arguments[])
		{
			struct s_arguments {
				const tag* group_tag;
				cstring add_boost_asserts_cstr;
			}* args = CAST_PTR(s_arguments*, arguments);
			
			tag group_tag = *args->group_tag;
			TagGroups::TagSwap(group_tag);

			bool add_boost_asserts = true;
			ValueConversion::FromString(args->add_boost_asserts_cstr, add_boost_asserts);

			// get the tag groups definition
			tag_group* tag_group_def = blam::tag_group_get(group_tag);
			if(tag_group_def)
			{
				printf_s("creating c++ definition of %s\n", tag_group_def->name);

				char file_name[MAX_PATH];
				sprintf_s(file_name, "%s.hpp", tag_group_def->name);
				FILE* file;
				fopen_s(&file, file_name, "w+");

				if(!file || ferror(file))
				{
					if(file)
						fclose(file);
					YELO_WARN("OS_tool: failed to create file %s\n", 
						file_name);
					return;
				}

				WriteCppDefinition(file, tag_group_def->header_block_definition, group_tag, add_boost_asserts);

				fclose(file);
				printf_s("definition saved as %s\n", file_name);
			}
			else
			{
				YELO_WARN("OS_tool: failed to get tag definition for %s\n",
					TagGroups::group_tag_to_string{group_tag}.ToString());
			}
		}
	};
};
#endif