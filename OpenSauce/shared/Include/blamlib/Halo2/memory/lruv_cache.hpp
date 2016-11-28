/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/Halo2/memory/data.hpp>

namespace Yelo
{
	namespace Memory
	{
		struct s_lruv_cache_block
		{
			UNKNOWN_TYPE(int32); // not sure if the header is a short or a datum_index, please check km00!
			UNKNOWN_TYPE(int32);
			UNKNOWN_TYPE(int32);
			int32 next_block_index;
			int32 prev_block_index;
			int32 last_used_frame_index;
		};

		struct s_lruv_cache
		{
			tag_string name;
			void* delete_proc;
			void* locked_proc;
			void* unknown_proc;
			int32 hole_algorithm_type;
			int32 block_size;
			int32 page_size_bits;
			long_flags flags;
			int32 first_block_index;
			int32 last_block_index;
			UNKNOWN_TYPE(int32);
			UNKNOWN_TYPE(int32);
			UNKNOWN_TYPE(int32);
			UNKNOWN_TYPE(int32);
			UNKNOWN_TYPE(int32);
			UNKNOWN_TYPE(int32);
			UNKNOWN_TYPE(int32);
			UNKNOWN_TYPE(int32);
			s_data_array* blocks;
			tag signature;
			void* allocation_procs_vtable;
		};
	};
};