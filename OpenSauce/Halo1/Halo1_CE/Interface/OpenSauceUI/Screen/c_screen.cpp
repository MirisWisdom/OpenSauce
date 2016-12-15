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
		c_screen::c_screen(Control::control_ptr_t root_control)
			: m_root_control(root_control)
			, m_control_resource_list()
		{
			AddControlResource(root_control);
		}

#pragma region i_screen
		Control::control_ptr_t c_screen::GetRootControl() const
		{
			return m_root_control;
		}

		Control::control_ptr_t c_screen::GetControl(const uint32 resource_id)
		{
			YELO_ASSERT_DISPLAY(m_control_resource_list.find(resource_id) != m_control_resource_list.end(), "Unknown resource ID");

			return m_control_resource_list[resource_id];
		}
#pragma endregion

		void c_screen::AddControlResource(Control::control_ptr_t control)
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

		void c_screen::Show()
		{
			GetRootControl()->Show();
		}

		void c_screen::Hide()
		{
			GetRootControl()->Hide();
		}
	};};};
};
#endif