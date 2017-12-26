#pragma once

#include <blamlib/tag_files/s_tag_iterator.h>
#include <blamlib/tag_files/tag_groups.hpp>

namespace Yelo { namespace TagGroups
{
	// Note: when used in range based for loops this will create an unnecessary copy operation, but with SSE2 it shouldn't be that bad
	class c_tag_iterator
	{
		s_tag_iterator m_state;
		datum_index m_tag_index;

		explicit c_tag_iterator(
			const void* end_hack_dummy);

	public:
		explicit c_tag_iterator(
			const tag group_tag_filter);

		template <typename t_type>
		c_tag_iterator() :
			m_tag_index(datum_index::null)
		{
			blam::tag_iterator_new<t_type>(m_state);
		}

		// Get an iterator that doesn't have any specific group_tag filter
		static c_tag_iterator all();
		datum_index next();

		bool operator!=(
			const c_tag_iterator& other) const;
		c_tag_iterator& operator++();
		datum_index operator*() const;

		c_tag_iterator& begin(); /*const*/
		static c_tag_iterator end(); /*const*/
	};
}}
