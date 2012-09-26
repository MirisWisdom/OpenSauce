/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Memory
	{
		struct s_memory_pool_block
		{
			// 'head'
			tag					head;
			int32				size;
			// the pointer to the beginning of this block
			void*				block_address;
			// the pool block that follows this one
			s_memory_pool_block*	next;
			// the pool block the comes before this one
			s_memory_pool_block*	prev;
			// 'tail'
			tag					tail;
		};

		struct s_memory_pool
		{
			// 'head'
			tag					signature;
			// 32 character string
			tag_string			name;
			// next address to allocate a block at
			void*				base_address;
			// total size of the pool
			int32				size;
			// total size left in the pool thats not owned by anything
			int32				free_size;
			// pointer to the first pool item
			s_memory_pool_block*	first;
			// pointer to the last pool item
			s_memory_pool_block*	last;
		};
	};
};