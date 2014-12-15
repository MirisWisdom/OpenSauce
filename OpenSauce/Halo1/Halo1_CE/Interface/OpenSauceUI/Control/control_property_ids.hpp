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
#define K_PROPERTY_POSITION_ID			RESOURCE_ID_DEBUG("Position")
#define K_PROPERTY_SIZE_ID				RESOURCE_ID_DEBUG("Size")
#define K_PROPERTY_DOCK_ID				RESOURCE_ID_DEBUG("Dock")
#define K_PROPERTY_MARGIN_ID			RESOURCE_ID_DEBUG("Margin")
#define K_PROPERTY_PADDING_ID			RESOURCE_ID_DEBUG("Padding")
#define K_PROPERTY_HIDDEN_ID			RESOURCE_ID_DEBUG("Hidden")
#define K_PROPERTY_DISABLED_ID			RESOURCE_ID_DEBUG("Disabled")
#pragma endregion

#pragma region CheckBox
#define K_PROPERTY_CHECKED_ID			RESOURCE_ID_DEBUG("Checked")
#pragma endregion
		
#pragma region Label
#define K_PROPERTY_TEXT_ID				RESOURCE_ID_DEBUG("Text")
#define K_PROPERTY_TEXTTYPEFACE_ID		RESOURCE_ID_DEBUG("TextTypeface")
#define K_PROPERTY_TEXTSIZE_ID			RESOURCE_ID_DEBUG("TextSize")
#define K_PROPERTY_TEXTBOLD_ID			RESOURCE_ID_DEBUG("TextBold")
#define K_PROPERTY_TEXTCOLOR_ID			RESOURCE_ID_DEBUG("TextColor")
#define K_PROPERTY_TEXTVALIGNMENT_ID	RESOURCE_ID_DEBUG("TextVAlignment")
#define K_PROPERTY_TEXTHALIGNMENT_ID	RESOURCE_ID_DEBUG("TextHAlignment")
#define K_PROPERTY_TEXTWRAP_ID			RESOURCE_ID_DEBUG("TextWrap")
#define K_PROPERTY_TEXTPADDING_ID		RESOURCE_ID_DEBUG("TextPadding")
#pragma endregion

#pragma region Scroll Control
#define K_PROPERTY_INNERSIZE_ID			RESOURCE_ID_DEBUG("InnerSize")
#define K_PROPERTY_SCROLLH_ID			RESOURCE_ID_DEBUG("ScrollH")
#define K_PROPERTY_SCROLLV_ID			RESOURCE_ID_DEBUG("ScrollV")
#define K_PROPERTY_AUTOHIDEBARS_ID		RESOURCE_ID_DEBUG("AutoHideBars")
#pragma endregion

#pragma region Slider
#define K_PROPERTY_CLAMPTONOTCHES_ID	RESOURCE_ID_DEBUG("ClampToNotches")
#define K_PROPERTY_NOTCHCOUNT_ID		RESOURCE_ID_DEBUG("NotchCount")
#pragma endregion

#pragma region TextBox
#define K_PROPERTY_EDITABLE_ID			RESOURCE_ID_DEBUG("Editable")
#pragma endregion
		
#pragma region General
#define K_PROPERTY_VALUE_ID				RESOURCE_ID_DEBUG("Value")
#define K_PROPERTY_COLOR_ID				RESOURCE_ID_DEBUG("Color")
#define K_PROPERTY_MIN_ID				RESOURCE_ID_DEBUG("Min")
#define K_PROPERTY_MAX_ID				RESOURCE_ID_DEBUG("Max")
#pragma endregion
	};};
};
#endif