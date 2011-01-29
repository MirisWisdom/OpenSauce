/*
    Yelo: Open Sauce SDK
		Halo 1 (CE) Edition
    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "Common/Precompile.hpp"
#include "Networking/Server.hpp"

namespace Yelo
{
#define __EL_INCLUDE_ID			__EL_INCLUDE_NETWORKING
#define __EL_INCLUDE_FILE_ID	__EL_NETWORKING_SERVER
#include "Memory/_EngineLayout.inl"

	namespace Networking
	{
		wcstring ServerPassword()											PTR_IMP_GET2(sv_password);
		cstring ServerRconPassword()										PTR_IMP_GET2(sv_rcon_password);
		int32 ServerTimelimit()												PTR_IMP_GET(sv_timelimit);
		int32 ConnectionPort()												PTR_IMP_GET(halo_argv_port);
		int32 ConnectionClientPort()										PTR_IMP_GET(halo_argv_client_port);
	};

	namespace Server
	{
		s_network_sv_file_globals* NetworkSvFileGlobals()					PTR_IMP_GET2(network_sv_file_globals);
		s_network_sv_mapcycle_globals* NetworkSvMapcycleGlobals()			PTR_IMP_GET2(network_sv_mapcycle_globals);
		s_network_sv_globals* NetworkSvGlobals()							PTR_IMP_GET2(network_sv_globals);

		void Initialize()
		{
		}

		void Dispose()
		{
		}


#if PLATFORM_IS_DEDI
		int32 ServerInstance()						PTR_IMP_GET(server_instance);

		// server log rotation shit: 0x4D1AC0
		// hook somewhere around 0x4D17F4 to log that the log file is being rotated


		//////////////////////////////////////////////////////////////////////////
		// Event Type interface

		bool EventTypeLoggingStateGet(Enums::server_event_type event_type)
		{
			if( IN_RANGE_ENUM(event_type, Enums::_server_event_type) )
				return GET_PTR2(event_type_logging)[event_type];
			return false;
		}

		void EventTypeLoggingStateSet(Enums::server_event_type event_type, bool state)
		{
			if( IN_RANGE_ENUM(event_type, Enums::_server_event_type) )
				GET_PTR2(event_type_logging)[event_type] = state;
		}

		bool EventTypeEchoingStateGet(Enums::server_event_type event_type)
		{
			if( IN_RANGE_ENUM(event_type, Enums::_server_event_type) )
				return GET_PTR2(event_type_echoing)[event_type];
			return false;
		}

		void EventTypeEchoingStateSet(Enums::server_event_type event_type, bool state)
		{
			if( IN_RANGE_ENUM(event_type, Enums::_server_event_type) )
				GET_PTR2(event_type_echoing)[event_type] = state;
		}
#endif
	};
};