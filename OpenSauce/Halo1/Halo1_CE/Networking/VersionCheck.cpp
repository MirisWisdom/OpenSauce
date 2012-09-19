/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Networking/VersionCheck.hpp"

#ifdef YELO_VERSION_CHECK_ENABLE

#include <time.h>
#include <ctime>

#include <Common/Halo1/YeloSettingsVersion.hpp>

#include "Networking/HTTP/HTTPClient.hpp"
#if PLATFORM_IS_USER
	#include "Networking/VersionCheckClient.hpp"
#elif PLATFORM_IS_DEDI
	#include "Networking/VersionCheckDedi.hpp"
#endif

namespace Yelo
{
	namespace Networking { namespace VersionCheck
	{
		/////////////////////////////////////////
		//namespace functions
		void		Initialize()
		{
			c_version_check_manager_base::VersionChecker().Initialize();
		}
		void		Dispose()
		{
			c_version_check_manager_base::VersionChecker().Dispose();
		}

		void		LoadSettings(TiXmlElement* vc_element)
		{
			c_version_check_manager_base::VersionChecker().LoadSettings(vc_element);
		}
		void		SaveSettings(TiXmlElement* vc_element)
		{
			c_version_check_manager_base::VersionChecker().SaveSettings(vc_element);
		}

		void		InitializeForNewMap()
		{
			c_version_check_manager_base::VersionChecker().InitializeForNewMap();
		}
		void		Update(real delta_time)
		{
			c_version_check_manager_base::VersionChecker().Update(delta_time);
		}

		void*		RequestCompleted_Callback(const bool download_succeeded,
			const char* buffer,
			const GHTTPByteCount buffer_length,
			void* component_data)
		{
			return c_version_check_manager_base::VersionChecker().RequestCompleted_Callback(download_succeeded,
				buffer,
				buffer_length,
				component_data);
		}

		void*		RequestCancelled_Callback(void* component_data)
		{
			return c_version_check_manager_base::VersionChecker().RequestCancelled_Callback(component_data);
		}

#if PLATFORM_IS_USER
		static c_version_check_manager_user		g_instance;
		
		void		Initialize3D(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters)
		{
			g_instance.Initialize3D(pDevice, pParameters);
		}
		void		OnLostDevice()
		{
			g_instance.OnLostDevice();
		}
		void		OnResetDevice(D3DPRESENT_PARAMETERS* pParameters)
		{
			g_instance.OnResetDevice(pParameters);
		}
		void		Render()
		{
			g_instance.Render();
		}
		void		Release()
		{
			g_instance.Release();
		}
#elif PLATFORM_IS_DEDI
		static c_version_check_manager_dedi		g_instance;
#endif

		/////////////////////////////////////////
		//c_version_check_manager_base
		/////////////////////////////////////////
		cstring						c_version_check_manager_base::g_fallback_xml_location = 
			"http://www.thefieryscythe.co.uk/Halo1_CE_Version.xml";

		/////////////////////////////////////////
		//static functions
		c_version_check_manager_base& c_version_check_manager_base::VersionChecker() { return g_instance; }

		void*	c_version_check_manager_base::RequestCompleted_Callback(const bool download_succeeded, const char* buffer, const GHTTPByteCount buffer_length, void* component_data)
		{
			ASSERT(!component_data, "the component data for a http request is NULL");

			c_version_downloader* source = CAST_PTR(c_version_downloader*, component_data);

			source->DownloadCompleteCallback(download_succeeded, buffer, buffer_length, component_data);

			return "";
		}

		void*	c_version_check_manager_base::RequestCancelled_Callback(void* component_data)
		{
			ASSERT(!component_data, "the component data for a http request is NULL");

			c_version_downloader* source = CAST_PTR(c_version_downloader*, component_data);

			source->DownloadCancelledCallback(component_data);

			return "";
		}

		/////////////////////////////////////////
		//non-static functions
		/*!
		 * \brief
		 * Initializes the xml downloaders.
		 *
		 * Initializes the xml downloaders and sets the initial build version.
		 */
		void		c_version_check_manager_base::Initialize()
		{
			m_current_version.SetBuild(K_OPENSAUCE_VERSION_BUILD_MAJ, K_OPENSAUCE_VERSION_BUILD_MIN, K_OPENSAUCE_VERSION_BUILD);
			m_available_version.SetBuild(K_OPENSAUCE_VERSION_BUILD_MAJ, K_OPENSAUCE_VERSION_BUILD_MIN, K_OPENSAUCE_VERSION_BUILD);

			// when no requests are made and a new map is loaded GS asserts as 0 is a valid connection id
			// -1 is not so we check against this later to avoid trying to close a non existant request
			for(int32 i = 0; i < NUMBEROF(m_xml_sources); i++)
				m_xml_sources[i].Ctor();
		}

		/*!
		 * \brief
		 * Disposes of the xml downloaders.
		 * 
		 * Disposes of the xml downloaders.
		 */
		void		c_version_check_manager_base::Dispose()
		{
			for(int32 i = 0; i < NUMBEROF(m_xml_sources); i++)
				m_xml_sources[i].Dtor();
		}

		/*!
		 * \brief
		 * Loads the server list from the user settings.
		 * 
		 * \param xml_element
		 * Pointer to an xml element that parents the version checkers settings.
		 * 
		 * Loads the server list and the date the available version was last checked on, from the users settings.
		 */
		void		c_version_check_manager_base::LoadSettings(TiXmlElement* xml_element)
		{
			m_states.last_checked_day = 0;
			m_states.last_checked_month = 0;
			m_states.last_checked_year = 0;

			for(int i = 0; i < NUMBEROF(m_version_xml.urls); i++)
				m_version_xml.urls[i][0] = 0;

			// if there is no settings element, we still want to set the default
			// xml location
			if(xml_element == NULL)
			{
				m_version_xml.list_version = 0;
				strcpy_s(m_version_xml.urls[0], sizeof(HTTP::t_http_url), c_version_check_manager_base::g_fallback_xml_location);
				return;
			}

			//do-while-false for easy fall-through
			do
			{
				//get the last date the version was checked
				if(!xml_element->Attribute("day", &m_states.last_checked_day)) { m_states.last_checked_day = 0; break; }
				if(!xml_element->Attribute("month", &m_states.last_checked_month)) { m_states.last_checked_month = 0; break; }
				if(!xml_element->Attribute("year", &m_states.last_checked_year)) { m_states.last_checked_year = 0; break; }
			}while(false);

			UpdateDateState();

			//get the xml locations from the user settings
			TiXmlElement* server_list = xml_element->FirstChildElement("server_list");
			if(!server_list)
				return;

			if(!server_list->Attribute("version", &m_version_xml.list_version))
				return;

			TiXmlElement* server = server_list->FirstChildElement("server");
			if(!server)
				return;

			for(int i = 0; server && (i < NUMBEROF(m_version_xml.urls)); i++)
			{
				const char* url = server->GetText();
				if(!is_null_or_empty(url))
					strcpy_s(m_version_xml.urls[i], sizeof(HTTP::t_http_url), url);

				server = server->NextSiblingElement("server");
			}
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
		void		c_version_check_manager_base::SaveSettings(TiXmlElement* xml_element)
		{
			//set the attributes for the last date the version was checked
			xml_element->SetAttribute("day", m_states.last_checked_day);
			xml_element->SetAttribute("month", m_states.last_checked_month);
			xml_element->SetAttribute("year", m_states.last_checked_year);

			//save the current file urls to the user settings
			TiXmlElement* server_list = new TiXmlElement("server_list");
			server_list->SetAttribute("version", m_version_xml.list_version);

			for(int32 i = 0; i < NUMBEROF(m_version_xml.urls); i++)
			{
				if(is_null_or_empty(m_version_xml.urls[i]))
					continue;

				TiXmlElement* server = new TiXmlElement("server");
				TiXmlText* server_address = new TiXmlText(m_version_xml.urls[i]);

				server->LinkEndChild(server_address);
				server_list->LinkEndChild(server);
			}
			xml_element->LinkEndChild(server_list);
		}

		/*!
		 * \brief
		 * Updates the xml downloaders.
		 * 
		 * \param delta_time
		 * Time in seconds that has passed since the last update.
		 * 
		 * Updates the xml downloaders.
		 */
		void		c_version_check_manager_base::Update(real delta_time)
		{
			if(!m_states.is_request_in_progress)
				return;

			m_states.is_request_in_progress = false;
			for(int32 i = 0; i < NUMBEROF(m_xml_sources); i++)
			{
				m_xml_sources[i].Update(delta_time);
				m_states.is_request_in_progress |= (m_xml_sources[i].Status() == Enums::_http_file_download_status_in_progress);
			}

			// if all of the requests are complete, update the clients xml sources
			if(!m_states.is_request_in_progress)
				UpdateVersion();
		}

		/*!
		 * \brief
		 * Starts the update checking process.
		 * 
		 * Sends the requests for the xml files from the xml urls.
		 */
		void		c_version_check_manager_base::CheckForUpdates()
		{
			m_states.is_request_in_progress = false;

			// start the xml downloaders
			for(int i = 0; i < NUMBEROF(m_xml_sources); i++)
			{
				// if the xml source is empty, don't start the download
				if(is_null_or_empty(m_version_xml.urls[i]))
					continue;

				m_xml_sources[i].SetURL(m_version_xml.urls[i]);
				m_xml_sources[i].Start();

				m_states.is_request_in_progress |= (m_xml_sources[i].Status() == Enums::_http_file_download_status_in_progress);
			}
		}

		/*!
		 * \brief
		 * Updates the m_states.checked_today flag.
		 * 
		 * Updates the m_states.checked_today flag.
		 */
		void		c_version_check_manager_base::UpdateDateState()
		{
			// determine whether the version has already been checked today
			time_t time_today;
			time(&time_today);
			tm* local_time = localtime(&time_today);

			if((m_states.last_checked_day == local_time->tm_mday) &&
				(m_states.last_checked_month == local_time->tm_mon) &&
				(m_states.last_checked_year == 1900 + local_time->tm_year))
				m_states.checked_today = true;
		}

		/*!
		 * \brief
		 * Updates the available os version.
		 * 
		 * Updates the available os version from the data that has been downloaded.
		 */
		void		c_version_check_manager_base::UpdateVersion()
		{			
			// set the last date checked to todays date
			time_t time_today;
			time(&time_today);
			tm* local_time = localtime(&time_today);

			m_states.last_checked_day = local_time->tm_mday;
			m_states.last_checked_month = local_time->tm_mon;
			m_states.last_checked_year = 1900 + local_time->tm_year;

			for(int i = 0; i < NUMBEROF(m_xml_sources); i++)
			{
				c_version_downloader& downloader = m_xml_sources[i];

				// clean up the download request
				if(downloader.Status() != Enums::_http_file_download_status_idle)
					downloader.Stop();

				if(downloader.Status() != Enums::_http_file_download_status_succeeded)
					continue;

				// see whether the downloaded version is newer than the current
				if(m_available_version.CompareTo(downloader.OSVersion()) > 0)
				{
					m_available_version = downloader.OSVersion();
					m_states.is_new_version_available = true;
				}

				// if the server list version is newer, copy the new list
				if(m_version_xml.list_version < downloader.URLListVersion())
				{
					m_version_xml.list_version = downloader.URLListVersion();

					LinkedListIterator<c_version_server> url_iterator(downloader.VersionURLList());

					for(int j = 0; j < NUMBEROF(m_version_xml.urls); j++)
					{
						m_version_xml.urls[j][0] = 0;

						if(url_iterator.MoveNext())
							strcpy_s(m_version_xml.urls[j], sizeof(HTTP::t_http_url), url_iterator.Current()->m_version_xml_url);
					}
				}
			}
		}
	}; };
};

#else

namespace Yelo
{
	namespace Networking { namespace VersionCheck
	{
		void		Initialize() {}
		void		Dispose() {}

#ifdef PLATFORM_IS_USER
		void		Initialize3D(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters) {}
		void		OnLostDevice() {}
		void		OnResetDevice(D3DPRESENT_PARAMETERS* pParameters) {}
		void		Render() {}
		void		Release() {}
#endif

		void		LoadSettings(TiXmlElement* dx9_element) {}
		void		SaveSettings(TiXmlElement* dx9_element) {}

		void		InitializeForNewMap() {}
		void		Update(real delta_time) {}
	}; };
};
#endif