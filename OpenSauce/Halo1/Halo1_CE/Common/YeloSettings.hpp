/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <YeloLib/configuration/c_configuration_container.hpp>
#include <YeloLib/Halo1/open_sauce/settings/yelo_shared_settings.hpp>

namespace Yelo
{
	namespace Settings
	{
		extern cstring K_CHAT_LOG_FILENAME;
		
		void InitializeSettings();
		void DisposeSettings();

		void Load();
		void Save();
		void Clear();

		void RegisterConfigurationContainer(Configuration::c_configuration_container* container
			, std::function<void()> pre_load_callback = nullptr
			, std::function<void()> post_load_callback = nullptr
			, std::function<void()> pre_save_callback = nullptr
			, std::function<void()> post_save_callback = nullptr);
		void UnregisterConfigurationContainer(Configuration::c_configuration_container* container);
	};
};