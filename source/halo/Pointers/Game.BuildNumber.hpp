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

#include <yelo/cseries/MacrosCpp.hpp>
#include <Game/GameBuildNumber.hpp>
#include <Memory/MemoryInterface.hpp>
#include <yelo/memory/memory_interface_base.hpp>

namespace Yelo
{
	namespace BuildNumber
	{
		ENGINE_PTR(char, game_build_version, DUO_PTR(K_GAME_BUILD_VERSION));
		ENGINE_PTR(char, game_build_version_gamespy, DUO_PTR(K_GAME_BUILD_VERSION_GAMESPY));

		ENGINE_PTR(Enums::network_game_protocol_id, network_version_id1, DUO_PTR(K_NETWORK_VERSION_ID1));
		ENGINE_PTR(Enums::network_game_protocol_id, network_version_id2, DUO_PTR(K_NETWORK_VERSION_ID2));
		ENGINE_PTR(Enums::network_game_protocol_id, network_version_id3, DUO_PTR(K_NETWORK_VERSION_ID3));

		FUNC_PTR(GAME_STATE_HEADER_IS_VALID_HOOK, DUO_PTR(K_GAME_STATE_HEADER_IS_VALID_HOOK));
		FUNC_PTR(GAME_STATE_HEADER_IS_VALID_HOOK_RET, DUO_PTR(K_GAME_STATE_HEADER_IS_VALID_HOOK_RET));
		FUNC_PTR(GAME_STATE_HEADER_TRY_AND_LOAD_HOOK, DUO_PTR(K_GAME_STATE_HEADER_TRY_AND_LOAD_HOOK));
		FUNC_PTR(GAME_STATE_HEADER_TRY_AND_LOAD_HOOK_RET_TRUE, DUO_PTR(K_GAME_STATE_HEADER_TRY_AND_LOAD_HOOK_RET_TRUE));
		FUNC_PTR(GAME_STATE_HEADER_TRY_AND_LOAD_HOOK_RET_FALSE, DUO_PTR(K_GAME_STATE_HEADER_TRY_AND_LOAD_HOOK_RET_FALSE));
	}
}
