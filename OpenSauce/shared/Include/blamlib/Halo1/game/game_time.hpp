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
		struct s_game_time_globals;

		s_game_time_globals*		GameTimeGlobals();
	};

	namespace blam
	{
		game_ticks_t game_time_get();

		game_time_t game_time_get_elapsed();

		game_ticks_t local_time_get();

		game_time_t local_time_get_elapsed();

		bool game_in_progress();

		bool game_time_get_paused();

		void game_time_set_paused(bool paused);

		float game_time_get_speed();

		void game_time_set_speed(float speed);
	};
};