/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <YeloLib/configuration/i_configuration_file.hpp>
#include <YeloLib/configuration/i_configuration_leaf.hpp>
#include <YeloLib/configuration/c_configuration_file_factory.hpp>
#include <YeloLib/configuration/c_configuration_container.hpp>

namespace Yelo
{
	namespace Settings
	{
		class c_settings_manager final
		{
			struct s_settings_container_entry
			{
				std::function<void()> m_pre_load_callback;
				std::function<void()> m_post_load_callback;
				std::function<void()> m_pre_save_callback;
				std::function<void()> m_post_save_callback;
				Configuration::c_configuration_container* m_container_ptr;
			};

			const std::string m_root_node_path;
			Configuration::configuration_file_ptr_t m_settings;
			std::vector<s_settings_container_entry> m_settings_containers;			
			
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the root configuration node. </summary>
			///
			/// <returns>	The configuration root node. </returns>
			std::unique_ptr<Configuration::i_configuration_leaf> GetRootNode(const std::string& root_node_path) const;

		public:
			c_settings_manager(const std::string& root_node);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Loads settings from the given file. </summary>
			///
			/// <param name="file_path">	The settings file to load. </param>
			void Load(const std::string& file_path);

			/// <summary>	Saves the settings to file. </summary>
			void Save();

			/// <summary>	Clears the current settings. </summary>
			void Clear();

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Registers a configuration container with the settings system. </summary>
			///
			/// <param name="container">		 	The container to register. </param>
			/// <param name="pre_load_callback"> 	The pre load callback. </param>
			/// <param name="post_load_callback">	The post load callback. </param>
			/// <param name="pre_save_callback"> 	The pre save callback. </param>
			/// <param name="post_save_callback">	The post save callback. </param>
			void RegisterConfigurationContainer(Configuration::c_configuration_container& container
				, const std::function<void()>& pre_load_callback
				, const std::function<void()>& post_load_callback
				, const std::function<void()>& pre_save_callback
				, const std::function<void()>& post_save_callback);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Unregisters a configuration container from the settings system. </summary>
			///
			/// <param name="container">	The container to unregister. </param>
			void UnregisterConfigurationContainer(Configuration::c_configuration_container& container);
		};
	};
};