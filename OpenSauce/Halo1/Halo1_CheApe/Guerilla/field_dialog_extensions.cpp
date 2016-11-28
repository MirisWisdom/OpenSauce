/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Guerilla/field_dialog_extensions.hpp"

#if PLATFORM_TYPE == PLATFORM_GUERILLA
#include <blamlib/Halo1/guerilla/field_dialog_structures.hpp>
#include "Guerilla/string_dialog_extensions.hpp"
#include "Guerilla/tag_reference_dialog_extensions.hpp"
#include "TagGroups/TagGroups.hpp"

namespace Yelo
{
	namespace Guerilla
	{
		//////////////////////////////////////////////////////////////////////////
		// feigning string_id hacks
		namespace field_dialog_string_id_extensions
		{
			static HBRUSH OnCtlColor__field_name(void* CDC_, void* CWnd_, field_information* field_info,
				HBRUSH brush)
			{
				switch(field_info->field->type)
				{
				case Enums::_field_string:
					brush = string_dialog_extensions::OnCtlColor__field_name(CDC_, CWnd_, field_info, brush);
					break;

				case Enums::_field_tag_reference:
					brush = tag_reference_dialog_extensions::OnCtlColor__field_name(CDC_, CWnd_, field_info, brush);
					break;

				YELO_ASSERT_CASE_UNREACHABLE();
				}

				return brush;
			}

			static API_FUNC_NAKED void OnCtlColor__hook()
			{
				static const uintptr_t STOCK_CODE =			tag_reference_dialog_extensions::ONCTLCOLOR + 0x2A;
				static const uintptr_t STOCK_SET_BK_COLOR =	tag_reference_dialog_extensions::ONCTLCOLOR + 0x47;
				static const uintptr_t THE_END =			tag_reference_dialog_extensions::ONCTLCOLOR + 0x53;

				// esi = this
				// ebx = CDC*
				// edi = CWnd*
				API_FUNC_NAKED_START_()
					mov		ecx, [esi+k_field_dialog_field_information_offset]

					cmp		eax, k_idc_tag_reference_name
					jz		OnCtlColor__tag_name
					cmp		eax, k_idc_field_dialog_name // for both string_dialog and tag_reference_dialog
					jnz		the_end
//OnCtlColor__field_name:
					push	ebp
					push	ecx
					push	edi
					push	ebx
					call	OnCtlColor__field_name
					mov		ebp, eax
					jmp		the_end
OnCtlColor__tag_name:
					push	ebp
					push	ecx
					push	edi
					push	ebx
					call	tag_reference_dialog_extensions::OnCtlColor__tag_name
					mov		ebp, eax

the_end:
					jmp		THE_END
//stock_code:
//					jmp		STOCK_CODE
				API_FUNC_NAKED_END_()
			}

			static API_FUNC_NAKED void DoDataExchange__string_dialog_MaxChars_hook(
				/*CDataExchange* */void* pDX, 
				/*CString const& */void* value, 
				int nChars)
			{
				API_FUNC_NAKED_START()
					push	esi		// string_dialog* this

					mov		esi, [esi+k_field_dialog_field_information_offset]
					push	esi
					push	nChars
					push	value
					push	pDX
					call	string_dialog_extensions::DoDataExchange__MaxChars_hack

					pop		esi
				API_FUNC_NAKED_END(3)
			}

			void Initialize()
			{
				Memory::WriteRelativeJmp(OnCtlColor__hook,
					CAST_PTR(void*, tag_reference_dialog_extensions::ONCTLCOLOR_HOOK), true);
				Memory::WriteRelativeCall(DoDataExchange__string_dialog_MaxChars_hook,
					CAST_PTR(void*, string_dialog_extensions::DODATAEXCHANGE_HOOK), false);
			}
		};
	};
};
#endif