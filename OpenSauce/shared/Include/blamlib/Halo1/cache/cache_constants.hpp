/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_physical_memory_base_address =		0x40000000,

			// Original allocation size
			k_physical_memory_map_allocation_size = 0x01B40000,

			// Original maximum cache size
			k_max_cache_size = 0x018000000,

			k_max_cache_vertex_y_index_buffer_size = 0x2000000,
		};
	};
};