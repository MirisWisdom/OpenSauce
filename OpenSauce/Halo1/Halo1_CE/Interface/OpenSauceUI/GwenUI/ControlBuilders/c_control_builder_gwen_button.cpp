/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI/GwenUI/ControlBuilders/c_control_builder_gwen_button.hpp"

#if !PLATFORM_IS_DEDI

#include <Gwen/Controls/Button.h>
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
		DEFINE_PROPERTY_INTERFACE(button_depressed,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(Button, control.GetControlPtr());

				output.m_bool = gwen_control.IsDepressed();
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(Button, control.GetControlPtr());

				gwen_control.SetDepressed(value.m_bool);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(Button, control.GetControlPtr());

				bool value;
				ValueConversion::FromString(value_string, value);
				gwen_control.SetDepressed(value);
			}
		);

		DEFINE_PROPERTY_INTERFACE(button_is_toggle,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(Button, control.GetControlPtr());

				output.m_bool = gwen_control.IsToggle();
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(Button, control.GetControlPtr());

				gwen_control.SetIsToggle(value.m_bool);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(Button, control.GetControlPtr());

				bool value;
				ValueConversion::FromString(value_string, value);
				gwen_control.SetIsToggle(value);
			}
		);

		DEFINE_PROPERTY_INTERFACE(button_toggle_state,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(Button, control.GetControlPtr());

				output.m_bool = gwen_control.GetToggleState();
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(Button, control.GetControlPtr());

				gwen_control.SetToggleState(value.m_bool);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(Button, control.GetControlPtr());

				bool value;
				ValueConversion::FromString(value_string, value);
				gwen_control.SetToggleState(value);
			}
		);

		DEFINE_PROPERTY_INTERFACE(button_image,
			nullptr,
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(Button, control.GetControlPtr());

				gwen_control.SetImage(value.m_cstring);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(Button, control.GetControlPtr());
				
				gwen_control.SetImage(value_string);
			}
		);

		DEFINE_PROPERTY_INTERFACE(button_image_alpha,
			nullptr,
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(Button, control.GetControlPtr());

				gwen_control.SetImageAlpha(value.m_real);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(Button, control.GetControlPtr());

				real value;
				ValueConversion::FromString(value_string, value);
				gwen_control.SetImageAlpha(value);
			}
		);

		Gwen::Controls::Base* c_control_builder_gwen_button::CreateInstance(Gwen::Controls::Base* parent) const
		{
			return new Gwen::Controls::Button(parent);
		}

		void c_control_builder_gwen_button::SetDefaultValues(Gwen::Controls::Base* control) const
		{
			c_control_builder_gwen_label::SetDefaultValues(control);

			auto& gwen_control = GWEN_CTRL_REF(Button, control);
			gwen_control.SetIsToggle(false);
		}

		void c_control_builder_gwen_button::GetPropertyInterfaces(Control::property_interface_map_t& property_interfaces) const
		{
			c_control_builder_gwen_label::GetPropertyInterfaces(property_interfaces);
			
			property_interfaces[K_PROPERTY_DEPRESSED_ID] = GET_PROPERTY_INTERFACE(button_depressed);
			property_interfaces[K_PROPERTY_ISTOGGLE_ID] = GET_PROPERTY_INTERFACE(button_is_toggle);
			property_interfaces[K_PROPERTY_TOGGLESTATE_ID] = GET_PROPERTY_INTERFACE(button_toggle_state);
			property_interfaces[K_PROPERTY_IMAGE_ID] = GET_PROPERTY_INTERFACE(button_image);
			property_interfaces[K_PROPERTY_IMAGEALPHA_ID] = GET_PROPERTY_INTERFACE(button_image_alpha);
		}

		void c_control_builder_gwen_button::GetEventHandlers(Gwen::Controls::Base* control, Control::event_handler_map_t& event_handlers) const
		{
			c_control_builder_gwen_label::GetEventHandlers(control, event_handlers);

			auto& gwen_control = GWEN_CTRL_REF(Button, control);
			
			event_handlers[K_EVENT_PRESS_ID] = std::make_unique<c_event_handler_gwen>(gwen_control.onPress);
			event_handlers[K_EVENT_RIGHTPRESS_ID] = std::make_unique<c_event_handler_gwen>(gwen_control.onRightPress);
			event_handlers[K_EVENT_DOWN_ID] = std::make_unique<c_event_handler_gwen>(gwen_control.onDown);
			event_handlers[K_EVENT_UP_ID] = std::make_unique<c_event_handler_gwen>(gwen_control.onUp);
			event_handlers[K_EVENT_DOUBLECLICK_ID] = std::make_unique<c_event_handler_gwen>(gwen_control.onDoubleClick);
			event_handlers[K_EVENT_TOGGLE_ID] = std::make_unique<c_event_handler_gwen>(gwen_control.onToggle,
				[](Gwen::Event::Info& info, Control::s_interface_value& output)
				{
					auto& gwen_control = GWEN_CTRL_REF(Button, info.ControlCaller);

					output.m_bool = gwen_control.GetToggleState();
				});
			event_handlers[K_EVENT_TOGGLEON_ID] = std::make_unique<c_event_handler_gwen>(gwen_control.onToggleOn);
			event_handlers[K_EVENT_TOGGLEOFF_ID] = std::make_unique<c_event_handler_gwen>(gwen_control.onToggleOff);
		}
	};};};};
};
#endif