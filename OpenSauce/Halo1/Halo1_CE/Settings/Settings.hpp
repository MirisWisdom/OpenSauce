/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <YeloLib/open_sauce/settings/c_settings_manager.hpp>
#include <YeloLib/Halo1/open_sauce/settings/yelo_shared_settings.hpp>

namespace Yelo
{
	namespace Settings
	{
		extern cstring K_CHAT_LOG_FILENAME;

		c_settings_manager& Manager();
		
		void InitializeSettings();
		void DisposeSettings();

		void Load();
		void Save();
	};
};