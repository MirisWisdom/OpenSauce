/*
	Yelo: Open Sauce SDK
	Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include <blam/cseries/cseries_base.hpp>

namespace Yelo
{
	namespace Input
	{
		struct GamepadAxisState
		{
			byte Speed; // not even used
			sbyte Direction; // -16 to 16, screen coords
		};
	}
}
#endif
