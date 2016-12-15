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
		typedef bool (*t_service_started)(void);

		struct s_http_service
		{
#ifdef _DEBUG
			const char*			m_id;
#endif
			const char*			m_uri_root;

			t_service_started	ServiceStarted;
			mg_callback_t		Callback;
		};

		bool ServerStarted();

		void Initialize();
		void Dispose();

		void Update(real delta);

		int FindHeader(const mg_request_info* request_info, const char* name);

		void SendResponse(mg_connection* connection,
			const Enums::http_status_code status,
			c_http_header* header = nullptr,
			const char* body = nullptr,
			const uint32 body_length = 0);

		void* HTTPServerSetThreadCount(void** arguments);
		void* HTTPServerSetRoot(void** arguments);
		void* HTTPServerSetThrottle(void** arguments);
		void* HTTPServerSetPorts(void** arguments);
		void* HTTPServerShowConfig();
		void* HTTPServerStart();
		void* HTTPServerStop();
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