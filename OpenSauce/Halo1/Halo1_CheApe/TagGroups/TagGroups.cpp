/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "TagGroups/TagGroups.hpp"

#include <blamlib/Halo1/items/weapon_definitions.hpp>

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

		static void InitializeFixesForWeaponGroup()
		{
			tag_group_definition* weapon_group = tag_group_get<s_weapon_definition>();

			// field the weapon's magazines field
			int32 field_index = tag_block_find_field(weapon_group->definition, Enums::_field_block, "magazines");
			if(field_index != NONE)
			{
				tag_block_definition* magazines_block = weapon_group->definition->fields[field_index].Definition<tag_block_definition>();

				// find the magazine's magazine-objects field
				field_index = tag_block_find_field(magazines_block, Enums::_field_block, "magazines");
				if(field_index != NONE)
				{
					tag_field& magazine_objects_field = magazines_block->fields[field_index];
					magazine_objects_field.name = "magazine objects"; // give the field a more descriptive name

					tag_block_definition* magazine_objects_block = magazine_objects_field.Definition<tag_block_definition>();
					magazine_objects_block->format_proc = NULL; // Bungie seems to have made a copy&paste error and gave the objects block the format-magazines function

					// find the magazine-object's equipment reference field
					field_index = tag_block_find_field(magazine_objects_block, Enums::_field_tag_reference, "equipment");
					if(field_index != NONE)
					{
						tag_field& equipment_reference_field = magazine_objects_block->fields[field_index];
						equipment_reference_field.name = "equipment^"; // set the name to include markup for 'block name' (since we zapped the, incorrect, format function for this block)
					}
				}
			}
		}
		void InitializeFixes()
		{
			InitializeFixesForWeaponGroup();
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

		void tag_reference_clear(tag_reference& reference)
		{
			// The engine's code will free (ie, YELO_FREE) the reference's name 
			// when tag_block_delete_element (which is called by tag_unload) is ran
			void* ptr = YELO_MALLOC(Enums::k_max_tag_name_length+1, false);

			reference.name = CAST_PTR(tag_reference_name_t, ptr);
			reference.name_length = 0;
			reference.group_tag = NONE;
			reference.tag_index = datum_index::null;
		}
	};

	API_FUNC_NAKED cstring tag_get_name(datum_index tag_index)
	{
		static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_GET_NAME);

		API_FUNC_NAKED_START()
			push	tag_index
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

	API_FUNC_NAKED tag_group_definition* tag_group_get(tag group_tag)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_GROUP_GET);

		API_FUNC_NAKED_START()
			push	group_tag
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(1);
	}

	API_FUNC_NAKED void tag_rename(datum_index tag_index, cstring new_name)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_RENAME);

		API_FUNC_NAKED_START()
			push	new_name
			push	tag_index
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(2);
	}

	API_FUNC_NAKED tag_block* tag_block_index_resolve(datum_index tag_index, tag_field* block_index_field, int32 index)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_BLOCK_INDEX_RESOLVE);

		API_FUNC_NAKED_START()
			push	index
			push	block_index_field
			push	tag_index
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(3);
	}

	API_FUNC_NAKED uint32 tag_size(datum_index tag_index)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_SIZE);

		API_FUNC_NAKED_START()
			push	tag_index
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(1);
	}



	API_FUNC_NAKED int32 tag_reference_set(tag_reference* reference, tag group_tag, cstring name)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_REFERENCE_SET);

		API_FUNC_NAKED_START()
			push	name
			push	group_tag
			push	reference
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(3);
	}

	API_FUNC_NAKED uint32 tag_block_size(tag_block* block)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_BLOCK_SIZE);

		API_FUNC_NAKED_START()
			push	block
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(1);
	}

	API_FUNC_NAKED void* tag_block_get_element(tag_block* block, int32 element)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_BLOCK_GET_ELEMENT);

		API_FUNC_NAKED_START()
			push	element
			push	block
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(2);
	}

	API_FUNC_NAKED datum_index tag_new(tag group_name, cstring name)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_NEW);

		API_FUNC_NAKED_START()
			push	name
			push	group_name
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(2);
	}

	API_FUNC_NAKED bool tag_save(datum_index tag_index)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_SAVE);

		API_FUNC_NAKED_START()
			push	tag_index
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(1);
	}

	API_FUNC_NAKED void tag_block_delete_element(void* block, int32 element)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_BLOCK_DELETE_ELEMENT);

		API_FUNC_NAKED_START()
			push	element
			push	block
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(2);
	}

	API_FUNC_NAKED int32 tag_block_add_element(tag_block* block)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_BLOCK_ADD_ELEMENT);

		API_FUNC_NAKED_START()
			push	block
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(1);
	}

	API_FUNC_NAKED bool tag_block_resize(tag_block* block, int32 element_count)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_BLOCK_RESIZE);

		API_FUNC_NAKED_START()
			push	element_count
			push	block
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(2);
	}

	API_FUNC_NAKED void* tag_block_insert_element(tag_block* block, int32 index)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_BLOCK_INSERT_ELEMENT);

		API_FUNC_NAKED_START()
			push	index
			push	block
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(2);
	}

	API_FUNC_NAKED int32 tag_block_duplicate_element(tag_block* block, int32 element)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_BLOCK_DUPLICATE_ELEMENT);

		API_FUNC_NAKED_START()
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
		API_FUNC_NAKED_END(2);
	}

	API_FUNC_NAKED datum_index tag_load(tag group_tag, cstring name, long_flags file_flags)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_LOAD);

		API_FUNC_NAKED_START()
			push	file_flags
			push	name
			push	group_tag
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(3);
	}

	API_FUNC_NAKED void tag_unload(datum_index tag_index)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_UNLOAD);

		API_FUNC_NAKED_START()
			push	tag_index
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(1);
	}


	API_FUNC_NAKED void tag_iterator_new(TagGroups::tag_iterator& iter, const tag group_tag_filter)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_ITERATOR_NEW);

		API_FUNC_NAKED_START()
			push	group_tag_filter
			push	iter
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(2);
	}

	API_FUNC_NAKED datum_index tag_iterator_next(TagGroups::tag_iterator& iter)
	{
		static const uint32 FUNCTION = GET_FUNC_PTR(TAG_ITERATOR_NEXT);

		API_FUNC_NAKED_START()
			push	iter
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(1);
	}
};