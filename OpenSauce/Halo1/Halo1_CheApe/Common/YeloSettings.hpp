/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

#include <Common/Halo1/YeloSettingsDefinitions.hpp>
#include <Common/Halo1/YeloSharedSettings.hpp>

namespace Yelo
{
	namespace Settings
	{
		struct s_profile
		{
			bool is_valid;
			bool has_affinity_mask;
			PAD16;
			bool is_ignored;	// is this profile ignored by any tools?
			bool ignored_by[3]; // index with PLATFORM_ID

			uint32 affinity_mask;

			char root_path[MAX_PATH];

			struct {
				char data[MAX_PATH];
				char maps[MAX_PATH];
				char tags[MAX_PATH];
				char tags_folder_name[64];
			}paths;

			struct {
				uint32 base_adddress_override; // Tag memory base address override
			}tool;

			void Parse(TiXmlElement* element);

			// The profile will be valid if all elements of the XML settings 
			// were properly parsed
			API_INLINE bool IsValid() const		{ return is_valid; }

			// The profile will be ignored if the XML settings have an ignore setting for this platform 
			// or the profile wasn't properly parsed
			API_INLINE bool IsIgnored() const	{ return is_ignored && ignored_by[PLATFORM_ID-1]; }

			API_INLINE cstring GetRootPath() const			{ return root_path[0]  == '\0' ? NULL : root_path; }
			API_INLINE cstring GetDataOverridePath() const	{ return paths.data[0] == '\0' ? NULL : paths.data; }
			API_INLINE cstring GetMapsOverridePath() const	{ return paths.maps[0] == '\0' ? NULL : paths.maps; }
			API_INLINE cstring GetTagsOverridePath() const	{ return paths.tags[0] == '\0' ? NULL : paths.tags; }
		};
		struct s_settings
		{
			s_profile active_profile;

			void Parse(TiXmlElement* element, cstring arg_profile, cstring default_profile);

			API_INLINE cstring GetRootPath() const
			{
				cstring result = active_profile.GetRootPath();

				return !active_profile.IsIgnored() && result != NULL ? result 
					: "";
			}

			API_INLINE cstring GetDataPath() const
			{
				cstring result = active_profile.GetDataOverridePath();

				return !active_profile.IsIgnored() && result != NULL ? result 
					: "data\\";
			}

			API_INLINE cstring GetTagsPath() const
			{
				cstring result = active_profile.GetTagsOverridePath();

				return !active_profile.IsIgnored() && result != NULL ? result 
					: "tags\\";
			}

			API_INLINE cstring GetMapsPath() const
			{
				cstring result = active_profile.GetMapsOverridePath();

				return !active_profile.IsIgnored() && result != NULL ? result 
					: "maps\\";
			}
		};
		const s_settings& Get();

		void Initialize();
		void Dispose();

		void LoadSettings();
		void SaveSettings();

		void ParseEnvironmentVariablesCheApe(std::string& parse_string);
	};
};