/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/Halo1/saved_games/saved_game_constants.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum {
			//////////////////////////////////////////////////////////////////////////
			// physical_memory_map

			k_physical_memory_base_address =		0x40000000,

			// Original allocation size
			k_physical_memory_map_allocation_size = 0x01B40000,

			// Default address of the game state in memory
			k_game_state_base_address = k_physical_memory_base_address,
			// Default address of the tag cache in memory (comes right after the game state memory by default)
			k_tag_cache_base_address = k_game_state_base_address + k_game_state_allocation_size,

			//////////////////////////////////////////////////////////////////////////
			// resource caches

			k_maximum_number_of_cached_sounds = 512,
			k_maximum_number_of_cached_textures = 4096,

			//////////////////////////////////////////////////////////////////////////
			// cache files

			// Original maximum cache size
			k_max_cache_size = 0x018000000,

			k_max_cache_vertex_y_index_buffer_size = 0x2000000,

			k_number_of_cached_map_files = 6,
			k_number_of_cache_read_buffers = 8, // NUMBER_OF_READ_BUFFERS
			k_number_of_cache_write_buffers = 1, // NUMBER_OF_WRITE_BUFFERS
			k_number_of_cache_overlapped_structures = 11, // NUMBER_OF_OVERLAPPED_STRUCTURES
			k_maximum_simultaneous_cache_requests = 0x200,
		};
	};
};