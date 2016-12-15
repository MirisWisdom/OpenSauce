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
		struct field_dialog_extensions
		{
			static inline void OnCtlColor__string_id_field_name(void* CDC_)
			{
				// Set the field-name control text to blue
				CDC__SetBkColor(CDC_, RGB(0x00, 0x00, 0xFF));
			}

			static inline void AFXAPI DDV_MaxChars(
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
		// feigning string_id hacks
		namespace field_dialog_string_id_extensions // namespace b/c you can't use API_FUNC_NAKED on member functions (even static ones)
		{
			void Initialize();
		};
	};
};
#endif