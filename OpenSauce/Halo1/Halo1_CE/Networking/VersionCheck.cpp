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
#include "Networking/VersionCheck.hpp"

#ifdef YELO_VERSION_CHECK_ENABLE

#include <time.h>
#include <ctime>

#include "TagGroups/CacheFiles.hpp"

#if PLATFORM_IS_USER
	#include "Networking/VersionCheckClient.hpp"
#elif PLATFORM_IS_DEDI
	#include "Networking/VersionCheckDedi.hpp"
#endif

namespace Yelo
{
	namespace Networking { namespace VersionCheck
	{
		void		c_version_display_manager_base::SetCurrentVersionStringImpl(wcstring version_string)
		{
			m_strings.current_version[0] = L'\0';
			wcscat_s(m_strings.current_version, version_string);
		}

		void		c_version_display_manager_base::SetAvailableVersionStringImpl(wcstring version_string)
		{
			m_strings.available_version[0] = L'\0';
			wcscat_s(m_strings.available_version, version_string);
		}

		/////////////////////////////////////////
		//namespace functions
		void		Initialize()
		{
			c_version_check_manager::VersionChecker().Initialize();
		}
		void		Dispose()
		{
			c_version_check_manager::VersionChecker().Dispose();
		}

		void		Initialize3D(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters)
		{
			c_version_check_manager::VersionChecker().Initialize3D(pDevice, pParameters);			
		}
		void		OnLostDevice()
		{
			c_version_check_manager::VersionChecker().OnLostDevice();
		}
		void		OnResetDevice(D3DPRESENT_PARAMETERS* pParameters)
		{
			c_version_check_manager::VersionChecker().OnResetDevice(pParameters);
		}
		void		Render()
		{
			c_version_check_manager::VersionChecker().Render();
		}
		void		Release()
		{
			c_version_check_manager::VersionChecker().Release();
		}

		void		LoadSettings(TiXmlElement* dx9_element)
		{
			c_version_check_manager::VersionChecker().LoadSettings(dx9_element);
		}
		void		SaveSettings(TiXmlElement* dx9_element)
		{
			c_version_check_manager::VersionChecker().SaveSettings(dx9_element);
		}

		void		InitializeForNewMap()
		{
			c_version_check_manager::VersionChecker().InitializeForNewMap();
		}
		void		Update(real delta_time)
		{
			c_version_check_manager::VersionChecker().Update(delta_time);
		}

		/////////////////////////////////////////
		//c_version_check_manager
		/////////////////////////////////////////

		/////////////////////////////////////////
		//static variables
		c_version_check_manager		c_version_check_manager::g_instance;
		cstring						c_version_check_manager::g_fallback_xml_location = 
			"http://open-sauce.googlecode.com/hg/OpenSauce/Halo1/Halo1_CE/Halo1_CE_Version.xml";

		/////////////////////////////////////////
		//static functions
		GHTTPBool	c_version_check_manager::GetRequestComplete(GHTTPRequest request,
			GHTTPResult result,
			char* buffer,
			GHTTPByteCount bufferLen,
			char* headers,
			void* param)
		{
			s_xml_source* source = CAST_PTR(s_xml_source*, param);	
			source->request_get_completed = true;

			// delete any data left behind from previous gets
			delete source->data;
			source->data = NULL;

			// copy the file into the source xml object
			if(result == GHTTPSuccess)
			{
				source->request_get_succeeded = true;
				source->data = new char[(unsigned int)bufferLen];
				memcpy_s(source->data, (rsize_t)bufferLen, buffer, (rsize_t)bufferLen);
			}
			else
				source->request_get_succeeded = false;

			g_instance.ProcessVersionXml();

			return GHTTPTrue;	
		}
		/////////////////////////////////////////
		//non-static functions
		/*!
		 * \brief
		 * Initialises the ghttp library and the display manager.
		 *
		 * Starts up the ghttp library, initialises the display manager and sets the initial values for it.
		 */
		void		c_version_check_manager::Initialize()
		{
			ghttpStartup();

			m_current_version.SetBuild(2, 5, 0);
			m_available_version.SetBuild(2, 5, 0);

			c_version_display_manager::g_instance.Initialize();
		}
		/*!
		 * \brief
		 * Disposes of the ghttp library and display manager.
		 * 
		 * Disposes of the ghttp library and display manager.
		 */
		void		c_version_check_manager::Dispose()
		{
			c_version_display_manager::g_instance.Dispose();

			ghttpCleanup();
		}

		/*!
		 * \brief
		 * Sets up the display managers initial values.
		 * 
		 * \param pDevice
		 * The current render device.
		 * 
		 * \param pParameters
		 * Pointer to the parameters the device was created with.
		 * 
		 * Initialises the display managers Direwct3D resources, 
		 * then sets its version strings to their initial values.
		 */
		void		c_version_check_manager::Initialize3D(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters)
		{
			c_version_display_manager::g_instance.Initialize3D(pDevice, pParameters);

			wchar_t current_string[32];
			swprintf_s(current_string, 32, L"v%i.%i.%i", m_current_version.m_major, m_current_version.m_minor, m_current_version.m_build);
			c_version_display_manager::g_instance.SetCurrentVersionString(current_string);

			c_version_display_manager::g_instance.SetAvailableVersionString(L"");
		}
		void		c_version_check_manager::OnLostDevice()
		{
			c_version_display_manager::g_instance.OnLostDevice();
		}
		void		c_version_check_manager::OnResetDevice(D3DPRESENT_PARAMETERS* pParameters)
		{
			c_version_display_manager::g_instance.OnResetDevice(pParameters);
		}
		void		c_version_check_manager::Render()
		{
			// if we are in the main menu, show the version number
			if(m_states.is_in_menu)
				c_version_display_manager::g_instance.Render();
		}
		void		c_version_check_manager::Release()
		{
			c_version_display_manager::g_instance.Release();
		}
		/*!
		 * \brief
		 * Loads the server list from the user settings.
		 * 
		 * \param xml_element
		 * Pointer to an xml element that parents the version checkers settings.
		 * 
		 * Loads the server list and the date the available was last checked on, from the users settings.
		 */
		void		c_version_check_manager::LoadSettings(TiXmlElement* xml_element)
		{
			if(xml_element == NULL) return;

			m_states.last_checked_day = 0;
			m_states.last_checked_month = 0;
			m_states.last_checked_year = 0;
			do
			{
				const char* attribute = NULL;
				//get the last date the version was checked
				attribute = xml_element->Attribute("day", &m_states.last_checked_day);
				if(attribute == NULL) { m_states.last_checked_day = 0; break; }
				attribute = xml_element->Attribute("month", &m_states.last_checked_month);
				if(attribute == NULL) { m_states.last_checked_month = 0; break; }
				attribute = xml_element->Attribute("year", &m_states.last_checked_year);
				if(attribute == NULL) { m_states.last_checked_year = 0; break; }
			}while(false);

			// determine whether the version has already been checked today
			time_t time_today;
			time(&time_today);
			tm* local_time = localtime(&time_today);

			if((m_states.last_checked_day == local_time->tm_mday) &&
				(m_states.last_checked_month == local_time->tm_mon) &&
				(m_states.last_checked_year == 1900 + local_time->tm_year))
				m_states.checked_today = true;

			//get the xml locations from the user settings
			TiXmlElement* server_list = xml_element->FirstChildElement("server_list");
			if(server_list)
				LoadXmlServers(server_list);
		}
		/*!
		 * \brief
		 * Saves the current server list to the users settings.
		 * 
		 * \param xml_element
		 * Pointer to an xml element that parents the version checkers settings.
		 * 
		 * Saves the current server list and current date to the users settings.
		 */
		void		c_version_check_manager::SaveSettings(TiXmlElement* xml_element)
		{
			//set the attributes for the last date the version was checked
			xml_element->SetAttribute("day", m_states.last_checked_day);
			xml_element->SetAttribute("month", m_states.last_checked_month);
			xml_element->SetAttribute("year", m_states.last_checked_year);

			//save the current file locations to the user settings
			TiXmlElement* server_list = new TiXmlElement("server_list");
			for(int i = 0; i < NUMBEROF(m_xml_sources); i++)
			{
				if((m_xml_sources[i].xml_address == NULL) ||
					(strlen(m_xml_sources[i].xml_address) == 0)) 
					continue;

				TiXmlElement* server = new TiXmlElement("server");
				TiXmlText* server_address = new TiXmlText(m_xml_sources[i].xml_address);

				server->LinkEndChild(server_address);
				server_list->LinkEndChild(server);
			}
			xml_element->LinkEndChild(server_list);
		}
		/*!
		 * \brief
		 * Checks for a new version when the Main Menu is loaded.
		 * 
		 * If the game is loading the main menu, then check for the available online version.
		 * The update check will only occur once per day, and only once per game session.
		 * If the update does not complete before a new map is loaded, the http requests
		 * are canceled, and the update is re-run the next time the game is in the main menu.
		 * If the update check has been completed, and a new version is available,
		 * the display manager will be set to display it's animation for 20 seconds, each time
		 * the main menu is loaded.
		 */
		void		c_version_check_manager::InitializeForNewMap()
		{
			// find out if we are on the main menu
			if(Cache::CacheFileGlobals()->current_cache.header.cache_type == 2)
				m_states.is_in_menu = true;
			else
				m_states.is_in_menu = false;

			// if we are on the main menu, and a new version is available play the animation
			if(m_states.is_in_menu && m_states.is_new_version)
				c_version_display_manager::g_instance.StartUpdateDisplay(20);
			else
				c_version_display_manager::g_instance.ResetDisplay();

			// if we are on the main menu and the update check hasn't been done yet, set it going
			// otherwise if we are loading a new map and the version check is not complete, cancel 
			// the requests
			if(m_states.is_in_menu && !m_states.checked_this_session && !m_states.checked_today)
				CheckForUpdates();
			else if(!m_states.checked_this_session && !m_states.checked_today)
			{
				for(int i = 0; i < NUMBEROF(m_xml_sources); i++)
				{
					ghttpCancelRequest(m_xml_sources[i].request_id);

					m_xml_sources[i].request_id = 0;
					m_xml_sources[i].request_get_attempted = false;
					m_xml_sources[i].request_get_completed = false;
					m_xml_sources[i].request_get_succeeded = false;
					delete m_xml_sources[i].data;
					m_xml_sources[i].data = NULL;
				}
			}
		}
		/*!
		 * \brief
		 * Updates the ghttp library and display manager.
		 * 
		 * \param delta_time
		 * Time in seconds that has passed since the last update.
		 * 
		 * Updates the ghttp library to update any requests, and updates the display manager.
		 */
		void		c_version_check_manager::Update(real delta_time)
		{	
			// update the ghttp library so that it can update the requests
			if(!m_states.checked_this_session && !m_states.checked_today)
				ghttpThink();
			// if we are in the main menu, update the display manager
			if(m_states.is_in_menu)
				c_version_display_manager::g_instance.Update(delta_time);
		}
		/*!
		 * \brief
		 * Reads new xml locations from an xml element.
		 * 
		 * Reads new xml locations from an xml element. Currently the
		 * maximum number of locations is 3.
		 */
		void		c_version_check_manager::LoadXmlServers(TiXmlElement* server_list)
		{						
			TiXmlElement* server = server_list->FirstChildElement("server");
			for(int i = 0; i < NUMBEROF(m_xml_sources); i++)
			{
				// delete the previous locations regardless of whether there is a replacement
				delete m_xml_sources[i].xml_address;
				m_xml_sources[i].xml_address = NULL;

				if(server == NULL) continue;

				// get the text value of each server
				const char* text_value = server->GetText();
				if(text_value && (strlen(text_value) != 0))
				{
					int text_length = strlen(text_value) + 1;
					m_xml_sources[i].xml_address = new char[text_length];
					memcpy_s(m_xml_sources[i].xml_address, text_length, text_value, text_length);

					server = server->NextSiblingElement("server");
				}
			}
		}

		/*!
		 * \brief
		 * Starts the update checking process.
		 * 
		 * Sends the get requests for the xml files from the xml locations.
		 */
		void		c_version_check_manager::CheckForUpdates()
		{
			// get the xml files from each server
			bool has_valid_source = false;

			// send a get request to the server using the ghttp library
			// this will return immediately but will call the request complete
			// callback once the get request is complete which will then continue
			// the update checking process
			for(int i = 0; i < NUMBEROF(m_xml_sources); i++)
			{
				if(m_xml_sources[i].xml_address)
				{
					m_xml_sources[i].request_id = ghttpGetEx(m_xml_sources[i].xml_address,
						NULL,
						NULL,
						0,
						NULL,
						GHTTPFalse,
						GHTTPFalse,
						NULL,
						GetRequestComplete,
						&m_xml_sources[i]);
					m_xml_sources[i].request_get_attempted = true;
					has_valid_source = true;
				}
			}

			// if the server sources list has no valid locations, fallback to the cefault
			if(!has_valid_source)
			{
				m_xml_sources[0].request_id = ghttpGetEx(g_fallback_xml_location,
					NULL,
					NULL,
					0,
					NULL,
					GHTTPFalse,
					GHTTPFalse,
					NULL,
					GetRequestComplete,
					&m_xml_sources[0]);
				m_xml_sources[0].request_get_attempted = true;
			}
		}		
		/*!
		 * \brief
		 * Processes the downloaded xml files.
		 * 
		 * Once all of the file requests are complete, this function parses the xml files
		 * to get the latest version that is available, and updates the xml file locations.
		 */
		void		c_version_check_manager::ProcessVersionXml()
		{
			// do not continue until all of the attempted http requests are complete
			for(int i = 0; i < NUMBEROF(m_xml_sources); i++)
				if(m_xml_sources[i].request_get_attempted &&
					!m_xml_sources[i].request_get_completed)
					return;

			int server_list_version = 0;

			// process the downloaded xml files
			for(int i = 0; i < NUMBEROF(m_xml_sources); i++)
			{
				if(!m_xml_sources[i].request_get_attempted || 
					!m_xml_sources[i].request_get_completed ||
					!m_xml_sources[i].request_get_succeeded) continue;

				TiXmlDocument version_doc("version_doc");
				version_doc.Parse(m_xml_sources[i].data);

				TiXmlElement* root = version_doc.RootElement();
				if(!root) continue;

				// process all of the version elements, getting the latest one
				TiXmlElement* version = root->FirstChildElement("version");
				while(version)
				{
					int major = 0;
					int minor = 0;
					int build = 0;

					const char* attribute = NULL;
					
					do
					{
						attribute = version->Attribute("major", &major);
						if(attribute == NULL) break;
						attribute = version->Attribute("minor", &minor);
						if(attribute == NULL) break;
						attribute = version->Attribute("build", &build);
						if(attribute == NULL) break;

						if((m_available_version.m_major < major) ||
							(m_available_version.m_minor < minor) ||
							(m_available_version.m_build < build))
							m_available_version.SetBuild(major, minor, build);
					}
					while(false);

					version = version->NextSiblingElement("version");
				}

				// update the server list
				TiXmlElement* server_list = root->FirstChildElement("server_list");
				if(server_list)
				{
					int version = 0;
					const char* attribute = server_list->Attribute("version", &version);

					// if the list version is newer, update the xml locations
					if(attribute && (server_list_version < version))
					{
						server_list_version = version;

						LoadXmlServers(server_list);
					}
				}

				// the xml data has been parsed so is no longer needed
				delete m_xml_sources[i].data;
				m_xml_sources[i].data = NULL;
				m_xml_sources[i].request_get_attempted = false;
				m_xml_sources[i].request_get_completed = false;
				m_xml_sources[i].request_get_succeeded = false;
				m_xml_sources[i].request_id = 0;
			}

			UpdateState();
		}

		/*!
		 * \brief
		 * Updates the state the version checker is in.
		 * 
		 * Updates the state of the component after the version check is complete.
		 */
		void		c_version_check_manager::UpdateState()
		{
			// this point is only reached once the update attempts are complete
			m_states.checked_this_session = true;
			
			// set the last date checked to todays date
			time_t time_today;
			time(&time_today);
			tm* local_time = localtime(&time_today);

			m_states.last_checked_day = local_time->tm_mday;
			m_states.last_checked_month = local_time->tm_mon;
			m_states.last_checked_year = 1900 + local_time->tm_year;

			// if there is a new version available, update the display manager with the new version string
			// and display the version to the user
			if((m_current_version.m_major < m_available_version.m_major) ||
				(m_current_version.m_minor < m_available_version.m_minor) ||
				(m_current_version.m_build < m_available_version.m_build))
			{
				m_states.is_new_version = true;

				wchar_t available_string[32];
				swprintf_s(available_string, 32, L"v%i.%i.%i available!", m_available_version.m_major, m_available_version.m_minor, m_available_version.m_build);
				c_version_display_manager::g_instance.SetAvailableVersionString(available_string);

				c_version_display_manager::g_instance.StartUpdateDisplay(20);
			}
		}
	};};
};
#endif