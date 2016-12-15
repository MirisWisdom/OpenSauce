/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Networking/HTTP/MapDownloadServer.hpp"

#if PLATFORM_IS_DEDI
#include <map>
#include <blamlib/Halo1/cache/cache_files.hpp>
#include <blamlib/Halo1/main/console.hpp>
#include <YeloLib/memory/linked_list.hpp>
#include <YeloLib/Halo1/cache/cache_files_yelo.hpp>
#include <YeloLib/cseries/value_conversion.hpp>

#include "Common/FileIO.hpp"
#include "Settings/Settings.hpp"
#include "Networking/HTTP/HTTP.hpp"
#include "Networking/HTTP/HTTPServer.hpp"
#include "Networking/Server.hpp"

namespace Yelo
{
	namespace Networking { namespace HTTP { namespace Server { namespace MapDownload
	{
		enum
		{
			k_max_mutex_wait = 5000,		// maximum amount of time to wait for a mutex request (milliseconds)
			k_max_host_url_length = 1500,	// ~2000 is the  max for IE, and we could have two MAX_PATH length names in the request URL
		};

		class c_part_element : public LinkedListNode<c_part_element>
		{
		public:
			std::string m_name;
			std::string m_redirect_address;

			void Ctor()
			{
				ClearNodeData();

				m_name.assign("");
				m_redirect_address.assign("");
			}
		};

		class c_map_element : public LinkedListNode<c_map_element>
		{
			HANDLE			m_access_mutex;
			volatile uint32	m_lock_count;

		public:
			std::string		m_name;
			TiXmlElement*	m_part_definition;
			c_part_element*	m_parts;

			void Ctor()
			{
				m_access_mutex = CreateMutex(nullptr, false, nullptr);
				YELO_ASSERT_DISPLAY(m_access_mutex, "failed to create map element mutex");
				m_lock_count = 0;

				ClearNodeData();

				m_name.assign("");
				m_part_definition = nullptr;
				m_parts = nullptr;
			}

			void Dtor()
			{
				CloseHandle(m_access_mutex);
				m_access_mutex = INVALID_HANDLE_VALUE;

				m_name.assign("");
				delete m_part_definition;
				m_part_definition = nullptr;
				m_parts = nullptr;
			}

			uint32	LockCount()
			{
				return m_lock_count;
			}

			bool	Lock()
			{
				m_lock_count++;

				if(WaitForSingleObject(m_access_mutex, k_max_mutex_wait) == WAIT_OBJECT_0)
					return true;

				m_lock_count--;
				return false;
			}

			void	Unlock()
			{
				m_lock_count--;
				ReleaseMutex(m_access_mutex);
			}
		};

		class c_map_part_definition_list
		{
			HANDLE			m_access_mutex;
			c_map_element*	m_map_part_definitions;

		public:
			void Ctor()
			{
				m_access_mutex = CreateMutex(nullptr, false, nullptr);
				YELO_ASSERT_DISPLAY(m_access_mutex, "failed to create map list mutex");

				m_map_part_definitions = nullptr;
			}

			void Dtor()
			{
				CloseHandle(m_access_mutex);
				m_access_mutex = INVALID_HANDLE_VALUE;
			}

			bool Lock()
			{
				return WaitForSingleObject(m_access_mutex, k_max_mutex_wait) == WAIT_OBJECT_0;
			}

			void Unlock()
			{
				ReleaseMutex(m_access_mutex);
			}

			bool HasEntries()
			{
				return m_map_part_definitions != nullptr;
			}

			c_map_element* FindDefinition(const char* map_name)
			{
				c_map_element* matched_definition = nullptr;

				if(!m_map_part_definitions)
					return matched_definition;

				if(Lock())
				{
					LinkedListIterator<c_map_element> map_iterator(m_map_part_definitions);

					while(map_iterator.MoveNext())
					{
						if(!map_iterator.Current()->Lock())
						{
							YELO_ASSERT_DISPLAY(false, "failed to lock a map element when searching a part definition list");
							continue;
						}

						if(_stricmp(map_iterator.Current()->m_name.c_str(), map_name) == 0)
							matched_definition = map_iterator.Current();

						map_iterator.Current()->Unlock();
					}

					Unlock();
				}

				return matched_definition;
			}

			bool AddDefinition(c_map_element* map_element)
			{
				if(FindDefinition(map_element->m_name.c_str()))
					return false;

				if(!Lock())
					return false;

				AppendLinkedListNode(m_map_part_definitions, map_element);

				Unlock();
				return true;
			}

			bool RemoveDefinition(c_map_element* map_element)
			{
				if(!Lock())
				{
					YELO_ASSERT_DISPLAY(false, "failed to lock a definition list for map removal");
					return false;
				}

				// wait for the map element to be unlocked
				while(map_element->LockCount() != 0);

				// remove the map element from the list
				RemoveLinkedListNode(m_map_part_definitions, map_element);

				Unlock();
				return true;
			}

			c_map_element* GetDefinitionByIndex(uint32 index)
			{
				if(!Lock())
				{
					YELO_ASSERT_DISPLAY(false, "failed to lock a definition list");
					return false;
				}

				uint32 map_count = GetListLength(m_map_part_definitions);

				c_map_element* map_element = nullptr;

				if(index < map_count)
					map_element = GetNodeByIndex(m_map_part_definitions, index);

				Unlock();

				return map_element;
			}
		};

		class c_map_download_globals
		{
		public:
			struct
			{
				volatile bool system_active;
				bool host_set;
				bool definitions_path_set;
				PAD8;
			}m_flags;

			char					m_map_part_definitions_path[MAX_PATH];
			char					m_host_address[k_max_host_url_length];

			volatile uint32			m_requests_in_progress; // TODO: why not move this to after m_flags and get rid of the PAD8?
		};
		static c_map_download_globals		g_map_download_globals;
		static std::map<char, c_map_part_definition_list>	g_map_part_definition_lists;

		bool ServiceStarted() { return g_map_download_globals.m_flags.system_active; }

		/*!
		 * \brief
		 * Sets the folder containing the map part definitions.
		 * 
		 * \param path
		 * The path to set as the definitions source folder.
		 * 
		 * Sets the folder containing the map part definitions.
		 */
		void SetMapPartDefinitionsPath(const char* path)
		{
			if(g_map_download_globals.m_flags.system_active)
			{
				blam::console_printf(false, "Cannot change the map part definitions path whilst the system is active");
				return;
			}

			g_map_download_globals.m_map_part_definitions_path[0] = 0;
			g_map_download_globals.m_flags.definitions_path_set = false;

			// check the path exists
			if(!FileIO::PathExists(path))
			{
				blam::console_printf(false, "The path \"%s\" does not exist", path);
				return;
			}

			// copy the path
			errno_t result = strcpy_s(g_map_download_globals.m_map_part_definitions_path, path);
			if(result == ERANGE)
			{
				blam::console_printf(false, "The map part definition path is too long");
				return;
			}
			else if(result)
			{
				blam::console_printf(false, "Failed to copy the map part definition path");
				return;
			}

			g_map_download_globals.m_flags.definitions_path_set = true;
		}

		/*!
		 * \brief
		 * Sets the host URL root for the map part redirects.
		 * 
		 * \param root
		 * The URL root address for the map part folders.
		 * 
		 * Sets the host URL root for the map part redirects.
		 */
		void SetRedirectRoot(const char* root)
		{
			if(g_map_download_globals.m_flags.system_active)
			{
				blam::console_printf(false, "Cannot change the URL redirect root whilst the system is active");
				return;
			}

			g_map_download_globals.m_host_address[0] = 0;
			g_map_download_globals.m_flags.host_set = false;

			// check that the url is valid
			c_url_interface url_interface;
			url_interface.ParseURL(root);

			if(url_interface.m_scheme.length() && url_interface.m_address.length())
			{
				// copy the url
				errno_t result = strcpy_s(g_map_download_globals.m_host_address, root);
				if(result == ERANGE)
				{
					blam::console_printf(false, "The host root url is too long (MAX 2000 characters)");
					return;
				}
				else if(result || !FileIO::AppendDirectorySlash(g_map_download_globals.m_host_address, '/'))
				{
					blam::console_printf(false, "Failed to copy the host root url");
					return;
				}
			}
			else
			{
				blam::console_printf(false, "The provided URL does not have a scheme or address section");
				return;
			}

			g_map_download_globals.m_flags.host_set = true;
		}

		/*!
		 * \brief
		 * Gets the definition list index for a specific map name.
		 * 
		 * \param character
		 * The first character of the map name.
		 * 
		 * \returns
		 * Returns the definition list for the specified character.
		 * 
		 * Gets the definition list index for a specific map name.
		 */
		static c_map_part_definition_list* GetList(const char character, const bool create = false)
		{
			char lower_char = tolower(character);
			if(g_map_part_definition_lists.find(lower_char) == g_map_part_definition_lists.end())
			{
				if(create)
				{
					g_map_part_definition_lists[lower_char] = c_map_part_definition_list();
					g_map_part_definition_lists[lower_char].Ctor();
				}
				else
				{
					return nullptr;
				}
			}

			return &g_map_part_definition_lists[lower_char];
		}

		/*!
		 * \brief
		 * Searches the loaded map definitions for a specific map.
		 * 
		 * \param map_name
		 * Name of the map to search for.
		 * 
		 * \returns
		 * Returns a pointer to the map definition element if a matching entry is found, otherwise returns NULL.
		 * 
		 * Searches the loaded map definitions for a specific map. The map name should not have an extension.
		 */
		static c_map_element* FindMap(const char* map_name)
		{
			auto* list = GetList(map_name[0]);

			if(!list || !list->HasEntries())
				return nullptr;

			return list->FindDefinition(map_name);
		}

		/*!
		 * \brief
		 * Searches a map definition entry for a specific part.
		 * 
		 * \param map_element
		 * The map element to search in.
		 * 
		 * \param part_name
		 * Name of the part to search for.
		 * 
		 * \returns
		 * Returns a pointer to the part element in a matching entry is found, otherwise returns NULL.
		 * 
		 * Searches a map definition entry for a specific part.
		 */
		static const c_part_element* FindPart(c_map_element* map_element, const char* part_name)
		{
			// iterate through all the maps parts
			LinkedListIterator<c_part_element> part_iterator(map_element->m_parts);
			while(part_iterator.MoveNext())
			{
				// if the part name matches, return the current part element
				if(part_iterator.Current()->m_name.compare(part_name) == 0)
					return part_iterator.Current();
			}

			return nullptr;
		}

		/*!
		 * \brief
		 * Adds a map element to the relevant definition list.
		 * 
		 * \param map_element
		 * The map definition element to add.
		 * 
		 * \returns
		 * Returns true if the map was added, otherwise returns false if it is a duplicate, or if the list could not be locked.
		 * 
		 * Adds a map element to the relevant definition list.
		 */
		static bool AddMapToList(c_map_element* map_element)
		{
			auto* list = GetList(map_element->m_name[0], true);
			if(!list->AddDefinition(map_element))
			{
				blam::console_printf(false, "failed to add a map definition to the definition list");
				return false;
			}
			return true;
		}

		/*!
		 * \brief
		 * Deletes the map part list from a map definition element.
		 * 
		 * \param map_element
		 * Pointer to the map element to remove the parts from.
		 * 
		 * Deletes the map part list from a map definition element.
		 */
		static void RemoveMapParts(c_map_element* map_element)
		{
			// delete the map parts list
			DeleteLinkedList(map_element->m_parts);
		}

		/*!
		 * \brief
		 * Deletes a map definition from a specific definition list.
		 * 
		 * \param map_list
		 * The map list the map element resides in.
		 * 
		 * \param map_element
		 * The map element to remove from the definition list.
		 * 
		 * Deletes a map definition from a specific definition list.
		 */
		static void RemoveMapDefinition(c_map_part_definition_list& map_list, c_map_element* map_element)
		{
			if(!map_list.RemoveDefinition(map_element))
			{
				blam::console_printf(false, "failed to remove map from a map list");
				return;
			}

			RemoveMapParts(map_element);

			// delete allocated memory and close the mutex
			map_element->Dtor();

			delete map_element;
		}

		/*!
		 * \brief
		 * Processes an xml map node to populate a map element with its parts.
		 * 
		 * \param map_element
		 * The map element to populate.
		 * 
		 * \param map_definition
		 * The xml map part definition to process.
		 * 
		 * \param host_directory
		 * The relative directory of the maps parts on the file server.
		 * 
		 * \returns
		 * Returns true if the map parts were added to the map element successfully, otherwise returns false.
		 * 
		 * Processes an xml map element to populate a map element with its parts.
		 */
		static bool AddMapParts(c_map_element* map_element, const TiXmlElement* map_definition, const char* host_directory)
		{
			// iterate through all of the maps parts
			const TiXmlElement* current_part = map_definition->FirstChildElement("part");
			if(!current_part)
			{
				blam::console_printf(false, "No part nodes present for %s", map_element->m_name.c_str());
				return false;
			}

			do
			{
				const char* name = current_part->Attribute("name");
				const char* md5 = current_part->Attribute("md5");
				const char* encrypted = current_part->Attribute("encrypted");
				const char* unencryptedmd5 = current_part->Attribute("unencrypted_md5");

				// check the md5 strings are the correct length
				bool md5_valid = false;
				if(md5)
					md5_valid = (strlen(md5) == 32);

				bool unencrypted_md5_valid = false;
				if(unencryptedmd5)
					unencrypted_md5_valid = (strlen(unencryptedmd5) == 32);

				bool encrypted_value = false;
				if(encrypted)
					ValueConversion::FromString(encrypted, encrypted_value);

				bool encryption_valid = true;
				if((encrypted_value && !unencryptedmd5) || (!encrypted_value && unencryptedmd5))
					encryption_valid = false;
				else if(encrypted_value && !unencrypted_md5_valid)
					encryption_valid = false;

				// if the part is valid, create a new part element
				if(name && md5 && md5_valid && encryption_valid)
				{
					c_part_element* part_element = new c_part_element();
					part_element->Ctor();

					part_element->m_name.assign(name);

					// build the parts redirection address
					c_url_interface url_interface;
					url_interface.ParseURL(g_map_download_globals.m_host_address);

					if(host_directory && (strlen(host_directory) > 0))
						url_interface.AppendPath(host_directory);
					url_interface.AppendPath(name);

					part_element->m_redirect_address = url_interface.GetURL();

					// add the part to the maps part list
					AppendLinkedListNode(map_element->m_parts, part_element);
				}
				else
				{
					if(!name)
						blam::console_printf(false, "A part node is missing its name");
					if(!md5)
						blam::console_printf(false, "A part node is missing its md5 checksum");
					else if(!md5_valid)
						blam::console_printf(false, "A part nodes md5 checksum is too long/short");
					if(encrypted_value && !unencryptedmd5)
						blam::console_printf(false, "A part node is encrypted but has no unencrypted_md5 attribute");
					else if(!encrypted_value && unencryptedmd5)
						blam::console_printf(false, "A part node is not encrypted but has an unencrypted_md5 attribute");
					else if(encrypted_value && !unencrypted_md5_valid)
						blam::console_printf(false, "An encrypted part nodes unencrypted md5 checksum is too long/short");

					// delete the part list to leave the map element unchanged
					DeleteLinkedList(map_element->m_parts);

					return false;
				}
			}while(current_part = current_part->NextSiblingElement("part"));

			return true;
		}

		/*!
		 * \brief
		 * Processes an xml document to parse the map part definition it contains and add it to the list.
		 * 
		 * \param definition_path
		 * The path to the xml document on disk.
		 * 
		 * \returns
		 * Returns true if the xml was valid and processed successfully otherwise returns false.
		 * 
		 * Processes an xml document to parse the map part definition it contains and add it to the list.
		 */
		static bool AddMapDefinition(const char* definition_path)
		{
			TiXmlDocument definition;
			// load the map part definition
			definition.LoadFile(definition_path);

			// check the xml loaded correctly
			if(definition.Error())
			{
				blam::console_printf("Failed to load and parse the map part definition\nError: %s", definition.ErrorDesc());
				return false;
			}

			// get the xml root
			TiXmlElement* root_node = definition.FirstChildElement("osHTTPServer");
			if(!root_node)
			{
				blam::console_printf(false, "The map part definition has no osHTTPServer root node");
				return false;
			}

			// get the map element
			const TiXmlElement* map_node = root_node->FirstChildElement("map_download");
			if(!map_node)
			{
				blam::console_printf(false, "The map part definition has no map_download node");
				return false;
			}

			const char* name = map_node->Attribute("name");
			const char* md5 = map_node->Attribute("md5");
			const char* algorithm = map_node->Attribute("algorithm");
			const char* host_directory = map_node->Attribute("host_directory");

			// check the md5 string is the correct length
			bool md5_valid = false;
			if(md5)
				md5_valid = (strlen(md5) == 32);

			// if the map is valid, create a new map element
			if(name && md5 && md5_valid && algorithm)
			{
				// remove the extension from the map name
				std::string map_name(name);
				std::string::size_type extension_offset = std::string::npos;

				if((extension_offset = map_name.find(Cache::K_MAP_FILE_EXTENSION)) != std::string::npos)
					map_name.resize(extension_offset);
				else if((extension_offset = map_name.find(Cache::K_MAP_FILE_EXTENSION_YELO)) != std::string::npos)
					map_name.resize(extension_offset);

				if(FindMap(map_name.c_str()))
				{
					blam::console_printf(false, "Map definition for %s already exists", map_name.c_str());
					return false;
				}

				c_map_element* map_element = new c_map_element();
				map_element->Ctor();

				// assign the name of the map and remove its extension for later comparisons
				map_element->m_name = map_name;

				// create a copy of the maps part definition to send to the client
				// remove the host_directory attribute from the client copy as it is unnecessary
				map_element->m_part_definition = map_node->Clone()->ToElement();
				map_element->m_part_definition->RemoveAttribute("host_directory");

				// add the parts to the map element, host_directory can be null
				if(!AddMapParts(map_element, map_node, host_directory))
				{
					// a problem occurred when adding the maps part elements so delete the map element and return
					delete map_element;
					return false;
				}

				// add the completed map element to the definition lists
				if(!AddMapToList(map_element))
				{
					RemoveMapParts(map_element);

					delete map_element;
					return false;
				}
			}
			else
			{
				if(!name)
					blam::console_printf(false, "A map node is missing its name");
				if(!md5)
					blam::console_printf(false, "A map node is missing its md5 checksum");
				else if(!md5_valid)
					blam::console_printf(false, "A map nodes md5 checksum is too long/short");
				if(!algorithm)
					blam::console_printf(false, "A map node does not state its compression algorithm");
				if(!host_directory)
					blam::console_printf(false, "A map node is missing its host_directory");

				return false;
			}

			return true;
		}

		/*!
		 * \brief
		 * Unloads the currently loaded map part definitions.
		 * 
		 * Unloads the currently loaded map part definitions.
		 */
		static void UnloadMapPartDefinitions()
		{
			// loop through all of the map part definition lists
			for(auto& list : g_map_part_definition_lists)
			{
				// delete the first element in the lists until they are empty
				c_map_element* current_element = nullptr;
				while(current_element = list.second.GetDefinitionByIndex(0))
					RemoveMapDefinition(list.second, current_element);
			}

			g_map_part_definition_lists.clear();
		}

		/*!
		 * \brief
		 * Adds the map part definitions in the definition directory to the map lists.
		 * 
		 * \returns
		 * Returns true if any map part definitions were loaded successfully, otherwise returns false.
		 * 
		 * Adds the map part definitions in the definition directory to the map lists.
		 */
		bool LoadMapPartDefinitions()
		{
			// remove any existing definitions
			UnloadMapPartDefinitions();

			HANDLE file_search = INVALID_HANDLE_VALUE;
			WIN32_FIND_DATA file_description;
			char path_string[MAX_PATH] = "";

			// create the search path and file filter
			FileIO::PathBuild(path_string, false, 2, g_map_download_globals.m_map_part_definitions_path, "*.xml");

			file_search = FindFirstFile(path_string, &file_description);
			if(file_search == INVALID_HANDLE_VALUE)
			{
				blam::console_printf(false, "No part definitions in the part definition directory");
				return false;
			}

			// loop for every .xml in the definition directory
			do
			{
				// create the relative path of the definition file
				FileIO::PathBuild(path_string, false, 2, g_map_download_globals.m_map_part_definitions_path, file_description.cFileName);

				// attempt to add the definition to the definition lists
				if(AddMapDefinition(path_string))
					blam::console_printf(false, "Added %s to map server", file_description.cFileName);
				else
					blam::console_printf(false, "Failed to add %s to map server", file_description.cFileName);

			}while(FindNextFile(file_search, &file_description));

			FindClose(file_search);

			return true;
		}

		/*!
		 * \brief
		 * Resets the map download globals to their initial values.
		 * 
		 * Resets the map download globals to their initial values.
		 */
		static void ResetGlobals()
		{
			g_map_download_globals.m_flags.system_active = false;

			g_map_download_globals.m_host_address[0] = 0;
			g_map_download_globals.m_flags.host_set = false;

			g_map_download_globals.m_map_part_definitions_path[0] = 0;
			g_map_download_globals.m_flags.definitions_path_set = false;
		}

		/*!
		 * \brief
		 * Responds to a map part definition request.
		 * 
		 * \param connection
		 * The connection the response is to be sent to.
		 * 
		 * \param request_info
		 * Pointer to the request information sent to the server by the client.
		 * 
		 * \param map
		 * The map definition element to send a response with.
		 * 
		 * Responds to a map part definition request.
		 * 
		 * \remarks
		 * If the request cannot be generated for any reason a server error response code is sent to the client.
		 */
		static void SendMapPartDefinition(mg_connection* connection, c_map_element* map)
		{
			const mg_request_info* request_info = mg_get_request_info(connection);

			// create a new xml document
			TiXmlDocument map_part_definition;

			// add the xml declaration
			TiXmlDeclaration* declaration = new TiXmlDeclaration("1.0", "utf-8", nullptr);
			if(!declaration)
			{
				SendResponse(connection, Enums::_http_status_code_server_error_internal_server_error);
				return;
			}
			map_part_definition.LinkEndChild(declaration);

			// add the root xml element
			TiXmlElement* root = new TiXmlElement("osHTTPServer");
			if(!root)
			{
				SendResponse(connection, Enums::_http_status_code_server_error_internal_server_error);
				return;
			}
			map_part_definition.LinkEndChild(root);

			if(!map->Lock())
			{
				SendResponse(connection, Enums::_http_status_code_server_error_internal_server_error);
				return;
			}

			// create a copy of the map definition xml and add it to the xml document
			TiXmlNode* map_element_clone = map->m_part_definition->Clone();

			map->Unlock();

			if(!map_element_clone)
			{
				SendResponse(connection, Enums::_http_status_code_server_error_internal_server_error);
				return;
			}

			TiXmlElement* map_element = map_element_clone->ToElement();
			if(!map_element)
			{
				delete map_element_clone;

				SendResponse(connection, Enums::_http_status_code_server_error_internal_server_error);
				return;
			}
			root->LinkEndChild(map_element);

			// create an xml printer to get the textual result of the xml node tree
			TiXmlPrinter xml_printer;
			map_part_definition.Accept(&xml_printer);

			// get the final length of the xml document as a string
			// max int size is 2147483647, so 10 chars plus the NULL
			char length_string[NUMBEROF("2147483647")];
			if(-1 == sprintf_s(length_string, "%i", xml_printer.Size()))
			{
				SendResponse(connection, Enums::_http_status_code_server_error_internal_server_error);
				return;
			}

			// send the xml document to the client
			c_http_header response_headers;
			response_headers.AddHeader("Content-Type", "text/xml");
			response_headers.AddHeader("Content-Length", length_string);

			SendResponse(connection, Enums::_http_status_code_successful_ok, &response_headers, xml_printer.CStr(), xml_printer.Size());
		}

		/*!
		 * \brief
		 * Sends a map part definition to the client if the request is valid.
		 * 
		 * \param connection
		 * The connection the response is to be sent to.
		 * 
		 * \param request_info
		 * Pointer to the request information sent to the server by the client.
		 * 
		 * Sends a map part definition to the client if the request is valid.
		 * 
		 * \remarks
		 * A client error status code is returned to the client if the request is incomplete or if no matching map is found.
		 */
		static void ProcessMapPartDefinitionRequest(mg_connection* connection,
			std::string map_name)
		{
			const mg_request_info* request_info = mg_get_request_info(connection);

			// look for the map definition element
			c_map_element* map_element = FindMap(map_name.c_str());

			if(!map_element)
			{
				// the map wasn't found so respond with the file not found status code
				SendResponse(connection, Enums::_http_status_code_client_error_not_found);
				return;
			}

			// the map was found so respond with its map part definition
			SendMapPartDefinition(connection, map_element);
		}

		/*!
		 * \brief
		 * Redirects a part request to the map file server if the request is valid.
		 * 
		 * \param connection
		 * The connection the redirect is to be sent to.
		 * 
		 * \param request_info
		 * Pointer to the request information sent to the server by the client.
		 * 
		 * Redirects a part request to the map file server if the request is valid.
		 * 
		 * \remarks
		 * A client error status code is sent to the client if the request is incomplete, or if the map/part was not found.
		 */
		static void ProcessMapPartDownloadReferral(mg_connection* connection,
			std::string map_name,
			std::string part_name)
		{
			const mg_request_info* request_info = mg_get_request_info(connection);

			// remove the extension from the map name
			std::string::size_type extension_offset = std::string::npos;
			if((extension_offset = map_name.find(Cache::K_MAP_FILE_EXTENSION)) != std::string::npos)
				map_name.resize(extension_offset);
			else if((extension_offset = map_name.find(Cache::K_MAP_FILE_EXTENSION_YELO)) != std::string::npos)
				map_name.resize(extension_offset);

			// look for the part in the part definition
			c_map_element* map_element = FindMap(map_name.c_str());

			if(!map_element)
			{
				// the part was not found so respond with the file not found status code
				SendResponse(connection, Enums::_http_status_code_client_error_not_found);
				return;
			}

			if(!map_element->Lock())
			{
				// the part was not found so respond with the file not found status code
				SendResponse(connection, Enums::_http_status_code_client_error_request_timeout);
				return;
			}

			const c_part_element* part_element = FindPart(map_element, part_name.c_str());

			if(part_element)
			{
				// the part element exists, so respond with a redirect to the part file
				c_http_header response_headers;
				response_headers.AddHeader("Content-Type", "text/html");
				response_headers.AddHeader("Content-Length", "0");
				response_headers.AddHeader("Location", part_element->m_redirect_address.c_str());

				SendResponse(connection, Enums::_http_status_code_redirection_found, &response_headers);
			}
			else
			{
				// the part was not found so respond with the file not found status code
				SendResponse(connection, Enums::_http_status_code_client_error_not_found);
			}

			map_element->Unlock();
		}

		/*!
		 * \brief
		 * The callback called by the HTTP server when a URL starting with "map_download" is requested.
		 * 
		 * \param callback_event
		 * The type of event that has been started by mongoose.
		 * 
		 * \param connection
		 * The connection the response is to be sent to.
		 * 
		 * \param request_info
		 * Pointer to the request information sent to the server by the client.
		 * 
		 * \returns
		 * Always returns a non-NULL value (currently an empty character string);
		 * 
		 * The callback called by the HTTP server when a URL starting with "map_download" is requested.
		 */
		void* ServerCallback(mg_event callback_event, mg_connection* connection)
		{
			const mg_request_info* request_info = mg_get_request_info(connection);

			// the system is not running, so deny the request
			if(!g_map_download_globals.m_flags.system_active)
				SendResponse(connection, Enums::_http_status_code_server_error_service_unavailable);
			else if(callback_event == MG_NEW_REQUEST)
			{
				// deny the request if it has come from somewhere other than the HCE client
				int agent_header_index = FindHeader(request_info, "User-Agent");
				if((-1 == agent_header_index) || (!request_info->http_headers[agent_header_index].value))
				{
					SendResponse(connection, Enums::_http_status_code_client_error_method_not_allowed);
					return "";
				}
				else
				{
					const char* value_pointer = strstr(request_info->http_headers[agent_header_index].value, K_HTTP_CLIENT_ID);
					if(value_pointer != request_info->http_headers[agent_header_index].value)
					{
						SendResponse(connection, Enums::_http_status_code_client_error_method_not_allowed);
						return "";
					}
				}

				// keep track of the number of requests in progress
				g_map_download_globals.m_requests_in_progress++;

				if(strcmp(request_info->request_method, "GET") != 0)
				{
					// the other http methods are not supported
					SendResponse(connection, Enums::_http_status_code_client_error_method_not_allowed);
				}
				else
				{
					c_url_interface url_interface;
					url_interface.ParsePath(request_info->uri);
					url_interface.ParseQuery(request_info->query_string);

					std::string resource = url_interface.GetResource();

					// the actual part definition file might not be called this but it is accessed by this id anyway
					if((resource.length() == 0) || (resource.compare("map_download") != 0))
						SendResponse(connection, Enums::_http_status_code_client_error_method_not_allowed);
					else
					{
						std::string map_name = "";
						std::string part_name = "";
						bool has_map_name = url_interface.GetQueryValue("map", map_name) && (map_name.length() != 0);
						bool has_part_name = url_interface.GetQueryValue("part", part_name) && (part_name.length() != 0);

						if(has_map_name && has_part_name)
							ProcessMapPartDownloadReferral(connection, map_name, part_name);
						else if(has_map_name && !has_part_name)
							ProcessMapPartDefinitionRequest(connection, map_name);
						else
							SendResponse(connection, Enums::_http_status_code_client_error_bad_request);
					}
				}

				// request complete, reduce the requests in progress count
				YELO_ASSERT_DISPLAY(g_map_download_globals.m_requests_in_progress != 0, "requests in progress is already 0");
				g_map_download_globals.m_requests_in_progress--;
			}
			else
				SendResponse(connection, Enums::_http_status_code_client_error_method_not_allowed);

			return "";
		}

		/*!
		 * \brief
		 * Starts the map download server.
		 * 
		 * Starts the map download server.
		 * 
		 * \remarks
		 * The map download server can only be started when both the url root and map part definitions path are set.
		 */
		void StartMapServer()
		{
			// check the server startup conditions are met
			if(!HTTP::Server::ServerStarted())
			{
				blam::console_printf(false, "The HTTP server is not running");
				return;
			}

			if(Yelo::Server::NetworkSvGlobals()->initialized)
			{
				blam::console_printf(false, "Cannot start the map download server whilst the mp server is running");
				return;
			}

			if(!g_map_download_globals.m_flags.definitions_path_set)
			{
				blam::console_printf(false, "The map part definitions path has not yet been set");
				return;
			}

			if(!g_map_download_globals.m_flags.host_set)
			{
				blam::console_printf(false, "The file host root url has not yet been set");
				return;
			}

			if(g_map_download_globals.m_flags.system_active)
			{
				blam::console_printf(false, "The map download server is already running");
				return;
			}

			if(!LoadMapPartDefinitions())
			{
				blam::console_printf(false, "Failed to start the map download server");
				return;
			}

			g_map_download_globals.m_flags.system_active = true;

			blam::console_printf(false, "Map download server started successfully");
		}

		/*!
		 * \brief
		 * Stops the map download server.
		 * 
		 * Stops the map download server.
		 */
		void StopMapServer()
		{
			// check the server shutdown conditions are met
			if(Yelo::Server::NetworkSvGlobals()->initialized)
			{
				blam::console_printf(false, "Cannot stop the map download server whilst the multiplayer server is running");
				return;
			}

			if(!g_map_download_globals.m_flags.system_active)
			{
				blam::console_printf(false, "The map download server is not running");
				return;
			}

			// disable the system so that no more requests are made
			g_map_download_globals.m_flags.system_active = false;

			// wait until any remaining requests are complete
			while(g_map_download_globals.m_requests_in_progress)
				Sleep(100);

			UnloadMapPartDefinitions();

			blam::console_printf(false, "The map download server has been stopped");
		}

		/*!
		 * \brief
		 * Initializes the system by resetting the globals.
		 * 
		 * Initializes the system by resetting the globals.
		 */
		void Initialize()
		{
			ResetGlobals();
		}

		/*!
		 * \brief
		 * Stops the map download server prior to the dedi exiting.
		 * 
		 * Stops the map download server prior to the dedi exiting.
		 */
		void Dispose()
		{
			// stop the server if it is running
			StopMapServer();

			for(auto& list : g_map_part_definition_lists)
				list.second.Dtor();

			g_map_part_definition_lists.clear();
		}

		/*!
		 * \brief
		 * Starts the map server.
		 * 
		 * \returns
		 * Returns NULL.
		 * 
		 * Starts the map server. The server can only be started when no MP game is in progress.
		 */
		void* MapDownloadStartServer()
		{
			StartMapServer();

			return nullptr;
		}

		/*!
		 * \brief
		 * Stops the map server.
		 * 
		 * \returns
		 * Returns NULL.
		 * 
		 * Stops the map server.
		 */
		void* MapDownloadStopServer()
		{
			StopMapServer();

			return nullptr;
		}

		/*!
		 * \brief
		 * Sets the root URL of the map parts.
		 * 
		 * \param arguments
		 * Pointer to the commands arguments.
		 * 
		 * \returns
		 * Returns NULL.
		 * 
		 * Sets the root URL of the map parts.
		 */
		void* MapDownloadSetHost(void** arguments)
		{
			struct s_arguments {
				const char* host;
			}* args = CAST_PTR(s_arguments*, arguments);

			SetRedirectRoot(args->host);

			return nullptr;
		}

		/*!
		 * \brief
		 * Sets the relative or absolute path to the map part definitions folder.
		 * 
		 * \param arguments
		 * Pointer to the commands arguments.
		 * 
		 * \returns
		 * Returns NULL.
		 * 
		 * Sets the relative or absolute path to the map part definitions folder.
		 */
		void* MapDownloadSetPartDefinitionsPath(void** arguments)
		{
			struct s_arguments {
				const char* map_part_definitions;
			}* args = CAST_PTR(s_arguments*, arguments);

			SetMapPartDefinitionsPath(args->map_part_definitions);

			return nullptr;
		}

		/*!
		 * \brief
		 * Reloads the map part definitions without requiring the server to be stopped.
		 * 
		 * \returns
		 * Returns NULL.
		 * 
		 * Reloads the map part definitions without requiring the server to be stopped.
		 */
		void* MapDownloadReloadMapPartDefinitions()
		{
			LoadMapPartDefinitions();

			return nullptr;
		}
	};};};};
};
#endif
