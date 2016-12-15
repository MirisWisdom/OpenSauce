/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once
#if PLATFORM_TYPE == PLATFORM_TOOL

#include <blamlib/Halo1/tag_files/tag_groups.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		class c_cache_file_memory_layout_table;
		class c_cache_file_memory_gestalt;
	};

	namespace Cache
	{
		class c_cache_file_memory_controller
		{
			TagGroups::c_cache_file_memory_layout_table* m_layout;
			TagGroups::c_cache_file_memory_gestalt* m_gesalt;

			int16 RegisterBlockDefinition(const tag_block_definition* block_definition);
			int16 RegisterDataDefinition(const tag_data_definition* data_definition);

		public:
			uintptr_t GetVirtualBaseAddress() const;
			void SetVirtualBaseAddress(uintptr_t base_address);

			uintptr_t RegisterBlockElements(const tag_block* block, uintptr_t cache_address);
		};
	};
};
#endif