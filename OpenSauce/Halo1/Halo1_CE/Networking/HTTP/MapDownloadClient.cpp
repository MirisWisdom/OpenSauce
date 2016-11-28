/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Networking/HTTP/MapDownloadClient.hpp"

#if !PLATFORM_IS_DEDI
#include <blamlib/Halo1/main/main.hpp>
#include <blamlib/Halo1/networking/network_client_manager_structures.hpp>
#include <blamlib/Halo1/interface/map_list.hpp>

#include <YeloLib/memory/linked_list.hpp>
#include <YeloLib/memory/compression/7zip_codec.hpp>
#include <YeloLib/memory/compression/zip_codec.hpp>
#include <YeloLib/memory/security/xxtea.hpp>
#include <YeloLib/Halo1/cache/cache_files_yelo.hpp>
#include <YeloLib/cseries/value_conversion.hpp>

#include "Common/FileIO.hpp"
#include "Settings/Settings.hpp"

#include "Game/EngineFunctions.hpp"
#include "Game/GameBuildNumber.hpp"
#include "Interface/Controls.hpp"
#include "Interface/OpenSauceUI/Screen/screen_ids.hpp"
#include "Interface/OpenSauceUI.hpp"
#include "Networking/Networking.hpp"
#include "Networking/Server.hpp"
#include "Memory/FunctionInterface.hpp"
#include "TagGroups/CacheFiles.hpp"

#include "Networking/HTTP/HTTP.hpp"
#include "Networking/HTTP/HTTPClient.hpp"
#include "Networking/HTTP/HTTPServer.hpp"
#include "Networking/HTTP/c_file_downloader.hpp"
#include "Networking/HTTP/c_xml_downloader.hpp"

#include "Networking/HTTP/MapDownloadClientDisplay.hpp"

namespace Yelo
{
	// TODO: set the master server list to add servers directly to the global list rather than copying after the download
	// TODO: comments, check current are correct and add missing comments
	namespace Networking { namespace HTTP { namespace Client { namespace MapDownload
	{
		static const char* g_master_server_list_url = 
#ifdef _DEBUG
			"http://os.halomods.com/Halo1/CE/MapDownload/Halo1_CE_MapDownloadMasterServers.debug.xml";
#else
			"http://os.halomods.com/Halo1/CE/MapDownload/Halo1_CE_MapDownloadMasterServers.xml";
#endif

#pragma region Enums
		enum
		{
			k_server_name_length_max = 64,
			k_server_description_length_max = 256,
			k_md5_string_length_max = 33,
			k_max_download_attempts = 3,
			k_max_part_size = 1048576, // maps are split in to 1MB parts
			k_max_part_download_size = k_max_part_size + 1, // the downloaded data is NULL terminated
			k_server_password_length = 8 + 1,
			k_connection_timeout = 2, // time to wait before cancelling a pending connection
			k_retry_delay_time = 1, // time to wait between download attempts (seconds)
			k_reconnect_delay_time = 2 // time to wait before reconnecting to the server (seconds)
		};

		enum request_type
		{
			_request_type_master_server_list,
			_request_type_map_part_definition,
			_request_type_map_part,

			_request_type
		};

		struct s_request_data
		{
			request_type m_request_type;
			PAD16;
		};

		enum map_compression_format : uint16
		{
			_map_compression_format_zip,
			_map_compression_format_7zip,

			_map_compression_format
		};

		enum map_download_update_stage : byte_enum
		{
			_map_download_update_stage_idle,					// no map downloads in progress
			_map_download_update_stage_start_download,			// map download requested, start the map download thread
			_map_download_update_stage_downloading_map,			// wait for the map download thread to finish
			_map_download_update_stage_map_download_completion,	// close the download thread, add the map to the headers and reconnect to the server
			_map_download_update_stage_failed,					// wait for the user to press esc to cancel the download
			_map_download_update_stage_cancelled,				// clean up all resources and reset
			_map_download_update_stage_complete,				// reset the map download globals and set back to idle

			_map_download_update_stage
		};

		enum download_completion_state : _enum
		{
			_download_completion_state_begin,
			_download_completion_state_reconnect_delay,
			_download_completion_state_reconnect,
			_download_completion_state_complete,

			_download_completion_state
		};

		enum map_download_stage : long_enum
		{
			_map_download_stage_build_server_list,
			_map_download_stage_map_part_definition, // download the part definition
			_map_download_stage_map_part_download, // download the map parts
			_map_download_stage_map_archive_extraction, // extract the map
			_map_download_stage_complete,
			_map_download_stage_failed,
			_map_download_stage_cancelled,

			_map_download_stage
		};

		enum master_server_list_state : _enum
		{
			_master_server_list_state_begin,
			_master_server_list_state_downloading,
			_master_server_list_state_download_failed,
			_master_server_list_state_download_succeeded,
			_master_server_list_state_stage_complete,

			_master_server_list_state
		};

		enum map_part_definition_state : _enum
		{
			_map_part_definition_state_begin,
			_map_part_definition_state_start_download,
			_map_part_definition_state_downloading,
			_map_part_definition_state_download_failed,
			_map_part_definition_state_download_succeeded,
			_map_part_definition_state_stage_complete,

			_map_part_definition_state
		};

		enum part_download_state : _enum
		{
			_part_download_state_begin,
			_part_download_state_prepare_download,
			_part_download_state_ready_to_download,
			_part_download_state_downloading,
			_part_download_state_download_failed,
			_part_download_state_download_complete,
			_part_download_state_part_ready_to_save,
			_part_download_state_stage_complete,

			_part_download_state
		};

		enum archive_extraction_state : _enum
		{
			_archive_extraction_state_begin,
			_archive_extraction_state_archive_ready,
			_archive_extraction_state_complete,

			_archive_extraction_state
		};
#pragma endregion

		class c_part_element : public LinkedListNode<c_part_element>
		{
		public:
			char					m_name[MAX_PATH];
			char					m_md5[k_md5_string_length_max];
			uint32					m_size;
			bool					m_encrypted;
			PAD24;
			char					m_unencrypted_md5[k_md5_string_length_max];
			uint32					m_total_bytes;
			uint32					m_received_bytes;

			void Ctor()
			{
				ClearNodeData();
				m_name[0] = 0;
				m_md5[0] = 0;
				m_encrypted = false;
				m_unencrypted_md5[0] = 0;
				m_total_bytes = 0;
				m_received_bytes = 0;
			}
		};

		class c_map_element
		{
		public:
			char					m_name[MAX_PATH];
			char					m_filename[MAX_PATH];
			char					m_md5[k_md5_string_length_max];
			uint32					m_uncompressed_size;
			uint32					m_compressed_size;
			map_compression_format	m_format;
			bool					m_map_is_yelo;
			PAD8;

			c_part_element*			m_parts;

			void Ctor()
			{
				m_name[0] = 0;
				m_filename[0] = 0;
				m_md5[0] = 0;
				m_format = _map_compression_format;
				m_map_is_yelo = false;

				m_parts = nullptr;
			}

			void Dtor()
			{
				m_name[0] = 0;
				m_filename[0] = 0;
				m_md5[0] = 0;
				m_format = _map_compression_format;
				m_map_is_yelo = false;

				if(m_parts)
				{
					DeleteLinkedList(m_parts);
					m_parts = nullptr;
				}
			}
		};

		class c_http_server : public LinkedListNode<c_http_server>
		{
		public:
			char		m_name[k_server_name_length_max];
			char		m_address[Enums::k_max_url_length];
			char		m_description[k_server_description_length_max];

			virtual void Ctor()
			{
				ClearNodeData();

				m_name[0] = 0;
				m_address[0] = 0;
				m_description[0] = 0;
			}

			virtual void Dtor()
			{
				m_name[0] = 0;
				m_address[0] = 0;
				m_description[0] = 0;
			}

			bool SetHTTPServer(const char* name, const char* address, const char* description)
			{
				m_name[0] = 0;
				m_address[0] = 0;
				m_description[0] = 0;

				if(-1 == strcpy_s(m_name, name))
					return false;

				if(-1 == strcpy_s(m_address, address))
					return false;

				if(-1 == strcpy_s(m_description, description))
					return false;

				return true;
			}
		};

		class c_dedicated_server : public c_http_server
		{
		public:
			char		m_mp_address[Enums::k_max_ip_port_string_length];
			PAD16;
			char		m_mp_password[k_server_password_length];
			PAD24;
			byte		m_mp_password_key[16];
			Enums::network_game_protocol_id	m_mp_gamever;


		protected:
			void ClearBuffers()
			{
				m_mp_address[0] = '\0';
				m_mp_password[0] = '\0';
				ZeroMemory(m_mp_password_key, k_server_password_length+1);
			}

			/*!
			 * \brief
			 * Copies the servers IP and port for downloading the map and reconnecting later.
			 * 
			 * \returns
			 * Returns true if the server addresses were created successfully, otherwise false.
			 * 
			 * Copies the servers IP and port for downloading the map and reconnecting later.
			 */
			bool	SetServerAddresses()
			{
				// get the server address
				const Networking::s_transport_address& server_transport = NetworkGameClient()->server_address;

				bool success = true;
				char server_ip_string[Enums::k_max_ip_port_string_length] = "";

				// set up the server url string
				// generate the ip string for either ipv4 or ipv6
				if(server_transport.address_length == Enums::k_ipv4_address_length)
				{
					if(-1 == sprintf_s(server_ip_string,
						"%i.%i.%i.%i",
						server_transport.ipv4.class_d,
						server_transport.ipv4.class_c,
						server_transport.ipv4.class_b,
						server_transport.ipv4.class_a))
						success = false;
				}
				else
				{
					if(-1 == sprintf_s(server_ip_string,
						"[%04X:%04X:%04X:%04X:%04X:%04X:%04X:%04X]",
						server_transport.ipv6.class_h,
						server_transport.ipv6.class_g,
						server_transport.ipv6.class_f,
						server_transport.ipv6.class_e,
						server_transport.ipv6.class_d,
						server_transport.ipv6.class_c,
						server_transport.ipv6.class_b,
						server_transport.ipv6.class_a))
						success = false;
				}

				// build a string with the ip and port, for reconnecting to the server
				if(-1 == sprintf_s(m_mp_address, "%s:%i", server_ip_string, server_transport.port))
					success = false;

				if(-1 == sprintf_s(server_ip_string, "%s:%i/%s",
					server_ip_string,
					server_transport.port + 1,
					Server::GetServiceURIRoot(Enums::_http_service_enumeration_map_download)))
					success = false;

				if(success)
					success = SetHTTPServer("Dedicated Server", server_ip_string, "This download is being provided to you by the dedicated server host");

				m_mp_gamever = BuildNumber::GetAdvertisedVersion();

				return success;
			}

		public:
			void Ctor()
			{
				c_http_server::Ctor();

				ClearBuffers();
			}

			void Dtor()
			{
				c_http_server::Dtor();

				ClearBuffers();
			}

			/*!
			 * \brief
			 * Gets the server addresses and password for the currently connected dedicated server.
			 * 
			 * \returns
			 * Returns true if the details were collected successfully.
			 * 
			 * Gets the server addresses and password for the currently connected dedicated server.
			 */
			bool SetCurrentDedicatedServer()
			{
				// copy the server password
				m_mp_password[0] = 0;

				if(-1 == sprintf_s(m_mp_password, sizeof(m_mp_password), "%S", Networking::NetworkGameClient()->join_data.parameters.password))
					return false;

				// generate the server password hash
				GSMD5_CTX md5_context;
				GSMD5Init(&md5_context);
				GSMD5Update(&md5_context, CAST_PTR(byte*, &m_mp_password[0]), strlen(m_mp_password));
				GSMD5Final(m_mp_password_key, &md5_context);

				if(!SetServerAddresses())
					return false;

				return true;
			}
		};

		class c_http_server_instance : public LinkedListNode<c_http_server_instance>
		{
			c_http_server*		m_server;
		public:
			c_http_server* Server() { return m_server; }

			c_http_server_instance(c_http_server* server) : m_server(server) { ClearNodeData(); }
		};

		class c_master_server_list : public c_xml_downloader
		{
			struct
			{
				c_http_server**		server_list;
			}m_master_server_list;

			/*!
			 * \brief
			 * Creates a list of servers from the downloaded server list.
			 * 
			 * \returns
			 * True if the file was processed successfully, otherwise returns false.
			 * 
			 * Creates a list of servers from the downloaded server list.
			 */
			bool	ProcessMasterServerList()
			{
				YELO_ASSERT_DISPLAY(m_master_server_list.server_list, "master server list downloader list pointer not set");

				// get the root element
				TiXmlElement* root = Document().FirstChildElement("osHTTPServer");
				if(!root)
					return false;

				// get the first server node
				TiXmlElement* server_node = root->FirstChildElement("map_download_server");
				if(!server_node)
					return false;

				do
				{
					const char* name = server_node->Attribute("name");
					TiXmlElement* address_element = server_node->FirstChildElement("address");
					TiXmlElement* description_element = server_node->FirstChildElement("description");

					if(name && address_element && address_element->GetText())
					{
						c_url_interface address;
						address.ParseURLA(address_element->GetText());

						if(address.m_address.length() == 0)
							continue;

						const char* description;
						if(description_element && description_element->GetText())
							description = description_element->GetText();
						else
							description = "";

						// create a new master server entry
						auto download_server = new c_http_server();
						download_server->Ctor();
						download_server->SetHTTPServer(name, address.GetURL().c_str(), description);

						AppendLinkedListNode(*m_master_server_list.server_list, download_server);
					}
					else
						continue;
				}while(server_node = server_node->NextSiblingElement("map_download_server"));

				return true;
			}

		protected:
			void GetArguments(s_request_args& request_args)
			{
				c_xml_downloader::GetArguments(request_args);

				auto request_data = new s_request_data();
				request_data->m_request_type = _request_type_master_server_list;
				
				request_args.m_component = Enums::_http_client_component_map_download;
				request_args.m_component_data = request_data;

				request_args.m_manual_think = true;
				request_args.m_request_timeout = k_connection_timeout;
			}

		public:
			void SetServerList(c_http_server** server_list) { m_master_server_list.server_list = server_list; }

			void Ctor()
			{
				c_xml_downloader::Ctor();

				m_master_server_list.server_list = nullptr;
			}

			void Dtor()
			{
				c_xml_downloader::Dtor();
				m_master_server_list.server_list = nullptr;
			}

			void* DownloadCompleteCallback(const bool download_succeeded, const char* buffer, const GHTTPByteCount buffer_length, void* component_data)
			{
				c_xml_downloader::DownloadCompleteCallback(download_succeeded, buffer, buffer_length, component_data);

				auto request_data = CAST_PTR(s_request_data*, component_data);
				delete request_data;

				if(Status() != Enums::_http_file_download_status_failed)
				{
					if((Status() == Enums::_http_file_download_status_succeeded) && !ProcessMasterServerList())
						Status() = Enums::_http_file_download_status_failed;
					else
						Status() = Enums::_http_file_download_status_succeeded;
				}

				if((Status() == Enums::_http_file_download_status_failed) && Retry())
					Status() = Enums::_http_file_download_status_retry;

				return nullptr;
			}

			void* DownloadCancelledCallback(void* component_data)
			{
				c_xml_downloader::DownloadCancelledCallback(component_data);

				auto request_data = CAST_PTR(s_request_data*, component_data);
				delete request_data;

				Status() = Enums::_http_file_download_status_cancelled;

				return nullptr;
			}
		};

		class c_map_part_definition : public c_xml_downloader
		{
			struct
			{
				c_map_element map_element;
			}m_map_part_definition;

			/*!
			 * \brief
			 * Populates a c_map_element's part list from a valid xml map element.
			 * 
			 * \param map_element
			 * The xml map element to read part information from.
			 * 
			 * \param map
			 * The map element to add parts to.
			 * 
			 * \returns
			 * True if all of the part information was valid and added to the map element successfully.
			 * 
			 * Populates a c_map_elements part list from a valid xml map element.
			 */
			bool	AddMapParts(const TiXmlElement* map_element, c_map_element& map)
			{
				// if there are no parts, return false
				const TiXmlElement* current_part = map_element->FirstChildElement("part");
				if(!current_part)
					return false;

				do
				{
					const char* name = current_part->Attribute("name");
					const char* md5 = current_part->Attribute("md5");
					const char* size = current_part->Attribute("size");
					const char* encrypted = current_part->Attribute("encrypted");
					const char* unencrypted_md5 = current_part->Attribute("unencrypted_md5");

					bool encrypted_value = false;
					if(encrypted)
					{
						ValueConversion::FromString(encrypted, encrypted_value);
					}

					bool encryption_valid = true;
					if(encrypted_value && !unencrypted_md5)
						encryption_valid = false;

					// every part must have a name and md5 checksum
					if(name && md5 && encryption_valid && size)
					{
						c_part_element* part_element = new c_part_element();

						part_element->Ctor();

						strcpy_s(part_element->m_name, name);
						strcpy_s(part_element->m_md5, md5);
						if(1 != sscanf_s(size, "%i", &part_element->m_size))
							part_element->m_size = 0;
						part_element->m_encrypted = encrypted_value;
						if(encrypted && unencrypted_md5)
							strcpy_s(part_element->m_unencrypted_md5, unencrypted_md5);

						AppendLinkedListNode(map.m_parts, part_element);
					}
					else
					{
						// delete the parts added up to this point
						if(map.m_parts)
							DeleteLinkedList(map.m_parts);
						return false;
					}
				}while(current_part = current_part->NextSiblingElement("part"));

				return true;
			}

		protected:
			void GetArguments(s_request_args& request_args)
			{
				c_xml_downloader::GetArguments(request_args);

				s_request_data* request_data = new s_request_data();
				request_data->m_request_type = _request_type_map_part_definition;
				
				request_args.m_component = Enums::_http_client_component_map_download;
				request_args.m_component_data = request_data;

				request_args.m_manual_think = true;
				request_args.m_request_timeout = k_connection_timeout;
			}

		public:
			void Ctor()
			{
				c_xml_downloader::Ctor();

				m_map_part_definition.map_element.Ctor();
			}

			void Dtor()
			{
				c_xml_downloader::Dtor();

				m_map_part_definition.map_element.Dtor();
			}

			c_map_element& MapElement() { return m_map_part_definition.map_element; }

			/*!
			 * \brief
			 * Processes a downloaded map part definition to get information about a map and its parts.
			 * 
			 * \returns
			 * Returns true if the part definition was processed successfully, otherwise returns false.
			 * 
			 * Processes a downloaded map part definition to get information about a map and its parts.
			 */
			bool ProcessMapPartDefinition()
			{
				// get the root element
				TiXmlElement* root = Document().FirstChildElement("osHTTPServer");
				if(!root)
					return false;

				// there should only be one map element in the definition
				TiXmlElement* map = root->FirstChildElement("map_download");
				if(!map)
					return false;

				const char* name = map->Attribute("name");
				const char* md5 = map->Attribute("md5");
				const char* uncompressed_size = map->Attribute("uncompressed_size");
				const char* compressed_size = map->Attribute("compressed_size");
				const char* algorithm = map->Attribute("algorithm");

				// must have both the name and md5 checksum
				if(name && md5 && algorithm && uncompressed_size && compressed_size)
				{
					// get the maps extension to determine whether it is yelo or not
					char extension[6] = "";
					if(!FileIO::GetFileExtension(extension, sizeof(extension), name))
						return false;

					if(strcmp(extension, Cache::K_MAP_FILE_EXTENSION_YELO) == 0)
						m_map_part_definition.map_element.m_map_is_yelo = true;
					else if(strcmp(extension, Cache::K_MAP_FILE_EXTENSION) == 0)
						m_map_part_definition.map_element.m_map_is_yelo = false;
					else
						return false;

					strcpy_s(m_map_part_definition.map_element.m_filename, name);
					strcpy_s(m_map_part_definition.map_element.m_md5, md5);

					if(1 != sscanf_s(uncompressed_size, "%u", &m_map_part_definition.map_element.m_uncompressed_size))
						uncompressed_size = 0;
					if(1 != sscanf_s(compressed_size, "%u", &m_map_part_definition.map_element.m_compressed_size))
						compressed_size = 0;

					// set which format the map is compressed with
					if(strcmp(algorithm, "zip") == 0)
						m_map_part_definition.map_element.m_format = _map_compression_format_zip;
					else if(strcmp(algorithm, "7zip") == 0)
						m_map_part_definition.map_element.m_format = _map_compression_format_7zip;
					else
						return false;

					if(!AddMapParts(map, m_map_part_definition.map_element))
						return false;
				}
				else
					return false;
				return true;
			}

			void* DownloadCompleteCallback(const bool download_succeeded, const char* buffer, const GHTTPByteCount buffer_length, void* component_data)
			{
				c_xml_downloader::DownloadCompleteCallback(download_succeeded, buffer, buffer_length, component_data);

				s_request_data* request_data = CAST_PTR(s_request_data*, component_data);
				delete request_data;

				// process the part definition
				if((Status() == Enums::_http_file_download_status_failed) || !ProcessMapPartDefinition())
					Status() = Enums::_http_file_download_status_failed;
				else
					Status() = Enums::_http_file_download_status_succeeded;

				// if the downloaded failed retry if applicable
				if((Status() == Enums::_http_file_download_status_failed) && Retry())
					Status() = Enums::_http_file_download_status_retry;

				return NULL;
			}

			void* DownloadCancelledCallback(void* component_data)
			{
				c_xml_downloader::DownloadCancelledCallback(component_data);

				s_request_data* request_data = CAST_PTR(s_request_data*, component_data);
				delete request_data;

				Status() = Enums::_http_file_download_status_cancelled;

				return NULL;
			}
		};

		class c_part_downloader : public c_file_downloader
		{
			struct
			{
				c_part_element*				m_part_element;
				uint32						m_total_bytes;
				uint32						m_received_bytes;
				byte*						m_decryption_key;
				byte						m_data[k_max_part_download_size];

				void Clear()
				{
					m_part_element = nullptr;
					m_total_bytes = 0;
					m_received_bytes = 0;
					ZeroMemory(m_data, sizeof(m_data));
					m_decryption_key = nullptr;
				}
			}m_part_downloader;

		protected:
			void GetArguments(s_request_args& request_args)
			{
				s_request_data* request_data = new s_request_data();
				request_data->m_request_type = _request_type_map_part;
				
				request_args.m_component = Enums::_http_client_component_map_download;
				request_args.m_component_data = request_data;

				request_args.m_delete_buffer = false;
				request_args.m_manual_think = true;
				request_args.m_request_timeout = k_connection_timeout;

				request_args.m_buffer = &m_part_downloader.m_data[0];
				request_args.m_buffer_size = k_max_part_download_size;
			}

		public:
			void Ctor()
			{
				c_file_downloader::Ctor();

				m_file_downloader.m_attempts.max_attempts = k_max_download_attempts;
				m_file_downloader.m_attempts.retry_delay = k_retry_delay_time;

				m_part_downloader.Clear();
			}

			void Dtor()
			{
				c_file_downloader::Dtor();

				m_file_downloader.m_attempts.max_attempts = k_max_download_attempts;
				m_file_downloader.m_attempts.retry_delay = k_retry_delay_time;

				m_part_downloader.Clear();
			}

			void SetPart(c_part_element* part)
			{
				m_part_downloader.m_part_element = part;
			}

			void SetDecryptionKey(byte* key)
			{
				m_part_downloader.m_decryption_key = key;
			}

			/*!
			 * \brief
			 * Checks the integrity of the part data that was downloaded.
			 * 
			 * \returns
			 * Returns true if the part data is valid, otherwise returns false.
			 * 
			 * Checks the integrity of the part data that was downloaded.
			 */
			bool	ValidatePart()
			{
				if(m_part_downloader.m_total_bytes != m_part_downloader.m_part_element->m_size)
					return false;

				//compare the datas md5 with that in the part definition
				return Engine::CompareMD5(m_part_downloader.m_data,
					m_part_downloader.m_total_bytes,
					m_part_downloader.m_part_element->m_md5);
			}

			/*!
			 * \brief
			 * Decrypts the downloaded part data and checks whether it is valid.
			 * 
			 * \returns
			 * Returns true if the part data was decrypted and validated successfully, otherwise returns false.
			 * 
			 * Decrypts the downloaded part data and checks whether it is valid.
			 */
			bool	DecryptPart()
			{
				if(!m_part_downloader.m_part_element->m_encrypted)
					return true;

				// decrypt the part using the supplied server password key
				if(!Cryptography::XXTEA::Decrypt(Enums::_cryptography_xxtea_block_size_256,
					m_part_downloader.m_data,
					m_part_downloader.m_total_bytes,
					m_part_downloader.m_decryption_key))
					return false;

				// verify the unencrypted data with the parts unencrypted md5
				if(!Engine::CompareMD5(m_part_downloader.m_data,
					m_part_downloader.m_total_bytes,
					m_part_downloader.m_part_element->m_unencrypted_md5))
					return false;

				return true;
			}

			/*!
			 * \brief
			 * Saves the current parts downloaded data to file.
			 * 
			 * \returns
			 * Returns true if the parts data was saved successfully.
			 * 
			 * Saves the current parts downloaded data to file.
			 */
			bool	SavePart(const char* save_location)
			{
				char save_file[MAX_PATH] = "";

				if(!FileIO::PathBuild(save_file, false, 2, save_location, m_part_downloader.m_part_element->m_name))
					return false;

				FileIO::s_file_info file_info;
				bool success = false;
				// do-while-false for simpler cleanup
				do
				{
					// create the part file for writing
					if(Enums::_file_io_open_error_none != FileIO::OpenFile(file_info,
						save_file,
						Enums::_file_io_open_access_type_write,
						Enums::_file_io_open_create_option_new))
						break;

					// write the part data to file
					if(Enums::_file_io_write_error_none != FileIO::WriteToFile(file_info,
						m_part_downloader.m_data,
						m_part_downloader.m_total_bytes))
						break;

					success = true;
				}while(false);

				// close the part file
				FileIO::CloseFile(file_info);

				return success;
			}

			/*!
			 * \brief
			 * Updates the file downloader, and sets the parts download progress.
			 * 
			 * \param delta
			 * Time in seconds since the last update.
			 * 
			 * Updates the file downloader, and sets the parts download progress.
			 */
			void	Update(real delta)
			{
				c_file_downloader::Update(delta);

				if(Status() == Enums::_http_file_download_status_in_progress)
				{
					if(QueryDownloadState(m_file_downloader.m_http.request_index) >= GHTTPReceivingFile)
					{
						m_part_downloader.m_total_bytes = QueryBytesTotal(m_file_downloader.m_http.request_index);
						m_part_downloader.m_received_bytes = QueryBytesReceived(m_file_downloader.m_http.request_index);
					}
				}
				else if(Status() != Enums::_http_file_download_status_succeeded)
				{
					m_part_downloader.m_total_bytes = 0;
					m_part_downloader.m_received_bytes = 0;
				}

				m_part_downloader.m_part_element->m_total_bytes = m_part_downloader.m_total_bytes;
				m_part_downloader.m_part_element->m_received_bytes = m_part_downloader.m_received_bytes;
			}

			void*	DownloadCompleteCallback(const bool download_succeeded, const char* buffer, const GHTTPByteCount buffer_length, void* component_data)
			{
				c_file_downloader::DownloadCompleteCallback(download_succeeded, buffer, buffer_length, component_data);

				s_request_data* request_data = CAST_PTR(s_request_data*, component_data);
				delete request_data;

				m_part_downloader.m_total_bytes = 0;
				m_part_downloader.m_received_bytes = 0;

				// if the returned data is larger than the maximum permitted, fail
				if(!download_succeeded || !buffer || (buffer_length > k_max_part_size) || (buffer_length == 0))
				{
					Status() = Enums::_http_file_download_status_failed;
					return NULL;
				}

				if(buffer_length != m_part_downloader.m_part_element->m_size)
				{
					Status() = Enums::_http_file_download_status_failed;
					return nullptr;
				}

				// download size can be less than 1MB
				m_part_downloader.m_total_bytes = (uint32)buffer_length;
				m_part_downloader.m_received_bytes = (uint32)buffer_length;

				// validate and decrypt the part
				if(!ValidatePart() || !DecryptPart())
					Status() = Enums::_http_file_download_status_failed;
				else
					Status() = Enums::_http_file_download_status_succeeded;

				// if validation or decryption failed retry the download if applicable
				if((Status() == Enums::_http_file_download_status_failed) && Retry())
					Status() = Enums::_http_file_download_status_retry;

				return nullptr;
			}

			void*	DownloadCancelledCallback(void* component_data)
			{
				c_file_downloader::DownloadCancelledCallback(component_data);

				auto request_data = CAST_PTR(s_request_data*, component_data);
				delete request_data;

				m_part_downloader.m_total_bytes = 0;
				m_part_downloader.m_received_bytes = 0;

				Status() = Enums::_http_file_download_status_cancelled;

				return nullptr;
			}
		};

		struct s_map_download_globals
		{
			map_download_update_stage	m_map_download_update_stage;
			PAD16;

			struct
			{
				char						user_download_directory[MAX_PATH];
				char						parts_save_directory[MAX_PATH];
				char						archive_save_location[MAX_PATH];
			}m_paths;

			struct
			{
				c_dedicated_server			dedicated_server;

				c_http_server_instance*		server_list_head;
				LinkedListIterator<c_http_server_instance>* server_iterator;
			}m_servers;

			struct
			{
				HANDLE						thread_handle;
				map_download_stage			stage;

				volatile bool				stop_download;
				bool						in_progress;
				PAD16;
			}m_download_thread;

			struct
			{
				master_server_list_state	state;
				bool						completed;
				bool						succeeded;

				c_http_server*			server_list;
				c_master_server_list		downloader;
			}m_master_server_list;

			struct
			{
				map_part_definition_state	state;
				PAD16;

				c_map_part_definition		downloader;
				real						progress;
			}m_map_part_definition;

			struct
			{
				part_download_state			state;
				PAD16;

				LinkedListIterator<c_part_element>* part_iterator;
				c_part_downloader			downloader;
			}m_part_download;

			struct
			{
				archive_extraction_state	state;
				bool						using_stock_maps_path;
				PAD8;
			}m_archive_extraction;

			struct
			{
				download_completion_state	state;
				PAD16;

				real						reconnect_delay;
			}m_download_completion;
		};
		static s_map_download_globals	g_map_download_globals;
		static HANDLE					g_globals_access_mutex;

#pragma region Settings
		c_settings_container::c_settings_container()
			: Configuration::c_configuration_container("Networking.MapDownload")
			, m_enabled("Enabled", true)
		{ }

		const std::vector<Configuration::i_configuration_value* const> c_settings_container::GetMembers()
		{
			return std::vector<i_configuration_value* const> { &m_enabled };
		}
#pragma endregion

		/*!
		 * \brief
		 * Updates the download display with the current map download progress.
		 * 
		 * Updates the download display with the current map download progress.
		 */
		void	UpdateDownloadDisplay()
		{
			WaitForSingleObject(g_globals_access_mutex, INFINITE);

			g_map_download_display.SetMapName(g_map_download_globals.m_map_part_definition.downloader.MapElement().m_name);

			// build the details string
			switch(g_map_download_globals.m_map_download_update_stage)
			{
			case _map_download_update_stage_downloading_map:
				{
					switch(g_map_download_globals.m_download_thread.stage)
					{
					case _map_download_stage_map_part_definition:
						g_map_download_globals.m_map_part_definition.progress = 0;
						g_map_download_display.SetProviderTitle("Searching");
						g_map_download_display.SetProviderDescription("Looking for a map download provider...");
						g_map_download_display.SetMapDownloadState(_map_download_state_searching);
						break;
					case _map_download_stage_map_part_download:
						g_map_download_display.SetMapDownloadState(_map_download_state_downloading);
						break;
					case _map_download_stage_map_archive_extraction:
						g_map_download_display.SetMapDownloadState(_map_download_state_extracting);
						break;
					default:
						g_map_download_display.SetMapDownloadState(_map_download_state_completed);
						break;
					};
				}
				break;
			case _map_download_update_stage_map_download_completion:
				g_map_download_display.SetMapDownloadState(_map_download_state_reconnecting);
				break;
			case _map_download_update_stage_failed:
				g_map_download_display.SetMapDownloadState(_map_download_state_failed);
				g_map_download_display.SetProviderTitle("");
				g_map_download_display.SetProviderDescription("");
				break;
			}
			
			// display the countdown to server reconnection
			if(_map_download_update_stage_map_download_completion == g_map_download_globals.m_map_download_update_stage)
			{
				g_map_download_display.SetReconnectTime(g_map_download_globals.m_download_completion.reconnect_delay);
			}

			// set the part download details
			if(g_map_download_globals.m_download_thread.stage >= _map_download_stage_map_part_download)
			{
				LinkedListIterator<c_part_element> part_iterator(g_map_download_globals.m_map_part_definition.downloader.MapElement().m_parts);

				// get the cumulative byte count of all the parts downloaded
				uint32 received_bytes_total = 0;
				while(part_iterator.MoveNext())
					received_bytes_total += part_iterator.Current()->m_received_bytes;

				// calculate the progress of the download
				real progress;
				if(received_bytes_total == 0)
					progress = 0;
				else
					progress = (received_bytes_total / (real)g_map_download_globals.m_map_part_definition.downloader.MapElement().m_compressed_size);

				// clamp the progress between 0 and 1
				progress = __max(0, progress);
				progress = __min(1, progress);

				g_map_download_globals.m_map_part_definition.progress = progress;
			}

			if((_map_download_stage_map_part_definition == g_map_download_globals.m_download_thread.stage) ||
				(_map_download_stage_map_part_download == g_map_download_globals.m_download_thread.stage))
			{
				int32 http_request = -1;
				real progress = 0;

				// set the download status
				switch(g_map_download_globals.m_download_thread.stage)
				{
				case _map_download_stage_map_part_definition:
					http_request = g_map_download_globals.m_map_part_definition.downloader.HTTPRequestIndex();
					break;
				case _map_download_stage_map_part_download:
					http_request = g_map_download_globals.m_part_download.downloader.HTTPRequestIndex();

					// get the download progress of the part
					uint32 received = 0;
					uint32 total = 0;
					if(g_map_download_globals.m_part_download.part_iterator && g_map_download_globals.m_part_download.part_iterator->Current())
					{
						received = g_map_download_globals.m_part_download.part_iterator->Current()->m_received_bytes;
						total = g_map_download_globals.m_part_download.part_iterator->Current()->m_total_bytes;
					}

					if((received == 0) || (total == 0))
						progress = 0;
					else
						progress = (real)received / (real)total;

					// clamp the progress between 0 and 1
					progress = __max(0, progress);
					progress = __min(1, progress);

					g_map_download_display.SetPartProgress(progress);

					if(g_map_download_globals.m_servers.server_iterator && g_map_download_globals.m_servers.server_iterator->Current())
					{
						// add the provider title
						g_map_download_display.SetProviderTitle(g_map_download_globals.m_servers.server_iterator->Current()->Server()->m_name);
						// add the provider description
						g_map_download_display.SetProviderDescription(g_map_download_globals.m_servers.server_iterator->Current()->Server()->m_description);
					}
					break;
				};
			}
			g_map_download_display.SetMapProgress(g_map_download_globals.m_map_part_definition.progress);
			ReleaseMutex(g_globals_access_mutex);
		}

		/*!
		 * \brief
		 * Prepares the part download variables for downloading a map part.
		 * 
		 * \returns
		 * Returns false if a download is ready to start, otherwise returns true if there are no more parts to download.
		 * 
		 * Prepares the part download variables for downloading a map part.
		 */
		bool	SetupPartDownload()
		{
			LinkedListIterator<c_part_element>*& part_iterator = g_map_download_globals.m_part_download.part_iterator;

			if(!part_iterator)
				part_iterator = new LinkedListIterator<c_part_element>(g_map_download_globals.m_map_part_definition.downloader.MapElement().m_parts);

			// if MoveNext is true, we are not at the end of the list yet
			if(part_iterator->MoveNext())
				return false;
			else
			{
				// all parts downloaded, delete the iterator
				delete part_iterator;
				part_iterator = nullptr;

				return true;
			}
		}

		/*!
		 * \brief
		 * Combines the downloaded parts into a single archive, ready for extraction.
		 * 
		 * \returns
		 * Returns true if the archive was combined successfully, otherwise returns false.
		 * 
		 * Combines the downloaded parts into a single archive, ready for extraction.
		 */
		bool	CombineMapArchive()
		{
			// create the map archive path
			g_map_download_globals.m_paths.archive_save_location[0] = 0;

			const char* extension = ".comp";
			switch(g_map_download_globals.m_map_part_definition.downloader.MapElement().m_format)
			{
			case _map_compression_format_zip:
				extension = ".zip";
				break;
			case _map_compression_format_7zip:
				extension = ".7z";
				break;
			};

			bool path_combined = FileIO::PathBuild(g_map_download_globals.m_paths.archive_save_location,
				false, 2,
				g_map_download_globals.m_paths.parts_save_directory,
				g_map_download_globals.m_map_part_definition.downloader.MapElement().m_filename);

			if(!path_combined || (0 != strcat_s(g_map_download_globals.m_paths.archive_save_location, MAX_PATH, extension)))
				return false;

			FileIO::s_file_info archive_file;

			// create the archive file for writing
			if(Enums::_file_io_open_error_none != FileIO::OpenFile(archive_file,
				g_map_download_globals.m_paths.archive_save_location,
				Enums::_file_io_open_access_type_write,
				Enums::_file_io_open_create_option_new))
				return false;

			// start from the first part element
			LinkedListIterator<c_part_element> part_iterator(g_map_download_globals.m_map_part_definition.downloader.MapElement().m_parts);

			if(!part_iterator.MoveNext())
				return false;

			bool archive_success = true;
			do
			{
				FileIO::s_file_info part_file;

				bool part_success = false;
				// do-while-false for simpler cleanup
				do
				{
					// get the parts file path
					char part_location[MAX_PATH] = "";
					if(!FileIO::PathBuild(part_location, false, 2, g_map_download_globals.m_paths.parts_save_directory, part_iterator.Current()->m_name))
						break;

					// open the archive part file
					if(Enums::_file_io_open_error_none != FileIO::OpenFile(part_file,
						part_location,
						Enums::_file_io_open_access_type_read,
						Enums::_file_io_open_create_option_open_existing))
						break;

					// memory map the archive part
					if(Enums::_file_io_read_error_none != FileIO::MemoryMapFile(part_file))
						break;

					// write the part data to the archive file
					if(Enums::_file_io_write_error_none != FileIO::WriteToFile(archive_file, CAST_PTR(char*, part_file.data_pointer), part_file.data_length))
						break;

					part_success = true;
				}while(false);

				FileIO::CloseFile(part_file);

				archive_success &= part_success;

			}while(archive_success && part_iterator.MoveNext());

			FileIO::CloseFile(archive_file);

			return archive_success;
		}

		/*!
		 * \brief
		 * Extracts the map from the downloaded map archive, saving it to the maps directory.
		 * 
		 * \returns
		 * Returns true if the map was extracted and validated successfully, otherwise returns false.
		 * 
		 * Extracts the map from the downloaded map archive, saving it to the maps directory.
		 */
		bool	ExtractMapArchive()
		{
			c_map_element& map_element = g_map_download_globals.m_map_part_definition.downloader.MapElement();

			byte* uncompressed_data = nullptr;
			size_t uncompressed_size = 0;
			bool success = false;

			switch(map_element.m_format)
			{
			case _map_compression_format_zip:
				{
					unzFile zip_file = nullptr;
					if(!Compression::Zip::OpenZipFile(g_map_download_globals.m_paths.archive_save_location, &zip_file))
						break;

					// decompress the map in to memory
					success = Compression::Zip::DecompressFileEntry(zip_file,
						map_element.m_filename,
						uncompressed_data,
						uncompressed_size);

					Compression::Zip::CloseZipFile(zip_file);
				}
				break;
			case _map_compression_format_7zip:
				{
					// decompress the map in to memory
					success = Compression::SevenZip::DecompressFileEntry(g_map_download_globals.m_paths.archive_save_location,
						map_element.m_filename,
						uncompressed_data,
						uncompressed_size);
				}
				break;

			YELO_ASSERT_CASE_UNREACHABLE();
			};

			// if the decompression failed, delete the data
			if(!success)
				return false;

			// validate the data
			success = Engine::CompareMD5(uncompressed_data, uncompressed_size, map_element.m_md5);
			success &= (uncompressed_size == map_element.m_uncompressed_size);

			if(success)
			{
				FileIO::s_file_info map_file;
				// save the map to the maps directory
				success = false;
				// do-while-false for simpler cleanup
				do
				{
					// build the output file path
					char map_file_path[MAX_PATH] = "";

					// If the user's maps directory does not exist, try to create and use it, otherwise default to the stock maps directory
					cstring maps_directory = Settings::UserProfileMapsPath();
					g_map_download_globals.m_archive_extraction.using_stock_maps_path = false;

					if(!Settings::UserProfileMapsPathExists())
					{
						if(!FileIO::BuildDirectoryTree(Settings::UserProfileMapsPath()))
						{
							maps_directory = Settings::PlatformUserMapsPath();
							g_map_download_globals.m_archive_extraction.using_stock_maps_path = true;
						}
					}

					if(!FileIO::PathBuild(map_file_path,
						false, 2, maps_directory,
						map_element.m_filename))
						break;
				
					// create the map file
					if(Enums::_file_io_open_error_none != FileIO::OpenFile(map_file,
						map_file_path,
						Enums::_file_io_open_access_type_write,
						Enums::_file_io_open_create_options_create_if_missing))
						break;

					// write the uncompressed data to the map file
					if(Enums::_file_io_write_error_none != FileIO::WriteToFile(map_file,
						uncompressed_data,
						(DWORD)uncompressed_size))
						break;

					success = true;
				}while(false);

				FileIO::CloseFile(map_file);
			}

			delete [] uncompressed_data;
			uncompressed_data = nullptr;

			return success;
		}

		/*!
		 * \brief
		 * Adds a downloaded map to Halo's map headers so that it can be loaded by the game.
		 * 
		 * \returns
		 * Returns true if the map was added to Halo's headers successfully.
		 * 
		 * Adds a downloaded map to Halo's map headers so that it can be loaded by the game.
		 */
		bool	AddMapToMapHeaders()
		{
			c_map_element& map_element = g_map_download_globals.m_map_part_definition.downloader.MapElement();

			int map_list_index = NONE;

			cstring maps_directory = Settings::UserProfileMapsPath();
			if(g_map_download_globals.m_archive_extraction.using_stock_maps_path)
			{
				maps_directory = Settings::PlatformUserMapsPath();
			}

			Interface::MapListAddMapFromPath(maps_directory, map_element.m_filename);

			return map_list_index != NONE;
		}

		/*!
		 * \brief
		 * Reconnects to the server, using its IP and the provided password (if applicable).
		 * 
		 * Reconnects to the server, using its IP and the provided password (if applicable).
		 */
		void	ReconnectToServer()
		{
			BuildNumber::ChangeAdvertisedVersionId(g_map_download_globals.m_servers.dedicated_server.m_mp_gamever);
			blam::main_connect(g_map_download_globals.m_servers.dedicated_server.m_mp_address,
				g_map_download_globals.m_servers.dedicated_server.m_mp_password);
		}

		/*!
		 * \brief
		 * Creates the directory tree to save the downloaded parts to.
		 * 
		 * \returns
		 * Returns true if the directories were created successfully, otherwise returns false.
		 * 
		 * Creates the directory tree to save the downloaded parts to.
		 * 
		 * \remarks
		 * The part data is downloaded to the users documents, in their My Games\Halo CE\OpenSauce directory.
		 */
		bool	BuildOutputDirectories()
		{
			if(!FileIO::PathBuild(g_map_download_globals.m_paths.parts_save_directory,
				true, 2,
				g_map_download_globals.m_paths.user_download_directory,
				g_map_download_globals.m_map_part_definition.downloader.MapElement().m_name))
				return false;

			if(!FileIO::PathExists(g_map_download_globals.m_paths.user_download_directory))
				if(!CreateDirectory(g_map_download_globals.m_paths.user_download_directory, nullptr))
					return false;

			if(!FileIO::PathExists(g_map_download_globals.m_paths.parts_save_directory))
				if(!CreateDirectory(g_map_download_globals.m_paths.parts_save_directory, nullptr))
					return false;

			return true;
		}

		/*!
		 * \brief
		 * Builds a list of server references which will be downloaded from in order.
		 * 
		 * \returns
		 * True if the list was created successfully, otherwise returns false.
		 * 
		 * Builds a list of server references which will be downloaded from in order.
		 */
		bool	BuildServerInstanceList()
		{
			// add the dedicated server as the first to download from
			auto instance = new c_http_server_instance(&g_map_download_globals.m_servers.dedicated_server);

			AppendLinkedListNode(g_map_download_globals.m_servers.server_list_head, instance);

			// TODO: move this to a global shuffle namespace
			//shuffle loop
			int list_length = GetListLength(g_map_download_globals.m_master_server_list.server_list);
			YELO_ASSERT_DISPLAY(list_length <= 256, "max server list count reached"); // TODO: named constant

			if(list_length > 0)
			{
				char* list_indices = new char[list_length];

				// fill the index list
				for(char i = 0; i < list_length; i++)
					list_indices[i] = i;

				// re-seed the random number generator, always get the same sequence otherwise, even if seeded on load
				srand((uint32)time(0));

				// shuffle the indices to a pseudo random pattern
				// this uses the Durstenfeld shuffle algorithm
				char end_index = list_length - 1;
				for(; end_index > 0; end_index--)
				{
					int index = (int)(end_index * ((real)rand() / (real)RAND_MAX));

					char swap_temp = list_indices[end_index];
					list_indices[end_index] = list_indices[index];
					list_indices[index] = swap_temp;
				}

				// add the master servers to the list as per the randomly shuffled indices
				// this prevents any 1 master server getting significantly more traffic than another server providing the same map
				for(char i = 0; i < list_length; i++)
				{
					instance = new c_http_server_instance(GetNodeByIndex(g_map_download_globals.m_master_server_list.server_list, list_indices[i]));

					AppendLinkedListNode(g_map_download_globals.m_servers.server_list_head, instance);
				}

				delete [] list_indices;
			}

			// create the server list iterator
			g_map_download_globals.m_servers.server_iterator = new LinkedListIterator<c_http_server_instance>(g_map_download_globals.m_servers.server_list_head);
			// set to the first server
			g_map_download_globals.m_servers.server_iterator->MoveNext();

			return true;
		}

		/*!
		 * \brief
		 * Deletes resources allocated/created during the download process.
		 * 
		 * Deletes resources allocated/created during the download process.
		 */
		void	CleanupMapDownload()
		{
			// delete the server list resources
			g_map_download_globals.m_servers.dedicated_server.Dtor();

			delete g_map_download_globals.m_servers.server_iterator;
			g_map_download_globals.m_servers.server_iterator = nullptr;

			if(g_map_download_globals.m_servers.server_list_head)
				DeleteLinkedList(g_map_download_globals.m_servers.server_list_head);

			// delete the map part definition resources
			g_map_download_globals.m_map_part_definition.downloader.Stop();
			g_map_download_globals.m_map_part_definition.downloader.Dtor();

			// delete the part download resources
			delete g_map_download_globals.m_part_download.part_iterator;
			g_map_download_globals.m_part_download.part_iterator = nullptr;

			g_map_download_globals.m_part_download.downloader.Stop();
			g_map_download_globals.m_part_download.downloader.Dtor();

			// delete the download directory and all its contents
			// no need to check the return value as if the deletion failed there isn't anything we can do about it
			FileIO::DirectoryDelete(g_map_download_globals.m_paths.user_download_directory, true, true);
		}

		/*!
		 * \brief
		 * Deletes resources allocated/created during the master server list download.
		 * 
		 * Deletes resources allocated/created during the master server list download.
		 */
		void	CleanupMasterServerDownload()
		{
			// delete the master server list resources
			g_map_download_globals.m_master_server_list.downloader.Stop();
			g_map_download_globals.m_master_server_list.downloader.Dtor();

			if(g_map_download_globals.m_master_server_list.server_list)
				DeleteLinkedList(g_map_download_globals.m_master_server_list.server_list);
			g_map_download_globals.m_master_server_list.server_list = nullptr;
		}

		/*!
		 * \brief
		 * Resets all of the systems values to their initial values.
		 * 
		 * Resets all of the systems values to their initial values.
		 */
		void	ResetMapDownload()
		{
			// reset globals for new map download
			g_map_download_globals.m_servers.dedicated_server.Ctor();
			g_map_download_globals.m_servers.server_iterator = nullptr;
			g_map_download_globals.m_servers.server_list_head = nullptr;

			g_map_download_globals.m_download_thread.stop_download = false;
			g_map_download_globals.m_download_thread.in_progress = false;
			g_map_download_globals.m_download_thread.thread_handle = INVALID_HANDLE_VALUE;
			g_map_download_globals.m_download_thread.stage = _map_download_stage_build_server_list;

			// reset map part definition stage
			g_map_download_globals.m_map_part_definition.state = _map_part_definition_state_begin;
			g_map_download_globals.m_map_part_definition.downloader.Ctor();

			// reset part download stage
			g_map_download_globals.m_part_download.state = _part_download_state_begin;
			g_map_download_globals.m_part_download.downloader.Ctor();
			g_map_download_globals.m_paths.parts_save_directory[0] = 0;

			// reset archive extraction stage
			g_map_download_globals.m_archive_extraction.state = _archive_extraction_state_begin;
			g_map_download_globals.m_paths.archive_save_location[0] = 0;

			// reset download completion stage
			g_map_download_globals.m_download_completion.state = _download_completion_state_begin;
			g_map_download_globals.m_download_completion.reconnect_delay = k_reconnect_delay_time;
		}

		/*!
		 * \brief
		 * Resets all of the master server download values to their initial values.
		 * 
		 * Resets all of the master server download values to their initial values.
		 */
		void	ResetMasterServerDownload()
		{
			// reset master server list stage
			g_map_download_globals.m_master_server_list.state = _master_server_list_state_begin;
			g_map_download_globals.m_master_server_list.completed = false;
			g_map_download_globals.m_master_server_list.succeeded = false;
			g_map_download_globals.m_master_server_list.server_list = nullptr;
			g_map_download_globals.m_master_server_list.downloader.Ctor();
			g_map_download_globals.m_master_server_list.downloader.SetServerList(&g_map_download_globals.m_master_server_list.server_list);
		}

		/*!
		 * \brief
		 * Progresses through the steps for downloading the master server list xml.
		 * 
		 * \param delta
		 * The time in seconds since the last update.
		 * 
		 * \returns
		 * Returns true if the download has ended, otherwise returns false.
		 * 
		 * Progresses through the steps for downloading the master server list xml.
		 * 
		 * \remarks
		 * The master server list downloaded in the following steps:
		 * 1. Initiate the server list download.
		 * 2. Wait for the server list download to complete successfully.
		 * 3. If the download failed, continue without the master servers.
		 */
		bool UpdateStage_MasterServerList(real delta)
		{
			c_master_server_list& server_list_downloader = g_map_download_globals.m_master_server_list.downloader;

			switch(g_map_download_globals.m_master_server_list.state)
			{
			case _master_server_list_state_begin:
				{
					// build the master server url
					// use the url_interface class to keep the output consistent
					c_url_interface url_interface;
					url_interface.ParseURL(g_master_server_list_url);

					// download the master server list
					server_list_downloader.SetURL(url_interface.GetURL().c_str());
					server_list_downloader.Start();

					// if the download failed end the download
					if(server_list_downloader.Status() == Enums::_http_file_download_status_failed)
						return true;

					g_map_download_globals.m_master_server_list.state = _master_server_list_state_downloading;
				}
			case _master_server_list_state_downloading:
				{
					server_list_downloader.Update(delta);

					switch(server_list_downloader.Status())
					{
					case Enums::_http_file_download_status_succeeded:
						g_map_download_globals.m_master_server_list.state = _master_server_list_state_download_succeeded;
						break;
					case Enums::_http_file_download_status_cancelled:
					case Enums::_http_file_download_status_failed:
						g_map_download_globals.m_master_server_list.state = _master_server_list_state_download_failed;
						break;
					case Enums::_http_file_download_status_retry:
					case Enums::_http_file_download_status_in_progress:
						break;

					YELO_ASSERT_CASE_UNREACHABLE();
					};
				}
				break;
			case _master_server_list_state_download_failed:
				{
					// clean up the download request
					server_list_downloader.Stop();

					g_map_download_globals.m_master_server_list.succeeded = false;

					return true;
				}
			case _master_server_list_state_download_succeeded:
				{
					// clean up the download request
					server_list_downloader.Stop();

					g_map_download_globals.m_master_server_list.state = _master_server_list_state_stage_complete;
					g_map_download_globals.m_master_server_list.succeeded = true;
				}
				break;

			YELO_ASSERT_CASE_UNREACHABLE();
			}

			// if this stage is complete, move on to the next stage
			if(_master_server_list_state_stage_complete == g_map_download_globals.m_master_server_list.state)
				return true;
			return false;
		}

		/*!
		 * \brief
		 * Progresses through the steps for downloading the requested maps part definition xml.
		 * 
		 * \param delta
		 * The time in seconds since the last update.
		 * 
		 * \returns
		 * Returns the next map download stage to progress to.
		 * 
		 * Progresses through the steps for downloading the requested maps part definition xml.
		 * 
		 * \remarks
		 * The part definition is downloaded in the following steps:
		 * 1. Initiate the part definition download.
		 * 2. Wait for the part definition download to complete successfully.
		 * 3. Move on to the part download stage.
		 */
		map_download_stage UpdateStage_MapPartDefinition(real delta)
		{
			c_map_part_definition& definition_downloader = g_map_download_globals.m_map_part_definition.downloader;

			switch(g_map_download_globals.m_map_part_definition.state)
			{
			case _map_part_definition_state_begin:
				{
					YELO_ASSERT_DISPLAY(g_map_download_globals.m_servers.server_iterator->Current(), "server list iterators current instance is null");

					c_http_server* current_server = g_map_download_globals.m_servers.server_iterator->Current()->Server();

					// build the part definition url
					c_url_interface url_interface;
					url_interface.ParseURL(current_server->m_address);
					url_interface.m_scheme.assign("http");
					url_interface.AddQuery("map", definition_downloader.MapElement().m_name);

					definition_downloader.SetURL(url_interface.GetURL().c_str());
					definition_downloader.Start();

					// download the map part definition
					if(definition_downloader.Status() == Enums::_http_file_download_status_failed)
						return _map_download_stage_failed;

					g_map_download_globals.m_map_part_definition.state = _map_part_definition_state_downloading;
				}
			case _map_part_definition_state_downloading:
				{
					definition_downloader.Update(delta);

					switch(definition_downloader.Status())
					{
					case Enums::_http_file_download_status_succeeded:
						g_map_download_globals.m_map_part_definition.state = _map_part_definition_state_download_succeeded;
						break;
					case Enums::_http_file_download_status_cancelled:
					case Enums::_http_file_download_status_failed:
						g_map_download_globals.m_map_part_definition.state = _map_part_definition_state_download_failed;
						break;
					case Enums::_http_file_download_status_retry:
					case Enums::_http_file_download_status_in_progress:
						break;

					YELO_ASSERT_CASE_UNREACHABLE();
					};
				}
				break;
			case _map_part_definition_state_download_failed:
				{
					// clean up the request
					definition_downloader.Stop();

					return _map_download_stage_failed;
				}
				break;
			case _map_part_definition_state_download_succeeded:
				{
					// clean up the definition http request
					definition_downloader.Stop();

					g_map_download_globals.m_map_part_definition.state = _map_part_definition_state_stage_complete;
				}
				break;

			YELO_ASSERT_CASE_UNREACHABLE();
			}

			// if this stage is complete, move on to the next stage
			if(_map_part_definition_state_stage_complete == g_map_download_globals.m_map_part_definition.state)
				return _map_download_stage_map_part_download;
			return _map_download_stage_map_part_definition;
		}

		/*!
		 * \brief
		 * Progresses through the steps for downloading the maps parts.
		 * 
		 * \param delta
		 * The time in seconds since the last update.
		 * 
		 * \returns
		 * Returns the next map download stage to progress to.
		 * 
		 * Progresses through the steps for downloading the maps parts.
		 * 
		 * \remarks
		 * The map parts are downloaded in the following steps:
		 * 1. Create the directories to download to.
		 * 2. Set up the globals, ready for downloading the map part.
		 * 3. Initiate the part download.
		 * 4. Wait for the download to complete successfully.
		 * 5. Save the part to file.
		 * 6. Move on to the next part. If all parts are downloaded continue.
		 * 7. Move on to the map extraction stage.
		 */
		map_download_stage UpdateStage_PartDownload(real delta)
		{
			c_part_downloader& part_downloader = g_map_download_globals.m_part_download.downloader;

			switch(g_map_download_globals.m_part_download.state)
			{
			case _part_download_state_begin:
				{
					// create the directory the map parts will be saved to
					if(!BuildOutputDirectories())
						return _map_download_stage_failed;

					g_map_download_globals.m_part_download.state = _part_download_state_prepare_download;
				}
			case _part_download_state_prepare_download:
				{
					// set up for the next download, returns true if all parts have been downloaded
					if(SetupPartDownload())
					{
						g_map_download_globals.m_part_download.state = _part_download_state_stage_complete;
						break;
					}
					
					g_map_download_globals.m_part_download.state = _part_download_state_ready_to_download;
				}
			case _part_download_state_ready_to_download:
				{
					YELO_ASSERT_DISPLAY(g_map_download_globals.m_servers.server_iterator->Current(), "server list iterators current instance is null");

					c_http_server* current_server = g_map_download_globals.m_servers.server_iterator->Current()->Server();

					// build the part request url
					c_url_interface url_interface;
					url_interface.ParseURL(current_server->m_address);
					url_interface.m_scheme.assign("http");
					url_interface.AddQuery("map", g_map_download_globals.m_map_part_definition.downloader.MapElement().m_name);
					url_interface.AddQuery("part", g_map_download_globals.m_part_download.part_iterator->Current()->m_name);

					// download the current part
					part_downloader.Ctor();
					part_downloader.SetURL(url_interface.GetURL().c_str());
					part_downloader.SetPart(g_map_download_globals.m_part_download.part_iterator->Current());
					part_downloader.Start();

					if(g_map_download_globals.m_part_download.part_iterator->Current()->m_encrypted)
					{
						if(*g_map_download_globals.m_servers.dedicated_server.m_mp_password == '\0')
							return _map_download_stage_failed;
						g_map_download_globals.m_part_download.downloader.SetDecryptionKey(&g_map_download_globals.m_servers.dedicated_server.m_mp_password_key[0]);
					}

					if(part_downloader.Status() == Enums::_http_file_download_status_failed)
						return _map_download_stage_failed;

					g_map_download_globals.m_part_download.state = _part_download_state_downloading;
				}
			case _part_download_state_downloading:
				{
					part_downloader.Update(delta);

					switch(part_downloader.Status())
					{
					case Enums::_http_file_download_status_succeeded:
						g_map_download_globals.m_part_download.state = _part_download_state_download_complete;
						break;
					case Enums::_http_file_download_status_cancelled:
					case Enums::_http_file_download_status_failed:
						g_map_download_globals.m_part_download.state = _part_download_state_download_failed;
						break;
					case Enums::_http_file_download_status_retry:
					case Enums::_http_file_download_status_in_progress:
						break;
					
					YELO_ASSERT_CASE_UNREACHABLE();
					};
				}
				break;
			case _part_download_state_download_failed:
				{
					// clean up the request
					part_downloader.Stop();

					if(g_map_download_globals.m_part_download.part_iterator && g_map_download_globals.m_part_download.part_iterator->Current())
					{
						g_map_download_globals.m_part_download.part_iterator->Current()->m_total_bytes = 0;
						g_map_download_globals.m_part_download.part_iterator->Current()->m_received_bytes = 0;
					}

					return _map_download_stage_failed;
				}
			case _part_download_state_download_complete:
				{
					// clean up the part request
					part_downloader.Stop();

					g_map_download_globals.m_part_download.state = _part_download_state_part_ready_to_save;
				}
			case _part_download_state_part_ready_to_save:
				{
					char save_directory[MAX_PATH] = "";

					if(!FileIO::PathBuild(save_directory, false, 2, g_map_download_globals.m_paths.user_download_directory, 
						g_map_download_globals.m_map_part_definition.downloader.MapElement().m_name))
						return _map_download_stage_failed;

					// the part is valid and unencrypted if necessary, so save it to file
					if(part_downloader.SavePart(save_directory))
						g_map_download_globals.m_part_download.state = _part_download_state_prepare_download;
					else
						return _map_download_stage_failed;
				}
				break;
			
			YELO_ASSERT_CASE_UNREACHABLE();
			}

			// if this stage is complete, move on to the next stage
			if(_part_download_state_stage_complete == g_map_download_globals.m_part_download.state)
				return _map_download_stage_map_archive_extraction;
			return _map_download_stage_map_part_download;
		}

		/*!
		 * \brief
		 * Progresses through the steps for extracting the map archive.
		 * 
		 * \returns
		 * Returns the next map download stage to progress to.
		 * 
		 * Progresses through the steps for extracting the map archive.
		 * 
		 * \remarks
		 * The steps for extracting the map archive are as follows:
		 * 1. Combine the map parts into a single file.
		 * 2. Decompress the archive, saving the map to the maps folder.
		 */
		map_download_stage UpdateStage_ArchiveExtraction()
		{
			// combine all parts into a single archive
			if(!CombineMapArchive())
				return _map_download_stage_failed;

			// extract the map to the maps folder
			if(!ExtractMapArchive())
				return _map_download_stage_failed;
			return _map_download_stage_complete;
		}

		/*!
		 * \brief
		 * Main function for the map download thread.
		 * 
		 * \returns
		 * Returns 0 if the download failed, 1 if the download was cancelled and 2 if the download succeeded.
		 * 
		 * Main function for the map download thread.
		 */
		DWORD	DownloadMap(void*)
		{
			real delta = 0;

			// flag used by the main thread to see if the thread is running
			g_map_download_globals.m_download_thread.in_progress = true;

			DWORD return_value = NONE;
			do
			{
				// if the main thread sets the stop flag, cancel the download
				if(g_map_download_globals.m_download_thread.stop_download)
					g_map_download_globals.m_download_thread.stage = _map_download_stage_cancelled;

				// wait for access to the globals (is only locked when the display code is updating)
				WaitForSingleObject(g_globals_access_mutex, INFINITE);

				// update the current download stage
				switch(g_map_download_globals.m_download_thread.stage)
				{
				case _map_download_stage_build_server_list:
					{
						if(!BuildServerInstanceList())
						{
							g_map_download_globals.m_download_thread.stage = _map_download_stage_failed;
							break;
						}
					}
				case _map_download_stage_map_part_definition:
					{
						map_download_stage stage_result = UpdateStage_MapPartDefinition(delta);

						// if the map part definition download from the dedi failed, attempt to download from the master servers
						if((stage_result == _map_download_stage_failed) && g_map_download_globals.m_servers.server_iterator->MoveNext())
						{
							g_map_download_globals.m_download_thread.stage = _map_download_stage_map_part_definition;
							g_map_download_globals.m_map_part_definition.state = _map_part_definition_state_begin;
						}
						else
							g_map_download_globals.m_download_thread.stage = stage_result;
					}
					break;
				case _map_download_stage_map_part_download:
					g_map_download_globals.m_download_thread.stage = UpdateStage_PartDownload(delta);
					break;
				case _map_download_stage_map_archive_extraction:
					// release the globals mutex since extraction can take a little while, which would freeze the main thread
					ReleaseMutex(g_globals_access_mutex);

					g_map_download_globals.m_download_thread.stage = UpdateStage_ArchiveExtraction();

					// re-engage the globals mutex
					WaitForSingleObject(g_globals_access_mutex, INFINITE);
					break;
				case _map_download_stage_failed:
					// download failed, break out
					g_map_download_globals.m_download_thread.in_progress = false;
					return_value = 0;
					break;
				case _map_download_stage_cancelled:
					// download canceled, break out
					g_map_download_globals.m_download_thread.in_progress = false;
					return_value = 1;
					break;
				case _map_download_stage_complete:
					// download complete, break out
					g_map_download_globals.m_download_thread.in_progress = false;
					return_value = 2;
					break;
				
				YELO_ASSERT_CASE_UNREACHABLE();
				};

				ReleaseMutex(g_globals_access_mutex);

				// sleep to give the http system time to receive data
				Sleep(20);
				delta = 20.0f / 1000.0f;
			}while(g_map_download_globals.m_download_thread.in_progress);

			return return_value;
		}

		/*!
		 * \brief
		 * Starts a new thread for downloading the requested map.
		 * 
		 * \returns
		 * Returns true if the thread started successfully, otherwise false.
		 * 
		 * Starts a new thread for downloading the requested map.
		 */
		bool	StartDownloadThread()
		{
			// start the download thread
			g_map_download_globals.m_download_thread.thread_handle = CreateThread(nullptr, 0, DownloadMap, nullptr, 0, nullptr); //TODO: V513 http://www.viva64.com/en/V513 Use _beginthreadex/_endthreadex functions instead of CreateThread/ExitThread functions.
			return g_map_download_globals.m_download_thread.thread_handle != nullptr;
		}

		/*!
		 * \brief
		 * Sets the stop download flag and waits for the map download thread to finish.
		 * 
		 * Sets the stop download flag and waits for the map download thread to finish.
		 */
		void	StopDownloadThread()
		{
			// settings this flag causes the download thread to cancel on its next update
			g_map_download_globals.m_download_thread.stop_download = true;

			// wait for the thread to exit
			while(g_map_download_globals.m_download_thread.in_progress)
				Sleep(20);
		}

		/*!
		 * \brief
		 * Starts the map download thread and waits for it to complete.
		 * 
		 * \returns
		 * Returns the next map download stage to progress to.
		 * 
		 * Starts the map download thread and waits for it to complete.
		 */
		map_download_update_stage UpdateStage_DownloadingMap()
		{
			// wait for thread completion, handle any errors
			switch(WaitForSingleObject(g_map_download_globals.m_download_thread.thread_handle, 10))
			{
			case WAIT_OBJECT_0:
				{
					// the thread has finished, handle its return code
					DWORD return_code;
					if(!GetExitCodeThread(g_map_download_globals.m_download_thread.thread_handle, &return_code))
						return _map_download_update_stage_failed;

					if(!CloseHandle(g_map_download_globals.m_download_thread.thread_handle))
					{
						g_map_download_globals.m_download_thread.thread_handle = INVALID_HANDLE_VALUE;
						return _map_download_update_stage_failed;
					}
					g_map_download_globals.m_download_thread.thread_handle = INVALID_HANDLE_VALUE;

					switch(return_code)
					{
					case 0:
						return _map_download_update_stage_failed;
					case 1:
						return _map_download_update_stage_cancelled;
					case 2:
						return _map_download_update_stage_map_download_completion;

					YELO_ASSERT_CASE_UNREACHABLE();
					}
				}
				break;
			case WAIT_TIMEOUT:
				// the thread is still running
				break;
			case WAIT_FAILED:
				YELO_ASSERT_DISPLAY(false, "WAIT_FAILED value in c_map_download_manager::Update");
				break;
			case WAIT_ABANDONED:
				YELO_ASSERT_DISPLAY(false, "WAIT_ABANDONED value in c_map_download_manager::Update");
				break;
			
			YELO_ASSERT_CASE_UNREACHABLE();
			};

			return _map_download_update_stage_downloading_map;
		}

		/*!
		 * \brief
		 * Progresses through the steps required to finish a map download.
		 * 
		 * \param delta
		 * The time in seconds that has passed since the last update.
		 * 
		 * \returns
		 * Returns the next map download stage to progress to.
		 * 
		 * Progresses through the steps required to finish a map download and reconnect to a server.
		 * 
		 * \remarks
		 * The download completion has the following steps:
		 * 1. Add the map to Halo's map headers.
		 * 2. Wait a short while to give the player time to cancel reconnecting if they have tabbed in after the download completed.
		 * 3. Reconnect to the server.
		 */
		map_download_update_stage UpdateStage_DownloadCompletion(real delta)
		{
			switch(g_map_download_globals.m_download_completion.state)
			{
			case _download_completion_state_begin:
				{
					// add the new map to the map headers
					AddMapToMapHeaders();

					g_map_download_globals.m_download_completion.state = _download_completion_state_reconnect_delay;
				}
			case _download_completion_state_reconnect_delay:
				{
					// wait a few seconds before reconnecting to the server
					if(ProgressDelay(g_map_download_globals.m_download_completion.reconnect_delay, delta, true, 0))
						g_map_download_globals.m_download_completion.state = _download_completion_state_reconnect;
				}
				break;
			case _download_completion_state_reconnect:
				{
					ReconnectToServer();

					g_map_download_globals.m_download_completion.state = _download_completion_state_complete;
				}
				break;
			};

			// if this stage is complete, move on to the next stage
			if(g_map_download_globals.m_download_completion.state == _download_completion_state_complete)
				return _map_download_update_stage_complete;
			return _map_download_update_stage_map_download_completion;
		}

		/*!
		 * \brief
		 * The update function, called as part of Halos update loop, which processes a map download request.
		 * 
		 * \param delta
		 * The time in seconds that has passed since the last update.
		 * 
		 * The update function, called as part of Halos update loop, which processes a map download request.
		 */
		void	Update(real delta)
		{
			if(!c_settings_mapdownload::Instance()->m_enabled)
				return;

			// if the master server list download has not started/finished update the stage
			if(!g_map_download_globals.m_master_server_list.completed)
			{
				if((g_map_download_globals.m_master_server_list.completed = UpdateStage_MasterServerList(delta)) &&
					g_map_download_globals.m_master_server_list.succeeded)
				{
					// download is completed, so destroy the downloader
					g_map_download_globals.m_master_server_list.downloader.Dtor();
				}
			}

			// cancel the map download if the player presses esc
			if(_map_download_update_stage_idle != g_map_download_globals.m_map_download_update_stage)
			{
				if(Input::GetKeyState(Enums::_key_code_escape))
					g_map_download_globals.m_map_download_update_stage = _map_download_update_stage_cancelled;
			}

			// update the current download stage
			switch(g_map_download_globals.m_map_download_update_stage)
			{
			case _map_download_update_stage_idle:
				break;
			case _map_download_update_stage_start_download:
				// Display the download UI
				Interface::OpenSauceUI::ShowScreen(K_SCREEN_MAP_DOWNLOAD);

				// start a new thread for downloading the map
				if(!StartDownloadThread())
				{
					g_map_download_globals.m_map_download_update_stage = _map_download_update_stage_failed;
					break;
				}
				g_map_download_globals.m_map_download_update_stage = _map_download_update_stage_downloading_map;
			case _map_download_update_stage_downloading_map:
				// see if the download thread is complete
				g_map_download_globals.m_map_download_update_stage = UpdateStage_DownloadingMap();
				break;
			case _map_download_update_stage_map_download_completion:
				// add the map to the headers and reconnect to the server
				g_map_download_globals.m_map_download_update_stage = UpdateStage_DownloadCompletion(delta);
				break;
			case _map_download_update_stage_failed:
				// wait for the user to press esc
				break;
			case _map_download_update_stage_cancelled:
				// user pressed esc
			case _map_download_update_stage_complete:
				// stop the download thread if it is still running
				StopDownloadThread();

				// reset for the next download
				CleanupMapDownload();
				ResetMapDownload();
				g_map_download_display.ResetDisplay();
				g_map_download_globals.m_map_download_update_stage = _map_download_update_stage_idle;

				// Hide the download UI
				Interface::OpenSauceUI::HideScreen(K_SCREEN_MAP_DOWNLOAD);
				break;

			YELO_ASSERT_CASE_UNREACHABLE();
			};

			// update the display strings
			if(_map_download_update_stage_idle != g_map_download_globals.m_map_download_update_stage)
				UpdateDownloadDisplay();
		}

		/*!
		 * \brief
		 * Sets up the map download system for it to try and download a map via the currently connected server.
		 * 
		 * \param map_name
		 * The name of the map to download.
		 * 
		 * Sets up the map download system for it to try and download a map via the currently connected server.
		 * 
		 * \remarks
		 * This should only be called by the map loading code when the player does not have a required map available.
		 */
		void	AddMapForDownload(const char* map_name)
		{
			if(!c_settings_mapdownload::Instance()->m_enabled || !Interface::OpenSauceUI::IsUIAvailable())
				return;

			if(_map_download_update_stage_idle != g_map_download_globals.m_map_download_update_stage)
				return;

			// make sure the map name is valid
			if(is_null_or_empty(map_name)) return;

			bool success = true;
			// copy the map name for later use
			if(-1 == strcpy_s(g_map_download_globals.m_map_part_definition.downloader.MapElement().m_name, map_name))
				success = false;

			if(!g_map_download_globals.m_servers.dedicated_server.SetCurrentDedicatedServer())
				success = false;

			// failed to collect the required data, so reset and return
			if(!success)
			{
				ResetMapDownload();
				return;
			}

			g_map_download_globals.m_map_download_update_stage = _map_download_update_stage_start_download;
		}

		/*!
		 * \brief
		 * Sets the map download systems variables to their default values.
		 * 
		 * Sets the map download systems variables to their default values.
		 */
		void	Initialize()
		{
			c_settings_mapdownload::Register(Settings::Manager());

			PathCombine(g_map_download_globals.m_paths.user_download_directory, Settings::OpenSauceProfilePath(), "map_download");

			ResetMasterServerDownload();
			ResetMapDownload();

			g_globals_access_mutex = CreateMutex(nullptr, FALSE, nullptr);
		}

		/*!
		 * \brief
		 * Forces a clean up of all resources, files and handles, before Halo closes.
		 * 
		 * Forces a clean up of all resources, files and handles, before Halo closes.
		 */
		void	Dispose()
		{
			// stop the download thread if it is still running
			StopDownloadThread();

			CleanupMasterServerDownload();

			CloseHandle(g_globals_access_mutex);
			
			c_settings_mapdownload::Unregister(Settings::Manager());
		}

		/*!
		 * \brief
		 * The callback which is called when a map download HTTP request is completed.
		 * 
		 * \param download_succeeded
		 * When true the HTTP request was successful.
		 * 
		 * \param buffer
		 * Pointer to the downloaded data.
		 * 
		 * \param buffer_length
		 * Length of the downloaded data.
		 * 
		 * \param component_data
		 * Pointer to memory passed to the HTTP request function.
		 * 
		 * \returns
		 * Returns NULL to clear the component data pointer in the HTTP request.
		 * 
		 * The callback which is called when a map download HTTP request is completed.
		 */
		void*	RequestCompleted_Callback(const bool download_succeeded, const char* buffer, const GHTTPByteCount buffer_length, void* component_data)
		{
			YELO_ASSERT_DISPLAY(component_data, "the component data for a http request is NULL");

			auto request_data = CAST_PTR(s_request_data*, component_data);

			switch(request_data->m_request_type)
			{
				// master server list being downloaded
			case _request_type_master_server_list:
				g_map_download_globals.m_master_server_list.downloader.DownloadCompleteCallback(download_succeeded, buffer, buffer_length, component_data);

				switch(g_map_download_globals.m_master_server_list.downloader.Status())
				{
					case Enums::_http_file_download_status_in_progress:
					case Enums::_http_file_download_status_retry:
						break;
					case Enums::_http_file_download_status_cancelled:
					case Enums::_http_file_download_status_failed:
						g_map_download_globals.m_master_server_list.state = _master_server_list_state_download_failed;
						break;
					case Enums::_http_file_download_status_succeeded:
						g_map_download_globals.m_master_server_list.state = _master_server_list_state_download_succeeded;
						break;
				};
				break;
				// map part definition being downloaded
			case _request_type_map_part_definition:
				g_map_download_globals.m_map_part_definition.downloader.DownloadCompleteCallback(download_succeeded, buffer, buffer_length, component_data);

				switch(g_map_download_globals.m_map_part_definition.downloader.Status())
				{
					case Enums::_http_file_download_status_in_progress:
					case Enums::_http_file_download_status_retry:
						break;
					case Enums::_http_file_download_status_cancelled:
					case Enums::_http_file_download_status_failed:
						g_map_download_globals.m_map_part_definition.state = _map_part_definition_state_download_failed;
						break;
					case Enums::_http_file_download_status_succeeded:
						g_map_download_globals.m_map_part_definition.state = _map_part_definition_state_download_succeeded;
						break;
				};
				break;
				// map part being downloaded
			case _request_type_map_part:
				g_map_download_globals.m_part_download.downloader.DownloadCompleteCallback(download_succeeded, buffer, buffer_length, component_data);

				switch(g_map_download_globals.m_part_download.downloader.Status())
				{
					case Enums::_http_file_download_status_in_progress:
					case Enums::_http_file_download_status_retry:
						break;
					case Enums::_http_file_download_status_cancelled:
					case Enums::_http_file_download_status_failed:
						g_map_download_globals.m_part_download.state = _part_download_state_download_failed;
						break;
					case Enums::_http_file_download_status_succeeded:
						g_map_download_globals.m_part_download.state = _part_download_state_download_complete;
						break;
				};
			}

			return nullptr;
		}

		/*!
		 * \brief
		 *. The callback which is called when a map download HTTP download request is canceled.
		 * 
		 * \param component_data
		 * Pointer to memory passed to the HTTP request function.
		 * 
		 * \returns
		 * Returns NULL to clear the component data pointer in the HTTP request.
		 * 
		 *. The callback which is called when a map download HTTP download request is canceled.
		 */
		void*	RequestCancelled_Callback(void* component_data)
		{
			auto request_data = CAST_PTR(s_request_data*, component_data);

			switch(request_data->m_request_type)
			{
			case _request_type_master_server_list:
				g_map_download_globals.m_master_server_list.downloader.DownloadCancelledCallback(component_data);
				break;
			case _request_type_map_part_definition:
				g_map_download_globals.m_map_part_definition.downloader.DownloadCancelledCallback(component_data);
				break;
			case _request_type_map_part:
				g_map_download_globals.m_part_download.downloader.DownloadCancelledCallback(component_data);
				break;
			}

			return nullptr;
		}
	};};};};
};
#endif