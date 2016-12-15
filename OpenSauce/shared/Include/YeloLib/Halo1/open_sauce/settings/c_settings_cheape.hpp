/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

#if PLATFORM_IS_EDITOR

#include <blamlib/Halo1/tag_files/tag_files.hpp>
#include <blamlib/Halo1/cache/cache_files.hpp>

#include <YeloLib/configuration/c_configuration_container.hpp>
#include <YeloLib/configuration/c_configuration_value.hpp>
#include <YeloLib/configuration/c_configuration_container_list.hpp>
#include <YeloLib/open_sauce/settings/c_settings_singleton.hpp>

#include <YeloLib/Halo1/open_sauce/settings/yelo_shared_settings.hpp>

namespace Yelo
{
	namespace Settings
	{
#pragma region Containers
		/// <summary>	The paths container. </summary>
		class c_paths_container final
			: public Configuration::c_configuration_container
		{
		public:
			Configuration::c_configuration_value<std::string> m_root;
			Configuration::c_configuration_value<std::string> m_data_directory;
			Configuration::c_configuration_value<std::string> m_maps_directory;
			Configuration::c_configuration_value<std::string> m_tags_directory;

			c_paths_container();

		protected:
			const std::vector<i_configuration_value* const> GetMembers() override;
		};

		class c_profile_container final
			: public Configuration::c_configuration_container
		{
		public:
			Configuration::c_configuration_value<std::string>	m_name;
			c_paths_container									m_paths;

			c_profile_container();

		protected:
			const std::vector<i_configuration_value* const> GetMembers() override;
		};

		class c_cheape_container final
			: public Configuration::c_configuration_container
		{
		public:
			Configuration::c_configuration_container_list<c_profile_container> m_profiles;

			c_cheape_container();

		protected:
			const std::vector<i_configuration_value* const> GetMembers() override;
		};
#pragma endregion

#pragma region Profile
		class i_settings_profile
			abstract
		{
		public:
			virtual cstring GetRootPath() = 0;
			virtual cstring GetMapsPath() = 0;
			virtual cstring GetDataPath() = 0;
			virtual cstring GetTagsPath() = 0;
			virtual cstring GetTagsName() = 0;
		};
		
		class c_settings_profile final
			: public i_settings_profile
		{
			std::string m_root_directory;
			std::string m_data_directory;
			std::string m_tags_directory;
			std::string m_maps_directory;
			std::string m_tags_folder;
			
			void SetProfile(c_profile_container& profile);

		public:
			c_settings_profile(c_profile_container& profile);

			cstring GetRootPath() override;
			cstring GetMapsPath() override;
			cstring GetDataPath() override;
			cstring GetTagsPath() override;
			cstring GetTagsName() override;
		};

		class c_settings_profile_null final
			: public i_settings_profile
		{
			cstring GetRootPath() override { return ".\\"; };
			cstring GetMapsPath() override { return Cache::K_MAP_FILES_DIRECTORY; };
			cstring GetDataPath() override { return TagGroups::K_DATA_FILES_DIRECTORY; };
			cstring GetTagsPath() override { return TagGroups::K_TAG_FILES_DIRECTORY; };
			cstring GetTagsName() override { return "tags"; };
		};
#pragma endregion

#pragma region CheApe Settings
		class c_settings_cheape final
			: public c_settings_singleton<c_cheape_container, c_settings_cheape>
		{
			static std::unique_ptr<i_settings_profile> g_settings_profile;
			static bool g_is_custom_profile;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets a pointer to the requested profile if it is not ignored by the platform. </summary>
			///
			/// <param name="name">	The name of the profile to get. </param>
			///
			/// <returns>	null if it fails, else a pointer to the profile. </returns>
			c_profile_container* GetProfile(const std::string& name);

		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Query if the settings are using a custom profile. </summary>
			///
			/// <returns>	true if using a custom profile, false if not. </returns>
			static bool IsCustomProfile();

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets the active profile. </summary>
			///
			/// <remarks>	The editor will assert if the requested profile is missing. </remarks>
			///
			/// <param name="requested">	Name of the profile to use. </param>
			static void SetActiveProfile(const std::string& requested);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the current profile. </summary>
			///
			/// <returns>	An i_settings_profile interface </returns>
			static i_settings_profile& Profile();
		};
#pragma endregion
	};
};
#endif