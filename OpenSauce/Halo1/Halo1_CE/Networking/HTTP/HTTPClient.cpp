/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Networking/HTTP/HTTPClient.hpp"

#if defined(YELO_USE_GAMESPY_OPEN)
#include <GameSpyOpen/md5.h>

#include "Networking/HTTP/HTTP.hpp"
#include "Networking/HTTP/HTTPServer.hpp"
#include "Networking/HTTP/MapDownloadClient.hpp"
#include "Networking/VersionCheck.hpp"

namespace Yelo
{
	namespace Networking { namespace HTTP { namespace Client
	{
		enum
		{
			k_max_simultaneous_requests = 8,
		};

		enum ghttp_request_type
		{
			_ghttp_request_type_get,
			_ghttp_request_type_save,
			_ghttp_request_type_stream,
			_ghttp_request_type_head,

			_ghttp_request_type
		};

		class s_http_client_request_data
		{
		public:
			struct
			{
				bool							m_in_use;
				bool							m_delete_buffer;
				bool							m_manual_think;
				PAD8;
			}m_flags;

			struct
			{
				GHTTPState						m_state;

				int								m_status_code;

				GHTTPByteCount					m_bytes_received;
				GHTTPByteCount					m_bytes_total;

				char							m_url[Enums::k_max_url_length];
			}m_progress;

			GHTTPRequest					m_request_id;
			ghttp_request_type				m_request_type;

			Enums::http_client_component	m_component_index;
			void*							m_component_data;

			void Ctor()
			{
				m_flags.m_in_use = false;
				m_flags.m_delete_buffer = true;
				m_flags.m_manual_think = false;

				m_progress.m_state = GHTTPSocketInit;

				m_progress.m_status_code = 0;

				m_progress.m_bytes_received = 0;
				m_progress.m_bytes_total = 0;

				m_progress.m_url[0] = 0;

				m_request_id = -1;
				m_request_type = _ghttp_request_type;

				m_component_index = Enums::_http_client_component;
				m_component_data = nullptr;
			}
		};

		HANDLE							g_request_index_mutex = INVALID_HANDLE_VALUE;
		std::array<s_http_client_request_data, k_max_simultaneous_requests>
										g_request_index;

		// success, buffer, buffer length, component data, returns new value for the component data
		typedef void* (*proc_http_complete_callback)(const bool, const char*, const GHTTPByteCount, void*);
		// component data, returns new value for the component data
		typedef void* (*proc_http_cancelled_callback)(void*);

		struct s_http_component
		{
#ifdef DEBUG
			const char* component_id;
#endif
			proc_initialize					Initialize;
			proc_dispose					Dispose;
			proc_update						Update;
			proc_http_complete_callback		CompletedCallback;
			proc_http_cancelled_callback	CancelledCallback;
		};

#ifdef DEBUG
#define HTTP_COMPONENT_CLIENT(id, initialize, dispose, update, comp_callback, canc_callback){ id, initialize, dispose, update, comp_callback, canc_callback }
#else
#define HTTP_COMPONENT_CLIENT(id, initialize, dispose, update, comp_callback, canc_callback){ initialize, dispose, update, comp_callback, canc_callback }
#endif
		static s_http_component g_http_components[] =
		{
#if !PLATFORM_IS_DEDI
			HTTP_COMPONENT_CLIENT("MapDownloadClient", MapDownload::Initialize, MapDownload::Dispose, MapDownload::Update, MapDownload::RequestCompleted_Callback, MapDownload::RequestCancelled_Callback),
#endif
			HTTP_COMPONENT_CLIENT("VersionChecker", nullptr, nullptr, nullptr, Networking::VersionCheck::RequestCompleted_Callback, Networking::VersionCheck::RequestCancelled_Callback),
		};BOOST_STATIC_ASSERT(NUMBEROF(g_http_components) == Enums::_http_client_component);

#define K_HTTP_CLIENT_HEADER K_HTTP_CLIENT_ID "/" BOOST_STRINGIZE(K_OPENSAUCE_VERSION_BUILD_MAJ) "." BOOST_STRINGIZE(K_OPENSAUCE_VERSION_BUILD_MIN) "." BOOST_STRINGIZE(K_OPENSAUCE_VERSION_BUILD)

		/*!
		 * \brief
		 * Increments/Decrements a time value, testing whether it has reached a certain point to create a delay.
		 * 
		 * \param counter
		 * The time value to increment/decrement.
		 * 
		 * \param delta
		 * The time to increment/decrement the variable by.
		 * 
		 * \param decrease
		 * Determines whether to increase or decrease the time variable. True for decrease, false for increase.
		 * 
		 * \param limit
		 * The upper/lower limit for the time variable.
		 * 
		 * \returns
		 * When the time variable reaches the limit value true is returned, otherwise false.
		 * 
		 * Increments/Decrements a time value, testing whether it has reached a certain point to create a delay.
		 */
		bool	ProgressDelay(real& counter, real delta, const bool decrease, const real limit)
		{
			if(decrease)
			{
				// decrease the counter
				counter -= delta;

				// if the counter is less than the limit, set it to the limit and return true
				if(counter <= limit)
				{
					counter = limit;
					return true;
				}
			}
			else
			{
				// increase the counter
				counter += delta;

				// if the counter is more than the limit, set it to the limit and return true
				if(counter >= limit)
				{
					counter = limit;
					return true;
				}
			}

			return false;
		}

		/*!
		 * \brief
		 * Initializes the GHTTP service then initialises the client http components.
		 * 
		 * Initializes the GHTTP service then initialises the client http components.
		 */
		void		Initialize()
		{
			g_request_index_mutex = CreateMutex(nullptr, false, nullptr);

			for (auto& request : g_request_index)
				request.Ctor();

			ghttpStartup();

			for (auto& component : g_http_components)
				if (component.Initialize)
					component.Initialize();
		}

		/*!
		 * \brief
		 * Disposes the client http components then cleans up the GHTTP service.
		 * 
		 * Disposes the client http components then cleans up the GHTTP service.
		 */
		void		Dispose()
		{
			for(int i = 0; i < NUMBEROF(g_http_components); i++)
				if(g_http_components[i].Dispose)
					g_http_components[i].Dispose();

			ghttpCleanup();

			CloseHandle(g_request_index_mutex);
		}

		/*!
		 * \brief
		 * Updates the client http components and updates the http requests that are in progress.
		 * 
		 * \param delta
		 * The time in seconds since the last update.
		 * 
		 * Updates the client http components and updates the http requests that are in progress.
		 */
		void		Update(real delta)
		{
			for(int i = 0; i < NUMBEROF(g_http_components); i++)
				if(g_http_components[i].Update)
					g_http_components[i].Update(delta);

			// only update requests that don't have the manual_think flag set, as they are probably being updated in a seperate thread
			WaitForSingleObject(g_request_index_mutex, INFINITE);
			for (auto& request : g_request_index)
				if (request.m_flags.m_in_use && !request.m_flags.m_manual_think)
					ghttpRequestThink(request.m_request_id);

			ReleaseMutex(g_request_index_mutex);
		}

		/*!
		 * \brief
		 * Searches for a free request slot and prepares it for a new request.
		 * 
		 * \param index
		 * The index variable to set with the slot index. If the function fails index will be -1.
		 * 
		 * \returns
		 * True if a free request slot was found and set up.
		 * 
		 * Searches for a free request slot and prepares it for a new request.
		 */
		bool		SetupRequest(int32& index)
		{
			index = NONE;

			// TODO: just range for g_request_index and use break when the if() is true
			// find a slot that is not in use
			for(int32 i = 0; (i < k_max_simultaneous_requests) && (index == NONE); i++)
				if(g_request_index[i].m_flags.m_in_use == false)
					index = i;

			// no free slot so return false
			if(index == -1)
				return false;

			// free index found so initialise it
			g_request_index[index].Ctor();

			g_request_index[index].m_flags.m_in_use = true;

			return true;
		}

		/*!
		 * \brief
		 * The callback that is called by GHTTP when a request is complete.
		 * 
		 * \param request
		 * The internal GHTTP index of the completed request.
		 * 
		 * \param result
		 * The result of the request.
		 * 
		 * \param buffer
		 * A pointer to the data returned by the request.
		 * 
		 * \param bufferLen
		 * The length of the data pointed to by "buffer".
		 * 
		 * \param headers
		 * The header string returned by the request.
		 * 
		 * \param param
		 * A pointer to used defined data attached to the request when it was made.
		 * 
		 * \returns
		 * Returns GHTTPTrue or GHTTPFalse depending on whether the buffer needs to be deleted by GHTTP.
		 * 
		 * The callback that is called by GHTTP when a request is complete.
		 */
		GHTTPBool	RequestComplete(GHTTPRequest request,
				GHTTPResult result,
				char* buffer,
				GHTTPByteCount bufferLen,
				char* headers,
				void* param)
		{
			WaitForSingleObject(g_request_index_mutex, INFINITE);
			auto request_user_data = CAST_PTR(s_http_client_request_data*, param);

			YELO_ASSERT_DISPLAY(g_http_components[request_user_data->m_component_index].CompletedCallback, "a http component has no completed callback assigned");
			YELO_ASSERT_DISPLAY(g_http_components[request_user_data->m_component_index].CancelledCallback, "a http component has no cancelled callback assigned");

			if(result == GHTTPRequestCancelled)
			{
				// call the components canceled callback, which returns the new component data value
				request_user_data->m_component_data = 
					g_http_components[request_user_data->m_component_index].CancelledCallback(request_user_data->m_component_data);
			}
			else
			{
				// call the components completed callback, which returns the new component data value
				request_user_data->m_component_data = 
					g_http_components[request_user_data->m_component_index].CompletedCallback(
						GHTTPSuccess == result,
						buffer,
						bufferLen,
						request_user_data->m_component_data);
			}

			ghttp_request_type request_type = request_user_data->m_request_type;
			bool delete_buffer = request_user_data->m_flags.m_delete_buffer;

			ReleaseMutex(g_request_index_mutex);

			// different processes need to take place for each request type
			switch(request_type)
			{
			case _ghttp_request_type_get:
				// if the buffer needs deleting (allocated by GHTTP/user allocated but ok to delete) return GHTTPTrue
				// if the buffer should be left (user allocated/static buffer) return GHTTPFalse
				if(delete_buffer)
					return GHTTPTrue;
				else
					return GHTTPFalse;
			
			YELO_ASSERT_CASE_UNREACHABLE();
			}

			return GHTTPTrue;
		}

		/*!
		 * \brief
		 * The callback called by GHTTP when a request is updated.
		 * 
		 * \param request
		 * The request that has been updated.
		 * 
		 * \param state
		 * The current connection state of the request.
		 * 
		 * \param buffer
		 * The data that has been received as part of the request so far.
		 * 
		 * \param bufferLen
		 * The length of the data pointed to by "buffer".
		 * 
		 * \param bytesReceived
		 * The amount of data received as part of the request.
		 * 
		 * \param totalSize
		 * The total amount of data that will be received for the request.
		 * 
		 * \param param
		 * A pointer to used defined data attached to the request when it was made.
		 * 
		 * The callback called by GHTTP when a request is updated.
		 */
		void		RequestProgress(
			GHTTPRequest request,
			GHTTPState state,
			const char * buffer,
			GHTTPByteCount bufferLen,
			GHTTPByteCount bytesReceived,
			GHTTPByteCount totalSize,
			void * param)
		{
			WaitForSingleObject(g_request_index_mutex, INFINITE);
			auto request_user_data = CAST_PTR(s_http_client_request_data*, param);

			// update the progress variables for the current request
			request_user_data->m_progress.m_state = state;

			// the bytes received and total size can be use to get the download percentage
			request_user_data->m_progress.m_bytes_received = bytesReceived;
			request_user_data->m_progress.m_bytes_total = totalSize;

			// the url is only a valid string when the state is GHTTPReceivingFile
			if(state != GHTTPReceivingFile)
				request_user_data->m_progress.m_url[0] = 0;
			else
			{
				// copy the url string to the request data
				// have to store a copy as due to multithreading the url pointer when used elsewhere could have a garbage string
				if(0 != strcpy_s(request_user_data->m_progress.m_url, ghttpGetURL(request)))
					request_user_data->m_progress.m_url[0] = 0;
			}

			// the status code is only a valid value when the state is more than GHTTPReceivingStatus
			if(state > GHTTPReceivingStatus)
				ghttpGetResponseStatus(request, &request_user_data->m_progress.m_status_code);
			else
				request_user_data->m_progress.m_status_code = 0;

			ReleaseMutex(g_request_index_mutex);
		}

		/*!
		 * \brief
		 * Ends a request that is in progress.
		 * 
		 * \param index
		 * The index of the request to abort. This a reference and will be set to -1 before this function returns.
		 * 
		 * Ends a request that is in progress.
		 */
		void		HTTPRequestAbort(int32& index)
		{
			WaitForSingleObject(g_request_index_mutex, INFINITE);

			YELO_ASSERT_DISPLAY(index >= 0 && index < k_max_simultaneous_requests, "http request index outside of the arrays bounds");

			// cancel the http request
			YELO_ASSERT_DISPLAY(g_request_index[index].m_request_id >= 0, "attempting to abort an invalid request");
			if(g_request_index[index].m_request_id >= 0)
			{
				ghttpCancelRequest(g_request_index[index].m_request_id);
				// TODO: this might not be a good idea; not sure but otherwise the http request stays in a cancelled state on the next request
				// and the callback doesn't get called
				ghttpRequestThink(g_request_index[index].m_request_id);
			}

			// reset the request slot to default values
			g_request_index[index].Ctor();

			index = -1;

			ReleaseMutex(g_request_index_mutex);
		}

		/*!
		 * \brief
		 * Forces the requested HTTP request to be updated, outside of the normal update loop.
		 * 
		 * \param index
		 * Index of the HTTP request to update.
		 * 
		 * Forces the requested HTTP request to be updated, outside of the normal update loop.
		 */
		void		HTTPRequestThink(int32 index)
		{
			YELO_ASSERT_DISPLAY(index >= 0 && index < k_max_simultaneous_requests, "http request index outside of the arrays bounds");
			ghttpRequestThink(g_request_index[index].m_request_id);
		}

		/*!
		 * \brief
		 * Starts a HTTP Get request.
		 * 
		 * \param URL
		 * The HTTP URL to request.
		 * 
		 * \param headers
		 * A header string to sent along with the request.
		 * 
		 * \param component_index
		 * The index of the component that is making the request. This is use to route the response to the correct component.
		 * 
		 * \param component_data
		 * Optional data that the component needs to be attached to the request.
		 * 
		 * \param buffer
		 * An optional data buffer to copy the received data in to. If NULL, GHTTP allocates memory for the data itself.
		 * 
		 * \param buffer_size
		 * The size of the data pointed to by "buffer".
		 * 
		 * \param delete_buffer
		 * When true, the data pointer to by "buffer" is automatically deleted when the request is complete.
		 * 
		 * \param blocking
		 * When true this function does not return until the request is complete.
		 * 
		 * \param manual_think
		 * When true the request is not updated in the main update loop and must be updated by the calling component.
		 * 
		 * \returns
		 * If the request fails -1 is returned. Otherwise the requests slot index is returned.
		 * 
		 * Starts a HTTP Get request.
		 */
		int32		HTTPRequestGet(const char* URL,
			const char* headers,
			Enums::http_client_component component_index,
			void* component_data,
			char* buffer,
			int32 buffer_size,
			bool delete_buffer,
			bool blocking,
			bool manual_think)
		{
			WaitForSingleObject(g_request_index_mutex, INFINITE);

			int32 request_index = NONE;
			// allocate a request slot and reset its values
			// if no request slots are available, return -1
			if(!SetupRequest(request_index))
				return NONE;

			s_http_client_request_data& request_data = g_request_index[request_index];

			// set the users options
			request_data.m_flags.m_delete_buffer = delete_buffer;
			request_data.m_flags.m_manual_think = manual_think;
			request_data.m_request_type = _ghttp_request_type_get;
			request_data.m_component_index = component_index;
			request_data.m_component_data = component_data;

			// create the standard client headers
			c_http_header client_headers;
			client_headers.AddHeader("User-Agent", K_HTTP_CLIENT_HEADER);

			std::string header_string = client_headers.GetHeaders();

			// append any extra headers
			if(headers)
				header_string.append(headers);

			// start the request
			int32 request_id = ghttpGetEx(URL,
				header_string.c_str(),
				buffer,
				buffer_size,
				nullptr,
				GHTTPFalse,
				(blocking ? GHTTPTrue : GHTTPFalse),
				RequestProgress,
				RequestComplete,
				&g_request_index[request_index]);

			request_data.m_request_id = request_id;

			ReleaseMutex(g_request_index_mutex);

			// if the request failed, free up the request slot and return -1
			if(request_id < 0)
			{
				HTTPRequestAbort(request_index);
				return NONE;
			}

			return request_index;
		}

		/*!
		 * \brief
		 * Gets the total bytes that will be received by an in-progress request.
		 * 
		 * \param request_index
		 * The index of the request to query.
		 * 
		 * \returns
		 * The number of bytes that will be received by a request. Returns -1 if the request index is invalid.
		 * 
		 * Gets the total bytes that will be received by an in-progress request.
		 */
		int QueryBytesTotal(int32 request_index)
		{
			if(request_index <= NONE || !g_request_index[request_index].m_flags.m_in_use)
				return NONE;

			return (int)g_request_index[request_index].m_progress.m_bytes_total;
		}

		/*!
		 * \brief
		 * Gets the number of bytes received so far by an in-progress request.
		 * 
		 * \param request_index
		 * The index of the request to query.
		 * 
		 * \returns
		 * The number of bytes that has been received so far by a request. Returns -1 if the request index is invalid.
		 * 
		 * Gets the number of bytes received so far by an in-progress request.
		 */
		int QueryBytesReceived(int32 request_index)
		{
			if(request_index <= NONE || !g_request_index[request_index].m_flags.m_in_use)
				return NONE;

			return (int)g_request_index[request_index].m_progress.m_bytes_received;
		}

		/*!
		 * \brief
		 * Gets the URL for the data being received by an in-progress request.
		 * 
		 * \param request_index
		 * The index of the request to query.
		 * 
		 * \returns
		 * Returns a pointer to the URL string the request is currently receiving. Returns NULL if the request index is invalid
		 * or if the request is not at a stage where it is receiving data.
		 * 
		 * Gets the URL for the data being received by an in-progress request.
		 */
		const char* QueryDownloadURL(int32 request_index)
		{
			if(request_index <= NONE || !g_request_index[request_index].m_flags.m_in_use)
				return nullptr;

			return &g_request_index[request_index].m_progress.m_url[0];
		}

		/*!
		 * \brief
		 * Gets the current download stage of an in-progress request.
		 * 
		 * \param request_index
		 * The index of the request to query.
		 * 
		 * \returns
		 * Returns the GHTTPState of the request. Returns GHTTPSocketInit if the request index is invalid, or if
		 * the request is not connected to a server yet.
		 * 
		 * Gets the current download stage of an in-progress request.
		 */
		GHTTPState QueryDownloadState(int32 request_index)
		{
			if(request_index <= NONE || !g_request_index[request_index].m_flags.m_in_use)
				return GHTTPSocketInit;

			return g_request_index[request_index].m_progress.m_state;
		}

		/*!
		 * \brief
		 * Gets the status code returned for an in-progress request, when GHTTP successfully connected to a server.
		 * 
		 * \param request_index
		 * The index of the request to query.
		 * 
		 * \returns
		 * The status code returned when the last server connection completed. Returns 0 if the request index is invalid.
		 * 
		 * Gets the status code returned for an in-progress request, when GHTTP successfully connected to a server.
		 */
		int QueryResponseStatus(int32 request_index)
		{
			if(request_index <= NONE || !g_request_index[request_index].m_flags.m_in_use)
				return 0;

			return g_request_index[request_index].m_progress.m_status_code;
		}
	};};}
};
#endif