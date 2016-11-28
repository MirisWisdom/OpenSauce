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
		struct s_data_array;

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