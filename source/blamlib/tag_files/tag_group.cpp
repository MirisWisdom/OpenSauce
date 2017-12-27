#include <Common/Precompile.hpp>
#include <blamlib/tag_files/tag_group.h>

#include <blamlib/tag_files/tag_block_definition.h>

namespace Yelo
{
	#if PLATFORM_IS_EDITOR
	TagGroups::s_tag_field_set_runtime_data* tag_group::get_header_runtime_info() const
	{
		return header_block_definition->get_runtime_info();
	}

	bool tag_group::is_debug_only() const
	{
		return flags.test(e_tag_group_flags::debug_only_yelo_bit);
	}
	#endif
}
