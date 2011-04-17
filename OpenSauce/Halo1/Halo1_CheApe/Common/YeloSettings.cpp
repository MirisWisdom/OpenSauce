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
#include "Common/YeloSettings.hpp"

#include <Shellapi.h>

#include "Engine/EngineFunctions.hpp"

namespace Yelo
{
	namespace Settings
	{
		static s_settings global_settings;
		const s_settings& Get()	{ return global_settings; }

		void s_profile::Parse(TiXmlElement* element)
		{
			is_valid = true;

			cstring value = element->Attribute("ignoredByGuerilla");
			ignored_by[0] = Settings::ParseBoolean(value);

			value = element->Attribute("ignoredByTool");
			ignored_by[1] = Settings::ParseBoolean(value);

			value = element->Attribute("ignoredBySapien");
			ignored_by[2] = Settings::ParseBoolean(value);

			is_ignored = ignored_by[0] || ignored_by[1] || ignored_by[2];

			// Parse the affinity mask which the tools will use
			value = element->Attribute("affinityMask");
			if(has_affinity_mask = (value != NULL))
			{
				// We expect a base 16 number
				unsigned long mask = strtoul(value, NULL, 16);

				// If there was an overflow, don't record the value
				if(errno == ERANGE)
					has_affinity_mask = false;
				else
					affinity_mask = mask;
			}

			TiXmlElement* path_element = element->FirstChildElement("root");
			if(path_element != NULL)
			{
				value = path_element->GetText();
				if(is_valid = (value != NULL))
				{
					strcpy_s(root_path, value);
					PathAppendA(root_path, "\\");

					strcpy_s(paths.data, root_path);
					strcpy_s(paths.maps, root_path);
					strcpy_s(paths.tags, root_path);

					PathAppendA(paths.data, "data\\");
					PathAppendA(paths.maps, "maps\\");
					// Finish tags path in the code below...
				}
			}

			// Parse Tool specific settings
			{ TiXmlElement* tool_element = element->FirstChildElement("tool");
				if(tool_element != NULL)
				{
					value = element->Attribute("baseAddressOverride");

					// We expect a base 16 number
					unsigned long addr = strtoul(value, NULL, 16);

					tool.base_adddress_override = errno == ERANGE ? 0 : addr;
				}
			}

			if(is_valid)
			{
				path_element = element->FirstChildElement("data");
				if(path_element != NULL && (value = path_element->GetText()) != NULL)
				{
					strcpy_s(paths.data, value);
					PathAppendA(paths.data, "\\");
				}

				// We don't process a "maps" path element anymore, that's now just based on "root"

				path_element = element->FirstChildElement("tagsName");
				// If no explicit name is given, assume the default of "tags" is the name
				value = path_element != NULL ? path_element->GetText() :
					"tags";

				// Finish tags path here
				if(is_valid = (value != NULL && value[0] != '\0'))
				{
					strcpy_s(paths.tags_folder_name, value);
					PathAppendA(paths.tags_folder_name, "\\");

					// [tags_folder_name] already has the directory separator appended
					PathAppendA(paths.tags, paths.tags_folder_name);
				}
			}


			// If we failed to parse the profile, set everything to ignore
			if(!is_valid)
				is_ignored = ignored_by[0] = ignored_by[1] = ignored_by[2] = true;
		}

		// Enumerate the child elements of [element]'s "Profile" looking for an element named 
		// [profile_name].
		// Returns NULL if one isn't found
		static TiXmlElement* FindProfileByName(TiXmlElement* element, cstring profile_name)
		{
			TiXmlElement* profile_element = element->FirstChildElement("Profile");
			while(profile_element != NULL)
			{
				cstring name = profile_element->Attribute("name");
				if(name != NULL && !_stricmp(profile_name, name))
					break;

				profile_element = profile_element->NextSiblingElement();
			}

			return profile_element;
		}
		void s_settings::Parse(TiXmlElement* element, cstring arg_profile, cstring default_profile)
		{
			TiXmlElement* profile_element = NULL;

			// Try to find the command line profile name or the default if nothing is specified on the command line
			if(arg_profile != NULL)		profile_element = FindProfileByName(element, arg_profile);
			if(profile_element == NULL)	profile_element = FindProfileByName(element, default_profile);

			if(profile_element != NULL)
				global_settings.active_profile.Parse(profile_element);
		}


		void Initialize()
		{
			Yelo::Settings::SharedInitialize("");

			LoadSettings();
		}

		void Dispose()
		{
			Yelo::Settings::SharedDispose();
		}

		// Parse the command line for the editor profile option and copy the result to 
		// [profile_name].
		// Sets [profile_name] to an empty string if an option isn't found.
		static void GetCommandLineDefaultProfile(char profile_name[64])
		{
#define CMDLINE_ARG_EDITOR_PROFILE L"-editorProfile:"
			profile_name[0] = '\0';

			int cmds_count = 0;
			LPWSTR* cmds = CommandLineToArgvW(GetCommandLineW(), &cmds_count);

			for(int x = 0; x < cmds_count; x++)
			{
				wcstring cmd = wcsstr(cmds[x], CMDLINE_ARG_EDITOR_PROFILE);

				if(cmd != NULL)
				{
					const size_t k_parameter_length = NUMBEROF(CMDLINE_ARG_EDITOR_PROFILE)-1;
					cmd = cmd + k_parameter_length;

					const size_t length = wcslen(cmd);

					if(length > 0)
						wstring_to_string(profile_name, NUMBEROF_C(profile_name)-1, cmd);
				}
			}

			LocalFree(cmds);
#undef CMDLINE_ARG_EDITOR_PROFILE
		}

		void LoadSettings()
		{
			char file_path[MAX_PATH];
			strcpy_s(file_path, "OS_Settings.Editor.xml");

			TiXmlDocument xml_settings = TiXmlDocument();
			TiXmlElement* root_element = GenericSettingsParse(xml_settings, file_path, NULL);

			if(root_element != NULL)
			{
				char profile_name[64];
				GetCommandLineDefaultProfile(profile_name);

				// If no option is found, we'll try to fall back on the default profile attribute value. 
				// If neither value exists then no profile will be used!
				cstring default_profile = root_element->Attribute("defaultProfile");

				TiXmlElement* profiles_element = root_element->FirstChildElement("profiles");
				if(profiles_element != NULL && default_profile != NULL)
					global_settings.Parse(profiles_element, 
						profile_name[0] != '\0' ? profile_name : NULL,
						default_profile);
			}
		}

		void SaveSettings()
		{
		}
	};
};