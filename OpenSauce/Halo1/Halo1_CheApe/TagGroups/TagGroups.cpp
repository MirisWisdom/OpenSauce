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
		tag_instance_data_t&	TagInstances()	DPTR_IMP_GET_BYREF(tag_instance_data);

		API_FUNC_NAKED void Initialize()
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_FILES_OPEN);

			__asm {
				call	FUNCTION
				retn
			}
		}

		static void InitializeFixesForWeaponGroup()
		{
			tag_group* weapon_group = blam::tag_group_get<s_weapon_definition>();

			// field the weapon's magazines field
			int32 field_index = weapon_group->header_block_definition->find_field_index(Enums::_field_block, "magazines");
			if(field_index != NONE)
			{
				auto* magazines_block = weapon_group->header_block_definition->fields[field_index].Definition<tag_block_definition>();

				// find the magazine's magazine-objects field
				field_index = magazines_block->find_field_index(Enums::_field_block, "magazines");
				if(field_index != NONE)
				{
					tag_field& magazine_objects_field = magazines_block->fields[field_index];
					magazine_objects_field.name = "magazine objects"; // give the field a more descriptive name

					auto* magazine_objects_block = magazine_objects_field.Definition<tag_block_definition>();
					magazine_objects_block->format_proc = nullptr; // Bungie seems to have made a copy&paste error and gave the objects block the format-magazines function

					// find the magazine-object's equipment reference field
					field_index = magazine_objects_block->find_field_index(Enums::_field_tag_reference, "equipment");
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


		API_FUNC_NAKED void tag_groups_set_model_upgrade_hack(BOOL hack_enabled)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_GROUPS_SET_MODEL_UPGRADE_HACK);

			API_FUNC_NAKED_START()
				push	hack_enabled
				call	FUNCTION
			API_FUNC_NAKED_END_CDECL(1);
		}
	};

	namespace blam
	{
		//////////////////////////////////////////////////////////////////////////
		// tag_files/tag_files
		API_FUNC_NAKED bool PLATFORM_API tag_file_read(int32 file_position, size_t buffer_size, void *buffer)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_FILE_READ);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED bool PLATFORM_API tag_file_read_only(tag group_tag, cstring name)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_FILE_READ_ONLY);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED bool PLATFORM_API tag_file_exists(tag group_tag, cstring name)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_FILE_EXISTS);

			__asm	jmp	FUNCTION
		}
		//////////////////////////////////////////////////////////////////////////
		// tag_files/tag_groups
		API_FUNC_NAKED tag PLATFORM_API tag_get_group_tag(datum_index tag_index)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_GET_GROUP_TAG);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED tag_block* PLATFORM_API tag_get_root_block(datum_index tag_index)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_GET_ROOT_BLOCK);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED cstring PLATFORM_API tag_get_name(datum_index tag_index)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_GET_NAME);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED void PLATFORM_API tag_orphan(datum_index tag_index)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_ORPHAN);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED datum_index PLATFORM_API tag_loaded(tag group_tag, cstring name)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_LOADED);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED bool PLATFORM_API tag_data_resize(tag_data* data, size_t new_size)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_DATA_RESIZE);

			__asm	jmp	FUNCTION
		}

		API_FUNC_NAKED TagGroups::s_tag_field_scan_state& PLATFORM_API tag_field_scan_state_new(TagGroups::s_tag_field_scan_state& state, 
			const tag_field* fields, void* fields_address)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_FIELD_SCAN_STATE_NEW);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED void PLATFORM_API tag_field_scan_state_add_field_type(TagGroups::s_tag_field_scan_state& state, 
			Enums::field_type field_type)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_FIELD_SCAN_STATE_ADD_FIELD_TYPE);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED bool PLATFORM_API tag_field_scan(TagGroups::s_tag_field_scan_state& state)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_FIELD_SCAN);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED bool PLATFORM_API tag_read_only(datum_index tag_index)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_READ_ONLY);

			__asm	jmp	FUNCTION
		}

		API_FUNC_NAKED void PLATFORM_API tag_iterator_new(TagGroups::s_tag_iterator& iter, const tag group_tag_filter)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_ITERATOR_NEW);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED datum_index PLATFORM_API tag_iterator_next(TagGroups::s_tag_iterator& iter)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_ITERATOR_NEXT);

			__asm	jmp	FUNCTION
		}


		API_FUNC_NAKED void* PLATFORM_API tag_get(tag group_tag, datum_index tag_index)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_GET);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED tag_group* PLATFORM_API tag_group_get(tag group_tag)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_GROUP_GET);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED void PLATFORM_API tag_rename(datum_index tag_index, cstring new_name)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_RENAME);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED tag_block* PLATFORM_API tag_block_index_resolve(datum_index tag_index, tag_field* block_index_field, int32 index)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_BLOCK_INDEX_RESOLVE);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED uint32 PLATFORM_API tag_size(datum_index tag_index)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_SIZE);

			__asm	jmp	FUNCTION
		}

		API_FUNC_NAKED void PLATFORM_API tag_reference_set(tag_reference& reference, tag group_tag, cstring name)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_REFERENCE_SET);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED uint32 PLATFORM_API tag_block_size(tag_block* block)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_BLOCK_SIZE);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED void* PLATFORM_API tag_block_get_element(tag_block* block, int32 element_index)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_BLOCK_GET_ELEMENT);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED const void* PLATFORM_API tag_block_get_element(const tag_block* block, int32 element_index)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_BLOCK_GET_ELEMENT);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED datum_index PLATFORM_API tag_new(tag group_name, cstring name)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_NEW);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED bool PLATFORM_API tag_save(datum_index tag_index)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_SAVE);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED void PLATFORM_API tag_block_delete_element(tag_block* block, int32 element_index)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_BLOCK_DELETE_ELEMENT);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED int32 PLATFORM_API tag_block_add_element(tag_block* block)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_BLOCK_ADD_ELEMENT);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED void PLATFORM_API tag_unload(datum_index tag_index)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_UNLOAD);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED bool PLATFORM_API tag_block_resize(tag_block* block, int32 element_count)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_BLOCK_RESIZE);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED int32 PLATFORM_API tag_block_insert_element(tag_block* block, int32 index)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_BLOCK_INSERT_ELEMENT);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED int32 PLATFORM_API tag_block_duplicate_element(tag_block* block, int32 element_index)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_BLOCK_DUPLICATE_ELEMENT);

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
				push	element_index
				push	block
				call	FUNCTION
				add		esp, 4 * 4

	fail:
				mov		eax, esi

				pop		esi
			API_FUNC_NAKED_END_()//(2);
		}

		API_FUNC_NAKED datum_index PLATFORM_API tag_load(tag group_tag, cstring name, long_flags file_flags)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_LOAD);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED datum_index PLATFORM_API tag_reload(tag group_tag, cstring name)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_RELOAD);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED void PLATFORM_API tag_load_children(datum_index tag_index)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_LOAD_CHILDREN);

			__asm	jmp	FUNCTION
		}
	};
};