/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once
#if PLATFORM_TYPE == PLATFORM_GUERILLA
#include "Guerilla/field_dialog_extensions.hpp"

namespace Yelo
{
	class field_information;

	namespace Guerilla
	{
		struct string_dialog_extensions : field_dialog_extensions
		{
			static HBRUSH OnCtlColor__field_name(void* CDC_, void* CWnd_, field_information* field_info, 
				HBRUSH brush);

			static void DoDataExchange__MaxChars_hack(
				/*CDataExchange* */void* pDX,
				/*CString const& */void* value,
				int nChars,
				const field_information* field_info);

			static const uintptr_t DODATAEXCHANGE = 0x40F5D0;
			static const uintptr_t DODATAEXCHANGE_HOOK = DODATAEXCHANGE + 0x6B;

			static AFX_MSGMAP* messageMap;
			static const AFX_MSGMAP_ENTRY* messageEntries;
			static AFX_MSGMAP_ENTRY messageEntries_replacement[];

			static void Initialize();
		};
	};
};
#endif