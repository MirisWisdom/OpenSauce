/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_maximum_number_of_local_players = 
#if PLATFORM_TARGET == PLATFORM_TARGET_PC
				1,
#else // assume console
				4,
#endif

			k_multiplayer_maximum_players = 
#if PLATFORM_IS_STUBBS
				1,
#else
				16,
#endif

			// for team_data (which has a maximum of 16 datums) and such.
			// This is 1 in Stubbs, I'm positive team_data uses k_multiplayer_maximum_players. 
			k_multiplayer_maximum_teams = k_multiplayer_maximum_players,
		};
	};

	/// <summary>	The integral type used to represent game ticks. </summary>
	typedef int32 game_ticks_t;
	/// <summary>	The integral type used to represent (relative) game timers. </summary>
	/// <remarks>	Steps are in ticks. Generally used for countdowns </remarks>
	typedef int16 game_time_t;
};