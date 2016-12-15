/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/saved_games/game_state.hpp>
#include <blamlib/Halo1/saved_games/saved_game_constants.hpp>
#include <YeloLib/Halo1/saved_games/game_state_yelo.hpp>

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

			union {
				byte _unused[32];
				s_yelo_header_data yelo;
			};
		}; BOOST_STATIC_ASSERT( sizeof(s_header_data) == 0x14C );

		struct s_game_state_globals
		{
			void* base_address;
			uint32 cpu_allocation_size;
			uint32 gpu_allocation_size; // unused
			uint32 crc;
			bool locked;
			bool saved;
			PAD16;
			uint32 revert_time;
			s_header_data* header;
			HANDLE autosave_thread;
			bool buffer_allocated; PAD24;
			void* buffer;
			uint32 buffer_size;
			bool file_open;
			bool file_valid_for_read;
			PAD16;
			HANDLE file_handle;
			char file_path[256];
			char core_path[256];
			HANDLE autosave_event;
			bool autosave_finished;
			bool creating_autosave;
			PAD16;
		}; BOOST_STATIC_ASSERT( sizeof(s_game_state_globals) == 0x23C );
	};
};