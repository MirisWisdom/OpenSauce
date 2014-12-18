/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include "Interface/Controls.hpp"
#include "Interface/OpenSauceUI/Control/i_control.hpp"
#include "Interface/OpenSauceUI/Control/i_mouse_pointer.hpp"
#include "Interface/OpenSauceUI/Screen/i_screen_controller.hpp"

namespace Yelo
{
	namespace Flags
	{
		enum osui_game_state : uint16
		{
			_osui_game_state_none				= 0,
			_osui_game_state_main_menu			= 1 << 1,
			_osui_game_state_loading			= 1 << 2,
			_osui_game_state_in_game			= 1 << 3,
			_osui_game_state_pause_menu			= 1 << 4
		};

		enum osui_screen_flags : uint16
		{
			_osui_screen_flags_none				= 0,
			_osui_screen_flags_show_cursor		= 1 << 0,
			_osui_screen_flags_is_modal			= 1 << 1,
			_osui_screen_flags_key_toggled		= 1 << 2,
			_osui_screen_flags_esckey_toggled	= 1 << 3,
			_osui_screen_flags_always_visible	= 1 << 4,
			_osui_screen_flags_always_update	= 1 << 5,
			_osui_screen_flags_disable_movement	= 1 << 6
		};
	};

	namespace Interface { namespace OpenSauceUI { namespace Screen
	{
		/// <summary>	Manager for screen visibility. </summary>
		class c_screen_display_manager
		{
			struct s_screen_instance
			{
				const Flags::osui_game_state m_game_states;
				const Flags::osui_screen_flags m_screen_flags;
				const Enums::key_code m_toggle_key;
				t_screen_controller_ptr m_screen_controller;

				bool m_is_visible;
				byte m_previous_key_state;
			};
			
			Control::i_canvas& m_canvas;
			Control::i_mouse_pointer& m_mouse_pointer;
			ControlFactory::c_control_factory& m_control_factory;

			Flags::osui_game_state m_current_state;
			int16 m_active_screen_count;
			int16 m_mouse_show_count;
			int16 m_modal_screen_count;
			int16 m_disable_movement_count;
			byte m_previous_esckey_state;
			PAD24;

			std::vector<s_screen_instance> m_screen_instances;
			std::vector<s_screen_instance*> m_current_stage_instances;

		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Constructor. </summary>
			///
			/// <param name="canvas">		  	[in] The target canvas. </param>
			/// <param name="mouse_pointer">  	[in] The mouse pointer. </param>
			/// <param name="control_factory">	[in] The control factory. </param>
			c_screen_display_manager(Control::i_canvas& canvas
				, Control::i_mouse_pointer& mouse_pointer
				, ControlFactory::c_control_factory& control_factory);

			bool ScreenActive();

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Adds a screen controller. </summary>
			///
			/// <param name="game_states"> 	The states that the screen is valid in. </param>
			/// <param name="screen_flags">	The screens flags. </param>
			/// <param name="toggle_key">  	The toggle key. </param>
			/// <param name="is_modal">	   	When true the screen will disable Halo's input whilst displayed. </param>
			/// <param name="show_cursor"> 	When true the OpenSauceUI cursor will be displayed and Halo's will be hidden. </param>
			/// <param name="controller">  	The screen controller. </param>
			void AddScreenController(const Flags::osui_game_state game_states
				, const Flags::osui_screen_flags screen_flags
				, const Enums::key_code toggle_key
				, t_screen_controller_ptr controller);

			/// <summary>	Destroys all screens and controllers. </summary>
			void ClearScreenControllers();

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets the current game ui state. </summary>
			///
			/// <param name="state">	The game state. </param>
			void SetGameState(const Flags::osui_game_state state);

			/// <summary>	Updates the active screen controllers. </summary>
			void Update();

		private:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Shows the screen. </summary>
			///
			/// <param name="screen">	[in] The screen. </param>
			void ShowScreen(s_screen_instance& screen);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Hides the screen. </summary>
			///
			/// <param name="screen">	[in] The screen. </param>
			void HideScreen(s_screen_instance& screen);

			/// <summary>	Shows the mouse pointer. </summary>
			void ShowMousePointer();

			/// <summary>	Hides the mouse pointer. </summary>
			void HideMousePointer();

			/// <summary>	Enables the halo user interface. </summary>
			void EnableHaloUI();

			/// <summary>	Disables the halo user interface</summary>
			void DisableHaloUI();

			/// <summary>	Enables player movement. </summary>
			void EnableMovement();

			/// <summary>	Disables player movement</summary>
			void DisableMovement();
		};
	};};};
};
#endif