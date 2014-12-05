/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI/Screen/c_screen.hpp"

#if !PLATFORM_IS_DEDI

#include "Interface/OpenSauceUI/resource_id.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace Screen
	{
		c_screen::c_screen(t_control_ptr root_control)
			: m_root_control(root_control)
			, m_control_resource_list()
		{
			AddControlResource(root_control);
		}

#pragma region i_screen
		t_control_ptr c_screen::GetRootControl() const
		{
			return m_root_control;
		}

		t_control_ptr c_screen::GetControl(const uint32 resource_id)
		{
			return m_control_resource_list[resource_id];
		}
#pragma endregion

		void c_screen::AddControlResource(t_control_ptr control)
		{
			// If the control has a resource id add it to the resource list
			if(control->GetResourceID() != RESOURCE_ID_NONE)
			{
				m_control_resource_list[control->GetResourceID()] = control;
			}

			// Add the controls children to the resource list if appropriate
			for(auto& child : control->Controls())
			{
				AddControlResource(child);
			}
		}
	};};};
};
#endif