/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI/GwenUI/ControlBuilders/c_control_builder_gwen_checkboxwithlabel.hpp"

#if !PLATFORM_IS_DEDI

#include <Gwen/Controls/CheckBox.h>
#include <YeloLib/cseries/value_conversion.hpp>

#include "Interface/OpenSauceUI/Control/control_property_ids.hpp"
#include "Interface/OpenSauceUI/Control/control_event_ids.hpp"
#include "Interface/OpenSauceUI/Control/c_control_base.hpp"
#include "Interface/OpenSauceUI/GwenUI/ControlBuilders/gwen_control_util_label.hpp"
#include "Interface/OpenSauceUI/GwenUI/c_event_handler_gwen.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace GwenUI { namespace ControlBuilders
	{
		DEFINE_PROPERTY_INTERFACE(checkboxwithlabel_text,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(CheckBoxWithLabel, control.GetControlPtr());
				auto text = Label_GetText(*gwen_control.Label());

				output.SetString(text);
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(CheckBoxWithLabel, control.GetControlPtr());

				Label_SetText(*gwen_control.Label(), value.m_cstring);
			},
			[](Control::i_control& control, cstring value)
			{
				auto& gwen_control = GWEN_CTRL_REF(CheckBoxWithLabel, control.GetControlPtr());

				Label_SetText(*gwen_control.Label(), value);
			}
		);

		DEFINE_PROPERTY_INTERFACE(checkboxwithlabel_text_typeface,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(CheckBoxWithLabel, control.GetControlPtr());
				auto text = Label_GetTextTypeface(*gwen_control.Label());

				output.SetString(text.c_str());
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(CheckBoxWithLabel, control.GetControlPtr());

				Label_SetTextTypeface(*gwen_control.Label(), value.m_cstring);
			},
			[](Control::i_control& control, cstring value)
			{
				auto& gwen_control = GWEN_CTRL_REF(CheckBoxWithLabel, control.GetControlPtr());

				Label_SetTextTypeface(*gwen_control.Label(), value);
			}
		);

		DEFINE_PROPERTY_INTERFACE(checkboxwithlabel_text_size,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(CheckBoxWithLabel, control.GetControlPtr());

				output.m_int32 = Label_GetTextSize(*gwen_control.Label());
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(CheckBoxWithLabel, control.GetControlPtr());

				Label_SetTextSize(*gwen_control.Label(), value.m_real);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(CheckBoxWithLabel, control.GetControlPtr());

				real value;
				ValueConversion::FromString(value_string, value);
				Label_SetTextSize(*gwen_control.Label(), value);
			}
		);

		DEFINE_PROPERTY_INTERFACE(checkboxwithlabel_text_bold,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(CheckBoxWithLabel, control.GetControlPtr());

				output.m_bool = Label_GetTextBold(*gwen_control.Label());
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(CheckBoxWithLabel, control.GetControlPtr());

				return Label_SetTextBold(*gwen_control.Label(), value.m_bool);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(CheckBoxWithLabel, control.GetControlPtr());

				bool value;
				ValueConversion::FromString(value_string, value);
				return Label_SetTextBold(*gwen_control.Label(), value);
			}
		);

		DEFINE_PROPERTY_INTERFACE(checkboxwithlabel_text_vertical_align,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(CheckBoxWithLabel, control.GetControlPtr());

				output.m_int32 = Label_GetVerticalAlignment(*gwen_control.Label());
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(CheckBoxWithLabel, control.GetControlPtr());

				Label_SetVerticalAlignment(*gwen_control.Label(), value.m_int32);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(CheckBoxWithLabel, control.GetControlPtr());

				Label_SetVerticalAlignment(*gwen_control.Label(), value_string);
			}
		);

		DEFINE_PROPERTY_INTERFACE(checkboxwithlabel_text_horizontal_align,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(CheckBoxWithLabel, control.GetControlPtr());

				output.m_int32 = Label_GetHorizontalAlignment(*gwen_control.Label());
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(CheckBoxWithLabel, control.GetControlPtr());

				Label_SetHorizontalAlignment(*gwen_control.Label(), value.m_int32);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(CheckBoxWithLabel, control.GetControlPtr());

				Label_SetHorizontalAlignment(*gwen_control.Label(), value_string);
			}
		);

		DEFINE_PROPERTY_INTERFACE(checkboxwithlabel_text_color,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(CheckBoxWithLabel, control.GetControlPtr());

				output.m_argb_color = Label_GetTextColor(*gwen_control.Label());
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(CheckBoxWithLabel, control.GetControlPtr());

				Label_SetTextColor(*gwen_control.Label(), value.m_argb_color);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(CheckBoxWithLabel, control.GetControlPtr());

				argb_color value;
				ValueConversion::FromString(value_string, value);
				Label_SetTextColor(*gwen_control.Label(), value);
			}
		);

		DEFINE_PROPERTY_INTERFACE(checkboxwithlabel_text_vertical_alignment,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(CheckBoxWithLabel, control.GetControlPtr());

				output.m_int32 = Label_GetVerticalAlignment(*gwen_control.Label());
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(CheckBoxWithLabel, control.GetControlPtr());

				Label_SetVerticalAlignment(*gwen_control.Label(), value.m_int32);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(CheckBoxWithLabel, control.GetControlPtr());

				Label_SetVerticalAlignment(*gwen_control.Label(), value_string);
			}
		);

		DEFINE_PROPERTY_INTERFACE(checkboxwithlabel_text_horizontal_alignment,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(CheckBoxWithLabel, control.GetControlPtr());

				output.m_int32 = Label_GetHorizontalAlignment(*gwen_control.Label());
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(CheckBoxWithLabel, control.GetControlPtr());

				Label_SetHorizontalAlignment(*gwen_control.Label(), value.m_int32);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(CheckBoxWithLabel, control.GetControlPtr());

				Label_SetHorizontalAlignment(*gwen_control.Label(), value_string);
			}
		);

		DEFINE_PROPERTY_INTERFACE(checkboxwithlabel_text_wrap,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(CheckBoxWithLabel, control.GetControlPtr());

				output.m_bool = Label_GetWrap(*gwen_control.Label());
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(CheckBoxWithLabel, control.GetControlPtr());

				return Label_SetWrap(*gwen_control.Label(), value.m_bool);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(CheckBoxWithLabel, control.GetControlPtr());

				bool value;
				ValueConversion::FromString(value_string, value);
				return Label_SetWrap(*gwen_control.Label(), value);
			}
		);

		DEFINE_PROPERTY_INTERFACE(checkboxwithlabel_text_padding,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(CheckBoxWithLabel, control.GetControlPtr());

				output.m_rectangle2d = Label_GetTextPadding(*gwen_control.Label());
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(CheckBoxWithLabel, control.GetControlPtr());

				Label_SetTextPadding(*gwen_control.Label(), value.m_rectangle2d);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(CheckBoxWithLabel, control.GetControlPtr());

				rectangle2d value;
				ValueConversion::FromString(value_string, value);
				Label_SetTextPadding(*gwen_control.Label(), value);
			}
		);

		DEFINE_PROPERTY_INTERFACE(checkboxwithlabel_checked,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(CheckBoxWithLabel, control.GetControlPtr());

				output.m_bool = gwen_control.Checkbox()->IsChecked();
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(CheckBoxWithLabel, control.GetControlPtr());

				gwen_control.Checkbox()->SetChecked(value.m_bool);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(CheckBoxWithLabel, control.GetControlPtr());

				bool value;
				ValueConversion::FromString(value_string, value);
				gwen_control.Checkbox()->SetChecked(value);
			}
		);

		DEFINE_PROPERTY_INTERFACE(checkboxwithlabel_disabled,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(CheckBoxWithLabel, control.GetControlPtr());

				output.m_bool = gwen_control.Checkbox()->IsDisabled();
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(CheckBoxWithLabel, control.GetControlPtr());

				gwen_control.Checkbox()->SetDisabled(value.m_bool);
				gwen_control.Checkbox()->SetDisabled(value.m_bool);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(CheckBoxWithLabel, control.GetControlPtr());

				bool value;
				ValueConversion::FromString(value_string, value);
				gwen_control.Checkbox()->SetDisabled(value);
				gwen_control.Checkbox()->SetDisabled(value);
			}
		);

		Gwen::Controls::Base* c_control_builder_gwen_checkboxwithlabel::CreateInstance(Gwen::Controls::Base* parent) const
		{
			return new Gwen::Controls::CheckBoxWithLabel(parent);
		}

		void c_control_builder_gwen_checkboxwithlabel::SetDefaultValues(Gwen::Controls::Base* control) const
		{
			c_control_builder_gwen_base::SetDefaultValues(control);
			
			auto& gwen_control = GWEN_CTRL_REF(CheckBoxWithLabel, control);
			auto& label = *gwen_control.Label();

			Label_SetText(label, "NOT_SET");
			Label_SetTextTypeface(label,  "Candara");
			Label_SetTextSize(label, 16);
			Label_SetTextBold(label, false);
			Label_SetHorizontalAlignment(label, Gwen::Pos::Left);
			Label_SetVerticalAlignment(label, Gwen::Pos::CenterV);
			Label_SetWrap(label, false);
			Label_SetTextPadding(label, rectangle2d { 0, 0, 0, 0 });
			Label_SetTextColor(label, argb_color { 255, 0, 0, 0 });
		}

		void c_control_builder_gwen_checkboxwithlabel::GetPropertyInterfaces(Control::property_interface_map_t& property_interfaces) const
		{
			c_control_builder_gwen_base::GetPropertyInterfaces(property_interfaces);
			
			property_interfaces[K_PROPERTY_TEXT_ID] = GET_PROPERTY_INTERFACE(checkboxwithlabel_text);
			property_interfaces[K_PROPERTY_TEXTTYPEFACE_ID] = GET_PROPERTY_INTERFACE(checkboxwithlabel_text_typeface);
			property_interfaces[K_PROPERTY_TEXTSIZE_ID] = GET_PROPERTY_INTERFACE(checkboxwithlabel_text_size);
			property_interfaces[K_PROPERTY_TEXTBOLD_ID] = GET_PROPERTY_INTERFACE(checkboxwithlabel_text_bold);
			property_interfaces[K_PROPERTY_TEXTCOLOR_ID] = GET_PROPERTY_INTERFACE(checkboxwithlabel_text_color);
			property_interfaces[K_PROPERTY_TEXTVALIGNMENT_ID] = GET_PROPERTY_INTERFACE(checkboxwithlabel_text_vertical_alignment);
			property_interfaces[K_PROPERTY_TEXTHALIGNMENT_ID] = GET_PROPERTY_INTERFACE(checkboxwithlabel_text_horizontal_alignment);
			property_interfaces[K_PROPERTY_TEXTWRAP_ID] = GET_PROPERTY_INTERFACE(checkboxwithlabel_text_wrap);
			property_interfaces[K_PROPERTY_TEXTPADDING_ID] = GET_PROPERTY_INTERFACE(checkboxwithlabel_text_padding);
			property_interfaces[K_PROPERTY_CHECKED_ID] = GET_PROPERTY_INTERFACE(checkboxwithlabel_checked);
			property_interfaces[K_PROPERTY_DISABLED_ID] = GET_PROPERTY_INTERFACE(checkboxwithlabel_disabled);
		}

		void c_control_builder_gwen_checkboxwithlabel::GetEventHandlers(Gwen::Controls::Base* control, Control::event_handler_map_t& event_handlers) const
		{
			c_control_builder_gwen_base::GetEventHandlers(control, event_handlers);
			
			auto& gwen_control = GWEN_CTRL_REF(CheckBoxWithLabel, control);
			auto& checkbox = *gwen_control.Checkbox();

			event_handlers[K_EVENT_CHECKED_ID] = std::make_unique<c_event_handler_gwen>(checkbox.onChecked);
			event_handlers[K_EVENT_UNCHECKED_ID] = std::make_unique<c_event_handler_gwen>(checkbox.onUnChecked);
			event_handlers[K_EVENT_CHECKCHANGED_ID] = std::make_unique<c_event_handler_gwen>(checkbox.onCheckChanged,
				[](Gwen::Event::Info& info, Control::s_interface_value& output)
				{
					auto& gwen_control = GWEN_CTRL_REF(CheckBox, info.ControlCaller);

					output.m_bool = gwen_control.IsChecked();
				});
		}
	};};};};
};
#endif