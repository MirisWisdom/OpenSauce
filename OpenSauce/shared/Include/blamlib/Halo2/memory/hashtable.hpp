/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Memory
	{
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
	};
};