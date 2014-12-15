/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include "Interface/OpenSauceUI/resource_id.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI
	{
#pragma region Base
#define K_EVENT_HOVERENTER_ID			RESOURCE_ID_DEBUG("HoverEnter")
#define K_EVENT_HOVERLEAVE_ID			RESOURCE_ID_DEBUG("HoverLeave")
#pragma endregion
		
#pragma region CheckBox
#define K_EVENT_CHECKED_ID RESOURCE_ID_DEBUG("Checked")
#define K_EVENT_UNCHECKED_ID RESOURCE_ID_DEBUG("UnChecked")
#define K_EVENT_CHECKCHANGED_ID RESOURCE_ID_DEBUG("CheckChanged")
#pragma endregion

#pragma region CheckBox
#define K_EVENT_TEXTCHANGED_ID RESOURCE_ID_DEBUG("TextChanged")
#define K_EVENT_RETURNPRESSED_ID RESOURCE_ID_DEBUG("ReturnPressed")
#pragma endregion

#pragma region Misc
#define K_EVENT_VALUECHANGED_ID RESOURCE_ID_DEBUG("ValueChanged")
#pragma endregion
	};};
};
#endif