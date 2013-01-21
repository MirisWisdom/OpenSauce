/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"

#if PLATFORM_ID == PLATFORM_GUERILLA
#include <YeloLib/Halo1/tag_files/string_id_yelo.hpp>
#include "TagGroups/TagGroups.hpp"

namespace Yelo
{
	namespace Guerilla
	{
		//////////////////////////////////////////////////////////////////////////
		// CBrush
		class MFC_CBrush;
		static API_FUNC_NAKED BOOL CBrush__Dtor(MFC_CBrush* brush)
		{
			static const uintptr_t FUNCTION = 0x404C10;

			API_FUNC_NAKED_START()
				mov		ecx, brush
				call	FUNCTION
			API_FUNC_NAKED_END(1)
		}
		static API_FUNC_NAKED BOOL CBrush__CreateSolidBrush(MFC_CBrush* brush, COLORREF color)
		{
			static const uintptr_t FUNCTION = 0x414060;

			API_FUNC_NAKED_START()
				mov		ecx, brush
				push	color
				call	FUNCTION
			API_FUNC_NAKED_END(2)
		}
		class MFC_CBrush
		{
			void** vftable;
			PAD32; // not actually padding

		public:
			MFC_CBrush(COLORREF color)	{ CBrush__CreateSolidBrush(this, color); }
			~MFC_CBrush()				{ CBrush__Dtor(this); }
		};
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		// CDC
		static API_FUNC_NAKED COLORREF CDC__SetBkMode(void* CDC_, int mode)
		{
			static const uintptr_t FUNCTION = 0x856281;

			API_FUNC_NAKED_START()
				mov		ecx, CDC_
				push	mode
				call	FUNCTION
				API_FUNC_NAKED_END(2)
		}
		static API_FUNC_NAKED COLORREF CDC__SetBkColor(void* CDC_, COLORREF color)
		{
			API_FUNC_NAKED_START()
				mov		ecx, CDC_
				mov		eax, [ecx]
				push	color
				call	[eax+0x30]
			API_FUNC_NAKED_END(2)
		}
		static API_FUNC_NAKED COLORREF CDC__SetTextColor(void* CDC_, COLORREF color)
		{
			API_FUNC_NAKED_START()
				mov		ecx, CDC_
				mov		eax, [ecx]
				push	color
				call	[eax+0x34]
			API_FUNC_NAKED_END(2)
		}
		//////////////////////////////////////////////////////////////////////////


		// NOTE: Bungie defines their own controls for the roles of CEdit,
		// which derive from CWnd. So you can't just treat the edit control of a field as as CEdit
		enum {
			// tag_reference_dialog's tag-name control
			k_idc_tag_reference_name = 0x140,
			// tag_reference_dialog's group-tag control
			k_idc_tag_reference_group = 0x141,

			// field_dialog_named's 'name' control
			k_idc_field_dialog_name = 0x154,

			// field_dialog_named_with_units's 'units' control
			k_idc_field_dialog_units = 0x17F,
		};
		static HBRUSH tag_reference_dialog__OnCtlColor__field_name(void* CDC_, void* CWnd_, field_information* field_info,
			HBRUSH brush)
		{
			tag_reference* reference = CAST_PTR(tag_reference*, field_info->address);
			tag_reference_definition* definition = field_info->field->Definition<tag_reference_definition>();

			if(definition->group_tag == TagGroups::s_string_id_yelo_definition::k_group_tag)
			{
				// Set the field-name control text to blue
				CDC__SetBkColor(CDC_, RGB(0x00, 0x00, 0xFF));
			}

			return brush;
		}
		static HBRUSH tag_reference_dialog__OnCtlColor__tag_name(void* CDC_, void* CWnd_, field_information* field_info,
			HBRUSH brush)
		{
			tag_reference_definition* definition = field_info->field->Definition<tag_reference_definition>();
			COLORREF color = 0;

			if(definition->group_tag == TagGroups::s_string_id_yelo_definition::k_group_tag)
			{
				// Set the tag-name control text to blue
				CDC__SetBkColor(CDC_, RGB(0x00, 0x00, 0xFF));
			}

			tag_reference* reference = CAST_PTR(tag_reference*, field_info->address);
			if(reference == NULL || !tag_file_exists(reference->group_tag, reference->name))
			{
				// Set the tag-name control text to red
				CDC__SetBkColor(CDC_, RGB(0xFF, 0x00, 0x00) | color);
			}

			return brush;
		}
		static API_FUNC_NAKED void tag_reference_dialog__OnCtlColor__hook()
		{
			static const uintptr_t STOCK_CODE = 0x40FE1A;
			static const uintptr_t STOCK_SET_BK_COLOR = 0x40FE37;
			static const uintptr_t THE_END = 0x40FE43;

			// esi = this
			// ebx = CDC*
			// edi = CWnd*
			API_FUNC_NAKED_START_()
				mov		ecx, [esi+0x70] // field_info

				cmp		eax, k_idc_tag_reference_name
				jz		OnCtlColor__tag_name
				cmp		eax, k_idc_field_dialog_name
				jnz		the_end
//OnCtlColor__field_name:
				push	ebp
				push	ecx
				push	edi
				push	ebx
				call	tag_reference_dialog__OnCtlColor__field_name
				mov		ebp, eax
				jmp		the_end
OnCtlColor__tag_name:
				push	ebp
				push	ecx
				push	edi
				push	ebx
				call	tag_reference_dialog__OnCtlColor__tag_name
				mov		ebp, eax

the_end:
				jmp		THE_END
//stock_code:
				jmp		STOCK_CODE
			API_FUNC_NAKED_END_()
		}

		void IntializeBeforeTagGroupsInitalize()
		{
		}

		void Initialize()
		{
			static const uintptr_t TAG_REFERENCE_DIALOG__ONCTLCOLOR_HOOK = 0x40FE13;

			Memory::WriteRelativeJmp(tag_reference_dialog__OnCtlColor__hook,
				CAST_PTR(void*, TAG_REFERENCE_DIALOG__ONCTLCOLOR_HOOK), true);
		}

		void Dispose()
		{
		}
	};
};
#endif