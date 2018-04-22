#pragma once

#include <blam/cseries/cseries_base.hpp>
#include <blam/cseries/enum_templates.h>
#include <blam/memory/byte_swapping_base.hpp>
#include <blam/memory/datum_index.hpp>
#include <blam/tag_files/c_tag_iterator.h>
#include <blam/tag_files/tag_groups_base.hpp>

namespace Yelo
{
	struct tag_field;

	namespace TagGroups
	{
		struct s_tag_field_set_runtime_data;
	}

	namespace e_tag_block_definition_flags
	{
		typedef enum : long_flags
		{
			// set when block/data/reference fields are found during initialization
			has_children_bit,

			k_count
		}type_t;

		typedef flags_type<type_t> flags_t;
	}

	// Called as each element is read from the tag stream
	// NOTE: tag_index is non-standard, and will only be valid when invoked by OS code.
	// We can add additional parameters so long as PLATFORM_API is __cdecl (where the caller cleans up the stack).
	// NOTE: tag_index is not guaranteed to be not NONE! Eg: tag_block_add_element
	typedef bool (PLATFORM_API* proc_tag_block_postprocess_element_t)(
		void* element,
		Enums::tag_postprocess_mode mode,
		datum_index tag_index);
	// if [formatted_buffer] returns empty, the default block formatting is done
	typedef cstring (PLATFORM_API* proc_tag_block_format_t)(
		datum_index tag_index,
		tag_block* block,
		int32 element_index,
		char formatted_buffer[Enums::k_tag_block_format_buffer_size]);
	// Procedure called during tag_block_delete_element, but before all the child data is freed
	typedef void (PLATFORM_API* proc_tag_block_delete_element_t)(
		tag_block* block,
		int32 element_index);

	struct tag_block_definition
	{
		cstring name;
		e_tag_block_definition_flags::flags_t flags;
		int32 maximum_element_count;
		size_t element_size;
		void* unused0;
		tag_field* fields;
		void* unused1;
		proc_tag_block_postprocess_element_t postprocess_proc;
		proc_tag_block_format_t format_proc;
		proc_tag_block_delete_element_t delete_proc;
		byte_swap_code_t* byte_swap_codes;

		#if PLATFORM_IS_EDITOR
		// Searches the definition for a field of type [field_type] with a name which starts 
		// with [name] characters. Optionally starts at a specific field index.
		// Returns NONE if this fails.
		int32 find_field_index(
			const _enum field_type,
			cstring name,
			int32 start_index = NONE) const;
		tag_field* find_field(
			const _enum field_type,
			cstring name,
			const int32 start_index = NONE) const;
		tag_block_definition* find_block_field(
			cstring name,
			const int32 start_index = NONE) const;

		size_t get_alignment_bit() const;
		TagGroups::s_tag_field_set_runtime_data* get_runtime_info() const;
		void set_runtime_info(
			TagGroups::s_tag_field_set_runtime_data* const info);

		// t_action: void operator()([const] tag_block_definition* block, [const] tag_field& field)
		template <class t_action, bool k_assert_field_type>
		void fields_do_action(
			t_action& action = t_action())
		{
			for (auto& field : *this)
			{
				if (k_assert_field_type)
				{
					YELO_ASSERT(field.type >= 0 && field.type<e_field_type::k_count);
				}

				action(this, field);
			}
		}

		// Mainly a visitor for startup/shutdown processes, performs an action (via a functor) on a root block definition
		// then repeats the action for all child blocks (etc)
		// Uses CRT assert()'s since it is assumed this is used after the group definitions have been verified
		// t_action: void operator()([const] tag_block_definition* block_definition)
		template <class t_action>
		void do_recursive_action(
			t_action& action = t_action())
		{
			action(this); // perform the action on the root first

			size_t field_index = 0;

			do
			{
				const auto& field = fields[field_index];
				assert(field.type >= 0 && field.type<e_field_type::k_count);

				if (field.type == e_field_type::block)
				{
					assert(field.definition != this);

					action(field.get_definition<tag_block_definition>());
				}
			} while (fields[field_index++].type != e_field_type::terminator);
		}

		struct s_field_iterator
		{
		private:
			tag_field* m_fields;

		public:
			s_field_iterator(); // for EndHack
			explicit s_field_iterator(
				const tag_block_definition* const definition);

			bool operator!=(
				const s_field_iterator& other) const;
			s_field_iterator& operator++();
			tag_field& operator*() const;

		private:
			bool is_end_hack() const;
		};

		s_field_iterator begin() const;
		static s_field_iterator end();
		#endif
	};

	ASSERT_SIZE(tag_block_definition, 0x2C);
}
