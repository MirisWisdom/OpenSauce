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

#include <YeloLib/Halo1/open_sauce/yelo_version.hpp>

#include <YeloLib/configuration/c_configuration_value.hpp>
#include <YeloLib/configuration/c_configuration_value_list.hpp>
#include <YeloLib/open_sauce/settings/c_settings_singleton.hpp>

#include "Networking/HTTP/HTTPClient.hpp"
#if PLATFORM_IS_USER
	#include "Networking/VersionCheckClient.hpp"
#elif PLATFORM_IS_DEDI
	#include "Networking/VersionCheckDedi.hpp"
#endif

// TODO: finish reimplementing for loops as ranged for, where applicable

namespace Yelo
{
	namespace Networking { namespace VersionCheck
	{
#pragma region Settings
		class c_settings_container
			: public Configuration::c_configuration_container
		{
		public:
			class c_version_check_date
				: public Configuration::c_configuration_container
			{
			public:
				Configuration::c_configuration_value<int32> m_day;
				Configuration::c_configuration_value<int32> m_month;
				Configuration::c_configuration_value<int32> m_year;

				c_version_check_date()
					: Configuration::c_configuration_container("Date")
					, m_day("Day", 0)
					, m_month("Month", 0)
					, m_year("Year", 0)
				{ }
				
			protected:
				const std::vector<i_configuration_value* const> GetMembers() final override
				{
					return std::vector<i_configuration_value* const>
					{
						&m_day,
						&m_month,
						&m_year
					};
				}
			};
			class c_version_check_server_list
				: public Configuration::c_configuration_container
			{
			public:
				Configuration::c_configuration_value<int32> m_version;
				Configuration::c_configuration_value_list<std::string> m_servers;

				c_version_check_server_list()
					: Configuration::c_configuration_container("ServerList")
					, m_version("Version", 0)
					, m_servers("Server", "")
				{ }
				
			protected:
				const std::vector<i_configuration_value* const> GetMembers() final override
				{
					return std::vector<i_configuration_value* const>
					{
						&m_version,
						&m_servers
					};
				}
			};

			c_version_check_date m_last_checked;
			c_version_check_server_list m_server_list;

			c_settings_container()
				: Configuration::c_configuration_container("Networking.VersionCheck")
				, m_last_checked()
				, m_server_list()
			{ }
			
		protected:
			const std::vector<i_configuration_value* const> GetMembers() final override
			{
				return std::vector<i_configuration_value* const>
				{
					&m_last_checked,
					&m_server_list
				};
			}
		};

		class c_settings_version_check
			: public Settings::c_settings_singleton<c_settings_container, c_settings_version_check>
		{
		public:
			void PostLoad() final override
			{
				c_version_check_manager_base::VersionChecker().TestForUpdate();
			}
		};
#pragma endregion

		/////////////////////////////////////////
		//namespace functions
		void		Initialize()
		{
			c_settings_version_check::Register(Settings::Manager());

			c_version_check_manager_base::VersionChecker().Initialize();
		}
		void		Dispose()
		{
			c_version_check_manager_base::VersionChecker().Dispose();

			c_settings_version_check::Unregister(Settings::Manager());
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
#elif PLATFORM_IS_DEDI
		static c_version_check_manager_dedi		g_instance;
#endif

		/////////////////////////////////////////
		//c_version_check_manager_base
		/////////////////////////////////////////
		std::string						c_version_check_manager_base::g_fallback_xml_location = 
			"http://os.halomods.com/Halo1/CE/Halo1_CE_Version.xml";

		/////////////////////////////////////////
		//static functions
		c_version_check_manager_base& c_version_check_manager_base::VersionChecker() { return g_instance; }

		void*	c_version_check_manager_base::RequestCompleted_Callback(const bool download_succeeded, const char* buffer, const GHTTPByteCount buffer_length, void* component_data)
		{
			YELO_ASSERT_DISPLAY(component_data, "the component data for a http request is NULL");

			auto* source = CAST_PTR(c_version_downloader*, component_data);

			source->DownloadCompleteCallback(download_succeeded, buffer, buffer_length, component_data);

			return "";
		}

		void*	c_version_check_manager_base::RequestCancelled_Callback(void* component_data)
		{
			YELO_ASSERT_DISPLAY(component_data, "the component data for a http request is NULL");

			auto* source = CAST_PTR(c_version_downloader*, component_data);

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
			m_current_version.SetBuildDate(K_OPENSAUCE_BUILD_DATE_STR);
			m_available_version.SetBuild(K_OPENSAUCE_VERSION_BUILD_MAJ, K_OPENSAUCE_VERSION_BUILD_MIN, K_OPENSAUCE_VERSION_BUILD);
			m_available_version.SetBuildDate("");

			// when no requests are made and a new map is loaded GS asserts as 0 is a valid connection id
			// -1 is not so we check against this later to avoid trying to close a non existent request
			for (auto& xml_source : m_xml_sources)
				xml_source.Ctor();
		}

		/*!
		 * \brief
		 * Disposes of the xml downloaders.
		 * 
		 * Disposes of the xml downloaders.
		 */
		void		c_version_check_manager_base::Dispose()
		{
			for(auto& xml_source : m_xml_sources)
			{
				xml_source.Stop();
				xml_source.Dtor();
			}
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
			for(auto& xml_source : m_xml_sources)
			{
				xml_source.Update(delta_time);
				m_states.is_request_in_progress |= (xml_source.Status() == Enums::_http_file_download_status_in_progress);
			}

			// if all of the requests are complete, update the clients xml sources
			if(!m_states.is_request_in_progress)
				UpdateVersion();
		}

		void		c_version_check_manager_base::TestForUpdate()
		{
			UpdateDateState();
		}

		bool		c_version_check_manager_base::IsNewVersionAvailable()
		{
			return m_states.is_new_version_available;
		}

		const s_version& c_version_check_manager_base::CurrentVersion()
		{
			return m_current_version;
		}

		const s_version& c_version_check_manager_base::AvailableVersion()
		{
			return m_available_version;
		}

		/*!
		 * \brief
		 * Starts the update checking process.
		 * 
		 * Sends the requests for the xml files from the xml urls.
		 */
		void		c_version_check_manager_base::CheckForUpdates()
		{
			auto& settings_instance = c_settings_version_check::Instance().Get();
			m_states.is_request_in_progress = false;
			
			bool has_url = false;
			for(auto url : settings_instance.m_server_list.m_servers)
			{
				if(!url.Get().empty())
				{
					has_url = true;
				}
			}

			if(!has_url)
			{
				settings_instance.m_server_list.m_servers.Clear();
				settings_instance.m_server_list.m_servers.AddEntry() = g_fallback_xml_location;
			}

			// start the xml downloaders
			int downloader_index = 0;
			for(auto url : settings_instance.m_server_list.m_servers)
			{
				if(downloader_index >= NUMBEROF(m_xml_sources))
				{
					break;
				}

				m_xml_sources[downloader_index].SetURL(url.Get().c_str());
				m_xml_sources[downloader_index].Start();

				m_states.is_request_in_progress |= (m_xml_sources[downloader_index].Status() == Enums::_http_file_download_status_in_progress);

				downloader_index++;
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
			tm local_time; localtime_s(&local_time, &time_today);
			
			auto& settings_instance = c_settings_version_check::Instance().Get();
			if( (settings_instance.m_last_checked.m_day == local_time.tm_mday) &&
				(settings_instance.m_last_checked.m_month == local_time.tm_mon + 1) &&
				(settings_instance.m_last_checked.m_year == 1900 + local_time.tm_year))
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
			tm local_time; localtime_s(&local_time, &time_today);

			auto& settings_instance = c_settings_version_check::Instance().Get();

			settings_instance.m_last_checked.m_day = local_time.tm_mday;
			settings_instance.m_last_checked.m_month = local_time.tm_mon + 1;
			settings_instance.m_last_checked.m_year = 1900 + local_time.tm_year;

			for(auto& downloader : m_xml_sources)
			{
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
				if(settings_instance.m_server_list.m_version < downloader.URLListVersion())
				{
					settings_instance.m_server_list.m_servers.Clear();
					settings_instance.m_server_list.m_version = downloader.URLListVersion();

					LinkedListIterator<c_version_server> url_iterator(downloader.VersionURLList());

					if(url_iterator.MoveNext())
					{
						std::string url_string(url_iterator.Current()->m_version_xml_url);
						settings_instance.m_server_list.m_servers.AddEntry() = url_string;
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

		void		InitializeForNewMap() {}
		void		Update(real delta_time) {}
	}; };
};
#endif