/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Networking/HTTP/BanManager.hpp"

#if PLATFORM_IS_DEDI
#include <blamlib/Halo1/main/console.hpp>
#include <YeloLib/memory/linked_list.hpp>
#include <YeloLib/Halo1/open_sauce/settings/yelo_shared_settings.hpp>

#include "Common/FileIO.hpp"
#include "Networking/HTTP/HTTP.hpp"
#include "Networking/HTTP/HTTPServer.hpp"

namespace Yelo
{
	namespace Networking { namespace HTTP { namespace Server { namespace BanManager
	{
		class c_http_ip_ban_base : public LinkedListNode<c_http_ip_ban_base>
		{
		protected:
			struct
			{
				bool is_active;
				PAD24;
			}m_flags;

		public:
			union
			{
				union
				{
					struct
					{
						byte	d;
						byte	c;
						byte	b;
						byte	a;
					};
					uint32 abcd;
				}v4;

				union
				{
					struct
					{
						int16	a;
						int16	b;
						int16	c;
						int16	d;

						int16	e;
						int16	f;
						int16	g;
						int16	h;
					};
				}v6;
			}m_ip;
			Enums::http_ip_ban_ip_version m_ip_version;

		public:
			bool& IsActive() { return m_flags.is_active; }

			bool CompareIP(uint32 ip)
			{
				return ip == m_ip.v4.abcd;
			}

			bool CompareIP(uint16 a, uint16 b, uint16 c, uint16 d)
			{
				return (a == m_ip.v4.a) &&
					(b == m_ip.v4.b) &&
					(c == m_ip.v4.c) &&
					(d == m_ip.v4.d);
			}

			bool CompareIP(uint16 a, uint16 b, uint16 c, uint16 d, uint16 e, uint16 f, uint16 g, uint16 h)
			{
				return (a == m_ip.v6.a) &&
					(b == m_ip.v6.b) &&
					(c == m_ip.v6.c) &&
					(d == m_ip.v6.d) &&
					(e == m_ip.v6.e) &&
					(f == m_ip.v6.f) &&
					(g == m_ip.v6.g) &&
					(h == m_ip.v6.h);
			}

		public:
			c_http_ip_ban_base(uint32 ip)
			{
				ClearNodeData();
				m_flags.is_active = false;

				m_ip.v4.abcd = ip;
				m_ip_version = Enums::_http_ip_ban_ip_version_ipv4;
			}

			c_http_ip_ban_base(byte a, byte b, byte c, byte d)
			{
				ClearNodeData();
				m_flags.is_active = false;

				m_ip.v4.a = a;
				m_ip.v4.b = b;
				m_ip.v4.c = c;
				m_ip.v4.d = d;
				m_ip_version = Enums::_http_ip_ban_ip_version_ipv4;
			}

			c_http_ip_ban_base(int16 a, int16 b, int16 c, int16 d, int16 e, int16 f, int16 g, int16 h)
			{
				ClearNodeData();
				m_flags.is_active = false;

				m_ip.v6.a = a;
				m_ip.v6.b = b;
				m_ip.v6.c = c;
				m_ip.v6.d = d;
				m_ip.v6.e = e;
				m_ip.v6.f = f;
				m_ip.v6.g = g;
				m_ip.v6.h = h;
				m_ip_version = Enums::_http_ip_ban_ip_version_ipv6;
			}
		};

		class c_http_ip_ban_connection : public c_http_ip_ban_base
		{
			struct
			{
				bool	forget;
				PAD24;
			}m_connection_ban_flags;

			struct
			{
				uint32	total_connections;
				real	cooldown_delta;
			}m_connection_count;

			real m_forget_connection_delta;

		public:
			bool Forget()
			{
				return m_connection_ban_flags.forget;
			}

			void Update(real delta, const uint32 max_connections, const real connection_cooldown, const real forget_connection_time)
			{
				// if the forget connection time has elapsed, this can be deleted
				m_forget_connection_delta += delta;

				if(m_forget_connection_delta >= forget_connection_time)
					m_connection_ban_flags.forget = true;

				if(!m_connection_count.total_connections)
					return;

				// if the connection cooldown has elapsed, reduce the total connections count by 1
				m_connection_count.cooldown_delta += delta;

				if(m_connection_count.cooldown_delta >= connection_cooldown)
					m_connection_count.total_connections--;

				// if the total connections is below the max connection count disable the connection ban
				if(m_connection_count.total_connections < max_connections)
					m_flags.is_active = false;
			}

			void AddConnection(const uint32 max_connections)
			{
				// increase the number of connections this IP has made
				m_connection_count.total_connections++;

				if(m_connection_count.total_connections >= max_connections)
					m_flags.is_active = true;

				// reset the connection forget and cooldown deltas
				m_forget_connection_delta = 0;
				m_connection_count.cooldown_delta = 0;
			}

			c_http_ip_ban_connection(uint32 ip) : c_http_ip_ban_base(ip)
			{
				m_connection_ban_flags.forget = false;

				m_connection_count.cooldown_delta = 0;
				m_connection_count.total_connections = 0;

				m_forget_connection_delta = 0;
			}

			c_http_ip_ban_connection(byte a, byte b, byte c, byte d) : c_http_ip_ban_base(a, b, c, d)
			{
				m_connection_ban_flags.forget = false;

				m_connection_count.cooldown_delta = 0;
				m_connection_count.total_connections = 0;

				m_forget_connection_delta = 0;
			}

			c_http_ip_ban_connection(int16 a, int16 b, int16 c, int16 d, int16 e, int16 f, int16 g, int16 h) : c_http_ip_ban_base(a, b, c, d, e, f, g, h)
			{
				m_connection_ban_flags.forget = false;

				m_connection_count.cooldown_delta = 0;
				m_connection_count.total_connections = 0;

				m_forget_connection_delta = 0;
			}
		};

		// IP ban class for permanent bans set manually by the admin
		class c_http_ip_ban_permanent : public c_http_ip_ban_base
		{
		public:
			c_http_ip_ban_permanent(uint32 ip) : c_http_ip_ban_base(ip)
			{
				m_flags.is_active = true;
			}

			c_http_ip_ban_permanent(byte a, byte b, byte c, byte d) : c_http_ip_ban_base(a, b, c, d)
			{
				m_flags.is_active = true;
			}

			c_http_ip_ban_permanent(int16 a, int16 b, int16 c, int16 d, int16 e, int16 f, int16 g, int16 h) : c_http_ip_ban_base(a, b, c, d, e, f, g, h)
			{
				m_flags.is_active = true;
			}
		};

		class s_ban_manager_globals
		{
		public:
			char				m_ban_list_file[MAX_PATH];

			volatile uint32		m_max_connections_per_ip;
			volatile real		m_connection_cooldown_time;
			volatile real		m_forget_connection_time;
		};
		static s_ban_manager_globals		g_ban_manager_globals;

		HANDLE								g_ban_list_mutex = INVALID_HANDLE_VALUE;
		static c_http_ip_ban_connection*	g_connect_ban_list = nullptr;
		static c_http_ip_ban_permanent*		g_permanent_ban_list = nullptr;

		/*!
		 * \brief
		 * Looks for an IP in the ban lists and returns whether the IP is banned or not.
		 * 
		 * \param ip
		 * The IPv4 IP in a single uint32.
		 * 
		 * \returns
		 * True if the IP is banned, otherwise returns false.
		 * 
		 * Looks for an IP in th ban lists and returns whether the IP is banned or not.
		 */
		bool IPBanned(uint32 ip)
		{
			WaitForSingleObject(g_ban_list_mutex, INFINITE);

			c_http_ip_ban_base* ban_element;

			// see if the ip is permanently banned
			ban_element = g_permanent_ban_list;

			while(ban_element)
			{
				if(ban_element->CompareIP(ip))
				{
					ReleaseMutex(g_ban_list_mutex);
					return ban_element->IsActive();
				}
				ban_element = ban_element->GetNext();
			}

			// see if the ip is banned in the connection ban list
			ban_element = g_connect_ban_list;

			while(ban_element)
			{
				if(ban_element->CompareIP(ip))
				{
					ReleaseMutex(g_ban_list_mutex);
					return ban_element->IsActive();
				}
				ban_element = ban_element->GetNext();
			}

			ReleaseMutex(g_ban_list_mutex);
			return false;
		}

		/*!
		 * \brief
		 * Looks for an IP in the ban lists and returns whether the IP is banned or not.
		 * 
		 * \param version
		 * The IP version to search for.
		 * 
		 * \param a
		 * Part a of the IP address.
		 * 
		 * \param b
		 * Part b of the IP address.
		 * 
		 * \param c
		 * Part c of the IP address.
		 * 
		 * \param d
		 * Part d of the IP address.
		 * 
		 * \param e
		 * Part e of the IP address.
		 * 
		 * \param f
		 * Part f of the IP address.
		 * 
		 * \param g
		 * Part g of the IP address.
		 * 
		 * \param h
		 * Part h of the IP address.
		 * 
		 * \returns
		 * True if the IP is banned, otherwise returns false.
		 * 
		 * Looks for an IP in th ban lists and returns whether the IP is banned or not.
		 */
		bool IPBanned(Enums::http_ip_ban_ip_version version, uint16 a, uint16 b, uint16 c, uint16 d, uint16 e, uint16 f, uint16 g, uint16 h)
		{
			WaitForSingleObject(g_ban_list_mutex, INFINITE);
			c_http_ip_ban_base* ban_element;

			// see if the ip is permanently banned
			ban_element = g_permanent_ban_list;

			while(ban_element)
			{
				bool found = false;
				switch(version)
				{
				case Enums::_http_ip_ban_ip_version_ipv4:
					found = ban_element->CompareIP(a, b, c, d);
					break;
				case Enums::_http_ip_ban_ip_version_ipv6:
					found = ban_element->CompareIP(a, b, c, d, e, f, g, h);
					break;
				}

				if(found)
				{
					ReleaseMutex(g_ban_list_mutex);
					return ban_element->IsActive();
				}

				ban_element = ban_element->GetNext();
			}

			// see if the ip is banned in the connection ban list
			ban_element = g_connect_ban_list;

			while(ban_element)
			{
				bool found = false;
				switch(version)
				{
				case Enums::_http_ip_ban_ip_version_ipv4:
					found = ban_element->CompareIP(a, b, c, d);
					break;
				case Enums::_http_ip_ban_ip_version_ipv6:
					found = ban_element->CompareIP(a, b, c, d, e, f, g, h);
					break;
				}

				if(found)
				{
					ReleaseMutex(g_ban_list_mutex);
					return ban_element->IsActive();
				}

				ban_element = ban_element->GetNext();
			}
			return false;
		}

		/*!
		 * \brief
		 * Updates the http server ban list file path with the supplied suffix.
		 * 
		 * \param suffix
		 * String containing the suffix to append to the ban list file name.
		 * 
		 * \returns
		 * True if the path was built successfully, otherwise false.
		 * 
		 * Updates the http server ban list file path with the supplied suffix.
		 */
		bool SetBanlistFileSuffix(const char* suffix)
		{
			g_ban_manager_globals.m_ban_list_file[0] = 0;

			char filename[MAX_PATH] = "";

			if(-1 == sprintf_s(filename, "httpserver_banlist%s.txt", suffix))
			{
				blam::console_printf(false, "Failed to set the httpserver banlist file suffix");

				g_ban_manager_globals.m_ban_list_file[0] = 0;
				return false;
			}

			if(!FileIO::PathBuild(g_ban_manager_globals.m_ban_list_file, false, 2, Settings::OpenSauceProfilePath(), filename))
			{
				blam::console_printf(false, "Failed to set the httpserver banlist file suffix");

				g_ban_manager_globals. m_ban_list_file[0] = 0;
				return false;
			}

			return true;
		}

		/*!
		 * \brief
		 * Reads the http ban list and adds the permanent bans within.
		 * 
		 * Reads the http ban list and adds the permanent bans within.
		 */
		void LoadBanList()
		{
			FileIO::s_file_info banlist;
			// open the ban list if it exists, otherwise return
			if(FileIO::PathExists(g_ban_manager_globals.m_ban_list_file))
			{
				blam::console_printf(false, "Reading http server ban list file (%s).", g_ban_manager_globals.m_ban_list_file);

				if(Enums::_file_io_open_error_none != FileIO::OpenFile(banlist,
					g_ban_manager_globals.m_ban_list_file,
					Enums::_file_io_open_access_type_read))
				{
					blam::console_printf(false, "Failed to open the existing httpserver banlist");
					return;
				}

				// if the ban list is empty return
				if(banlist.file_size == 0)
				{
					FileIO::CloseFile(banlist);
					return;
				}

				// memory map the file for reading
				if(Enums::_file_io_read_error_none != FileIO::MemoryMapFile(banlist))
				{
					blam::console_printf(false, "Failed to open the existing httpserver banlist");
					FileIO::CloseFile(banlist);
					return;
				}
			}
			else
				return;

			char* file_offset = CAST_PTR(char*, banlist.data_pointer);

			if(strlen(file_offset) == 0)
			{
				FileIO::CloseFile(banlist);
				return;
			}

			WaitForSingleObject(g_ban_list_mutex, INFINITE);

			// delete any existing banlist entries
			if(g_permanent_ban_list)
				DeleteLinkedList(g_permanent_ban_list);

			size_t line_offset = 0;
			bool success = true;
			do
			{
				// skip past comments and new lines
				if(file_offset[line_offset] == '#')
					continue;
				if(file_offset[line_offset] == '\r')
					line_offset++;
				if(file_offset[line_offset] == '\n')
					line_offset++;
				if(banlist.data_length == line_offset)
					break;

				char ip_type[5] = "";
				char ip[Enums::k_max_ip_string_length] = "";

				// get the ip type and ip in separate strings
				int count = sscanf_s(&file_offset[line_offset], "%s %s\r\n", 
					ip_type, NUMBEROF(ip_type), 
					ip, NUMBEROF(ip));

				if(count != 2)
				{
					success = false;
					break;
				}

				Enums::http_ip_ban_ip_version ip_version = Enums::_http_ip_ban_ip_version;

				// determine the ip version
				if(strcmp(ip_type, "IPv4") == 0)
					ip_version = Enums::_http_ip_ban_ip_version_ipv4;
				else if(strcmp(ip_type, "IPv6") == 0)
					ip_version = Enums::_http_ip_ban_ip_version_ipv6;
				else
				{
					success = false;
					break;
				}

				byte a8 = 0, b8 = 0, c8 = 0, d8 = 0;
				short a16 = 0, b16 = 0, c16 = 0, d16 = 0, e16 = 0, f16 = 0, g16 = 0, h16 = 0;

				// separate the ip address
				success = true;
				switch(ip_version)
				{
				case Enums::_http_ip_ban_ip_version_ipv4:
					count = sscanf_s(ip, "%i.%i.%i.%i", &a8, &b8, &c8, &d8);

					if(count != 4)
						success = false;
					break;
				case Enums::_http_ip_ban_ip_version_ipv6:
					count = sscanf_s(ip, "[%04X:%04X:%04X:%04X:%04X:%04X:%04X:%04X]", &a16, &b16, &c16, &d16, &e16, &f16, &g16, &h16);

					if(count != 8)
						success = false;
					break;
				};

				if(!success)
					break;

				c_http_ip_ban_permanent* permanent_ban = nullptr;
				if(ip_version == Enums::_http_ip_ban_ip_version_ipv4)
					permanent_ban = new c_http_ip_ban_permanent(a8, b8, c8, d8);
				else
					permanent_ban = new c_http_ip_ban_permanent(a16, b16, c16, d16, e16, f16, g16, h16);

				// ad the ban list entry to the list
				AppendLinkedListNode(g_permanent_ban_list, permanent_ban);
			}while(banlist.data_length != (line_offset += strcspn(&file_offset[line_offset], "\r\n\0")));

			if(!success)
			{
				blam::console_printf(false, "Failed to read the httpserver banlist");
				if(g_permanent_ban_list)
					DeleteLinkedList(g_permanent_ban_list);
			}

			ReleaseMutex(g_ban_list_mutex);

			FileIO::CloseFile(banlist);
		}

		/*!
		 * \brief
		 * Saves the permanent ban list entries to file.
		 * 
		 * Saves the permanent ban list entries to file.
		 */
		void SaveBanList()
		{
			FileIO::s_file_info banlist;

			// create a new ban list file
			if(Enums::_file_io_open_error_none != FileIO::OpenFile(banlist,
				g_ban_manager_globals.m_ban_list_file,
				Enums::_file_io_open_access_type_write,
				Enums::_file_io_open_create_option_new))
			{
				blam::console_printf(false, "Failed to write the httpserver banlist");
				return;
			}

			char buffer[47] = "# ip version, ip\r\n";

			// write the header comment
			if(Enums::_file_io_write_error_none != FileIO::WriteToFile(banlist, buffer, strlen(buffer)))
			{
				blam::console_printf(false, "Failed to write the httpserver banlist");
				FileIO::CloseFile(banlist);
				return;
			}

			c_http_ip_ban_base* ban_element = g_permanent_ban_list;

			// build the ban list entry
			bool success = true;
			while(ban_element && success)
			{
				switch(ban_element->m_ip_version)
				{
				case Enums::_http_ip_ban_ip_version_ipv4:
					if(-1 == sprintf_s(buffer, "IPv4 %i.%i.%i.%i\r\n",
						ban_element->m_ip.v4.a, ban_element->m_ip.v4.b,
						ban_element->m_ip.v4.c, ban_element->m_ip.v4.d))
					{
						success = false;
						break;
					}
					break;
				case Enums::_http_ip_ban_ip_version_ipv6:
					if(-1 == sprintf_s(buffer, "IPv6 [%04X:%04X:%04X:%04X:%04X:%04X:%04X:%04X]\r\n",
						ban_element->m_ip.v6.a, ban_element->m_ip.v6.b,
						ban_element->m_ip.v6.c, ban_element->m_ip.v6.d,
						ban_element->m_ip.v6.e, ban_element->m_ip.v6.f,
						ban_element->m_ip.v6.g, ban_element->m_ip.v6.h))
					{
						success = false;
						break;
					}
					break;
				};

				// write the ban list entry to file
				if(success && (Enums::_file_io_write_error_none != FileIO::WriteToFile(banlist, buffer, strlen(buffer))))
					success = false;

				ban_element = ban_element->GetNext();
			};

			if(!success)
				blam::console_printf(false, "Failed to write the httpserver banlist");

			FileIO::CloseFile(banlist);
		}

		/*!
		 * \brief
		 * Prints all of the permanent ban list entries to the console with their indices.
		 * 
		 * Prints all of the permanent ban list entries to the console with their indices.
		 */
		void PrintBanList()
		{
			WaitForSingleObject(g_ban_list_mutex, INFINITE);

			// print the table column names
			blam::console_printf(false, "[Index\tIP]");

			c_http_ip_ban_base* permanent_ban = g_permanent_ban_list;

			// print the index and ip of each ban entry
			uint32 index = 0;
			while(permanent_ban)
			{
				switch(permanent_ban->m_ip_version)
				{
				case Enums::_http_ip_ban_ip_version_ipv4:
					blam::console_printf(false, "%i\t%i.%i.%i.%i",
						index,
						permanent_ban->m_ip.v4.a,
						permanent_ban->m_ip.v4.b,
						permanent_ban->m_ip.v4.c,
						permanent_ban->m_ip.v4.d);
					break;
				case Enums::_http_ip_ban_ip_version_ipv6:
					blam::console_printf(false, "%i\t[%04X:%04X:%04X:%04X:%04X:%04X:%04X:%04X]",
						index,
						permanent_ban->m_ip.v6.a,
						permanent_ban->m_ip.v6.b,
						permanent_ban->m_ip.v6.c,
						permanent_ban->m_ip.v6.d,
						permanent_ban->m_ip.v6.e,
						permanent_ban->m_ip.v6.f,
						permanent_ban->m_ip.v6.g,
						permanent_ban->m_ip.v6.h);
					break;
				};
				index++;
				permanent_ban = permanent_ban->GetNext();
			};

			ReleaseMutex(g_ban_list_mutex);
		}

		/*!
		 * \brief
		 * Sets the connection ban variables.
		 * 
		 * \param max_connections
		 * The maximum number of connections an IP can make before it's connctions are denied.
		 * 
		 * \param cooldown_time
		 * The time in seconds for a single connection to be forgotten.
		 * 
		 * \param forget_time
		 * The amount of time to wait before forgetting an IP.
		 * 
		 * Sets the connection ban variables.
		 */
		void SetConnectionBan(const uint32 max_connections, const real cooldown_time, const real forget_time)
		{
			g_ban_manager_globals.m_max_connections_per_ip = max_connections;
			g_ban_manager_globals.m_connection_cooldown_time = cooldown_time;
			g_ban_manager_globals.m_forget_connection_time = forget_time;
		}

		/*!
		 * \brief
		 * Increments the number of connections an IP has had.
		 * 
		 * \param ip
		 * The IPv4 IP in a single uint32.
		 * 
		 * Increments the number of connections an IP has had. If no matching IP is found in the connection
		 * ban list a new element is added.
		 */
		void AddConnection(uint32 ip)
		{
			if(g_ban_manager_globals.m_max_connections_per_ip == 0)
				return;

			WaitForSingleObject(g_ban_list_mutex, INFINITE);

			c_http_ip_ban_base* ban_element = g_connect_ban_list;

			// find an existing connection with the same ip
			while(ban_element)
			{
				if(ban_element->CompareIP(ip))
				{
					c_http_ip_ban_connection* connection_ban = CAST_PTR(c_http_ip_ban_connection*, ban_element);

					// increment the ip's connection count and return
					connection_ban->AddConnection(g_ban_manager_globals.m_max_connections_per_ip);
					ReleaseMutex(g_ban_list_mutex);
					return;
				}
				ban_element = ban_element->GetNext();
			}

			// if an existing connection was not found, add a new connection element
			c_http_ip_ban_connection* new_connection = new c_http_ip_ban_connection(ip);
			new_connection->AddConnection(g_ban_manager_globals.m_max_connections_per_ip);

			AppendLinkedListNode(g_connect_ban_list, new_connection);
			ReleaseMutex(g_ban_list_mutex);
		}

		/*!
		 * \brief
		 * Updates the ban elements in the connection ban list.
		 * 
		 * \param delta
		 * The amount of time that has passed in seconds since the last update.
		 * 
		 * Updates the ban elements in the connection ban list. If a connections forget time has elapsed it is
		 * removed from the connection ban list and deleted.
		 */
		void UpdateConnectionBans(real delta)
		{
			WaitForSingleObject(g_ban_list_mutex, INFINITE);

			c_http_ip_ban_base* ban_element = g_connect_ban_list;

			while(ban_element)
			{
				c_http_ip_ban_connection* connection_ban = CAST_PTR(c_http_ip_ban_connection*, ban_element);

				// update the connection
				connection_ban->Update(delta, g_ban_manager_globals.m_max_connections_per_ip,
					g_ban_manager_globals.m_connection_cooldown_time,
					g_ban_manager_globals.m_forget_connection_time);

				// if the connection can be forgotten hold on to the connection ban object
				c_http_ip_ban_connection* forget_connection = nullptr;
				if(connection_ban->Forget())
					forget_connection = connection_ban;

				ban_element = connection_ban->GetNext();

				// remove the connection that needs to be forgotten, and delete it
				if(forget_connection)
				{
					RemoveLinkedListNode(g_connect_ban_list, forget_connection);
					delete forget_connection;
				}
			};

			ReleaseMutex(g_ban_list_mutex);
		}

		/*!
		 * \brief
		 * Adds an IPv4 or IPv6 IP address to the permanent ban list, if it is not already banned.
		 * 
		 * \param version
		 * The IP version of the address.
		 * 
		 * \param a
		 * Part a of the IP address.
		 * 
		 * \param b
		 * Part b of the IP address.
		 * 
		 * \param c
		 * Part c of the IP address.
		 * 
		 * \param d
		 * Part d of the IP address.
		 * 
		 * \param e
		 * Part e of the IP address.
		 * 
		 * \param f
		 * Part f of the IP address.
		 * 
		 * \param g
		 * Part g of the IP address.
		 * 
		 * \param h
		 * Part h of the IP address.
		 * 
		 * Adds an IPv4 or IPv6 IP address to the permanent ban list, if it is not already banned.
		 */
		void AddPermanentBan(Enums::http_ip_ban_ip_version version, uint16 a, uint16 b, uint16 c, uint16 d, uint16 e, uint16 f, uint16 g, uint16 h)
		{
			if(g_ban_manager_globals.m_ban_list_file[0] == 0)
			{
				blam::console_printf(false, "Http server banlist file not set");
				return;
			}

			WaitForSingleObject(g_ban_list_mutex, INFINITE);

			c_http_ip_ban_permanent* permanent_ban = nullptr;
			switch(version)
			{
			case Enums::_http_ip_ban_ip_version_ipv4:
				// check the address values are less than 256
				if((a > UCHAR_MAX) || (b > UCHAR_MAX) || (c > UCHAR_MAX) || (d > UCHAR_MAX))
				{
					blam::console_printf(false, "IP address values out of range for IPv4");
					ReleaseMutex(g_ban_list_mutex);
					return;
				}

				// if the ip is not already banned, create a new ban list entry
				if(!IPBanned(version, a, b, c, d, e, f, g, h))
				{
					permanent_ban = new c_http_ip_ban_permanent((byte)a, (byte)b, (byte)c, (byte)d);
				}
				break;
			default:
				blam::console_printf(false, "IPv6 not currently supported by the http server.");
			};

			if(!permanent_ban)
				blam::console_printf(false, "That IP is already banned");
			else
			{
				AppendLinkedListNode(g_permanent_ban_list, permanent_ban);

				SaveBanList();
			}

			ReleaseMutex(g_ban_list_mutex);
		}

		/*!
		 * \brief
		 * Removes an entry from the permanent ban list.
		 * 
		 * \param ban_index
		 * The index of the permanent ban to remove.
		 * 
		 * Removes an entry from the permanent ban list.
		 */
		void RemovePermanentBan(uint16 ban_index)
		{
			WaitForSingleObject(g_ban_list_mutex, INFINITE);

			uint32 banlist_count = GetListLength(g_permanent_ban_list);
			if(!banlist_count || (ban_index >= banlist_count))
			{
				blam::console_printf(false, "IP ban index out of bounds.");
				ReleaseMutex(g_ban_list_mutex);
				return;
			}

			// get the node
			c_http_ip_ban_permanent* ban_element = GetNodeByIndex(g_permanent_ban_list, ban_index);

			YELO_ASSERT_DISPLAY(ban_element, "permanent ban not found when the index is within bounds");

			// remove it from the permanent ban list
			RemoveLinkedListNode(g_permanent_ban_list, ban_element);
			delete ban_element;

			// update the ban list file
			SaveBanList();

			ReleaseMutex(g_ban_list_mutex);
		}

		/*!
		 * \brief
		 * Initializes globals to their default values, and creates the ban list mutex.
		 * 
		 * Initializes globals to their default values, and creates the ban list mutex.
		 */
		void Initialize()
		{
			g_ban_manager_globals.m_max_connections_per_ip = 0;
			g_ban_manager_globals.m_connection_cooldown_time = 0;
			g_ban_manager_globals.m_forget_connection_time = 0;

			SetBanlistFileSuffix("");

			g_ban_list_mutex = CreateMutex(nullptr, false, nullptr);
		}

		/*!
		 * \brief
		 * Closes the ban list mutex.
		 * 
		 * Closes the ban list mutex.
		 */
		void Dispose()
		{
			CloseHandle(g_ban_list_mutex);
			g_ban_list_mutex = INVALID_HANDLE_VALUE;
		}

		/*!
		 * \brief
		 * Dedi only scripting function for setting the connection ban variables.
		 * 
		 * \param arguments
		 * Pointer to the HS arguments.
		 * 
		 * \returns
		 * Returns NULL.
		 * 
		 * Dedi only scripting function for setting the connection ban variables.
		 */
		void* HTTPServerSetConnectionBan(void** arguments)
		{
			struct s_arguments {
				const int32 max_connections_per_ip;
				const real connection_cooldown_time;
				const real forget_connection_time;
			}* args = CAST_PTR(s_arguments*, arguments);

			BanManager::SetConnectionBan(args->max_connections_per_ip, args->connection_cooldown_time, args->forget_connection_time);

			return nullptr;
		}

		/*!
		 * \brief
		 * Dedi only scripting function for printing the permanent ban list entries.
		 * 
		 * \returns
		 * Returns NULL.
		 * 
		 * Dedi only scripting function for printing the permanent ban list entries.
		 */
		void* HTTPServerBanlist()
		{
			PrintBanList();

			return nullptr;
		}

		/*!
		 * \brief
		 * Dedi only scripting function for setting the http server ban list file suffix.
		 * 
		 * \param arguments
		 * Pointer to the HS arguments.
		 * 
		 * \returns
		 * Returns NULL.
		 * 
		 * Dedi only scripting function for setting the http server ban list file suffix.
		 */
		void* HTTPServerBanlistFile(void** arguments)
		{
			struct s_arguments {
				const char* suffix;
			}* args = CAST_PTR(s_arguments*, arguments);

			LoadBanList();

			return nullptr;
		}

		/*!
		 * \brief
		 * Dedi only scripting function for banning an IP from requesting data from the http server.
		 * 
		 * \param arguments
		 * Pointer to the HS arguments.
		 * 
		 * \returns
		 * Returns NULL.
		 * 
		 * Dedi only scripting function for banning an IP from requesting data from the http server.
		 */
		void* HTTPServerBanIP(void** arguments)
		{
			struct s_arguments {
				bool ip_version;
				PAD24;
				uint16 a;
				PAD16;
				uint16 b;
				PAD16;
				uint16 c;
				PAD16;
				uint16 d;
				PAD16;
				uint16 e;
				PAD16;
				uint16 f;
				PAD16;
				uint16 g;
				PAD16;
				uint16 h;
				PAD16;
			}* args = CAST_PTR(s_arguments*, arguments);

			AddPermanentBan((args->ip_version ? Enums::_http_ip_ban_ip_version_ipv6 :Enums::_http_ip_ban_ip_version_ipv4),
				args->a, args->b, args->c, args->d,
				args->e, args->f, args->g, args->h);

			return nullptr;
		}

		/*!
		 * \brief
		 * Dedi only scripting function for unbanning an IP from requesting data from the http server.
		 * 
		 * \param arguments
		 * Pointer to the HS arguments.
		 * 
		 * \returns
		 * Returns NULL.
		 * 
		 * Dedi only scripting function for unbanning an IP from requesting data from the http server.
		 */
		void* HTTPServerUnbanIP(void** arguments)
		{
			struct s_arguments {
				uint16 index; // TODO: HS doesn't have anything but signed integers
				PAD16;
			}* args = CAST_PTR(s_arguments*, arguments);

			RemovePermanentBan(args->index);

			return nullptr;
		}
	};};};};
};
#endif