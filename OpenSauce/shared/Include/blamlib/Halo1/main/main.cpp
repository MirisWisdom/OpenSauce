/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <blamlib/Halo1/main/main.hpp>

#include <blamlib/Halo1/main/levels.hpp>
#include <blamlib/Halo1/main/main_structures.hpp>

namespace Yelo
{
	namespace GameState
	{
#if !PLATFORM_IS_EDITOR
		void s_main_globals::QuitToMainMenu()
		{
			map.switch_to_structure_bsp = NONE;
			map.save_map = false;
			map.main_menu_scenario_load = true;
		}

		bool IsLocal()	{ return MainGlobals()->game_connection == Enums::_game_connection_local; }
		bool IsServer() { return MainGlobals()->game_connection == Enums::_game_connection_network_server; }
		bool IsClient() { return MainGlobals()->game_connection == Enums::_game_connection_network_client; }
#endif
	};

	namespace Main
	{
		cstring k_halo1_campaign_level_names[Enums::k_number_of_halo1_campaign_levels] = {
			R"(levels\a10\a10)",
			R"(levels\a30\a30)",
			R"(levels\a50\a50)",
			R"(levels\b30\b30)",
			R"(levels\b40\b40)",
			R"(levels\c10\c10)",
			R"(levels\c20\c20)",
			R"(levels\c40\c40)",
			R"(levels\d20\d20)",
			R"(levels\4d0\d40)",
		};

		const std::string& RegistryGetGameExePath()
		{
			static std::string g_exe_path = std::string();
			static bool initialized;

			do {
				if (initialized)
					break;

				initialized = true;

				HKEY hkey;
				auto success = RegOpenKeyEx(HKEY_LOCAL_MACHINE, R"(Software\Microsoft\Microsoft Games\Halo CE)", 0, KEY_READ, &hkey);
				if (success != ERROR_SUCCESS)
					break;

				char local_path[MAX_PATH];
				// engine uses 255, but the key is a string, which means the length includes any terminating null char
				DWORD local_path_size = sizeof(local_path);
				success = RegQueryValueEx(hkey, "EXE Path", nullptr, nullptr, CAST_PTR(byte*, local_path), &local_path_size);
				RegCloseKey(hkey);
				if (success != ERROR_SUCCESS)
					break;

				g_exe_path.assign(local_path);
				char last_char = g_exe_path.back();
				if (last_char != '\\')
					g_exe_path.append("\\");

			} while (false);

			return g_exe_path;
		}
	};


	namespace blam
	{
		Enums::game_connection game_connection()	{ return GameState::MainGlobals()->game_connection; }

		cstring main_get_map_name()					{ return GameState::MainGlobals()->scenario_tag_path; }

		int16 PLATFORM_API main_get_campaign_level_from_name(cstring level_name)
		{
			char name[128] = { };

			// NOTE: engine doesn't do this, but level_name is usually a scenario path
			if (cstring last_slash = strrchr(level_name, '\\'))
				level_name = last_slash+1;

			strncpy_s(name, level_name, NUMBEROF(name)-1);
			name[NUMBEROF(name)-1] = '\0';

			_strlwr(name);

			using namespace Enums;

				 if (strstr(name, "a10"))	return _halo1_campaign_level_a10;
			else if (strstr(name, "a30"))	return _halo1_campaign_level_a30;
			else if (strstr(name, "a50"))	return _halo1_campaign_level_a50;
			else if (strstr(name, "b30"))	return _halo1_campaign_level_b30;
			else if (strstr(name, "b40"))	return _halo1_campaign_level_b40;
			else if (strstr(name, "c10"))	return _halo1_campaign_level_c10;
			else if (strstr(name, "c20"))	return _halo1_campaign_level_c20;
			else if (strstr(name, "c40"))	return _halo1_campaign_level_c40;
			else if (strstr(name, "d20"))	return _halo1_campaign_level_d20;
			else if (strstr(name, "d40"))	return _halo1_campaign_level_d40;

			return NONE;
		}

		cstring PLATFORM_API main_get_campaign_level_name(_enum level_index)
		{
			if (level_index >= 0 && level_index < NUMBEROF(Main::k_halo1_campaign_level_names))
				return Main::k_halo1_campaign_level_names[level_index];

			return nullptr;
		}
	};
};

