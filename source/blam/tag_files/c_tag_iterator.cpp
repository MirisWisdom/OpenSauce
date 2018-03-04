#include <Common/Precompile.hpp>
#include <blamlib/tag_files/c_tag_iterator.h>

#include <blamlib/tag_files/tag_groups.hpp>

namespace Yelo { namespace TagGroups
{
	c_tag_iterator::c_tag_iterator(
		const void* end_hack_dummy)
	{
		m_state.instances_iterator.SetEndHack();
	}

	c_tag_iterator::c_tag_iterator(
		const tag group_tag_filter) :
		m_tag_index(datum_index::null)
	{
		blam::tag_iterator_new(m_state, group_tag_filter);
	}

	c_tag_iterator c_tag_iterator::all()
	{
		return c_tag_iterator(NONE);
	}

	datum_index c_tag_iterator::next()
	{
		return m_tag_index = blam::tag_iterator_next(m_state);
	}

	bool c_tag_iterator::operator!=(
		const c_tag_iterator& other) const
	{
		// NOTE: we intentionally don't compare the group_tag filter
		return m_state.instances_iterator != other.m_state.instances_iterator;
	}

	c_tag_iterator& c_tag_iterator::operator++()
	{
		next();
		return *this;
	}

	datum_index c_tag_iterator::operator*() const
	{
		return m_tag_index;
	}

	c_tag_iterator& c_tag_iterator::begin()
	{
		this->next();
		return *this;
	}

	c_tag_iterator c_tag_iterator::end()
	{
		return c_tag_iterator(nullptr);
	}
}}
