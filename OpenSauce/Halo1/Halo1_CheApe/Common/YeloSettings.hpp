/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

#include <YeloLib/Halo1/open_sauce/settings/che_ape_settings.hpp>
#include <YeloLib/Halo1/open_sauce/settings/yelo_shared_settings.hpp>

namespace Yelo
{
	namespace Settings
	{
		void Initialize();
		void Dispose();

		void LoadSettings();
		void SaveSettings();

		void ParseEnvironmentVariablesCheApe(std::string& parse_string);
	};
};