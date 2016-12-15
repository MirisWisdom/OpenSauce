/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Guerilla/guerilla_application_extensions.hpp"

#if PLATFORM_TYPE == PLATFORM_GUERILLA
namespace Yelo
{
	namespace Guerilla
	{
		void /*__thiscall*/ application_extensions::OnFileXsyncOverride()
		{
		}

		void /*__thiscall*/ application_extensions::OnUpdateSourceControlCheckinOverride(MFC_CCmdUI* pCmdUI)
		{
			pCmdUI->Enable(false);
		}
		void /*__thiscall*/ application_extensions::OnUpdateSourceControlCheckoutOverride(MFC_CCmdUI* pCmdUI)
		{
			pCmdUI->Enable(false);
		}
		void /*__thiscall*/ application_extensions::OnUpdateSourceControlGetLatestOverride(MFC_CCmdUI* pCmdUI)
		{
			pCmdUI->Enable(false);
		}
		void /*__thiscall*/ application_extensions::OnUpdateSourceControlCheckoutUndoOverride(MFC_CCmdUI* pCmdUI)
		{
			pCmdUI->Enable(false);
		}

		void application_extensions::Initialize()
		{
			auto entries = CAST_PTR(AFX_MSGMAP_ENTRY*, MESSAGE_ENTRIES);
			//entries[_msgmap_entry_file_xsync].pfn = OnFileXsyncOverride;
			entries[_msgmap_entry_source_control_checkin_update].pfn = OnUpdateSourceControlCheckinOverride;
			entries[_msgmap_entry_source_control_checkout_update].pfn = OnUpdateSourceControlCheckoutOverride;
			entries[_msgmap_entry_source_control_get_latest_update].pfn = OnUpdateSourceControlGetLatestOverride;
			entries[_msgmap_entry_source_control_checkout_undo_update].pfn = OnUpdateSourceControlCheckoutUndoOverride;
		}
	};
};
#endif