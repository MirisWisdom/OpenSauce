/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI/GwenUI/ControlBuilders/c_control_builder_gwen_slider.hpp"

#if !PLATFORM_IS_DEDI

#include <Gwen/Controls/Slider.h>
#include <YeloLib/cseries/value_conversion.hpp>

#include "Interface/OpenSauceUI/Control/control_property_ids.hpp"
#include "Interface/OpenSauceUI/Control/control_event_ids.hpp"
#include "Interface/OpenSauceUI/Control/c_control_base.hpp"
#include "Interface/OpenSauceUI/GwenUI/c_event_handler_gwen.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace GwenUI { namespace ControlBuilders
	{
		DEFINE_PROPERTY_INTERFACE(slider_clamp_to_notches,
			nullptr,
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(Slider, control.GetControlPtr());

				gwen_control.SetClampToNotches(value.m_bool);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(Slider, control.GetControlPtr());

				bool value;
				ValueConversion::FromString(value_string, value);
				gwen_control.SetClampToNotches(value);
			}
		);

		DEFINE_PROPERTY_INTERFACE(slider_notch_count,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(Slider, control.GetControlPtr());

				output.m_int32 = gwen_control.GetNotchCount();
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(Slider, control.GetControlPtr());

				gwen_control.SetNotchCount(value.m_int32);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(Slider, control.GetControlPtr());

				int32 value;
				ValueConversion::FromString(value_string, value);
				gwen_control.SetNotchCount(value);
			}
		);

		DEFINE_PROPERTY_INTERFACE(slider_min,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(Slider, control.GetControlPtr());

				output.m_int32 = gwen_control.GetMin();
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(Slider, control.GetControlPtr());

				gwen_control.SetRange(value.m_real, gwen_control.GetMax());
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(Slider, control.GetControlPtr());

				real value;
				ValueConversion::FromString(value_string, value);
				gwen_control.SetRange(value, gwen_control.GetMax());
			}
		);

		DEFINE_PROPERTY_INTERFACE(slider_max,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(Slider, control.GetControlPtr());

				output.m_int32 = gwen_control.GetMax();
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(Slider, control.GetControlPtr());

				gwen_control.SetRange(gwen_control.GetMin(), value.m_real);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(Slider, control.GetControlPtr());

				real value;
				ValueConversion::FromString(value_string, value);
				gwen_control.SetRange(gwen_control.GetMin(), value);
			}
		);

		DEFINE_PROPERTY_INTERFACE(slider_value,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(Slider, control.GetControlPtr());

				output.m_real = gwen_control.GetFloatValue();
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(Slider, control.GetControlPtr());

				gwen_control.SetFloatValue(value.m_real);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(Slider, control.GetControlPtr());

				real value;
				ValueConversion::FromString(value_string, value);
				gwen_control.SetFloatValue(value);
			}
		);

		void c_control_builder_gwen_slider::SetDefaultValues(Gwen::Controls::Base* control) const
		{
			c_control_builder_gwen_base::SetDefaultValues(control);

			auto& gwen_control = GWEN_CTRL_REF(Slider, control);

			gwen_control.SetClampToNotches(false);
			gwen_control.SetNotchCount(0);
			gwen_control.SetRange(0.0f, 1.0f);
			gwen_control.SetFloatValue(0.5f);
		}

		void c_control_builder_gwen_slider::GetPropertyInterfaces(Control::property_interface_map_t& property_interfaces) const
		{
			c_control_builder_gwen_base::GetPropertyInterfaces(property_interfaces);

			property_interfaces[K_PROPERTY_CLAMPTONOTCHES_ID] = GET_PROPERTY_INTERFACE(slider_clamp_to_notches);
			property_interfaces[K_PROPERTY_NOTCHCOUNT_ID] = GET_PROPERTY_INTERFACE(slider_notch_count);
			property_interfaces[K_PROPERTY_MIN_ID] = GET_PROPERTY_INTERFACE(slider_min);
			property_interfaces[K_PROPERTY_MAX_ID] = GET_PROPERTY_INTERFACE(slider_max);
			property_interfaces[K_PROPERTY_VALUE_ID] = GET_PROPERTY_INTERFACE(slider_value);
		}

		void c_control_builder_gwen_slider::GetEventHandlers(Gwen::Controls::Base* control, Control::event_handler_map_t& event_handlers) const
		{
			c_control_builder_gwen_base::GetEventHandlers(control, event_handlers);
			
			auto& gwen_control = GWEN_CTRL_REF(Slider, control);

			event_handlers[K_EVENT_VALUECHANGED_ID] = std::make_unique<c_event_handler_gwen>(gwen_control.onValueChanged,
				[](Gwen::Event::Info& info, Control::s_interface_value& output)
				{
					auto& gwen_control = GWEN_CTRL_REF(Slider, info.ControlCaller);

					output.m_real = gwen_control.GetFloatValue();
				});
		}
	};};};};
};
#endif