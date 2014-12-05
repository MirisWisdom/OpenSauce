/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI/c_control_base.hpp"

#if !PLATFORM_IS_DEDI

#include "Interface/OpenSauceUI/resource_id.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI
	{
#pragma region c_control_base
		c_control_base::c_control_base()
			: m_resource_id(RESOURCE_ID_NONE)
			, m_child_controls()
		{ }

		t_control_list::const_iterator c_control_base::FindControl(const t_control_ptr control) const
		{
			// Find the control using a pointer comparison
			return std::find_if(m_child_controls.begin(), m_child_controls.end(),
				[&](const t_control_ptr& entry)
				{
					return entry == control;
				}
			);
		}
			
#pragma region i_control
		uint32 c_control_base::GetResourceID() const
		{
			return m_resource_id;
		}

		void c_control_base::SetResourceID(const uint32 resource_id)
		{
			m_resource_id = resource_id;
		}

		void c_control_base::AddControl(t_control_ptr control)
		{
			// Add the control to the list if it does not already exist
			auto found_entry = FindControl(control);
			if(found_entry == m_child_controls.end())
			{
				m_child_controls.push_back(control);
			}
		}

		void c_control_base::RemoveControl(t_control_ptr control)
		{
			// Remove the control from the list if it exists
			auto found_entry = FindControl(control);
			if(found_entry != m_child_controls.end())
			{
				m_child_controls.erase(found_entry);
			}
		}

		t_control_list& c_control_base::Controls()
		{
			return m_child_controls;
		}
#pragma endregion
#pragma endregion
	};};
};
#endif