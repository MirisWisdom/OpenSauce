/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/game/game_configuration.hpp>
#include <blamlib/Halo1/game/players.hpp>

namespace Yelo
{
	namespace GameEngine
	{
		struct s_oddball_globals
		{
			int32 score_to_win;
			int32 team_score[Enums::k_multiplayer_maximum_players];
			int32 individual_score[Enums::k_multiplayer_maximum_players];
			int32 undocumented1[Enums::k_multiplayer_maximum_players];
			datum_index current_ball_owners[Enums::k_multiplayer_maximum_players]; // player_index
			int32 current_ball_held_times[Enums::k_multiplayer_maximum_players]; // game-ticks
		}; //BOOST_STATIC_ASSERT( sizeof(s_slayer_globals) == 0x );
	};
};