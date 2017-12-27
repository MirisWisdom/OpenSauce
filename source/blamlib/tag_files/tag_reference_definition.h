#pragma once

#include <blamlib/cseries/cseries_base.hpp>
#include <blamlib/cseries/enum_templates.h>

namespace Yelo
{
	namespace e_tag_reference_flags
	{
		typedef enum : long_flags
		{
			// checked in the tag reference solving code.
			// last condition checked after an assortment of conditionals
			// and if this is FALSE, it won't resolve
			// NOTE: I think this a deprecated flag for loading the 'default' definition of a group.
			// This flag would cause a call of tag_load(group_tag, NULL, 0) to occur. However,
			// tag_load asserts name != NULL.
			// Flag isn't checked in H2EK's code (so more than likely deprecated)
			unknown0_bit,
			non_resolving_bit,

			k_count
		} type_t;

		typedef flags_type<type_t> flags_t;
	}

	struct tag_reference_definition
	{
		e_tag_reference_flags::flags_t flags;
		tag group_tag;
		tag* group_tags;

		#if PLATFORM_IS_EDITOR
		struct s_group_tag_iterator
		{
		private:
			tag* m_group_tags;

		public:
			s_group_tag_iterator(); // for EndHack
			explicit s_group_tag_iterator(
				const tag_reference_definition* definition);

			bool operator!=(
				const s_group_tag_iterator& other) const;
			s_group_tag_iterator& operator++();
			tag& operator*() const;

		private:
			bool is_end_hack() const;
		};

		s_group_tag_iterator begin() const;
		static s_group_tag_iterator end();
		#endif
	};

	BOOST_STATIC_ASSERT(sizeof(tag_reference_definition) == 0xC);
}
