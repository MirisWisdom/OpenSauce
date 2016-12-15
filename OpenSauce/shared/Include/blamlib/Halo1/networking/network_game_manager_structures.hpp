/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/networking/network_game_manager.hpp>
#include <blamlib/Halo1/saved_games/game_variant.hpp>

#include <YeloLib/Halo1/open_sauce/blam_memory_upgrades.hpp>

namespace Yelo
{
	namespace Networking
	{
		struct s_network_game_map
		{
			int32 version;
			char name[128];
		}; BOOST_STATIC_ASSERT( sizeof(s_network_game_map) == 0x84 );

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

		// For increased player counts game states
		struct s_network_game_yelo : s_network_game
		{
			s_network_game_player players_yelo[Enums::k_maximum_network_machine_count_upgrade];
		};
	};
};