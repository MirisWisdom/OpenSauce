/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include <blam/blam.h>
#include <blam/cache/physical_memory_map.hpp>

#include <blam/cache/physical_memory_map_structures.hpp>

namespace Yelo
{
	namespace blam
	{
		void* physical_memory_map_get_game_state_address()
		{
			return GameState::PhysicalMemoryMapGlobals()->game_state_base_address;
		}
		void* physical_memory_map_get_tag_cache_address()
		{
			return GameState::PhysicalMemoryMapGlobals()->tag_cache_base_address;
		}
		void* physical_memory_map_get_sound_cache_address()
		{
			return GameState::PhysicalMemoryMapGlobals()->sound_cache_base_address;
		}
		void* physical_memory_map_get_texture_cache_address()
		{
			return GameState::PhysicalMemoryMapGlobals()->texture_cache_base_address;
		}
	}
}