/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI/GwenUI/c_mouse_pointer_gwen.hpp"

#if !PLATFORM_IS_DEDI

#include <Gwen/Gwen.h>

#include "Interface/OpenSauceUI/Control/control_property_ids.hpp"
#include "Interface/OpenSauceUI/Definitions/c_control_definition.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace GwenUI
	{
		c_mouse_pointer_gwen::c_mouse_pointer_gwen(ControlFactory::c_control_factory& control_factory, Control::i_canvas& canvas)
			: m_control_factory(control_factory)
			, m_canvas(canvas)
			, m_mouse_control(nullptr)
			, m_position_property(nullptr)
		{ }

		void c_mouse_pointer_gwen::BuildMouse(Input::i_control_input& control_input)
		{		
			// Create the mouse pointer control
			Definitions::c_control_definition mouse_definition;
			mouse_definition.m_type = "Pointer";

			m_mouse_control = m_control_factory.BuildControl(m_canvas, mouse_definition);
			m_position_property = m_mouse_control->GetPropertyInterface(K_PROPERTY_POSITION_ID);

			Hide();

			control_input.AttachMouseInputHandler(this);
		}

		void c_mouse_pointer_gwen::DestroyMouse(Input::i_control_input& control_input)
		{
			control_input.DetachMouseInputHandler(this);
			m_position_property = nullptr;
			m_mouse_control.reset();
		}

		void c_mouse_pointer_gwen::Show()
		{
			if(!m_mouse_control)
			{
				return;
			}

			auto& gwen_control = *CAST_PTR(Gwen::Controls::Base*, m_mouse_control->GetControlPtr());

			gwen_control.SetDisabled(false);
			gwen_control.Show();
			gwen_control.BringToFront();
		}

		void c_mouse_pointer_gwen::Hide()
		{
			if(!m_mouse_control)
			{
				return;
			}

			auto& gwen_control = *CAST_PTR(Gwen::Controls::Base*, m_mouse_control->GetControlPtr());

			gwen_control.Hide();
			gwen_control.SetDisabled(true);
		}
		
#pragma region i_control_mouse_handler
		void c_mouse_pointer_gwen::OnMousePositionUpdate(const point2d& absolute, const point2d& relative)
		{
			m_position_property->Set(*m_mouse_control, Control::s_interface_value(absolute));
		}
#pragma endregion
	};};};
};
#endif