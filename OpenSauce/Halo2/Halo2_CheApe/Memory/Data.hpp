/*
    Yelo: Open Sauce SDK
		Halo 2 (Editing Kit) Edition

	See license\OpenSauce\Halo2_CheApe for specific license information
*/
#pragma once

#include <Memory/Halo2/DataShared.hpp>

namespace Yelo
{
	namespace Memory
	{
		struct s_resource_database_item {
			uint32 size;
			void* address;
			uint32 hash;
			bool unknown;
			PAD24;
		}; BOOST_STATIC_ASSERT( sizeof(s_resource_database_item) == 0x10 );
		struct s_resource_database {
			tag_string name;
			uint32 key_size;
			void* allocator;
			void* hash_table;
			int32 count;
			int32 maximum_count;
			HANDLE unknown1;
			HANDLE unknown2;
			void* base_address;
			s_resource_database_item* items;
		}; BOOST_STATIC_ASSERT( sizeof(s_resource_database) == 0x44 );
	};
};