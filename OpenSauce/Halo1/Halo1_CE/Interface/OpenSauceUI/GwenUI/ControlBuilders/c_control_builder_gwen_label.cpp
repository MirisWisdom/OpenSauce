/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI/GwenUI/ControlBuilders/c_control_builder_gwen_label.hpp"

#if !PLATFORM_IS_DEDI

#include <Gwen/Controls/Label.h>
#include <YeloLib/cseries/value_conversion.hpp>

#include "Interface/OpenSauceUI/Control/control_property_ids.hpp"
#include "Interface/OpenSauceUI/Control/c_control_base.hpp"
#include "Interface/OpenSauceUI/GwenUI/ControlBuilders/gwen_control_util_label.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace GwenUI { namespace ControlBuilders
	{
		DEFINE_PROPERTY_INTERFACE(label_text,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(Label, control.GetControlPtr());
				auto text = Label_GetText(gwen_control);

				output.SetString(text);
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(Label, control.GetControlPtr());

				Label_SetText(gwen_control, value.m_cstring);
			},
			[](Control::i_control& control, cstring value)
			{
				auto& gwen_control = GWEN_CTRL_REF(Label, control.GetControlPtr());

				Label_SetText(gwen_control, value);
			}
		);

		DEFINE_PROPERTY_INTERFACE(label_text_typeface,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(Label, control.GetControlPtr());
				auto text = Label_GetTextTypeface(gwen_control);
				
				output.SetString(text.c_str());
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(Label, control.GetControlPtr());

				Label_SetTextTypeface(gwen_control, value.m_cstring);
			},
			[](Control::i_control& control, cstring value)
			{
				auto& gwen_control = GWEN_CTRL_REF(Label, control.GetControlPtr());

				Label_SetTextTypeface(gwen_control, value);
			}
		);

		DEFINE_PROPERTY_INTERFACE(label_text_size,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(Label, control.GetControlPtr());

				output.m_int32 = Label_GetTextSize(gwen_control);
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(Label, control.GetControlPtr());

				Label_SetTextSize(gwen_control, value.m_real);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(Label, control.GetControlPtr());

				real value;
				ValueConversion::FromString(value_string, value);
				Label_SetTextSize(gwen_control, value);
			}
		);

		DEFINE_PROPERTY_INTERFACE(label_text_bold,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(Label, control.GetControlPtr());

				output.m_bool = Label_GetTextBold(gwen_control);
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(Label, control.GetControlPtr());

				return Label_SetTextBold(gwen_control, value.m_bool);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(Label, control.GetControlPtr());

				bool value;
				ValueConversion::FromString(value_string, value);
				return Label_SetTextBold(gwen_control, value);
			}
		);

		DEFINE_PROPERTY_INTERFACE(label_text_color,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(Label, control.GetControlPtr());

				output.m_argb_color = Label_GetTextColor(gwen_control);
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(Label, control.GetControlPtr());

				Label_SetTextColor(gwen_control, value.m_argb_color);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(Label, control.GetControlPtr());

				argb_color value;
				ValueConversion::FromString(value_string, value);
				Label_SetTextColor(gwen_control, value);
			}
		);

		DEFINE_PROPERTY_INTERFACE(label_text_vertical_alignment,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(Label, control.GetControlPtr());

				output.m_int32 = Label_GetVerticalAlignment(gwen_control);
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(Label, control.GetControlPtr());

				Label_SetVerticalAlignment(gwen_control, value.m_int32);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(Label, control.GetControlPtr());

				Label_SetVerticalAlignment(gwen_control, value_string);
			}
		);

		DEFINE_PROPERTY_INTERFACE(label_text_horizontal_alignment,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(Label, control.GetControlPtr());

				output.m_int32 = Label_GetHorizontalAlignment(gwen_control);
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(Label, control.GetControlPtr());

				Label_SetHorizontalAlignment(gwen_control, value.m_int32);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(Label, control.GetControlPtr());

				Label_SetHorizontalAlignment(gwen_control, value_string);
			}
		);

		DEFINE_PROPERTY_INTERFACE(label_text_wrap,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(Label, control.GetControlPtr());

				output.m_bool = Label_GetWrap(gwen_control);
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(Label, control.GetControlPtr());

				return Label_SetWrap(gwen_control, value.m_bool);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(Label, control.GetControlPtr());

				bool value;
				ValueConversion::FromString(value_string, value);
				return Label_SetWrap(gwen_control, value);
			}
		);

		DEFINE_PROPERTY_INTERFACE(label_text_padding,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(Label, control.GetControlPtr());

				output.m_rectangle2d = Label_GetTextPadding(gwen_control);
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(Label, control.GetControlPtr());

				Label_SetTextPadding(gwen_control, value.m_rectangle2d);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(Label, control.GetControlPtr());

				rectangle2d value;
				ValueConversion::FromString(value_string, value);
				Label_SetTextPadding(gwen_control, value);
			}
		);

		Gwen::Controls::Base* c_control_builder_gwen_label::CreateInstance(Gwen::Controls::Base* parent) const
		{
			return new Gwen::Controls::Label(parent);
		}

		void c_control_builder_gwen_label::SetDefaultValues(Gwen::Controls::Base* control) const
		{
			c_control_builder_gwen_base::SetDefaultValues(control);

			auto& gwen_control = GWEN_CTRL_REF(Label, control);

			Label_SetText(gwen_control, "NOT_SET");
			Label_SetTextTypeface(gwen_control, "Candara");
			Label_SetTextSize(gwen_control, 16);
			Label_SetTextBold(gwen_control, false);
			Label_SetHorizontalAlignment(gwen_control, Gwen::Pos::Left);
			Label_SetVerticalAlignment(gwen_control, Gwen::Pos::CenterV);
			Label_SetWrap(gwen_control, false);
			Label_SetTextPadding(gwen_control, rectangle2d { 0, 0, 0, 0 });
			Label_SetTextColor(gwen_control, argb_color { 255, 0, 0, 0 });
		}

		void c_control_builder_gwen_label::GetPropertyInterfaces(Control::property_interface_map_t& property_interfaces) const
		{
			c_control_builder_gwen_base::GetPropertyInterfaces(property_interfaces);
			
			property_interfaces[K_PROPERTY_TEXT_ID] = GET_PROPERTY_INTERFACE(label_text);
			property_interfaces[K_PROPERTY_TEXTTYPEFACE_ID] = GET_PROPERTY_INTERFACE(label_text_typeface);
			property_interfaces[K_PROPERTY_TEXTSIZE_ID] = GET_PROPERTY_INTERFACE(label_text_size);
			property_interfaces[K_PROPERTY_TEXTBOLD_ID] = GET_PROPERTY_INTERFACE(label_text_bold);
			property_interfaces[K_PROPERTY_TEXTCOLOR_ID] = GET_PROPERTY_INTERFACE(label_text_color);
			property_interfaces[K_PROPERTY_TEXTVALIGNMENT_ID] = GET_PROPERTY_INTERFACE(label_text_vertical_alignment);
			property_interfaces[K_PROPERTY_TEXTHALIGNMENT_ID] = GET_PROPERTY_INTERFACE(label_text_horizontal_alignment);
			property_interfaces[K_PROPERTY_TEXTWRAP_ID] = GET_PROPERTY_INTERFACE(label_text_wrap);
			property_interfaces[K_PROPERTY_TEXTPADDING_ID] = GET_PROPERTY_INTERFACE(label_text_padding);
		}
	};};};};
};
#endif