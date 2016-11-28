/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Settings/Settings.hpp"

#include <blamlib/Halo1/cache/cache_files.hpp>
#include <blamlib/Halo1/tag_files/tag_files.hpp>

#include <Shellapi.h>

#include "Engine/EngineFunctions.hpp"
#include "Rasterizer/Rasterizer.hpp"

#include <YeloLib/Halo1/open_sauce/settings/c_settings_cheape.hpp>
#include "Settings/SettingsSapien.hpp"
#include "Settings/SettingsTool.hpp"

namespace Yelo
{
	namespace Settings
	{
		static c_settings_manager g_settings_manager("OpenSauce");

		c_settings_manager& Manager()
		{
			return g_settings_manager;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// 	Parse the command line for the editor profile option and copy the result to
		/// 	[profile_name]. Sets [profile_name] to an empty string if an option isn't found.
		/// </summary>
		///
		/// <param name="profile_name">	Output buffer for the profile name. </param>
		void GetCommandLineDefaultProfile(string128 profile_name)
		{
#define CMDLINE_ARG_EDITOR_PROFILE L"-editorProfile:"
			profile_name[0] = '\0';

			int cmds_count = 0;
			auto cmds = std::unique_ptr<LPWSTR[], winapi_local_deleter>(
				CommandLineToArgvW(GetCommandLineW(), &cmds_count));

			for(int x = 0; x < cmds_count; x++)
			{
				wcstring cmd = wcsstr(cmds[x], CMDLINE_ARG_EDITOR_PROFILE);

				if(cmd != nullptr)
				{
					const size_t k_parameter_length = NUMBEROF(CMDLINE_ARG_EDITOR_PROFILE)-1;
					cmd = cmd + k_parameter_length;

					const size_t length = wcslen(cmd);

					if(length > 0)
						wstring_to_string(profile_name, Enums::k_string_128_length, cmd);
				}
			}

#undef CMDLINE_ARG_EDITOR_PROFILE
		}

		/// <summary>	Initializes the settings. </summary>
		void InitializeSettings()
		{
			Yelo::Settings::SharedInitialize("");

			c_settings_cheape::Register(Manager());
			c_settings_tool::Register(Manager());
			c_settings_sapien::Register(Manager());
		}

		/// <summary>	Disposes the settings. </summary>
		void DisposeSettings()
		{
			c_settings_sapien::Unregister(Manager());
			c_settings_tool::Unregister(Manager());
			c_settings_cheape::Unregister(Manager());

			Yelo::Settings::SharedDispose();
		}

		/// <summary>	Loads the editor settings from file. </summary>
		void Load()
		{
			string128 profile_name = "";
			GetCommandLineDefaultProfile(profile_name);

			Manager().Load(K_EDITOR_FILENAME_XML);
			c_settings_cheape::SetActiveProfile(profile_name);
		}

		/// <summary>	Saves the editor settings to file. </summary>
		void Save()
		{
			Manager().Save();
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Replaces environment variables in a provided string. </summary>
		///
		/// <param name="parse_string">	[in,out] The string to parse. </param>
		void ParseEnvironmentVariablesCheApe(std::string& parse_string)
		{
			ParseEnvironmentVariables(parse_string);

			auto& profile = c_settings_cheape::Profile();
			ReplaceEnvironmentVariable(parse_string, "$(Root)", profile.GetRootPath());
			ReplaceEnvironmentVariable(parse_string, "$(Data)", profile.GetDataPath());
			ReplaceEnvironmentVariable(parse_string, "$(Tags)", profile.GetTagsPath());
			ReplaceEnvironmentVariable(parse_string, "$(Maps)", profile.GetMapsPath());
		}
	};
};