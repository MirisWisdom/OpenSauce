/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Networking/HTTP/HTTPServer.hpp"

#if PLATFORM_IS_DEDI
#include "Common/YeloSettings.hpp"
#include "Common/CmdLineSettings.hpp"
#include "mongoose/mongoose.h"
#include "Networking/HTTP/HTTP.hpp"
#include "Networking/HTTP/BanManager.hpp"
#include "Networking/HTTP/MapDownloadServer.hpp"
#include "Networking/Server.hpp"
#include "Game/EngineFunctions.hpp"
#endif

namespace Yelo
{
	namespace Networking { namespace HTTP { namespace Server
	{
#if PLATFORM_IS_DEDI

	#ifdef _DEBUG
	#define HTTP_COMPONENT_SERVER(id, root, callback) { id, root, callback }
	#else
	#define HTTP_COMPONENT_SERVER(id, root, callback) { root, callback }
	#endif

#else
	#define HTTP_COMPONENT_SERVER(id, root, callback) { root }
#endif
		static s_http_service g_http_services[] = 
		{
			HTTP_COMPONENT_SERVER("MapDownloadService", "map_download", Server::MapDownload::ServerCallback),
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
			ASSERT(service < Enums::_http_service_enumeration, "Invalid HTTP service root requested");

			return g_http_services[service].m_uri_root;
		}

#if PLATFORM_IS_DEDI

#define K_HTTP_SERVER_HEADER K_HTTP_DEDI_ID "/" BOOST_STRINGIZE(K_OPENSAUCE_VERSION_BUILD_MAJ) "." BOOST_STRINGIZE(K_OPENSAUCE_VERSION_BUILD_MIN) "." BOOST_STRINGIZE(K_OPENSAUCE_VERSION_BUILD)
#define K_HTTP_VERSION_STR "HTTP/" BOOST_STRINGIZE(HTTP_VERSION_MAJOR) "." BOOST_STRINGIZE(HTTP_VERSION_MINOR)

		const char* g_http_server_options[] = 
		{
			"listening_ports", NULL,
			"num_threads", NULL,
			"document_root", NULL,
			"enable_directory_listing", "no",
			NULL
		};

		struct s_http_server_globals
		{
			struct
			{
				bool	serve_files;
				PAD24;
			}m_flags;

			mg_context* m_context;
		};
		static s_http_server_globals g_http_server_globals;

		int FindHeader(const mg_request_info* request_info, const char* name)
		{
			for(int i = 0; i < NUMBEROF(request_info->http_headers); i++)
			{
				if(!request_info->http_headers[i].name)
					break;

				if(strcmp(request_info->http_headers[i].name, name) == 0)
					return i;
			}
			return -1;
		}

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
		static void* ServerCallback(mg_event callback_event, mg_connection* connection, const mg_request_info* request_info)
		{
			// if the return value is NULL, mongoose will attempt to serve the files itself
			void* return_value = (g_http_server_globals.m_flags.serve_files ? NULL : "");

			switch(callback_event)
			{
			case MG_NEW_REQUEST:
				break;
			case MG_EVENT_LOG:
				Yelo::Server::EventLog(Enums::_server_event_type_http_server, L"MONGOOSE\t%S", request_info->log_message);
				return "";
			case MG_INIT_SSL:
				ASSERT(callback_event != MG_INIT_SSL, "mongoose is attepting to use SSL, this cannot be happening!");
			case MG_HTTP_ERROR:    // HTTP error must be returned to the client
			case MG_REQUEST_COMPLETE:  // Mongoose has finished handling the request
				return "";
			};

			LogRequest(request_info->remote_ip, request_info->remote_port, request_info->uri, request_info->query_string);

			// if the ip is banned serve it no data
			if(BanManager::IPBanned(request_info->remote_ip))
			{
				SendResponse(connection, request_info, Enums::_http_status_code_client_error_unauthorized);
				return return_value;
			}
			else
				BanManager::AddConnection(request_info->remote_ip);

			c_url_interface url_interface;
			url_interface.ParsePath(request_info->uri);

			int index = -1;
			// search for a matching service by comparing its root "directory"
			if(url_interface.m_path.size() >= 1)
			{
				for(int i = 0; (-1 == index) && (i < NUMBEROF(g_http_services)); i++)
				{
					if((url_interface.m_path[0].compare(g_http_services[i].m_uri_root) == 0))
						index = i;
				}
			}

			// process the request
			if(-1 != index)
			{
				g_http_services[index].Callback(callback_event, connection, request_info);
				return "";
			}
			else if(!g_http_server_globals.m_flags.serve_files)
				SendResponse(connection, request_info, Enums::_http_status_code_client_error_not_found);

			return return_value;
		}

		/*!
		 * \brief
		 * Starts the HTTP server.
		 * 
		 * Starts the HTTP server.
		 */
		void Initialize()
		{
			BanManager::Initialize();

			// get the http root folder if the dedi will be serving files itself
			cstring http_root = NULL;
			Engine::GetCmdLineParameter("-http_root", &http_root);
			g_http_server_globals.m_flags.serve_files = (http_root != NULL);

			// get the thread count
			uint32 thread_count = 4;
			if(CMDLINE_GET_PARAM(http_threads).ParameterSet())
			{
				thread_count = CMDLINE_GET_PARAM(http_threads).GetValue();
				thread_count = (thread_count == 0 ? 4 : thread_count);
			}

			// build the mongoose options list
			char port_string[6] = "";
			char threads_string[6] = "";

			sprintf_s(port_string, 6, "%i", Networking::ConnectionPort() + 1);
			sprintf_s(threads_string, 6, "%i", thread_count);

			g_http_server_options[1] = port_string;
			g_http_server_options[3] = threads_string;
			g_http_server_options[5] = (http_root ? http_root : ".");

			// start the http server daemon
			g_http_server_globals.m_context = mg_start(
					ServerCallback,
					NULL,
					g_http_server_options);

			if(!g_http_server_globals.m_context)
				Engine::Console::TerminalPrint("Failed to start the HTTP server");
		}

		/*!
		 * \brief
		 * Stops the HTTP server and disposes the services.
		 * 
		 * Stops the HTTP server and disposes the services.
		 */
		void Dispose()
		{
			// stop the http server daemon
			mg_stop(g_http_server_globals.m_context);
			g_http_server_globals.m_context = NULL;

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
		void SendResponse(mg_connection* connection, const mg_request_info* request_info, const Enums::http_status_code status, c_http_header* header, const char* body, const uint32 body_length)
		{
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

			return NULL;
		}
#endif
	};};};
};