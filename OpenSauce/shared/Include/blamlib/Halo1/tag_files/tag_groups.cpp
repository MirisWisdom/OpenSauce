/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <blamlib/Halo1/tag_files/tag_groups.hpp>

#if PLATFORM_IS_EDITOR
	#include <blamlib/Halo1/tag_files/tag_group_loading.hpp>
	#include <YeloLib/Halo1/tag_files/string_id_yelo.hpp>

	#include "Engine/EngineFunctions.hpp"

namespace Yelo
{
	namespace TagGroups
	{
		int32 StringFieldGetLength(const tag_field* field)
		{
			assert(field->type == Enums::_field_string);

			uintptr_t definition = field->DefinitionCast<uintptr_t>();

			if(definition == 0 || TagFieldIsOldStringId(field))
				return Enums::k_tag_string_length;
			else // NOTE: the definition should have already been validated if tag_groups_initialize has already ran
				return CAST(int32, definition);
		}
		int32 StringFieldGetSize(const tag_field* field)
		{
			return StringFieldGetLength(field)+1;
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
			//std::memset(&m_state, 0, sizeof(m_state));
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

		bool c_tag_iterator::operator!=(const c_tag_iterator& other) const
		{
			// NOTE: we intentionally don't compare the group_tag filter
			return m_state.instances_iterator != other.m_state.instances_iterator;
		}
	};

	size_t tag_block::get_element_size() const
	{
		return definition->element_size;
	}

	size_t tag_field::GetSize(_Out_opt_ size_t* runtime_size) const
	{
		size_t field_size;

		switch(type)
		{
		case Enums::_field_string:
			field_size = TagGroups::StringFieldGetSize(this);
			break;

		case Enums::_field_tag_reference:
			field_size = CAST(int, TagGroups::k_tag_field_definitions[Enums::_field_tag_reference].size);

			if(runtime_size && TagGroups::TagFieldIsStringId(this))
				*runtime_size = field_size - string_id_yelo::k_debug_data_size;
			break;

		case Enums::_field_pad:
		case Enums::_field_skip:
			field_size = DefinitionCast<int32>();
			break;

		default:
			field_size = TagGroups::k_tag_field_definitions[type].size;
			break;
		}

		return field_size;
	}

	bool tag_field::IsReadOnly() const
	{
		return name && strchr(name, Enums::k_tag_field_markup_character_read_only); // NOTE: engine uses strrchr
	}
	bool tag_field::IsAdvanced() const
	{
		return name && strchr(name, Enums::k_tag_field_markup_character_advanced); // NOTE: engine uses strrchr
	}
	bool tag_field::IsBlockName() const
	{
		return name && strchr(name, Enums::k_tag_field_markup_character_block_name); // NOTE: engine uses strrchr
	}
	bool tag_field::IsInvisible() const
	{
		return name && strlen(name) == 0; // yes, a field with no name wouldn't be considered 'invisible', according to engine code
	}

	int32 tag_block_definition::FindFieldIndex(_enum field_type, cstring name, int32 start_index) const
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

	int __cdecl tag_group::QsortCompareByName(void*, const void* lhs, const void* rhs)
	{
		return strcmp(
			(*CAST_PTR(const tag_group*const*,lhs))->name, 
			(*CAST_PTR(const tag_group*const*,rhs))->name);
	}
	int __cdecl tag_group::QsortCompareByGroupTag(void*, const void* lhs, const void* rhs)
	{
		return
			(*CAST_PTR(const tag_group*const*,lhs))->group_tag - 
			(*CAST_PTR(const tag_group*const*,rhs))->group_tag;
	}

	namespace blam
	{
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

		void PLATFORM_API tag_iterator_new(TagGroups::s_tag_iterator& iter, const tag group_tag_filter)
		{
			data_iterator_new(iter.instances_iterator, &TagGroups::TagInstances().Header);
			iter.group_tag_filter = group_tag_filter;
		}

		datum_index PLATFORM_API tag_iterator_next(TagGroups::s_tag_iterator& iter)
		{
			tag group_tag_filter = iter.group_tag_filter;

			while (const void* datum = data_iterator_next(iter.instances_iterator))
			{
				auto instance = CAST_PTR(const s_tag_instance*, datum);

				if (group_tag_filter == NONE ||
					instance->group_tag == group_tag_filter ||
					instance->parent_group_tags[0] == group_tag_filter ||
					instance->parent_group_tags[1] == group_tag_filter)
				{
					return iter.instances_iterator.index;
				}
			}

			return datum_index::null;
		}


		void PLATFORM_API tag_reference_clear(tag_reference& reference)
		{
			// The engine's code will free (ie, YELO_FREE) the reference's name 
			// when tag_block_delete_element (which is called by tag_unload) is ran
//			void* ptr = YELO_MALLOC(Enums::k_max_tag_name_length+1, false); // TODO: check if NULL or memset instead?

//			reference.name = CAST_PTR(tag_reference_name_reference, ptr);
			YELO_ASSERT(reference.name);
			std::memset(reference.name, 0, Enums::k_max_tag_name_length+1);
			reference.name_length = 0;
			reference.group_tag = NONE;
			reference.tag_index = datum_index::null;
		}

		void PLATFORM_API tag_reference_set_impl(tag_reference& reference, tag group_tag, cstring name)
		{
			YELO_ASSERT( group_tag==NONE || tag_group_get(group_tag) );
			reference.group_tag = group_tag;

			size_t name_length = strlen(name);
			YELO_ASSERT( name_length<=Enums::k_max_tag_name_length ); // NOTE: engine does '<', but I'm pretty sure we want '<='

			YELO_ASSERT( reference.name );
			if(reference.name != name)
				strcpy(reference.name, name);

			reference.name_length = name_length;
		}
	};

	// Frees the pointers used in more complex fields (tag_data, etc)
	static void tag_block_delete_element_pointer_data(tag_block* block, int32 element_index)
	{
		auto* definition = block->definition;

		if( definition->delete_proc != nullptr )
			definition->delete_proc(block, element_index);

		// NOTE: YELO_FREE/DELETE take the pointers by reference, so that it can NULL them in the process

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
				YELO_DELETE( field.As<tag_reference>()->name );
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
		YELO_ASSERT( block && block->definition ); // engine actually does the asserts these after the allocation
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
		auto element_scratch = YELO_MALLOC_UNIQUE(element_size, false);
		{
			if(element_scratch == nullptr)
				return NONE;

			std::memcpy(element_scratch.get(), element, element_size);

			size_t following_elements_offset = element_size * index;
			size_t following_elements_size = element_size * ((block->count - index) - 1);
			size_t following_elements_new_offset = element_size * (index + 1);

			auto* address = CAST_PTR(byte*, block->address);
			std::memmove(address + following_elements_new_offset,
				address + following_elements_offset,
				following_elements_size);

			// copy the element we added for insertion. IE, insert it
			std::memcpy(blam::tag_block_get_element(block, index), element_scratch.get(), element_size);
		}

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
				std::memcpy(destination.GetFieldAddress(), source.GetFieldAddress(), source.GetFieldSize());
				break;

			case Enums::_field_data:
				{
					auto* dest_data = destination.GetFieldAs<tag_data>();
					auto* src_data = source.GetFieldAs<tag_data>();

					if(src_data->address != nullptr && blam::tag_data_resize(dest_data, src_data->size))
						std::memcpy(dest_data->address, src_data->address, src_data->size);
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

		auto element_scratch = YELO_MALLOC_UNIQUE(element_size, false);
		{
			if(element_scratch == nullptr)
				return false;

			void* left_element = blam::tag_block_get_element(block, left_element_index);
			void* right_element= blam::tag_block_get_element(block, right_element_index);

			std::memcpy(element_scratch.get(), left_element, element_size);
			std::memcpy(left_element, right_element, element_size);
			std::memcpy(right_element, element_scratch.get(), element_size);
		}

		return true;
	}
};

#include <blamlib/Halo1/tag_files/tag_groups.field_definitions.inl>

#endif