/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI/GwenUI/ControlBuilders/c_control_builder_gwen_textboxnumeric.hpp"

#if !PLATFORM_IS_DEDI

#include <Gwen/Controls/TextBox.h>
#include <YeloLib/cseries/value_conversion.hpp>

#include "Interface/OpenSauceUI/Control/control_property_ids.hpp"
#include "Interface/OpenSauceUI/Control/c_control_base.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace GwenUI { namespace ControlBuilders
	{
		DEFINE_PROPERTY_INTERFACE(textboxnumeric_value,
			[](Control::i_control& control, Control::s_interface_value& output)
			{
				auto& gwen_control = GWEN_CTRL_REF(TextBoxNumeric, control.GetControlPtr());

				output.m_real = gwen_control.GetFloatFromText();
			},
			nullptr,
			nullptr
		);

		Gwen::Controls::Base* c_control_builder_gwen_textboxnumeric::CreateInstance(Gwen::Controls::Base* parent) const
		{
			return new Gwen::Controls::TextBox(parent);
		}

		void c_control_builder_gwen_textboxnumeric::SetDefaultValues(Gwen::Controls::Base* control) const
		{
			c_control_builder_gwen_textbox::SetDefaultValues(control);

			auto& gwen_control = GWEN_CTRL_REF(TextBox, control);
			gwen_control.SetEditable(true);
			gwen_control.SetText("");
		}

		void c_control_builder_gwen_textboxnumeric::GetPropertyInterfaces(Control::property_interface_map_t& property_interfaces) const
		{
			c_control_builder_gwen_textbox::GetPropertyInterfaces(property_interfaces);
			
			property_interfaces[K_PROPERTY_VALUE_ID] = GET_PROPERTY_INTERFACE(textboxnumeric_value);
		}
	};};};};
};
#endif