/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI/GwenUI/ControlBuilders/c_control_builder_gwen_progressbar.hpp"

#if !PLATFORM_IS_DEDI

#include <Gwen/Controls/ProgressBar.h>
#include <YeloLib/cseries/value_conversion.hpp>

#include "Interface/OpenSauceUI/Control/control_property_ids.hpp"
#include "Interface/OpenSauceUI/Control/c_control_base.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace GwenUI { namespace ControlBuilders
	{
		DEFINE_PROPERTY_INTERFACE(progressbar_vertical,
			nullptr,
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(ProgressBar, control.GetControlPtr());

				if(value.m_bool)
				{
					gwen_control.SetVertical();
				}
				else
				{
					gwen_control.SetHorizontal();
				}
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(ProgressBar, control.GetControlPtr());

				bool value;
				ValueConversion::FromString(value_string, value);

				if(value)
				{
					gwen_control.SetVertical();
				}
				else
				{
					gwen_control.SetHorizontal();
				}
			}
		);

		DEFINE_PROPERTY_INTERFACE(progressbar_horizontal,
			nullptr,
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(ProgressBar, control.GetControlPtr());

				if(value.m_bool)
				{
					gwen_control.SetHorizontal();
				}
				else
				{
					gwen_control.SetVertical();
				}
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(ProgressBar, control.GetControlPtr());

				bool value;
				ValueConversion::FromString(value_string, value);

				if(value)
				{
					gwen_control.SetHorizontal();
				}
				else
				{
					gwen_control.SetVertical();
				}
			}
		);

		DEFINE_PROPERTY_INTERFACE(progressbar_value,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(ProgressBar, control.GetControlPtr());

				output.m_real = gwen_control.GetValue();
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(ProgressBar, control.GetControlPtr());

				gwen_control.SetValue(value.m_real);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(ProgressBar, control.GetControlPtr());

				real value;
				ValueConversion::FromString(value_string, value);
				gwen_control.SetValue(value);
			}
		);

		DEFINE_PROPERTY_INTERFACE(progressbar_cycle_speed,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(ProgressBar, control.GetControlPtr());

				output.m_real = gwen_control.GetCycleSpeed();
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(ProgressBar, control.GetControlPtr());

				gwen_control.SetCycleSpeed(value.m_real);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(ProgressBar, control.GetControlPtr());

				real value;
				ValueConversion::FromString(value_string, value);
				gwen_control.SetCycleSpeed(value);
			}
		);

		DEFINE_PROPERTY_INTERFACE(progressbar_auto_label,
			nullptr,
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(ProgressBar, control.GetControlPtr());

				gwen_control.SetAutoLabel(value.m_bool);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(ProgressBar, control.GetControlPtr());

				bool value;
				ValueConversion::FromString(value_string, value);
				gwen_control.SetAutoLabel(value);
			}
		);

		Gwen::Controls::Base* c_control_builder_gwen_progressbar::CreateInstance(Gwen::Controls::Base* parent) const
		{
			return new Gwen::Controls::ProgressBar(parent);
		}

		void c_control_builder_gwen_progressbar::SetDefaultValues(Gwen::Controls::Base* control) const
		{
			c_control_builder_gwen_label::SetDefaultValues(control);

			auto& gwen_control = GWEN_CTRL_REF(ProgressBar, control);

			gwen_control.SetHorizontal();
			gwen_control.SetCycleSpeed(0);
			gwen_control.SetAutoLabel(false);
		}

		void c_control_builder_gwen_progressbar::GetPropertyInterfaces(Control::property_interface_map_t& property_interfaces) const
		{
			c_control_builder_gwen_label::GetPropertyInterfaces(property_interfaces);
			
			property_interfaces[K_PROPERTY_VERTICAL_ID] = GET_PROPERTY_INTERFACE(progressbar_vertical);
			property_interfaces[K_PROPERTY_HORIZONTAL_ID] = GET_PROPERTY_INTERFACE(progressbar_horizontal);
			property_interfaces[K_PROPERTY_VALUE_ID] = GET_PROPERTY_INTERFACE(progressbar_value);
			property_interfaces[K_PROPERTY_CYCLESPEED_ID] = GET_PROPERTY_INTERFACE(progressbar_cycle_speed);
			property_interfaces[K_PROPERTY_AUTOLABEL_ID] = GET_PROPERTY_INTERFACE(progressbar_auto_label);
		}
	};};};};
};
#endif