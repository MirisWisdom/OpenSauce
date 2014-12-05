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

#include "Interface/OpenSauceUI/i_canvas.hpp"
#include "Interface/OpenSauceUI/Definitions/c_screen_definition.hpp"
#include "Interface/OpenSauceUI/ControlFactory/c_control_factory.hpp"
#include "Interface/OpenSauceUI/GwenUI/c_canvas_gwen.hpp"
#include "Interface/OpenSauceUI/GwenUI/c_control_builder_gwen.hpp"
#include "Interface/OpenSauceUI/GwenUI/c_mouse_pointer_gwen.hpp"
#include "Interface/OpenSauceUI/Input/c_control_input_halo.hpp"

#include "Interface/OpenSauceUI/Screen/c_screen_builder.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI
	{
		static t_canvas_ptr g_canvas;
		static ControlFactory::c_control_factory g_control_factory;
		static GwenUI::c_control_builder_gwen g_control_builder_gwen;
		static Input::c_control_input_halo g_control_input;
		static GwenUI::c_mouse_pointer_gwen g_mouse_pointer;
		static t_screen_ptr g_screen_mainmenu;

		void Initialize()
		{
			g_canvas = std::make_unique<GwenUI::c_canvas_gwen>();
			
			g_control_factory.AddControl("Label", &g_control_builder_gwen);
			g_control_factory.AddControl("LabelClickable", &g_control_builder_gwen);
			g_control_factory.AddControl("Button", &g_control_builder_gwen);
			g_control_factory.AddControl("TextBox", &g_control_builder_gwen);
			g_control_factory.AddControl("TextBoxMultiline", &g_control_builder_gwen);
			g_control_factory.AddControl("CheckBox", &g_control_builder_gwen);
			g_control_factory.AddControl("CheckBoxWithLabel", &g_control_builder_gwen);
			g_control_factory.AddControl("MenuStrip", &g_control_builder_gwen);
			g_control_factory.AddControl("ListBox", &g_control_builder_gwen);
			g_control_factory.AddControl("ImagePanel", &g_control_builder_gwen);
			g_control_factory.AddControl("PageControl", &g_control_builder_gwen);
			g_control_factory.AddControl("Rectangle", &g_control_builder_gwen);
			g_control_factory.AddControl("ProgressBar", &g_control_builder_gwen);
			g_control_factory.AddControl("ComboBox", &g_control_builder_gwen);
			g_control_factory.AddControl("HorizontalSlider", &g_control_builder_gwen);
			g_control_factory.AddControl("Pointer", &g_control_builder_gwen);
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


			static byte previous_state;
			byte state = Yelo::Input::GetKeyState(Enums::_Key0);
			if((state != previous_state) && (state == 1))
			{
				static t_control_ptr screen_ptr = nullptr;

				if(screen_ptr)
				{
					g_canvas->RemoveControl(screen_ptr);
					screen_ptr.reset();
				}
			}
			previous_state = state;
		}

		void LoadUI(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentation_parameters)
		{
			g_canvas->Initialize(device);
			g_canvas->SetSize(presentation_parameters->BackBufferWidth, presentation_parameters->BackBufferHeight);
			g_control_input.Initialize(CAST_PTR(Gwen::Controls::Canvas*, g_canvas->GetControlPtr()));
			g_control_input.SetMouseBounds(0, presentation_parameters->BackBufferWidth, 0, presentation_parameters->BackBufferHeight);


			auto& config_file = Configuration::c_configuration_file_factory::CreateConfigurationFile("D://test.json");
			config_file->Load();

			auto& root = config_file->Root()->GetChild("OpenSauceUI");

			auto& control_iterator = root->GetChildIterator("Screen");
			if(control_iterator->MoveNext())
			{
				Definitions::c_screen_definition screen;
				screen.GetValue(*control_iterator->Current());

				g_screen_mainmenu = Screen::c_screen_builder::BuildScreen(g_control_factory, screen, *g_canvas);
			}
			g_mouse_pointer.Initialize(g_control_factory, *g_canvas);
		}

		void Initialize3D(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentation_parameters)
		{
			LoadUI(device, presentation_parameters);
		}

		void OnLostDevice()
		{
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
			g_mouse_pointer.Release();
			g_canvas->Release();
		}
	};};
};
#endif