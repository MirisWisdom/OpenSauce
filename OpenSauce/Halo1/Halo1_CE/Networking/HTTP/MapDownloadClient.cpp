/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Networking/HTTP/MapDownloadClient.hpp"

#if !PLATFORM_IS_DEDI

#include "Common/LinkedList.hpp"
#include "Common/FileIO.hpp"
#include "Common/YeloSettings.hpp"
#include "Common/XXTEA.hpp"
#include "Common/7zip.hpp"
#include "Common/Zip.hpp"

#include "Networking/Networking.hpp"
#include "Networking/Server.hpp"
#include "Interface/TextBlock.hpp"
#include "Interface/Controls.hpp"
#include "Memory/FunctionInterface.hpp"
#include "Game/EngineFunctions.hpp"
#include "TagGroups/CacheFiles.hpp"

#include "Networking/HTTP/HTTP.hpp"
#include "Networking/HTTP/HTTPClient.hpp"
#include "Networking/HTTP/HTTPServer.hpp"
#include "Networking/HTTP/c_file_downloader.hpp"
#include "Networking/HTTP/c_xml_downloader.hpp"

namespace Yelo
{
	// TODO: set the master server list to add servers directly to the global list rather than copying after the download
	//TODO: comments, check current are correct and add missing comments
	namespace Networking { namespace HTTP { namespace Client { namespace MapDownload
	{
		static const char* g_master_server_list_url = "http://os.halomods.com/Halo1/CE/MapDownload/Halo1_CE_MapDownloadMasterServers.xml";

		enum
		{
			k_server_name_length_max = 65,
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

		enum map_download_update_stage
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

		enum download_completion_state
		{
			_download_completion_state_begin,
			_download_completion_state_reconnect_delay,
			_download_completion_state_reconnect,
			_download_completion_state_complete,

			_download_completion_state
		};

		enum map_download_stage
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

		enum master_server_list_state
		{
			_master_server_list_state_begin,
			_master_server_list_state_downloading,
			_master_server_list_state_download_failed,
			_master_server_list_state_download_succeeded,
			_master_server_list_state_stage_complete,

			_master_server_list_state
		};

		enum map_part_definition_state
		{
			_map_part_definition_state_begin,
			_map_part_definition_state_start_download,
			_map_part_definition_state_downloading,
			_map_part_definition_state_download_failed,
			_map_part_definition_state_download_succeeded,
			_map_part_definition_state_stage_complete,

			_map_part_definition_state
		};

		enum part_download_state
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

		enum archive_extraction_state
		{
			_archive_extraction_state_begin,
			_archive_extraction_state_archive_ready,
			_archive_extraction_state_complete,

			_archive_extraction_state
		};

		class c_part_element : public LinkedListNode<c_part_element>
		{
		public:
			char					m_name[MAX_PATH];
			char					m_md5[k_md5_string_length_max];
			bool					m_encrypted;
			PAD24;
			char					m_unencrypted_md5[k_md5_string_length_max];
			real					m_download_progress;

			void Ctor()
			{
				ClearNodeData();
				m_name[0] = 0;
				m_md5[0] = 0;
				m_encrypted = false;
				m_unencrypted_md5[0] = 0;
				m_download_progress = 0.0f;
			}
		};

		class c_map_element
		{
		public:
			char					m_name[MAX_PATH];
			char					m_filename[MAX_PATH];
			char					m_md5[k_md5_string_length_max];
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

				m_parts = NULL;
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
					m_parts = NULL;
				}
			}
		};

		class c_http_server : public LinkedListNode<c_http_server>
		{
		public:
			char		m_name[k_server_name_length_max];
			char		m_address[Enums::k_max_url_length];

			virtual void Ctor()
			{
				ClearNodeData();

				m_name[0] = 0;
				m_address[0] = 0;
			}

			virtual void Dtor()
			{
				m_name[0] = 0;
				m_address[0] = 0;
			}

			bool SetHTTPServer(const char* name, const char* address)
			{
				m_name[0] = NULL;
				m_address[0] = NULL;

				if(-1 == strcpy_s(m_name, k_server_name_length_max, name))
					return false;

				if(-1 == strcpy_s(m_address, Enums::k_max_url_length, address))
					return false;

				return true;
			}
		};

		class c_dedicated_server : public c_http_server
		{
		public:
			char		m_mp_address[Enums::k_max_ip_port_string_length];
			char		m_mp_password[k_server_password_length];
			byte		m_mp_password_key[16];

		protected:
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
				Networking::s_transport_address* server_transport = NetworkGameClient()->GetSvIpAddress();

				bool success = true;
				char server_ip_string[Enums::k_max_ip_port_string_length] = "";

				// set up the server url string
				// generate the ip string for either ipv4 or ipv6
				if(server_transport->address_length == 4)
				{
					if(-1 == sprintf_s(server_ip_string, Enums::k_max_ip_string_length,
						"%i.%i.%i.%i",
						server_transport->ipv4.class_d,
						server_transport->ipv4.class_c,
						server_transport->ipv4.class_b,
						server_transport->ipv4.class_a))
						success = false;
				}
				else
				{
					if(-1 == sprintf_s(server_ip_string, Enums::k_max_ip_string_length,
						"[%04X:%04X:%04X:%04X:%04X:%04X:%04X:%04X]",
						server_transport->ipv6.class_h,
						server_transport->ipv6.class_g,
						server_transport->ipv6.class_f,
						server_transport->ipv6.class_e,
						server_transport->ipv6.class_d,
						server_transport->ipv6.class_c,
						server_transport->ipv6.class_b,
						server_transport->ipv6.class_a))
						success = false;
				}

				// build a string with the ip and port, for reconnecting to the server
				if(-1 == sprintf_s(m_mp_address, Enums::k_max_ip_port_string_length, "%s:%i", server_ip_string, server_transport->port))
					success = false;

				if(-1 == sprintf_s(server_ip_string, Enums::k_max_ip_port_string_length, "%s:%i/%s",
					server_ip_string,
					server_transport->port + 1,
					Server::GetServiceURIRoot(Enums::_http_service_enumeration_map_download)))
					success = false;

				if(success)
					success = SetHTTPServer("Dedicated Server", server_ip_string);

				return success;
			}

		public:
			void Ctor()
			{
				c_http_server::Ctor();

				m_mp_address[0] = 0;
				m_mp_password[0] = 0;
				ZeroMemory(m_mp_password_key, k_server_password_length);
			}

			void Dtor()
			{
				c_http_server::Dtor();

				m_mp_address[0] = 0;
				m_mp_password[0] = 0;
				ZeroMemory(m_mp_password_key, k_server_password_length);
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

				if(-1 == sprintf_s(m_mp_password, sizeof(m_mp_password), "%S", Networking::NetworkGameClient()->GetPassword()))
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

		class c_master_server : public c_http_server
		{
		public:
			/*!
			 * \brief
			 * Duplicates the node and all of its siblings to create a complete copy of the list.
			 * 
			 * \param previous_node
			 * The node to set as the previous node.
			 * 
			 * \returns
			 * Returns a copy of the node with all of it's sibling nodes.
			 * 
			 * Duplicates the node and all of it's siblings to create a completed copy of the list.
			 */
			c_master_server* DeepCopy(c_master_server* previous_node)
			{
				c_master_server* copy = new c_master_server();

				copy->Ctor();
				copy->SetHTTPServer(m_name, m_address);

				if(GetNext())
					copy->SetNext(CAST_PTR(c_master_server*, GetNext())->DeepCopy(copy));

				copy->SetPrevious(previous_node);

				return copy;
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
				c_master_server*		server_list;
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
					const char* address = server_node->Attribute("address");

					if(name && address)
					{
						// create a new master server entry
						c_master_server* download_server = new c_master_server();
						download_server->Ctor();

						download_server->SetHTTPServer(name, address);

						AppendLinkedListNode(m_master_server_list.server_list, download_server);
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

				s_request_data* request_data = new s_request_data();
				request_data->m_request_type = _request_type_master_server_list;
				
				request_args.m_component = Enums::_http_client_component_map_download;
				request_args.m_component_data = request_data;

				request_args.m_manual_think = true;
				request_args.m_request_timeout = k_connection_timeout;
			}

		public:
			c_master_server*& ServerList() { return m_master_server_list.server_list; }

			void Ctor()
			{
				c_xml_downloader::Ctor();

				m_master_server_list.server_list = NULL;
			}

			void Dtor()
			{
				c_xml_downloader::Dtor();

				if(m_master_server_list.server_list)
					DeleteLinkedList(m_master_server_list.server_list);

				m_master_server_list.server_list = NULL;
			}

			void* DownloadCompleteCallback(const bool download_succeeded, const char* buffer, const GHTTPByteCount buffer_length, void* component_data)
			{
				c_xml_downloader::DownloadCompleteCallback(download_succeeded, buffer, buffer_length, component_data);

				s_request_data* request_data = CAST_PTR(s_request_data*, component_data);
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

		class c_map_part_definition : public c_xml_downloader
		{
			struct
			{
				struct
				{
					bool map_is_yelo;
					PAD24;
				}m_flags;

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
					const char* encrypted = current_part->Attribute("encrypted");
					const char* unencrypted_md5 = current_part->Attribute("unencrypted_md5");

					bool encrypted_value = false;
					if(encrypted)
						encrypted_value = Settings::ParseBoolean(encrypted);

					bool encryption_valid = true;
					if(encrypted_value && !unencrypted_md5)
						encryption_valid = false;

					// every part must have a name and md5 checksum
					if(name && md5 && encryption_valid)
					{
						c_part_element* part_element = new c_part_element();

						part_element->Ctor();

						strcpy_s(part_element->m_name, MAX_PATH, name);
						strcpy_s(part_element->m_md5, k_md5_string_length_max, md5);
						part_element->m_encrypted = encrypted_value;
						if(encrypted && unencrypted_md5)
							strcpy_s(part_element->m_unencrypted_md5, k_md5_string_length_max, unencrypted_md5);

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

				m_map_part_definition.m_flags.map_is_yelo = false;

				m_map_part_definition.map_element.Ctor();
			}

			void Dtor()
			{
				c_xml_downloader::Dtor();

				m_map_part_definition.m_flags.map_is_yelo = false;

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
				const char* algorithm = map->Attribute("algorithm");

				// must have both the name and md5 checksum
				if(name && md5 && algorithm)
				{
					// get the maps extension to determine whether it is yelo or not
					char extension[6] = "";
					if(!FileIO::GetFileExtension(extension, sizeof(extension), name))
						return false;

					if(strcmp(extension, ".yelo") == 0)
						m_map_part_definition.m_flags.map_is_yelo = true;
					else if(strcmp(extension, ".map") == 0)
						m_map_part_definition.m_flags.map_is_yelo = false;
					else
						return false;

					strcpy_s(m_map_part_definition.map_element.m_filename, MAX_PATH, name);
					strcpy_s(m_map_part_definition.map_element.m_md5, k_md5_string_length_max, md5);

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
				if((Status() = Enums::_http_file_download_status_succeeded) && !ProcessMapPartDefinition())
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
				uint32						m_size;
				char						m_data[k_max_part_download_size];
				byte*						m_decryption_key;
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

				m_part_downloader.m_part_element = NULL;
				m_part_downloader.m_size = 0;
				ZeroMemory(&m_part_downloader.m_data[0], sizeof(m_part_downloader.m_data));
				m_part_downloader.m_decryption_key = NULL;
			}

			void Dtor()
			{
				c_file_downloader::Dtor();

				m_file_downloader.m_attempts.max_attempts = k_max_download_attempts;
				m_file_downloader.m_attempts.retry_delay = k_retry_delay_time;

				m_part_downloader.m_part_element = NULL;
				m_part_downloader.m_size = 0;
				ZeroMemory(&m_part_downloader.m_data[0], sizeof(m_part_downloader.m_data));
				m_part_downloader.m_decryption_key = NULL;
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
				//compare the datas md5 with that in the part definition
				return Engine::CompareMD5(m_part_downloader.m_data,
					m_part_downloader.m_size,
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
					m_part_downloader.m_size,
					m_part_downloader.m_decryption_key))
					return false;

				// verify the unencrypted data with the parts unencrypted md5
				if(!Engine::CompareMD5(m_part_downloader.m_data,
					m_part_downloader.m_size,
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
			bool SavePart(const char* save_location)
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
						m_part_downloader.m_size))
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
			void Update(real delta)
			{
				c_file_downloader::Update(delta);

				if(Status() == Enums::_http_file_download_status_in_progress)
				{
					if(QueryDownloadState(m_file_downloader.m_http.request_index) >= GHTTPReceivingFile)
					{
						int total_bytes = QueryBytesTotal(m_file_downloader.m_http.request_index);
						int received_bytes = QueryBytesReceived(m_file_downloader.m_http.request_index);

						m_part_downloader.m_part_element->m_download_progress = (real)received_bytes / (real)total_bytes;
					}
					else
						m_part_downloader.m_part_element->m_download_progress = 0.0f;
				}

			}

			void* DownloadCompleteCallback(const bool download_succeeded, const char* buffer, const GHTTPByteCount buffer_length, void* component_data)
			{
				c_file_downloader::DownloadCompleteCallback(download_succeeded, buffer, buffer_length, component_data);

				s_request_data* request_data = CAST_PTR(s_request_data*, component_data);
				delete request_data;

				m_part_downloader.m_size = 0;

				// if the returned data is larger than the maximum permitted, fail
				if(!download_succeeded || !buffer || (buffer_length > k_max_part_size) || (buffer_length == 0))
				{
					Status() = Enums::_http_file_download_status_failed;
					return NULL;
				}

				// download size can be less than 1MB
				m_part_downloader.m_size = (uint32)buffer_length;

				// validate and decrypt the part
				if(!ValidatePart() || !DecryptPart())
					Status() = Enums::_http_file_download_status_failed;
				else
					Status() = Enums::_http_file_download_status_succeeded;

				// if validation or decryption failed retry the download if applicable
				if((Status() == Enums::_http_file_download_status_failed) && Retry())
					Status() = Enums::_http_file_download_status_retry;

				return NULL;
			}

			void* DownloadCancelledCallback(void* component_data)
			{
				c_file_downloader::DownloadCancelledCallback(component_data);

				s_request_data* request_data = CAST_PTR(s_request_data*, component_data);
				delete request_data;

				m_part_downloader.m_size = 0;

				Status() = Enums::_http_file_download_status_cancelled;

				return NULL;
			}
		};

		struct s_map_download_globals
		{
			bool						m_downloads_enabled;
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

				c_master_server*			server_list;
				c_master_server_list		downloader;
			}m_master_server_list;

			struct
			{
				map_part_definition_state	state;
				PAD16;

				c_map_part_definition		downloader;
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
				PAD16;
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

		enum
		{
			k_display_title_string_length = 19 + MAX_PATH,
			k_display_labels_string_length = 31,
			k_display_details_string_length = 100,
			k_display_cancel_string_length = 22,
		};

		struct s_download_display_globals
		{
			TextBlock* m_background_tint;
			TextBlock* m_dialog_background;

			TextBlock* m_title;
			TextBlock* m_details_labels;
			TextBlock* m_details_values;
			TextBlock* m_cancel;

			wchar_t m_title_string[k_display_title_string_length];
			wchar_t m_details_labels_string[k_display_labels_string_length];
			wchar_t m_details_values_string[k_display_details_string_length];
			wchar_t m_cancel_string[k_display_cancel_string_length];
		};
		static s_download_display_globals g_download_display_globals;

		/*!
		 * \brief
		 * Resets the display strings to their initial values.
		 * 
		 * Resets the display strings to their initial values.
		 */
		void	ResetDownloadDisplay()
		{
			g_download_display_globals.m_title_string[0] = 0;
			g_download_display_globals.m_details_labels_string[0] = 0;
			g_download_display_globals.m_details_values_string[0] = 0;
			g_download_display_globals.m_cancel_string[0] = 0;

			wcscpy_s(g_download_display_globals.m_title_string, k_display_title_string_length, L"OS Map Downloader\n");
			wcscpy_s(g_download_display_globals.m_details_labels_string, k_display_labels_string_length, L"Stage:\nProgress:\nStatus:\nHost:");
			wcscpy_s(g_download_display_globals.m_details_values_string, k_display_details_string_length, L"\n\n");
			wcscpy_s(g_download_display_globals.m_cancel_string, k_display_cancel_string_length, L"Press [esc] to cancel");
		}

		/*!
		 * \brief
		 * Initializes the display components to their sizes and positions.
		 * 
		 * Initializes the display components to their sizes and positions.
		 */
		void	SetupDisplay(D3DPRESENT_PARAMETERS* pParameters)
		{
			// full screen black tint
			g_download_display_globals.m_background_tint->SetIgnoreTextHeight(true);
			g_download_display_globals.m_background_tint->SetDimensions(pParameters->BackBufferWidth, pParameters->BackBufferHeight);
			g_download_display_globals.m_background_tint->SetBackColor(0xCC000000);
			g_download_display_globals.m_background_tint->Attach(Enums::_attach_method_top_left, 0, 0, 0, 0);

			// opaque background for the whole display
			g_download_display_globals.m_dialog_background->SetIgnoreTextHeight(true);
			g_download_display_globals.m_dialog_background->SetDimensions(500, 180);
			g_download_display_globals.m_dialog_background->SetBackColor(0xFF555555);
			g_download_display_globals.m_dialog_background->Attach(Enums::_attach_method_center, 0, 0, 0, 0);

			// title bar for the display title and map name
			g_download_display_globals.m_title->SetIgnoreTextHeight(true);
			g_download_display_globals.m_title->SetPadding(8);
			g_download_display_globals.m_title->SetFont("Lucida Sans Unicode", 25, FW_NORMAL, false, 6);
			g_download_display_globals.m_title->SetDimensions(500, 60);
			g_download_display_globals.m_title->SetBackColor(0xFF777777);
			g_download_display_globals.m_title->Attach(Enums::_attach_method_center, 0, 0, 0, -70);
			g_download_display_globals.m_title->SetText(g_download_display_globals.m_title_string);
			g_download_display_globals.m_title->SetTextAlign(DT_LEFT | DT_VCENTER);
			g_download_display_globals.m_title->SetTextColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.75f));

			// labels for the download details
			g_download_display_globals.m_details_labels->SetIgnoreTextHeight(true);
			g_download_display_globals.m_details_labels->SetFont("Lucida Sans Unicode", 25, FW_NORMAL, false, 6);
			g_download_display_globals.m_details_labels->SetPadding(0);
			g_download_display_globals.m_details_labels->SetDimensions(120, 115);
			g_download_display_globals.m_details_labels->SetBackColor(0x00000000);
			g_download_display_globals.m_details_labels->Attach(Enums::_attach_method_center, 0, 0, -190, 25);
			g_download_display_globals.m_details_labels->SetText(g_download_display_globals.m_details_labels_string);
			g_download_display_globals.m_details_labels->SetTextAlign(DT_RIGHT);
			g_download_display_globals.m_details_labels->SetTextColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.75f));

			// download details
			g_download_display_globals.m_details_values->SetIgnoreTextHeight(true);
			g_download_display_globals.m_details_values->SetFont("Lucida Sans Unicode", 25, FW_NORMAL, false, 6);
			g_download_display_globals.m_details_values->SetPadding(0);
			g_download_display_globals.m_details_values->SetDimensions(376, 115);
			g_download_display_globals.m_details_values->SetBackColor(0x00000000);
			g_download_display_globals.m_details_values->Attach(Enums::_attach_method_center, 0, 0, 62, 25);
			g_download_display_globals.m_details_values->SetText(g_download_display_globals.m_details_values_string);
			g_download_display_globals.m_details_values->SetTextAlign(DT_LEFT);
			g_download_display_globals.m_details_values->SetTextColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.75f));

			// cancel bar
			g_download_display_globals.m_cancel->SetIgnoreTextHeight(true);
			g_download_display_globals.m_cancel->SetPadding(0);
			g_download_display_globals.m_cancel->SetFont("Lucida Sans Unicode", 25, FW_NORMAL, false, 6);
			g_download_display_globals.m_cancel->SetDimensions(500, 30);
			g_download_display_globals.m_cancel->SetBackColor(0xFF777777);
			g_download_display_globals.m_cancel->Attach(Enums::_attach_method_center, 0, 0, 0, 100);
			g_download_display_globals.m_cancel->SetText(g_download_display_globals.m_cancel_string);
			g_download_display_globals.m_cancel->SetTextAlign(DT_CENTER | DT_VCENTER);
			g_download_display_globals.m_cancel->SetTextColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.75f));

			g_download_display_globals.m_background_tint->Refresh();
			g_download_display_globals.m_dialog_background->Refresh();
			g_download_display_globals.m_title->Refresh();
			g_download_display_globals.m_details_labels->Refresh();
			g_download_display_globals.m_details_values->Refresh();
			g_download_display_globals.m_cancel->Refresh();
		}

		/*!
		 * \brief
		 * Allocates the TextBlocks used to display the download progress.
		 * 
		 * \param pDevice
		 * The current render device.
		 * 
		 * \param pParameters
		 * The current devices presentation parameters.
		 * 
		 * Allocates the TextBlocks used to display the download progress.
		 */
		void	Initialize3D(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters)
		{
			g_download_display_globals.m_background_tint = new TextBlock(pDevice, pParameters);
			g_download_display_globals.m_dialog_background = new TextBlock(pDevice, pParameters);
			g_download_display_globals.m_title = new TextBlock(pDevice, pParameters);
			g_download_display_globals.m_details_labels = new TextBlock(pDevice, pParameters);
			g_download_display_globals.m_details_values = new TextBlock(pDevice, pParameters);
			g_download_display_globals.m_cancel = new TextBlock(pDevice, pParameters);

			ResetDownloadDisplay();
			SetupDisplay(pParameters);
		}

		/*!
		 * \brief
		 * Releases download display resources, before the device is reset.
		 * 
		 * Releases download display resources, before the device is reset.
		 */
		void	OnLostDevice()
		{
			g_download_display_globals.m_background_tint->OnLostDevice();
			g_download_display_globals.m_dialog_background->OnLostDevice();
			g_download_display_globals.m_title->OnLostDevice();
			g_download_display_globals.m_details_labels->OnLostDevice();
			g_download_display_globals.m_details_values->OnLostDevice();
			g_download_display_globals.m_cancel->OnLostDevice();
		}

		/*!
		 * \brief
		 * Re-allocates resources after the device has been reset.
		 * 
		 * \param pParameters
		 * The presentation parameters the device was reset with.
		 * 
		 * Re-allocates resources after the device has been reset.
		 */
		void	OnResetDevice(D3DPRESENT_PARAMETERS* pParameters)
		{
			g_download_display_globals.m_background_tint->OnResetDevice(pParameters);
			g_download_display_globals.m_dialog_background->OnResetDevice(pParameters);
			g_download_display_globals.m_title->OnResetDevice(pParameters);
			g_download_display_globals.m_details_labels->OnResetDevice(pParameters);
			g_download_display_globals.m_details_values->OnResetDevice(pParameters);
			g_download_display_globals.m_cancel->OnResetDevice(pParameters);

			SetupDisplay(pParameters);
		}

		/*!
		 * \brief
		 * Renders the download progress display.
		 * 
		 * Renders the download progress display.
		 */
		void	Render()
		{
			if(g_map_download_globals.m_map_download_update_stage == _map_download_update_stage_idle)
				return;

			g_download_display_globals.m_background_tint->Render();
			g_download_display_globals.m_dialog_background->Render();
			g_download_display_globals.m_title->Render(g_download_display_globals.m_title_string);
			g_download_display_globals.m_details_labels->Render(g_download_display_globals.m_details_labels_string);
			g_download_display_globals.m_details_values->Render(g_download_display_globals.m_details_values_string);
			g_download_display_globals.m_cancel->Render(g_download_display_globals.m_cancel_string);
		}

		/*!
		 * \brief
		 * Releases the displays resources and deletes the display components.
		 * 
		 * Releases the displays resources and deletes the display components.
		 */
		void	Release()
		{
			g_download_display_globals.m_background_tint->Release();
			g_download_display_globals.m_dialog_background->Release();
			g_download_display_globals.m_title->Release();
			g_download_display_globals.m_details_labels->Release();
			g_download_display_globals.m_details_values->Release();
			g_download_display_globals.m_cancel->Release();

			delete g_download_display_globals.m_background_tint;
			g_download_display_globals.m_background_tint = NULL;

			delete g_download_display_globals.m_dialog_background;
			g_download_display_globals.m_dialog_background = NULL;

			delete g_download_display_globals.m_title;
			g_download_display_globals.m_title = NULL;

			delete g_download_display_globals.m_details_labels;
			g_download_display_globals.m_details_labels = NULL;

			delete g_download_display_globals.m_details_values;
			g_download_display_globals.m_details_values = NULL;

			delete g_download_display_globals.m_cancel;
			g_download_display_globals.m_cancel = NULL;
		}

		/*!
		 * \brief
		 * Updates the download display with the current map download progress.
		 * 
		 * Updates the download display with the current map download progress.
		 */
		void	UpdateDownloadDisplay()
		{
			WaitForSingleObject(g_globals_access_mutex, INFINITE);

			// has to be MAX_PATH as we print a map name to it (just in case)
			wchar_t buffer[MAX_PATH];

			// build the title string
			swprintf_s(g_download_display_globals.m_title_string,
				k_display_title_string_length,
				L"OS Map Downloader\n%S",
				g_map_download_globals.m_map_part_definition.downloader.MapElement().m_name);

			// build the details string
			g_download_display_globals.m_details_values_string[0] = 0;

			wcstring stage_string = NULL;
			switch(g_map_download_globals.m_map_download_update_stage)
			{
			case _map_download_update_stage_downloading_map:
				{
					switch(g_map_download_globals.m_download_thread.stage)
					{
					case _map_download_stage_map_part_definition:
						stage_string = L"Downloading Map Part List";
						break;
					case _map_download_stage_map_part_download:
						stage_string = L"Downloading Map";
						break;
					case _map_download_stage_map_archive_extraction:
						stage_string = L"Extracting Map";
						break;
					default:
						stage_string = L"";
						break;
					};
				}
				break;
			case _map_download_update_stage_map_download_completion:
				stage_string = L"Connecting To Server";
				break;
			case _map_download_update_stage_failed:
				stage_string = L"Download Failed";
				break;
			}

			if(stage_string)
				wcscat_s(g_download_display_globals.m_details_values_string,
					k_display_details_string_length,
					stage_string);
			
			// display the countdown to server reconnection
			if(_map_download_update_stage_map_download_completion == g_map_download_globals.m_map_download_update_stage)
			{
				swprintf_s(buffer, MAX_PATH, L" (%1.f)",
					ceil(g_map_download_globals.m_download_completion.reconnect_delay));

				wcscat_s(g_download_display_globals.m_details_values_string,
					k_display_details_string_length,
					buffer);
			}

			wcscat_s(g_download_display_globals.m_details_values_string,
				k_display_details_string_length,
				L"\n");

			// print the part download details
			if(_map_download_stage_map_part_download == g_map_download_globals.m_download_thread.stage)
			{
				// get the number of parts
				int part_count = GetListLength(g_map_download_globals.m_map_part_definition.downloader.MapElement().m_parts);

				LinkedListIterator<c_part_element> part_iterator(g_map_download_globals.m_map_part_definition.downloader.MapElement().m_parts);

				// get the cumulative progress of all the parts downloaded
				real percentage = 0.0f;
				while(part_iterator.MoveNext())
					percentage += part_iterator.Current()->m_download_progress;

				// calculate the percentage of the total
				percentage = (percentage / (real)part_count) * 100;

				swprintf_s(buffer, MAX_PATH, L"%3.f%%", percentage);

				wcscat_s(g_download_display_globals.m_details_values_string,
					k_display_details_string_length,
					buffer);
			}

			wcscat_s(g_download_display_globals.m_details_values_string,
				k_display_details_string_length,
				L"\n");

			if((_map_download_stage_map_part_definition == g_map_download_globals.m_download_thread.stage) ||
				(_map_download_stage_map_part_download == g_map_download_globals.m_download_thread.stage))
			{
				int32 http_request = -1;
				// print the download status
				switch(g_map_download_globals.m_download_thread.stage)
				{
				case _map_download_stage_map_part_definition:
					http_request = g_map_download_globals.m_map_part_definition.downloader.HTTPRequestIndex();
					break;
				case _map_download_stage_map_part_download:
					http_request = g_map_download_globals.m_part_download.downloader.HTTPRequestIndex();
					break;
				};

				// default to printing "Connecting" text if no download is in progress
				bool print_connecting = true;

				if(-1 != http_request)
				{
					GHTTPState download_state = QueryDownloadState(http_request);

					// if the status code is not 2xx either something has gone wrong, or we are being redirected
					// either way, display the connecting text
					bool status_successful = false;
					if(download_state > GHTTPReceivingStatus)
					{
						int status_code = QueryResponseStatus(http_request);

						// integer math, shouldn't round this up
						if(status_code / 100 == 2)
							status_successful = true;
					}

					// only display the download details if the intended file is being downloaded (only on a 2xx status code)
					if((GHTTPReceivingFile == download_state) && status_successful)
					{
						wcscat_s(g_download_display_globals.m_details_values_string,
							k_display_details_string_length,
							L"Downloading");

						print_connecting = false;
					}
				}

				if(print_connecting)
				{
					wcscat_s(g_download_display_globals.m_details_values_string,
						k_display_details_string_length,
						L"Connecting");
				}

				if(g_map_download_globals.m_servers.server_iterator && g_map_download_globals.m_servers.server_iterator->Current())
				{
					// add the host name
					swprintf_s(buffer, MAX_PATH, L"\n%S", g_map_download_globals.m_servers.server_iterator->Current()->Server()->m_name);

					wcscat_s(g_download_display_globals.m_details_values_string,
						k_display_details_string_length,
						buffer);
				}
			}

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
				part_iterator = NULL;

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

			unsigned char* uncompressed_data = NULL;
			size_t uncompressed_size = 0;
			bool success = false;

			switch(map_element.m_format)
			{
			case _map_compression_format_zip:
				{
					unzFile zip_file = NULL;
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
			default:
				ASSERT(true, "invalid compression format");
			};

			// if the decompression failed, delete the data
			if(!success)
				return false;

			// validate the data
			success = Engine::CompareMD5(CAST_PTR(cstring, uncompressed_data), (DWORD)uncompressed_size, map_element.m_md5);

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

					if(!FileIO::PathBuild(map_file_path,
						false, 2, "maps",
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
						CAST_PTR(const char*, uncompressed_data),
						(DWORD)uncompressed_size))
						break;

					success = true;
				}while(false);

				FileIO::CloseFile(map_file);
			}

			delete [] uncompressed_data;
			uncompressed_data = NULL;

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

			// calculate the maps crc value
			// cannot test agains the cache's internal crc value as that is not always correct
			uint32 map_crc = Cache::CalculateCRC(map_element.m_filename);

			if(map_crc == 0xFFFFFFFF)
				return false;

			cstring map_extension = (map_element.m_map_is_yelo ? ".yelo" : ".map");

			// skip the checksum calculation as that would unload the ui...which is all kinds of bad
			Engine::MapListAddMap(map_element.m_filename, map_extension, true);

			// set the crc for the last map entry which is the map that was just added
			Cache::t_multiplayer_map_data* map_data = Cache::MultiplayerMaps();
			map_data->elements[map_data->count - 1].crc = map_crc;

			return true;
		}

		/*!
		 * \brief
		 * Reconnects to the server, using its IP and the provided password (if applicable).
		 * 
		 * Reconnects to the server, using its IP and the provided password (if applicable).
		 */
		void	ReconnectToServer()
		{
			Engine::Networking::ConnectToServer(g_map_download_globals.m_servers.dedicated_server.m_mp_address,
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
				if(!CreateDirectory(g_map_download_globals.m_paths.user_download_directory, NULL))
					return false;

			if(!FileIO::PathExists(g_map_download_globals.m_paths.parts_save_directory))
				if(!CreateDirectory(g_map_download_globals.m_paths.parts_save_directory, NULL))
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
			c_http_server_instance* instance = new c_http_server_instance(&g_map_download_globals.m_servers.dedicated_server);

			AppendLinkedListNode(g_map_download_globals.m_servers.server_list_head, instance);

			// TODO: move this to a global shuffle namespace
			//shuffle loop
			int list_length = GetListLength(g_map_download_globals.m_master_server_list.server_list);
			ASSERT(list_length > 256, "max server list count reached");

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
			g_map_download_globals.m_servers.server_iterator = NULL;

			if(g_map_download_globals.m_servers.server_list_head)
				DeleteLinkedList(g_map_download_globals.m_servers.server_list_head);

			// delete the map part definition resources
			g_map_download_globals.m_map_part_definition.downloader.Stop();
			g_map_download_globals.m_map_part_definition.downloader.Dtor();

			// delete the part download resources
			delete g_map_download_globals.m_part_download.part_iterator;
			g_map_download_globals.m_part_download.part_iterator = NULL;

			g_map_download_globals.m_part_download.downloader.Stop();
			g_map_download_globals.m_part_download.downloader.Dtor();

			// delete the download directory and all its contents
			// no need to check the return value as if the deletion failed there isn't anything we can do about it
			FileIO::Delete_Directory(g_map_download_globals.m_paths.user_download_directory, true, true);
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
			g_map_download_globals.m_master_server_list.server_list = NULL;
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
			g_map_download_globals.m_servers.server_iterator = NULL;
			g_map_download_globals.m_servers.server_list_head = NULL;

			g_map_download_globals.m_download_thread.stop_download = false;
			g_map_download_globals.m_download_thread.in_progress = false;
			g_map_download_globals.m_download_thread.thread_handle = (HANDLE)NULL_HANDLE;
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
			g_map_download_globals.m_master_server_list.server_list = NULL;
			g_map_download_globals.m_master_server_list.downloader.Ctor();
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
					default:
						ASSERT(true, "invalid file download status");
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
			default:
				ASSERT(true, "invalid master server list state");
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
					ASSERT(!g_map_download_globals.m_servers.server_iterator->Current(), "server list iterators current instance is null");

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
					default:
						ASSERT(true, "invalid file download status");
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
			default:
				ASSERT(true, "invalid map part definition state");
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
					ASSERT(!g_map_download_globals.m_servers.server_iterator->Current(), "server list iterators current instance is null");

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
					default:
						ASSERT(true, "invalid file download status");
					};
				}
				break;
			case _part_download_state_download_failed:
				{
					// clean up the request
					part_downloader.Stop();

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
			default:
				ASSERT(true, "invalid part download state");
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

			int32 return_value = -1;
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
					// download cancelled, break out
					g_map_download_globals.m_download_thread.in_progress = false;
					return_value = 1;
					break;
				case _map_download_stage_complete:
					// download complete, break out
					g_map_download_globals.m_download_thread.in_progress = false;
					return_value = 2;
					break;
				default:
					ASSERT(true, "invalid map download stage");
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
			g_map_download_globals.m_download_thread.thread_handle = CreateThread(NULL, 0, DownloadMap, NULL, 0, NULL);
			return g_map_download_globals.m_download_thread.thread_handle != 0;
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
						g_map_download_globals.m_download_thread.thread_handle = (HANDLE)NULL_HANDLE;
						return _map_download_update_stage_failed;
					}
					g_map_download_globals.m_download_thread.thread_handle = (HANDLE)NULL_HANDLE;

					switch(return_code)
					{
					case 0:
						return _map_download_update_stage_failed;
					case 1:
						return _map_download_update_stage_cancelled;
					case 2:
						return _map_download_update_stage_map_download_completion;
					default:
						ASSERT(true, "map download thread returned an invalid code");
					}
				}
				break;
			case WAIT_TIMEOUT:
				// the thread is still running
				break;
			case WAIT_FAILED:
				ASSERT(true, "WAIT_FAILED value in c_map_download_manager::Update");
			case WAIT_ABANDONED:
				ASSERT(true, "WAIT_ABANDONED value in c_map_download_manager::Update");
			default:
				ASSERT(true, "invalid wait return value in c_map_download_manager::Update");
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
			if(!g_map_download_globals.m_downloads_enabled)
				return;

			// if the master server list download has not started/finished update the stage
			if(!g_map_download_globals.m_master_server_list.completed)
			{
				if((g_map_download_globals.m_master_server_list.completed = UpdateStage_MasterServerList(delta)) &&
					g_map_download_globals.m_master_server_list.succeeded)
				{
					// download is completed, so deep copy the server list and destroy the downloaders copy
					g_map_download_globals.m_master_server_list.server_list = g_map_download_globals.m_master_server_list.downloader.ServerList()->DeepCopy(NULL);
					g_map_download_globals.m_master_server_list.downloader.Dtor();
				}
			}

			// cancel the map download if the player presses esc
			if(_map_download_update_stage_idle != g_map_download_globals.m_map_download_update_stage)
			{
				if(Input::GetKeyState(Enums::_KeyEsc))
					g_map_download_globals.m_map_download_update_stage = _map_download_update_stage_cancelled;
			}

			// update the current download stage
			switch(g_map_download_globals.m_map_download_update_stage)
			{
			case _map_download_update_stage_idle:
				break;
			case _map_download_update_stage_start_download:
				// disable the update function for ui widgets, to prevent menu navigation mid-download
				Memory::FunctionProcessUpdateUIWidgetsDisabled() = true;

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
				ResetDownloadDisplay();
				g_map_download_globals.m_map_download_update_stage = _map_download_update_stage_idle;

				// the download is complete/cancelled so re-enable the ui widget update function
				Memory::FunctionProcessUpdateUIWidgetsDisabled() = false;
				break;
			default:
				ASSERT(true, "invalid map download stage");
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
			if(!g_map_download_globals.m_downloads_enabled)
				return;

			if(_map_download_update_stage_idle != g_map_download_globals.m_map_download_update_stage)
				return;

			// make sure the map name is valid
			if(!map_name || (strlen(map_name) == 0)) return;

			bool success = true;
			// copy the map name for later use
			if(-1 == strcpy_s(g_map_download_globals.m_map_part_definition.downloader.MapElement().m_name, MAX_PATH, map_name))
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
		 * Sets the map download systems variables to their deafult values.
		 * 
		 * Sets the map download systems variables to their deafult values.
		 */
		void	Initialize()
		{
			PathCombine(g_map_download_globals.m_paths.user_download_directory, Settings::OpenSauceProfilePath(), "map_download");

			ResetMasterServerDownload();
			ResetMapDownload();

			g_globals_access_mutex = CreateMutex(NULL, FALSE, NULL);
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
		}

		/*!
		 * \brief
		 * Saves whether map downloads are enabled to the users settings.
		 * 
		 * \param xml_element
		 * The parent element to add the map_download element to.
		 * 
		 * Saves whether map downloads are enabled to the users settings.
		 */
		void	LoadSettings(TiXmlElement* xml_element)
		{
			g_map_download_globals.m_downloads_enabled = true;

			if(!xml_element)
				return;

			TiXmlElement* map_download_element = xml_element->FirstChildElement("map_download");
			if(!map_download_element)
				return;

			const char* enabled = map_download_element->Attribute("enabled");
			if(enabled)
				g_map_download_globals.m_downloads_enabled = Settings::ParseBoolean(enabled);
		}

		/*!
		 * \brief
		 * Reads whether map downloads are enabled from the users settings.
		 * 
		 * \param xml_element
		 * The parent xml element to read the setting from.
		 * 
		 * Reads whether map downloads are enabled from the users settings.
		 */
		void	SaveSettings(TiXmlElement* xml_element)
		{
			if(!xml_element)
				return;

			TiXmlElement* map_download_element = new TiXmlElement("map_download");
			xml_element->LinkEndChild(map_download_element);

			map_download_element->SetAttribute("enabled", Settings::BooleanToString(g_map_download_globals.m_downloads_enabled));
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
			ASSERT(!component_data, "the component data for a http request is NULL");

			s_request_data* request_data = CAST_PTR(s_request_data*, component_data);

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

			return NULL;
		}

		/*!
		 * \brief
		 *. The callback which is called when a map download HTTP download request is cancelled.
		 * 
		 * \param component_data
		 * Pointer to memory passed to the HTTP request function.
		 * 
		 * \returns
		 * Returns NULL to clear the component data pointer in the HTTP request.
		 * 
		 *. The callback which is called when a map download HTTP download request is cancelled.
		 */
		void*	RequestCancelled_Callback(void* component_data)
		{
			s_request_data* request_data = CAST_PTR(s_request_data*, component_data);

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

			return NULL;
		}
	};};};};
};
#endif