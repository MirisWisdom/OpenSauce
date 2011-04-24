/*
    Yelo: Open Sauce SDK
		Halo 1 (CE) Edition
    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "Common/Precompile.hpp"
#include "Common/YeloSettings.hpp"

#include "Memory/MemoryInterface.hpp"

#include "Game/Camera.hpp"
#include "Interface/GameUI.hpp"
#include "Objects/Objects.hpp"
#include "Rasterizer/Rasterizer.hpp"
#include "Networking/VersionCheck.hpp"

namespace Yelo
{
//////////////////////////////////////////////////////////////////////////
// Client settings
#if PLATFORM_IS_USER
	namespace Settings
	{
		static void LoadSettingsForClient(TiXmlElement* client)
		{
			TiXmlElement* dx9_element = NULL,
						* fov_element = NULL,
						* hud_element = NULL,
						* objects_element = NULL,
						* version_check_element = NULL
				;

			if(client != NULL)
			{
				dx9_element = client->FirstChildElement("dx9");
				fov_element = client->FirstChildElement("Fov");
				hud_element = client->FirstChildElement("Hud");
				objects_element = client->FirstChildElement("objects");
				version_check_element = client->FirstChildElement("version_check");
			}

			Rasterizer::LoadSettings(dx9_element);
			Fov::LoadSettings(fov_element);
			Hud::LoadSettings(hud_element);
			Objects::LoadSettings(objects_element);
			Networking::VersionCheck::LoadSettings(version_check_element);
		}

		static void SaveSettingsForClient(TiXmlElement* client)
		{
			TiXmlElement* dx9_element = NULL,
						* fov_element = NULL,
						* hud_element = NULL,
						* objects_element = NULL,
						* version_check_element = NULL
				;

			dx9_element = new TiXmlElement("dx9");
				client->LinkEndChild(dx9_element);
			fov_element = new TiXmlElement("Fov");
				client->LinkEndChild(fov_element);
			hud_element = new TiXmlElement("Hud");
				client->LinkEndChild(hud_element);
			objects_element = new TiXmlElement("objects");
				client->LinkEndChild(objects_element);
			version_check_element = new TiXmlElement("version_check");
				client->LinkEndChild(version_check_element);

			Rasterizer::SaveSettings(dx9_element);
			Fov::SaveSettings(fov_element);
			Hud::SaveSettings(hud_element);
			Objects::SaveSettings(objects_element);
			Networking::VersionCheck::SaveSettings(version_check_element);
		}
	};
#endif


//////////////////////////////////////////////////////////////////////////
// Server settings

	namespace Settings
	{
		static void LoadSettingsForServer(TiXmlElement* server)
		{
#if PLATFORM_IS_DEDI
			TiXmlElement* version_check_element = NULL;

			if(server != NULL)
			{
				version_check_element = server->FirstChildElement("version_check");
			}

			Networking::VersionCheck::LoadSettings(version_check_element);
#endif
		}

		static void SaveSettingsForServer(TiXmlElement* server)
		{
#if PLATFORM_IS_DEDI
			TiXmlElement* version_check_element = NULL;

			version_check_element = new TiXmlElement("version_check");
			server->LinkEndChild(version_check_element);

			Networking::VersionCheck::SaveSettings(version_check_element);
#endif
		}
	};


//////////////////////////////////////////////////////////////////////////
// Interface

	namespace Settings
	{
		cstring K_USER_FILENAME_XML = "OS_Settings.User.xml";
		cstring K_SERVER_FILENAME_XML = "OS_Settings.Server.xml";

#define __EL_INCLUDE_ID			__EL_INCLUDE_OPEN_SAUCE
#define __EL_INCLUDE_FILE_ID	__EL_COMMON_YELO_SETTINGS
#include "Memory/_EngineLayout.inl"

		static struct {
			static cstring ProfilePath() PTR_IMP_GET2(g_profile_path);

		}Internal;

	//////////////////////////////////////////////////////////////////////////

		void Initialize()
		{
			// get the override profile path, if any
			cstring profile_path = Internal.ProfilePath();
			
			Settings::SharedInitialize(profile_path);

			// Get the usable profile path. Either overridden with '-profile'
			// or defaulting to the User's documents folder.
			profile_path = Settings::UserProfilePath();


			LoadSettings();
		}

		void Dispose()
		{
			SaveSettings();

			Settings::SharedDispose();
		}


		static void LoadSettingsUser()
		{
#if PLATFORM_IS_USER
			TiXmlElement* root = NULL;
			TiXmlDocument xml_settings = TiXmlDocument();

			char file_path[MAX_PATH];
			if(GetSettingsFilePath(K_USER_FILENAME_XML, file_path))
				root = GenericSettingsParse(xml_settings, file_path, "client");

			LoadSettingsForClient(root);
#endif
		}

		static void LoadSettingsServer()
		{
			TiXmlElement* root = NULL;
			TiXmlDocument xml_settings = TiXmlDocument();

			char file_path[MAX_PATH];
			if(GetSettingsFilePath(K_SERVER_FILENAME_XML, file_path))
				root = GenericSettingsParse(xml_settings, file_path, "server");

			LoadSettingsForServer(root);
		}

		void LoadSettings()
		{
			LoadSettingsUser();
			LoadSettingsServer();
		}

		static void SaveSettingsUser()
		{
#if PLATFORM_IS_USER
			char file_path[MAX_PATH];
			GetSettingsFilePath(K_USER_FILENAME_XML, file_path);
			TiXmlDocument xml_settings(file_path);
			TiXmlElement* root = GenericSettingsWrite(xml_settings, "osYeloSettings", "client");

			SaveSettingsForClient(root);
			xml_settings.SaveFile(file_path);
#endif
		}

		static void SaveSettingsServer()
		{
			char file_path[MAX_PATH];
			GetSettingsFilePath(K_SERVER_FILENAME_XML, file_path);
			TiXmlDocument xml_settings(file_path);
			TiXmlElement* root = GenericSettingsWrite(xml_settings, "osYeloSettings", "server");

			SaveSettingsForServer(root);
			xml_settings.SaveFile(file_path);
		}

		void SaveSettings()
		{
			SaveSettingsUser();
			SaveSettingsServer();
		}
	};
};