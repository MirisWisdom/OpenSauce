#pragma once

#include <blamlib/cache/data_file_structures.hpp>

namespace Yelo { namespace Cache
{
	// build_cache_file_globals (tools) and cache_file_globals (runtime) share the same ordering of the s_data_file instances, so I wrap them in this fake struct
	struct s_data_file_globals
	{
		s_data_file sounds;
		s_data_file locale;
		s_data_file bitmaps;

		s_data_file& get(
			const Enums::data_file_reference_type data_file);

		#if PLATFORM_TYPE == PLATFORM_TOOL
		void save();
		void preprocess_for_save();
		#endif
	};

	ASSERT_SIZE(s_data_file_globals, 0xC0);
}}
