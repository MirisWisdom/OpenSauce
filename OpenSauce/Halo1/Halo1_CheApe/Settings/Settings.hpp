/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

#include <YeloLib/open_sauce/settings/c_settings_manager.hpp>
#include <YeloLib/Halo1/open_sauce/settings/yelo_shared_settings.hpp>

namespace Yelo
{
	namespace Settings
	{
		c_settings_manager& Manager();

		void Load();
		void Save();

		void InitializeSettings();
		void DisposeSettings();

		void ParseEnvironmentVariablesCheApe(std::string& parse_string);
	};
};