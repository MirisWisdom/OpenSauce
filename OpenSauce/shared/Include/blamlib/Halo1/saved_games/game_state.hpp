/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/cache/physical_memory_map.hpp>
#include <YeloLib/Halo1/saved_games/game_state_yelo.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_game_state_allocation_size =			0x00440000,

			// Default address of the game state in memory
			k_game_state_base_address =				k_physical_memory_base_address,
			// Max amount of memory addressable by the game state. After this comes tag memory
			k_game_state_cpu_size =					k_game_state_allocation_size,
		};
	};

	namespace GameState
	{
		struct s_header_data
		{
			uint32 allocation_crc;
			char level[256];
			tag_string version;
			int16 player_spawn_count;
			_enum game_difficulty;
			uint32 cache_crc;

			union {
				byte _unused[32];
				s_yelo_header_data yelo;
			};
		}; BOOST_STATIC_ASSERT( sizeof(s_header_data) == 0x14C );

		struct s_game_state_globals
		{
			void* base_address;
			uint32 cpu_allocation_size;
			PAD32; // unused
			uint32 crc;
			bool locked;
			bool saved;
			PAD16;
			uint32 revert_time;
			s_header_data* header;

			// etc... there are more fields to this but I could give a flying fuck less about them
		};
	};
};