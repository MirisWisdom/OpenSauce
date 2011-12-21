/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
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
#if PLATFORM_IS_DEDI
			EventLog = CAST_PTR(proc_sv_event_log, GET_FUNC_VPTR(SV_EVENT_LOG_WITH_TIME));
			EventEcho = CAST_PTR(proc_sv_event_log, GET_FUNC_VPTR(SV_EVENT_ECHO));
#endif
		}

		void Dispose()
		{
		}


#if PLATFORM_IS_DEDI
		int32 ServerInstance()						PTR_IMP_GET(server_instance);

		// server log rotation shit: 0x4D1AC0
		// hook somewhere around 0x4D17F4 to log that the log file is being rotated

		proc_sv_event_log EventLog;
		proc_sv_event_log EventEcho;

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

		//////////////////////////////////////////////////////////////////////////

		void EventChatLog(wcstring mode, wcstring source, wcstring message)
		{
			if(message != NULL)
			{
				if(mode == NULL)	mode = L"NONE   ";
				if(source == NULL)	source = L"NONE";

				wcstring k_log_format = L"%s\t%s\t%s";
				wcstring k_echo_format = L"[%s] %s: %s";

				EventLog(Enums::_server_event_type_chat, k_log_format, mode, source, message);
				EventLog(Enums::_server_event_type_chat, k_echo_format, mode, source, message);
			}
		}
#endif
	};
};