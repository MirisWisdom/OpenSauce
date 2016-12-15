/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

namespace Yelo
{
	namespace GameState
	{
		struct s_physical_memory_map_globals;

		s_physical_memory_map_globals* PhysicalMemoryMapGlobals();
	};

	namespace blam
	{
		void physical_memory_map_initialize();
		void physical_memory_map_dispose();

		void* physical_memory_map_get_game_state_address();
		void* physical_memory_map_get_tag_cache_address();
		void* physical_memory_map_get_sound_cache_address();
		void* physical_memory_map_get_texture_cache_address();
	};
};