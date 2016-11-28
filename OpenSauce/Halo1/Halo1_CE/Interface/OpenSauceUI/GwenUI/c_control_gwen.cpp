/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI/GwenUI/c_control_gwen.hpp"

#if !PLATFORM_IS_DEDI

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace GwenUI
	{
		c_control_gwen::c_control_gwen(Control::i_control& parent
			, Gwen::Controls::Base* gwen_control
			, const Control::property_interface_map_t& property_interfaces
			, const Control::event_handler_map_t& event_handlers)
			: c_control_base(parent)
			, m_control(gwen_control)
		{
			// Add all property interfaces
			for(auto& property_interface : property_interfaces)
			{
				AddPropertyInterface(property_interface.first, property_interface.second);
			}

			// Add all event handlers
			for(auto& event_handler : event_handlers)
			{
				AddEventHandler(event_handler.first, event_handler.second);
			}
		}

#pragma region i_control
		rectangle2d c_control_gwen::GetBounds() const
		{
			auto& bounds = m_control->GetBounds();

			return rectangle2d { bounds.x, bounds.y, bounds.h, bounds.w };
		}

		void* c_control_gwen::GetControlPtr() const
		{
			return m_control;
		}

		void c_control_gwen::Show()
		{
			m_control->Show();
		}

		void c_control_gwen::Hide()
		{
			m_control->Hide();
		}
#pragma endregion
	};};};
};
#endif