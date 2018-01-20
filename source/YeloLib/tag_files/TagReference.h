#pragma once

#include <blamlib/tag_files/tag_reference.h>

namespace Yelo
{
	// Template'd tag_reference that is nifty, and allows for better markup in the code
	template <tag k_tag>
	struct TagReference
	{
		tag group_tag;
		#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
		// path, without tag group extension, to the tag reference
		tag_reference_name_reference name;
		int32 name_length;
		#endif
		datum_index tag_index;

		operator datum_index() const
		{
			return tag_index;
		}

		bool operator ==(
			const uint32& rhs) const
		{
			return this->tag_index == rhs;
		};

		bool is_valid() const
		{
			return group_tag == k_tag;
		}

		void reset()
		{
			group_tag = k_tag;
			tag_index = datum_index::null;
		}

		static tag tag()
		{
			return k_tag;
		}
	};

	#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
	ASSERT_SIZE(TagReference<NONE>, 0x10);
	#else
	ASSERT_SIZE(TagReference<NONE>, 0x8);
	#endif

	template <>
	inline bool TagReference<NONE>::is_valid()
	{
		return true;
	}
}
