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
			Gwen::UnicodeChar m_lowercase;
			Gwen::UnicodeChar m_uppercase;
		};

		static const s_wchar_case g_gwen_character_map[Enums::_Key] =
		{
			{ true, 0, 0 },
			{ true, 0, 0 },
			{ true, 0, 0 },
			{ true, 0, 0 },
			{ true, 0, 0 },
			{ true, 0, 0 },
			{ true, 0, 0 },
			{ true, 0, 0 },
			{ true, 0, 0 },
			{ true, 0, 0 },
			{ true, 0, 0 },
			{ true, 0, 0 },
			{ true, 0, 0 },
			{ true, 0, 0 },
			{ true, 0, 0 },

			{ true, 0, 0 },
			{ true, L'`', L'¬' },

			{ true, L'1', L'!' },
			{ true, L'2', L'"' },
			{ true, L'3', L'#' },
			{ true, L'4', L'$' },
			{ true, L'5', L'%' },
			{ true, L'6', L'^' },
			{ true, L'7', L'&' },
			{ true, L'8', L'*' },
			{ true, L'9', L'(' },
			{ true, L'0', L')' },
			{ true, L'-', L'_' },
			{ true, L'=', L'+' },
			{ true, 0, 0 },

			{ true, 0, 0 },
			{ false, L'q', L'Q' },
			{ false, L'w', L'W' },
			{ false, L'e', L'E' },
			{ false, L'r', L'R' },
			{ false, L't', L'T' },
			{ false, L'y', L'Y' },
			{ false, L'u', L'U' },
			{ false, L'i', L'I' },
			{ false, L'o', L'O' },
			{ false, L'p', L'P' },
			{ true, L'[', L'{' },
			{ true, L']', L'}' },
			{ true, L'\\', L'|' },

			{ true, 0, 0 },
			{ false, L'a', L'A' },
			{ false, L's', L'S' },
			{ false, L'd', L'D' },
			{ false, L'f', L'F' },
			{ false, L'g', L'G' },
			{ false, L'h', L'H' },
			{ false, L'j', L'J' },
			{ false, L'k', L'K' },
			{ false, L'l', L'L' },
			{ true, L';', L':' },
			{ true, L'\'', L'"' },
			{ true, 0, 0 },

			{ true, 0, 0 },
			{ false, L'z', L'Z' },
			{ false, L'x', L'X' },
			{ false, L'c', L'C' },
			{ false, L'v', L'V' },
			{ false, L'b', L'B' },
			{ false, L'n', L'N' },
			{ false, L'm', L'M' },
			{ true, L',', L'<' },
			{ true, L'.', L'>' },
			{ true, L'/', L'?' },
			{ true, 0, 0 },

			{ true, 0, 0 },
			{ true, 0, 0 },
			{ true, 0, 0 },
			{ true, L' ', L' ' },
			{ true, 0, 0 },
			{ true, 0, 0 },
			{ true, 0, 0 },
			{ true, 0, 0 },

			{ true, 0, 0 },
			{ true, 0, 0 },
			{ true, 0, 0 },
			{ true, 0, 0 },

			{ true, 0, 0 },
			{ true, 0, 0 },
			{ true, 0, 0 },
			{ true, 0, 0 },
			{ true, 0, 0 },
			{ true, 0, 0 },

			{ true, 0, 0 },
			{ true, L'/', L'/' },
			{ true, L'*', L'+' },
			{ true, L'0', L'0' },
			{ true, L'1', L'1' },
			{ true, L'2', L'2' },
			{ true, L'3', L'3' },
			{ true, L'4', L'4' },
			{ true, L'5', L'5' },
			{ true, L'6', L'6' },
			{ true, L'7', L'7' },
			{ true, L'8', L'8' },
			{ true, L'9', L'9' },
			{ true, L'-', L'-' },
			{ true, L'+', L'+' },
			{ true, 0, 0 },
			{ true, L'.', L'.' },
		};
#pragma endregion

		void c_control_input_halo::Initialize(Gwen::Controls::Canvas* canvas)
		{
			m_canvas = canvas;
		}

		void c_control_input_halo::SetMouseBounds(const int min_x, const int max_x, const int min_y, const int max_y)
		{
			m_mouse.bounds.left = min_x;
			m_mouse.bounds.right = max_x;
			m_mouse.bounds.bottom = min_y;
			m_mouse.bounds.top = max_y;
		}

		void c_control_input_halo::SetMousePosition(const int pos_x, const int pos_y)
		{
			m_mouse.position.x = pos_x;
			m_mouse.position.y = pos_y;
		}

		void c_control_input_halo::GetMousePosition(int& pos_x, int& pos_y)
		{
			pos_x = m_mouse.position.x;
			pos_y = m_mouse.position.y;
		}

		void c_control_input_halo::Update()
		{
			if (!m_canvas)
			{
				return;
			}

			// Update the mouse position
			int x = m_mouse.position.x + Yelo::Input::GetMouseAxisState(Enums::_MouseAxisX);
			int y = m_mouse.position.y + -Yelo::Input::GetMouseAxisState(Enums::_MouseAxisY);
			int dx = x - m_mouse.position.x;
			int dy = y - m_mouse.position.y;

			m_mouse.position.x = __min(x,					m_mouse.bounds.right);
			m_mouse.position.x = __max(m_mouse.position.x,	m_mouse.bounds.left);
			m_mouse.position.y = __min(y,					m_mouse.bounds.top);
			m_mouse.position.y = __max(m_mouse.position.y,	m_mouse.bounds.bottom);

			m_canvas->InputMouseMoved(m_mouse.position.x, m_mouse.position.y, dx, dy);
#ifdef WM_MOUSEWHEEL 
			m_canvas->InputMouseWheel(Yelo::Input::GetMouseAxisState(Enums::_MouseAxisWheel));
#endif

			// Update the mouse button's
			auto get_mouse_input =
				[&](const Enums::MouseButton button, const int32 index)
				{
					auto value = Yelo::Input::GetMouseButtonState(Enums::_MouseButton1) > 0;

					if(value != m_mouse.mouse_button_states[index])
					{
						m_canvas->InputMouseButton( index, value);
						m_mouse.mouse_button_states[index] = value;
					}
				};
			
			get_mouse_input(Enums::_MouseButton1, 0);
			get_mouse_input(Enums::_MouseButton2, 1);
			get_mouse_input(Enums::_MouseButton3, 2);

			// Update the keyboard key's
			for(int i = 0; i < Enums::_Key; i++)
			{
				Enums::Key key = (Enums::Key)i;

				// Only process a keys input if it has changed since the last update
				bool key_state = (Yelo::Input::GetKeyState(key) > 0);
				if(m_keyboard.key_states[i] == key_state)
					continue;
				m_keyboard.key_states[i] = key_state;

				// These keys are not supported/handled by gwen so skip them
				if(Enums::_KeyCaps == key) continue;
				else if(Enums::_KeyInsert == key) continue;
				else if(Enums::_KeyPgUp == key) continue;
				else if(Enums::_KeyPgDown == key) continue;
				else if(Enums::_KeyAppsMenu == key) continue;
				else if(Enums::_KeyLWin == key) continue;
				else if(Enums::_KeyNumsLock == key) continue;
				else if((key >= Enums::_KeyF1) && (key <= Enums::_KeyPause)) continue;

				// Map special keys to gwens key enum values
				int gwen_key = -1;
				if((Enums::_KeyLShift == i) || (Enums::_KeyRShift == i)) gwen_key = Gwen::Key::Shift;
				else if((Enums::_KeyEnter == i) || (Enums::_KeyNumEnter == i)) gwen_key = Gwen::Key::Return;
				else if(Enums::_KeyBackspace == i) gwen_key = Gwen::Key::Backspace;
				else if(Enums::_KeyDelete == i) gwen_key = Gwen::Key::Delete;
				else if(Enums::_KeyLeft == i) gwen_key = Gwen::Key::Left;
				else if(Enums::_KeyRight == i) gwen_key = Gwen::Key::Right;
				else if(Enums::_KeyTab == i) gwen_key = Gwen::Key::Tab;
				else if(Enums::_KeySpace == i) gwen_key = Gwen::Key::Space;
				else if(Enums::_KeyHome == i) gwen_key = Gwen::Key::Home;
				else if(Enums::_KeyEnd == i) gwen_key = Gwen::Key::End;
				else if((Enums::_KeyLCtrl == i) || (Enums::_KeyRCtrl == i)) gwen_key = Gwen::Key::Control;
				else if((Enums::_KeyLAlt == i) || (Enums::_KeyRAlt == i)) gwen_key = Gwen::Key::Alt;
				else if(Enums::_KeySpace == i) gwen_key = Gwen::Key::Space;
				else if(Enums::_KeyUp == i) gwen_key = Gwen::Key::Up;
				else if(Enums::_KeyDown == i) gwen_key = Gwen::Key::Down;
				else if(Enums::_KeyEsc == i) gwen_key = Gwen::Key::Escape;

				// If the key is a letter, or if it is the space bar, send a character to gwen
				if((gwen_key == -1) || (gwen_key == Gwen::Key::Space))
				{
					auto& mapped_character = g_gwen_character_map[i];

					YELO_ASSERT(mapped_character.m_lowercase != 0);

					// Only send a character when the key is pressed
					if(key_state)
					{
						Gwen::UnicodeChar character;

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

						// Send the character to gwen
						m_canvas->InputCharacter( character );
					}
				}

				// If the key is not a letter key, send it to gwen
				if(gwen_key != -1)
				{
					m_canvas->InputKey(gwen_key, key_state);
				}
			}
		}
	};};};
};
#endif