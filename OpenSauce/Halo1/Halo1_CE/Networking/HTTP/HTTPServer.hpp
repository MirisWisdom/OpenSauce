/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if PLATFORM_IS_DEDI
#include "mongoose/mongoose.h"
#include "Networking/HTTP/HTTP.hpp"
#endif

namespace Yelo
{
	namespace Enums
	{
		enum http_service_enumeration
		{
			_http_service_enumeration_map_download,

			_http_service_enumeration
		};
	};

	namespace Networking { namespace HTTP { namespace Server
	{
#if PLATFORM_IS_DEDI

		struct s_http_service
		{
#ifdef _DEBUG
			const char*		m_id;
#endif
			const char*		m_uri_root;

			mg_callback_t	Callback;
		};

		void Initialize();
		void Dispose();

		void Update(real delta);

		int FindHeader(const mg_request_info* request_info, const char* name);

		void SendResponse(mg_connection* connection,
			const mg_request_info* request_info,
			const Enums::http_status_code status,
			c_http_header* header = NULL,
			const char* body = NULL,
			const uint32 body_length = 0);

		void* HTTPServerLogEnable(void** arguments);
#else
		struct s_http_service
		{
			const char*		m_uri_root;
		};
#endif

		const char* GetServiceURIRoot(Enums::http_service_enumeration service);
	};};};
};