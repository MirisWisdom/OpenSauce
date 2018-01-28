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

#include <Networking/GameSpyApi.hpp>
#include <Memory/MemoryInterface.hpp>
#include <yelolib/memory/memory_interface_base.hpp>

namespace Yelo
{
	namespace Networking
	{
		ENGINE_DPTR(s_gamespy_socket, gs_Socket, DUO_PTR(K_GS_SOCKET));
		ENGINE_DPTR(s_gamespy_socket, gs_LoopbackSocket, DUO_PTR(K_GS_LOOPBACKSOCKET));
		ENGINE_PTR(s_gamespy_config, gamespy_config, DUO_PTR(K_GAMESPY_CONFIG));
		ENGINE_PTR(s_gamespy_globals, gamespy_globals, DUO_PTR(K_GAMESPY_GLOBALS));
		ENGINE_PTR(s_gamespy_server_browser_globals, gamespy_server_browser_globals, DUO_PTR(K_GAMESPY_SERVER_BROWSER_GLOBALS));
		ENGINE_DPTR(s_gamespy_qr2, gamespy_qr2, DUO_PTR(K_GAMESPY_QR2));
		ENGINE_PTR(bool, g_gamespy_patch_check_for_updates, K_G_GAMESPY_PATCH_CHECK_FOR_UPDATES, PTR_NULL);

		ENGINE_PTR(s_gamespy_product, gamespy_products_list, DUO_PTR(K_GAMESPY_PRODUCTS_LIST)); // [4]

		DATA_PTR(compiler_null_string, DUO_PTR(K_COMPILER_NULL_STRING)); // TODO: not the best place for this...
		FUNC_PTR(GAMESPY_GET_CLIENT_KEY_HASH, DUO_PTR(K_GAMESPY_GET_CLIENT_KEY_HASH));
		DATA_PTR(GAMESPY_PATCH_SPAWN_CHECK_FOR_UPDATES_THREAD_CALL, K_GAMESPY_PATCH_SPAWN_CHECK_FOR_UPDATES_THREAD_CALL, DATA_PTR_NULL);

		namespace GameSpy
		{
			ENGINE_PTR(cstring, Matchup1Hostname, DUO_PTR(K_MATCHUP1_HOSTNAME));
			ENGINE_PTR(cstring, Matchup2Hostname, DUO_PTR(K_MATCHUP2_HOSTNAME));
			ENGINE_PTR(cstring, MASTER_ADDR_REFERENCE, DUO_PTR(K_MASTER_ADDR_REFERENCE));
			ENGINE_PTR(cstring, MASTER_ADDR_SB_REFERENCE, DUO_PTR(K_MASTER_ADDR_SB_REFERENCE));
			ENGINE_PTR(cstring, PTA_DEFAULT_VERCHECK_URL_REFERENCE, DUO_PTR(K_PTA_DEFAULT_VERCHECK_URL_REFERENCE));

			FUNC_PTR(CREATE_GAMESPY_QR2_HOOK, DUO_PTR(K_CREATE_GAMESPY_QR2_HOOK));
			FUNC_PTR(QR2_REGISTER_KEY, DUO_PTR(K_QR2_REGISTER_KEY));

			FUNC_PTR(SBSERVER_GET_BOOL_VALUE, K_SBSERVER_GET_BOOL_VALUE, FUNC_PTR_NULL);
			FUNC_PTR(SBSERVER_GET_INT_VALUE, K_SBSERVER_GET_INT_VALUE, FUNC_PTR_NULL);
			FUNC_PTR(SBSERVER_GET_STRING_VALUE, K_SBSERVER_GET_STRING_VALUE, FUNC_PTR_NULL);
		}
	}
}
