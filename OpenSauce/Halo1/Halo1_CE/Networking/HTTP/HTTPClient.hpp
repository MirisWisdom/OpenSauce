/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if defined(YELO_USE_GAMESPY_OPEN)

namespace Yelo
{
	namespace Enums
	{
		enum http_client_component
		{
#if !PLATFORM_IS_DEDI
			_http_client_component_map_download,
#endif
			_http_client_component_version_checker,

			_http_client_component,
		};
	};

	namespace Networking { namespace HTTP { namespace Client
	{
		void Initialize();
		void Dispose();

		void Update(real delta);

		bool ProgressDelay(real& counter, real delta, const bool decrease, const real limit);

		int32 HTTPRequestGet(const char* URL,
			const char* headers,
			Enums::http_client_component component_index,
			void* component_data,
			char* file_buffer,
			int32 file_buffer_size,
			bool delete_buffer,
			bool blocking,
			bool manual_think);
		void HTTPRequestAbort(int32& index);
		void HTTPRequestThink(int32 index);

		int QueryBytesTotal(int32 request_index);
		int QueryBytesReceived(int32 request_index);
		const char* QueryDownloadURL(int32 request_index);
		GHTTPState QueryDownloadState(int32 request_index);
		int QueryResponseStatus(int32 request_index);
	};};}
};
#endif