/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include "Pointers/1.10/HaloCE_110_Runtime.Interface.inl"
#include "Pointers/1.10/HaloCE_110_Runtime_Manual.Interface.inl"
#endif

#include <Interface/ControlStates.h>
#include <Interface/PositionState.h>
#include <Memory/MemoryInterface.hpp>
#include <yelolib/memory/memory_interface_base.hpp>

namespace Yelo
{
	namespace Input
	{
		FUNC_PTR(INPUT_UPDATE_HOOK, K_INPUT_UPDATE_HOOK, FUNC_PTR_NULL);

		ENGINE_PTR(ControlSettings, Settings, K_SETTINGS, PTR_NULL);
		ENGINE_PTR(ControlStates, ControlState, K_CONTROL_STATE, PTR_NULL);
		ENGINE_PTR(PositionState, MousePositionState, K_MOUSE_POSITION_STATE, PTR_NULL);
		ENGINE_PTR(byte, InputStateFlags, K_INPUT_STATE_FLAGS, PTR_NULL);
	}
}
