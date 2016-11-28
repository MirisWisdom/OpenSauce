/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI/GwenUI/ControlBuilders/c_control_builder_gwen.hpp"

#if !PLATFORM_IS_DEDI

#include "Interface/OpenSauceUI/resource_id.hpp"
#include "Interface/OpenSauceUI/GwenUI/c_control_gwen.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace GwenUI { namespace ControlBuilders
	{
		Control::control_ptr_t c_control_builder_gwen::Build(ControlFactory::c_control_factory& control_registry
			, Definitions::c_control_definition& control_definition
			, Control::i_control& parent) const
		{
			// Create an instance of the gwen control
			auto* gwen_control = CreateInstance(GWEN_CTRL_PTR(Base, parent.GetControlPtr()));
			SetDefaultValues(gwen_control);

			// Build the control type's property interfaces and event handlers
			Control::property_interface_map_t property_interfaces;
			GetPropertyInterfaces(property_interfaces);

			Control::event_handler_map_t event_handlers;
			GetEventHandlers(gwen_control, event_handlers);

			// Build the control
			Control::control_ptr_t control = std::make_unique<c_control_gwen>(parent
				, gwen_control
				, property_interfaces
				, event_handlers);
			control->SetResourceID(control_definition.GetID());

			// Set the control's properties from the control definition
			for(auto& control_property : control_definition.m_properties)
			{
				auto* property_interface = control->GetPropertyInterface(CRCString(control_property.m_name.Get().c_str()));

				YELO_ASSERT_DISPLAY(property_interface, "Failed to find a property called %s", control_property.m_name.Get().c_str());

				if(property_interface)
				{
					property_interface->Set(*control, control_property.m_value.Get().c_str());
				}
			}

			// Build the control's children
			BuildChildren(control_registry, control_definition.m_controls.Get(), *control);

			return control;
		}
	};};};};
};
#endif