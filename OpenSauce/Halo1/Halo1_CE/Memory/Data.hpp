/*
    Yelo: Open Sauce SDK
		Halo 1 (CE) Edition
    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include <Memory/DataShared.hpp>
#include <Memory/Halo1/DataShared.hpp>

namespace Yelo
{
	namespace Memory
	{
		struct s_simple_circular_queue
		{
			int32 max_queue_size;	// 0x0
			uint32 entry_size;		// 0x4
			void** user_data;		// 0x8, references to user_data structures
			int32 start;			// 0xC
			int32 end;				// 0x10
		}; BOOST_STATIC_ASSERT( sizeof(s_simple_circular_queue) == 0x14 );


		template<typename DatumT> struct DynamicArray
		{
		private:
			size_t element_size;
			int32 count;
			DatumT* elements;

		public:
			int32 GetCount() const { return count; }

			API_INLINE DatumT* operator [](int32 index)
			{
				if(index < 0 || index >= count) return NULL;

				return &CAST_PTR(DatumT*, elements)[index];
			}

			API_INLINE const DatumT* operator [](int32 index) const
			{
				if(index < 0 || index >= count) return NULL;

				return &CAST_PTR(DatumT*, elements)[index];
			}
		};

		// I've only seen gbx code use this structure format...
		// Therefore, I declare ye' GbxArray!
		// Guess they didn't want to change dynamic_array to support a 'capacity'
		template<typename DatumT> struct GbxArray
		{
		private:
			DatumT* elements;
			int32 count;
			int32 capacity;

		public:
			int32 GetCount() const { return count; }

			int32 GetCapacity() const { return capacity; }

			API_INLINE DatumT* operator [](int32 index)
			{
				if(index < 0 || index >= count) return NULL;

				return &CAST_PTR(DatumT*, elements)[index];
			}

			API_INLINE const DatumT* operator [](int32 index) const
			{
				if(index < 0 || index >= count) return NULL;

				return &CAST_PTR(DatumT*, elements)[index];
			}
		};


		//////////////////////////////////////////////////////////////////////////
		// Memory Pool

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

		//////////////////////////////////////////////////////////////////////////
		// Least Recently Used Cache

		typedef void (*lruv_block_delete_proc)(datum_index block_index);
		typedef bool (*lruv_locked_block_proc)(datum_index block_index);

		struct s_lruv_cache
		{
			tag_string				name;
			lruv_block_delete_proc	delete_proc;
			lruv_locked_block_proc	locked_block_proc;
			int32					page_count;
			int32					page_size_bits;
			int32					frame_index;
			datum_index				first_block_index;
			datum_index				last_block_index;
			s_data_array*			data;
			tag						signature;
		};

		struct s_lruv_cache_block
		{
			UNKNOWN_TYPE(int32);
			int32				page_count;
			datum_index			first_page_index;
			datum_index			next_block_index;
			datum_index			previous_block_index;
			int32				last_used_frame_index;
			UNKNOWN_TYPE(int32); // 0x18
		};
	};
};