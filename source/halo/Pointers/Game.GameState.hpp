/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include "Pointers/1.10/HaloCE_110_Runtime.Game.inl"
#include "Pointers/1.10/HaloCE_110_Runtime_Manual.Game.inl"
#else
#include "Pointers/1.10/HaloCE_110_Dedi.Game.inl"
#include "Pointers/1.10/HaloCE_110_Dedi_Manual.Game.inl"
#endif

#include <blam/cache/physical_memory_map_structures.hpp>
#include <blam/game/game_allegiance_structures.hpp>
#include <blam/game/game_globals_definitions.hpp>
#include <blam/game/game_globals_structures.hpp>
#include <blam/game/game_time_structures.hpp>
#include <blam/main/main_structures.hpp>
#include <blam/physics/physics.hpp>
#include <blam/saved_games/game_state_structures.hpp>
#include <Memory/MemoryInterface.hpp>
#include <yelo/memory/memory_interface_base.hpp>

namespace Yelo
{
	namespace GameState
	{
		static uintptr_t K_GAME_STATE_BEFORE_SAVE_PROCS_CALLS[] = {
			K_GAME_STATE_BEFORE_SAVE_PROCS_CALL_0,
			K_GAME_STATE_BEFORE_SAVE_PROCS_CALL_1,
		};
		static uintptr_t K_GAME_STATE_BEFORE_LOAD_PROCS_CALLS[] = {
			K_GAME_STATE_BEFORE_LOAD_PROCS_CALL_0,
			K_GAME_STATE_BEFORE_LOAD_PROCS_CALL_1,
			K_GAME_STATE_BEFORE_LOAD_PROCS_CALL_2,
		};
		static uintptr_t K_GAME_STATE_AFTER_LOAD_PROCS_REFS[] = {
			K_GAME_STATE_AFTER_LOAD_PROCS_REF_0,
			K_GAME_STATE_AFTER_LOAD_PROCS_REF_1,
			K_GAME_STATE_AFTER_LOAD_PROCS_REF_2,
		};
		static uintptr_t K_GAME_STATE_AFTER_LOAD_PROCS_COUNTS[] = {
			K_GAME_STATE_AFTER_LOAD_PROCS_COUNT_0,
			K_GAME_STATE_AFTER_LOAD_PROCS_COUNT_1,
			K_GAME_STATE_AFTER_LOAD_PROCS_COUNT_2,
		};

		ENGINE_PTR(s_main_globals, main_globals, DUO_PTR(K_MAIN_GLOBALS));

		ENGINE_PTR(s_physical_memory_map_globals, physical_memory_globals, DUO_PTR(K_PHYSICAL_MEMORY_GLOBALS));
		FUNC_PTR(PHYSICAL_MEMORY_INITIALIZE, DUO_PTR(K_PHYSICAL_MEMORY_INITIALIZE));
		DATA_PTR(PHYSICAL_MEMORY_ALLOCATION_SIZE, DUO_PTR(K_PHYSICAL_MEMORY_ALLOCATION_SIZE));

		ENGINE_PTR(s_game_state_globals, game_state_globals, DUO_PTR(K_GAME_STATE_GLOBALS));
		ENGINE_DPTR(s_game_globals, game_globals, DUO_PTR(K_GAME_GLOBALS));
		ENGINE_DPTR(s_game_time_globals, game_time_globals, DUO_PTR(K_GAME_TIME_GLOBALS));
		ENGINE_DPTR(s_game_allegiance_globals, game_allegiance_globals, DUO_PTR(K_GAME_ALLEGIANCE_GLOBALS));

		ENGINE_PTR(TagGroups::s_game_globals*, global_game_globals, DUO_PTR(K_GLOBAL_GAME_GLOBALS));
		ENGINE_PTR(s_physics_globals, global_physics, DUO_PTR(K_GLOBAL_PHYSICS));
		ENGINE_PTR(s_point_physics_globals, global_point_physics, DUO_PTR(K_GLOBAL_POINT_PHYSICS));

		ENGINE_PTR(byte, developer_mode, DUO_PTR(K_DEVELOPER_MODE));
		ENGINE_PTR(bool, transport_dumping, DUO_PTR(K_TRANSPORT_DUMPING));
		ENGINE_PTR(char, init_txt_filename, K_INIT_TXT_FILENAME, PTR_NULL);
		ENGINE_PTR(bool, devmode_enabled, DUO_PTR(K_DEVMODE_ENABLED));

		FUNC_PTR(GAME_INITIALIZE_HOOK, DUO_PTR(K_GAME_INITIALIZE_HOOK));
		FUNC_PTR(GAME_INITIALIZE_FOR_NEW_MAP_HOOK, DUO_PTR(K_GAME_INITIALIZE_FOR_NEW_MAP_HOOK));
		FUNC_PTR(GAME_DISPOSE_FROM_OLD_MAP_HOOK, DUO_PTR(K_GAME_DISPOSE_FROM_OLD_MAP_HOOK));
		static void** K_GAME_SCENARIO_SWITCH_BSP_CALLS[] =
		{
			CAST_PTR(void**, K_GAME_SCENARIO_SWITCH_BSP_CALL_0),
			CAST_PTR(void**, K_GAME_SCENARIO_SWITCH_BSP_CALL_1),
			CAST_PTR(void**, K_GAME_SCENARIO_SWITCH_BSP_CALL_2),
			CAST_PTR(void**, K_GAME_SCENARIO_SWITCH_BSP_CALL_3),
			CAST_PTR(void**, K_GAME_SCENARIO_SWITCH_BSP_CALL_4),
		};
		FUNC_PTR(GAME_DISPOSE_FROM_OLD_BSP_HOOK, DUO_PTR(K_GAME_DISPOSE_FROM_OLD_BSP_HOOK));

		FUNC_PTR(GAME_SERVER_QR2_STRING_MATCHES_GAMEVER_CALL, K_GAME_SERVER_QR2_STRING_MATCHES_GAMEVER_CALL, FUNC_PTR_NULL);

		FUNC_PTR(GAME_CREATE_NETWORK_SERVER, DUO_PTR(K_GAME_CREATE_NETWORK_SERVER));
		FUNC_PTR(GAME_CREATE_NETWORK_CLIENT, DUO_PTR(K_GAME_CREATE_NETWORK_CLIENT));
		FUNC_PTR(GAME_CREATE_NETWORK_SERVER_CALL, K_GAME_CREATE_NETWORK_SERVER_CALL, FUNC_PTR_NULL);

		#if PLATFORM_IS_USER
		static void** K_GAME_CREATE_NETWORK_CLIENT_CALL[] =
		{
			CAST_PTR(void**, K_GAME_CREATE_NETWORK_CLIENT_CALL_0),
			CAST_PTR(void**, K_GAME_CREATE_NETWORK_CLIENT_CALL_1),
			CAST_PTR(void**, K_GAME_CREATE_NETWORK_CLIENT_CALL_2),
			CAST_PTR(void**, K_GAME_CREATE_NETWORK_CLIENT_CALL_3),
			CAST_PTR(void**, K_GAME_CREATE_NETWORK_CLIENT_CALL_4),
			CAST_PTR(void**, K_GAME_CREATE_NETWORK_CLIENT_CALL_5),
			CAST_PTR(void**, K_GAME_CREATE_NETWORK_CLIENT_CALL_6),
		};
		#endif
	}
}
