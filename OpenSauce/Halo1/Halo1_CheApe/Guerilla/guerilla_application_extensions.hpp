/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once
#if PLATFORM_TYPE == PLATFORM_GUERILLA
#include <YeloLib/Halo1/guerilla/mfc_interfaces.hpp>

namespace Yelo
{
	namespace Guerilla
	{
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
				_msgmap_entry_source_control_checkout_undo_update,
				_msgmap_entry_file_new,
			};

			static const uintptr_t MESSAGE_ENTRIES = 0x89AB50;

			static void /*__thiscall*/ OnFileXsyncOverride();
			static void /*__thiscall*/ OnUpdateSourceControlCheckinOverride(MFC_CCmdUI* pCmdUI);
			static void /*__thiscall*/ OnUpdateSourceControlCheckoutOverride(MFC_CCmdUI* pCmdUI);
			static void /*__thiscall*/ OnUpdateSourceControlGetLatestOverride(MFC_CCmdUI* pCmdUI);
			static void /*__thiscall*/ OnUpdateSourceControlCheckoutUndoOverride(MFC_CCmdUI* pCmdUI);
			static void Initialize();
		};
	};
};
#endif