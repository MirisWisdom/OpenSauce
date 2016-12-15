/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI/GwenUI/ControlBuilders/c_control_builder_gwen_scrollcontrol.hpp"

#if !PLATFORM_IS_DEDI

#include <Gwen/Controls/ScrollControl.h>
#include <YeloLib/cseries/value_conversion.hpp>

#include "Interface/OpenSauceUI/Control/control_property_ids.hpp"
#include "Interface/OpenSauceUI/Control/c_control_base.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace GwenUI { namespace ControlBuilders
	{
		DEFINE_PROPERTY_INTERFACE(scrollcontrol_innersize,
			nullptr,
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(ScrollControl, control.GetControlPtr());

				gwen_control.SetInnerSize(value.m_point2d.x, value.m_point2d.y);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(ScrollControl, control.GetControlPtr());

				point2d value;
				ValueConversion::FromString(value_string, value);
				gwen_control.SetInnerSize(value.x, value.y);
			}
		);

		DEFINE_PROPERTY_INTERFACE(scrollcontrol_scrollh,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(ScrollControl, control.GetControlPtr());

				output.m_bool = gwen_control.CanScrollH();
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(ScrollControl, control.GetControlPtr());

				gwen_control.SetScroll(value.m_bool, gwen_control.CanScrollV());
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(ScrollControl, control.GetControlPtr());

				bool value;
				ValueConversion::FromString(value_string, value);
				gwen_control.SetScroll(value, gwen_control.CanScrollV());
			}
		);

		DEFINE_PROPERTY_INTERFACE(scrollcontrol_scrollv,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(ScrollControl, control.GetControlPtr());

				output.m_bool = gwen_control.CanScrollV();
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(ScrollControl, control.GetControlPtr());

				gwen_control.SetScroll(gwen_control.CanScrollH(), value.m_bool);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(ScrollControl, control.GetControlPtr());

				bool value;
				ValueConversion::FromString(value_string, value);
				gwen_control.SetScroll(gwen_control.CanScrollH(), value);
			}
		);

		DEFINE_PROPERTY_INTERFACE(scrollcontrol_autohidebars,
			nullptr,
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(ScrollControl, control.GetControlPtr());

				gwen_control.SetAutoHideBars(value.m_bool);
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(ScrollControl, control.GetControlPtr());

				bool value;
				ValueConversion::FromString(value_string, value);
				gwen_control.SetAutoHideBars(value);
			}
		);

		Gwen::Controls::Base* c_control_builder_gwen_scrollcontrol::CreateInstance(Gwen::Controls::Base* parent) const
		{
			return new Gwen::Controls::ScrollControl(parent);
		}

		void c_control_builder_gwen_scrollcontrol::SetDefaultValues(Gwen::Controls::Base* control) const
		{
			c_control_builder_gwen_base::SetDefaultValues(control);

			auto& gwen_control = GWEN_CTRL_REF(ScrollControl, control);

			gwen_control.SetInnerSize(100, 100);
			gwen_control.SetAutoHideBars(true);
			gwen_control.SetScroll(false, false);
		}

		void c_control_builder_gwen_scrollcontrol::GetPropertyInterfaces(Control::property_interface_map_t& property_interfaces) const
		{
			c_control_builder_gwen_base::GetPropertyInterfaces(property_interfaces);
			
			property_interfaces[K_PROPERTY_INNERSIZE_ID] = GET_PROPERTY_INTERFACE(scrollcontrol_innersize);
			property_interfaces[K_PROPERTY_SCROLLH_ID] = GET_PROPERTY_INTERFACE(scrollcontrol_scrollh);
			property_interfaces[K_PROPERTY_SCROLLV_ID] = GET_PROPERTY_INTERFACE(scrollcontrol_scrollv);
			property_interfaces[K_PROPERTY_AUTOHIDEBARS_ID] = GET_PROPERTY_INTERFACE(scrollcontrol_autohidebars);
		}
	};};};};
};
#endif