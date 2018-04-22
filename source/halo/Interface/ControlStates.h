/*
	Yelo: Open Sauce SDK
	Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include <blam/input/input_windows.hpp>
#include <Interface/ControlTypes.hpp>
#include <Interface/GamepadAxisState.h>

namespace Yelo
{
	namespace Input
	{
		struct ControlStates
		{
			// key states (how long its been pressed until 0xFF, 0 if not pressed)
			byte Keys[Enums::k_number_of_keys];

			PAD(0, sizeof(byte) * 383);

			int32 MouseAxis[Enums::_MouseAxis];
			byte MouseButton[Enums::_MouseButton];

			PAD(1, sizeof(byte) * 4680);

			byte GamepadButton[Enums::_GamepadButton];
			GamepadAxisState GamepadAxis[Enums::_GamepadAxis];

			PAD(2, sizeof(byte) * 56);

			int32 GamepadDpad[Enums::_GamepadDpad];
		};
	}
}
#endif
