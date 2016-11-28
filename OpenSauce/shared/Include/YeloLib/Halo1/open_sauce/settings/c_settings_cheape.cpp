/*
	Yelo: Open Sauce SDK
	Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/Halo1/open_sauce/settings/c_settings_cheape.hpp>

#if PLATFORM_IS_EDITOR

#include <YeloLib/configuration/c_configuration_value.hpp>

using namespace Yelo::Configuration;
using namespace boost;

namespace Yelo
{
	namespace Settings
	{
		std::unique_ptr<i_settings_profile> c_settings_cheape::g_settings_profile(std::make_unique<c_settings_profile_null>());
		bool c_settings_cheape::g_is_custom_profile;
		
#pragma region Containers
		c_paths_container::c_paths_container()
			: c_configuration_container("Paths")
			, m_root("Root", "")
			, m_data_directory("DataDirectory", "")
			, m_maps_directory("MapsDirectory", "")
			, m_tags_directory("TagsDirectory", "")
		{ }

		const std::vector<i_configuration_value* const> c_paths_container::GetMembers()
		{
			return std::vector<i_configuration_value* const>
			{
				&m_root,
				&m_data_directory,
				&m_maps_directory,
				&m_tags_directory
			};
		}

		c_profile_container::c_profile_container()
			: c_configuration_container("Profile")
			, m_name("Name", "")
			, m_paths()
		{ }

		const std::vector<i_configuration_value* const> c_profile_container::GetMembers()
		{
			return std::vector<i_configuration_value* const> { &m_name, &m_paths };
		}

		c_cheape_container::c_cheape_container()
			: c_configuration_container("Profiles")
			, m_profiles("Profile", []() { return c_profile_container(); })
		{ }

		const std::vector<i_configuration_value* const> c_cheape_container::GetMembers()
		{
			return std::vector<i_configuration_value* const> { &m_profiles };
		}
#pragma endregion

#pragma region Profile
		c_settings_profile::c_settings_profile(c_profile_container& profile)
			: m_root_directory()
			, m_data_directory()
			, m_tags_directory()
			, m_maps_directory()
			, m_tags_folder()
		{
			SetProfile(profile);
		}

		void c_settings_profile::SetProfile(c_profile_container& profile)
		{
			auto& paths = profile.m_paths;
			YELO_ASSERT_DISPLAY(!paths.m_root.Get().empty(), "Root directory missing from editor profile");
			YELO_ASSERT_DISPLAY(!paths.m_data_directory.Get().empty(), "Data directory name missing from editor profile");
			YELO_ASSERT_DISPLAY(!paths.m_tags_directory.Get().empty(), "Tags directory name missing from editor profile");
			YELO_ASSERT_DISPLAY(!paths.m_maps_directory.Get().empty(), "Maps directory name missing from editor profile");
			
			m_root_directory = (filesystem::path(paths.m_root.Get())).string();
			m_data_directory = (filesystem::path(m_root_directory) /= paths.m_data_directory.Get()).string();
			m_tags_directory = (filesystem::path(m_root_directory) /= paths.m_tags_directory.Get()).string();
			m_maps_directory = (filesystem::path(m_root_directory) /= paths.m_maps_directory.Get()).string();
			m_tags_folder = paths.m_tags_directory;
			
			YELO_ASSERT_DISPLAY(filesystem::exists(m_root_directory), "Root directory defined in an editor profile does not exist: [%s]", m_root_directory.c_str());
			YELO_ASSERT_DISPLAY(filesystem::exists(m_data_directory), "Data directory defined in an editor profile does not exist: [%s]", m_data_directory.c_str());
			YELO_ASSERT_DISPLAY(filesystem::exists(m_tags_directory), "Tags directory defined in an editor profile does not exist: [%s]", m_tags_directory.c_str());
			YELO_ASSERT_DISPLAY(filesystem::exists(m_maps_directory), "Maps directory defined in an editor profile does not exist: [%s]", m_maps_directory.c_str());
		}

		cstring c_settings_profile::GetRootPath()
		{
			return (m_root_directory.empty() ? nullptr : m_root_directory.c_str());
		}

		cstring c_settings_profile::GetDataPath()
		{
			return (m_data_directory.empty() ? nullptr : m_data_directory.c_str());
		}

		cstring c_settings_profile::GetTagsPath()
		{
			return (m_tags_directory.empty() ? nullptr : m_tags_directory.c_str());
		}

		cstring c_settings_profile::GetTagsName()
		{
			return (m_tags_folder.empty() ? nullptr : m_tags_folder.c_str());
		}

		cstring c_settings_profile::GetMapsPath()
		{
			return (m_maps_directory.empty() ? nullptr : m_maps_directory.c_str());
		}
#pragma endregion
		
#pragma region CheApe Settings
		c_profile_container* c_settings_cheape::GetProfile(const std::string& name)
		{
			auto& settings_instance = Get();

			auto& found = find_if(settings_instance.m_profiles.begin(), settings_instance.m_profiles.end(),
				[name](c_profile_container& profile)
				{
					return profile.m_name.Get() == name;
				}
			);

			if(found != settings_instance.m_profiles.end())
			{
				c_profile_container& profile(*found);
				return &profile;
			}

			return nullptr;
		}

		bool c_settings_cheape::IsCustomProfile()
		{
			return g_is_custom_profile;
		}

		void c_settings_cheape::SetActiveProfile(const std::string& requested)
		{
			if (requested == "")
			{
				return;
			}

			g_is_custom_profile = true;

			auto profile = Instance().GetProfile(requested);
			YELO_ASSERT_DISPLAY(profile, "Requested editor profile not found.");
			
			g_settings_profile = std::make_unique<c_settings_profile>(*profile);
		}

		i_settings_profile& c_settings_cheape::Profile()
		{
			return *g_settings_profile;
		}
#pragma endregion
	};
};
#endif