/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI/Screen/c_screen_display_manager.hpp"

#if !PLATFORM_IS_DEDI

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace Screen
	{
		void c_screen_display_manager::AddScreenController(const uint32 screen_id, t_screen_controller_ptr controller)
		{
			YELO_ASSERT_DISPLAY(m_screen_controllers.find(screen_id) == m_screen_controllers.end(), "Attempted to add duplicate screen controller to display manager");

			// Add the controller and set it's initial state to hidden
			m_screen_controllers[screen_id] = s_screen_controller_state { false, controller };

			controller->Hide();
		}

		void c_screen_display_manager::RemoveScreenController(const uint32 screen_id)
		{
			YELO_ASSERT_DISPLAY(m_screen_controllers.find(screen_id) != m_screen_controllers.end(), "Attempted to remove non-existant screen controller from display manager");

			// Remove the controller
			m_screen_controllers.erase(screen_id);
		}

		void c_screen_display_manager::ShowScreen(const uint32 screen_id)
		{
			// If the screen is not already visible, show it
			auto& state = m_screen_controllers[screen_id];

			if(!state.visible)
			{
				state.controller->Show();
				state.visible = true;
			}
		}

		void c_screen_display_manager::HideScreen(const uint32 screen_id)
		{
			// If the screen is not already hidden, hide it
			auto& state = m_screen_controllers[screen_id];

			if(state.visible)
			{
				state.controller->Hide();
				state.visible = false;
			}
		}
	};};};
};
#endif