/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Guerilla/string_dialog_extensions.hpp"

#if PLATFORM_TYPE == PLATFORM_GUERILLA
#include <YeloLib/Halo1/tag_files/string_id_yelo.hpp>
#include "Guerilla/tag_reference_dialog_extensions.hpp"
#include "TagGroups/TagGroups.hpp"

namespace Yelo
{
	namespace Guerilla
	{
		HBRUSH string_dialog_extensions::OnCtlColor__field_name(void* CDC_, void* CWnd_, field_information* field_info,
			HBRUSH brush)
		{
			if (TagGroups::TagFieldIsOldStringId(field_info->field))
				OnCtlColor__string_id_field_name(CDC_);

			return brush;
		}

		void string_dialog_extensions::DoDataExchange__MaxChars_hack(
			/*CDataExchange* */void* pDX,
			/*CString const& */void* value,
			int nChars,
			const field_information* field_info)
		{
			nChars = TagGroups::StringFieldGetLength(field_info->field);

			DDV_MaxChars(pDX, value, nChars);
		}

		AFX_MSGMAP* string_dialog_extensions::messageMap = CAST_PTR(AFX_MSGMAP*, 0x898E0C);
		const AFX_MSGMAP_ENTRY* string_dialog_extensions::messageEntries = CAST_PTR(AFX_MSGMAP_ENTRY*, 0x898E18);
		AFX_MSGMAP_ENTRY string_dialog_extensions::messageEntries_replacement[] = {
			{}, // for string_dialog's original message entry

			// same ENTRY data as tag_reference_dialog's OnCtlColor, ON_WM_CTRCOLOR()
			{/*WM_CTLCOLOR*/ 0x19, 0x0, 0x0, 0x0, /*AfxSig_hDWw, AfxSig_CTLCOLOR*/ 0x8, 
				CAST_PTR(void*, tag_reference_dialog_extensions::ONCTLCOLOR) },
			{0, 0, 0, 0, /*AfxSig_end*/ 0, nullptr}, // END()
		};

		void string_dialog_extensions::Initialize()
		{
			std::memcpy(messageEntries_replacement, messageEntries, sizeof(*messageEntries) * 1);
			messageMap->lpEntries = messageEntries_replacement;
		}
	};
};
#endif