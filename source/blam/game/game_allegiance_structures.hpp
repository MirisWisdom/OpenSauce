/*
Yelo: Open Sauce SDK
Halo 1 (CE) Edition

See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blam/game/game_allegiance.hpp>

namespace Yelo
{
	namespace GameState
	{
		struct s_game_allegiance_globals
		{
			struct s_allegiance
			{
				e_game_team::type_t this_team;
				e_game_team::type_t other_team;
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
			}; ASSERT_SIZE(s_allegiance, 0x12);

			int16 allegiances_count;
			s_allegiance allegiances[8];
			PAD16;
			long_flags ally_mapping_flags[ BIT_VECTOR_SIZE_IN_DWORDS(e_game_team::k_count * e_game_team::k_count) ]; // 0x94
			long_flags enemy_mapping_flags[ BIT_VECTOR_SIZE_IN_DWORDS(e_game_team::k_count * e_game_team::k_count) ]; // 0xA4
		}; ASSERT_SIZE(s_game_allegiance_globals, 0xB4);
	};
};