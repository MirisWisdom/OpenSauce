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
		struct s_yelo_header_data
		{
			PAD8; // since the unused 32 bytes in s_header_data is more than likely a tag_string, we don't want to touch what is actually the first char (we want it to stay zero)
			struct {
				bool initialized : 1;
				bool game_state_upgrades_on : 1;
			}flags;
		}; BOOST_STATIC_ASSERT( sizeof(s_yelo_header_data) <= 0x20 );
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