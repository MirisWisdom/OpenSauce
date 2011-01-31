/*
    Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition
    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "Common/Precompile.hpp"
#include "TagGroups/TagGroups.hpp"

namespace Yelo
{
#define __EL_INCLUDE_ID			__EL_INCLUDE_TAGGROUPS
#define __EL_INCLUDE_FILE_ID	__EL_TAGGROUPS_TAG_GROUPS
#include "Memory/_EngineLayout.inl"

	namespace TagGroups
	{
		t_tag_instance_data*	TagInstances()	DPTR_IMP_GET(tag_instance_data);

		API_FUNC_NAKED void Initialize()
		{
			static const uint32 FUNCTION = GET_FUNC_PTR(TAG_FILES_OPEN);

			__asm {
				call	FUNCTION
				retn
			}
		}

		int32 tag_block_find_field(const tag_block_definition* def, 
			_enum field_type, cstring name, int32 start_index)
		{
			YELO_ASSERT( def );
			YELO_ASSERT( def->fields );
			YELO_ASSERT( IN_RANGE_ENUM(field_type, Enums::_field_type) );
			YELO_ASSERT( name );

			if(start_index == NONE) start_index = 0;

			size_t name_length = strlen(name);

			for(const tag_field* cur = def->fields + start_index; cur->field_type != Enums::_field_terminator; cur++)
			{
				if(cur->field_type == field_type && !strncmp(cur->name, name, name_length))
					return cur - def->fields;
			}

			return NONE;
		}
	};

	API_FUNC_NAKED void* tag_get(tag group_tag, datum_index tag_index)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_GET);

		NAKED_FUNC_START()
			push	tag_index
			push	group_tag
			call	FUNCTION
		NAKED_FUNC_END_CDECL(2);
	}

	API_FUNC_NAKED tag_group_definition* tag_group_get(tag group_tag)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_GROUP_GET);

		NAKED_FUNC_START()
			push	group_tag
			call	FUNCTION
		NAKED_FUNC_END_CDECL(1);
	}

	API_FUNC_NAKED void tag_rename(datum_index tag_index, cstring new_name)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_RENAME);

		NAKED_FUNC_START()
			push	new_name
			push	tag_index
			call	FUNCTION
		NAKED_FUNC_END_CDECL(2);
	}

	API_FUNC_NAKED tag_block* tag_block_index_resolve(datum_index tag_index, tag_field* block_index_field, int32 index)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_BLOCK_INDEX_RESOLVE);

		NAKED_FUNC_START()
			push	index
			push	block_index_field
			push	tag_index
			call	FUNCTION
		NAKED_FUNC_END_CDECL(3);
	}

	API_FUNC_NAKED uint32 tag_size(datum_index tag_index)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_SIZE);

		NAKED_FUNC_START()
			push	tag_index
			call	FUNCTION
		NAKED_FUNC_END_CDECL(1);
	}



	API_FUNC_NAKED int32 tag_reference_set(tag_reference* reference, tag group_tag, cstring name)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_REFERENCE_SET);

		NAKED_FUNC_START()
			push	name
			push	group_tag
			push	reference
			call	FUNCTION
		NAKED_FUNC_END_CDECL(3);
	}

	API_FUNC_NAKED uint32 tag_block_size(tag_block* block)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_BLOCK_SIZE);

		NAKED_FUNC_START()
			push	block
			call	FUNCTION
		NAKED_FUNC_END_CDECL(1);
	}

	API_FUNC_NAKED void* tag_block_get_element(tag_block* block, int32 element)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_BLOCK_GET_ELEMENT);

		NAKED_FUNC_START()
			push	element
			push	block
			call	FUNCTION
		NAKED_FUNC_END_CDECL(2);
	}

	API_FUNC_NAKED datum_index tag_new(tag group_name, cstring name)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_NEW);

		NAKED_FUNC_START()
			push	name
			push	group_name
			call	FUNCTION
		NAKED_FUNC_END_CDECL(2);
	}

	API_FUNC_NAKED bool tag_save(datum_index tag_index)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_SAVE);

		NAKED_FUNC_START()
			push	tag_index
			call	FUNCTION
		NAKED_FUNC_END_CDECL(1);
	}

	API_FUNC_NAKED void tag_block_delete_element(void* block, int32 element)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_BLOCK_DELETE_ELEMENT);

		NAKED_FUNC_START()
			push	element
			push	block
			call	FUNCTION
		NAKED_FUNC_END_CDECL(2);
	}

	API_FUNC_NAKED int32 tag_block_add_element(tag_block* block)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_BLOCK_ADD_ELEMENT);

		NAKED_FUNC_START()
			push	block
			call	FUNCTION
		NAKED_FUNC_END_CDECL(1);
	}

	API_FUNC_NAKED bool tag_block_resize(tag_block* block, int32 element_count)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_BLOCK_RESIZE);

		NAKED_FUNC_START()
			push	element_count
			push	block
			call	FUNCTION
		NAKED_FUNC_END_CDECL(2);
	}

	API_FUNC_NAKED void* tag_block_insert_element(tag_block* block, int32 index)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_BLOCK_INSERT_ELEMENT);

		NAKED_FUNC_START()
			push	index
			push	block
			call	FUNCTION
		NAKED_FUNC_END_CDECL(2);
	}

	API_FUNC_NAKED int32 tag_block_duplicate_element(tag_block* block, int32 element)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_BLOCK_DUPLICATE_ELEMENT);

		NAKED_FUNC_START()
			push	esi
			
			push	block
			call	tag_block_add_element
			add		esp, 4 * 1
			mov		esi, eax
			cmp		esi, NULL_HANDLE
			jz		fail

			push	esi
			push	block
			push	element
			push	block
			call	FUNCTION
			add		esp, 4 * 4

fail:
			mov		eax, esi

			pop		esi
		NAKED_FUNC_END(2);
	}

	API_FUNC_NAKED datum_index tag_load(tag group_tag, cstring name, long_flags file_flags)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_LOAD);

		NAKED_FUNC_START()
			push	file_flags
			push	name
			push	group_tag
			call	FUNCTION
		NAKED_FUNC_END_CDECL(3);
	}

	API_FUNC_NAKED void tag_unload(datum_index tag_index)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_UNLOAD);

		NAKED_FUNC_START()
			push	tag_index
			call	FUNCTION
			add		esp, 4 * 1
		NAKED_FUNC_END(1);
	}


	API_FUNC_NAKED void tag_iterator_new(TagGroups::tag_iterator& iter, const tag group_tag)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_ITERATOR_NEW);

		NAKED_FUNC_START()
			push	group_tag
			push	iter
			call	FUNCTION
			add		esp, 4 * 2
		NAKED_FUNC_END(2);
	}

	API_FUNC_NAKED datum_index tag_iterator_next(TagGroups::tag_iterator& iter)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_ITERATOR_NEXT);

		NAKED_FUNC_START()
			push	iter
			call	FUNCTION
			add		esp, 4 * 1
		NAKED_FUNC_END(1);
	}
};