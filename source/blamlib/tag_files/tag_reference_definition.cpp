#include <Common/Precompile.hpp>
#include <blamlib/tag_files/tag_reference_definition.h>

namespace Yelo
{
	#if PLATFORM_IS_EDITOR
	bool tag_reference_definition::s_group_tag_iterator::is_end_hack() const
	{
		return m_group_tags == nullptr;
	}

	tag_reference_definition::s_group_tag_iterator::s_group_tag_iterator()
		: m_group_tags(nullptr) {}

	tag_reference_definition::s_group_tag_iterator::s_group_tag_iterator(
		const tag_reference_definition* definition)
		: m_group_tags(definition->group_tags) {}

	bool tag_reference_definition::s_group_tag_iterator::operator!=(
		const s_group_tag_iterator& other) const
	{
		if (other.is_end_hack())
		{
			return *m_group_tags != NONE;
		}
		if (this->is_end_hack())
		{
			return *other.m_group_tags != NONE;
		}

		return m_group_tags != other.m_group_tags;
	}

	tag_reference_definition::s_group_tag_iterator& tag_reference_definition::s_group_tag_iterator::operator++()
	{
		++m_group_tags;
		return *this;
	}

	tag& tag_reference_definition::s_group_tag_iterator::operator*() const
	{
		return *m_group_tags;
	}

	tag_reference_definition::s_group_tag_iterator tag_reference_definition::begin() const
	{
		return s_group_tag_iterator(this);
	}

	tag_reference_definition::s_group_tag_iterator tag_reference_definition::end()
	{
		return s_group_tag_iterator();
	}
	#endif
}
