/*
	Yelo: Open Sauce SDK
	Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

namespace Yelo
{
	namespace Input
	{
		struct PositionState
		{
			//PAD16;
			//bool Moving; // true during mouse movement
			//PAD8;
			PAD32;
			int32 Position[2]; // menu space coordinates (0,0) to (640,480)
		};
	}
}
#endif
