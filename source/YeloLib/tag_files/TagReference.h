#pragma once

#include <blamlib/tag_files/tag_reference.h>

namespace Yelo
{
	// Template'd tag_reference that is nifty, and allows for better markup in the code
	template <tag k_tag>
	struct TagReference
	{
		// group tag identifier for this reference
		tag GroupTag;

		#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
		// path, without tag group extension, to the tag reference
		tag_reference_name_reference Name;
		// length of the reference name
		int32 NameLength;
		#endif

		// datum index of this reference in the tag index
		datum_index TagIndex;

		operator datum_index()
		{
			return TagIndex;
		}

		OVERRIDE_OPERATOR_MATH_BOOL_TYPE(uint32, TagIndex, == );

		// Returns whether or not this reference has a valid group reference
		bool IsValid() const
		{
			return GroupTag == k_tag;
		}

		// Sets this reference to null
		void Reset()
		{
			GroupTag = k_tag;
			TagIndex = datum_index::null;
		}

		tag Tag() const
		{
			return k_tag;
		}
	};

	#if !defined(PLATFORM_USE_CONDENSED_TAG_INTERFACE)
	BOOST_STATIC_ASSERT(sizeof(TagReference<NONE>) == 0x10);
	#else
	BOOST_STATIC_ASSERT(sizeof(TagReference<NONE>) == 0x8);
	#endif

	template <>
	inline bool TagReference<NONE>::IsValid() const
	{
		return true;
	}
}
