#include <Common/Precompile.hpp>
#include <blamlib/tag_files/tag_block.h>

#include <blamlib/cseries/cseries_base.hpp>
#include <blamlib/tag_files/tag_block_definition.h>
#include <blamlib/tag_files/tag_groups_base.hpp>

namespace Yelo
{
	int32 tag_block::add_element()
	{
		return blam::tag_block_add_element(this);
	}

	void* tag_block::add_and_get_element()
	{
		return blam::tag_block_add_and_get_element(this);
	}

	void tag_block::delete_element(
		const int32 element)
	{
		blam::tag_block_delete_element(this, element);
	}

	void* tag_block::get_element(
		const int32 element)
	{
		return blam::tag_block_get_element(this, element);
	}

	bool tag_block::resize(
		const int32 element_count)
	{
		return blam::tag_block_resize(this, element_count);
	}

	#if PLATFORM_IS_EDITOR
	size_t tag_block::get_element_size() const
	{
		return definition->element_size;
	}

	tag_block::s_iterator_result::s_iterator_result(
		void* const address,
		const int32 index): address(address)
		, index(index) {}

	tag_block::s_iterator::s_iterator(
		tag_block& block,
		const size_t element_size,
		const size_t element_index)
		: m_address(reinterpret_cast<byte*>(block.address) + element_size * element_index)
		, m_element_index(element_index)
		, m_element_size(element_size) { }

	bool tag_block::s_iterator::operator!=(
		const s_iterator& other) const
	{
		return m_address != other.m_address;
	}

	tag_block::s_iterator& tag_block::s_iterator::operator++()
	{
		m_address += m_element_size;
		++m_element_index;
		return *this;
	}

	tag_block::s_iterator_result tag_block::s_iterator::operator*() const
	{
		return s_iterator_result(m_address, m_element_index);
	}

	tag_block::s_iterator tag_block::begin()
	{
		return s_iterator(*this, this->get_element_size());
	}

	tag_block::s_iterator tag_block::end()
	{
		return s_iterator(*this, this->get_element_size(), static_cast<size_t>(this->count));
	}
	#endif
}
