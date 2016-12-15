/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/Halo2/memory/data.hpp>
#include <blamlib/Halo2/memory/hashtable.hpp>
#include <blamlib/Halo2/memory/memory_pool.hpp>

namespace Yelo
{
	namespace Memory
	{
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