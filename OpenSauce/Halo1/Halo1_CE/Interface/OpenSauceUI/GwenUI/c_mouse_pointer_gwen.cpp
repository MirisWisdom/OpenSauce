/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI/GwenUI/c_mouse_pointer_gwen.hpp"

#if !PLATFORM_IS_DEDI

#include <Gwen/Gwen.h>

#include "Interface/OpenSauceUI/Definitions/c_control_definition.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace GwenUI
	{
		void c_mouse_pointer_gwen::Initialize(ControlFactory::c_control_factory& control_factory, Control::i_canvas& canvas)
		{
			// Create the mouse pointer control
			Definitions::c_control_definition mouse_definition;
			mouse_definition.m_type = "Pointer";

			auto& fore_color = mouse_definition.m_properties.AddEntry();
			fore_color.m_name = "Color";
			fore_color.m_value = "255 255 0 255";

			m_mouse_control = control_factory.BuildControl(canvas, mouse_definition);

			InterfaceChanged();
		}

		void c_mouse_pointer_gwen::Release()
		{
			m_mouse_control.reset();
		}

		void c_mouse_pointer_gwen::SetPosition(const int x, const int y)
		{
			auto* gwen_control = CAST_PTR(Gwen::Controls::Base*, m_mouse_control->GetControlPtr());
			gwen_control->SetPos(x, y);
		}

		void c_mouse_pointer_gwen::GetPosition(int& x, int& y)
		{
			auto* gwen_control = CAST_PTR(Gwen::Controls::Base*, m_mouse_control->GetControlPtr());
			auto position = gwen_control->GetPos();
			
			x = position.x;
			y = position.y;
		}

		void c_mouse_pointer_gwen::InterfaceChanged()
		{
			auto* gwen_control = CAST_PTR(Gwen::Controls::Base*, m_mouse_control->GetControlPtr());
			gwen_control->BringToFront();
			gwen_control->SetMouseInputEnabled(false);
		}
	};};};
};
#endif