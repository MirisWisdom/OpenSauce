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
			"http://open-sauce.googlecode.com/hg/OpenSauce/Halo1/Halo1_CE/Halo1_CE_Version.xml";

		/////////////////////////////////////////
		//static functions
		c_version_check_manager_base& c_version_check_manager_base::VersionChecker() { return g_instance; }

		GHTTPBool	c_version_check_manager_base::GetRequestComplete(GHTTPRequest request,
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
		 * Initialises the ghttp library.
		 *
		 * Starts up the ghttp library and sets the initial build version.
		 */
		void		c_version_check_manager_base::Initialize()
		{
			ghttpStartup();

			m_current_version.SetBuild(2, 5, 0);
			m_available_version.SetBuild(2, 5, 0);

			// when no requests are made and a new map is loaded GS asserts as 0 is a valid connection id
			// -1 is not so we check against this later to avoid trying to close a non existant request
			for(int32 i = 0; i < NUMBEROF(m_xml_sources); i++)
				m_xml_sources[i].request_id = -1;
		}
		/*!
		 * \brief
		 * Disposes of the ghttp library.
		 * 
		 * Disposes of the ghttp library.
		 */
		void		c_version_check_manager_base::Dispose()
		{
			ghttpCleanup();

			for(int32 i = 0; i < NUMBEROF(m_xml_sources); i++)
			{
				delete m_xml_sources[i].xml_address;
				m_xml_sources[i].xml_address = NULL;
			}
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
		void		c_version_check_manager_base::LoadSettings(TiXmlElement* xml_element)
		{
			m_states.last_checked_day = 0;
			m_states.last_checked_month = 0;
			m_states.last_checked_year = 0;
			// if there is no settings element, we still want to set the default
			// xml location
			if(xml_element == NULL)
			{
				LoadXmlServers(NULL);
				return;
			}

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

			UpdateDateState();

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
		void		c_version_check_manager_base::SaveSettings(TiXmlElement* xml_element)
		{
			//set the attributes for the last date the version was checked
			xml_element->SetAttribute("day", m_states.last_checked_day);
			xml_element->SetAttribute("month", m_states.last_checked_month);
			xml_element->SetAttribute("year", m_states.last_checked_year);

			//save the current file locations to the user settings
			TiXmlElement* server_list = new TiXmlElement("server_list");
			for(int32 i = 0; i < NUMBEROF(m_xml_sources); i++)
			{
				if(is_null_or_empty(m_xml_sources[i].xml_address))
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
		 * Updates the ghttp library.
		 * 
		 * \param delta_time
		 * Time in seconds that has passed since the last update.
		 * 
		 * Updates the ghttp library to progress any requests.
		 */
		void		c_version_check_manager_base::Update(real delta_time)
		{	
			// update the ghttp library so that it can update the requests
			if(m_states.is_request_in_progress)
				ghttpThink();
		}
		/*!
		 * \brief
		 * Reads new xml locations from an xml element.
		 * 
		 * Reads new xml locations from an xml element. Currently the
		 * maximum number of locations is 3.
		 */
		void		c_version_check_manager_base::LoadXmlServers(TiXmlElement* server_list)
		{	
			bool has_valid_source = false;

			if(server_list)
			{
				TiXmlElement* server = server_list->FirstChildElement("server");
				for(int32 i = 0; i < NUMBEROF(m_xml_sources); i++)
				{
					// delete the previous locations regardless of whether there is a replacement
					delete m_xml_sources[i].xml_address;
					m_xml_sources[i].xml_address = NULL;

					if(server == NULL) continue;

					// get the text value of each server
					cstring text_value = server->GetText();
					if( !is_null_or_empty(text_value) )
					{
						int text_length = strlen(text_value) + 1;
						m_xml_sources[i].xml_address = new char[text_length];
						memcpy_s(m_xml_sources[i].xml_address, text_length, text_value, text_length);

						server = server->NextSiblingElement("server");
						has_valid_source = true;
					}
				}
			}
			// if there are no server list entries, set index 0 to use the fallback
			if(!has_valid_source)
			{
				int text_length = strlen(g_fallback_xml_location) + 1;
				m_xml_sources[0].xml_address = new char[text_length];
				memcpy_s(m_xml_sources[0].xml_address, text_length, g_fallback_xml_location, text_length);
			}
		}
		/*!
		 * \brief
		 * Starts the update checking process.
		 * 
		 * Sends the get requests for the xml files from the xml locations.
		 */
		void		c_version_check_manager_base::CheckForUpdates(bool do_blocking)
		{
			m_states.is_request_in_progress = true;

			// if we are blocking, request_get_attempted must be set to true on 
			// all valid source objects before any requests are made, so that
			// the update process does not continue until all requests are complete
			if(do_blocking)
			{
				for(int i = 0; i < NUMBEROF(m_xml_sources); i++)
					if(m_xml_sources[i].xml_address)
						m_xml_sources[i].request_get_attempted = true;
			}

			// send a get request to the server using the ghttp library
			// if do_blocking is false this will return immediately but will call 
			// the request complete callback once the get request is complete, which
			// will then continue the update checking process.
			// if do_blocking is true the function will not return until the request
			// is complete
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
						do_blocking ? GHTTPTrue : GHTTPFalse,
						NULL,
						GetRequestComplete,
						&m_xml_sources[i]);
					m_xml_sources[i].request_get_attempted = true;
				}
			}
		}		
		/*!
		 * \brief
		 * Processes the downloaded xml files.
		 * 
		 * Once all of the file requests are complete, this function parses the xml files
		 * to get the latest version that is available, and updates the xml file locations.
		 */
		void		c_version_check_manager_base::ProcessVersionXml()
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
					int source_server_list_version = 0;
					const char* attribute = server_list->Attribute("version", &source_server_list_version);

					// if the list version is newer, update the xml locations
					if(attribute && (server_list_version < source_server_list_version))
					{
						server_list_version = source_server_list_version;

						LoadXmlServers(server_list);
					}
				}

				// the xml data has been parsed so is no longer needed
				delete m_xml_sources[i].data;
				m_xml_sources[i].data = NULL;
				m_xml_sources[i].request_get_attempted = false;
				m_xml_sources[i].request_get_completed = false;
				m_xml_sources[i].request_get_succeeded = false;
				m_xml_sources[i].request_id = -1;
			}

			UpdateState();
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
		 * Updates the state the version checker is in.
		 * 
		 * Updates the state of the component after the version check is complete.
		 */
		void		c_version_check_manager_base::UpdateState()
		{
			// this point is only reached once the update attempts are complete
			m_states.is_request_in_progress = false;
			
			// set the last date checked to todays date
			time_t time_today;
			time(&time_today);
			tm* local_time = localtime(&time_today);

			m_states.last_checked_day = local_time->tm_mday;
			m_states.last_checked_month = local_time->tm_mon;
			m_states.last_checked_year = 1900 + local_time->tm_year;

			m_states.is_new_version =	(m_current_version.m_major < m_available_version.m_major) ||
										(m_current_version.m_minor < m_available_version.m_minor) ||
										(m_current_version.m_build < m_available_version.m_build);
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