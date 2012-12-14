/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Tool/BuildCppDefinition.hpp"
#if PLATFORM_ID == PLATFORM_TOOL
#include <YeloLib/Halo1/open_sauce/settings/yelo_shared_settings.hpp>
#include "TagGroups/TagGroups.hpp"
#include "Common/StringEditing.hpp"

namespace Yelo
{
	namespace Tool
	{
#include "Tool/BuildCppDefinition.inl"

		void PLATFORM_API build_cpp_definition(void** arguments)
		{
			struct s_arguments {
				tag* tag_group;
				cstring add_boost_asserts_cstr;
			}* args = CAST_PTR(s_arguments*, arguments);
			
			TagGroups::TagSwap(*args->tag_group);
			bool add_boost_asserts = Settings::ParseBoolean(args->add_boost_asserts_cstr);

			// get the tag groups definition
			Yelo::tag_group* tag_group_def = Yelo::tag_group_get(*args->tag_group);
			if(tag_group_def)
			{
				printf_s("creating c++ definition of %s\n", tag_group_def->name);

				char file_name[MAX_PATH];
				sprintf_s(file_name, MAX_PATH, "%s.hpp", tag_group_def->name);
				FILE* file;
				fopen_s(&file, file_name, "w+");

				if(!file || ferror(file))
				{
					if(file)
						fclose(file);
					YELO_ERROR(_error_message_priority_warning, 
						"OS_tool: failed to create file %s\n", file_name);
					return;
				}

				WriteCppDefinition(file, tag_group_def->header_block_definition, *args->tag_group, add_boost_asserts);

				fclose(file);
				printf_s("definition saved as %s\n", file_name);
			}
			else
			{
				TagGroups::group_tag_to_string gt; gt.Terminate();
				gt.group = *args->tag_group;
				gt.TagSwap();

				YELO_ERROR(_error_message_priority_warning, "OS_tool: failed to get tag definition for %s\n", gt.str);
			}
		}
	};
};
#endif