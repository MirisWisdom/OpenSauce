/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI/GwenUI/c_control_builder_gwen.hpp"

#if !PLATFORM_IS_DEDI

#include <Gwen/Util/ControlFactory/ControlFactory.h>

#include "Interface/OpenSauceUI/GwenUI/c_control_gwen.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace GwenUI
	{
		t_control_ptr c_control_builder_gwen::Build(ControlFactory::c_control_factory& control_registry
			, Definitions::c_control_definition& control_definition
			, i_control& parent) const
		{				
			// Get the control factory for the specified control type
			auto* control_factory = Gwen::ControlFactory::Find(control_definition.m_type.Get());
			if(!control_factory)
			{
				return nullptr;
			}

			// Create an instance of the control and add set it's properties
			auto* new_control = control_factory->CreateInstance(CAST_PTR(Gwen::Controls::Base*, parent.GetControlPtr()));
			for(auto& control_property : control_definition.m_properties)
			{
				auto& value_string = control_property.m_value.Get();
				Gwen::UnicodeString value_wstring(value_string.begin(), value_string.end());

				control_factory->SetControlValue(new_control, control_property.m_name, value_wstring);
			}

			// Build the control's children
			t_control_ptr control = std::make_unique<c_control_gwen>(new_control);
			BuildChildren(control_registry, control_definition, *control);

			control->SetResourceID(control_definition.GetID());

			return control;
		}
	};};};
};
#endif