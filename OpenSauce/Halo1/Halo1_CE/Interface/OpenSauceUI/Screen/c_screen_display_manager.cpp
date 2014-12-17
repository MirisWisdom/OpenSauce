/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI/Screen/c_screen_display_manager.hpp"

#if !PLATFORM_IS_DEDI

#include "Memory/FunctionInterface.hpp"
#include "Interface/Controls.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace Screen
	{
		c_screen_display_manager::c_screen_display_manager(Control::i_canvas& canvas
			, Control::i_mouse_pointer& mouse_pointer
			, ControlFactory::c_control_factory& control_factory)
			: m_canvas(canvas)
			, m_mouse_pointer(mouse_pointer)
			, m_control_factory(control_factory)
			, m_current_state(Flags::_osui_game_state_none)
			, m_mouse_show_count(0)
			, m_modal_screen_count(0)
			, m_disable_movement_count(0)
			, m_screen_instances()
			, m_current_stage_instances()
		{ }

		void c_screen_display_manager::AddScreenController(const Flags::osui_game_state game_states
			, const Flags::osui_screen_flags screen_flags
			, const Enums::Key toggle_key
			, t_screen_controller_ptr controller)
		{
			m_screen_instances.push_back(s_screen_instance { game_states, screen_flags, toggle_key, controller });
		}

		void c_screen_display_manager::ClearScreenControllers()
		{
			// Destroy all screens and delete all screen instances
			m_current_stage_instances.clear();

			for(auto& instance: m_screen_instances)
			{
				HideScreen(instance);
				instance.m_screen_controller->DestroyScreen(m_canvas);
			}
			m_screen_instances.clear();

			m_current_state = Flags::_osui_game_state_none;
		}

		void c_screen_display_manager::SetGameState(const Flags::osui_game_state state)
		{
			if(m_current_state == state)
			{
				return;
			}

			m_current_state = state;

			// Destroy and screens that are not valid for this stage
			for(auto* instance : m_current_stage_instances)
			{
				bool applicable_state = (instance->m_game_states & state) > 0;
				if(!applicable_state)
				{
					HideScreen(*instance);
					instance->m_screen_controller->DestroyScreen(m_canvas);
				}
			}

			m_current_stage_instances.clear();

			// Get all of the screen instances for this state
			for(auto& instance : m_screen_instances)
			{
				if(instance.m_game_states & state)
				{
					m_current_stage_instances.push_back(&instance);

					// Build the screen instances
					instance.m_screen_controller->BuildScreen(m_control_factory, m_canvas);

					// If the screen should always be visible set it to be displayed
					if(instance.m_screen_flags & Flags::_osui_screen_flags_always_visible)
					{
						ShowScreen(instance);
					}
					else
					{
						HideScreen(instance);
					}
				}
			}
		}

		void c_screen_display_manager::Update()
		{
			// Get whether esc has been pressed
			bool close_all = false;
			auto esc_state = Yelo::Input::GetKeyState(Enums::_KeyEsc);
			if((esc_state != m_previous_esckey_state) && (esc_state == 1))
			{
				close_all = true;
			}
			m_previous_esckey_state = esc_state;

			bool screen_esc_hidden = false;

			// If the screen has a toggle key, check whether the key has been pressed and toggle the screen Also close
			for(auto* instance : m_current_stage_instances)
			{
				auto& screen_instance = *instance;

				if(screen_instance.m_screen_flags & Flags::_osui_screen_flags_key_toggled)
				{
					// If esc was pressed and the screen can be closed with esc, close it
					if((screen_instance.m_screen_flags & Flags::_osui_screen_flags_esckey_toggled) && close_all)
					{
						if(screen_instance.m_is_visible)
						{
							HideScreen(screen_instance);

							screen_esc_hidden = true;
						}
					}
					else
					{
						// If the screen has a toggle key, check whether the key has been pressed and toggle the screen Also close
						auto state = Yelo::Input::GetKeyState(screen_instance.m_toggle_key);
						if((state != screen_instance.m_previous_key_state) && (state == 1))
						{
							if(screen_instance.m_is_visible)
							{
								HideScreen(screen_instance);
							}
							else
							{
								ShowScreen(screen_instance);
							}
						}
						screen_instance.m_previous_key_state = state;
					}
				}

				// Update screens that are visible or set to always update
				if((screen_instance.m_screen_flags & Flags::_osui_screen_flags_always_update)
					|| screen_instance.m_is_visible)
				{
					screen_instance.m_screen_controller->Update();
				}
			}

			// If the escape button triggered a screen to be closed, reset it's state so that Halo doesn't react to it
			if(screen_esc_hidden)
			{
				Input::SetKeyState(Enums::_KeyEsc, 0);
			}
		}
		
		void c_screen_display_manager::ShowScreen(s_screen_instance& screen)
		{
			screen.m_screen_controller->Show();
			screen.m_is_visible = true;

			if(screen.m_screen_flags & Flags::_osui_screen_flags_is_modal)
			{
				DisableHaloUI();
			}

			if(screen.m_screen_flags & Flags::_osui_screen_flags_show_cursor)
			{
				ShowMousePointer();
			}

			if(screen.m_screen_flags & Flags::_osui_screen_flags_disable_movement)
			{
				DisableMovement();
			}
		}

		void c_screen_display_manager::HideScreen(s_screen_instance& screen)
		{
			screen.m_screen_controller->Hide();
			screen.m_is_visible = false;

			if(screen.m_screen_flags & Flags::_osui_screen_flags_is_modal)
			{
				EnableHaloUI();
			}

			if(screen.m_screen_flags & Flags::_osui_screen_flags_show_cursor)
			{
				HideMousePointer();
			}

			if(screen.m_screen_flags & Flags::_osui_screen_flags_disable_movement)
			{
				EnableMovement();
			}
		}

		void c_screen_display_manager::ShowMousePointer()
		{
			if(!m_mouse_show_count)
			{
				m_mouse_pointer.Show();
				Memory::FunctionProcessRenderCursorDisabled() = true;
			}

			m_mouse_show_count++;
		}

		void c_screen_display_manager::HideMousePointer()
		{
			m_mouse_show_count--;

			if(m_mouse_show_count <= 0)
			{
				m_mouse_show_count = 0;
				m_mouse_pointer.Hide();
				Memory::FunctionProcessRenderCursorDisabled() = false;
			}
		}

		void c_screen_display_manager::EnableHaloUI()
		{
			m_modal_screen_count--;

			if(m_modal_screen_count <= 0)
			{
				m_modal_screen_count = 0;
				Memory::FunctionProcessUpdateUIWidgetsDisabled() = false;
			}
		}

		void c_screen_display_manager::DisableHaloUI()
		{
			if(!m_modal_screen_count)
			{
				Memory::FunctionProcessUpdateUIWidgetsDisabled() = true;
			}

			m_modal_screen_count++;
		}

		void c_screen_display_manager::EnableMovement()
		{		
			m_disable_movement_count--;

			if(m_disable_movement_count <= 0)
			{
				m_disable_movement_count = 0;
				Input::AllowMovement(true);
			}
		}

		void c_screen_display_manager::DisableMovement()
		{
			if(!m_disable_movement_count)
			{
				Input::AllowMovement(false);
			}

			m_disable_movement_count++;
		}
	};};};
};
#endif