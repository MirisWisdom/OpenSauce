/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/Halo1/tag_files/tag_groups.hpp>
#include <blamlib/Halo1/tag_files/tag_groups_structures.hpp>

namespace Yelo
{
	namespace blam
	{
		bool PLATFORM_API tag_data_load(void* block_element, tag_data* data, void* address);
		void PLATFORM_API tag_data_unload(void* block_element, tag_data* data);

		bool PLATFORM_API tag_block_read_children_recursive(const tag_block_definition *definition, void *address, int32 count, 
			int32 *position_reference, long_flags read_flags);
	};
};