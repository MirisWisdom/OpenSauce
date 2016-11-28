/*
    Yelo: Open Sauce SDK
		Halo 2 (Editing Kit) Edition

	See license\OpenSauce\Halo2_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "TagGroups/TagGroups.hpp"

#include <blamlib/Halo2/shaders/shader_definitions.hpp>
#include <blamlib/Halo2/sound/sound_definitions.hpp>

namespace Yelo
{
#define __EL_INCLUDE_ID			__EL_INCLUDE_TAGGROUPS
#define __EL_INCLUDE_FILE_ID	__EL_TAGGROUPS_TAG_GROUPS
#include "Memory/_EngineLayout.inl"

#include "TagGroups/TagGroups.PostprocessOverrides.inl"

	namespace TagGroups
	{
		void Initialize()
		{
			PostprocessOverridesInitialize();
		}

		void Dispose()
		{
			PostprocessOverridesDispose();
		}
	};

	size_t tag_block::get_element_size() const
	{
		return definition->field_set_latest->size;
	}

	API_FUNC_NAKED tag tag_get_group_tag(datum_index tag_index)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_GET_GROUP_TAG);

		API_FUNC_NAKED_START()
			push	tag_index
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(1);
	}

	API_FUNC_NAKED tag_block* tag_get_root_block(datum_index tag_index)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_GET_ROOT_BLOCK);

		API_FUNC_NAKED_START()
			push	tag_index
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(1);
	}

	API_FUNC_NAKED cstring tag_get_name(datum_index tag_index)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_GET_NAME);

		API_FUNC_NAKED_START()
			push	tag_index
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(1);
	}

	API_FUNC_NAKED bool tag_file_exists(tag group_tag, cstring name)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_FILE_EXISTS);

		API_FUNC_NAKED_START()
			push	name
			push	group_tag
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(2);
	}

	API_FUNC_NAKED void tag_block_clear(tag_block* block, tag_block_definition* definition)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_BLOCK_CLEAR);

		API_FUNC_NAKED_START()
			push	definition
			push	block
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(2);
	}

	API_FUNC_NAKED void tag_reference_clear(tag_reference& reference)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_REFERENCE_CLEAR);

		API_FUNC_NAKED_START()
			push	reference
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(1);
	}

	API_FUNC_NAKED void tag_reference_zero(tag_reference& reference)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_REFERENCE_ZERO);

		API_FUNC_NAKED_START()
			push	reference
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(1);
	}

	API_FUNC_NAKED void tag_reference_move(tag_reference& dst, tag_reference& src)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_REFERENCE_MOVE);

		API_FUNC_NAKED_START()
			push	src
			push	dst
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(2);
	}

	API_FUNC_NAKED bool tag_references_compare(const tag_reference& lhs, const tag_reference& rhs)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_REFERENCES_COMPARE);

		API_FUNC_NAKED_START()
			push	rhs
			push	lhs
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(2);
	}

	API_FUNC_NAKED bool tag_read_only(datum_index tag_index)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_IS_READ_ONLY);

		API_FUNC_NAKED_START()
			push	tag_index
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(1);
	}

	API_FUNC_NAKED void tag_block_move(tag_block* to_block, tag_block* from_block)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_BLOCK_MOVE);

		API_FUNC_NAKED_START()
			push	from_block
			push	to_block
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(2);
	}

	API_FUNC_NAKED void tag_reference_set(tag_reference& reference, tag group_tag, cstring name)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_REFERENCE_SET);

		API_FUNC_NAKED_START()
			push	name
			push	group_tag
			push	reference
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(3);
	}

	API_FUNC_NAKED void tag_reference_copy(const tag_reference& src, tag_reference& dst)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_REFERENCE_COPY);

		API_FUNC_NAKED_START()
			push	dst
			push	src
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(2);
	}

	API_FUNC_NAKED tag_group* tag_group_get_next(tag_group* start)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_GROUP_GET_NEXT);

		API_FUNC_NAKED_START()
			push	start
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(1);
	}

	API_FUNC_NAKED void* tag_data_get_pointer(tag_data& data, uint32 _offset)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_DATA_GET_POINTER);

		API_FUNC_NAKED_START()
			push	_offset
			push	data
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(2);
	}

	API_FUNC_NAKED char* string_id_get_string(string_id id, string_id_value out_value)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(STRING_ID_GET_STRING);

		API_FUNC_NAKED_START()
			push	out_value
			push	id
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(2);
	}

	API_FUNC_NAKED cstring string_id_get_value(string_id id)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(STRING_ID_GET_VALUE);

		API_FUNC_NAKED_START()
			push	id
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(1);
	}

	API_FUNC_NAKED void tag_block_delete_all(tag_block* block)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_BLOCK_DELETE_ALL);

		API_FUNC_NAKED_START()
			push	block
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(1);
	}

	API_FUNC_NAKED datum_index find_tag_instance(tag group_tag, cstring name, datum_index* out_index)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(FIND_TAG_INSTANCE);

		API_FUNC_NAKED_START()
			push	out_index
			mov		eax, name
			push	group_tag
			call	FUNCTION
			add		esp, 4 * 2
		API_FUNC_NAKED_END(3);
	}


	API_FUNC_NAKED tag_group* tag_group_get(tag group_tag)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_GROUP_GET);

		API_FUNC_NAKED_START()
			push	group_tag
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(1);
	}

	API_FUNC_NAKED void* tag_get(tag group_tag, datum_index tag_index)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_GET);

		API_FUNC_NAKED_START()
			push	tag_index
			push	group_tag
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(2);
	}

	API_FUNC_NAKED void tag_unload(datum_index tag_index)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_UNLOAD);

		API_FUNC_NAKED_START()
			push	tag_index
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(1);
	}

	API_FUNC_NAKED void tag_orphan(datum_index tag_index)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_ORPHAN);

		API_FUNC_NAKED_START()
			push	tag_index
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(1);
	}

	API_FUNC_NAKED bool tag_rename(datum_index tag_index, cstring new_name)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_RENAME);

		API_FUNC_NAKED_START()
			push	new_name
			push	tag_index
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(2);
	}

	API_FUNC_NAKED datum_index tag_loaded(tag group_tag, cstring name)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_LOADED);

		API_FUNC_NAKED_START()
			push	name
			push	group_tag
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(2);
	}

	API_FUNC_NAKED bool tag_data_resize(tag_data* data, size_t new_size)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_DATA_RESIZE);

		API_FUNC_NAKED_START()
			push	new_size
			push	data
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(2);
	}


	API_FUNC_NAKED datum_index tag_new(tag group_tag, cstring name)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_NEW);

		API_FUNC_NAKED_START()
			push	name
			push	group_tag
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(2);
	}

	API_FUNC_NAKED void* tag_block_get_element_with_size(tag_block* block, int32 element_index, size_t element_size)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_BLOCK_GET_ELEMENT_WITH_SIZE);

		API_FUNC_NAKED_START()
			push	element_size
			push	element_index
			push	block
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(3);
	}

	API_FUNC_NAKED bool tag_save(datum_index tag_index)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_SAVE);

		API_FUNC_NAKED_START()
			push	tag_index
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(1);
	}

	API_FUNC_NAKED void* tag_block_get_element(tag_block* block, int32 element_index)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_BLOCK_GET_ELEMENT);

		API_FUNC_NAKED_START()
			push	element_index
			push	block
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(2);
	}
	API_FUNC_NAKED const void* tag_block_get_element(const tag_block* block, int32 element_index)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_BLOCK_GET_ELEMENT);

		API_FUNC_NAKED_START()
			push	element_index
			push	block
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(2);
	}

	API_FUNC_NAKED datum_index tag_load(tag group_tag, cstring name, long_flags flags)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_LOAD);

		API_FUNC_NAKED_START()
			push	flags
			push	name
			push	group_tag
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(3);
	}

	API_FUNC_NAKED int32 tag_block_add_element(tag_block* block)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_BLOCK_ADD_ELEMENT);

		API_FUNC_NAKED_START()
			push	block
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(1);
	}

	API_FUNC_NAKED bool tag_reference_is_valid(const tag_reference& reference)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_REFERENCE_IS_VALID);

		API_FUNC_NAKED_START()
			push	reference
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(1);
	}

	API_FUNC_NAKED bool duplicate_recursive_tag_block(const tag_block* src_block, const int32 src_index, tag_block* dst_block, int32 dst_index)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(DUPLICATE_RECURSIVE_TAG_BLOCK);

		API_FUNC_NAKED_START()
			push	dst_index
			push	dst_block
			push	src_index
			push	src_block
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(4);
	}

	API_FUNC_NAKED datum_index tag_reload(tag group_tag, cstring name)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_RELOAD);

		API_FUNC_NAKED_START()
			push	name
			push	group_tag
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(2);
	}

	API_FUNC_NAKED bool tag_block_resize(tag_block* block, int32 new_count)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_BLOCK_RESIZE);

		API_FUNC_NAKED_START()
			push	new_count
			push	block
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(2);
	}

	API_FUNC_NAKED void tag_load_children(datum_index tag_index)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_LOAD_CHILDREN);

		API_FUNC_NAKED_START()
			push	tag_index
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(1);
	}

	API_FUNC_NAKED bool tag_block_copy(const tag_block* src_block, tag_block* dst_block)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_BLOCK_COPY);

		API_FUNC_NAKED_START()
			push	dst_block
			push	src_block
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(2);
	}

	API_FUNC_NAKED datum_index tag_copy_and_orphan(datum_index tag_index)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_COPY_AND_ORPHAN);

		API_FUNC_NAKED_START()
			push	tag_index
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(1);
	}

	API_FUNC_NAKED void tag_block_delete_element(tag_block* block, int32 element_index)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_BLOCK_DELETE_ELEMENT);

		API_FUNC_NAKED_START()
			push	element_index
			push	block
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(2);
	}
};