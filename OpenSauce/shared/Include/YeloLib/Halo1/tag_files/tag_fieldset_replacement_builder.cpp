/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#if PLATFORM_IS_EDITOR
#include <YeloLib/Halo1/tag_files/tag_fieldset_replacement_builder.hpp>

#include <blamlib/Halo1/tag_files/tag_group_verification.hpp>
#include <YeloLib/Halo1/tag_files/string_id_yelo.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		const tag_field* c_tag_field_set_replacement_builder::GetSourceFields() const
		{
			return m_source_scanner.GetFields();
		}
		int32 c_tag_field_set_replacement_builder::GetSourceFieldIndex() const
		{
			return m_source_scanner.GetFieldIndex();
		}
		size_t c_tag_field_set_replacement_builder::GetSourceFieldOffset() const
		{
			return m_source_scanner.GetFieldOffset();
		}
		bool c_tag_field_set_replacement_builder::NotEndOfSourceFields() const
		{
			return !m_source_scanner.IsDone();
		}
		bool c_tag_field_set_replacement_builder::SourceFieldMatchesName(cstring field_name) const
		{
			assert(field_name);

			int target_length = strlen(field_name);

			// Get the source field name, if the name is empty compare against the length of the target name
			cstring source_field_name = m_source_scanner.GetTagFieldName();
			if (source_field_name == nullptr)
			{
				return target_length == 0;
			}

			// Get the length of the source field's name
			int source_length = 0;
			cstring name_end = strpbrk(source_field_name, "!*^:#");
			if(name_end == nullptr)
			{
				source_length = strlen(source_field_name);
			}
			else
			{
				source_length = name_end - source_field_name;
			}

			// Compare the names by length and characters
			return (target_length == source_length) && strncmp(source_field_name, field_name, source_length)==0;
		}

		bool c_tag_field_set_replacement_builder::NotEndOfTargetFields() const
		{
			return m_target_fields_cursor < m_target_fields_max_count;
		}
		void c_tag_field_set_replacement_builder::CopyFieldToTargetFields(const tag_field* field)
		{
			YELO_ASSERT( NotEndOfTargetFields() );

			tag_field* dst = &m_target_fields[m_target_fields_cursor++];
			std::memcpy(dst, field, sizeof(*field));

			m_total_fields_size += field->GetSize(nullptr);
		}

		void c_tag_field_set_replacement_builder::CopySourceField(int32 index)
		{
			const tag_field* src = &GetSourceFields()[index];
			CopyFieldToTargetFields(src);
		}
		bool c_tag_field_set_replacement_builder::NextSourceField(bool copy_current_field_first)
		{
			YELO_ASSERT(NotEndOfSourceFields());

			if (copy_current_field_first)
				CopySourceField(GetSourceFieldIndex());

			return m_source_scanner.Scan();
		}

		bool c_tag_field_set_replacement_builder::DetectPaddingInfo()
		{
			bool found_pad_field = false;
			m_padding.detected_pad_field_first_index = GetSourceFieldIndex();
			m_padding.detected_pad_field_count = 0;
			m_padding.detected_pad_size = 0;

			m_padding.untapped_offset = GetSourceFieldOffset();
			m_padding.untapped_useless_size = 0;

			while (NotEndOfSourceFields() && m_source_scanner.GetTagFieldType() == Enums::_field_pad)
			{
				found_pad_field = true;
				m_padding.detected_pad_field_count++;
				m_padding.detected_pad_size += m_source_scanner.GetFieldSize();
				NextSourceField(false);
			}

			m_padding.untapped_useless_size = m_padding.detected_pad_size;

			return found_pad_field;
		}

		c_tag_field_set_replacement_builder::c_tag_field_set_replacement_builder(
			const tag_block_definition* source_definition,
			tag_field* target_fields, size_t target_fields_max_count, size_t& target_fields_count_ref)
			: m_source_definition( source_definition )

			, m_target_fields( target_fields )
			, m_target_fields_cursor( 0 )
			, m_target_fields_count( target_fields_count_ref )
			, m_target_fields_max_count( target_fields_max_count )

			, m_flags()
			, m_padding()

			, m_source_scanner( source_definition->fields )

			, m_total_fields_size( 0 )
		{
			YELO_ASSERT( source_definition->fields != nullptr );
			YELO_ASSERT( target_fields != nullptr && target_fields_max_count > 0 );
			// the target fields shouldn't be initialized yet
			YELO_ASSERT( target_fields_count_ref == 0 );

			m_source_scanner.AddAllFieldTypes();
			m_source_scanner.Scan();
		}
		c_tag_field_set_replacement_builder::~c_tag_field_set_replacement_builder()
		{
			YELO_ASSERT_DISPLAY(m_source_scanner.IsDone(), "replacement builder for %s finalized without actually finishing",
				m_source_definition->name);
		}

		void c_tag_field_set_replacement_builder::VerifyTotalFieldsSize(size_t expected_size)
		{
			YELO_ASSERT_DISPLAY(expected_size == m_total_fields_size, 
				"replacement builder failed on %s, sizeof expected %u, got %u",
				m_source_definition->name, expected_size, m_total_fields_size);
		}
		tag_field* c_tag_field_set_replacement_builder::Finish()
		{
			while (NotEndOfSourceFields())
				NextSourceField(true);

			CopySourceField(GetSourceFieldIndex()); // copy the source field's terminator

			return m_target_fields;
		}

		void c_tag_field_set_replacement_builder::CopyFieldsFromSource(size_t field_count)
		{
			for (size_t x = 0; x < field_count && NotEndOfSourceFields(); x++)
				NextSourceField(true);
		}
		void c_tag_field_set_replacement_builder::SkipFieldsFromSource(size_t field_count)
		{
			for (size_t x = 0; x < field_count && NotEndOfSourceFields(); x++)
				NextSourceField(false);
		}

		size_t c_tag_field_set_replacement_builder::CopyFieldsFromSourceUpTo(cstring field_name)
		{
			if (field_name == nullptr)
				field_name = "";
			size_t field_count = 0;

			for (bool name_matches = false; !name_matches && NotEndOfSourceFields(); field_count++)
			{
				name_matches = SourceFieldMatchesName(field_name);
				NextSourceField(true);
			}
			return field_count;
		}
		size_t c_tag_field_set_replacement_builder::SkipFieldsFromSourceUpTo(cstring field_name)
		{
			if (field_name == nullptr)
				field_name = "";
			size_t field_count = 0;

			for (bool name_matches = false; !name_matches && NotEndOfSourceFields(); field_count++)
			{
				name_matches = SourceFieldMatchesName(field_name);
				NextSourceField(false);
			}

			return field_count;
		}

		void c_tag_field_set_replacement_builder::UselessPaddingBegin(size_t pad_size)
		{
			m_flags.insertion_failed = false;
			bool found_pad_field = DetectPaddingInfo();

			if (!found_pad_field)
				return;

			// pad_size is non-zero if there's an explicit amount of useless padding to use from a range of padding fields
			if (pad_size != 0)
				m_padding.untapped_useless_size = pad_size;
		}
		bool c_tag_field_set_replacement_builder::UselessPaddingEnd(_Out_opt_ size_t* ending_pad_size)
		{
			if (ending_pad_size != nullptr)
				*ending_pad_size = m_padding.untapped_useless_size;

			if (!m_flags.insertion_failed)
			{
				InsertPadData(m_padding.untapped_offset,
					CAST(int32, m_padding.untapped_useless_size));
			}

			return m_flags.insertion_failed;
		}

		c_tag_field_set_replacement_builder& c_tag_field_set_replacement_builder::AddExplanation(cstring name, cstring description)
		{
			if (name == nullptr)
			{
				YELO_WARN("tried to add an explanation field to %s with no name",
					m_source_definition->name);
			}
			else if (description == nullptr)
			{
				YELO_WARN("tried to add an explanation field %s to %s with no description",
					name, m_source_definition->name);
			}
			else
			{
				tag_field new_field = { Enums::_field_explanation, name,
					CAST_QUAL(char*, description) }; // definition is void*, cast to char* to play nice with initializer

				CopyFieldToTargetFields(&new_field);
			}

			return *this; // for operation chaining
		}

		c_tag_field_set_replacement_builder& c_tag_field_set_replacement_builder::InsertField(size_t expected_offset, 
			Enums::field_type type, cstring name, void* definition)
		{
			// do-while-false, for easier cleanup and simple flow
			do {
				if (m_flags.insertion_failed) // previous insertion failed, don't do anything
					break;

				// start by assuming shit is gonna fail
				m_flags.insertion_failed = true;

				// we currently don't support field arrays. don't silently fail the insert, this is a big problem.
				YELO_ASSERT_DISPLAY(type != Enums::_field_array_start && type != Enums::_field_array_end,
					"tried to insert a field array while building a new fieldset for %s", m_source_definition->name);

				if (!IN_RANGE_ENUM(type, Enums::k_number_of_tag_field_types))
				{
					YELO_WARN("tried to insert an invalid field of type %d named '%s' in %s",
						CAST(int, type), name, m_source_definition->name);

					break;
				}
				cstring field_type_name = k_tag_field_definitions[type].name;

				tag_field new_field = { type, name, definition };
				VerifyTagField(m_source_definition, new_field);

				if (expected_offset != m_padding.untapped_offset)
				{
					YELO_WARN("requested an %s insert for '%s' at offset %d but got offset %d",
						field_type_name, name, expected_offset, m_padding.untapped_offset);

					break;
				}

				size_t field_size = new_field.GetSize(nullptr);
				if (field_size > m_padding.untapped_useless_size)
				{
					YELO_WARN("requested an %s insert for '%s' (size %d) at offset %d but ran out of space (%d free)",
						field_type_name, name, field_size, expected_offset, m_padding.untapped_useless_size);

					break;
				}

				if (m_target_fields_cursor >= (m_target_fields_max_count-1))
				{
					YELO_WARN("requested an %s insert for '%s', but ran out of field slots before the terminator (%d)",
						field_type_name, name, m_target_fields_max_count-1);

					break;
				}

				// if we got here, we didn't 'break' because nothing was broke :p
				m_flags.insertion_failed = false;

				CopyFieldToTargetFields(&new_field);
				m_padding.untapped_offset += field_size;
				m_padding.untapped_useless_size -= field_size;
			} while (false);

			return *this; // for Insert chaining
		}

		c_tag_field_set_replacement_builder& c_tag_field_set_replacement_builder::InsertEnum(size_t expected_offset,
			size_t enum_size, cstring name, string_list* enum_definition)
		{
			Enums::field_type type;
			switch (enum_size)
			{
			case sizeof(_enum): type = Enums::_field_enum; break;

			YELO_ASSERT_CASE_UNREACHABLE();
			}

			return InsertField(expected_offset, type, name, enum_definition);
		}
		c_tag_field_set_replacement_builder& c_tag_field_set_replacement_builder::InsertFlags(size_t expected_offset,
			size_t flags_size, cstring name, string_list* flags_definition)
		{
			Enums::field_type type;
			switch (flags_size)
			{
			case sizeof(byte_flags): type = Enums::_field_byte_flags; break;
			case sizeof(word_flags): type = Enums::_field_word_flags; break;
			case sizeof(long_flags): type = Enums::_field_long_flags; break;

			YELO_ASSERT_CASE_UNREACHABLE();
			}

			return InsertField(expected_offset, type, name, flags_definition);
		}
		c_tag_field_set_replacement_builder& c_tag_field_set_replacement_builder::InsertBlockIndex(size_t expected_offset,
			size_t index_size, cstring name, tag_block_definition* block_definition)
		{
			Enums::field_type type;
			switch (index_size)
			{
			case sizeof(int16): type = Enums::_field_short_block_index; break;
			case sizeof(int32): type = Enums::_field_long_block_index; break;

			YELO_ASSERT_CASE_UNREACHABLE();
			}

			return InsertField(expected_offset, type, name, block_definition);
		}

		c_tag_field_set_replacement_builder& c_tag_field_set_replacement_builder::InsertStringId(size_t expected_offset,
			cstring name)
		{
			YELO_ASSERT(string_id_yelo::global_reference_definition);

			return 
				 InsertTagReference(expected_offset, 
					name, string_id_yelo::global_reference_definition)
				.InsertPostprocessedData(expected_offset+sizeof(tag_reference), 
					sizeof(string_id), "string_id");
		}

		c_tag_field_set_replacement_builder& c_tag_field_set_replacement_builder::InsertBlock(size_t expected_offset,
			cstring name, tag_block_definition* block_definition)
		{
			YELO_ASSERT(block_definition);

			return InsertField(expected_offset,
				Enums::_field_block, name, block_definition);
		}

		c_tag_field_set_replacement_builder& c_tag_field_set_replacement_builder::InsertData(size_t expected_offset,
			cstring name, tag_data_definition* data_definition)
		{
			YELO_ASSERT(data_definition);

			return InsertField(expected_offset,
				Enums::_field_data, name, data_definition);
		}

		c_tag_field_set_replacement_builder& c_tag_field_set_replacement_builder::InsertTagReference(size_t expected_offset,
			cstring name, tag_reference_definition* reference_definition)
		{
			YELO_ASSERT(reference_definition);

			return InsertField(expected_offset,
				Enums::_field_tag_reference, name, reference_definition);
		}

		c_tag_field_set_replacement_builder& c_tag_field_set_replacement_builder::InsertPadData(size_t expected_offset,
			int32 padding_size, cstring name)
		{
			assert(padding_size > 0);

			return InsertField(expected_offset,
				Enums::_field_pad, name, CAST_PTR(void*, padding_size));
		}
		c_tag_field_set_replacement_builder& c_tag_field_set_replacement_builder::InsertPostprocessedData(size_t expected_offset,
			int32 postprocessed_data_size, cstring name)
		{
			assert(postprocessed_data_size > 0);

			return InsertPadData(expected_offset,
				postprocessed_data_size, name);
		}
		c_tag_field_set_replacement_builder& c_tag_field_set_replacement_builder::InsertSkipData(size_t expected_offset,
			int32 skip_size, cstring name)
		{
			assert(skip_size > 0);

			return InsertField(expected_offset,
				Enums::_field_skip, name, CAST_PTR(void*, skip_size));
		}
	};
};

#endif