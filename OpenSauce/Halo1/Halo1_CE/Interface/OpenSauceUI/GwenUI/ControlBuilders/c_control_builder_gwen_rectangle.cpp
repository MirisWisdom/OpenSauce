/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI/GwenUI/ControlBuilders/c_control_builder_gwen_rectangle.hpp"

#if !PLATFORM_IS_DEDI

#include <Gwen/Controls/Rectangle.h>
#include <YeloLib/cseries/value_conversion.hpp>

#include "Interface/OpenSauceUI/Control/control_property_ids.hpp"
#include "Interface/OpenSauceUI/Control/c_control_base.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace GwenUI { namespace ControlBuilders
	{
		DEFINE_PROPERTY_INTERFACE(rectangle_color,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(Rectangle, control.GetControlPtr());

				auto color = gwen_control.GetColor();
				output.m_argb_color.alpha = color.a;
				output.m_argb_color.red = color.r;
				output.m_argb_color.green = color.g;
				output.m_argb_color.blue = color.b;
			},
			[](Control::i_control& control, const Control::s_interface_value& value)
			{
				auto& gwen_control = GWEN_CTRL_REF(Rectangle, control.GetControlPtr());

				gwen_control.SetColor(Gwen::Color(value.m_argb_color.red, value.m_argb_color.green, value.m_argb_color.blue, value.m_argb_color.alpha));
			},
			[](Control::i_control& control, cstring value_string)
			{
				auto& gwen_control = GWEN_CTRL_REF(Rectangle, control.GetControlPtr());

				argb_color value;
				ValueConversion::FromString(value_string, value);
				gwen_control.SetColor(Gwen::Color(value.red, value.green, value.blue, value.alpha));
			}
		);

		Gwen::Controls::Base* c_control_builder_gwen_rectangle::CreateInstance(Gwen::Controls::Base* parent) const
		{
			return new Gwen::Controls::Rectangle(parent);
		}

		void c_control_builder_gwen_rectangle::SetDefaultValues(Gwen::Controls::Base* control) const
		{
			c_control_builder_gwen_base::SetDefaultValues(control);

			auto& gwen_control = GWEN_CTRL_REF(Rectangle, control);

			gwen_control.SetColor(Gwen::Color(255, 255, 255, 255));
		}

		void c_control_builder_gwen_rectangle::GetPropertyInterfaces(Control::property_interface_map_t& property_interfaces) const
		{
			c_control_builder_gwen_base::GetPropertyInterfaces(property_interfaces);

			property_interfaces[K_PROPERTY_COLOR_ID] = GET_PROPERTY_INTERFACE(rectangle_color);
		}
	};};};};
};
#endif