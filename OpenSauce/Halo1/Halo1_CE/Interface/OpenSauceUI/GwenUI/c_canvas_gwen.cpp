/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI/GwenUI/c_canvas_gwen.hpp"

#if !PLATFORM_IS_DEDI

#include "Interface/OpenSauceUI/resource_id.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace GwenUI
	{
		Control::control_list_t::iterator c_canvas_gwen::FindChildControl(Control::control_ptr_t control)
		{
			return std::find_if(m_child_controls.begin(), m_child_controls.end(),
				[&](Control::control_ptr_t entry)
				{
					return entry == control;
				});
		}

#pragma region i_canvas
			void c_canvas_gwen::Initialize(IDirect3DDevice9* device, c_packed_file& ui_package, Input::i_control_input& control_input)
			{
				m_renderer = std::make_unique<c_gwen_renderer_halo>(device, ui_package);
				m_skin = std::make_unique<Gwen::Skin::TexturedBase>(nullptr);
				m_canvas = std::make_unique<Gwen::Controls::Canvas>(m_skin.get());

				m_canvas->SetDrawBackground(false);
				m_skin->SetRender(m_renderer.get());
				m_skin->Init("OpenSauceUISkin");

				control_input.AttachMouseInputHandler(this);
				control_input.AttachKeyboardInputHandler(this);
			}

			void c_canvas_gwen::Release(Input::i_control_input& control_input)
			{
				control_input.DetachMouseInputHandler(this);
				control_input.DetachKeyboardInputHandler(this);

				m_canvas.reset();
				m_skin.reset();

				m_renderer->Release();
				m_renderer.reset();
			}

			void c_canvas_gwen::SetSize(const int32 width, const int32 height)
			{
				m_canvas->SetSize(width, height);
			}

			void c_canvas_gwen::SetScale(const real scale)
			{
				m_canvas->SetScale(scale);
			}

			void c_canvas_gwen::Draw() const
			{
				m_canvas->RenderCanvas();
			}
#pragma endregion

#pragma region i_control
			Control::i_control* c_canvas_gwen::Parent()
			{
				return this;
			}

			uint32 c_canvas_gwen::GetResourceID() const
			{
				return RESOURCE_ID("#CNV_main_canvas", 0x356C7F9C);
			}

			rectangle2d c_canvas_gwen::GetBounds() const
			{
				auto& bounds = m_canvas->GetBounds();

				return rectangle2d { bounds.x, bounds.y, bounds.h, bounds.w };
			}

			void* c_canvas_gwen::GetControlPtr() const
			{
				return m_canvas.get();
			}

			void c_canvas_gwen::AddControl(Control::control_ptr_t control)
			{
				//Check whether the control already exists
				auto existing_control = FindChildControl(control);

				YELO_ASSERT_DISPLAY(existing_control == m_child_controls.end(), "Attempted to add a control instance to the canvas multiple times");

				if(existing_control == m_child_controls.end())
				{
					m_child_controls.push_back(control);
				}
			}

			void c_canvas_gwen::RemoveControl(Control::control_ptr_t control)
			{
				//Find the control to remove
				auto existing_control = FindChildControl(control);

				YELO_ASSERT_DISPLAY(existing_control != m_child_controls.end(), "Attempted to remove a non-existant control from the canvas");

				if(existing_control != m_child_controls.end())
				{
					// Remove the control's gwen controls from the canvas
					m_canvas->Children.remove_if(
						[&](Gwen::Controls::Base* child_control)
						{
							return CAST_PTR(Gwen::Controls::Base*, control->GetControlPtr()) == child_control;
						}
					);

					m_child_controls.erase(existing_control);
				}
			}

			Control::control_list_t& c_canvas_gwen::Controls()
			{
				return m_child_controls;
			}
#pragma endregion
			
#pragma region i_control_mouse_handler
			void c_canvas_gwen::OnMousePositionUpdate(const point2d& absolute, const point2d& relative)
			{
				m_canvas->InputMouseMoved(absolute.x, absolute.y, relative.x, relative.y);
			}

			void c_canvas_gwen::OnMouseButtonUpdate(const int button, bool value)
			{
				m_canvas->InputMouseButton(button, value);
			}

			void c_canvas_gwen::OnMouseWheelUpdate(const int value)
			{
				m_canvas->InputMouseWheel(value);
			}
#pragma endregion

#pragma region i_control_keyboard_handler
			void c_canvas_gwen::OnKeyboardCharacterPressed(const wchar_t character)
			{
				m_canvas->InputCharacter(character);
			}

			void c_canvas_gwen::OnKeyboardButtonUpdate(const Enums::key_code key, const bool value)
			{
				// Map special keys to gwens key enum values
				int gwen_key = -1;
				switch(key)
				{
					case Enums::_key_code_escape:
						gwen_key = Gwen::Key::Escape;
						break;
					case Enums::_key_code_shift:
					case Enums::_key_code_left_shift:
					case Enums::_key_code_right_shift:
						gwen_key = Gwen::Key::Shift;
						break;
					case Enums::_key_code_enter:
					case Enums::_key_code_num_enter:
						gwen_key = Gwen::Key::Return;
						break;
					case Enums::_key_code_backspace:
						gwen_key = Gwen::Key::Backspace;
						break;
					case Enums::_key_code_delete:
						gwen_key = Gwen::Key::Delete;
						break;
					case Enums::_key_code_tab:
						gwen_key = Gwen::Key::Tab;
						break;
					case Enums::_key_code_home:
						gwen_key = Gwen::Key::Home;
						break;
					case Enums::_key_code_end:
						gwen_key = Gwen::Key::End;
						break;
					case Enums::_key_code_ctrl:
					case Enums::_key_code_left_ctrl:
					case Enums::_key_code_right_ctrl:
						gwen_key = Gwen::Key::Control;
						break;
					case Enums::_key_code_alt:
					case Enums::_key_code_left_alt:
					case Enums::_key_code_right_alt:
						gwen_key = Gwen::Key::Alt;
						break;
					case Enums::_key_code_up:
						gwen_key = Gwen::Key::Up;
						break;
					case Enums::_key_code_down:
						gwen_key = Gwen::Key::Down;
						break;
					case Enums::_key_code_left:
						gwen_key = Gwen::Key::Left;
						break;
					case Enums::_key_code_right:
						gwen_key = Gwen::Key::Right;
						break;
				};

				if(gwen_key == -1)
				{
					return;
				}

				m_canvas->InputKey(gwen_key, value);
			}
#pragma endregion
	};};};
};
#endif