/*
Yelo: Open Sauce SDK
Halo 1 (CE) Edition

See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/game/game_allegiance.hpp>

namespace Yelo
{
	namespace GameState
	{
		struct s_game_allegiance_globals
		{
			struct s_allegiance
			{
				Enums::game_team this_team;
				Enums::game_team other_team;
				int16 threshold;
				UNKNOWN_TYPE(int16);	// 0x6
				UNKNOWN_TYPE(bool);		// 0x8
				UNKNOWN_TYPE(bool);		// 0x9
				bool is_broken;			// 0xA
				UNKNOWN_TYPE(bool);		// 0xB
				UNKNOWN_TYPE(bool);		// 0xC
				PAD8;
				int16 current_incidents;// 0xE
				UNKNOWN_TYPE(int16);	// 0x10
			}; BOOST_STATIC_ASSERT( sizeof(s_allegiance) == 0x12 );

			int16 allegiances_count;
			s_allegiance allegiances[8];
			PAD16;
			long_flags ally_mapping_flags[ BIT_VECTOR_SIZE_IN_DWORDS(Enums::k_number_of_game_teams * Enums::k_number_of_game_teams) ]; // 0x94
			long_flags enemy_mapping_flags[ BIT_VECTOR_SIZE_IN_DWORDS(Enums::k_number_of_game_teams * Enums::k_number_of_game_teams) ]; // 0xA4
		}; BOOST_STATIC_ASSERT( sizeof(s_game_allegiance_globals) == 0xB4 );
	};
};