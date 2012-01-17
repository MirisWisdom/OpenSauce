/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Networking/Networking.hpp"

#include "Memory/MemoryInterface.hpp"
#include "Networking/MessageDeltas.hpp"

namespace Yelo
{
	namespace Networking
	{
#define __EL_INCLUDE_ID			__EL_INCLUDE_NETWORKING
#define __EL_INCLUDE_FILE_ID	__EL_NETWORKING_NETWORKING
#include "Memory/_EngineLayout.inl"

#ifdef API_DEBUG
		cstring message_packet_to_string_table[] = {
			"client-broadcast_game_search",
			"client-ping",
			"server-game_advertise",
			"server-pong",
			"server-new_client_challenge",
			"server-machine_accepted",
			"server-machine_rejected",
			"server-game_is_ending_holdup",
			"server-game_settings_update",
			"server-pregame_countdown",
			"server-begin_game",
			"server-graceful_game_exit_pregame",
			"server-pregame_keep_alive",
			"server-postgame_keep_alive",
			"client-join_game_request",
			"client-add_player_request_pregame",
			"client-remove_player_request_pregame",
			"client-settings_request",
			"client-player_settings_request",
			"client-game_start_request",
			"client-graceful_game_exit_pregame",
			"client-map_is_precached_pregame",
			"server-game_update",
			"server-add_player_ingame",
			"server-remove_player_ingame",
			"server-game_over",
			"client-loaded",
			"client-game_update",
			"client-add_player_request_ingame",
			"client-remove_player_request_ingame",
			"client-graceful_game_exit_ingame",
			"client-host_crashed_cry_for_help",
			"client-join_new_host",
			"server-reconnect",
			"server-graceful_game_exit",
			"client-remove_player_request_postgame",
			"client-switch_to_pregame",
			"client-graceful_game_exit_postgame",
		};
#endif

		s_update_client_globals* UpdateClientGlobals()						PTR_IMP_GET2(update_client_globals);
		s_update_server_globals* UpdateServerGlobals()						PTR_IMP_GET2(update_server_globals);

		network_update_globals* UpdateSettings()							PTR_IMP_GET2(network_update_globals);

		Enums::game_connection GameConnection()								PTR_IMP_GET(global_game_connection);
		bool IsLocal()	{ return GET_PTR(global_game_connection) == Enums::_game_connection_local; }
		bool IsServer() { return GET_PTR(global_game_connection) == Enums::_game_connection_network_server; }
		bool IsClient() { return GET_PTR(global_game_connection) == Enums::_game_connection_network_client; }

		s_network_game_server* NetworkGameServer()							DPTR_IMP_GET(global_network_game_server);
		s_network_game_client* NetworkGameClient()							DPTR_IMP_GET(global_network_game_client);

		//////////////////////////////////////////////////////////////////////////
		// Player Hooks (join/leave)
		static void OnPlayerJoin(s_network_player* player)
		{
		}

		static void OnPlayerExit(s_network_player* player)
		{
		}

		#pragma region OnPlayerJoinDelegate
		API_FUNC_NAKED static void PLATFORM_API OnPlayerJoinDelegate(s_network_game* network_game_data)
		{
			static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(NETWORK_GAME_ADD_PLAYER);

			__asm {
				// eax = struct s_network_player

				push	ebp
				mov		ebp, esp
				push	esi
				mov		esi, eax

				push	eax		// s_network_player.player_list_index
				call	OnPlayerJoin

				mov		eax, esi
				mov		esi, network_game_data
				push	esi
				call	TEMP_CALL_ADDR
				add		esp, 4

				pop		esi
				mov		esp, ebp
				pop		ebp
				retn
			}
		}
		#pragma endregion

		#pragma region OnPlayerExitDelegate
		API_FUNC_NAKED static void PLATFORM_API OnPlayerExitDelegate()
		{
			static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(NETWORK_GAME_REMOVE_PLAYER);

			__asm {
				// esi = s_network_game

				push	esi
				push	edi		// save edi just in case the compiler does some nasty stuff
				push	edi		// s_network_player
				call	OnPlayerExit
				pop		edi
				pop		esi

				call	TEMP_CALL_ADDR

				retn
			}
		}
		#pragma endregion
		//////////////////////////////////////////////////////////////////////////

		void Initialize()
		{
			MessageDeltas::Initialize();

			// NOTE: Uncomment these if you wish to detect
			// when players enter and leave
			//Memory::WriteRelativeCall(OnPlayerJoinDelegate, 
			//	GET_FUNC_VPTR(NETWORK_GAME_SERVER_ADD_PLAYER_TO_GAME__HOOK_ADD_PLAYER));
			//Memory::WriteRelativeCall(OnPlayerExitDelegate, 
			//	GET_FUNC_VPTR(CLIENT_MACHINE_CLEANUP__HOOK_REMOVE_PLAYER));
		}

		void Dispose()
		{
			MessageDeltas::Dispose();
		}
	};
};