/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI.hpp"

#if !PLATFORM_IS_DEDI
#include <YeloLib/configuration/c_configuration_file_factory.hpp>

#include "Rasterizer/DX9/DX9.hpp"
#include "Interface/Controls.hpp"

#include "Interface/OpenSauceUI/resource_id.hpp"
#include "Interface/OpenSauceUI/Control/i_canvas.hpp"
#include "Interface/OpenSauceUI/Definitions/c_screen_definition.hpp"
#include "Interface/OpenSauceUI/ControlFactory/c_control_factory.hpp"
#include "Interface/OpenSauceUI/Input/c_control_input_halo.hpp"

#include "Interface/OpenSauceUI/Screen/c_screen_display_manager.hpp"
#include "Interface/OpenSauceUI/Screen/c_screen_controller_mainmenu.hpp"

#include "Interface/OpenSauceUI/GwenUI/c_canvas_gwen.hpp"
#include "Interface/OpenSauceUI/GwenUI/c_mouse_pointer_gwen.hpp"

#include "Interface/OpenSauceUI/GwenUI/ControlBuilders/c_control_builder_gwen_label.hpp"
#include "Interface/OpenSauceUI/GwenUI/ControlBuilders/c_control_builder_gwen_checkbox.hpp"
#include "Interface/OpenSauceUI/GwenUI/ControlBuilders/c_control_builder_gwen_checkboxwithlabel.hpp"
#include "Interface/OpenSauceUI/GwenUI/ControlBuilders/c_control_builder_gwen_rectangle.hpp"
#include "Interface/OpenSauceUI/GwenUI/ControlBuilders/c_control_builder_gwen_horizontalslider.hpp"
#include "Interface/OpenSauceUI/GwenUI/ControlBuilders/c_control_builder_gwen_verticalslider.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI
	{
		static Control::t_canvas_ptr g_canvas;
		static ControlFactory::c_control_factory g_control_factory;
		static Input::c_control_input_halo g_control_input;
		static GwenUI::c_mouse_pointer_gwen g_mouse_pointer;

		static std::unique_ptr<Screen::c_screen_display_manager> g_screen_display_manager;

		void LoadUI(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentation_parameters)
		{
			g_canvas->Initialize(device);
			g_canvas->SetSize(presentation_parameters->BackBufferWidth, presentation_parameters->BackBufferHeight);
			g_control_input.Initialize(CAST_PTR(Gwen::Controls::Canvas*, g_canvas->GetControlPtr()));
			g_control_input.SetMouseBounds(0, presentation_parameters->BackBufferWidth, 0, presentation_parameters->BackBufferHeight);
			g_mouse_pointer.Initialize(g_control_factory, *g_canvas);

			g_screen_display_manager->AddScreenController(Flags::_osui_game_state_all
				, (Flags::osui_screen_flags)(Flags::_osui_screen_flags_key_toggled | Flags::_osui_screen_flags_show_cursor | Flags::_osui_screen_flags_is_modal)
				, Enums::Key::_KeyF7
				, false
				, true
				, std::make_shared<Screen::c_screen_controller_mainmenu>());

			g_screen_display_manager->SetGameState(Flags::_osui_game_state_main_menu);
		}

		void UnloadUI()
		{
			g_screen_display_manager->ClearScreenControllers();
		}

		void Initialize()
		{
			g_canvas = std::make_unique<GwenUI::c_canvas_gwen>();
			
			g_control_factory.AddControl("Label", std::make_unique<GwenUI::ControlBuilders::c_control_builder_gwen_label>());
			g_control_factory.AddControl("CheckBox", std::make_unique<GwenUI::ControlBuilders::c_control_builder_gwen_checkbox>());
			g_control_factory.AddControl("CheckBoxWithLabel", std::make_unique<GwenUI::ControlBuilders::c_control_builder_gwen_checkboxwithlabel>());
			g_control_factory.AddControl("Rectangle", std::make_unique<GwenUI::ControlBuilders::c_control_builder_gwen_rectangle>());
			g_control_factory.AddControl("HorizontalSlider", std::make_unique<GwenUI::ControlBuilders::c_control_builder_gwen_horizontalslider>());
			g_control_factory.AddControl("VerticalSlider", std::make_unique<GwenUI::ControlBuilders::c_control_builder_gwen_verticalslider>());

			g_control_factory.AddControl("Pointer", std::make_unique<GwenUI::ControlBuilders::c_control_builder_gwen_rectangle>());

			g_screen_display_manager = std::make_unique<Screen::c_screen_display_manager>(*g_canvas, g_mouse_pointer, g_control_factory);
		}

		void Dispose()
		{
			g_canvas.reset();
		}

		void Update()
		{
			g_control_input.Update();

			int x;
			int y;
			g_control_input.GetMousePosition(x, y);
			g_mouse_pointer.SetPosition(x, y);

			g_screen_display_manager->Update();
		}

		void Initialize3D(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentation_parameters)
		{
			LoadUI(device, presentation_parameters);
		}

		void OnLostDevice()
		{
			UnloadUI();
			g_mouse_pointer.Release();
			g_canvas->Release();
		}

		void OnResetDevice(D3DPRESENT_PARAMETERS* presentation_parameters)
		{
			LoadUI(DX9::Direct3DDevice(), presentation_parameters);
		}

		void Render()
		{
			g_canvas->Draw();
		}

		void Release()
		{
			UnloadUI();
			g_mouse_pointer.Release();
			g_canvas->Release();
		}
	};};
};
#endif