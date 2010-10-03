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
#include "Common/Precompile.hpp"
#include "Tool/BuildCppDefinition.hpp"
#if PLATFORM_ID == PLATFORM_TOOL
#include <Common/Halo1/YeloSharedSettings.hpp>
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
			
			TagSwap(*args->tag_group);
			bool add_boost_asserts = Settings::ParseBoolean(args->add_boost_asserts_cstr);

			// get the tag groups definition
			Yelo::tag_group_definition* tag_group_def = Yelo::tag_group_get(*args->tag_group);
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

				AddBlock(tag_group_def->definition, true, *args->tag_group);
				WriteCppDefinition(file, add_boost_asserts);

				fclose(file);
				printf_s("definition saved as %s\n", file_name);
			}
			else
			{
				union{
					struct{
						tag group;
						PAD8;
					};
					char group_cstr[5];
				};
				group_cstr[4] = '\0';
				group = *args->tag_group;
				TagSwap(group);
				YELO_ERROR(_error_message_priority_warning, "OS_tool: failed to get tag definition for %s\n", group_cstr);
			}
		}
	};
};
#endif