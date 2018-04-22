#include <blam/blam.h>
#include <blam/tag_files/tag_reference.h>

#include <blam/tag_files/tag_groups_base.hpp>

namespace Yelo
{
	tag_reference::operator datum_index() const
	{
		return tag_index;
	}

	#if PLATFORM_IS_EDITOR
	void tag_reference::clear()
	{
		blam::tag_reference_clear(*this);
	}

	void tag_reference::set(
		tag group_tag,
		cstring name)
	{
		blam::tag_reference_set(*this, group_tag, name);
	}
	#endif
}
