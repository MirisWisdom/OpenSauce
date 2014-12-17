/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI/Input/c_control_input_halo.hpp"

#if !PLATFORM_IS_DEDI

#include <Gwen/Gwen.h>

#include "Interface/Controls.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace Input
	{
#pragma region Character Map
		struct s_wchar_case
		{
			bool m_ignore_caps_lock;
			PAD24;
			wchar_t m_lowercase;
			wchar_t m_uppercase;
		};

		static const s_wchar_case g_character_map[Enums::_Key] =
		{
			{ true, 0, 0 },				// 0
			{ true, 0, 0 },
			{ true, 0, 0 },
			{ true, 0, 0 },
			{ true, 0, 0 },
			{ true, 0, 0 },
			{ true, 0, 0 },
			{ true, 0, 0 },
			{ true, 0, 0 },
			{ true, 0, 0 },
			{ true, 0, 0 },				// 10
			{ true, 0, 0 },
			{ true, 0, 0 },
			{ true, 0, 0 },
			{ true, 0, 0 },

			{ true, 0, 0 },
			{ true, L'`', L'¬' },

			{ true, L'1', L'!' },
			{ true, L'2', L'"' },
			{ true, L'3', L'#' },
			{ true, L'4', L'$' },		// 20
			{ true, L'5', L'%' },
			{ true, L'6', L'^' },
			{ true, L'7', L'&' },
			{ true, L'8', L'*' },
			{ true, L'9', L'(' },
			{ true, L'0', L')' },
			{ true, L'-', L'_' },
			{ true, L'=', L'+' },
			{ true, 0, 0 },

			{ true, 0, 0 },				// 30
			{ false, L'q', L'Q' },
			{ false, L'w', L'W' },
			{ false, L'e', L'E' },
			{ false, L'r', L'R' },
			{ false, L't', L'T' },
			{ false, L'y', L'Y' },
			{ false, L'u', L'U' },
			{ false, L'i', L'I' },
			{ false, L'o', L'O' },
			{ false, L'p', L'P' },		// 40
			{ true, L'[', L'{' },
			{ true, L']', L'}' },
			{ true, L'\\', L'|' },

			{ true, 0, 0 },
			{ false, L'a', L'A' },
			{ false, L's', L'S' },
			{ false, L'd', L'D' },
			{ false, L'f', L'F' },
			{ false, L'g', L'G' },
			{ false, L'h', L'H' },		// 50
			{ false, L'j', L'J' },
			{ false, L'k', L'K' },
			{ false, L'l', L'L' },
			{ true, L';', L':' },
			{ true, L'\'', L'"' },
			{ true, 0, 0 },

			{ true, 0, 0 },
			{ false, L'z', L'Z' },
			{ false, L'x', L'X' },
			{ false, L'c', L'C' },		// 60
			{ false, L'v', L'V' },
			{ false, L'b', L'B' },
			{ false, L'n', L'N' },
			{ false, L'm', L'M' },
			{ true, L',', L'<' },
			{ true, L'.', L'>' },
			{ true, L'/', L'?' },
			{ true, 0, 0 },

			{ true, 0, 0 },
			{ true, 0, 0 },				// 70
			{ true, 0, 0 },
			{ true, L' ', L' ' },
			{ true, 0, 0 },
			{ true, 0, 0 },
			{ true, 0, 0 },
			{ true, 0, 0 },

			{ true, 0, 0 },
			{ true, 0, 0 },
			{ true, 0, 0 },
			{ true, 0, 0 },				// 80

			{ true, 0, 0 },
			{ true, 0, 0 },
			{ true, 0, 0 },
			{ true, 0, 0 },
			{ true, 0, 0 },
			{ true, 0, 0 },

			{ true, 0, 0 },
			{ true, L'/', L'/' },
			{ true, L'*', L'+' },
			{ true, L'0', L'0' },		// 90
			{ true, L'1', L'1' },
			{ true, L'2', L'2' },
			{ true, L'3', L'3' },
			{ true, L'4', L'4' },
			{ true, L'5', L'5' },
			{ true, L'6', L'6' },
			{ true, L'7', L'7' },
			{ true, L'8', L'8' },
			{ true, L'9', L'9' },
			{ true, L'-', L'-' },		// 100
			{ true, L'+', L'+' },
			{ true, 0, 0 },
			{ true, L'.', L'.' },
		};
#pragma endregion

		c_control_input_halo::c_control_input_halo()
			: m_mouse_input_handlers()
			, m_keyboard_input_handlers()
			, m_mouse_bounds()
			, m_mouse_position()
		{ }

		void c_control_input_halo::SetMouseBounds(const int min_x, const int max_x, const int min_y, const int max_y)
		{
			m_mouse_bounds.left = min_x;
			m_mouse_bounds.right = max_x;
			m_mouse_bounds.bottom = min_y;
			m_mouse_bounds.top = max_y;
		}

		void c_control_input_halo::Update()
		{
			//UpdateMouseMovement();
			//UpdateMouseButtons();
			UpdateKeyboardButtons();
		}

		void c_control_input_halo::AttachMouseInputHandler(i_control_mouse_handler* handler)
		{
			auto existing_entry = std::find_if(m_mouse_input_handlers.begin(), m_mouse_input_handlers.end(),
				[handler](i_control_mouse_handler* entry)
				{
					return entry == handler;
				});

			if(existing_entry == m_mouse_input_handlers.end())
			{
				m_mouse_input_handlers.push_back(handler);
			}
		}

		void c_control_input_halo::DetachMouseInputHandler(const i_control_mouse_handler* handler)
		{
			auto existing_entry = std::find_if(m_mouse_input_handlers.begin(), m_mouse_input_handlers.end(),
				[handler](const i_control_mouse_handler* entry)
				{
					return entry == handler;
				});

			if(existing_entry != m_mouse_input_handlers.end())
			{
				m_mouse_input_handlers.erase(existing_entry);
			}
		}

		void c_control_input_halo::AttachKeyboardInputHandler(i_control_keyboard_handler* handler)
		{
			auto existing_entry = std::find_if(m_keyboard_input_handlers.begin(), m_keyboard_input_handlers.end(),
				[handler](i_control_keyboard_handler* entry)
				{
					return entry == handler;
				});

			if(existing_entry == m_keyboard_input_handlers.end())
			{
				m_keyboard_input_handlers.push_back(handler);
			}
		}

		void c_control_input_halo::DetachKeyboardInputHandler(const i_control_keyboard_handler* handler)
		{
			auto existing_entry = std::find_if(m_keyboard_input_handlers.begin(), m_keyboard_input_handlers.end(),
				[handler](const i_control_keyboard_handler* entry)
				{
					return entry == handler;
				});

			if(existing_entry != m_keyboard_input_handlers.end())
			{
				m_keyboard_input_handlers.erase(existing_entry);
			}
		}

		void c_control_input_halo::UpdateMouseMovement()
		{
			// Update the mouse position
			int x = m_mouse_position.x + Yelo::Input::GetMouseAxisState(Enums::_MouseAxisX);
			int y = m_mouse_position.y + -Yelo::Input::GetMouseAxisState(Enums::_MouseAxisY);
			int dx = x - m_mouse_position.x;
			int dy = y - m_mouse_position.y;

			m_mouse_position.x = __min(x,					m_mouse_bounds.right);
			m_mouse_position.x = __max(m_mouse_position.x,	m_mouse_bounds.left);
			m_mouse_position.y = __min(y,					m_mouse_bounds.top);
			m_mouse_position.y = __max(m_mouse_position.y,	m_mouse_bounds.bottom);

			for(auto handler : m_mouse_input_handlers)
			{
				handler->OnMousePositionUpdate(m_mouse_position, point2d { dx, dy });
			}
		}

		void c_control_input_halo::UpdateMouseButtons()
		{
#ifdef WM_MOUSEWHEEL
			for(auto handler : m_mouse_input_handlers)
			{
				handler->OnMouseWheelUpdate(Yelo::Input::GetMouseAxisState(Enums::_MouseAxisWheel));
			}
#endif

			// Update the mouse button's
			auto get_mouse_input =
				[&](const Enums::MouseButton button, const int32 index)
				{
					auto value = Yelo::Input::GetMouseButtonState(Enums::_MouseButton1) > 0;

					if(value != m_mouse_button_states[index])
					{
						for(auto handler : m_mouse_input_handlers)
						{
							handler->OnMouseButtonUpdate(index, value);
						}

						m_mouse_button_states[index] = value;
					}
				};
			
			get_mouse_input(Enums::_MouseButton1, 0);
			get_mouse_input(Enums::_MouseButton2, 1);
			get_mouse_input(Enums::_MouseButton3, 2);
		}

		void c_control_input_halo::UpdateKeyboardButtons()
		{
			// Update the keyboard key's
			for(int i = 0; i < Enums::_Key; i++)
			{
				Enums::Key key = (Enums::Key)i;

				// Only process a keys input if it has changed since the last update
				bool key_state = (Yelo::Input::GetKeyState(key) > 0);
				if(m_keyboard_states[i] == key_state)
				{
					continue;
				}
				m_keyboard_states[i] = key_state;

				// These keys are not supported/handled so skip them
				
				bool is_handled = true;
				switch(key)
				{
					case Enums::_KeyCaps:
					case Enums::_KeyInsert:
					case Enums::_KeyPgUp:
					case Enums::_KeyPgDown:
					case Enums::_KeyAppsMenu:
					case Enums::_KeyLWin:
					case Enums::_KeyNumsLock:
					case Enums::_KeyF1:
					case Enums::_KeyF2:
					case Enums::_KeyF3:
					case Enums::_KeyF4:
					case Enums::_KeyF5:
					case Enums::_KeyF6:
					case Enums::_KeyF7:
					case Enums::_KeyF8:
					case Enums::_KeyF9:
					case Enums::_KeyF10:
					case Enums::_KeyF11:
					case Enums::_KeyF12:
					case Enums::_KeyPrntScrn:
					case Enums::_KeyScrollLock:
					case Enums::_KeyPause:
						is_handled = false;
				};

				if(!is_handled)
				{
					continue;
				}

				bool is_character = true;
				switch(key)
				{
					case Enums::_KeyLShift:
					case Enums::_KeyRShift:
					case Enums::_KeyEnter:
					case Enums::_KeyNumEnter:
					case Enums::_KeyBackspace:
					case Enums::_KeyDelete:
					case Enums::_KeyLeft:
					case Enums::_KeyRight:
					case Enums::_KeyTab:
					case Enums::_KeyHome:
					case Enums::_KeyEnd:
					case Enums::_KeyLCtrl:
					case Enums::_KeyRCtrl:
					case Enums::_KeyLAlt:
					case Enums::_KeyRAlt:
					case Enums::_KeyUp:
					case Enums::_KeyDown:
					case Enums::_KeyEsc:
						is_character = false;
						break;
				};

				// If the key is a character
				if(is_character)
				{
					auto& mapped_character = g_character_map[i];

					YELO_ASSERT(mapped_character.m_lowercase != 0);

					// Only send a character when the key is pressed
					if(key_state)
					{
						wchar_t character;

						// Manually handle toggling between upper and lower case
						bool use_capslock = ((GetKeyState(VK_CAPITAL) & 0x0001) > 0) && !mapped_character.m_ignore_caps_lock;
						bool use_shift = (GetKeyState(VK_SHIFT) & 0x8000) > 0;

						if(use_capslock && use_shift)
							character = mapped_character.m_lowercase;
						else if(use_capslock && !use_shift)
							character = mapped_character.m_uppercase;
						else if(use_shift)
							character = mapped_character.m_uppercase;
						else
							character = mapped_character.m_lowercase;

						// Send the character to the handlers
						for(auto handler : m_keyboard_input_handlers)
						{
							handler->OnKeyboardCharacterPressed(character);
						}
					}
				}
				else
				{
					// Send the character to the handlers
					for(auto handler : m_keyboard_input_handlers)
					{
						handler->OnKeyboardButtonUpdate(key, key_state);
					}
				}
			}
		}
	};};};
};
#endif