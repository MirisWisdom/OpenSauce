/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/Halo1/tag_files/tag_groups.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		template<typename TFieldSetType, int kMaxFieldCount>
		class c_tag_fieldset_replacement_memory
		{
			static int g_new_fields_count;
			static tag_field g_new_fields[kMaxFieldCount];

		public:
			int GetMaxFieldCount() const		{ return kMaxFieldCount; }

			int GetFieldCount() const			{ return g_new_fields_count; }
			int SetFieldCount(int count) const	{ g_new_fields_count = count; }

			tag_field* GetFields() const		{ return g_new_fields; }
		};

		class c_tag_fieldset_replacement_builder
		{
			const tag_field* m_source_fields;
			int m_source_fields_cursor;

			tag_field* m_target_fields;
			int m_target_fields_cursor;
			int m_target_fields_max_count;

			int m_available_pad_size;
			c_tag_field_scanner scanner;	// source_fields scanner, for field size/offset interop

		private:
			// Goto the next source field, optionally copying the current one to the current target field
			void NextSourceField(bool copy_current_field_first);
			bool CurrentSourceFieldIsTerminator() const;

			c_tag_fieldset_replacement_builder(
				const tag_field* source_fields,
				tag_field* target_fields, int target_fields_size, int target_fields_count);
		public:
			template<typename TFieldSetType, int kMaxFieldCount>
			c_tag_fieldset_replacement_builder(const tag_field* source_fields,
				c_tag_fieldset_replacement_memory<TFieldSetType, kMaxFieldCount> memory)
			{
				c_tag_fieldset_replacement_builder(source_fields, 
					memory.GetFields(), memory.GetMaxFieldCount(), memory.GetFieldCount())
			}

			void CopyFieldsFromSource(int field_count);
			void SkipFieldsFromSource(int field_count);

			void UselessPaddingBegin(int pad_size);
			int UselessPaddingEnd();

			tag_field& InsertField(tag_field insert_field, size_t expected_offset);
		};
	};
};