/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
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
			byte _unk3[32];
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