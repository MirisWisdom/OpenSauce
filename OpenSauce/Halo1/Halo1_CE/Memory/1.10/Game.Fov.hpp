/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include "Memory/1.10/Pointers/HaloCE_110_Runtime.Game.inl"
#include "Memory/1.10/Pointers/HaloCE_110_Runtime_Manual.Game.inl"
#else
#include "Memory/1.10/Pointers/HaloCE_110_Dedi.Game.inl"
#include "Memory/1.10/Pointers/HaloCE_110_Dedi_Manual.Game.inl"
#endif

#include <YeloLib/memory/memory_interface_base.hpp>
#include <Memory/MemoryInterface.hpp>

namespace Yelo
{
	namespace Fov
	{
		FUNC_PTR(OBSERVER_UPDATE_COMMAND_HOOK, K_OBSERVER_UPDATE_COMMAND_HOOK, FUNC_PTR_NULL);

		#pragma region OBSERVER_UPDATE_POSITIONS
		FUNC_PTR(OBSERVER_UPDATE_POSITIONS, K_OBSERVER_UPDATE_POSITIONS, FUNC_PTR_NULL);
		ENGINE_PTR(bool, OBSERVER_UPDATE_POSITIONS_no_scope_blur, K_OBSERVER_UPDATE_POSITIONS_NO_SCOPE_BLUR, PTR_NULL);
		// Address of the call opcodes to [OBSERVER_UPDATE_POSITIONS] inside the function
		// known as [OBSERVER_TICK]
		FUNC_PTR(OBSERVER_TICK_CALL_HOOK_OBSERVER_UPDATE_POSITIONS, K_OBSERVER_TICK_CALL_HOOK_OBSERVER_UPDATE_POSITIONS, FUNC_PTR_NULL);
		#pragma endregion

		ENGINE_PTR(byte, MAX_FOV_CHECK_JMP, K_MAX_FOV_CHECK_JMP, PTR_NULL);
	}
}
