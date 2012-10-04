/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/game/players.hpp>
#include <blamlib/Halo1/saved_games/game_variant.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_network_game_maximum_player_count = k_multiplayer_maximum_players,
		};
	};

	namespace Networking
	{
		struct s_network_game_map
		{
			int32 version;
			char name[128];
		}; BOOST_STATIC_ASSERT( sizeof(s_network_game_map) == 0x84 );

		struct s_network_game_player
		{
			wchar_t name[12];			// 0x0
			int16 primary_color_index;	// 0x18
			int16 icon_index;			// 0x1A
			sbyte machine_index;		// 0x1C
			sbyte controller_index;		// 0x1D
			// These values will be the same on the client as they are on the server, so 
			// we can use them safely for player operations in multiplayer code
			sbyte team_index;			// 0x1E
			sbyte player_list_index;	// 0x1F
		}; BOOST_STATIC_ASSERT( sizeof(s_network_game_player) == 0x20 );

		struct s_network_game
		{
			wchar_t name[64];						// 0x0
			s_network_game_map map;					// 0x80
			GameEngine::s_game_variant game_variant;// 0x104
			byte _unused_game_engine;				// 0x1DC
			byte maximum_players;					// 0x1DD
			int16 difficulty_level;					// 0x1DE
			int16 player_count;						// 0x1E0
			s_network_game_player players[Enums::k_network_game_maximum_player_count];	// 0x1E2
			PAD16;									// 0x3E2
			int32 network_game_random_seed;			// 0x3E4
			int32 number_of_games_played;			// 0x3E8
			int32 local_data;						// 0x3EC
		}; BOOST_STATIC_ASSERT( sizeof(s_network_game) == 0x3F0 );
	};
};