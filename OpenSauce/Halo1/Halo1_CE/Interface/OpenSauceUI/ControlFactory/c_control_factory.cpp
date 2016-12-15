/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI/ControlFactory/c_control_factory.hpp"

#if !PLATFORM_IS_DEDI

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace ControlFactory
	{
		std::vector<c_control_factory::s_control_type>::iterator c_control_factory::FindControlType(const std::string& type_name)
		{
			// Find the control by name
			return std::find_if(m_controls.begin(), m_controls.end(),
				[&](s_control_type& entry)
				{
					return entry.m_type_name == type_name;
				}
			);
		}

		void c_control_factory::AddControl(const std::string& type_name, control_builder_ptr_t builder)
		{
			// Test whether an existing control is present
			auto existing_control = FindControlType(type_name);

			// If a control type already exists, return
			YELO_ASSERT_DISPLAY(existing_control == m_controls.end(), "Attempted to add duplicate controls to a control factory: %s", type_name.c_str());

			if(existing_control != m_controls.end())
			{
				return;
			}

			// Add the control type to the controls list
			m_controls.push_back(s_control_type { type_name, builder });
		}

		Control::control_ptr_t c_control_factory::BuildControl(Control::i_control& parent, Definitions::c_control_definition& definition)
		{
			// Find the requested control
			auto control_definition = FindControlType(definition.m_type.Get());

			YELO_ASSERT_DISPLAY(control_definition != m_controls.end(), "Attempted to build a control of an unknown type: %s", definition.m_type.Get().c_str());

			// Build the control and add it to the parent
			auto& control = control_definition->m_control_builder->Build(*this, definition, parent);

			parent.AddControl(control);

			return control;
		}
	};};};
};
#endif