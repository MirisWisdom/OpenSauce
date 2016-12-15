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
		enum game_team : _enum
		{
			_game_team_none = NONE,

			_game_team_default = 0,
			_game_team_player,
			_game_team_human,
			_game_team_covenant,
			_game_team_flood,
			_game_team_sentinel,
			_game_team_unused6,
			_game_team_unused7,
			_game_team_unused8,
			_game_team_unused9,

			k_number_of_game_teams
		};
	};

	namespace GameState
	{
		struct s_game_allegiance_globals;

		s_game_allegiance_globals*	GameAllegianceGlobals();
	};

	namespace blam
	{
		extern cstring g_game_team_names[Enums::k_number_of_game_teams];

		// Checks to see if [team_to_test] is an enemy of [team]
		bool PLATFORM_API game_team_is_enemy(long_enum team, long_enum team_to_test);
	};
};