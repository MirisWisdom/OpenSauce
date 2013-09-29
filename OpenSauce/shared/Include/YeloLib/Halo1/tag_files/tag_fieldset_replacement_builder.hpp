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
		template<typename TFieldSetType, size_t kMaxFieldCount>
		class c_tag_field_set_replacement_memory
		{
			static size_t g_new_fields_count;
			static std::array<tag_field, kMaxFieldCount> g_new_fields;

		public:
			size_t GetMaxFieldCount() const				{ return kMaxFieldCount; }

			size_t GetFieldCount() const				{ return g_new_fields_count; }
			void SetFieldCount(size_t count) const		{ g_new_fields_count = count; }

			tag_field* GetFields() const				{ return g_new_fields.data(); }
			tag_field& operator[](size_t index) const	{ return g_new_fields[index]; }
		};

		class c_tag_field_set_replacement_builder
		{
			const tag_field* m_source_fields;
			size_t m_source_fields_cursor;

			tag_field* m_target_fields;
			size_t m_target_fields_cursor;
			size_t m_target_fields_max_count;

			size_t m_available_pad_size;
			c_tag_field_scanner scanner;	// source_fields scanner, for field size/offset interop

		private:
			// Goto the next source field, optionally copying the current one to the current target field
			void NextSourceField(bool copy_current_field_first);
			bool CurrentSourceFieldIsTerminator() const;

			c_tag_field_set_replacement_builder(
				const tag_field* source_fields,
				tag_field* target_fields, size_t target_fields_size, size_t target_fields_count);
		public:
			template<typename TFieldSetType, size_t kMaxFieldCount>
			c_tag_field_set_replacement_builder(const tag_field* source_fields,
				c_tag_field_set_replacement_memory<TFieldSetType, kMaxFieldCount> memory)
			{
				// TODO: have to wait until VS2013 for delegating constructors (this is creating a temp object, not ctoring this)
				c_tag_field_set_replacement_builder(source_fields, 
					memory.GetFields(), memory.GetMaxFieldCount(), memory.GetFieldCount())
			}

			void CopyFieldsFromSource(size_t field_count);
			void SkipFieldsFromSource(size_t field_count);

			void UselessPaddingBegin(size_t pad_size);
			size_t UselessPaddingEnd();

			tag_field& InsertField(tag_field insert_field, size_t expected_offset);
		};
	};
};