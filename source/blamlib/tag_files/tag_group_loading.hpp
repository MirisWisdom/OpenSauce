/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/cseries/cseries_base.hpp>
#include <blamlib/cseries/enum_templates.h>
#include <blamlib/memory/datum_index.hpp>
#include <blamlib/tag_files/tag_groups_base.hpp>
#include <yelolib/tag_files/tag_groups_base_yelo.hpp>

namespace Yelo
{
	struct tag_data;
	struct tag_block_definition;

	namespace blam
	{
		cstring tag_load_error_string_get();
		// clear the error string used to track tags which failed to load
		void tag_load_error_string_clear();
		// print the tag names which failed to load to the normal error output (as warnings)
		void tag_load_error_string_print();

		bool PLATFORM_API tag_data_load(void* block_element, tag_data* data, void* address);
		void PLATFORM_API tag_data_unload(void* block_element, tag_data* data);

		bool tag_block_read_children_recursive(
			const tag_block_definition *definition,
			void *address,
			int32 count,
			int32 *position_reference,
			e_tag_load_flags::flags_t read_flags,
			// NOTE: nonstandard parameters
			datum_index tag_index);

#if PLATFORM_USES_CACHE_FILES
		inline void tag_load_error_string_clear() { }
		inline void tag_load_error_string_print() { }
#endif
	};
};