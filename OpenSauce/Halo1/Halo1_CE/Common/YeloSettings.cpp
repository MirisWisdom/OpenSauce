/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Common/YeloSettings.hpp"

#include <YeloLib/Halo1/shell/shell_windows_command_line.hpp>
#include <YeloLib/Halo1/open_sauce/settings/yelo_shared_settings.hpp>
#include <YeloLib/configuration/i_configuration_file.hpp>
#include <YeloLib/configuration/i_configuration_leaf.hpp>
#include <YeloLib/configuration/c_configuration_file_factory.hpp>
#include <YeloLib/configuration/c_configuration_container.hpp>

namespace Yelo
{
/////////////////////////////////////////////////////////////////////////
// Interface

	namespace Settings
	{
		cstring K_CHAT_LOG_FILENAME = "Game.ChatLog.txt";

		struct s_settings_globals
		{
			struct s_settings_container_entry
			{
				std::function<void()> m_pre_load_callback;
				std::function<void()> m_post_load_callback;
				std::function<void()> m_pre_save_callback;
				std::function<void()> m_post_save_callback;
				Configuration::c_configuration_container* m_container_ptr;
			};

			Configuration::t_configuration_file_ptr m_settings;
			std::vector<s_settings_container_entry> m_settings_containers;
		};
		static s_settings_globals g_settings_globals;
		
		/// <summary>	Initializes the shared settings. </summary>
		void InitializeSettings()
		{
			ReadCmdLineSettings();

			cstring profile_path = "";

			if(CMDLINE_GET_PARAM(path).ParameterSet())
				profile_path = CMDLINE_GET_PARAM(path).GetValue();
			
			Settings::SharedInitialize(profile_path);
		}

		/// <summary>	Disposes the shared settings. </summary>
		void DisposeSettings()
		{
			Settings::SharedDispose();
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the root configuration node. </summary>
		///
		/// <param name="container">	The container to get the root node from. </param>
		///
		/// <returns>	The configuration root node. </returns>
		std::unique_ptr<Configuration::i_configuration_leaf> GetRootNode(Configuration::c_configuration_container& container)
		{
			auto root_node = g_settings_globals.m_settings->Root()->GetChild("OpenSauce");
			if(!root_node)
			{
				root_node = g_settings_globals.m_settings->Root()->AddChild("OpenSauce");
			}

			return root_node;
		}
		
		/// <summary>	Loads the runtime settings file. </summary>
		void Load()
		{
			char file_path[MAX_PATH];
#if PLATFORM_IS_USER
			GetSettingsFilePath(K_USER_FILENAME_XML, file_path);
#else
			GetSettingsFilePath(K_SERVER_FILENAME_XML, file_path);
#endif
			g_settings_globals.m_settings = Configuration::c_configuration_file_factory::CreateConfigurationFile(file_path);
			g_settings_globals.m_settings->Load();

			for(auto entry : g_settings_globals.m_settings_containers)
			{
				if(entry.m_pre_load_callback)
				{
					entry.m_pre_load_callback();
				}

				auto root_node = GetRootNode(*entry.m_container_ptr);
				entry.m_container_ptr->GetValueFromParent(*root_node);

				if(entry.m_post_load_callback)
				{
					entry.m_post_load_callback();
				}
			}
		}

		/// <summary>	Saves the current configuration. </summary>
		void Save()
		{
			Clear();

			for(const auto& entry : g_settings_globals.m_settings_containers)
			{
				if(entry.m_pre_save_callback)
				{
					entry.m_pre_save_callback();
				}

				auto root_node = GetRootNode(*entry.m_container_ptr);
				entry.m_container_ptr->SetValueToParent(*root_node);

				if(entry.m_post_save_callback)
				{
					entry.m_post_save_callback();
				}
			}

			g_settings_globals.m_settings->Save();
		}

		/// <summary>	Clears the current settings. </summary>
		void Clear()
		{
			g_settings_globals.m_settings->Clear();
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Registers a configuration container with the settings system. </summary>
		///
		/// <param name="container">		 	The container to register. </param>
		/// <param name="pre_load_callback"> 	The pre load callback. </param>
		/// <param name="post_load_callback">	The post load callback. </param>
		/// <param name="pre_save_callback"> 	The pre save callback. </param>
		/// <param name="post_save_callback">	The post save callback. </param>
		void RegisterConfigurationContainer(Configuration::c_configuration_container* container
			, std::function<void()> pre_load_callback
			, std::function<void()> post_load_callback
			, std::function<void()> pre_save_callback
			, std::function<void()> post_save_callback)
		{
			s_settings_globals::s_settings_container_entry entry =
			{
				pre_load_callback,
				post_load_callback,
				pre_save_callback,
				post_save_callback,
				container
			};

			g_settings_globals.m_settings_containers.push_back(entry);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Unregisters a configuration container from the settings system. </summary>
		///
		/// <param name="container">	The container to unregister. </param>
		void UnregisterConfigurationContainer(Configuration::c_configuration_container* container)
		{
			auto entry_iterator = std::find_if(g_settings_globals.m_settings_containers.begin(), g_settings_globals.m_settings_containers.end(),
				[container](s_settings_globals::s_settings_container_entry& entry)
				{
					return entry.m_container_ptr == container;
				}
			);

			if(entry_iterator != g_settings_globals.m_settings_containers.end())
			{
				g_settings_globals.m_settings_containers.erase(entry_iterator);
			}
		}
	};
};