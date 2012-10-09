/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Common/YeloSettings.hpp"

#include "Common/CmdLineSettings.hpp"
#include "Game/EngineFunctions.hpp"
#include "Memory/MemoryInterface.hpp"

#include "Game/Camera.hpp"
#include "Interface/GameUI.hpp"
#include "Networking/Networking.hpp"
#include "Networking/VersionCheck.hpp"
#include "Networking/HTTP/MapDownloadClient.hpp"
#include "Objects/Objects.hpp"
#include "Rasterizer/Rasterizer.hpp"
#include "TagGroups/CacheFiles.hpp"

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
						* version_check_element = NULL,
						* cf_element = NULL,
						* networking_element = NULL
				;

			if(client != NULL)
			{
				dx9_element = client->FirstChildElement("dx9");
				fov_element = client->FirstChildElement("Fov");
				hud_element = client->FirstChildElement("Hud");
				networking_element = client->FirstChildElement("networking");
				objects_element = client->FirstChildElement("objects");
				version_check_element = client->FirstChildElement("version_check");
				cf_element = client->FirstChildElement("cacheFiles");
			}

			Rasterizer::LoadSettings(dx9_element);
			Fov::LoadSettings(fov_element);
			Hud::LoadSettings(hud_element);
			Networking::LoadSettings(networking_element);
			Objects::LoadSettings(objects_element);
			Networking::VersionCheck::LoadSettings(version_check_element);
			Cache::LoadSettings(cf_element);
			Networking::HTTP::Client::MapDownload::LoadSettings(networking_element);
		}

		static void SaveSettingsForClient(TiXmlElement* client)
		{
			TiXmlElement* dx9_element = NULL,
						* fov_element = NULL,
						* hud_element = NULL,
						* objects_element = NULL,
						* version_check_element = NULL,
						* networking_element = NULL,
						* cf_element = NULL
				;

			dx9_element = new TiXmlElement("dx9");
				client->LinkEndChild(dx9_element);
			fov_element = new TiXmlElement("Fov");
				client->LinkEndChild(fov_element);
			hud_element = new TiXmlElement("Hud");
				client->LinkEndChild(hud_element);
			networking_element = new TiXmlElement("networking");
				client->LinkEndChild(networking_element);
			objects_element = new TiXmlElement("objects");
				client->LinkEndChild(objects_element);
			version_check_element = new TiXmlElement("version_check");
				client->LinkEndChild(version_check_element);
			cf_element = new TiXmlElement("cacheFiles");
				client->LinkEndChild(cf_element);

			Rasterizer::SaveSettings(dx9_element);
			Fov::SaveSettings(fov_element);
			Hud::SaveSettings(hud_element);
			Networking::SaveSettings(networking_element);
			Objects::SaveSettings(objects_element);
			Networking::VersionCheck::SaveSettings(version_check_element);
			Cache::SaveSettings(cf_element);
			Networking::HTTP::Client::MapDownload::SaveSettings(networking_element);
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
			// TODO: make the Cache and Objects settings load as readonly from the client settings
			TiXmlElement* version_check_element = NULL,
						* objects_element = NULL,
						* cf_element = NULL
				;

			if(server != NULL)
			{
				objects_element = server->FirstChildElement("objects");
				version_check_element = server->FirstChildElement("version_check");
				cf_element = server->FirstChildElement("cacheFiles");
			}

			Objects::LoadSettings(objects_element);
			Networking::VersionCheck::LoadSettings(version_check_element);
			Cache::LoadSettings(cf_element);
#endif
		}

		static void SaveSettingsForServer(TiXmlElement* server)
		{
#if PLATFORM_IS_DEDI
			TiXmlElement* version_check_element = NULL
				;

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
		cstring K_DEBUG_FILENAME = "Game.debug.log";
		cstring K_EVENT_LOG_FILENAME = "Game.Log.txt";
		cstring K_CHAT_LOG_FILENAME = "Game.ChatLog.txt";

#define __EL_INCLUDE_ID			__EL_INCLUDE_OPEN_SAUCE
#define __EL_INCLUDE_FILE_ID	__EL_COMMON_YELO_SETTINGS
#include "Memory/_EngineLayout.inl"

		static struct {
			static cstring ProfilePath() PTR_IMP_GET2(g_profile_path);

		}Internal;

	//////////////////////////////////////////////////////////////////////////
		void Initialize()
		{
			ReadCmdLineSettings();

			cstring profile_path = "";

			if(CMDLINE_GET_PARAM(path).ParameterSet())
				profile_path = CMDLINE_GET_PARAM(path).GetValue();

			Settings::SharedInitialize(profile_path);

			LoadSettings();
		}

		void Dispose()
		{
			SaveSettings();

			Settings::SharedDispose();
		}


		// HACK: if this ever changes, be sure to update LoadSettingsUserHack in CheApe's YeloSettings.cpp!
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


		void GetTimeStampString(_Out_ tag_string time_str)
		{
			const size_t k_time_str_sizeof = sizeof(tag_string);

			memset(time_str, 0, k_time_str_sizeof);

			tm newtime;
			time_t aclock;	time( &aclock ); // Get time in seconds
			localtime_s( &newtime, &aclock ); // Convert time to struct tm form
			asctime_s( time_str, k_time_str_sizeof, &newtime );
		}

		void GetTimeStampStringForFile(_Out_ tag_string time_str)
		{
			const size_t k_time_str_sizeof = sizeof(tag_string);

			memset(time_str, 0, k_time_str_sizeof);

			tm newtime;
			time_t aclock;	time( &aclock ); // Get time in seconds
			localtime_s( &newtime, &aclock ); // Convert time to struct tm form

			sprintf_s(time_str, k_time_str_sizeof, "%04d_%02d_%02d_%02d_%02d_%02d", 
				newtime.tm_year + 1900,
				newtime.tm_mon + 1,
				newtime.tm_mday,
				newtime.tm_hour,
				newtime.tm_min,
				newtime.tm_sec);
		}
	};
};