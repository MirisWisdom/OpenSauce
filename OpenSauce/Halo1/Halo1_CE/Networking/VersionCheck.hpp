/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#ifdef YELO_VERSION_CHECK_ENABLE
#include <YeloLib/memory/linked_list.hpp>

#include "Networking/HTTP/HTTP.hpp"
#include "Networking/HTTP/HTTPClient.hpp"
#include "Networking/HTTP/c_xml_downloader.hpp"

namespace Yelo
{
	namespace Networking { namespace VersionCheck
	{
		void		Initialize();
		void		Dispose();

		void		InitializeForNewMap();
		void		Update(real delta_time);

		void*		RequestCompleted_Callback(const bool download_succeeded,
			const char* buffer,
			const GHTTPByteCount buffer_length,
			void* component_data);
		void*		RequestCancelled_Callback(void* component_data);


		struct s_version{
			int	m_major;
			int	m_minor;
			int	m_build;
			char m_build_date[32];

			void SetBuild(const int major, const int minor, const int build)
			{
				m_major = major;
				m_minor = minor;
				m_build = build;
			}

			void SetBuild(const s_version& other)
			{
				m_major = other.m_major;
				m_minor = other.m_minor;
				m_build = other.m_build;
			}

			void SetBuildDate(const char* build_date)
			{
				if(strlen(build_date) >= NUMBEROF(m_build_date))
				{
					m_build_date[0] = 0;
				}
				else
				{
					strcpy_s(m_build_date, build_date);
				}
			}

			void SetBuildDate(const s_version& other)
			{
				SetBuildDate(other.m_build_date);
			}

			int CompareTo(const s_version& other) const
			{
				// -1 if less than, plus 1 if more than 0 if equal
				if(m_major > other.m_major)
					return -1;
				else if(m_major < other.m_major)
					return 1;

				if(m_minor > other.m_minor)
					return -1;
				else if (m_minor < other.m_minor)
					return 1;

				if(m_build > other.m_build)
					return -1;
				else if (m_build < other.m_build)
					return 1;

				return 0;
			}
		};

		class c_version_server : public LinkedListNode<c_version_server>
		{
		public:
			HTTP::http_url_t	m_version_xml_url;

			c_version_server(const char* url)
			{
				ClearNodeData();

				strcpy_s(m_version_xml_url, sizeof(m_version_xml_url), url);
			}
		};

		class c_version_downloader : public HTTP::Client::c_xml_downloader
		{
			struct
			{
				s_version m_version;

				struct
				{
					int list_version;

					c_version_server* version_url_list;
				}m_servers;
			}m_version_downloader;

		protected:
			void GetArguments(s_request_args& request_args)
			{
				c_xml_downloader::GetArguments(request_args);

				request_args.m_component = Enums::_http_client_component_version_checker;
				request_args.m_component_data = this;
				request_args.m_request_timeout = 1.0f;
			}

		public:
			s_version OSVersion() { return m_version_downloader.m_version; }
			int URLListVersion() { return m_version_downloader.m_servers.list_version; }
			c_version_server*& VersionURLList() { return m_version_downloader.m_servers.version_url_list; }

			void Ctor()
			{
				c_xml_downloader::Ctor();

				m_version_downloader.m_version.m_major = 0;
				m_version_downloader.m_version.m_minor = 0;
				m_version_downloader.m_version.m_build = 0;
				m_version_downloader.m_version.m_build_date[0] = 0;

				m_version_downloader.m_servers.version_url_list = nullptr;
				m_version_downloader.m_servers.list_version = 0;
			}

			void Dtor()
			{
				c_xml_downloader::Dtor();

				m_version_downloader.m_version.m_major = 0;
				m_version_downloader.m_version.m_minor = 0;
				m_version_downloader.m_version.m_build = 0;
				m_version_downloader.m_version.m_build_date[0] = 0;

				if(m_version_downloader.m_servers.version_url_list)
					DeleteLinkedList(m_version_downloader.m_servers.version_url_list);
				m_version_downloader.m_servers.version_url_list = nullptr;
				m_version_downloader.m_servers.list_version = 0;
			}

			bool ProcessVersionDocument()
			{
				bool success = false;

				TiXmlElement* root = nullptr;
				TiXmlElement* version = nullptr;
				TiXmlElement* server_list = nullptr;
				TiXmlElement* server = nullptr;

				//do-while-false for easy fall-through
				do
				{
					root = Document().FirstChildElement("osVersionUpdate");
					if(!root) break;

					// process all of the version elements, getting the latest one
					version = root->FirstChildElement("version");
					if(!version) break;

					if(!version->Attribute("major", &m_version_downloader.m_version.m_major)) break;
					if(!version->Attribute("minor", &m_version_downloader.m_version.m_minor)) break;
					if(!version->Attribute("build", &m_version_downloader.m_version.m_build)) break;

					const char* build_date = version->GetText();
					if(!is_null_or_empty(build_date))
					{
						m_version_downloader.m_version.SetBuildDate(build_date);
					}

					// update the server list
					server_list = root->FirstChildElement("server_list");
					if(!server_list) break;
					
					if(!server_list->Attribute("version", &m_version_downloader.m_servers.list_version)) break;

					server = server_list->FirstChildElement("server");
					if(!server) break;

					success = true;
				}while(false);

				if(!success)
					return false;

				do
				{
					const char* url = server->GetText();

					if(!is_null_or_empty(url))
					{
						if(strlen(url) >= sizeof(HTTP::http_url_t))
							continue;

						c_version_server* version_server = new c_version_server(url);
						AppendLinkedListNode(m_version_downloader.m_servers.version_url_list, version_server);
					}
				}while(server = server->NextSiblingElement("server"));

				return true;
			}

			void* DownloadCompleteCallback(const bool download_succeeded, const char* buffer, const GHTTPByteCount buffer_length, void* component_data)
			{
				c_xml_downloader::DownloadCompleteCallback(download_succeeded, buffer, buffer_length, component_data);

				if((Status() == Enums::_http_file_download_status_succeeded) && !ProcessVersionDocument())
					Status() = Enums::_http_file_download_status_failed;
				else
					Status() = Enums::_http_file_download_status_succeeded;

				return NULL;
			}

			void* DownloadCancelledCallback(void* component_data)
			{
				c_xml_downloader::DownloadCancelledCallback(component_data);

				Status() = Enums::_http_file_download_status_cancelled;

				return NULL;
			}
		};

		/*!
		 * \brief
		 * Checks for a newer OS version and informs the user.
		 * 
		 * The version check manager will download an XML file from
		 * a predetermined location, which contains the currently
		 * available version of OpenSauce. It will then display this
		 * to the user if a newer version is available.
		 */
		class c_version_check_manager_base
			abstract
		{
		private:
			/// A hardcoded fallback xml location used when no other location is provided.
			static std::string g_fallback_xml_location;

		public:
			/** Returns a reference to a static instance of c_version_check_manager_base. */
			static c_version_check_manager_base& VersionChecker();

		protected:
			struct {
				/// Has the version xml been downloaded today
				bool checked_today;
				/// Is there a request in progress
				bool is_request_in_progress;

				bool is_new_version_available;
				PAD8;
			}m_states;

			c_version_downloader m_xml_sources[3];
			/// The version of the current OS installation
			s_version m_current_version;
			/// The version of OS that is available online
			s_version m_available_version;

		public:
			virtual void	Initialize();
			virtual void	Dispose();

			virtual void	InitializeForNewMap() {}
			virtual void	Update(real delta_time);

			void*		RequestCompleted_Callback(const bool download_succeeded,
				const char* buffer,
				const GHTTPByteCount buffer_length,
				void* component_data);
			void*		RequestCancelled_Callback(void* component_data);

			virtual void	TestForUpdate();

			bool IsNewVersionAvailable();
			const s_version& CurrentVersion();
			const s_version& AvailableVersion();

		protected:
			void			UpdateDateState();
			void			CheckForUpdates();
			void			ProcessVersionXml();
			virtual void	UpdateVersion();
		};
	}; };
};

#else

namespace Yelo
{
	namespace Networking { namespace VersionCheck
	{
		void		Initialize();
		void		Dispose();

#ifdef PLATFORM_IS_USER
		void		Initialize3D(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters);
		void		OnLostDevice();
		void		OnResetDevice(D3DPRESENT_PARAMETERS* pParameters);
		void		Render();
		void		Release();
#endif

		void		InitializeForNewMap();
		void		Update(real delta_time);
	}; };
};
#endif