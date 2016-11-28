/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI/GwenUI/ControlBuilders/c_control_builder_gwen_textbox.hpp"

#if !PLATFORM_IS_DEDI

#include <Gwen/Controls/TextBox.h>
#include <YeloLib/cseries/value_conversion.hpp>

#include "Interface/OpenSauceUI/Control/control_property_ids.hpp"
#include "Interface/OpenSauceUI/Control/control_event_ids.hpp"
#include "Interface/OpenSauceUI/Control/c_control_base.hpp"
#include "Interface/OpenSauceUI/GwenUI/c_event_handler_gwen.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace GwenUI { namespace ControlBuilders
	{
		DEFINE_PROPERTY_INTERFACE(textbox_editable,
			nullptr,
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(TextBox, control.GetControlPtr());

				gwen_control.SetEditable(value.m_bool);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(TextBox, control.GetControlPtr());

				bool value;
				ValueConversion::FromString(value_string, value);
				gwen_control.SetEditable(value);
			}
		);

		Gwen::Controls::Base* c_control_builder_gwen_textbox::CreateInstance(Gwen::Controls::Base* parent) const
		{
			return new Gwen::Controls::TextBox(parent);
		}

		void c_control_builder_gwen_textbox::SetDefaultValues(Gwen::Controls::Base* control) const
		{
			c_control_builder_gwen_label::SetDefaultValues(control);

			auto& gwen_control = GWEN_CTRL_REF(TextBox, control);
			gwen_control.SetEditable(true);
			gwen_control.SetText("");
		}

		void c_control_builder_gwen_textbox::GetPropertyInterfaces(Control::property_interface_map_t& property_interfaces) const
		{
			c_control_builder_gwen_label::GetPropertyInterfaces(property_interfaces);
			
			property_interfaces[K_PROPERTY_EDITABLE_ID] = GET_PROPERTY_INTERFACE(textbox_editable);
		}

		void c_control_builder_gwen_textbox::GetEventHandlers(Gwen::Controls::Base* control, Control::event_handler_map_t& event_handlers) const
		{
			c_control_builder_gwen_label::GetEventHandlers(control, event_handlers);
			
			auto& gwen_control = GWEN_CTRL_REF(TextBox, control);
			
			event_handlers[K_EVENT_TEXTCHANGED_ID] = std::make_unique<c_event_handler_gwen>(gwen_control.onTextChanged);
			event_handlers[K_EVENT_RETURNPRESSED_ID] = std::make_unique<c_event_handler_gwen>(gwen_control.onReturnPressed);
		}
	};};};};
};
#endif