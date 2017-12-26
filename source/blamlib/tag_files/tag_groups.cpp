/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <blamlib/tag_files/tag_groups.hpp>

#include <blamlib/models/model_definitions.hpp>
#include <blamlib/tag_files/tag_field_scanner.hpp>
#include <blamlib/tag_files/tag_group_loading.hpp>
#include <YeloLib/tag_files/string_id_yelo.hpp>
#include <YeloLib/tag_files/tag_group_memory.hpp>
#include <blamlib/tag_files/s_tag_iterator.h>
#include <blamlib/tag_files/tag_block.h>

namespace Yelo { namespace TagGroups
{
	#if PLATFORM_IS_EDITOR
	bool g_gbxmodel_group_enabled = true;

	int32 string_field_get_length(
		const tag_field* field)
	{
		assert(field->type == e_field_type::string);

		const auto definition = field->get_definition_as<uintptr_t>();

		if (definition == 0 || TagFieldIsOldStringId(field))
		{
			return Enums::k_tag_string_length;
		}
		// NOTE: the definition should have already been validated if tag_groups_initialize has already ran
		return CAST(int32, definition);
	}

	int32 string_field_get_size(
		const tag_field* field)
	{
		return string_field_get_length(field) + 1;
	}

	char* try_and_get_group_name(
		const tag group_tag,
		long_string name)
	{
		const tag_group* group = blam::tag_group_get(group_tag);
		if (group != nullptr)
		{
			strcpy(name, group->name);
		}
		else if (group_tag == NONE)
		{
			strcpy(name, "NONE");
		}
		else if (group_tag == 0)
		{
			strcpy(name, "ZERO");
		}
		else
		{
			strcpy(name, group_tag_to_string {group_tag}.ToString());
		}

		return name;
	}

	void tag_data_delete(
		tag_data* const data,
		const size_t terminator_size)
	{
		blam::tag_data_resize(data, terminator_size);
	}

	bool tag_block_delete_all_elements(
		tag_block* const block)
	{
		return block->count > 0
			? blam::tag_block_resize(block, 0)
			: true;
	}
	#endif
};

#if PLATFORM_IS_EDITOR
int32 tag_block_definition::find_field_index(
	_enum field_type,
	cstring name,
	int32 start_index) const
{
	YELO_ASSERT( this );
	YELO_ASSERT( this->fields );
	YELO_ASSERT( IN_RANGE_ENUM(field_type, e_field_type::k_count) );
	YELO_ASSERT( name );

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
	_enum field_type,
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

bool tag_reference_definition::s_group_tag_iterator::operator!=(
	const s_group_tag_iterator& other) const
{
	if (other.IsEndHack())
	{
		return *m_group_tags != NONE;
	}
	if (this->IsEndHack())
	{
		return *other.m_group_tags != NONE;
	}

	return m_group_tags != other.m_group_tags;
}

bool tag_block_definition::s_field_iterator::operator!=(
	const s_field_iterator& other) const
{
	if (other.IsEndHack())
	{
		return m_fields->type != e_field_type::terminator;
	}
	if (this->IsEndHack())
	{
		return other.m_fields->type != e_field_type::terminator;
	}

	return m_fields != other.m_fields;
}

int __cdecl tag_group::compare_by_name_proc(
	void*,
	const tag_group*const* lhs,
	const tag_group*const* rhs)
{
	return strcmp(
		(*lhs)->name,
		(*rhs)->name);
}

int __cdecl tag_group::compare_by_group_tag_proc(
	void*,
	const tag_group*const* lhs,
	const tag_group*const* rhs)
{
	return
		(*lhs)->group_tag -
		(*rhs)->group_tag;
}

int __cdecl tag_group::search_by_name_proc(
	void*,
	const cstring key,
	const tag_group* const* group)
{
	return strcmp(key, (*group)->name);
}

namespace blam
{
	datum_index PLATFORM_API find_tag_instance(
		const tag group_tag,
		const cstring name)
	{
		for (auto instance : TagGroups::TagInstances())
		{
			if (instance->is_orphan || instance->is_reload || instance->group_tag != group_tag)
			{
				continue;
			}

			if (strcmp(instance->filename, name) == 0)
			{
				return instance.index;
			}
		}
		return datum_index::null;
	}

	void* PLATFORM_API tag_get(
		const tag group_tag,
		const datum_index tag_index)
	{
		const auto instance = TagGroups::TagInstances()[tag_index];
		const auto instance_address = instance->root_block.address;
		const auto instance_group_tag = instance->group_tag;

		if (instance_group_tag == group_tag ||
			instance->parent_group_tags[0] == group_tag ||
			instance->parent_group_tags[1] == group_tag ||
			group_tag == NONE)
		{
			return instance_address;
		}

		if (TagGroups::g_gbxmodel_group_enabled &&
			TagGroups::model_definition::k_group_tag == instance_group_tag ||
			TagGroups::gbxmodel_definition::k_group_tag == instance_group_tag)
		{
			if (group_tag == TagGroups::model_definition::k_group_tag ||
				group_tag == TagGroups::gbxmodel_definition::k_group_tag)
			{
				return instance_address;
			}
		}

		long_string group_name;
		TagGroups::try_and_get_group_name(group_tag, group_name);
		long_string instance_group_name;
		TagGroups::try_and_get_group_name(instance_group_tag, instance_group_name);

		YELO_ASSERT_DISPLAY(false, "tag_get(0x%x) expected group '%s' but got group '%s'",
			tag_index, group_name, instance_group_name);
	}

	void PLATFORM_API tag_block_generate_default_element(
		const tag_block_definition* const definition,
		void* const address)
	{
		memset(address, 0, definition->element_size);

		for (auto field : TagGroups::c_tag_field_scanner(definition->fields, address)
		     .AddFieldType(e_field_type::short_block_index)
		     .AddFieldType(e_field_type::long_block_index))
		{
			switch (field.GetType())
			{
				case e_field_type::short_block_index:
					*field.As<int16>() = NONE;
					break;

				case e_field_type::long_block_index:
					*field.As<int32>() = NONE;
					break;

				default:
					break;
			}
		}
	}

	void PLATFORM_API tag_iterator_new(
		TagGroups::s_tag_iterator& iter,
		const tag group_tag_filter)
	{
		data_iterator_new(iter.instances_iterator, &TagGroups::TagInstances().Header);
		iter.group_tag_filter = group_tag_filter;
	}

	datum_index PLATFORM_API tag_iterator_next(
		TagGroups::s_tag_iterator& iter)
	{
		const auto group_tag_filter = iter.group_tag_filter;

		while (const void* datum = data_iterator_next(iter.instances_iterator))
		{
			const auto instance = CAST_PTR(const s_tag_instance*, datum);

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

	void PLATFORM_API tag_reference_clear(
		tag_reference& reference)
	{
		// The engine's code will free (ie, YELO_FREE) the reference's name 
		// when tag_block_delete_element (which is called by tag_unload) is ran

		YELO_ASSERT(reference.name);
		memset(reference.name, 0, Enums::k_max_tag_name_length + 1);
		reference.name_length = 0;
		reference.group_tag = NONE;
		reference.tag_index = datum_index::null;
	}

	void PLATFORM_API tag_reference_set(
		tag_reference& reference,
		const tag group_tag,
		const cstring name)
	{
		YELO_ASSERT( group_tag==NONE || tag_group_get(group_tag) );
		reference.group_tag = group_tag;

		auto name_length = strlen(name);
		YELO_ASSERT( name_length<=Enums::k_max_tag_name_length ); // NOTE: engine does '<', but I'm pretty sure we want '<='

		YELO_ASSERT( reference.name );
		if (reference.name != name)
		{
			strcpy(reference.name, name);
		}

		reference.name_length = name_length;
	}

	datum_index PLATFORM_API tag_reference_try_and_get(
		const tag_reference* const reference)
	{
		YELO_ASSERT(reference);

		auto loaded_tag_index = tag_loaded(reference->group_tag, reference->name);
		YELO_ASSERT_DISPLAY(reference->tag_index == loaded_tag_index,
			"tag reference \"%s\" and actual index do not match: is %08lX but should be %08lX",
			reference->name, reference->tag_index, loaded_tag_index);

		return loaded_tag_index;
	}

	bool PLATFORM_API tag_reference_resolve(
		tag_reference* const reference)
	{
		YELO_ASSERT(reference);

		auto success = false;
		if (reference->group_tag != NONE && !is_null_or_empty(reference->name))
		{
			reference->tag_index = tag_load(reference->group_tag, reference->name, 0);
			success = !reference->tag_index.IsNull();
		}
		else
		{
			reference->tag_index = datum_index::null;
		}

		return success;
	}

	bool tag_reference_resolve(
		tag_reference& reference,
		const tag expected_group_tag)
	{
		auto success = false;
		if (reference.group_tag == expected_group_tag && !is_null_or_empty(reference.name))
		{
			reference.tag_index = tag_load(reference.group_tag, reference.name, 0);
			success = !reference.tag_index.IsNull();
		}
		else
		{
			reference.tag_index = datum_index::null;
		}

		return success;
	}

	bool PLATFORM_API tag_data_resize(
		tag_data* const data,
		const int32 new_size)
	{
		YELO_ASSERT(data && data->definition);
		YELO_ASSERT(data->address);

		auto result = false;
		if (new_size < 0)
		{
			YELO_WARN("tried to resize a %s @%p to a negative size %d",
				data->definition->name, data, new_size);
		}
		else if (new_size > data->definition->maximum_size)
		{
			YELO_WARN("tried to resize a %s @%p to %d which is larger than the max allowed %d",
				data->definition->name, data, new_size, data->definition->maximum_size);
		}
		else if (new_size == 0)
		{
			data->size = 0;
			result = true;
		}
		else
		{
			TAG_DATA_REALLOC(*data, new_size);
			result = data->address != nullptr;
		}

		return result;
	}

	void* PLATFORM_API tag_data_get_pointer(
		tag_data& data,
		const int32 offset,
		const int32 size)
	{
		YELO_ASSERT(size >= 0);
		YELO_ASSERT(offset >= 0 && offset+size <= data.size);

		return data.Bytes() + offset;
	}

	// Frees the pointers used in more complex fields (tag_data, etc)
	static void tag_block_delete_element_pointer_data(
		tag_block* const block,
		const int32 element_index)
	{
		auto* definition = block->definition;

		if (definition->delete_proc != nullptr)
		{
			definition->delete_proc(block, element_index);
		}

		// NOTE: YELO_FREE/DELETE take the pointers by reference, so that it can NULL them in the process

		for (auto field : TagGroups::c_tag_field_scanner(definition->fields, tag_block_get_element(block, element_index))
		     .AddFieldType(e_field_type::block)
		     .AddFieldType(e_field_type::data)
		     .AddFieldType(e_field_type::tag_reference))
		{
			switch (field.GetType())
			{
				case e_field_type::data:
					TAG_DATA_DELETE( *field.As<tag_data>() );
					break;

				case e_field_type::block:
					// engine actually does a while loop here, calling delete_element
					tag_block_resize(field.As<tag_block>(), 0);
					break;

				case e_field_type::tag_reference:
					YELO_DELETE( field.As<tag_reference>()->name );
					break;

				YELO_ASSERT_CASE_UNREACHABLE();
			}
		}
	}

	void PLATFORM_API tag_block_delete_element(
		tag_block*const block,
		const int32 element_index)
	{
		YELO_ASSERT( block && block->definition );
		YELO_ASSERT( block->count>=0 );

		auto* definition = block->definition;
		YELO_ASSERT_DISPLAY(definition->fields != nullptr,
			"'%s' block has NULL fields", definition->name);

		tag_block_delete_element_pointer_data(block, element_index);

		// move up the elements that follow the deleted element
		const int next_element_index = element_index + 1;
		if (next_element_index < block->count) // engine doesn't actually check if the deleted element is the last
		{
			const auto element_size = definition->element_size;
			const auto following_elements_offset = element_size * next_element_index;
			const size_t following_elements_size = element_size * ((block->count - element_index) - 1);

			memmove(
				tag_block_get_element(block, element_index),
				// elements will start consuming the memory at the deleted element
				CAST_PTR(byte*, block->address) + following_elements_offset,
				following_elements_size);
		}

		if (--block->count == 0) // free the elements and clear the pointer if that was the last element
		{
			TAG_BLOCK_DELETE( *block );
		}
	}

	int32 PLATFORM_API tag_block_add_element(
		tag_block* const block)
	{
		YELO_ASSERT( block && block->definition );

		auto* definition = block->definition;
		if (block->count >= definition->maximum_element_count)
		{
			YELO_WARN("tried to add more elements for a %s @%p #%d than allowed",
				definition->name, block, block->count);
			return NONE;
		}

		int add_index = block->count++;
		const auto new_address = TAG_BLOCK_REALLOC(*block, block->count);
		if (new_address == nullptr)
		{
			YELO_WARN("failed to allocate new elements for a %s @%p #%d",
				definition->name, block, block->count);
			return NONE;
		}

		const auto new_element = tag_block_get_element(block, add_index);
		tag_block_generate_default_element(definition, new_element);

		int32 dummy_position;
		const auto success = tag_block_read_children_recursive(
			definition,
			new_element,
			1,
			&dummy_position,
			FLAG(Flags::_tag_load_for_editor_bit),
			datum_index::null);

		if (!success)
		{
			--block->count;
			add_index = NONE;
		}

		return add_index;
	}

	bool PLATFORM_API tag_block_resize(
		tag_block* const block,
		const int32 element_count)
	{
		YELO_ASSERT( block && block->definition );
		YELO_ASSERT( block->count>=0 );

		YELO_ASSERT( element_count>=0 );

		// this is how resize is implemented in the engine. Hey, it handles both cases

		while (block->count > element_count)
		{
			tag_block_delete_element(block, block->count - 1);
		}

		while (block->count < element_count)
		{
			if (tag_block_add_element(block) == NONE)
			{
				return false;
			}
		}

		return true;
	}
};

static int32 PLATFORM_API tag_block_insert_element_impl(
	tag_block* const block,
	const int32 index)
{
	YELO_ASSERT( block && block->definition ); // engine actually does the asserts these after the allocation
	YELO_ASSERT( index>=0 && index<=block->count );

	auto* definition = block->definition;
	const int proceeding_element_index = index + 1;

	// get a new element
	const int add_index = blam::tag_block_add_element(block); // engine actually does this after the allocation
	if (add_index == NONE)
	{
		return NONE;
	}
	if (proceeding_element_index == block->count)
	{
		return add_index;
	}
	// the element was inserted at the end, nothing needs to be done

	const auto element = blam::tag_block_get_element(block, add_index);
	const auto element_size = definition->element_size;

	// copy the newly created element data to a temp buffer
	auto element_scratch = YELO_MALLOC_UNIQUE(element_size, false);
	{
		if (element_scratch == nullptr)
		{
			return NONE;
		}

		memcpy(element_scratch.get(), element, element_size);

		const size_t following_elements_offset = element_size * index;
		const size_t following_elements_size = element_size * ((block->count - index) - 1);
		const size_t following_elements_new_offset = element_size * (index + 1);

		auto* address = CAST_PTR(byte*, block->address);
		memmove(
			address + following_elements_new_offset,
			address + following_elements_offset,
			following_elements_size);

		// copy the element we added for insertion. IE, insert it
		memcpy(blam::tag_block_get_element(block, index), element_scratch.get(), element_size);
	}

	return index;
}

static bool tag_block_duplicate_element_recursive(
	tag_block* const source_block,
	const int32 source_element_index,
	tag_block* const destination_block,
	const int32 destination_element_index)
{
	YELO_ASSERT( source_block->definition==destination_block->address ); // engine doesn't actually do this first

	TagGroups::c_tag_field_scanner source(
		source_block->definition->fields,
		blam::tag_block_get_element(source_block, source_element_index));
	source.AddAllFieldTypes();

	TagGroups::c_tag_field_scanner destination(
		destination_block->definition->fields,
		blam::tag_block_get_element(destination_block, destination_element_index));
	destination.AddAllFieldTypes();

	while (source.Scan())
	{
		destination.Scan();
		YELO_ASSERT( source.GetTagFieldType()==destination.GetTagFieldType() );

		switch (source.GetTagFieldType())
		{
			default:
				memcpy(destination.GetFieldAddress(), source.GetFieldAddress(), source.GetFieldSize());
				break;

			case e_field_type::data:
				{
					auto* dest_data = destination.GetFieldAs<tag_data>();
					auto* src_data = source.GetFieldAs<tag_data>();

					if (src_data->address != nullptr && blam::tag_data_resize(dest_data, src_data->size))
					{
						memcpy(dest_data->address, src_data->address, src_data->size);
					}
				}
				break;

			case e_field_type::block:
				{
					auto* dest_block = destination.GetFieldAs<tag_block>();
					YELO_ASSERT( dest_block->count==0 );
					auto* src_block = source.GetFieldAs<tag_block>();

					if (blam::tag_block_resize(dest_block, src_block->count))
					{
						for (int x = 0; x < src_block->count; x++)
						{
							tag_block_duplicate_element_recursive(src_block, x, destination_block, x);
						}
					}
				}
				break;

			case e_field_type::tag_reference:
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

int32 PLATFORM_API tag_block_duplicate_element_impl(
	tag_block* const block,
	const int32 element_index)
{
	const int dup_index = blam::tag_block_add_element(block);
	if (dup_index != NONE)
	{
		tag_block_duplicate_element_recursive(
			block,
			element_index,
			block,
			dup_index);
	}

	return dup_index;
}

bool PLATFORM_API tag_block_swap_elements_impl(
	tag_block* const block,
	const int32 left_element_index,
	const int32 right_element_index)
{
	YELO_ASSERT( block && block->definition ); // engine actually does this after the allocation

	auto* definition = block->definition;
	auto element_size = definition->element_size;

	auto element_scratch = YELO_MALLOC_UNIQUE(element_size, false);
	{
		if (element_scratch == nullptr)
		{
			return false;
		}

		const auto left_element = blam::tag_block_get_element(block, left_element_index);
		const auto right_element = blam::tag_block_get_element(block, right_element_index);

		memcpy(element_scratch.get(), left_element, element_size);
		memcpy(left_element, right_element, element_size);
		memcpy(right_element, element_scratch.get(), element_size);
	}

	return true;
}
#endif
};

#if PLATFORM_IS_EDITOR
namespace
{
	using namespace Yelo;
	using namespace Enums;

	byte_swap_code_t k_string_byte_swap_codes[] =
		{sizeof(tag_string), 0};

	byte_swap_code_t k_char_integer_byte_swap_codes[] =
		{_bs_code_1byte, 0};
	byte_swap_code_t k_short_integer_byte_swap_codes[] =
		{_bs_code_2byte, 0};
	byte_swap_code_t k_long_integer_byte_swap_codes[] =
		{_bs_code_4byte, 0};

	byte_swap_code_t k_point2d_byte_swap_codes[] =
		{_bs_code_2byte, _bs_code_2byte, 0};

	byte_swap_code_t k_rectangle2d_byte_swap_codes[] =
		{_bs_code_2byte, _bs_code_2byte, _bs_code_2byte, _bs_code_2byte, 0};

	byte_swap_code_t k_real_point2d_byte_swap_codes[] =
		{_bs_code_4byte, _bs_code_4byte, 0};

	byte_swap_code_t k_real_point3d_byte_swap_codes[] =
		{_bs_code_4byte, _bs_code_4byte, _bs_code_4byte, 0};

	byte_swap_code_t k_real_rgb_color_byte_swap_codes[] =
		{_bs_code_4byte, _bs_code_4byte, _bs_code_4byte, 0};

	byte_swap_code_t k_real_argb_color_byte_swap_codes[] =
		{_bs_code_4byte, _bs_code_4byte, _bs_code_4byte, _bs_code_4byte, 0};

	byte_swap_code_t k_real_quaternion_byte_swap_codes[] =
		{_bs_code_4byte, _bs_code_4byte, _bs_code_4byte, _bs_code_4byte, 0};

	byte_swap_code_t k_real_plane3d_byte_swap_codes[] =
		{_bs_code_4byte, _bs_code_4byte, _bs_code_4byte, _bs_code_4byte, 0};

	byte_swap_code_t k_tag_reference_byte_swap_codes[] =
		{_bs_code_4byte, sizeof(tag_reference_name_reference), _bs_code_4byte, sizeof(datum_index), 0};

	byte_swap_code_t k_tag_block_byte_swap_codes[] =
		{_bs_code_4byte, _bs_code_4byte, sizeof(tag_block_definition*), 0};

	byte_swap_code_t k_tag_data_byte_swap_codes[] =
		{_bs_code_4byte, sizeof(int32) * 2, sizeof(void*) * 2, 0};

	byte_swap_code_t k_start_array_byte_swap_codes[] =
		{_bs_code_array_start, 0};

	byte_swap_code_t k_end_array_byte_swap_codes[] =
		{_bs_code_array_end, 0};

	byte_swap_code_t k_pad_byte_swap_codes[] =
		{0};

	byte_swap_code_t k_explanation_byte_swap_codes[] =
		{0};

	byte_swap_code_t k_custom_byte_swap_codes[] =
		{0};

	byte_swap_code_t k_terminator_byte_swap_codes[] =
		{_bs_code_array_end, 0};
};

namespace Yelo { namespace TagGroups
{
	const s_tag_field_definition k_tag_field_definitions[] = {
		{
			sizeof(tag_string),
			"string",
			k_string_byte_swap_codes,
			"tag_string"
		},
		{
			sizeof(sbyte),
			"char integer",
			k_char_integer_byte_swap_codes,
			"byte"
		},
		{
			sizeof(int16),
			"short integer",
			k_short_integer_byte_swap_codes,
			"int16"
		},
		{
			sizeof(int32),
			"long integer",
			k_long_integer_byte_swap_codes,
			"int32"
		},
		{
			sizeof(angle),
			"angle",
			k_long_integer_byte_swap_codes,
			"angle"
		},
		{
			sizeof(tag),
			"tag",
			k_long_integer_byte_swap_codes,
			"tag"
		},
		{
			sizeof(_enum),
			"enum",
			k_short_integer_byte_swap_codes,
			"_enum"
		},
		{
			sizeof(long_flags),
			"long flags",
			k_long_integer_byte_swap_codes,
			"long_flags"
		},
		{
			sizeof(word_flags),
			"word flags",
			k_short_integer_byte_swap_codes,
			"word_flags"
		},
		{
			sizeof(byte_flags),
			"byte flags",
			k_char_integer_byte_swap_codes,
			"byte_flags"
		},
		{
			sizeof(point2d),
			"point 2d",
			k_point2d_byte_swap_codes,
			"point2d"
		},
		{
			sizeof(rectangle2d),
			"rectangle 2d",
			k_rectangle2d_byte_swap_codes,
			"rectangle2d"
		},
		{
			sizeof(rgb_color),
			"rgb color",
			k_long_integer_byte_swap_codes,
			"rgb_color"
		},
		{
			sizeof(argb_color),
			"argb color",
			k_long_integer_byte_swap_codes,
			"argb_color"
		},

		{
			sizeof(real),
			"real",
			k_long_integer_byte_swap_codes,
			"real"
		},
		{
			sizeof(real_fraction),
			"real fraction",
			k_long_integer_byte_swap_codes,
			"real_fraction"
		},
		{
			sizeof(real_point2d),
			"real point 2d",
			k_real_point2d_byte_swap_codes,
			"real_point2d"
		},
		{
			sizeof(real_point3d),
			"real point 3d",
			k_real_point3d_byte_swap_codes,
			"real_point3d"
		},
		{
			sizeof(real_vector2d),
			"real vector 2d",
			k_real_point2d_byte_swap_codes,
			"real_vector2d"
		},
		{
			sizeof(real_vector3d),
			"real vector 3d",
			k_real_point3d_byte_swap_codes,
			"real_vector3d"
		},
		{
			sizeof(real_quaternion),
			"real quaternion",
			k_real_quaternion_byte_swap_codes,
			"real_quaternion"
		},
		{
			sizeof(real_euler_angles2d),
			"real euler angles 2d",
			k_real_point2d_byte_swap_codes,
			"real_euler_angles2d"
		},
		{
			sizeof(real_euler_angles3d),
			"real euler angles 3d",
			k_real_point3d_byte_swap_codes,
			"real_euler_angles3d"
		},
		{
			sizeof(real_plane2d),
			"real plane 2d",
			k_real_point3d_byte_swap_codes,
			"real_plane2d"
		},
		{
			sizeof(real_plane3d),
			"real plane 3d",
			k_real_plane3d_byte_swap_codes,
			"real_plane3d"
		},
		{
			sizeof(real_rgb_color),
			"real rgb color",
			k_real_rgb_color_byte_swap_codes,
			"real_rgb_color"
		},
		{
			sizeof(real_argb_color),
			"real argb color",
			k_real_argb_color_byte_swap_codes,
			"real_argb_color"
		},

		{
			sizeof(real_rgb_color),
			"real hsv color",
			k_real_rgb_color_byte_swap_codes,
			/*"real_hsv_color"*/
		},
		{
			sizeof(real_argb_color),
			"real ahsv color",
			k_real_argb_color_byte_swap_codes,
			/*"real_ahsv_color"*/
		},

		{
			sizeof(short_bounds),
			"short integer bounds",
			k_point2d_byte_swap_codes,
			"short_bounds"
		},
		{
			sizeof(angle_bounds),
			"angle bounds",
			k_real_point2d_byte_swap_codes,
			"angle_bounds"
		},
		{
			sizeof(real_bounds),
			"real bounds",
			k_real_point2d_byte_swap_codes,
			"real_bounds"
		},
		{
			sizeof(real_fraction_bounds),
			"fraction bounds",
			k_real_point2d_byte_swap_codes,
			"real_fraction_bounds"
		},

		{
			sizeof(tag_reference),
			"tag reference",
			k_tag_reference_byte_swap_codes,
			"tag_reference"
		},
		{
			sizeof(tag_block),
			"block",
			k_tag_block_byte_swap_codes,
			"tag_block"
		},
		{
			sizeof(int16),
			"short block index",
			k_short_integer_byte_swap_codes,
			"int16"
		},
		{
			sizeof(int32),
			"long block index",
			k_long_integer_byte_swap_codes,
			"int32"
		},
		{
			sizeof(tag_data),
			"data",
			k_tag_data_byte_swap_codes,
			"tag_data"
		},

		{0, "array start", k_start_array_byte_swap_codes},
		{0, "array end", k_end_array_byte_swap_codes},
		{0, "pad", k_pad_byte_swap_codes},
		{0, "skip", k_pad_byte_swap_codes},
		{0, "explanation", k_explanation_byte_swap_codes},
		{0, "custom", k_custom_byte_swap_codes},
		{0, "terminator X", k_terminator_byte_swap_codes},

	};
	BOOST_STATIC_ASSERT(NUMBEROF(k_tag_field_definitions) == e_field_type::k_count);
}}
#endif
