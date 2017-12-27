#include <Common/Precompile.hpp>
#include <blamlib/tag_files/tag_data_definition.h>

namespace Yelo
{
	bool tag_data_definition::is_considered_debug_only() const
	{
		return
			// never streamed is not really debug, but it requires explicit tag_data_load before it is loaded into memory
			flags.test(e_tag_data_flags::never_streamed_bit) ||
			flags.test(e_tag_data_flags::not_streamed_to_cache_bit);
	}
}
