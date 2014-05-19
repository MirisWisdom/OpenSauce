/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <blamlib/Halo1/main/main.hpp>

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
#if !PLATFORM_IS_EDITOR
		Enums::game_connection game_connection()	{ return GameState::MainGlobals()->game_connection; }
#endif
	};
};

