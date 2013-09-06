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
		s_tag_field_definition k_tag_field_definitions[] = {
			{ sizeof(tag_string) },
			{ sizeof(sbyte) },
			{ sizeof(int16) },
			{ sizeof(int32) },
			{ sizeof(angle) },
			{ sizeof(tag) },
			{ sizeof(_enum) },
			{ sizeof(long_flags) },
			{ sizeof(word_flags) },
			{ sizeof(byte_flags) },
			{ sizeof(point2d) },
			{ sizeof(rectangle2d) },
			{ sizeof(rgb_color) },
			{ sizeof(argb_color) },

			{ sizeof(real) },
			{ sizeof(real_fraction) },
			{ sizeof(real_point2d) },
			{ sizeof(real_point3d) },
			{ sizeof(real_vector2d) },
			{ sizeof(real_vector3d) },
			{ sizeof(real_quaternion) },
			{ sizeof(real_euler_angles2d) },
			{ sizeof(real_euler_angles3d) },
			{ sizeof(real_plane2d) },
			{ sizeof(real_plane3d) },
			{ sizeof(real_rgb_color) },
			{ sizeof(real_argb_color) },

			{ sizeof(real_rgb_color) },
			{ sizeof(real_argb_color) },

			{ sizeof(short_bounds) },
			{ sizeof(angle_bounds) },
			{ sizeof(real_bounds) },
			{ sizeof(real_fraction_bounds) },

			{ sizeof(tag_reference) },
			{ sizeof(tag_block) },
			{ sizeof(int16) },
			{ sizeof(int32) },
			{ sizeof(tag_data) },

			{ 0 },
			{ 0 },
			{ 0 },
			{ 0 },
			{ 0 },
			{ 0 },
			{ 0 },
		};
		BOOST_STATIC_ASSERT( NUMBEROF(k_tag_field_definitions) == Enums::k_number_of_tag_field_types );

		void tag_data_delete(tag_data* data, size_t terminator_size)
		{
			blam::tag_data_resize(data, terminator_size);
		}

		bool tag_block_delete_all_elements(tag_block* block)
		{
			return blam::tag_block_resize(block, 0);
		}


		c_tag_field_scanner::c_tag_field_scanner(const tag_field* fields, void* fields_address)
		{
			blam::tag_field_scan_state_new(m_state, fields, fields_address);
		}

		c_tag_field_scanner& c_tag_field_scanner::AddFieldType(Enums::field_type field_type)
		{
			blam::tag_field_scan_state_add_field_type(m_state, field_type);
			return *this;
		}

		void c_tag_field_scanner::AddAllFieldTypes()
		{
			memset(m_state.field_types, -1, sizeof(m_state.field_types));
		}

		bool c_tag_field_scanner::Scan()
		{
			return blam::tag_field_scan(m_state);
		}

		bool c_tag_field_scanner::TagFieldIsStringId() const
		{
			return	GetTagFieldType() == Enums::_field_tag_reference && 
					GetTagFieldDefinition<tag_reference_definition>()->group_tag == s_string_id_yelo_definition::k_group_tag;
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

	namespace blam
	{
		static void tag_group_postprocess_instance(datum_index tag_index, Enums::tag_postprocess_mode mode)
		{
			for(const tag_group* group = tag_group_get(tag_get_group_tag(tag_index));
				group != nullptr;
				group = tag_group_get(group->parent_group_tag))
			{
				if(group->child_count > 0 && group->header_block_definition->postprocess_proc != nullptr)
				{
					group->header_block_definition->postprocess_proc(tag_get(NONE, tag_index), mode);
				}

				auto proc = group->postprocess_proc;
				if(proc != nullptr)
					proc(tag_index, mode);
			}
		}
		static datum_index find_tag_instance(tag group_tag, cstring name)
		{
			for(auto instance : TagGroups::TagInstances())
			{

			}
			return datum_index::null;
		}

		void PLATFORM_API tag_reference_clear(tag_reference& reference)
		{
			// The engine's code will free (ie, YELO_FREE) the reference's name 
			// when tag_block_delete_element (which is called by tag_unload) is ran
			void* ptr = YELO_MALLOC(Enums::k_max_tag_name_length+1, false); // TODO: check if NULL or memset instead?

			reference.name = CAST_PTR(tag_reference_name_reference, ptr);
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

		TagGroups::c_tag_field_scanner scanner(block->definition->fields, 
			blam::tag_block_get_element(block, element_index));

		scanner	.AddFieldType(Enums::_field_block)
				.AddFieldType(Enums::_field_data)
				.AddFieldType(Enums::_field_tag_reference)
			;

		while(scanner.Scan())
		{
			switch(scanner.GetTagFieldType())
			{
			case Enums::_field_data:
				YELO_FREE( scanner.GetFieldAs<tag_data>()->address );
				break;

			case Enums::_field_block:
				// engine actually does a while loop here, calling delete_element
				blam::tag_block_resize(scanner.GetFieldAs<tag_block>(), 0);
				break;

			case Enums::_field_tag_reference:
				YELO_FREE( scanner.GetFieldAs<tag_reference>()->name );
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

	static void tag_block_generate_default_element(const tag_block_definition *definition, void *address)
	{
		memset(address, 0, definition->element_size);

		TagGroups::c_tag_field_scanner scanner(definition->fields, 
			address);

		scanner	.AddFieldType(Enums::_field_short_block_index)
				.AddFieldType(Enums::_field_long_block_index)
			;

		while(scanner.Scan())
		{
			switch(scanner.GetTagFieldType())
			{
			case Enums::_field_short_block_index:
				*scanner.GetFieldAs<int16>() = NONE;
				break;

			case Enums::_field_long_block_index:
				*scanner.GetFieldAs<int32>() = NONE;
				break;
			}
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
		tag_block_generate_default_element(definition, new_element);

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
#endif