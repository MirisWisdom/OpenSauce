/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <blamlib/Halo1/tag_files/tag_groups.hpp>

#if PLATFORM_IS_EDITOR
	#include <blamlib/Halo1/memory/byte_swapping.hpp>
	#include <blamlib/Halo1/tag_files/tag_group_loading.hpp>
	#include <YeloLib/Halo1/tag_files/string_id_yelo.hpp>

	#include "Engine/EngineFunctions.hpp"

namespace Yelo
{
	namespace TagGroups
	{
		tag_instance_data_t** TagInstanceData(); // forward declaration, defined in CheApe

		int32 StringFieldGetSize(const tag_field* field)
		{
			assert(field->type == Enums::_field_string);

			uintptr_t definition = CAST_PTR(uintptr_t, field->definition);

			if(definition == 0 || TagFieldIsOldStringId(field))
				return sizeof(tag_string);
			else // NOTE: the definition should have already been validated if tag_groups_initialize has already ran
				return CAST(int32, definition)+1;
		}

		void tag_data_delete(tag_data* data, size_t terminator_size)
		{
			blam::tag_data_resize(data, terminator_size);
		}

		bool tag_block_delete_all_elements(tag_block* block)
		{
			return blam::tag_block_resize(block, 0);
		}

		//////////////////////////////////////////////////////////////////////////
		// c_tag_iterator
		c_tag_iterator::c_tag_iterator(const void* endHackDummy) //:
			//m_tag_index(datum_index::null)
		{
			//memset(&m_state, 0, sizeof(m_state));
			m_state.instances_iterator.SetEndHack();
		}

		c_tag_iterator::c_tag_iterator(const tag group_tag_filter) :
			m_tag_index(datum_index::null)
		{
			blam::tag_iterator_new(m_state, group_tag_filter);
		}

		datum_index c_tag_iterator::Next()
		{
			return m_tag_index = blam::tag_iterator_next(m_state);
		}


		//////////////////////////////////////////////////////////////////////////
		// c_tag_field_scanner
		c_tag_field_scanner::c_tag_field_scanner(const tag_field* fields, void* fields_address)
		{
			blam::tag_field_scan_state_new(m_state, fields, fields_address);
			m_state.SetYeloScanState();
		}

		c_tag_field_scanner& c_tag_field_scanner::AddFieldType(Enums::field_type field_type)
		{
			blam::tag_field_scan_state_add_field_type(m_state, field_type);
			return *this;
		}

		c_tag_field_scanner& c_tag_field_scanner::AddAllFieldTypes()
		{
			memset(m_state.field_types, -1, sizeof(m_state.field_types));
			return *this;
		}

		bool c_tag_field_scanner::Scan()
		{
			return blam::tag_field_scan(m_state);
		}
		c_tag_field_scanner& c_tag_field_scanner::ScanToEnd()
		{
			for(int debug_scan_count = 0; Scan(); debug_scan_count++)
				assert(debug_scan_count <= Enums::k_maximum_field_byte_swap_codes);

			return *this;
		}

		bool c_tag_field_scanner::TagFieldIsStringId() const
		{
			return TagGroups::TagFieldIsStringId(m_state.found_field);
		}
		bool c_tag_field_scanner::TagFieldIsOldStringId() const
		{
			return TagGroups::TagFieldIsOldStringId(m_state.found_field);
		}

		bool c_tag_field_scanner::s_iterator::operator!=(const c_tag_field_scanner::s_iterator& other) const
		{
			if(other.IsEndHack())
				return !m_scanner->IsDone();
			else if(this->IsEndHack())
				return !other.m_scanner->IsDone();

			return m_scanner != other.m_scanner;
		}
	};

	size_t tag_block::get_element_size() const
	{
		return definition->element_size;
	}

	int32 tag_block_definition::find_field_index(_enum field_type, cstring name, int32 start_index) const
	{
		YELO_ASSERT( this );
		YELO_ASSERT( this->fields );
		YELO_ASSERT( IN_RANGE_ENUM(field_type, Enums::k_number_of_tag_field_types) );
		YELO_ASSERT( name );

		if(start_index == NONE) start_index = 0;

		size_t name_length = strlen(name);

		for(const tag_field* cur = this->fields + start_index; cur->type != Enums::_field_terminator; cur++)
		{
			if(cur->type == field_type && !strncmp(cur->name, name, name_length))
				return cur - this->fields;
		}

		return NONE;
	}

	bool tag_reference_definition::s_group_tag_iterator::operator!=(const tag_reference_definition::s_group_tag_iterator& other) const
	{
		if(other.IsEndHack())
			return *m_group_tags != NONE;
		else if(this->IsEndHack())
			return *other.m_group_tags != NONE;

		return m_group_tags != other.m_group_tags;
	}

	bool tag_block_definition::s_field_iterator::operator!=(const tag_block_definition::s_field_iterator& other) const
	{
		if(other.IsEndHack())
			return m_fields->type != Enums::_field_terminator;
		else if(this->IsEndHack())
			return other.m_fields->type != Enums::_field_terminator;

		return m_fields != other.m_fields;
	}

	namespace blam
	{
		static void verify_string_list_definition(const string_list* definition,
			const tag_block_definition* block_definition, cstring field_type_name)
		{
			YELO_ASSERT_DISPLAY(definition, "no definition specified for %s field in block %s.",
				field_type_name, block_definition->name); // NOTE: added owner block name to info
			YELO_ASSERT( definition->count>=0 );

			for(int x = 0; x < definition->count; x++)
			{
				if(definition->strings[x] == nullptr)
				{
					YELO_ERROR(_error_message_priority_assert, "%s field in block %s doesn't have enough strings",
						field_type_name, block_definition->name);
				}
			}
		}
		template<typename TEnum>
		static void verify_enum_field_definition(const tag_field& field,
			const tag_block_definition* block_definition)
		{
			auto* definition = field.Definition<string_list>();
			verify_string_list_definition(definition, block_definition, "enum");
		}
		template<typename TFlags>
		static void verify_flags_field_definition(const tag_field& field,
			const tag_block_definition* block_definition)
		{
			auto* definition = field.Definition<string_list>();
			verify_string_list_definition(definition, block_definition, "flags");

			YELO_ASSERT( definition->count<=BIT_COUNT(TFlags) );
		}
		static void verify_tag_reference_field_definition(const tag_field& field,
			const tag_block_definition* block_definition)
		{
			auto* definition = field.Definition<tag_reference_definition>();
			YELO_ASSERT_DISPLAY(definition, "no definition specified for tag reference field in block %s.",
				block_definition->name); // NOTE: added owner block name to info
			YELO_ASSERT( VALID_FLAGS(definition->flags, Flags::k_number_of_tag_group_tag_reference_flags) );

			if(definition->group_tag != NONE)
			{
				YELO_ASSERT( tag_group_get(definition->group_tag) );
				YELO_ASSERT( definition->group_tags==nullptr );
			}
			else if(definition->group_tags != nullptr)
			{
				for(auto group_tag : *definition)
				{
					YELO_ASSERT( tag_group_get(group_tag) );
				}
			}
		}
		static void verify_data_field_definition(const tag_field& field,
			const tag_block_definition* block_definition)
		{
			auto* definition = field.Definition<tag_data_definition>();
			YELO_ASSERT_DISPLAY(definition, "no definition specified for tag_data field in block %s.",
				block_definition->name); // NOTE: added owner block name to info
			YELO_ASSERT( definition->name );
			YELO_ASSERT( VALID_FLAGS(definition->flags, Flags::k_number_of_tag_data_definition_flags) );
			YELO_ASSERT( definition->maximum_size>0 );
		}
		static void verify_block_field_definitions(tag_block_definition* block)
		{
			YELO_ASSERT( block );
			YELO_ASSERT( block->name );
			YELO_ASSERT( VALID_FLAGS(block->flags, Flags::k_number_of_tag_block_definition_flags) );
			YELO_ASSERT( block->maximum_element_count>=0 );
			YELO_ASSERT( CAST(int,block->element_size)>=0 );
			YELO_ASSERT( block->fields );

			for(auto& field : *block)
			{
				switch(field.type)
				{
				case Enums::_field_string:
					{
						uintptr_t definition = CAST_PTR(uintptr_t, field.definition);

						YELO_ASSERT( definition==0 || definition <= Enums::k_long_string_length || 
							TagGroups::TagFieldIsOldStringId(&field) );
					}
					break;

				case Enums::_field_enum:
					verify_enum_field_definition<int16>(field, block);
					break;

				case Enums::_field_long_flags:	verify_flags_field_definition<long_flags>(field, block); break;
				case Enums::_field_word_flags:	verify_flags_field_definition<word_flags>(field, block); break;
				case Enums::_field_byte_flags:	verify_flags_field_definition<byte_flags>(field, block); break;

				case Enums::_field_tag_reference:
					verify_tag_reference_field_definition(field, block);
					break;

				case Enums::_field_block:
					{
						auto* definition = field.Definition<tag_block_definition>();
						YELO_ASSERT_DISPLAY(definition, "no definition specified for block field in block %s.",
							block->name); // NOTE: added owner block name to info

						verify_block_field_definitions(definition);
					}
					break;

				case Enums::_field_short_block_index:
				case Enums::_field_long_block_index:
					{
						auto* definition = field.Definition<tag_block_definition>();
						YELO_ASSERT_DISPLAY(definition, "no definition specified for block index field in block %s.",
							block->name); // NOTE: added owner block name to info
					}
					break;

				case Enums::_field_array_start:
					YELO_ASSERT( CAST_PTR(int32,field.definition)>0 );
					break;

				case Enums::_field_explanation:
					{
						cstring definition = field.Definition<const char>();
						YELO_ASSERT_DISPLAY(definition, "no definition specified for explanation field in block %s.",
							block->name); // NOTE: added owner block name to info
					}
					break;

				default:
					YELO_ASSERT( field.type>=0 && field.type<Enums::k_number_of_tag_field_types );
				}
			}
		}
		static void verify_group_field_definitions()
		{
			bool missing_header_definition = false;
			for(const tag_group* group = tag_group_get_next(); group; group = tag_group_get_next(group))
			{
				if(group->header_block_definition != nullptr)
					verify_block_field_definitions(group->header_block_definition);
				else
				{	// NOTE: added this warning
					YELO_WARN("tag group '%s' doesn't have a definition", group->name);
					missing_header_definition = true;
				}
			}

			if(missing_header_definition)
			{
				YELO_ASSERT( !"fix your goddamn tag groups" );
			}
		}

		static void build_group_parents()
		{
			for(tag_group* group = tag_group_get_next(); group; group = tag_group_get_next(group))
			{
				tag_group* parent = tag_group_get(group->parent_group_tag);
				for(tag_group* parent = tag_group_get(group->parent_group_tag); parent; parent = tag_group_get(parent->group_tag))
				{
					YELO_ASSERT( parent->child_count<Enums::k_maximum_children_per_tag );
					parent->child_group_tags[parent->child_count++] = group->group_tag;
				}
			}

			for(tag_group* group = tag_group_get_next(); group; group = tag_group_get_next(group))
			{
				if(group->parent_group_tag == NONE || group->child_count != 0)
					continue;

				// chain starts with the specific group, then increments to the last parent tag group
				struct {
					tag_group* group;
					int32 field_count;
				}tag_chain[Enums::k_maximum_tags_per_tag_chain];
				int32 tag_chain_size = 0;
				int32 total_field_count = 0;
				size_t total_header_size = 0;

				tag_group* parent_group = group;
				do {
					//YELO_ASSERT( parent_group->header_block_definition ); // don't assert here as this is checked

					auto scanner = TagGroups::c_tag_field_scanner(parent_group->header_block_definition->fields, nullptr)
						.ScanToEnd();

					total_field_count += 
						tag_chain[tag_chain_size].field_count = scanner.GetFieldIndex() - 1; // don't count the terminator
					total_header_size += 
						parent_group->header_block_definition->element_size;
					tag_chain[tag_chain_size].group = parent_group;

					tag_chain_size++;
					YELO_ASSERT( tag_chain_size<=Enums::k_maximum_tags_per_tag_chain );

				} while(tag_group_get(parent_group->parent_group_tag));

				// +1 for our terminator
				tag_field* combined_fields = YELO_NEW_ARRAY(tag_field, total_field_count+1);
				if(combined_fields == nullptr)
					continue; // TODO: this is what the engine does for out of memory...

				// copy the fields from the highest to lowest link in the chain
				tag_field* next_combined_field = combined_fields;
				for(int x = tag_chain_size-1; x >= 0; x--)
				{
					auto& link = tag_chain[tag_chain_size];
					// NOTE: ignore the warning C4996
					next_combined_field = std::copy_n(link.group->header_block_definition->fields, link.field_count,
						next_combined_field);
				}
				YELO_ASSERT( (next_combined_field-combined_fields)==total_field_count );

				memset(next_combined_field, 0, sizeof(*next_combined_field));
				next_combined_field->type = Enums::_field_terminator;

				group->header_block_definition->fields = combined_fields;
				group->header_block_definition->element_size = total_header_size;
			}
		}

		static void destroy_group_parents()
		{
			for(tag_group* group = tag_group_get_next(); group; group = tag_group_get_next(group))
			{
				if(group->parent_group_tag != NONE && group->child_count == 0)
				{
					YELO_DELETE_ARRAY(group->header_block_definition->fields);
					group->header_block_definition->fields = nullptr; // NOTE: engine didn't null this
				}
			}
		}

		static void build_byte_swap_codes_for_block_definition(tag_block_definition* block_definition)
		{
			if(block_definition->byte_swap_codes != nullptr)
				return;

			const tag_field* fields = block_definition->fields;
			int field_index = 0;

			// NOTE: we don't assert code_index since we're using std::array
			std::array<byte_swap_code_t, Enums::k_maximum_field_byte_swap_codes> codes = 
				{Enums::_bs_code_array_start, 1};
			int code_index = 2;

			bool has_children = false;

			do {
				const tag_field& field = fields[field_index];
				YELO_ASSERT( field.type>=0 && field.type<Enums::k_number_of_tag_field_types );

				for(byte_swap_code_t* field_codes = TagGroups::k_tag_field_definitions[field.type].byte_swap_codes;
					*field_codes != 0;
					field_codes++, code_index++)
				{
					codes[code_index] = *field_codes;
				}

				switch (field.type)
				{
				case Enums::_field_string: // NOTE: non-standard; added to support variable length string fields
					codes[code_index-1] = CAST(byte_swap_code_t, TagGroups::StringFieldGetSize(&field));
					break;

				case Enums::_field_array_start:
				case Enums::_field_array_end:
				case Enums::_field_pad:
					codes[code_index++] = CAST_PTR(int32, field.definition);
					break;

				case Enums::_field_block:
					// NOTE: this is already checked in verify_group_field_definitions
					//YELO_ASSERT_DISPLAY(field.definition, "element '%s' of block '%s' is NULL",
					//	field.name, block_definition->name);

					YELO_ASSERT(field.definition != block_definition); // a block shouldn't be able to have a child of itself, that would be infinite recursion

					build_byte_swap_codes_for_block_definition(field.Definition<tag_block_definition>());
					has_children = true;
					break;

				case Enums::_field_tag_reference:
				case Enums::_field_data:
					has_children = true;
					break;
				}

			} while(fields[field_index++].type != Enums::_field_terminator);

			if(has_children)
				SET_FLAG(block_definition->flags, Flags::_tag_block_has_children_bit, true);

			block_definition->byte_swap_codes = YELO_NEW_ARRAY(byte_swap_code_t, code_index);
			if(!block_definition->byte_swap_codes)
				YELO_WARN("couldn't allocate memory for tag block definition byte swapping codes (%s)",
					block_definition->name); // NOTE: added name to info

			std::copy_n(codes.data(), code_index, block_definition->byte_swap_codes);

			byte_swap_data_explicit(block_definition->name, block_definition->element_size, block_definition->byte_swap_codes,
				1, nullptr);
		}
		static void build_group_byte_swap_codes()
		{
			for(tag_group* group = tag_group_get_next(); group; group = tag_group_get_next(group))
				build_byte_swap_codes_for_block_definition(group->header_block_definition);
		}

		static void destroy_byte_swap_codes_for_block_definition(tag_block_definition* block_definition)
		{
			// NOTE: changed this check to come first instead of later, as all child blocks should have been taken care of if this is the case
			if(block_definition->byte_swap_codes == nullptr)
				return;

			const tag_field* fields = block_definition->fields;
			int field_index = 0;

			do {
				const tag_field& field = fields[field_index];
				YELO_ASSERT( field.type>=0 && field.type<Enums::k_number_of_tag_field_types );

				if(field.type == Enums::_field_block)
				{
					YELO_ASSERT(field.definition != block_definition);

					destroy_byte_swap_codes_for_block_definition(field.Definition<tag_block_definition>());
				}

			} while(fields[field_index++].type != Enums::_field_terminator);

			YELO_DELETE_ARRAY(block_definition->byte_swap_codes);
			block_definition->byte_swap_codes = nullptr;
		}
		static void destroy_group_byte_swap_codes()
		{
			for(tag_group* group = tag_group_get_next(); group; group = tag_group_get_next(group))
				destroy_byte_swap_codes_for_block_definition(group->header_block_definition);
		}

		void PLATFORM_API tag_groups_initialize()
		{
			TagGroups::group_tag_to_string group_string; group_string.Terminate();
			for(const tag_group* group = tag_group_get_next(); group; group = tag_group_get_next(group))
			{
				group_string.group = group->group_tag;
				YELO_ASSERT_DISPLAY(group==tag_group_get(group->group_tag), "there are two groups using the tag '%s'.",
					group_string.TagSwap().str);
			}
			verify_group_field_definitions();
			build_group_parents();
			build_group_byte_swap_codes();

			auto instance_data = CAST_PTR(Memory::s_data_array**, TagGroups::TagInstanceData());
			*instance_data = data_new<s_tag_instance, Enums::k_maximum_simultaneous_tag_instances_upgrade>("tag instance");
			if( *instance_data == nullptr )
			{
				tag_groups_dispose();
				YELO_ERROR(_error_message_priority_none, "couldn't allocate tag group globals");
			}
		}

		void PLATFORM_API tag_groups_dispose()
		{
			if( *TagGroups::TagInstanceData() != nullptr )
			{
				for(auto kv : TagGroups::TagInstances())
					tag_unload(kv.index);

				auto instance_data = CAST(Memory::s_data_array*, TagGroups::TagInstances());
				data_make_invalid(instance_data);
				data_dispose(instance_data);

				*TagGroups::TagInstanceData() = nullptr;
			}

			destroy_group_byte_swap_codes();
			destroy_group_parents();
		}

		bool PLATFORM_API tag_field_scan_impl(TagGroups::s_tag_field_scan_state& state)
		{
			YELO_ASSERT( !state.done );

			do {

			} while(true);
		}

		datum_index PLATFORM_API find_tag_instance(tag group_tag, cstring name)
		{
			for(auto instance : TagGroups::TagInstances())
			{
				if(instance->is_orphan || instance->is_reload || instance->group_tag != group_tag)
					continue;

				if(strcmp(instance->filename, name)==0)
					return instance.index;
			}
			return datum_index::null;
		}

		/*static*/ void PLATFORM_API tag_block_generate_default_element(const tag_block_definition *definition, void *address)
		{
			memset(address, 0, definition->element_size);

			for(auto field : TagGroups::c_tag_field_scanner(definition->fields, address)
				.AddFieldType(Enums::_field_short_block_index)
				.AddFieldType(Enums::_field_long_block_index) )
			{
				switch(field.GetType())
				{
				case Enums::_field_short_block_index:
					*field.As<int16>() = NONE;
					break;

				case Enums::_field_long_block_index:
					*field.As<int32>() = NONE;
					break;
				}
			}
		}

		void PLATFORM_API tag_reference_clear(tag_reference& reference)
		{
			// The engine's code will free (ie, YELO_FREE) the reference's name 
			// when tag_block_delete_element (which is called by tag_unload) is ran
//			void* ptr = YELO_MALLOC(Enums::k_max_tag_name_length+1, false); // TODO: check if NULL or memset instead?

//			reference.name = CAST_PTR(tag_reference_name_reference, ptr);
			YELO_ASSERT(reference.name);
			memset(reference.name, 0, Enums::k_max_tag_name_length+1);
			reference.name_length = 0;
			reference.group_tag = NONE;
			reference.tag_index = datum_index::null;
		}
	};

	// Frees the pointers used in more complex fields (tag_data, etc)
	// NOTE: currently doesn't clear the pointers as this is only used by tag_block_delete_element
	static void tag_block_delete_element_pointer_data(tag_block* block, int32 element_index)
	{
		auto* definition = block->definition;

		if(definition->dispose_element_proc != nullptr)
			definition->dispose_element_proc(block, element_index);

		for(auto field : TagGroups::c_tag_field_scanner(definition->fields, blam::tag_block_get_element(block, element_index))
			.AddFieldType(Enums::_field_block)
			.AddFieldType(Enums::_field_data)
			.AddFieldType(Enums::_field_tag_reference) )
		{
			switch(field.GetType())
			{
			case Enums::_field_data:
				YELO_FREE( field.As<tag_data>()->address );
				break;

			case Enums::_field_block:
				// engine actually does a while loop here, calling delete_element
				blam::tag_block_resize(field.As<tag_block>(), 0);
				break;

			case Enums::_field_tag_reference:
				YELO_FREE( field.As<tag_reference>()->name );
				break;

			YELO_ASSERT_CASE_UNREACHABLE();
			}
		}
	}
	static void PLATFORM_API tag_block_delete_element_impl(tag_block* block, int32 element_index)
	{
		YELO_ASSERT( block && block->definition );
		YELO_ASSERT( block->count>=0 );

		auto* definition = block->definition;
		YELO_ASSERT_DISPLAY(definition->fields != nullptr,
			"'%s' block has NULL fields", definition->name);

		tag_block_delete_element_pointer_data(block, element_index);

		// move up the elements that follow the deleted element
		int next_element_index = element_index + 1;
		if(next_element_index < block->count) // engine doesn't actually check if the deleted element is the last
		{
			size_t element_size = definition->element_size;
			size_t following_elements_offset = element_size * next_element_index;
			size_t following_elements_size = element_size * ((block->count - element_index) - 1);

			memmove(blam::tag_block_get_element(block, element_index), // elements will start consuming the memory at the deleted element
				CAST_PTR(byte*, block->address) + following_elements_offset,
				following_elements_size);
		}

		if(--block->count == 0) // free the elements and clear the pointer if that was the last element
		{
			YELO_FREE( block->address );
			block->address = nullptr;
		}
	}

	static int32 PLATFORM_API tag_block_add_element_impl(tag_block* block)
	{
		YELO_ASSERT( block && block->definition );

		auto* definition = block->definition;
		if(block->count >= definition->maximum_element_count)
			return NONE;

		void* new_address = YELO_REALLOC(block->address, (block->count+1) * definition->element_size);
		if(new_address == nullptr)
			return NONE;

		int add_index = block->count++;
		block->address = new_address;
		void* new_element = blam::tag_block_get_element(block, add_index);
		blam::tag_block_generate_default_element(definition, new_element);

		int32 dummy_position;
		bool success = blam::tag_block_read_children_recursive(definition, new_element, 1, &dummy_position,
			FLAG(Flags::_tag_load_for_editor_bit));

		if(!success)
		{
			--block->count;
			add_index = NONE;
		}

		return add_index;
	}

	static bool PLATFORM_API tag_block_resize_impl(tag_block* block, int32 element_count)
	{
		YELO_ASSERT( block && block->definition );
		YELO_ASSERT( block->count>=0 );

		YELO_ASSERT( element_count>=0 );

		// this is how resize is implemented in the engine. Hey, it handles both cases

		while(block->count > element_count)
			blam::tag_block_delete_element(block, block->count-1);

		while(block->count < element_count)
			if(blam::tag_block_add_element(block) == NONE)
				return false;

		return true;
	}

	static int32 PLATFORM_API tag_block_insert_element_impl(tag_block* block, int32 index)
	{
		YELO_ASSERT( block && block->definition ); // engine actually does asserts these after the allocation
		YELO_ASSERT( index>=0 && index<=block->count );

		auto* definition = block->definition;
		int proceeding_element_index = index + 1;

		// get a new element
		int add_index = blam::tag_block_add_element(block); // engine actually does this after the allocation
		if(add_index == NONE)
			return NONE;
		else if(proceeding_element_index == block->count)
			return add_index; // the element was inserted at the end, nothing needs to be done

		void* element = blam::tag_block_get_element(block, add_index);
		size_t element_size = definition->element_size;

		// copy the newly created element data to a temp buffer
		void* element_scratch = YELO_MALLOC(element_size, false);
		{
			if(element_scratch == nullptr)
				return NONE;

			memcpy(element_scratch, element, element_size);

			size_t following_elements_offset = element_size * index;
			size_t following_elements_size = element_size * ((block->count - index) - 1);
			size_t following_elements_new_offset = element_size * (index + 1);

			auto* address = CAST_PTR(byte*, block->address);
			memmove(address + following_elements_new_offset,
				address + following_elements_offset,
				following_elements_size);

			// copy the element we added for insertion. IE, insert it
			memcpy(blam::tag_block_get_element(block, index), element_scratch, element_size);
		}
		YELO_FREE(element_scratch);

		return index;
	}

	static bool tag_block_duplicate_element_recursive(
		tag_block *source_block, int32 source_element_index, 
		tag_block *destination_block, int32 destination_element_index)
	{
		YELO_ASSERT( source_block->definition==destination_block->address ); // engine doesn't actually do this first

		TagGroups::c_tag_field_scanner source(source_block->definition->fields, 
			blam::tag_block_get_element(source_block, source_element_index));
		source.AddAllFieldTypes();

		TagGroups::c_tag_field_scanner destination(destination_block->definition->fields, 
			blam::tag_block_get_element(destination_block, destination_element_index));
		destination.AddAllFieldTypes();

		while(source.Scan())
		{
			destination.Scan();
			YELO_ASSERT( source.GetTagFieldType()==destination.GetTagFieldType() );

			switch(source.GetTagFieldType())
			{
			default:
				memcpy(destination.GetFieldAddress(), source.GetFieldAddress(), source.GetFieldSize());
				break;

			case Enums::_field_data:
				{
					auto* dest_data = destination.GetFieldAs<tag_data>();
					auto* src_data = source.GetFieldAs<tag_data>();

					if(src_data->address != nullptr && blam::tag_data_resize(dest_data, src_data->size))
						memcpy(dest_data->address, src_data->address, src_data->size);
				}
				break;

			case Enums::_field_block:
				{
					auto* dest_block = destination.GetFieldAs<tag_block>();
					YELO_ASSERT( dest_block->count==0 );
					auto* src_block = source.GetFieldAs<tag_block>();

					if(blam::tag_block_resize(dest_block, src_block->count))
					{
						for(int x = 0; x < src_block->count; x++)
							tag_block_duplicate_element_recursive(src_block, x, destination_block, x);
					}
				}
				break;

			case Enums::_field_tag_reference:
				{
					auto* dest_ref = destination.GetFieldAs<tag_reference>();
					auto* src_ref = source.GetFieldAs<tag_reference>();

					blam::tag_reference_set(*dest_ref, src_ref->group_tag, src_ref->name);
					dest_ref->tag_index = src_ref->tag_index;
				}
				break;
			}
		}

		return true;
	}

	int32 PLATFORM_API tag_block_duplicate_element_impl(tag_block* block, int32 element_index)
	{
		int dup_index = blam::tag_block_add_element(block);
		if(dup_index != NONE)
		{
			tag_block_duplicate_element_recursive(
				block, element_index,
				block, dup_index);
		}

		return dup_index;
	}

	bool PLATFORM_API tag_block_swap_elements_impl(tag_block *block, int32 left_element_index, int32 right_element_index)
	{
		YELO_ASSERT( block && block->definition ); // engine actually does this after the allocation

		auto* definition = block->definition;
		size_t element_size = definition->element_size;

		void* element_scratch = YELO_MALLOC(element_size, false);
		{
			if(element_scratch == nullptr)
				return false;

			void* left_element = blam::tag_block_get_element(block, left_element_index);
			void* right_element= blam::tag_block_get_element(block, right_element_index);

			memcpy(element_scratch, left_element, element_size);
			memcpy(left_element, right_element, element_size);
			memcpy(right_element, element_scratch, element_size);
		}
		YELO_FREE(element_scratch);

		return true;
	}
};

#include <blamlib/Halo1/tag_files/tag_groups.field_definitions.inl>

#endif