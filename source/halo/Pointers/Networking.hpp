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

#include <blam/networking/network_client_manager_structures.hpp>
#include <blam/networking/network_game_globals.hpp>
#include <blam/networking/network_server_manager_structures.hpp>
#include <blam/networking/player_update_client.hpp>
#include <blam/networking/player_update_server.hpp>
#include <Memory/MemoryInterface.hpp>
#include <yelo/memory/memory_interface_base.hpp>

namespace Yelo
{
	namespace Networking
	{
		ENGINE_PTR(s_update_client_globals, update_client_globals, DUO_PTR(K_UPDATE_CLIENT_GLOBALS));
		ENGINE_PTR(s_update_server_globals, update_server_globals, DUO_PTR(K_UPDATE_SERVER_GLOBALS));
		ENGINE_PTR(network_update_globals, network_update_globals, DUO_PTR(K_NETWORK_UPDATE_GLOBALS));
		ENGINE_DPTR(s_network_game_server, global_network_game_server, DUO_PTR(K_GLOBAL_NETWORK_GAME_SERVER));
		ENGINE_DPTR(s_network_game_client, global_network_game_client, DUO_PTR(K_GLOBAL_NETWORK_GAME_CLIENT));

		FUNC_PTR(NETWORK_GAME_ADD_PLAYER, DUO_PTR(K_NETWORK_GAME_ADD_PLAYER));
		FUNC_PTR(NETWORK_GAME_SERVER_ADD_PLAYER_TO_GAME__HOOK_ADD_PLAYER, DUO_PTR(K_NETWORK_GAME_SERVER_ADD_PLAYER_TO_GAME__HOOK_ADD_PLAYER));
		FUNC_PTR(NETWORK_GAME_REMOVE_PLAYER, DUO_PTR(K_NETWORK_GAME_REMOVE_PLAYER));
		// just a few bytes after this gets called is another call to ngrp, but its for the s_network_client's game member
		FUNC_PTR(CLIENT_MACHINE_CLEANUP__HOOK_REMOVE_PLAYER, DUO_PTR(K_CLIENT_MACHINE_CLEANUP__HOOK_REMOVE_PLAYER));

		FUNC_PTR(NETWORK_CONNECTION_WRITE, DUO_PTR(K_NETWORK_CONNECTION_WRITE));
		FUNC_PTR(NETWORK_GAME_SERVER_SEND_REJECTION_MESSAGE, DUO_PTR(K_NETWORK_GAME_SERVER_SEND_REJECTION_MESSAGE));
		FUNC_PTR(NETWORK_GAME_SERVER_HOLDUP_NEW_CLIENT, DUO_PTR(K_NETWORK_GAME_SERVER_HOLDUP_NEW_CLIENT));
		FUNC_PTR(NETWORK_GAME_SERVER_SEND_MESSAGE_TO_MACHINE, DUO_PTR(K_NETWORK_GAME_SERVER_SEND_MESSAGE_TO_MACHINE));
		FUNC_PTR(NETWORK_GAME_SERVER_SEND_MESSAGE_TO_ALL_MACHINES, DUO_PTR(K_NETWORK_GAME_SERVER_SEND_MESSAGE_TO_ALL_MACHINES));
		FUNC_PTR(NETWORK_GAME_SERVER_SEND_MESSAGE_TO_ALL_MACHINES_INGAME, DUO_PTR(K_NETWORK_GAME_SERVER_SEND_MESSAGE_TO_ALL_MACHINES_INGAME));
	}
}
