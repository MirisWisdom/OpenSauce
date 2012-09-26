/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Networking
	{
		struct s_player_update_history
		{
			uint32 next_update_history_id; // maximum = 64
			PAD32; // void* to 0x418 structure (0x4 in that structure references the 'next', if once exists, 0x414 is another GlobalAlloc'd pointer)
			PAD32;
			uint32 number_of_playbacks;
			uint32 total_ticks;
			uint32 total_updates;
			PAD32; // 0x18
			PAD32; // 0x1C
			PAD32; // 0x20
			real avg_prediction_error;
			real avg_ticks_played_back;
		};
	};
};