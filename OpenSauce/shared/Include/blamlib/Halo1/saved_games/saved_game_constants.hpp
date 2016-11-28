/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_game_state_allocation_size =			0x00440000,

			// Max amount of memory addressable by the game state. After this comes tag memory
			k_game_state_cpu_size =					k_game_state_allocation_size,
		};
	};
};