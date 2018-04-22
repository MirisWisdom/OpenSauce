#pragma once

#include <blam/cseries/cseries_base.hpp>
#include <blam/cseries/enum_templates.h>

namespace Yelo
{
	namespace e_tag_data_flags
	{
		typedef enum : long_flags
		{
			// Never streamed, unless the tag is loaded with _tag_load_for_editor_bit
			// Used by the sound tag for delay loading the actual sample data. So, eg, when tool goes to build a cache
			// it has to use tag_data_load on the sound samples. Maybe a better name is just 'lazy_loaded'
			never_streamed_bit,
			is_text_data_bit,
			// ie, 'debug data'
			not_streamed_to_cache_bit,

			k_count
		} type_t;

		typedef flags_type<type_t> flags_t;
	}

	typedef void (PLATFORM_API* proc_tag_data_byte_swap)(
		void* block_element,
		void* address,
		int32 size);

	struct tag_data_definition
	{
		cstring name;
		e_tag_data_flags::flags_t flags;
		int32 maximum_size;
		proc_tag_data_byte_swap byte_swap_proc;

		bool is_considered_debug_only() const;
	};

	ASSERT_SIZE(tag_data_definition, 0x10);
}
