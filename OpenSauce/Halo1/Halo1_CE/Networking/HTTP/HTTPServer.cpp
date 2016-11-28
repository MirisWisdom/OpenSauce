/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Networking/HTTP/HTTPServer.hpp"

#if PLATFORM_IS_DEDI
#include <mongoose/mongoose.h>

#include <blamlib/Halo1/main/console.hpp>
#include <YeloLib/Halo1/shell/shell_windows_command_line.hpp>

#include "Settings/Settings.hpp"
#include "Common/FileIO.hpp"
#include "Networking/HTTP/HTTP.hpp"
#include "Networking/HTTP/BanManager.hpp"
#include "Networking/HTTP/MapDownloadServer.hpp"
#include "Networking/Server.hpp"
#endif

namespace Yelo
{
	namespace Networking { namespace HTTP { namespace Server
	{
#if PLATFORM_IS_DEDI

	#ifdef _DEBUG
	#define HTTP_COMPONENT_SERVER(id, root, service_started, callback) { id, root, service_started, callback }
	#else
	#define HTTP_COMPONENT_SERVER(id, root, service_started, callback) { root, service_started, callback }
	#endif

#else
	#define HTTP_COMPONENT_SERVER(id, root, service_started, callback) { root }
#endif
		static s_http_service g_http_services[] = 
		{
			HTTP_COMPONENT_SERVER("MapDownloadService", "map_download", Server::MapDownload::ServiceStarted, Server::MapDownload::ServerCallback),
		};BOOST_STATIC_ASSERT(Enums::_http_service_enumeration == NUMBEROF(g_http_services));

#undef HTTP_COMPONENT_SERVER

		/*!
		 * \brief
		 * Returns the URL root directory for the requested service enum.
		 * 
		 * \param service
		 * The service to return the root directory for.
		 * 
		 * \returns
		 * A char pointer to the root directory string.
		 * 
		 * Returns the URL root directory for the requested service enum.
		 */
		const char* GetServiceURIRoot(Enums::http_service_enumeration service)
		{
			YELO_ASSERT_DISPLAY(service < Enums::_http_service_enumeration, "Invalid HTTP service root requested");

			return g_http_services[service].m_uri_root;
		}

#if PLATFORM_IS_DEDI

#define K_HTTP_SERVER_HEADER K_HTTP_DEDI_ID "/" BOOST_STRINGIZE(K_OPENSAUCE_VERSION_BUILD_MAJ) "." BOOST_STRINGIZE(K_OPENSAUCE_VERSION_BUILD_MIN) "." BOOST_STRINGIZE(K_OPENSAUCE_VERSION_BUILD)
#define K_HTTP_VERSION_STR "HTTP/" BOOST_STRINGIZE(HTTP_VERSION_MAJOR) "." BOOST_STRINGIZE(HTTP_VERSION_MINOR)

		const char* g_http_server_options[] = 
		{
			"listening_ports", nullptr,
			"num_threads", nullptr,
			"document_root", nullptr,
			"throttle", nullptr,
			"enable_directory_listing", "no",
			nullptr
		};

		struct s_http_server_globals
		{
			struct
			{
				bool	serve_files;
				PAD24;
			}m_flags;

			struct
			{
				std::string ports;
				std::string root;
				std::string threads;
				std::string throttle;
			}m_config;

			mg_context* m_context;
		};
		static s_http_server_globals g_http_server_globals;

		bool ServerStarted() { return (g_http_server_globals.m_context != nullptr); }

		int FindHeader(const mg_request_info* request_info, const char* name)
		{
			for(int i = 0; i < NUMBEROF(request_info->http_headers); i++)
			{
				if(!request_info->http_headers[i].name)
					break;

				if(strcmp(request_info->http_headers[i].name, name) == 0)
					return i;
			}
			return NONE;
		}

		// TODO: condense these two functions
		static void LogRequest(const long ip,
			const long port,
			const char* uri,
			const char* query)
		{
			// union to break the ip into it's byte components
			union{
				long ip_address;
				struct
				{
					byte d;
					byte c;
					byte b;
					byte a;
				};
			};
			ip_address = ip;

			// write to the log
			Yelo::Server::EventLog(Enums::_server_event_type_http_server, L"REQUEST\t(%i.%i.%i.%i:%i)\t%S\t%S",
				a, b, c, d, port, uri, query);
		}

		static void LogReponse(const long ip,
			const long port,
			const Enums::http_status_code status_code)
		{
			// union to break the ip into it's byte components
			union{
				long ip_address;
				struct
				{
					byte d;
					byte c;
					byte b;
					byte a;
				};
			};
			ip_address = ip;

			// write to the log
			Yelo::Server::EventLog(Enums::_server_event_type_http_server, L"RESPONSE\t(%i.%i.%i.%i:%i)\t%i\t%S",
				a, b, c, d, port, g_status_codes[status_code].m_code, g_status_codes[status_code].m_reason);
		}

		/*!
		 * \brief
		 * The callback function all mongoose events pass through.
		 * 
		 * \param callback_event
		 * The type of event.
		 * 
		 * \param connection
		 * The connection information of the client.
		 * 
		 * \param request_info
		 * The request information for the event.
		 * 
		 * \returns
		 * Returns a pointer to an empty string. If a null pointer was returned, mongoose will treat the request as a normal file request.
		 * 
		 * The callback function all mongoose events pass through. The requests are passed to the appropriate service by
		 * comparing the first path directory to the service root's.
		 */
		static void* ServerCallback(mg_event callback_event, mg_connection* connection)
		{
			const mg_request_info* request_info = mg_get_request_info(connection);

			// if the return value is NULL, mongoose will attempt to serve the files itself
			void* return_value = (g_http_server_globals.m_flags.serve_files ? nullptr : "");

			switch(callback_event)
			{
			case MG_NEW_REQUEST:
				break;
			case MG_REQUEST_COMPLETE:
				return "";
			case MG_EVENT_LOG:
				Yelo::Server::EventLog(Enums::_server_event_type_http_server, L"MONGOOSE\t%S", request_info->ev_data);
				return "";
			case MG_INIT_SSL:
				YELO_ASSERT_DISPLAY(callback_event != MG_INIT_SSL, "mongoose is attepting to use SSL, this cannot be happening!");
			case MG_HTTP_ERROR:
				return "";
			case MG_WEBSOCKET_CONNECT:
			case MG_WEBSOCKET_READY:
			case MG_WEBSOCKET_MESSAGE:
			case MG_WEBSOCKET_CLOSE:
				YELO_ASSERT_DISPLAY(false, "mongoose is attepting to use websockets, this cannot be happening!");
			case MG_OPEN_FILE:
				return nullptr;
			
			YELO_ASSERT_CASE_UNREACHABLE();
			};

			LogRequest(request_info->remote_ip, request_info->remote_port, request_info->uri, request_info->query_string);

			// if the ip is banned serve it no data
			if(BanManager::IPBanned(request_info->remote_ip))
			{
				SendResponse(connection, Enums::_http_status_code_client_error_unauthorized);
				return return_value;
			}
			else
				BanManager::AddConnection(request_info->remote_ip);

			c_url_interface url_interface;
			url_interface.ParsePath(request_info->uri);

			int index = NONE;
			// search for a matching service by comparing its root "directory"
			if(url_interface.m_path.size() >= 1)
			{
				// TODO: ranged for, break on if()
				for(int i = 0; (NONE == index) && (i < NUMBEROF(g_http_services)); i++)
				{
					if((url_interface.m_path[0].compare(g_http_services[i].m_uri_root) == 0))
						index = i;
				}
			}

			// process the request
			if(NONE != index)
			{
				g_http_services[index].Callback(callback_event, connection);
				return "";
			}
			else if(!g_http_server_globals.m_flags.serve_files)
				SendResponse(connection, Enums::_http_status_code_client_error_not_found);

			return return_value;
		}

		/*!
		 * \brief
		 * Sets the ports config string.
		 * 
		 * \param port_string
		 * The ports string to use.
		 * 
		 * Sets the ports config string. The default port (MP port + 1) will always be added to the string.
		 */
		void SetPorts(const char* port_string)
		{
			// force the server to always listen on MP port  + 1
			char port[6];
			sprintf_s(port, "%i", Networking::ConnectionPort() + 1);

			g_http_server_globals.m_config.ports.assign(port);

			// append any other ports
			if(port_string && (strlen(port_string) > 0))
			{
				g_http_server_globals.m_config.ports.append(",");
				g_http_server_globals.m_config.ports.append(port_string);
			}
		}

		/*!
		 * \brief
		 * Sets the thread count config string.
		 * 
		 * \param count
		 * The number of threads to start.
		 * 
		 * Sets the thread count config string.
		 */
		void SetThreads(uint16 count)
		{
			// check the thread count bounds
			if(count > 64) // TODO: named constant?
				blam::console_printf(false, "Max thread count is 64");
			else if(count <= 0)
				blam::console_printf(false, "Must have at least 1 or more threads");
			else
			{
				// create the thread count string
				char threads_string[4];
				if(-1 == sprintf_s(threads_string, "%i", count))
					blam::console_printf(false, "Failed to set HTTP thread count");
				else
					g_http_server_globals.m_config.threads.assign(threads_string);
			}
		}

		/*!
		 * \brief
		 * Sets the HTTP root path config string.
		 * 
		 * \param root
		 * The path to use as the web root.
		 * 
		 * Sets the HTTP root path string.
		 */
		void SetRoot(const char* root)
		{
			cstring root_string = ".";
			g_http_server_globals.m_flags.serve_files = false;
			if(root && (strlen(root) > 0))
			{
				// if the path exists use it, otherwise default to not self serving files
				if(FileIO::PathExists(root))
				{
					root_string = root;
					g_http_server_globals.m_flags.serve_files = true;
				}
				else
					blam::console_printf(false, "Supplied HTTP root path does not exist, root path reset");
			}
			g_http_server_globals.m_config.root.assign(root_string);
		}

		/*!
		 * \brief
		 * Sets the throttle config string.
		 * 
		 * \param throttle
		 * Throttle string to use.
		 * 
		 * Sets the throttle config string.
		 */
		void SetThrottle(const char* throttle)
		{
			//TODO: validation of the throttle string
			cstring throttle_string = "*=0";
			if(throttle && (strlen(throttle) > 0))
			{
				throttle_string = throttle;
			}
			g_http_server_globals.m_config.throttle.assign(throttle_string);
		}

		/*!
		 * \brief
		 * Starts the HTTP server.
		 * 
		 * Starts the HTTP server.
		 */
		void StartServer()
		{
			// TODO: should probably use an enum for indexing options
			g_http_server_options[1] = g_http_server_globals.m_config.ports.c_str();
			g_http_server_options[3] = g_http_server_globals.m_config.threads.c_str();
			g_http_server_options[5] = g_http_server_globals.m_config.root.c_str();
			g_http_server_options[7] = g_http_server_globals.m_config.throttle.c_str();

			// start the http server daemon
			g_http_server_globals.m_context = mg_start(
					ServerCallback,
					nullptr,
					g_http_server_options);

			if(!g_http_server_globals.m_context)
				blam::console_printf(false, "HTTP server failed to start");
			else
				blam::console_printf(false, "HTTP server started successfully");
		}

		/*!
		 * \brief
		 * Stops the HTTP server.
		 * 
		 * Stops the HTTP server. All services must be stopped before the HTTP server.
		 */
		void StopServer()
		{
			// check that all services are not running, return if they are
			for (auto& service : g_http_services)
			{
				if(service.ServiceStarted())
				{
					blam::console_printf(false, "All HTTP services must be stopped before stopping the HTTP server");
					return;
				}
			}

			mg_stop(g_http_server_globals.m_context);
			g_http_server_globals.m_context = nullptr;

			blam::console_printf(false, "HTTP server stopped");
		}

		/*!
		 * \brief
		 * Initializes the default server config.
		 * 
		 * Initializes the default server config.
		 */
		void Initialize()
		{
			BanManager::Initialize();

			SetPorts(nullptr);
			SetThreads(4);
			SetRoot(nullptr);
			SetThrottle(nullptr);
		}

		/*!
		 * \brief
		 * Stops the HTTP server and disposes the services.
		 * 
		 * Stops the HTTP server and disposes the services.
		 */
		void Dispose()
		{
			if(ServerStarted())
			{
				// stop the http server daemon
				mg_stop(g_http_server_globals.m_context);
				g_http_server_globals.m_context = nullptr;
			}

			BanManager::Dispose();
		}

		/*!
		 * \brief
		 * Updates the ban manager.
		 * 
		 * \param delta
		 * The time in seconds since the last update.
		 * 
		 * Updates the ban manager.
		 */
		void Update(real delta)
		{
			BanManager::UpdateConnectionBans(delta);
		}

		/*!
		 * \brief
		 * Sends a HTTP response to the client.
		 * 
		 * \param connection
		 * The connection information for the client.
		 * 
		 * \param status
		 * The status code enum to respond with.
		 * 
		 * \param header
		 * A set of optional headers to send with the response.
		 * 
		 * \param body
		 * An optional block of arbitrary data to send as the message body.
		 * 
		 * \param body_length
		 * The length of the data pointed to by 'body'.
		 * 
		 * Sends a response formatted to the HTTP standard with a status code, optional headers and an optional body, to the client.
		 */
		void SendResponse(mg_connection* connection, const Enums::http_status_code status, c_http_header* header, const char* body, const uint32 body_length)
		{
			mg_request_info* request_info = mg_get_request_info(connection);

			LogReponse(request_info->remote_ip, request_info->remote_port, status);

			std::ostringstream response("");

			// set version
			response << K_HTTP_VERSION_STR << " ";

			// set status
			response << g_status_codes[status].m_code << " " << g_status_codes[status].m_reason << "\r\n";

			// add headers
			c_http_header server_headers;
			server_headers.AddHeader("Server", K_HTTP_SERVER_HEADER);
			response << server_headers.GetHeaders();

			// add extra headers
			if(header)
				response << header->GetHeaders();

			response << "\r\n";

			// add message body
			if(body && body_length)
				response.write(body, body_length);

			mg_write(connection, response.str().c_str(), response.str().size());
		}

		/*!
		 * \brief
		 * Sets how many threads the HTTP server should start.
		 * 
		 * \param arguments
		 * Pointer to the script arguments struct.
		 * 
		 * \returns
		 * returns NULL.
		 * 
		 * Sets how many threads the HTTP server should start.
		 */
		void* HTTPServerSetThreadCount(void** arguments)
		{
			struct s_arguments {
				const short thread_count;
				PAD16;
			}* args = CAST_PTR(s_arguments*, arguments);

			if(ServerStarted())
				blam::console_printf(false, "Cannot modify HTTP config whilst the server is running");
			else
				SetThreads(args->thread_count);

			return nullptr;
		}

		/*!
		 * \brief
		 * Sets the path to use as the web root folder.
		 * 
		 * \param arguments
		 * Pointer to the script arguments struct.
		 * 
		 * \returns
		 * returns NULL.
		 * 
		 * Sets the path to use as the web root folder.
		 */
		void* HTTPServerSetRoot(void** arguments)
		{
			struct s_arguments {
				cstring root;
			}* args = CAST_PTR(s_arguments*, arguments);

			if(ServerStarted())
				blam::console_printf(false, "Cannot modify HTTP config whilst the server is running");
			else
				SetRoot(args->root);

			return nullptr;
		}

		/*!
		 * \brief
		 * Sets the amount to throttle the servers bandwidth by.
		 * 
		 * \param arguments
		 * Pointer to the script arguments struct.
		 * 
		 * \returns
		 * returns NULL.
		 * 
		 * Sets the amount to throttle the servers bandwidth by.
		 */
		void* HTTPServerSetThrottle(void** arguments)
		{
			struct s_arguments {
				cstring throttle;
			}* args = CAST_PTR(s_arguments*, arguments);

			if(ServerStarted())
				blam::console_printf(false, "Cannot modify HTTP config whilst the server is running");
			else
				SetThrottle(args->throttle);

			return nullptr;
		}

		/*!
		 * \brief
		 * Sets additional ports to listen on.
		 * 
		 * \param arguments
		 * Pointer to the script arguments struct.
		 * 
		 * \returns
		 * returns NULL.
		 * 
		 * Sets additional ports to listen on.
		 */
		void* HTTPServerSetPorts(void** arguments)
		{
			struct s_arguments {
				cstring ports;
			}* args = CAST_PTR(s_arguments*, arguments);

			if(ServerStarted())
				blam::console_printf(false, "Cannot modify HTTP config whilst the server is running");
			else
				SetPorts(args->ports);

			return nullptr;
		}

		/*!
		 * \brief
		 * Starts the HTTP server
		 * 
		 * \returns
		 * Returns NULL.
		 * 
		 * Starts the HTTP server. Services cannot be started until the HTTP server is running.
		 */
		void* HTTPServerShowConfig()
		{
			blam::console_printf(false, "HTTP Root: %s", g_http_server_globals.m_config.root.c_str());
			blam::console_printf(false, "HTTP Ports: %s", g_http_server_globals.m_config.ports.c_str());
			blam::console_printf(false, "HTTP Threads: %s", g_http_server_globals.m_config.threads.c_str());
			blam::console_printf(false, "HTTP Throttle: %s", g_http_server_globals.m_config.throttle.c_str());

			return nullptr;
		}

		/*!
		 * \brief
		 * Starts the HTTP server
		 * 
		 * \returns
		 * Returns NULL.
		 * 
		 * Starts the HTTP server. Services cannot be started until the HTTP server is running.
		 */
		void* HTTPServerStart()
		{
			if(ServerStarted())
				blam::console_printf(false, "HTTP server is already running");
			else
				StartServer();

			return nullptr;
		}

		/*!
		 * \brief
		 * Stops the HTTP server
		 * 
		 * \returns
		 * Returns NULL.
		 * 
		 * Stops the HTTP server. All running services must be stopped for the HTTP server to be stopped.
		 */
		void* HTTPServerStop()
		{
			if(!ServerStarted())
				blam::console_printf(false, "HTTP server is not running");
			else
				StopServer();

			return nullptr;
		}

		/*!
		 * \brief
		 * Toggles whether to output server events to the server log.
		 * 
		 * \param arguments
		 * Pointer to the script arguments struct.
		 * 
		 * \returns
		 * Returns NULL.
		 * 
		 * Toggles whether to output server events to the server log.
		 */
		void* HTTPServerLogEnable(void** arguments)
		{
			struct s_arguments {
				const bool enable;
				PAD24;
			}* args = CAST_PTR(s_arguments*, arguments);

			Yelo::Server::EnableEventLogging(Enums::_server_event_type_http_server) = args->enable;

			return nullptr;
		}
#endif
	};};};
};