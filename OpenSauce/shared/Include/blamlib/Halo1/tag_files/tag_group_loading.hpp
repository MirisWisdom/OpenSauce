/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

namespace Yelo
{
	struct tag_data;
	struct tag_block_definition;

	namespace blam
	{
		bool PLATFORM_API tag_data_load(void* block_element, tag_data* data, void* address);
		void PLATFORM_API tag_data_unload(void* block_element, tag_data* data);

		bool tag_block_read_children_recursive(const tag_block_definition *definition, void *address, int32 count, 
			int32 *position_reference, long_flags read_flags,
			// NOTE: nonstandard parameters
			datum_index tag_index);
	};
};