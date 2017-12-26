#include <Common/Precompile.hpp>
#include <blamlib/tag_files/tag_block_definition.h>

namespace Yelo
{
	#if PLATFORM_IS_EDITOR
	int32 tag_block_definition::find_field_index(
		const _enum field_type,
		cstring name,
		int32 start_index) const
	{
		YELO_ASSERT(this);
		YELO_ASSERT(this->fields);
		YELO_ASSERT(IN_RANGE_ENUM(field_type, e_field_type::k_count));
		YELO_ASSERT(name);

		if (start_index == NONE)
		{
			start_index = 0;
		}

		const auto name_length = strlen(name);

		for (const tag_field* cur = this->fields + start_index; cur->type != e_field_type::terminator; cur++)
		{
			if (cur->type == field_type && !strncmp(cur->name, name, name_length))
			{
				return cur - this->fields;
			}
		}

		return NONE;
	}

	tag_field* tag_block_definition::find_field(
		const _enum field_type,
		cstring name,
		const int32 start_index) const
	{
		auto index = find_field_index(field_type, name, start_index);

		YELO_ASSERT_DISPLAY(index != NONE, "failed to find a %s field named %s in %s",
			TagGroups::k_tag_field_definitions[field_type].name, name, this->name);

		return &this->fields[index];
	}

	tag_block_definition* tag_block_definition::find_block_field(
		const cstring name,
		const int32 start_index) const
	{
		const auto block_field = find_field(e_field_type::block, name, start_index);

		return block_field->get_definition<tag_block_definition>();
	}

	bool tag_block_definition::s_field_iterator::is_end_hack() const
	{
		return m_fields == nullptr;
	}

	tag_block_definition::s_field_iterator::s_field_iterator()
		: m_fields(nullptr) {}

	tag_block_definition::s_field_iterator::s_field_iterator(
		const tag_block_definition* const definition)
		: m_fields(definition->fields) {}

	bool tag_block_definition::s_field_iterator::operator!=(
		const s_field_iterator& other) const
	{
		if (other.is_end_hack())
		{
			return m_fields->type != e_field_type::terminator;
		}
		if (this->is_end_hack())
		{
			return other.m_fields->type != e_field_type::terminator;
		}

		return m_fields != other.m_fields;
	}

	tag_block_definition::s_field_iterator& tag_block_definition::s_field_iterator::operator++()
	{
		++m_fields;
		return *this;
	}

	tag_field& tag_block_definition::s_field_iterator::operator*() const
	{
		return *m_fields;
	}

	tag_block_definition::s_field_iterator tag_block_definition::begin() const
	{
		return s_field_iterator(this);
	}

	tag_block_definition::s_field_iterator tag_block_definition::end()
	{
		return s_field_iterator();
	}
	#endif
}
