/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "TagGroups/TagGroups.hpp"

#include <blamlib/Halo1/items/weapon_definitions.hpp>
#include <YeloLib/Halo1/tag_files/string_id_yelo.hpp>

namespace Yelo
{
#define __EL_INCLUDE_ID			__EL_INCLUDE_TAGGROUPS
#define __EL_INCLUDE_FILE_ID	__EL_TAGGROUPS_TAG_GROUPS
#include "Memory/_EngineLayout.inl"

	namespace TagGroups
	{
		tag_instance_data_t*	TagInstances()	DPTR_IMP_GET(tag_instance_data);

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
			tag_group* weapon_group = tag_group_get<s_weapon_definition>();

			// field the weapon's magazines field
			int32 field_index = tag_block_find_field(weapon_group->header_block_definition, Enums::_field_block, "magazines");
			if(field_index != NONE)
			{
				tag_block_definition* magazines_block = weapon_group->header_block_definition->fields[field_index].Definition<tag_block_definition>();

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

			// This allows us to create tags which have child groups (eg, object, shader, unit, etc)
			byte* child_count_assert_jmp_code = CAST_PTR(byte*, GET_FUNC_VPTR(TAG_NEW_MOD_CHILD_COUNT_ASSERT_JMP));
			// change from jz to jmp
			*child_count_assert_jmp_code = Enums::_x86_opcode_jmp_short;
		}

		int32 tag_block_find_field(const tag_block_definition* def, 
			_enum field_type, cstring name, int32 start_index)
		{
			YELO_ASSERT( def );
			YELO_ASSERT( def->fields );
			YELO_ASSERT( IN_RANGE_ENUM(field_type, Enums::k_number_of_tag_field_types) );
			YELO_ASSERT( name );

			if(start_index == NONE) start_index = 0;

			size_t name_length = strlen(name);

			for(const tag_field* cur = def->fields + start_index; cur->type != Enums::_field_terminator; cur++)
			{
				if(cur->type == field_type && !strncmp(cur->name, name, name_length))
					return cur - def->fields;
			}

			return NONE;
		}

		void tag_data_delete(tag_data* data, size_t terminator_size)
		{
			tag_data_resize(data, terminator_size);
		}

		bool tag_block_delete_all_elements(tag_block* block)
		{
			return tag_block_resize(block, 0);
		}

		API_FUNC_NAKED s_tag_field_scan_state& field_scan_state_new(s_tag_field_scan_state& state, const tag_field* fields, void* fields_address)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_FIELD_SCAN_STATE_NEW);

			API_FUNC_NAKED_START()
				push	fields_address
				push	fields
				push	state
				call	FUNCTION
			API_FUNC_NAKED_END_CDECL(3);
		}
		API_FUNC_NAKED void field_scan_state_add_field_type(s_tag_field_scan_state& state, Enums::field_type field_type)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_FIELD_SCAN_STATE_ADD_FIELD_TYPE);

			API_FUNC_NAKED_START()
				push	field_type
				push	state
				call	FUNCTION
			API_FUNC_NAKED_END_CDECL(2);
		}
		API_FUNC_NAKED bool field_scan(s_tag_field_scan_state& state)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_FIELD_SCAN);

			API_FUNC_NAKED_START()
				push	state
				call	FUNCTION
			API_FUNC_NAKED_END_CDECL(1);
		}

		c_tag_field_scanner::c_tag_field_scanner(const tag_field* fields, void* fields_address)
		{
			field_scan_state_new(m_state, fields, fields_address);
		}

		c_tag_field_scanner& c_tag_field_scanner::AddFieldType(Enums::field_type field_type)
		{
			field_scan_state_add_field_type(m_state, field_type);
			return *this;
		}

		void c_tag_field_scanner::AddAllFieldTypes()
		{
			memset(m_state.field_types, -1, sizeof(m_state.field_types));
		}

		bool c_tag_field_scanner::Scan()
		{
			return field_scan(m_state);
		}

		bool c_tag_field_scanner::TagFieldIsStringId() const
		{
			return	GetTagFieldType() == Enums::_field_tag_reference && 
					GetTagFieldDefinition<tag_reference_definition>()->group_tag == s_string_id_yelo_definition::k_group_tag;
		}
	};

	bool tag_is_read_only(datum_index tag_index)
	{
		if( tag_index.IsNull() ) return false;

		return (*TagGroups::TagInstances())[tag_index]->is_read_only;
	}

	size_t tag_block::get_element_size() const
	{
		return definition->element_size;
	}

	void tag_reference_clear(tag_reference& reference)
	{
		// The engine's code will free (ie, YELO_FREE) the reference's name 
		// when tag_block_delete_element (which is called by tag_unload) is ran
		void* ptr = YELO_MALLOC(Enums::k_max_tag_name_length+1, false);

		reference.name = CAST_PTR(tag_reference_name_reference, ptr);
		reference.name_length = 0;
		reference.group_tag = NONE;
		reference.tag_index = datum_index::null;
	}

	API_FUNC_NAKED bool tag_data_resize(tag_data* data, size_t new_size)
	{
		static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_DATA_RESIZE);

		API_FUNC_NAKED_START()
			push	new_size
			push	data
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(1);
	}

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

	API_FUNC_NAKED tag_group* tag_group_get(tag group_tag)
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
	API_FUNC_NAKED const void* tag_block_get_element(const tag_block* block, int32 element)
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

	API_FUNC_NAKED void tag_block_delete_element(tag_block* block, int32 element)
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

	API_FUNC_NAKED int32 tag_block_insert_element(tag_block* block, int32 index)
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

	API_FUNC_NAKED bool tag_file_read_only(tag group_tag, cstring name)
	{
		static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_FILE_READ_ONLY);

		API_FUNC_NAKED_START()
			push	name
			push	group_tag
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(2);
	}

	API_FUNC_NAKED bool tag_file_exists(tag group_tag, cstring name)
	{
		static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_FILE_EXISTS);

		API_FUNC_NAKED_START()
			push	name
			push	group_tag
			call	FUNCTION
		API_FUNC_NAKED_END_CDECL(2);
	}
};