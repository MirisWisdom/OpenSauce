/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if PLATFORM_IS_DEDI
#include "mongoose/mongoose.h"

namespace Yelo
{
	namespace Networking { namespace HTTP { namespace Server { namespace MapDownload
	{
		bool ServiceStarted();
		void* ServerCallback(mg_event callback_event, mg_connection* connection);

		void Initialize();
		void Dispose();

		void* MapDownloadStartServer();
		void* MapDownloadStopServer();

		void* MapDownloadSetHost(void** arguments);
		void* MapDownloadSetPartDefinitionsPath(void** arguments);
		void* MapDownloadReloadMapPartDefinitions();
	};};};};
};
#endif