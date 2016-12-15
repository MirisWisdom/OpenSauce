/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/open_sauce/settings/c_settings_manager.hpp>

#include <YeloLib/configuration/i_configuration_file.hpp>
#include <YeloLib/configuration/i_configuration_leaf.hpp>
#include <YeloLib/configuration/c_configuration_file_factory.hpp>
#include <YeloLib/configuration/c_configuration_container.hpp>

using namespace Yelo::Configuration;

namespace Yelo
{
	namespace Settings
	{
		c_settings_manager::c_settings_manager(const std::string& root_node)
			: m_root_node_path(root_node)
			, m_settings(nullptr)
			, m_settings_containers()
		{ }

		std::unique_ptr<i_configuration_leaf> c_settings_manager::GetRootNode(const std::string& root_node_path) const
		{
			auto root_node = m_settings->Root()->GetChild(root_node_path);
			if(!root_node)
			{
				root_node = m_settings->Root()->AddChild(root_node_path);
			}

			return root_node;
		}

		void c_settings_manager::Load(const std::string& file_path)
		{
			// Load the configuration file.
			m_settings = c_configuration_file_factory::CreateConfigurationFile(file_path);

			if (!m_settings)
			{
				YELO_ASSERT("Failed to load or create a settings file.");
			}

			m_settings->Load();

			// Get the registered containers from the loaded settings
			auto root_node_ptr = GetRootNode(m_root_node_path);
			auto& root_node = *root_node_ptr;

			for(auto entry : m_settings_containers)
			{
				if(entry.m_pre_load_callback)
				{
					entry.m_pre_load_callback();
				}

				entry.m_container_ptr->GetValueFromParent(root_node);

				if(entry.m_post_load_callback)
				{
					entry.m_post_load_callback();
				}
			}
		}

		void c_settings_manager::Save()
		{
			Clear();

			// Add the registered containers to the settings
			auto root_node_ptr = GetRootNode(m_root_node_path);
			auto& root_node = *root_node_ptr;

			for(const auto& entry : m_settings_containers)
			{
				if(entry.m_pre_save_callback)
				{
					entry.m_pre_save_callback();
				}

				entry.m_container_ptr->SetValueToParent(root_node);

				if(entry.m_post_save_callback)
				{
					entry.m_post_save_callback();
				}
			}

			// Save the settings to file
			m_settings->Save();
		}

		void c_settings_manager::Clear()
		{
			m_settings->Clear();
		}

		void c_settings_manager::RegisterConfigurationContainer(c_configuration_container& container
			, const std::function<void()>& pre_load_callback
			, const std::function<void()>& post_load_callback
			, const std::function<void()>& pre_save_callback
			, const std::function<void()>& post_save_callback)
		{
			s_settings_container_entry entry =
			{
				pre_load_callback,
				post_load_callback,
				pre_save_callback,
				post_save_callback,
				&container
			};

			m_settings_containers.push_back(entry);
		}

		void c_settings_manager::UnregisterConfigurationContainer(c_configuration_container& container)
		{
			auto container_ptr = &container;

			// Find the containers' entry using it's pointer then remove it
			auto entry_iterator = find_if(m_settings_containers.begin(), m_settings_containers.end(),
				[container_ptr](s_settings_container_entry& entry)
				{
					return entry.m_container_ptr == container_ptr;
				}
			);

			if(entry_iterator != m_settings_containers.end())
			{
				m_settings_containers.erase(entry_iterator);
			}
		}
	};
};