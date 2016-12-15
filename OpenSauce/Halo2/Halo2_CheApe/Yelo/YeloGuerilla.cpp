/*
    Yelo: Open Sauce SDK
		Halo 2 (Editing Kit) Edition

	See license\OpenSauce\Halo2_CheApe for specific license information
*/
#include "Common/Precompile.hpp"

#if PLATFORM_ID == PLATFORM_GUERILLA

namespace Yelo
{
	namespace Guerilla
	{
		static void DisableTagTemplateViews()
		{
			FUNC_PTR(C_TAG_TEMPLATE_INTERFACE_FROM_GROUP_TAG, 0x48E730, FUNC_PTR_NULL, FUNC_PTR_NULL);
			static byte k_mod_bytes[] = { 0x33, 0xC0, 0xC3 }; // xor eax, eax; retn

			Memory::WriteMemory(GET_FUNC_VPTR(C_TAG_TEMPLATE_INTERFACE_FROM_GROUP_TAG), 
				k_mod_bytes, sizeof(k_mod_bytes));
		}

		static void DisplayAllFields()
		{
			FUNC_PTR(TAG_FIELD_IS_INVISIBLE_MOD,	0x47FEF2, FUNC_PTR_NULL, FUNC_PTR_NULL);
			FUNC_PTR(TAG_FIELD_IS_INVISIBLE_RETN_1,	0x47FF2B, FUNC_PTR_NULL, FUNC_PTR_NULL);

			static int32* _tag_field_enum_value_remapping_count = CAST_PTR(int32*, 0x901928);

			Memory::WriteRelativeJmp(GET_FUNC_VPTR(TAG_FIELD_IS_INVISIBLE_RETN_1),
				GET_FUNC_VPTR(TAG_FIELD_IS_INVISIBLE_MOD), true);

			*_tag_field_enum_value_remapping_count = 0;
		}

		static void AllowAllTags()
		{
			FUNC_PTR(NEW_DIALOG_FUNCTION_MOD,	0x409FEB, FUNC_PTR_NULL, FUNC_PTR_NULL);
			FUNC_PTR(NEW_DIALOG_FUNCTION_JMP_TO,0x40A011, FUNC_PTR_NULL, FUNC_PTR_NULL);

			Memory::WriteRelativeJmp(GET_FUNC_VPTR(NEW_DIALOG_FUNCTION_JMP_TO),
				GET_FUNC_VPTR(NEW_DIALOG_FUNCTION_MOD), true);

			FUNC_PTR(TAG_TREE_FUNCTION_MOD,		0x4476E7, FUNC_PTR_NULL, FUNC_PTR_NULL);
			FUNC_PTR(TAG_TREE_FUNCTION_JMP_TO,	0x44770A, FUNC_PTR_NULL, FUNC_PTR_NULL);

			Memory::WriteRelativeJmp(GET_FUNC_VPTR(TAG_TREE_FUNCTION_JMP_TO),
				GET_FUNC_VPTR(TAG_TREE_FUNCTION_MOD), true);

			FUNC_PTR(TAG_GROUP_IS_ALLOWED_BY_GROUP_TAG_MOD,		0x47FB10, FUNC_PTR_NULL, FUNC_PTR_NULL);
			FUNC_PTR(TAG_GROUP_IS_ALLOWED_BY_GROUP_TAG_JMP_TO,	0x47FB71, FUNC_PTR_NULL, FUNC_PTR_NULL);

			Memory::WriteRelativeJmp(GET_FUNC_VPTR(TAG_GROUP_IS_ALLOWED_BY_GROUP_TAG_JMP_TO),
				GET_FUNC_VPTR(TAG_GROUP_IS_ALLOWED_BY_GROUP_TAG_MOD), true);

			FUNC_PTR(TAG_GROUP_IS_ALLOWED_BY_FILE_EXTENSION_MOD,		0x47FB81, FUNC_PTR_NULL, FUNC_PTR_NULL);
			FUNC_PTR(TAG_GROUP_IS_ALLOWED_BY_FILE_EXTENSION_TAG_JMP_TO,	0x47FC5F, FUNC_PTR_NULL, FUNC_PTR_NULL);

			Memory::WriteRelativeJmp(GET_FUNC_VPTR(TAG_GROUP_IS_ALLOWED_BY_FILE_EXTENSION_TAG_JMP_TO),
				GET_FUNC_VPTR(TAG_GROUP_IS_ALLOWED_BY_FILE_EXTENSION_MOD), true);
		}

		static void AllowBaseTagGroupCreation()
		{
			FUNC_PTR(NEW_DIALOG_FUNCTION_MOD,	0x409FE0, FUNC_PTR_NULL, FUNC_PTR_NULL);
			static byte k_nop_code[11] = {0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90};

			Memory::WriteMemory(GET_FUNC_VPTR(NEW_DIALOG_FUNCTION_MOD), 
				k_nop_code, sizeof(k_nop_code));
		}

		void Initialize()
		{
		}

		void Dispose()
		{
		}
	};
};
#endif