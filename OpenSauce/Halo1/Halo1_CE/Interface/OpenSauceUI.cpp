/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI.hpp"

#if !PLATFORM_IS_DEDI

#include <YeloLib/configuration/c_configuration_file_factory.hpp>
#include <BlamLib/Halo1/game/game_globals.hpp>
#include <BlamLib/Halo1/game/game_globals_structures.hpp>
#include <BlamLib/Halo1/cache/cache_files.hpp>
#include <BlamLib/Halo1/cache/cache_files_globals.hpp>

#include "Game/GameState.hpp"
#include "Common/FileIO.hpp"
#include "Rasterizer/DX9/DX9.hpp"
#include "Interface/Controls.hpp"

#include "Interface/OpenSauceUI/resource_id.hpp"
#include "Interface/OpenSauceUI/Control/i_canvas.hpp"
#include "Interface/OpenSauceUI/Definitions/c_screen_definition.hpp"
#include "Interface/OpenSauceUI/ControlFactory/c_control_factory.hpp"
#include "Interface/OpenSauceUI/Input/c_control_input_halo.hpp"

#include "Interface/OpenSauceUI/Screen/c_screen_definition_registry.hpp"
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
#include "Interface/OpenSauceUI/GwenUI/ControlBuilders/c_control_builder_gwen_textbox.hpp"
#include "Interface/OpenSauceUI/GwenUI/ControlBuilders/c_control_builder_gwen_textboxnumeric.hpp"
#include "Interface/OpenSauceUI/GwenUI/ControlBuilders/c_control_builder_gwen_scrollcontrol.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI
	{
		static bool g_ui_package_present;
		static c_packed_file g_ui_package;
		static Control::t_canvas_ptr g_canvas;
		static ControlFactory::c_control_factory g_control_factory;
		static Input::c_control_input_halo g_control_input;
		static GwenUI::c_mouse_pointer_gwen g_mouse_pointer;
		static std::unique_ptr<Screen::c_screen_display_manager> g_screen_display_manager;

		void Initialize()
		{
			g_ui_package_present = true;

			// Determine whether the opensauce ui package is present
			FileIO::s_file_info file_info;
			Enums::file_io_open_error file_open_success = FileIO::OpenFileByID(file_info, "UI_OpenSauceUI_PAK");
			FileIO::CloseFile(file_info);

			g_ui_package_present &= (file_open_success == Enums::_file_io_open_error_none);

			if(!g_ui_package_present)
			{
				return;
			}

			// Load the UI package and initialise the canvas, display manager and control factory
			g_ui_package.OpenFile("UI_OpenSauceUI_PAK", true);

			g_canvas = std::make_unique<GwenUI::c_canvas_gwen>();
			g_screen_display_manager = std::make_unique<Screen::c_screen_display_manager>(*g_canvas, g_mouse_pointer, g_control_factory);
			
			g_control_factory.AddControl("Label", std::make_unique<GwenUI::ControlBuilders::c_control_builder_gwen_label>());
			g_control_factory.AddControl("CheckBox", std::make_unique<GwenUI::ControlBuilders::c_control_builder_gwen_checkbox>());
			g_control_factory.AddControl("CheckBoxWithLabel", std::make_unique<GwenUI::ControlBuilders::c_control_builder_gwen_checkboxwithlabel>());
			g_control_factory.AddControl("Rectangle", std::make_unique<GwenUI::ControlBuilders::c_control_builder_gwen_rectangle>());
			g_control_factory.AddControl("HorizontalSlider", std::make_unique<GwenUI::ControlBuilders::c_control_builder_gwen_horizontalslider>());
			g_control_factory.AddControl("VerticalSlider", std::make_unique<GwenUI::ControlBuilders::c_control_builder_gwen_verticalslider>());
			g_control_factory.AddControl("TextBox", std::make_unique<GwenUI::ControlBuilders::c_control_builder_gwen_textbox>());
			g_control_factory.AddControl("TextBoxNumeric", std::make_unique<GwenUI::ControlBuilders::c_control_builder_gwen_textboxnumeric>());
			g_control_factory.AddControl("ScrollControl", std::make_unique<GwenUI::ControlBuilders::c_control_builder_gwen_scrollcontrol>());

			g_control_factory.AddControl("Pointer", std::make_unique<GwenUI::ControlBuilders::c_control_builder_gwen_rectangle>());
		}

		void Dispose()
		{
			g_canvas.reset();
		}

		void Update()
		{
			if(!g_ui_package_present)
			{
				return;
			}

			g_control_input.Update();

			// Update the mouse pointer
			DOC_TODO("Implement a proper mouse pointer with events, tied to Halo's mouse position");
			int x;
			int y;
			g_control_input.GetMousePosition(x, y);
			g_mouse_pointer.SetPosition(x, y);

			// Update the UI according to the game's current state
			bool in_menu =				Yelo::Input::IsInMenu();
			bool in_chat =				Yelo::Input::IsInChat();
			bool is_loading =			GameState::GameGlobals()->map_loading_in_progress;
			bool is_main_menu_cache =	Yelo::Cache::CacheFileGlobals()->cache_header.cache_type == 2;

			_enum game_state = Flags::_osui_game_state_in_game;
			if(is_loading)
			{
				game_state = Flags::_osui_game_state_loading;
			}
			// Pause menu if the cache isn't a main menu but we are in a menu
			else if(!is_main_menu_cache && in_menu)
			{
				game_state = Flags::_osui_game_state_pause_menu;
			}
			// In game if the cache isn't a main menu and we aren't in a menu
			else if(!is_main_menu_cache && !in_menu)
			{
				game_state = Flags::_osui_game_state_in_game;
			}
			// In the main menu if the cache is a main menu and we are in a menu
			else if(is_main_menu_cache && in_menu)
			{
				game_state = Flags::_osui_game_state_main_menu;
			}
			g_screen_display_manager->SetGameState((Flags::osui_game_state)game_state);

			g_screen_display_manager->Update();
		}

		void LoadUI(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentation_parameters)
		{
			g_canvas->Initialize(device, g_ui_package);
			g_canvas->SetSize(presentation_parameters->BackBufferWidth, presentation_parameters->BackBufferHeight);
			g_control_input.Initialize(CAST_PTR(Gwen::Controls::Canvas*, g_canvas->GetControlPtr()));
			g_control_input.SetMouseBounds(0, presentation_parameters->BackBufferWidth, 0, presentation_parameters->BackBufferHeight);
			g_mouse_pointer.Initialize(g_control_factory, *g_canvas);

			Screen::c_screen_definition_registry definition_registry(g_ui_package);
			Definitions::c_screen_definition screen_definition;

			definition_registry.GetScreenDefinition("MainMenu", screen_definition);
			g_screen_display_manager->AddScreenController((Flags::osui_game_state)(Flags::_osui_game_state_main_menu | Flags::_osui_game_state_pause_menu)
				, (Flags::osui_screen_flags)(Flags::_osui_screen_flags_key_toggled
					| Flags::_osui_screen_flags_show_cursor
					| Flags::_osui_screen_flags_is_modal
					| Flags::_osui_screen_flags_esckey_toggled)
				, Enums::Key::_KeyF7
				, false
				, true
				, std::make_shared<Screen::c_screen_controller_mainmenu>(screen_definition));

			g_screen_display_manager->SetGameState(Flags::_osui_game_state_main_menu);
		}

		void UnloadUI()
		{
			g_screen_display_manager->ClearScreenControllers();
		}

		void Initialize3D(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentation_parameters)
		{
			if(!g_ui_package_present)
			{
				return;
			}

			LoadUI(device, presentation_parameters);
		}

		void OnLostDevice()
		{
			if(!g_ui_package_present)
			{
				return;
			}

			UnloadUI();
			g_mouse_pointer.Release();
			g_canvas->Release();
		}

		void OnResetDevice(D3DPRESENT_PARAMETERS* presentation_parameters)
		{
			if(!g_ui_package_present)
			{
				return;
			}

			LoadUI(DX9::Direct3DDevice(), presentation_parameters);
		}

		void Render()
		{
			if(!g_ui_package_present)
			{
				return;
			}

			g_canvas->Draw();
		}

		void Release()
		{
			if(!g_ui_package_present)
			{
				return;
			}

			UnloadUI();
			g_mouse_pointer.Release();
			g_canvas->Release();
		}
	};};
};
#endif