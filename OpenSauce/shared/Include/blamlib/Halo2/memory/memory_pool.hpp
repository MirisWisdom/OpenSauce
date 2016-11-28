/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Memory
	{
		struct s_memory_pool_block_header {
			tag signature;
			cstring file;
			uint32 line;
			uint32 time_code;
		}; BOOST_STATIC_ASSERT( sizeof(s_memory_pool_block_header) == 0x10 );

		struct s_memory_pool_block {
			uint32 size;					// Total size of this entire block plus sizeof(memory_pool_block)
			void** address;					// Pointer to a reference that interfaces with this block in the pool (see: object header)
			s_memory_pool_block* next_block;
			s_memory_pool_block* prev_block;

			// Total size of the block data
			inline uint32 DataSize() { return size - sizeof(s_memory_pool_block); }
		}; BOOST_STATIC_ASSERT( sizeof(s_memory_pool_block) == 0x10 );

		struct s_memory_pool {
			tag signature;
			tag_string name;
			void** allocator;
			void* next_address;
			uint32 size;
			uint32 free_space;
			s_memory_pool_block* first_block;
			s_memory_pool_block* last_block;
			bool dont_compact_blocks;
			bool allow_block_allocations_anywhere;
			PAD8;
			bool uses_debug_header;
		}; BOOST_STATIC_ASSERT( sizeof(s_memory_pool) == 0x40 );
	};
};