/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI/GwenUI/ControlBuilders/c_control_builder_gwen_checkbox.hpp"

#if !PLATFORM_IS_DEDI

#include <Gwen/Controls/Checkbox.h>
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
		DEFINE_PROPERTY_INTERFACE(checkbox_checked,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(CheckBox, control.GetControlPtr());

				output.m_bool = gwen_control.IsChecked();
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(CheckBox, control.GetControlPtr());

				gwen_control.SetChecked(value.m_bool);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(CheckBox, control.GetControlPtr());

				bool value;
				ValueConversion::FromString(value_string, value);
				gwen_control.SetChecked(value);
			}
		);

		Gwen::Controls::Base* c_control_builder_gwen_checkbox::CreateInstance(Gwen::Controls::Base* parent) const
		{
			return new Gwen::Controls::CheckBox(parent);
		}

		void c_control_builder_gwen_checkbox::SetDefaultValues(Gwen::Controls::Base* control) const
		{
			c_control_builder_gwen_base::SetDefaultValues(control);

			auto& gwen_control = GWEN_CTRL_REF(CheckBox, control);

			gwen_control.SetChecked(false);
		}

		void c_control_builder_gwen_checkbox::GetPropertyInterfaces(Control::property_interface_map_t& property_interfaces) const
		{
			c_control_builder_gwen_base::GetPropertyInterfaces(property_interfaces);
			
			property_interfaces[K_PROPERTY_CHECKED_ID] = GET_PROPERTY_INTERFACE(checkbox_checked);
		}

		void c_control_builder_gwen_checkbox::GetEventHandlers(Gwen::Controls::Base* control, Control::event_handler_map_t& event_handlers) const
		{
			c_control_builder_gwen_base::GetEventHandlers(control, event_handlers);

			auto& gwen_control = GWEN_CTRL_REF(CheckBox, control);

			event_handlers[K_EVENT_CHECKED_ID] = std::make_unique<c_event_handler_gwen>(gwen_control.onChecked);
			event_handlers[K_EVENT_UNCHECKED_ID] = std::make_unique<c_event_handler_gwen>(gwen_control.onUnChecked);
			event_handlers[K_EVENT_CHECKCHANGED_ID] = std::make_unique<c_event_handler_gwen>(gwen_control.onCheckChanged,
				[](Gwen::Event::Info& info, Control::s_interface_value& output)
				{
					auto& gwen_control = GWEN_CTRL_REF(CheckBox, info.ControlCaller);

					output.m_bool = gwen_control.IsChecked();
				});
		}
	};};};};
};
#endif