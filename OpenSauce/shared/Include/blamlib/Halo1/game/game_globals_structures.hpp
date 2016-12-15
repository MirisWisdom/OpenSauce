/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/game/game.hpp>
#include <blamlib/Halo1/game/game_globals.hpp>

namespace Yelo
{
	namespace GameState
	{
		struct s_game_globals
		{
			bool map_loaded;
			bool active;
			bool players_are_double_speed;
			bool map_loading_in_progress;
			real map_loading_precentage;
			s_game_options options;
		}; BOOST_STATIC_ASSERT( sizeof(s_game_globals) == 0x114 );
	};
};