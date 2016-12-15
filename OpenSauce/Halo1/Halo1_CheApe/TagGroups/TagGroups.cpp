/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "TagGroups/TagGroups.hpp"

#include <blamlib/Halo1/items/weapon_definitions.hpp>
#include <blamlib/Halo1/shaders/shader_definitions.hpp>
#include <blamlib/Halo1/tag_files/tag_field_scanner.hpp>
#include <blamlib/Halo1/tag_files/tag_files.hpp>
#include <blamlib/Halo1/tag_files/tag_files_structures.hpp> // need structs in order to do pointer math on TagFileGlobals
#include <blamlib/Halo1/tag_files/tag_group_loading.hpp>

namespace Yelo
{
#define __EL_INCLUDE_ID			__EL_INCLUDE_TAGGROUPS
#define __EL_INCLUDE_FILE_ID	__EL_TAGGROUPS_TAG_GROUPS
#include "Memory/_EngineLayout.inl"

	namespace TagGroups
	{
		tag_instance_data_t**	TagInstanceData()	DPTR_IMP_GET2(tag_instance_data);
		tag_instance_data_t&	TagInstances()		DPTR_IMP_GET_BYREF(tag_instance_data);

		s_tag_file_globals* TagFileGlobalsThreaded()			{ return TagFileGlobals() - 1; } // threaded globals actually appears in memory right before regular
		s_tag_file_globals* TagFileGlobals()					PTR_IMP_GET2(tag_file_globals);

		Memory::s_byte_swap_definition* TagHeaderBsDefinition()	PTR_IMP_GET2(tag_header_bs_definition);

		// Patches engine code to call our hooks or our implementations of various functions
		static void InitializeHooks()
		{
////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Glue for hooking an overloaded function. </summary>
///
/// <param name="blam_name">	Name of the function as it appears in the blam namespace. </param>
/// <param name="func_name">	Name of the FUNC_PTR. </param>
/// <param name="ret_type"> 	blam function's return type. </param>
#define INIT_HOOK_BY_EXPLICIT_TYPE(blam_name, func_name, ret_type, ...) \
	ret_type (PLATFORM_API* blam__##blam_name)(__VA_ARGS__) =			\
		blam::blam_name;												\
		Memory::WriteRelativeJmp(blam__##blam_name, GET_FUNC_VPTR(func_name), true);

			Memory::WriteRelativeJmp(blam::tag_groups_initialize, GET_FUNC_VPTR(TAG_GROUPS_INITIALIZE), true);
			Memory::WriteRelativeJmp(blam::tag_groups_dispose, GET_FUNC_VPTR(TAG_GROUPS_DISPOSE), true);
			Memory::WriteRelativeJmp(blam::tag_field_scan, GET_FUNC_VPTR(TAG_FIELD_SCAN), true);

			INIT_HOOK_BY_EXPLICIT_TYPE(tag_iterator_new, TAG_ITERATOR_NEW,
				void, s_tag_iterator&, tag);
			Memory::WriteRelativeJmp(blam::tag_iterator_next, GET_FUNC_VPTR(TAG_ITERATOR_NEXT), true);

			INIT_HOOK_BY_EXPLICIT_TYPE(tag_reference_set, TAG_REFERENCE_SET,
				void, tag_reference&, tag, cstring);

			Memory::WriteRelativeJmp(blam::tag_data_load, GET_FUNC_VPTR(TAG_DATA_LOAD), true);
#if PLATFORM_TYPE != PLATFORM_TOOL
			Memory::WriteRelativeJmp(blam::tag_data_unload, GET_FUNC_VPTR(TAG_DATA_UNLOAD), true);
#endif
			INIT_HOOK_BY_EXPLICIT_TYPE(tag_data_resize, TAG_DATA_RESIZE,
				bool, tag_data*, int32);

			INIT_HOOK_BY_EXPLICIT_TYPE(tag_block_delete_element, TAG_BLOCK_DELETE_ELEMENT,
				void, tag_block*, int32);
			INIT_HOOK_BY_EXPLICIT_TYPE(tag_block_resize, TAG_BLOCK_RESIZE, 
				bool, tag_block*, int32);
			INIT_HOOK_BY_EXPLICIT_TYPE(tag_block_add_element, TAG_BLOCK_ADD_ELEMENT,
				int32, tag_block*);

			INIT_HOOK_BY_EXPLICIT_TYPE(tag_get, TAG_GET,
				void*, tag, datum_index);
			INIT_HOOK_BY_EXPLICIT_TYPE(tag_new, TAG_NEW,
				datum_index, tag, cstring);
			Memory::WriteRelativeJmp(blam::tag_unload, GET_FUNC_VPTR(TAG_UNLOAD), true);
			INIT_HOOK_BY_EXPLICIT_TYPE(tag_load, TAG_LOAD,
				datum_index, tag, cstring, long_flags);
			INIT_HOOK_BY_EXPLICIT_TYPE(tag_reload, TAG_RELOAD,
				datum_index, tag, cstring);

#undef INIT_HOOK_BY_EXPLICIT_TYPE
		}
		API_FUNC_NAKED void Initialize()
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_FILES_OPEN);

			__asm {
				call	InitializeHooks
				call	FUNCTION
				retn
			}
		}

		// The shader type names list wasn't maintained at some point someone forgot to add the transparent chicago types 
		static void InitializeFixesForShaderGroups()
		{
			string_list* type_names_list = GET_PTR2(shader_type_names_list);
			cstring* type_names = type_names_list->strings;

			// If this fails, something was fixed, or an address is wrong
			YELO_ASSERT(type_names_list->count == Enums::k_number_of_shader_types && 
				type_names[Enums::k_number_of_shader_types-1]==nullptr && type_names[Enums::k_number_of_shader_types-2]==nullptr );

			// chicago types were inserted after the 'generic' type, so we have to move everything up by two
			for(_enum x = Enums::k_number_of_shader_types-1; x > Enums::_shader_type_transparent_chicago_extended; x--)
				type_names[x] = type_names[x-2];

			type_names[Enums::_shader_type_transparent_chicago] = "transparent chicago";
			type_names[Enums::_shader_type_transparent_chicago_extended] = "transparent chicago extended";
		}
		// weapon->magazines->objects errornously references the weapon->magazines format element proc (objects block can have more elements than magazines, which cases an assert in format)
		// This fixes the supposed typo and instead uses the equipment field to be the block's name
		static void InitializeFixesForWeaponGroup()
		{
			tag_group* weapon_group = blam::tag_group_get<s_weapon_definition>();

			// field the weapon's magazines field
			int32 field_index = weapon_group->header_block_definition->FindFieldIndex(Enums::_field_block, "magazines");
			if(field_index != NONE)
			{
				auto* magazines_block = weapon_group->header_block_definition->fields[field_index].Definition<tag_block_definition>();

				// find the magazine's magazine-objects field
				field_index = magazines_block->FindFieldIndex(Enums::_field_block, "magazines");
				if(field_index != NONE)
				{
					tag_field& magazine_objects_field = magazines_block->fields[field_index];
					magazine_objects_field.name = "magazine objects"; // give the field a more descriptive name

					auto* magazine_objects_block = magazine_objects_field.Definition<tag_block_definition>();
					magazine_objects_block->format_proc = nullptr; // Bungie seems to have made a copy&paste error and gave the objects block the format-magazines function

					// find the magazine-object's equipment reference field
					field_index = magazine_objects_block->FindFieldIndex(Enums::_field_tag_reference, "equipment");
					if(field_index != NONE)
					{
						tag_field& equipment_reference_field = magazine_objects_block->fields[field_index];
						equipment_reference_field.name = "equipment^"; // set the name to include markup for 'block name' (since we zapped the, incorrect, format function for this block)
					}
				}
			}
		}
		// unicode_string_list_string_reference_block's [unused1] references the unicode byteswap proc (which doesn't actually byteswap anything, code appears to be conditionally compiled out)
		// However, the string tag_data_definition doesn't define a byteswap proc
		// I'm going to assume this is a typo in the group definitions
		// Or it could be that [unused1] used to be a byteswap proc for blocks, back before bs codes were auto generated (the unicode_string_list group probably dates back to the start of the project)
		// Either way, this fixes the 'problem' (since OS uses the unused fields)
		static void InitializeFixesForUnicodeStringListGroup()
		{
			tag_group* group = blam::tag_group_get('ustr');

			int32 field_index = group->header_block_definition->FindFieldIndex(Enums::_field_block, "string reference");
			if(field_index != NONE)
			{
				auto* reference_block = group->header_block_definition->fields[field_index].Definition<tag_block_definition>();

				field_index = reference_block->FindFieldIndex(Enums::_field_data, "string");
				if(field_index != NONE)
				{
					tag_field& string_field = reference_block->fields[field_index];
					auto* definition = string_field.Definition<tag_data_definition>();

					definition->byte_swap_proc = CAST_PTR(proc_tag_data_byte_swap, reference_block->unused1);
				}

				reference_block->unused1 = nullptr; // (erroneously?) references the unicode byte swap procedure
			}
		}
		// NOTE: this is called from our implementation of tag_groups_initialize
		void InitializeFixes()
		{
			InitializeFixesForShaderGroups();
			InitializeFixesForUnicodeStringListGroup();
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
		API_FUNC_NAKED void PLATFORM_API tag_files_flush()
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_FILES_FLUSH);

			__asm	jmp	FUNCTION
		}
		API_FUNC_NAKED bool PLATFORM_API tag_file_open(tag group_tag, cstring filename, 
			bool* is_readonly, uint32* crc, bool from_file_system)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_FILE_OPEN);

			__asm	jmp	FUNCTION
		}
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
		API_FUNC_NAKED bool PLATFORM_API tag_file_get_file_reference(s_file_reference& reference,
			tag group_tag, cstring name)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_FILE_EXISTS);

			API_FUNC_NAKED_START()
				push	esi

				mov		esi, name
				push	group_tag
				push	reference
				call	FUNCTION
				add		esp, 4 * 2

				pop		esi
			API_FUNC_NAKED_END(0)
		}
		//////////////////////////////////////////////////////////////////////////
		// tag_files/tag_groups
		char* tag_group_loading_error_string =			CAST_PTR(char*,  GET_DATA_PTR(TAG_LOAD_ERROR_STRING));
		char** tag_group_loading_error_string_cursor =	CAST_PTR(char**, GET_DATA_PTR(TAG_LOAD_ERROR_STRING_CURSOR));

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
		API_FUNC_NAKED bool PLATFORM_API tag_read_only(datum_index tag_index)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_READ_ONLY);

			__asm	jmp	FUNCTION
		}


		API_FUNC_NAKED tag_group* PLATFORM_API tag_group_get_next(const tag_group* group)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_GROUP_GET_NEXT);

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
		API_FUNC_NAKED void PLATFORM_API tag_groups_dump_memory()
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_GROUPS_DUMP_MEMORY);

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
		API_FUNC_NAKED bool PLATFORM_API tag_save(datum_index tag_index)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_SAVE);

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

		API_FUNC_NAKED void PLATFORM_API tag_load_children(datum_index tag_index)
		{
			static const uintptr_t FUNCTION = GET_FUNC_PTR(TAG_LOAD_CHILDREN);

			__asm	jmp	FUNCTION
		}
	};
};