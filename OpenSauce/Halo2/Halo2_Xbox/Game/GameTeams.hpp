/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum game_team
		{
			_game_team_default,
			_game_team_player,
			_game_team_human,
			_game_team_covenant,
			_game_team_flood,
			_game_team_sentinel,
			_game_team_heretic,
			_game_team_prophet,
			_game_team_unused8,
			_game_team_unused9,
			_game_team_unused10,
			_game_team_unused11,
			_game_team_unused12,
			_game_team_unused13,
			_game_team_unused14,
			_game_team_unused15,
			_game_team,
		};
	};

	namespace GameState
	{
		struct s_game_allegiance_globals : TStructImpl(228)
		{
		};
		s_game_allegiance_globals* _GameAllegianceGlobals();
	};
};