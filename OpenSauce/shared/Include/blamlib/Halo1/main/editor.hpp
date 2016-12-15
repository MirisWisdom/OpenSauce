/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace blam
	{
		inline bool game_in_editor()
		{
#if PLATFORM_TYPE == PLATFORM_SAPIEN
			return true;
#else
			return false;
#endif
		}
	};
};