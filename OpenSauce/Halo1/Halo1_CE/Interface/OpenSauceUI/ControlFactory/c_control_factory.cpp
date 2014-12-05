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
		void c_control_factory::AddControl(const std::string& type_name, const i_control_builder* builder)
		{
			// Test whether an existing control already exists
			auto existing_control = std::find_if(m_controls.begin(), m_controls.end(),
				[&](s_control_type& entry)
				{
					return entry.m_type_name == type_name;
				}
			);

			// If a control type already exists, return
			YELO_ASSERT(existing_control == m_controls.end())
			if(existing_control != m_controls.end())
			{
				return;
			}

			// Add the control type to the controls list
			s_control_type control_type = { type_name, builder };
			m_controls.push_back(control_type);
		}

		t_control_ptr c_control_factory::BuildControl(i_control& parent, Definitions::c_control_definition& definition)
		{
			// Find the requested control
			auto control_definition = std::find_if(m_controls.begin(), m_controls.end(),
				[&](s_control_type& entry)
				{
					return entry.m_type_name == definition.m_type.Get();
				}
			);
			YELO_ASSERT(control_definition != m_controls.end());

			// Build the control and add it to the parent
			auto& control = control_definition->m_control_builder->Build(*this, definition, parent);
			parent.AddControl(control);

			return control;
		}
	};};};
};
#endif