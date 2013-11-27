/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"

#if PLATFORM_ID == PLATFORM_GUERILLA
#include <blamlib/Halo1/guerilla/field_dialog_structures.hpp>
#include <blamlib/Halo1/tag_files/tag_groups.hpp>
#include <YeloLib/Halo1/tag_files/string_id_yelo.hpp>
#include "Engine/EngineFunctions.hpp"
#include "TagGroups/TagGroups.hpp"
#include <afxwin.h>
namespace Yelo
{
	namespace Guerilla
	{
//see: #include <afxmsg_.h>
#define AFXAPI __stdcall
		struct AFX_MSGMAP_ENTRY
		{
			UINT nMessage;   // windows message
			UINT nCode;      // control code or WM_NOTIFY code
			UINT nID;        // control ID (or 0 for windows messages)
			UINT nLastID;    // used for entries specifying a range of control id's
			UINT_PTR nSig;   // signature type (action) or pointer to message #
			void* pfn;       // routine to call (or special value)
		};
		struct AFX_MSGMAP
		{
			const AFX_MSGMAP* pBaseMap;
			const AFX_MSGMAP_ENTRY* lpEntries;
		};

		//////////////////////////////////////////////////////////////////////////
		// CBrush
		class MFC_CBrush;
		static API_FUNC_NAKED BOOL /*__thiscall*/ CBrush__Dtor(MFC_CBrush* brush)
		{
			static const uintptr_t FUNCTION = 0x404C10;

			API_FUNC_NAKED_START()
				mov		ecx, brush
				call	FUNCTION
			API_FUNC_NAKED_END(1)
		}
		static API_FUNC_NAKED BOOL /*__thiscall*/ CBrush__CreateSolidBrush(MFC_CBrush* brush, COLORREF color)
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
		static API_FUNC_NAKED COLORREF /*__thiscall*/ CDC__SetBkMode(void* CDC_, int mode)
		{
			static const uintptr_t FUNCTION = 0x856281;

			API_FUNC_NAKED_START()
				mov		ecx, CDC_
				push	mode
				call	FUNCTION
			API_FUNC_NAKED_END(2)
		}
		static API_FUNC_NAKED COLORREF /*__thiscall*/ CDC__SetBkColor(void* CDC_, COLORREF color)
		{
			API_FUNC_NAKED_START()
				mov		ecx, CDC_
				mov		eax, [ecx]
				push	color
				call	[eax+0x30]
			API_FUNC_NAKED_END(2)
		}
		static API_FUNC_NAKED COLORREF /*__thiscall*/ CDC__SetTextColor(void* CDC_, COLORREF color)
		{
			API_FUNC_NAKED_START()
				mov		ecx, CDC_
				mov		eax, [ecx]
				push	color
				call	[eax+0x34]
			API_FUNC_NAKED_END(2)
		}
		//////////////////////////////////////////////////////////////////////////
		
		//////////////////////////////////////////////////////////////////////////
		// CCmdUI
		class MFC_CCmdUI
		{
		public:
			virtual void Enable(BOOL bOn);
			virtual void SetCheck(int nCheck = 1);   // 0, 1 or 2 (indeterminate)
			virtual void SetRadio(BOOL bOn);
			virtual void SetText(LPCTSTR lpszText);

			UINT m_nID;
			UINT m_nIndex;			// menu item or other index

			// if a menu item
			class CMenu* m_pMenu;   // NULL if not a menu
			class CMenu* m_pSubMenu;// sub containing menu item
									// if a popup sub menu - ID is for first in popup

			// if from some other window
			class CWnd* m_pOther;	// NULL if a menu or not a CWnd

			BOOL m_bEnableChanged;
			BOOL m_bContinueRouting;
			UINT m_nIndexMax;		// last + 1 for iterating m_nIndex

			class CMenu* m_pParentMenu;	// NULL if parent menu not easily determined
										//  (probably a secondary popup menu)
		};
		//////////////////////////////////////////////////////////////////////////


		struct field_dialog_extensions
		{
			static inline void OnCtlColor__string_id_field_name(void* CDC_)
			{
				// Set the field-name control text to blue
				CDC__SetBkColor(CDC_, RGB(0x00, 0x00, 0xFF));
			}

			static void AFXAPI DDV_MaxChars(
				/*CDataExchange* */void* pDX, 
				/*CString const& */void* value, 
				int nChars)
			{
				static auto* FUNCTION = CAST_PTR( void(AFXAPI*)(void*,void*,int) ,
					0x86029A);

				FUNCTION(pDX, value, nChars);
			}
		};

		//////////////////////////////////////////////////////////////////////////
		// tag_reference_dialog
		struct tag_reference_dialog_extensions : field_dialog_extensions
		{
			static HBRUSH OnCtlColor__field_name(void* CDC_, void* CWnd_, field_information* field_info, 
				HBRUSH brush)
			{
				auto* reference = CAST_PTR(tag_reference*, field_info->address);
				auto* definition = field_info->field->Definition<tag_reference_definition>();

				if(definition->group_tag == TagGroups::s_string_id_yelo_definition::k_group_tag)
					OnCtlColor__string_id_field_name(CDC_);

				return brush;
			}

			static HBRUSH OnCtlColor__tag_name(void* CDC_, void* CWnd_, field_information* field_info,
				HBRUSH brush)
			{
				auto* definition = field_info->field->Definition<tag_reference_definition>();
				COLORREF color = 0;

				if(definition->group_tag == TagGroups::s_string_id_yelo_definition::k_group_tag)
					OnCtlColor__string_id_field_name(CDC_);

				tag_reference* reference = CAST_PTR(tag_reference*, field_info->address);
				if(reference == nullptr || !blam::tag_file_exists(reference->group_tag, reference->name))
				{
					// Set the tag-name control text to red
					CDC__SetBkColor(CDC_, RGB(0xFF, 0x00, 0x00) | color);
				}

				return brush;
			}

			static const uintptr_t ONCTLCOLOR = 0x40FDF0;
			static const uintptr_t ONCTLCOLOR_HOOK = ONCTLCOLOR + 0x23;
		};

		//////////////////////////////////////////////////////////////////////////
		// string_dialog, for supporting 'old_string_id' and other hacks fields
		struct string_dialog_extensions : field_dialog_extensions
		{
			static HBRUSH OnCtlColor__field_name(void* CDC_, void* CWnd_, field_information* field_info, 
				HBRUSH brush)
			{
				if(TagGroups::TagFieldIsOldStringId(field_info->field))
					OnCtlColor__string_id_field_name(CDC_);

				return brush;
			}

			static void DoDataExchange__MaxChars_hack(
				/*CDataExchange* */void* pDX, 
				/*CString const& */void* value, 
				int nChars,
				const field_information* field_info)
			{
				nChars = TagGroups::StringFieldGetLength(field_info->field);

				DDV_MaxChars(pDX, value, nChars);
			}
			static const uintptr_t DODATAEXCHANGE = 0x40F5D0;
			static const uintptr_t DODATAEXCHANGE_HOOK = DODATAEXCHANGE + 0x6B;

			static AFX_MSGMAP* messageMap;
			static const AFX_MSGMAP_ENTRY* messageEntries;
			static AFX_MSGMAP_ENTRY messageEntries_replacement[];

			static void Initialize()
			{
				std::memcpy(messageEntries_replacement, messageEntries, sizeof(*messageEntries) * 1);
				messageMap->lpEntries = messageEntries_replacement;
			}
		};
		AFX_MSGMAP* string_dialog_extensions::messageMap = CAST_PTR(AFX_MSGMAP*, 0x898E0C);
		const AFX_MSGMAP_ENTRY* string_dialog_extensions::messageEntries = CAST_PTR(AFX_MSGMAP_ENTRY*, 0x898E18);
		AFX_MSGMAP_ENTRY string_dialog_extensions::messageEntries_replacement[] = {
			{}, // for string_dialog's original message entry

			// same ENTRY data as tag_reference_dialog's OnCtlColor, ON_WM_CTRCOLOR()
			{/*WM_CTLCOLOR*/ 0x19, 0x0, 0x0, 0x0, /*AfxSig_hDWw, AfxSig_CTLCOLOR*/ 0x8, 
				CAST_PTR(void*, tag_reference_dialog_extensions::ONCTLCOLOR) },
			{0, 0, 0, 0, /*AfxSig_end*/ 0, nullptr}, // END()
		};

		//////////////////////////////////////////////////////////////////////////
		// feigning string_id hacks
		namespace field_dialog_string_id_extensions // namespace b/c you can't use API_FUNC_NAKED on member functions (even static ones)
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

			static void Initialize()
			{
				Memory::WriteRelativeJmp(OnCtlColor__hook,
					CAST_PTR(void*, tag_reference_dialog_extensions::ONCTLCOLOR_HOOK), true);
				Memory::WriteRelativeCall(DoDataExchange__string_dialog_MaxChars_hook,
					CAST_PTR(void*, string_dialog_extensions::DODATAEXCHANGE_HOOK), false);
			}
		};

		struct application_extensions
		{
			enum {
				_msgmap_entry_file_open,
				_msgmap_entry_file_import,							// Ctrl+I
				_msgmap_entry_file_xsync,							// Ctrl+Shift+S
				_msgmap_entry_about,
				_msgmap_entry_source_control_checkout,				// Ctrl+K
				_msgmap_entry_source_control_checkin,				// Ctrl+U
				_msgmap_entry_source_control_checkout_undo,
				_msgmap_entry_source_control_get_latest,			// Ctrl+G
				_msgmap_entry_file_refresh,
				_msgmap_entry_source_control_checkin_update,		// update the menu options enablement
				_msgmap_entry_source_control_checkout_update,
				_msgmap_entry_source_control_get_latest_update,
				_msgmap_entry_source_contrl_checkout_undo_update,
				_msgmap_entry_file_new,
			};

			static const uintptr_t MESSAGE_ENTRIES = 0x89AB50;

			static void /*__thiscall*/ OnFileXsyncOverride()
			{
			}
			static void /*__thiscall*/ OnUpdateSourceControlCheckinOverride(MFC_CCmdUI* pCmdUI)
			{
				pCmdUI->Enable(false);
			}
			static void /*__thiscall*/ OnUpdateSourceControlCheckoutOverride(MFC_CCmdUI* pCmdUI)
			{
				pCmdUI->Enable(false);
			}
			static void /*__thiscall*/ OnUpdateSourceControlGetLatestOverride(MFC_CCmdUI* pCmdUI)
			{
				pCmdUI->Enable(false);
			}
			static void /*__thiscall*/ OnUpdateSourceControlCheckoutUndoOverride(MFC_CCmdUI* pCmdUI)
			{
				pCmdUI->Enable(false);
			}
			static void Initialize()
			{
				auto entries = CAST_PTR(AFX_MSGMAP_ENTRY*, MESSAGE_ENTRIES);
				//entries[_msgmap_entry_file_xsync].pfn = OnFileXsyncOverride;
				entries[_msgmap_entry_source_control_checkin_update].pfn = OnUpdateSourceControlCheckinOverride;
				entries[_msgmap_entry_source_control_checkout_update].pfn = OnUpdateSourceControlCheckoutOverride;
				entries[_msgmap_entry_source_control_get_latest_update].pfn = OnUpdateSourceControlGetLatestOverride;
				entries[_msgmap_entry_source_contrl_checkout_undo_update].pfn = OnUpdateSourceControlCheckoutUndoOverride;
			}
		};

		void IntializeBeforeTagGroupsInitalize()
		{
		}

		void Initialize()
		{
			string_dialog_extensions::Initialize();

			field_dialog_string_id_extensions::Initialize();

			application_extensions::Initialize();
		}

		void Dispose()
		{
		}
	};
};
#endif