/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blam/cseries/cseries_base.hpp>

namespace Yelo
{
	namespace e_game_team
	{
		typedef enum : _enum
		{
			none = NONE,

			default_team = 0,
			player,
			human,
			covenant,
			flood,
			sentinel,
			unused6,
			unused7,
			unused8,
			unused9,

			k_count
		} type_t;
	}

	namespace GameState
	{
		struct s_game_allegiance_globals;

		s_game_allegiance_globals*	GameAllegianceGlobals();
	};

	namespace blam
	{
		extern cstring g_game_team_names[e_game_team::k_count];

		// Checks to see if [team_to_test] is an enemy of [team]
		bool PLATFORM_API game_team_is_enemy(long_enum team, long_enum team_to_test);
	};
};