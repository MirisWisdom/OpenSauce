/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI/GwenUI/ControlBuilders/c_control_builder_gwen_pagecontrol.hpp"

#if !PLATFORM_IS_DEDI

#include <Gwen/Controls/PageControl.h>
#include <YeloLib/cseries/value_conversion.hpp>

#include "Interface/OpenSauceUI/Definitions/c_page_definition.hpp"
#include "Interface/OpenSauceUI/Control/control_property_ids.hpp"
#include "Interface/OpenSauceUI/Control/control_event_ids.hpp"
#include "Interface/OpenSauceUI/Control/c_control_base.hpp"
#include "Interface/OpenSauceUI/GwenUI/ControlBuilders/gwen_control_util_label.hpp"
#include "Interface/OpenSauceUI/GwenUI/c_event_handler_gwen.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace GwenUI { namespace ControlBuilders
	{
		DEFINE_PROPERTY_INTERFACE(pagecontrol_text_format,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());
				auto text = gwen_control.GetPageFormatString();

				output.SetString(text.c_str());
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				gwen_control.SetPageFormatString(value.m_cstring);
			},
			[](Control::i_control& control, cstring value)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());
				
				gwen_control.SetPageFormatString(value);
			}
		);

#pragma region Page Label
		DEFINE_PROPERTY_INTERFACE(pagecontrol_text_typeface,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());
				auto text = Label_GetTextTypeface(*gwen_control.Label());

				output.SetString(text.c_str());
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				Label_SetTextTypeface(*gwen_control.Label(), value.m_cstring);
			},
			[](Control::i_control& control, cstring value)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				Label_SetTextTypeface(*gwen_control.Label(), value);
			}
		);

		DEFINE_PROPERTY_INTERFACE(pagecontrol_text_size,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				output.m_int32 = Label_GetTextSize(*gwen_control.Label());
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				Label_SetTextSize(*gwen_control.Label(), value.m_real);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				real value;
				ValueConversion::FromString(value_string, value);
				Label_SetTextSize(*gwen_control.Label(), value);
			}
		);

		DEFINE_PROPERTY_INTERFACE(pagecontrol_text_bold,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				output.m_bool = Label_GetTextBold(*gwen_control.Label());
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				return Label_SetTextBold(*gwen_control.Label(), value.m_bool);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				bool value;
				ValueConversion::FromString(value_string, value);
				return Label_SetTextBold(*gwen_control.Label(), value);
			}
		);

		DEFINE_PROPERTY_INTERFACE(pagecontrol_text_color,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				output.m_argb_color = Label_GetTextColor(*gwen_control.Label());
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				Label_SetTextColor(*gwen_control.Label(), value.m_argb_color);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				argb_color value;
				ValueConversion::FromString(value_string, value);
				Label_SetTextColor(*gwen_control.Label(), value);
			}
		);

		DEFINE_PROPERTY_INTERFACE(pagecontrol_text_vertical_alignment,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				output.m_int32 = Label_GetVerticalAlignment(*gwen_control.Label());
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				Label_SetVerticalAlignment(*gwen_control.Label(), value.m_int32);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				Label_SetVerticalAlignment(*gwen_control.Label(), value_string);
			}
		);

		DEFINE_PROPERTY_INTERFACE(pagecontrol_text_horizontal_alignment,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				output.m_int32 = Label_GetHorizontalAlignment(*gwen_control.Label());
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				Label_SetHorizontalAlignment(*gwen_control.Label(), value.m_int32);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				Label_SetHorizontalAlignment(*gwen_control.Label(), value_string);
			}
		);

		DEFINE_PROPERTY_INTERFACE(pagecontrol_text_wrap,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				output.m_bool = Label_GetWrap(*gwen_control.Label());
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				return Label_SetWrap(*gwen_control.Label(), value.m_bool);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				bool value;
				ValueConversion::FromString(value_string, value);
				return Label_SetWrap(*gwen_control.Label(), value);
			}
		);

		DEFINE_PROPERTY_INTERFACE(pagecontrol_text_padding,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				output.m_rectangle2d = Label_GetTextPadding(*gwen_control.Label());
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				Label_SetTextPadding(*gwen_control.Label(), value.m_rectangle2d);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				rectangle2d value;
				ValueConversion::FromString(value_string, value);
				Label_SetTextPadding(*gwen_control.Label(), value);
			}
		);
#pragma endregion

#pragma region Back Button
		DEFINE_PROPERTY_INTERFACE(pagecontrol_back_button_text,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());
				auto text = Label_GetText(*gwen_control.BackButton());

				output.SetString(text);
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				Label_SetText(*gwen_control.BackButton(), value.m_cstring);
			},
			[](Control::i_control& control, cstring value)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				Label_SetText(*gwen_control.BackButton(), value);
			}
		);

		DEFINE_PROPERTY_INTERFACE(pagecontrol_back_button_typeface,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());
				auto text = Label_GetTextTypeface(*gwen_control.BackButton());

				output.SetString(text.c_str());
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				Label_SetTextTypeface(*gwen_control.BackButton(), value.m_cstring);
			},
			[](Control::i_control& control, cstring value)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				Label_SetTextTypeface(*gwen_control.BackButton(), value);
			}
		);

		DEFINE_PROPERTY_INTERFACE(pagecontrol_back_button_size,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				output.m_int32 = Label_GetTextSize(*gwen_control.BackButton());
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				Label_SetTextSize(*gwen_control.BackButton(), value.m_real);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				real value;
				ValueConversion::FromString(value_string, value);
				Label_SetTextSize(*gwen_control.BackButton(), value);
			}
		);

		DEFINE_PROPERTY_INTERFACE(pagecontrol_back_button_bold,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				output.m_bool = Label_GetTextBold(*gwen_control.BackButton());
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				return Label_SetTextBold(*gwen_control.BackButton(), value.m_bool);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				bool value;
				ValueConversion::FromString(value_string, value);
				return Label_SetTextBold(*gwen_control.BackButton(), value);
			}
		);

		DEFINE_PROPERTY_INTERFACE(pagecontrol_back_button_color,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				output.m_argb_color = Label_GetTextColor(*gwen_control.BackButton());
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				Label_SetTextColor(*gwen_control.BackButton(), value.m_argb_color);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				argb_color value;
				ValueConversion::FromString(value_string, value);
				Label_SetTextColor(*gwen_control.BackButton(), value);
			}
		);

		DEFINE_PROPERTY_INTERFACE(pagecontrol_back_button_vertical_alignment,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				output.m_int32 = Label_GetVerticalAlignment(*gwen_control.BackButton());
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				Label_SetVerticalAlignment(*gwen_control.BackButton(), value.m_int32);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				Label_SetVerticalAlignment(*gwen_control.BackButton(), value_string);
			}
		);

		DEFINE_PROPERTY_INTERFACE(pagecontrol_back_button_horizontal_alignment,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				output.m_int32 = Label_GetHorizontalAlignment(*gwen_control.BackButton());
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				Label_SetHorizontalAlignment(*gwen_control.BackButton(), value.m_int32);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				Label_SetHorizontalAlignment(*gwen_control.BackButton(), value_string);
			}
		);

		DEFINE_PROPERTY_INTERFACE(pagecontrol_back_button_wrap,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				output.m_bool = Label_GetWrap(*gwen_control.BackButton());
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				return Label_SetWrap(*gwen_control.BackButton(), value.m_bool);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				bool value;
				ValueConversion::FromString(value_string, value);
				return Label_SetWrap(*gwen_control.BackButton(), value);
			}
		);

		DEFINE_PROPERTY_INTERFACE(pagecontrol_back_button_padding,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				output.m_rectangle2d = Label_GetTextPadding(*gwen_control.BackButton());
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				Label_SetTextPadding(*gwen_control.BackButton(), value.m_rectangle2d);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				rectangle2d value;
				ValueConversion::FromString(value_string, value);
				Label_SetTextPadding(*gwen_control.BackButton(), value);
			}
		);
#pragma endregion

#pragma region Next Button
		DEFINE_PROPERTY_INTERFACE(pagecontrol_next_button_text,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());
				auto text = Label_GetText(*gwen_control.NextButton());

				output.SetString(text);
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				Label_SetText(*gwen_control.NextButton(), value.m_cstring);
			},
			[](Control::i_control& control, cstring value)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				Label_SetText(*gwen_control.NextButton(), value);
			}
		);

		DEFINE_PROPERTY_INTERFACE(pagecontrol_next_button_typeface,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());
				auto text = Label_GetTextTypeface(*gwen_control.NextButton());

				output.SetString(text.c_str());
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				Label_SetTextTypeface(*gwen_control.NextButton(), value.m_cstring);
			},
			[](Control::i_control& control, cstring value)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				Label_SetTextTypeface(*gwen_control.NextButton(), value);
			}
		);

		DEFINE_PROPERTY_INTERFACE(pagecontrol_next_button_size,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				output.m_int32 = Label_GetTextSize(*gwen_control.NextButton());
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				Label_SetTextSize(*gwen_control.NextButton(), value.m_real);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				real value;
				ValueConversion::FromString(value_string, value);
				Label_SetTextSize(*gwen_control.NextButton(), value);
			}
		);

		DEFINE_PROPERTY_INTERFACE(pagecontrol_next_button_bold,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				output.m_bool = Label_GetTextBold(*gwen_control.NextButton());
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				return Label_SetTextBold(*gwen_control.NextButton(), value.m_bool);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				bool value;
				ValueConversion::FromString(value_string, value);
				return Label_SetTextBold(*gwen_control.NextButton(), value);
			}
		);

		DEFINE_PROPERTY_INTERFACE(pagecontrol_next_button_color,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				output.m_argb_color = Label_GetTextColor(*gwen_control.NextButton());
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				Label_SetTextColor(*gwen_control.NextButton(), value.m_argb_color);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				argb_color value;
				ValueConversion::FromString(value_string, value);
				Label_SetTextColor(*gwen_control.NextButton(), value);
			}
		);

		DEFINE_PROPERTY_INTERFACE(pagecontrol_next_button_vertical_alignment,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				output.m_int32 = Label_GetVerticalAlignment(*gwen_control.NextButton());
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				Label_SetVerticalAlignment(*gwen_control.NextButton(), value.m_int32);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				Label_SetVerticalAlignment(*gwen_control.NextButton(), value_string);
			}
		);

		DEFINE_PROPERTY_INTERFACE(pagecontrol_next_button_horizontal_alignment,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				output.m_int32 = Label_GetHorizontalAlignment(*gwen_control.NextButton());
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				Label_SetHorizontalAlignment(*gwen_control.NextButton(), value.m_int32);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				Label_SetHorizontalAlignment(*gwen_control.NextButton(), value_string);
			}
		);

		DEFINE_PROPERTY_INTERFACE(pagecontrol_next_button_wrap,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				output.m_bool = Label_GetWrap(*gwen_control.NextButton());
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				return Label_SetWrap(*gwen_control.NextButton(), value.m_bool);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				bool value;
				ValueConversion::FromString(value_string, value);
				return Label_SetWrap(*gwen_control.NextButton(), value);
			}
		);

		DEFINE_PROPERTY_INTERFACE(pagecontrol_next_button_padding,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				output.m_rectangle2d = Label_GetTextPadding(*gwen_control.NextButton());
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				Label_SetTextPadding(*gwen_control.NextButton(), value.m_rectangle2d);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				rectangle2d value;
				ValueConversion::FromString(value_string, value);
				Label_SetTextPadding(*gwen_control.NextButton(), value);
			}
		);
#pragma endregion

#pragma region Finish Button
		DEFINE_PROPERTY_INTERFACE(pagecontrol_finish_button_text,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());
				auto text = Label_GetText(*gwen_control.FinishButton());

				output.SetString(text);
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				Label_SetText(*gwen_control.FinishButton(), value.m_cstring);
			},
			[](Control::i_control& control, cstring value)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				Label_SetText(*gwen_control.FinishButton(), value);
			}
		);

		DEFINE_PROPERTY_INTERFACE(pagecontrol_finish_button_typeface,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());
				auto text = Label_GetTextTypeface(*gwen_control.FinishButton());

				output.SetString(text.c_str());
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				Label_SetTextTypeface(*gwen_control.FinishButton(), value.m_cstring);
			},
			[](Control::i_control& control, cstring value)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				Label_SetTextTypeface(*gwen_control.FinishButton(), value);
			}
		);

		DEFINE_PROPERTY_INTERFACE(pagecontrol_finish_button_size,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				output.m_int32 = Label_GetTextSize(*gwen_control.FinishButton());
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				Label_SetTextSize(*gwen_control.FinishButton(), value.m_real);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				real value;
				ValueConversion::FromString(value_string, value);
				Label_SetTextSize(*gwen_control.FinishButton(), value);
			}
		);

		DEFINE_PROPERTY_INTERFACE(pagecontrol_finish_button_bold,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				output.m_bool = Label_GetTextBold(*gwen_control.FinishButton());
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				return Label_SetTextBold(*gwen_control.FinishButton(), value.m_bool);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				bool value;
				ValueConversion::FromString(value_string, value);
				return Label_SetTextBold(*gwen_control.FinishButton(), value);
			}
		);

		DEFINE_PROPERTY_INTERFACE(pagecontrol_finish_button_color,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				output.m_argb_color = Label_GetTextColor(*gwen_control.FinishButton());
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				Label_SetTextColor(*gwen_control.FinishButton(), value.m_argb_color);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				argb_color value;
				ValueConversion::FromString(value_string, value);
				Label_SetTextColor(*gwen_control.FinishButton(), value);
			}
		);

		DEFINE_PROPERTY_INTERFACE(pagecontrol_finish_button_vertical_alignment,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				output.m_int32 = Label_GetVerticalAlignment(*gwen_control.FinishButton());
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				Label_SetVerticalAlignment(*gwen_control.FinishButton(), value.m_int32);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				Label_SetVerticalAlignment(*gwen_control.FinishButton(), value_string);
			}
		);

		DEFINE_PROPERTY_INTERFACE(pagecontrol_finish_button_horizontal_alignment,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				output.m_int32 = Label_GetHorizontalAlignment(*gwen_control.FinishButton());
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				Label_SetHorizontalAlignment(*gwen_control.FinishButton(), value.m_int32);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				Label_SetHorizontalAlignment(*gwen_control.FinishButton(), value_string);
			}
		);

		DEFINE_PROPERTY_INTERFACE(pagecontrol_finish_button_wrap,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				output.m_bool = Label_GetWrap(*gwen_control.FinishButton());
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				return Label_SetWrap(*gwen_control.FinishButton(), value.m_bool);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				bool value;
				ValueConversion::FromString(value_string, value);
				return Label_SetWrap(*gwen_control.FinishButton(), value);
			}
		);

		DEFINE_PROPERTY_INTERFACE(pagecontrol_finish_button_padding,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				output.m_rectangle2d = Label_GetTextPadding(*gwen_control.FinishButton());
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				Label_SetTextPadding(*gwen_control.FinishButton(), value.m_rectangle2d);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				rectangle2d value;
				ValueConversion::FromString(value_string, value);
				Label_SetTextPadding(*gwen_control.FinishButton(), value);
			}
		);
#pragma endregion

		DEFINE_PROPERTY_INTERFACE(pagecontrol_page_count,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				output.m_uint32 = gwen_control.GetPageCount();
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				gwen_control.SetPageCount(value.m_uint32);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				uint32 value;
				ValueConversion::FromString(value_string, value);
				gwen_control.SetPageCount(value);
			}
		);

		DEFINE_PROPERTY_INTERFACE(pagecontrol_use_finish_button,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				output.m_bool = gwen_control.GetUseFinishButton();
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				gwen_control.SetUseFinishButton(value.m_bool);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(PageControl, control.GetControlPtr());

				bool value;
				ValueConversion::FromString(value_string, value);
				gwen_control.SetUseFinishButton(value);
			}
		);

		Gwen::Controls::Base* c_control_builder_gwen_pagecontrol::CreateInstance(Gwen::Controls::Base* parent) const
		{
			return new Gwen::Controls::PageControl(parent);
		}

		void c_control_builder_gwen_pagecontrol::SetDefaultValues(Gwen::Controls::Base* control) const
		{
			c_control_builder_gwen_base::SetDefaultValues(control);

			auto& gwen_control = GWEN_CTRL_REF(PageControl, control);

			gwen_control.SetPageCount(0);
			gwen_control.SetUseFinishButton(false);
			gwen_control.SetPageFormatString("NOT_SET %i %i");

			Label_SetTextTypeface(*gwen_control.Label(), "Candara");
			Label_SetTextSize(*gwen_control.Label(), 16);
			Label_SetTextBold(*gwen_control.Label(), false);
			Label_SetHorizontalAlignment(*gwen_control.Label(), Gwen::Pos::Left);
			Label_SetVerticalAlignment(*gwen_control.Label(), Gwen::Pos::CenterV);
			Label_SetWrap(*gwen_control.Label(), false);
			Label_SetTextPadding(*gwen_control.Label(), rectangle2d { 0, 0, 0, 0 });
			Label_SetTextColor(*gwen_control.Label(), argb_color { 255, 0, 0, 0 });

			Label_SetText(*gwen_control.BackButton(), "NOT_SET");
			Label_SetTextTypeface(*gwen_control.BackButton(), "Candara");
			Label_SetTextSize(*gwen_control.BackButton(), 16);
			Label_SetTextBold(*gwen_control.BackButton(), false);
			Label_SetHorizontalAlignment(*gwen_control.BackButton(), Gwen::Pos::Left);
			Label_SetVerticalAlignment(*gwen_control.BackButton(), Gwen::Pos::CenterV);
			Label_SetWrap(*gwen_control.BackButton(), false);
			Label_SetTextPadding(*gwen_control.BackButton(), rectangle2d { 0, 0, 0, 0 });
			Label_SetTextColor(*gwen_control.BackButton(), argb_color { 255, 0, 0, 0 });

			Label_SetText(*gwen_control.NextButton(), "NOT_SET");
			Label_SetTextTypeface(*gwen_control.NextButton(), "Candara");
			Label_SetTextSize(*gwen_control.NextButton(), 16);
			Label_SetTextBold(*gwen_control.NextButton(), false);
			Label_SetHorizontalAlignment(*gwen_control.NextButton(), Gwen::Pos::Left);
			Label_SetVerticalAlignment(*gwen_control.NextButton(), Gwen::Pos::CenterV);
			Label_SetWrap(*gwen_control.NextButton(), false);
			Label_SetTextPadding(*gwen_control.NextButton(), rectangle2d { 0, 0, 0, 0 });
			Label_SetTextColor(*gwen_control.NextButton(), argb_color { 255, 0, 0, 0 });

			Label_SetText(*gwen_control.FinishButton(), "NOT_SET");
			Label_SetTextTypeface(*gwen_control.FinishButton(), "Candara");
			Label_SetTextSize(*gwen_control.FinishButton(), 16);
			Label_SetTextBold(*gwen_control.FinishButton(), false);
			Label_SetHorizontalAlignment(*gwen_control.FinishButton(), Gwen::Pos::Left);
			Label_SetVerticalAlignment(*gwen_control.FinishButton(), Gwen::Pos::CenterV);
			Label_SetWrap(*gwen_control.FinishButton(), false);
			Label_SetTextPadding(*gwen_control.FinishButton(), rectangle2d { 0, 0, 0, 0 });
			Label_SetTextColor(*gwen_control.FinishButton(), argb_color { 255, 0, 0, 0 });
		}

		void c_control_builder_gwen_pagecontrol::GetPropertyInterfaces(Control::property_interface_map_t& property_interfaces) const
		{
			c_control_builder_gwen_base::GetPropertyInterfaces(property_interfaces);
			
			property_interfaces[K_PROPERTY_TEXTFORMAT_ID] = GET_PROPERTY_INTERFACE(pagecontrol_text_format);
			property_interfaces[K_PROPERTY_TEXTTYPEFACE_ID] = GET_PROPERTY_INTERFACE(pagecontrol_text_typeface);
			property_interfaces[K_PROPERTY_TEXTSIZE_ID] = GET_PROPERTY_INTERFACE(pagecontrol_text_size);
			property_interfaces[K_PROPERTY_TEXTBOLD_ID] = GET_PROPERTY_INTERFACE(pagecontrol_text_bold);
			property_interfaces[K_PROPERTY_TEXTCOLOR_ID] = GET_PROPERTY_INTERFACE(pagecontrol_text_color);
			property_interfaces[K_PROPERTY_TEXTVALIGNMENT_ID] = GET_PROPERTY_INTERFACE(pagecontrol_text_vertical_alignment);
			property_interfaces[K_PROPERTY_TEXTHALIGNMENT_ID] = GET_PROPERTY_INTERFACE(pagecontrol_text_horizontal_alignment);
			property_interfaces[K_PROPERTY_TEXTWRAP_ID] = GET_PROPERTY_INTERFACE(pagecontrol_text_wrap);
			property_interfaces[K_PROPERTY_TEXTPADDING_ID] = GET_PROPERTY_INTERFACE(pagecontrol_text_padding);
			
			property_interfaces[K_PROPERTY_BACKBUTTONTEXT_ID] = GET_PROPERTY_INTERFACE(pagecontrol_back_button_text);
			property_interfaces[K_PROPERTY_BACKBUTTONTEXTTYPEFACE_ID] = GET_PROPERTY_INTERFACE(pagecontrol_back_button_typeface);
			property_interfaces[K_PROPERTY_BACKBUTTONTEXTSIZE_ID] = GET_PROPERTY_INTERFACE(pagecontrol_back_button_size);
			property_interfaces[K_PROPERTY_BACKBUTTONTEXTBOLD_ID] = GET_PROPERTY_INTERFACE(pagecontrol_back_button_bold);
			property_interfaces[K_PROPERTY_BACKBUTTONTEXTCOLOR_ID] = GET_PROPERTY_INTERFACE(pagecontrol_back_button_color);
			property_interfaces[K_PROPERTY_BACKBUTTONTEXTVALIGNMENT_ID] = GET_PROPERTY_INTERFACE(pagecontrol_back_button_vertical_alignment);
			property_interfaces[K_PROPERTY_BACKBUTTONTEXTHALIGNMENT_ID] = GET_PROPERTY_INTERFACE(pagecontrol_back_button_horizontal_alignment);
			property_interfaces[K_PROPERTY_BACKBUTTONTEXTWRAP_ID] = GET_PROPERTY_INTERFACE(pagecontrol_back_button_wrap);
			property_interfaces[K_PROPERTY_BACKBUTTONTEXTPADDING_ID] = GET_PROPERTY_INTERFACE(pagecontrol_back_button_padding);
			
			property_interfaces[K_PROPERTY_NEXTBUTTONTEXT_ID] = GET_PROPERTY_INTERFACE(pagecontrol_next_button_text);
			property_interfaces[K_PROPERTY_NEXTBUTTONTEXTTYPEFACE_ID] = GET_PROPERTY_INTERFACE(pagecontrol_next_button_typeface);
			property_interfaces[K_PROPERTY_NEXTBUTTONTEXTSIZE_ID] = GET_PROPERTY_INTERFACE(pagecontrol_next_button_size);
			property_interfaces[K_PROPERTY_NEXTBUTTONTEXTBOLD_ID] = GET_PROPERTY_INTERFACE(pagecontrol_next_button_bold);
			property_interfaces[K_PROPERTY_NEXTBUTTONTEXTCOLOR_ID] = GET_PROPERTY_INTERFACE(pagecontrol_next_button_color);
			property_interfaces[K_PROPERTY_NEXTBUTTONTEXTVALIGNMENT_ID] = GET_PROPERTY_INTERFACE(pagecontrol_next_button_vertical_alignment);
			property_interfaces[K_PROPERTY_NEXTBUTTONTEXTHALIGNMENT_ID] = GET_PROPERTY_INTERFACE(pagecontrol_next_button_horizontal_alignment);
			property_interfaces[K_PROPERTY_NEXTBUTTONTEXTWRAP_ID] = GET_PROPERTY_INTERFACE(pagecontrol_next_button_wrap);
			property_interfaces[K_PROPERTY_NEXTBUTTONTEXTPADDING_ID] = GET_PROPERTY_INTERFACE(pagecontrol_next_button_padding);
			
			property_interfaces[K_PROPERTY_FINISHBUTTONTEXT_ID] = GET_PROPERTY_INTERFACE(pagecontrol_finish_button_text);
			property_interfaces[K_PROPERTY_FINISHBUTTONTEXTTYPEFACE_ID] = GET_PROPERTY_INTERFACE(pagecontrol_finish_button_typeface);
			property_interfaces[K_PROPERTY_FINISHBUTTONTEXTSIZE_ID] = GET_PROPERTY_INTERFACE(pagecontrol_finish_button_size);
			property_interfaces[K_PROPERTY_FINISHBUTTONTEXTBOLD_ID] = GET_PROPERTY_INTERFACE(pagecontrol_finish_button_bold);
			property_interfaces[K_PROPERTY_FINISHBUTTONTEXTCOLOR_ID] = GET_PROPERTY_INTERFACE(pagecontrol_finish_button_color);
			property_interfaces[K_PROPERTY_FINISHBUTTONTEXTVALIGNMENT_ID] = GET_PROPERTY_INTERFACE(pagecontrol_finish_button_vertical_alignment);
			property_interfaces[K_PROPERTY_FINISHBUTTONTEXTHALIGNMENT_ID] = GET_PROPERTY_INTERFACE(pagecontrol_finish_button_horizontal_alignment);
			property_interfaces[K_PROPERTY_FINISHBUTTONTEXTWRAP_ID] = GET_PROPERTY_INTERFACE(pagecontrol_finish_button_wrap);
			property_interfaces[K_PROPERTY_FINISHBUTTONTEXTPADDING_ID] = GET_PROPERTY_INTERFACE(pagecontrol_finish_button_padding);

			property_interfaces[K_PROPERTY_PAGECOUNT_ID] = GET_PROPERTY_INTERFACE(pagecontrol_page_count);
			property_interfaces[K_PROPERTY_USEFINISHBUTTON_ID] = GET_PROPERTY_INTERFACE(pagecontrol_use_finish_button);

		}

		void c_control_builder_gwen_pagecontrol::GetEventHandlers(Gwen::Controls::Base* control, Control::event_handler_map_t& event_handlers) const
		{
			c_control_builder_gwen_base::GetEventHandlers(control, event_handlers);
			
			auto& gwen_control = GWEN_CTRL_REF(PageControl, control);
			
			event_handlers[K_EVENT_PAGECHANGED_ID] = std::make_unique<c_event_handler_gwen>(gwen_control.onPageChanged,
				[](Gwen::Event::Info& info, Control::s_interface_value& output)
				{
					auto& gwen_control = GWEN_CTRL_REF(PageControl, info.ControlCaller);

					output.m_uint32 = gwen_control.GetPageNumber();
				});
			event_handlers[K_EVENT_FINISH_ID] = std::make_unique<c_event_handler_gwen>(gwen_control.onFinish);
		}

		Control::control_ptr_t c_control_builder_gwen_pagecontrol::Build(ControlFactory::c_control_factory& control_registry
			, Definitions::c_control_definition& control_definition
			, Control::i_control& parent) const
		{
			// Build the page control itself
			auto control = c_control_builder_gwen_base::Build(control_registry, control_definition, parent);

			// Set the page count
			auto& gwen_control = GWEN_CTRL_REF(PageControl, control->GetControlPtr());
			gwen_control.SetPageCount(control_definition.m_pages.Get().size());

			// Build the pages
			for(uint32 i = 0; i < control_definition.m_pages.Get().size(); i++)
			{
				auto& page = control_definition.m_pages.Get()[i];

				// Create a new osui control for the page base control
				auto* page_base = gwen_control.GetPage(i);
				Control::control_ptr_t page_control = std::make_unique<c_control_gwen>(*control
					, page_base
					, Control::property_interface_map_t()
					, Control::event_handler_map_t());

				control->AddControl(page_control);

				// Create the page's children and add them to the base
				BuildChildren(control_registry, page.m_controls.Get(), *page_control);
			}

			return control;
		}
	};};};};
};
#endif