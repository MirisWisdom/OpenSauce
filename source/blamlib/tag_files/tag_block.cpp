#include <Common/Precompile.hpp>
#include <blamlib/tag_files/tag_block.h>

#include <blamlib/tag_files/tag_groups_structures.hpp>

namespace Yelo
{
	#if PLATFORM_IS_EDITOR
	size_t tag_block::get_element_size() const
	{
		return definition->element_size;
	}

	void* tag_block::get_element(
		const int32 element)
	{
		return blam::tag_block_get_element(this, element);
	}

	void tag_block::delete_element(
		const int32 element)
	{
		blam::tag_block_delete_element(this, element);
	}

	int32 tag_block::add_element()
	{
		return blam::tag_block_add_element(this);
	}

	bool tag_block::resize(
		const int32 element_count)
	{
		return blam::tag_block_resize(this, element_count);
	}

	void* tag_block::add_and_get_element()
	{
		return blam::tag_block_add_and_get_element(this);
	}
	#endif
}
