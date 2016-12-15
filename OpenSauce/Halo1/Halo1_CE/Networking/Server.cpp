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

#if PLATFORM_IS_DEDI
		struct s_network_sv_logging_extension
		{
			wcstring m_log_titles[Enums::_server_event_type];

			std::array<byte, Enums::_server_event_type> m_event_enable_logging;
			std::array<byte, Enums::_server_event_type> m_event_enable_echoing;
		};
		static s_network_sv_logging_extension g_network_sv_logging_extension;

		byte& EnableEventLogging(Enums::server_event_type event_type)
		{
			YELO_ASSERT_DISPLAY(event_type < Enums::_server_event_type, "server event type is invalid");

			return g_network_sv_logging_extension.m_event_enable_logging[event_type];
		}

		byte& EnableEventEchoing(Enums::server_event_type event_type)
		{
			YELO_ASSERT_DISPLAY(event_type < Enums::_server_event_type, "server event type is invalid");

			return g_network_sv_logging_extension.m_event_enable_echoing[event_type];
		}
#endif

		void Initialize()
		{
#if PLATFORM_IS_DEDI
			EventLog = CAST_PTR(proc_sv_event_log, GET_FUNC_VPTR(SV_EVENT_LOG_WITH_TIME));
			EventEcho = CAST_PTR(proc_sv_event_log, GET_FUNC_VPTR(SV_EVENT_ECHO));

			// copy the log title pointers to the new array
			memcpy_s(g_network_sv_logging_extension.m_log_titles,
				sizeof(g_network_sv_logging_extension.m_log_titles),
				GET_PTR2(log_entry_titles),
				sizeof(wcstring) * Enums::_server_event_type_stock);

			// set the titles for any new event types
			g_network_sv_logging_extension.m_log_titles[Enums::_server_event_type_http_server] = L"HTTP_SERVER";

			// redirect the title list reference to the new array
			GET_PTR(log_entry_titles_ref) = &g_network_sv_logging_extension.m_log_titles[0];

			// no need to copy the logging/echoing arrays as they haven't been set yet

			// redirect all references to the enable logging/echoing toggle lists
			for(auto ptr : K_EVENT_ENABLE_LOGGING_0_REFS)
				*ptr = &g_network_sv_logging_extension.m_event_enable_logging[0];

			*GET_PTR(event_enable_logging_4_ref) = &g_network_sv_logging_extension.m_event_enable_logging[4];
			*GET_PTR(event_enable_logging_8_ref) = &g_network_sv_logging_extension.m_event_enable_logging[8];
			*GET_PTR(event_enable_logging_12_ref) = &g_network_sv_logging_extension.m_event_enable_logging[12];


			for(auto ptr : K_EVENT_ENABLE_ECHOING_0_REFS)
				*ptr = &g_network_sv_logging_extension.m_event_enable_echoing[0];

			*GET_PTR(event_enable_echoing_2_ref) = &g_network_sv_logging_extension.m_event_enable_echoing[2];
			*GET_PTR(event_enable_echoing_3_ref) = &g_network_sv_logging_extension.m_event_enable_echoing[3];
			*GET_PTR(event_enable_echoing_4_ref) = &g_network_sv_logging_extension.m_event_enable_echoing[4];
			*GET_PTR(event_enable_echoing_8_ref) = &g_network_sv_logging_extension.m_event_enable_echoing[8];
			*GET_PTR(event_enable_logging_12_ref) = &g_network_sv_logging_extension.m_event_enable_echoing[12];

			// set the toggles for any new logging events
			g_network_sv_logging_extension.m_event_enable_logging[Enums::_server_event_type_http_server] = 1;
			g_network_sv_logging_extension.m_event_enable_echoing[Enums::_server_event_type_http_server] = 0;
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
			if(message != nullptr)
			{
				if(mode == nullptr)		mode = L"NONE   ";
				if(source == nullptr)	source = L"NONE";

				wcstring k_log_format = L"%s\t%s\t%s";
				wcstring k_echo_format = L"[%s] %s: %s";

				EventLog(Enums::_server_event_type_chat, k_log_format, mode, source, message);
				EventLog(Enums::_server_event_type_chat, k_echo_format, mode, source, message);
			}
		}
#endif
	};
};