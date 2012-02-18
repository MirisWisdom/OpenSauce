/*
    Yelo: Open Sauce SDK
		Halo 2 (Editing Kit) Edition

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <Memory/DataShared.hpp>

namespace Yelo
{
	struct s_file_reference {
		tag			signature;
		word_flags	flags;
		_enum		location;
		long_string	file_name;
		HANDLE		handle;
		HRESULT		api_result;
	}; BOOST_STATIC_ASSERT( sizeof(s_file_reference) == 0x110 );

	namespace Flags
	{
		enum data_array
		{
			_data_array_can_disconnect_bit,
			_data_array_disconnected_bit,

			_data_array_allocation_bit,
			_data_array,

			_data_array_can_disconnect_flag =	FLAG(_data_array_can_disconnect_bit),
			_data_array_disconnected_flag =		FLAG(_data_array_disconnected_bit),

			_data_array_allocation_flag =	FLAG(_data_array_allocation_bit),
		};
	};

	namespace Memory
	{
		struct s_data_array {
			tag_string name;		// Name of the data array
			int32 maximum_count;	// How many datums this array can hold
			uint32 datum_size;		// Size of one datum
			byte alignment_bit;
			bool is_valid;			// True if this array is usable
			word_flags flags;
			tag signature;			// 'd@t@'
			void** allocator;		// allocation object
			struct {
				int32 next_index;	// Next available bit index in the bit vector that can be used
				int32 length;		// Current length (in bits) of the bit index
			}active_indices;
			int32 actual_count;		// Number of datums in use
			datum_index next_datum;
			void* data;
			byte* active_indices_bit_vector;

			__forceinline int32 GetLength() const { return actual_count; }

			static void* IteratorNext(void* iter);

		}; BOOST_STATIC_ASSERT( sizeof(s_data_array) == 0x4C );
		struct s_data_array_iterator {
			s_data_array* array;
			datum_index datum;
			int32 cursor;
		}; BOOST_STATIC_ASSERT( sizeof(s_data_array_iterator) == 0xC );

		template<typename DatumT, const size_t DatumCount> struct DataArray {
		private:
			typedef DataArray<DatumT,DatumCount> DataArrayT;

		public:
			struct Iterator {
			private:
				DataArrayT* Array;
				datum_index Datum;
				int32 CurrentIndex;

			public:
				Iterator(DataArrayT* array)
				{
					Array = array;
					Datum = datum_index::null;
					CurrentIndex = NONE;
				}

				DatumT* Next() {
					return CAST_PTR(DatumT*, s_data_array::IteratorNext(this));
				}

				datum_index Current() const { return this->Datum; }
			}; BOOST_STATIC_ASSERT( sizeof(Iterator) == sizeof(s_data_array_iterator) );

			s_data_array Header;

		public:

			Iterator& IteratorNew(Iterator& iter) {
				iter = Iterator(this);
			}

			API_INLINE DatumT* operator [](datum_index handle) { return &CAST_PTR(DatumT*, this->Header.data)[handle.index]; }

			API_INLINE operator DatumT*()
			{ 
				return CAST_PTR(DatumT*, this->Header.data);
			}

			API_INLINE size_t GetDatumCount() const { return DatumCount; }
		};

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


		struct s_hash_table_item {
			void* data;
			uint32 hash;
			s_hash_table_item* next;
		}; BOOST_STATIC_ASSERT( sizeof(s_hash_table_item) == 0xC );
		struct s_hash_table {
			tag_string name;
			uint32 buffer_size;
			int32 maximum_count;
			uint32 key_size;
			void* hash_proc; // uint32 (void* buffer)
			void* compare_proc; // bool (const void*, const void*)
			void** allocator;
			s_hash_table_item* items;
		}; BOOST_STATIC_ASSERT( sizeof(s_hash_table) == 0x3C );


		struct s_constant_pool_datum {
			uint32 header;
			void* address; // inside s_constant_pool->values_pool
		}; BOOST_STATIC_ASSERT( sizeof(s_constant_pool_datum) == 0x8 );
		struct s_constant_pool {
			tag_string name;
			void* sizeof_proc; // size_t (const void* buffer)
			void* hasher_proc;
			void* format_proc; // char* (char* buffer)
			void* comparer_proc;
			void* copy_proc; // void (const void* src, const void* dst)
			void** allocator;
			s_hash_table* keys;
			s_data_array* values;
			s_memory_pool* values_pool;
		}; BOOST_STATIC_ASSERT( sizeof(s_constant_pool) == 0x44 );
	};
};