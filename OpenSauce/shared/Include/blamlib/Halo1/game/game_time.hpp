/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace GameState
	{
		struct s_game_time_globals
		{
			bool initialized;		// 0x0
			bool active;			// 0x1
			bool paused;			// 0x2
			PAD8;
			UNKNOWN_TYPE(int16);	// 0x4
			UNKNOWN_TYPE(int16);	// 0x6
			UNKNOWN_TYPE(int16);	// 0x8
			PAD16;
			int32 game_time;		// 0xC
			int32 elapsed_time;		// 0x10
			int32 server_time;		// 0x14
			real game_speed;		// 0x18
			real leftover_time_sec;	// 0x1C
		}; BOOST_STATIC_ASSERT( sizeof(s_game_time_globals) == 0x20 );
	};
};