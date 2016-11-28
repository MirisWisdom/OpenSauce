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
#define K_EVENT_HOVERENTER_ID			RESOURCE_ID("HoverEnter",		0xA6F52EBF)
#define K_EVENT_HOVERLEAVE_ID			RESOURCE_ID("HoverLeave",		0x84356E01)
#pragma endregion
		
#pragma region Check Box
#define K_EVENT_CHECKED_ID				RESOURCE_ID("Checked",			0x62BE831F)
#define K_EVENT_UNCHECKED_ID			RESOURCE_ID("UnChecked",		0xE47FFE7F)
#define K_EVENT_CHECKCHANGED_ID			RESOURCE_ID("CheckChanged",		0xF8BCC1B2)
#pragma endregion

#pragma region Text Box
#define K_EVENT_TEXTCHANGED_ID			RESOURCE_ID("TextChanged",		0x066DF5C1)
#define K_EVENT_RETURNPRESSED_ID		RESOURCE_ID("ReturnPressed",	0xDA79B74F)
#pragma endregion

#pragma region Page Control
#define K_EVENT_PAGECHANGED_ID			RESOURCE_ID("PageChanged",		0x7C379A2C)
#define K_EVENT_FINISH_ID				RESOURCE_ID("Finish",			0x2765EE2E)
#pragma endregion

#pragma region Button
#define K_EVENT_PRESS_ID				RESOURCE_ID("Press",			0xBC0553B9)
#define K_EVENT_RIGHTPRESS_ID			RESOURCE_ID("RightPress",		0x264849AF)
#define K_EVENT_DOWN_ID					RESOURCE_ID("Down",				0xBCCD3F05)
#define K_EVENT_UP_ID					RESOURCE_ID("Up",				0xD610CAD2)
#define K_EVENT_DOUBLECLICK_ID			RESOURCE_ID("DoubleClick",		0xAC3A0761)
#define K_EVENT_TOGGLE_ID				RESOURCE_ID("Toggle",			0x2246A8BD)
#define K_EVENT_TOGGLEON_ID				RESOURCE_ID("ToggleOn",			0x1554D403)
#define K_EVENT_TOGGLEOFF_ID			RESOURCE_ID("ToggleOff",		0x2716A486)
#pragma endregion

#pragma region Misc
#define K_EVENT_VALUECHANGED_ID			RESOURCE_ID("ValueChanged",		0x210F67D8)
#pragma endregion
	};};
};
#endif