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
			typedef void** reference_t;

			tag					head;			// 'head'
			int32				size;			// size of the block, exclusive of this header, within the pool
			reference_t			reference;		// the pointer referencing the block allocation
			s_memory_pool_block*	next;		// linked list: the pool block that follows this one
			s_memory_pool_block*	prev;		// linked list: the pool block the comes before this one
			tag					tail;			// 'tail'
		};

		struct s_memory_pool
		{
			tag					signature;		// 'head'
			tag_string			name;			// debug code name for the allocation
			void*				base_address;	// next address to allocate a block at
			int32				size;			// total size of the pool
			int32				free_size;		// total size left in the pool thats not owned by anything
			s_memory_pool_block*	first;		// pointer to the first pool item
			s_memory_pool_block*	last;		// pointer to the last pool item

			inline void Initialize(cstring name, int32 pool_size);

			inline void Defragment();

			inline bool AllocateBlock(_Inout_ s_memory_pool_block::reference_t reference, int32 size);
			inline bool ReallocateBlock(_Inout_ s_memory_pool_block::reference_t reference, int32 new_size);
			inline bool FreeBlock(_Inout_ s_memory_pool_block::reference_t reference);
		};
	};

	namespace blam
	{
		// private, don't use unless you are memory_pool code
		void PLATFORM_API memory_pool_verify(const Memory::s_memory_pool* pool);

		void PLATFORM_API memory_pool_initialize(Memory::s_memory_pool* pool, cstring name, int32 pool_size);

		Memory::s_memory_pool* PLATFORM_API memory_pool_new(cstring name, int32 pool_size);

		void PLATFORM_API memory_pool_delete(Memory::s_memory_pool* pool);

		void PLATFORM_API memory_pool_defragment(Memory::s_memory_pool* pool);

		bool PLATFORM_API memory_pool_block_allocate(Memory::s_memory_pool* pool, _Inout_ s_memory_pool_block::reference_t reference, int32 size);

		bool PLATFORM_API memory_pool_block_reallocate(Memory::s_memory_pool* pool, _Inout_ s_memory_pool_block::reference_t reference, int32 new_size);

		void PLATFORM_API memory_pool_block_free(Memory::s_memory_pool* pool, _Inout_ s_memory_pool_block::reference_t reference);
	};

	namespace Memory
	{
		inline void s_memory_pool::Initialize(cstring name, int32 pool_size)
		{
			blam::memory_pool_initialize(this, name, pool_size);
		}
		inline void s_memory_pool::Defragment()
		{
			blam::memory_pool_defragment(this);
		}
		inline bool s_memory_pool::AllocateBlock(_Inout_ s_memory_pool_block::reference_t reference, int32 size)
		{
			blam::memory_pool_block_allocate(this, reference, size);
		}
		inline bool s_memory_pool::ReallocateBlock(_Inout_ s_memory_pool_block::reference_t reference, int32 new_size)
		{
			blam::memory_pool_block_reallocate(this, reference, new_size);
		}
		inline bool s_memory_pool::FreeBlock(_Inout_ s_memory_pool_block::reference_t reference)
		{
			blam::memory_pool_block_free(this, reference);
		}
	};
};