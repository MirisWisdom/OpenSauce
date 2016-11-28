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
		struct tag_reference_dialog_extensions : field_dialog_extensions
		{
			static HBRUSH OnCtlColor__field_name(void* CDC_, void* CWnd_, field_information* field_info, 
				HBRUSH brush);

			static HBRUSH OnCtlColor__tag_name(void* CDC_, void* CWnd_, field_information* field_info,
				HBRUSH brush);

			static const uintptr_t ONCTLCOLOR = 0x40FDF0;
			static const uintptr_t ONCTLCOLOR_HOOK = ONCTLCOLOR + 0x23;
		};
	};
};
#endif