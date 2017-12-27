#include <Common/Precompile.hpp>
#include <blamlib/tag_files/tag_data.h>

#include <blamlib/tag_files/tag_groups_base.hpp>

namespace Yelo
{
	byte* tag_data::Bytes() const
	{
		return reinterpret_cast<byte*>(address);
	}

#if PLATFORM_IS_EDITOR
	bool tag_data::resize(int32 new_size)
	{
		return blam::tag_data_resize(this, new_size);
	}
#endif
}
