/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI.hpp"

#if !PLATFORM_IS_DEDI

#include <YeloLib/configuration/c_configuration_file_factory.hpp>
#include <YeloLib/Halo1/cache/shared_cache_files.hpp>
#include <BlamLib/Halo1/game/game_globals.hpp>
#include <BlamLib/Halo1/game/game_globals_structures.hpp>
#include <BlamLib/Halo1/cache/cache_files.hpp>
#include <BlamLib/Halo1/cache/cache_files_globals.hpp>

#include "Game/GameState.hpp"
#include "Common/FileIO.hpp"
#include "Rasterizer/DX9/DX9.hpp"
#include "Interface/Controls.hpp"
#include "Interface/Keystone.hpp"

#include "Interface/OpenSauceUI/resource_id.hpp"
#include "Interface/OpenSauceUI/Control/i_canvas.hpp"
#include "Interface/OpenSauceUI/Definitions/c_screen_definition.hpp"
#include "Interface/OpenSauceUI/ControlFactory/c_control_factory.hpp"
#include "Interface/OpenSauceUI/Input/c_control_input_halo.hpp"

#include "Interface/OpenSauceUI/Screen/screen_ids.hpp"
#include "Interface/OpenSauceUI/Screen/c_screen_definition_reader.hpp"
#include "Interface/OpenSauceUI/Screen/c_screen_display_manager.hpp"
#include "Interface/OpenSauceUI/Screen/c_screen_controller_mainmenu.hpp"
#include "Interface/OpenSauceUI/Screen/c_screen_controller_mainmenubottombar.hpp"
#include "Interface/OpenSauceUI/Screen/c_screen_controller_mapdownload.hpp"
#include "Interface/OpenSauceUI/Screen/c_screen_controller_ingame.hpp"

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
#include "Interface/OpenSauceUI/GwenUI/ControlBuilders/c_control_builder_gwen_pagecontrol.hpp"
#include "Interface/OpenSauceUI/GwenUI/ControlBuilders/c_control_builder_gwen_button.hpp"
#include "Interface/OpenSauceUI/GwenUI/ControlBuilders/c_control_builder_gwen_pointer.hpp"
#include "Interface/OpenSauceUI/GwenUI/ControlBuilders/c_control_builder_gwen_progressbar.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI
	{
		static bool g_ui_package_present;
		static c_packed_file g_ui_package;

		static ControlFactory::c_control_factory g_control_factory;
		static Input::c_control_input_halo g_control_input;
		static Control::canvas_ptr_t g_canvas;
		static Control::mouse_pointer_ptr_t g_mouse_pointer;

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
			g_mouse_pointer = std::make_unique<GwenUI::c_mouse_pointer_gwen>(g_control_factory, *g_canvas);
			g_screen_display_manager = std::make_unique<Screen::c_screen_display_manager>(*g_canvas, *g_mouse_pointer, g_control_factory);

			Keystone::AttachWindowsMessageHandler(&g_control_input);

			g_control_factory.AddControl("Label", std::make_unique<GwenUI::ControlBuilders::c_control_builder_gwen_label>());
			g_control_factory.AddControl("CheckBox", std::make_unique<GwenUI::ControlBuilders::c_control_builder_gwen_checkbox>());
			g_control_factory.AddControl("CheckBoxWithLabel", std::make_unique<GwenUI::ControlBuilders::c_control_builder_gwen_checkboxwithlabel>());
			g_control_factory.AddControl("Rectangle", std::make_unique<GwenUI::ControlBuilders::c_control_builder_gwen_rectangle>());
			g_control_factory.AddControl("HorizontalSlider", std::make_unique<GwenUI::ControlBuilders::c_control_builder_gwen_horizontalslider>());
			g_control_factory.AddControl("VerticalSlider", std::make_unique<GwenUI::ControlBuilders::c_control_builder_gwen_verticalslider>());
			g_control_factory.AddControl("TextBox", std::make_unique<GwenUI::ControlBuilders::c_control_builder_gwen_textbox>());
			g_control_factory.AddControl("TextBoxNumeric", std::make_unique<GwenUI::ControlBuilders::c_control_builder_gwen_textboxnumeric>());
			g_control_factory.AddControl("ScrollControl", std::make_unique<GwenUI::ControlBuilders::c_control_builder_gwen_scrollcontrol>());
			g_control_factory.AddControl("PageControl", std::make_unique<GwenUI::ControlBuilders::c_control_builder_gwen_pagecontrol>());
			g_control_factory.AddControl("Button", std::make_unique<GwenUI::ControlBuilders::c_control_builder_gwen_button>());
			g_control_factory.AddControl("Pointer", std::make_unique<GwenUI::ControlBuilders::c_control_builder_gwen_pointer>());
			g_control_factory.AddControl("ProgressBar", std::make_unique<GwenUI::ControlBuilders::c_control_builder_gwen_progressbar>());
		}

		void Dispose()
		{
			Keystone::DetachWindowsMessageHandler(&g_control_input);

			g_screen_display_manager.reset();
			g_mouse_pointer.reset();
			g_canvas.reset();
			g_ui_package.CloseFile();
		}

		void Update()
		{
			if(!g_ui_package_present)
			{
				return;
			}

			if(g_screen_display_manager->ScreenActive())
			{
				g_control_input.Update();
			}

			// Update the UI according to the game's current state
			bool in_menu =				Yelo::Input::IsInMenu();
			bool in_chat =				Yelo::Input::IsInChat();
			bool is_loading =			GameState::GameGlobals()->map_loading_in_progress;
			bool is_main_menu_cache =	Yelo::Cache::CacheFileGlobals()->cache_header.cache_type == Enums::_shared_cache_type_main_menu;

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

		bool IsUIAvailable()
		{
			return g_ui_package_present;
		}

		void ShowScreen(const uint32 screen_id)
		{
			g_screen_display_manager->ShowScreen(screen_id);
		}

		void HideScreen(const uint32 screen_id)
		{
			g_screen_display_manager->HideScreen(screen_id);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Adds a screen controller to the display manager. </summary>
		///
		/// <typeparam name="ControllerType">	Type of the controller. </typeparam>
		/// <param name="screen_id">	  	Identifier for the screen. </param>
		/// <param name="definition_name">	Name of the definition. </param>
		/// <param name="loaded_states">  	The states the screen should be loaded in. </param>
		/// <param name="active_states">  	The active screen states. </param>
		/// <param name="screen_flags">   	The screen flags. </param>
		/// <param name="toggle_key">	  	(Optional) The screens toggle key. </param>
		template<typename ControllerType>
		static void AddScreenController(const uint32 screen_id
			, cstring definition_name
			, const Flags::osui_game_state loaded_states
			, const Flags::osui_game_state active_states
			, const Flags::osui_screen_flags screen_flags
			, const Enums::key_code toggle_key = Enums::k_number_of_keys)
		{
			// Get the screen definition from the ui package
			Definitions::c_screen_definition screen_definition;
			Screen::c_screen_definition_registry::GetScreenDefinition(g_ui_package, definition_name, screen_definition);

			// Create the controller and add it to the display manager
			g_screen_display_manager->AddScreenController(screen_id
				, loaded_states
				, active_states
				, screen_flags
				, toggle_key
				, std::make_shared<ControllerType>(screen_definition));
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Loads the OpenSauce user interface. </summary>
		///
		/// <param name="device">				  	[in] If non-null, the current render device. </param>
		/// <param name="presentation_parameters">
		/// 	[in,out] If non-null, the device's presentation parameters.
		/// </param>
		void LoadUI(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentation_parameters)
		{
			// Update the mouse bounds and initial position
			g_control_input.SetMouseBounds(0, presentation_parameters->BackBufferWidth, 0, presentation_parameters->BackBufferHeight);
			g_control_input.SetMousePosition(presentation_parameters->BackBufferWidth / 2, presentation_parameters->BackBufferHeight / 2);

			// Build the canvas and mouse pointer
			g_canvas->Initialize(device, g_ui_package, g_control_input);
			g_canvas->SetSize(presentation_parameters->BackBufferWidth, presentation_parameters->BackBufferHeight);
			g_mouse_pointer->BuildMouse(g_control_input);

			// Add all screen controllers
			AddScreenController<Screen::c_screen_controller_mainmenu>(K_SCREEN_MAIN_MENU
				, "MainMenu"
				, (Flags::osui_game_state)(Flags::_osui_game_state_main_menu | Flags::_osui_game_state_pause_menu | Flags::_osui_game_state_in_game)
				, (Flags::osui_game_state)(Flags::_osui_game_state_main_menu | Flags::_osui_game_state_pause_menu)
				, (Flags::osui_screen_flags)(Flags::_osui_screen_flags_key_toggled
					| Flags::_osui_screen_flags_show_cursor
					| Flags::_osui_screen_flags_is_modal
					| Flags::_osui_screen_flags_esckey_toggled)
				, Enums::_key_code_f7);

			AddScreenController<Screen::c_screen_controller_mainmenubottombar>(K_SCREEN_MAIN_MENU_BOTTOM
				, "MainMenuBottomBar"
				, Flags::_osui_game_state_main_menu
				, Flags::_osui_game_state_main_menu
				, Flags::_osui_screen_flags_always_visible);

			AddScreenController<Screen::c_screen_controller_mapdownload>(K_SCREEN_MAP_DOWNLOAD
				, "MapDownload"
				, (Flags::osui_game_state)(Flags::_osui_game_state_all)
				, (Flags::osui_game_state)(Flags::_osui_game_state_all)
				, Flags::_osui_screen_flags_is_modal);

			AddScreenController<Screen::c_screen_controller_ingame>(K_SCREEN_IN_GAME
				, "InGame"
				, (Flags::osui_game_state)(Flags::_osui_game_state_pause_menu | Flags::_osui_game_state_in_game)
				, Flags::_osui_game_state_in_game
				, (Flags::osui_screen_flags)(Flags::_osui_screen_flags_key_toggled
				| Flags::_osui_screen_flags_esckey_toggled
				| Flags::_osui_screen_flags_is_modal
				| Flags::_osui_screen_flags_show_cursor
				| Flags::_osui_screen_flags_disable_movement)
				, Enums::_key_code_f7);

			// Set the initial display state
			g_screen_display_manager->SetGameState(Flags::_osui_game_state_main_menu);
		}

		/// <summary>	Unloads the OpenSauce user interface. </summary>
		void UnloadUI()
		{
			// Delete the screen controllers, canvas and mouse
			g_screen_display_manager->ClearScreenControllers();
			g_mouse_pointer->DestroyMouse(g_control_input);
			g_canvas->Release(g_control_input);
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
		}
	};};
};
#endif