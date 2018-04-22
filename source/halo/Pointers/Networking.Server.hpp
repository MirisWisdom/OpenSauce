/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include "Pointers/1.10/HaloCE_110_Runtime.Networking.inl"
#include "Pointers/1.10/HaloCE_110_Runtime_Manual.Networking.inl"
#else
#include "Pointers/1.10/HaloCE_110_Dedi.Networking.inl"
#include "Pointers/1.10/HaloCE_110_Dedi_Manual.Networking.inl"
#endif

#include <Networking/Server.hpp>
#include <Memory/MemoryInterface.hpp>
#include <yelo/memory/memory_interface_base.hpp>

namespace Yelo
{
	namespace Networking
	{
		ENGINE_PTR(wchar_t, sv_password, DUO_PTR(K_SV_PASSWORD));
		ENGINE_PTR(char, sv_rcon_password, DUO_PTR(K_SV_RCON_PASSWORD));
		ENGINE_PTR(int32, sv_timelimit, DUO_PTR(K_SV_TIMELIMIT));
		ENGINE_PTR(int32, halo_argv_port, DUO_PTR(K_HALO_ARGV_PORT));
		ENGINE_PTR(int32, halo_argv_client_port, DUO_PTR(K_HALO_ARGV_CLIENT_PORT));
	}

	namespace Server
	{
		ENGINE_PTR(s_network_sv_file_globals, network_sv_file_globals, DUO_PTR(K_NETWORK_SV_FILE_GLOBALS));
		ENGINE_PTR(s_network_sv_mapcycle_globals, network_sv_mapcycle_globals, DUO_PTR(K_NETWORK_SV_MAPCYCLE_GLOBALS));
		ENGINE_PTR(s_network_sv_globals, network_sv_globals, DUO_PTR(K_NETWORK_SV_GLOBALS));

		#if PLATFORM_IS_DEDI
		ENGINE_PTR(int32, server_instance, PTR_NULL, K_SERVER_INSTANCE);

		FUNC_PTR(SV_EVENT_LOG_WITH_TIME, FUNC_PTR_NULL, K_SV_EVENT_LOG_WITH_TIME);
		FUNC_PTR(SV_EVENT_ECHO, FUNC_PTR_NULL, K_SV_EVENT_ECHO);

		ENGINE_PTR(bool, event_type_logging, PTR_NULL, K_EVENT_TYPE_LOGGING);
		ENGINE_PTR(bool, event_type_echoing, PTR_NULL, K_EVENT_TYPE_ECHOING);

		ENGINE_PTR(wcstring*, log_entry_titles, PTR_NULL, K_LOG_ENTRY_TITLES);
		ENGINE_PTR(wcstring*, log_entry_titles_ref, PTR_NULL, K_LOG_ENTRY_TITLES_REF);

		static byte** K_EVENT_ENABLE_LOGGING_0_REFS[] =
		{
			CAST_PTR(byte**, K_EVENT_ENABLE_LOGGING_0_REF_0),
			CAST_PTR(byte**, K_EVENT_ENABLE_LOGGING_0_REF_1)
		};

		ENGINE_PTR(byte**, event_enable_logging_4_ref, PTR_NULL, K_EVENT_ENABLE_LOGGING_4_REF);
		ENGINE_PTR(byte**, event_enable_logging_8_ref, PTR_NULL, K_EVENT_ENABLE_LOGGING_8_REF);
		ENGINE_PTR(byte**, event_enable_logging_12_ref, PTR_NULL, K_EVENT_ENABLE_LOGGING_12_REF);

		static byte** K_EVENT_ENABLE_ECHOING_0_REFS[] =
		{
			CAST_PTR(byte**, K_EVENT_ENABLE_ECHOING_0_REF_0),
			CAST_PTR(byte**, K_EVENT_ENABLE_ECHOING_0_REF_1),
			CAST_PTR(byte**, K_EVENT_ENABLE_ECHOING_0_REF_2)
		};

		ENGINE_PTR(byte**, event_enable_echoing_2_ref, PTR_NULL, K_EVENT_ENABLE_ECHOING_2_REF);
		ENGINE_PTR(byte**, event_enable_echoing_3_ref, PTR_NULL, K_EVENT_ENABLE_ECHOING_3_REF);
		ENGINE_PTR(byte**, event_enable_echoing_4_ref, PTR_NULL, K_EVENT_ENABLE_ECHOING_4_REF);
		ENGINE_PTR(byte**, event_enable_echoing_8_ref, PTR_NULL, K_EVENT_ENABLE_ECHOING_8_REF);
		ENGINE_PTR(byte**, event_enable_echoing_12_ref, PTR_NULL, K_EVENT_ENABLE_ECHOING_12_REF);
		#endif
	}
}
