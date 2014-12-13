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
#pragma region i_canvas
			void c_canvas_gwen::Initialize(IDirect3DDevice9* device)
			{
				m_renderer = std::make_unique<c_gwen_renderer_halo>(device);
				m_skin = std::make_unique<Gwen::Skin::TexturedBase>(nullptr);
				m_canvas = std::make_unique<Gwen::Controls::Canvas>(m_skin.get());

				m_canvas->SetDrawBackground(false);
				m_skin->SetRender(m_renderer.get());
				m_skin->Init( "OpenSauceUISkin" );
			}

			void c_canvas_gwen::Release()
			{
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
			uint32 c_canvas_gwen::GetResourceID() const
			{
				return RESOURCE_ID_DEBUG("#CNV_main_canvas");
			}

			void* c_canvas_gwen::GetControlPtr() const
			{
				return m_canvas.get();
			}

			void c_canvas_gwen::AddControl(Control::t_control_ptr control)
			{
				//Check whether the control already exists
				auto existing_control = std::find_if(m_child_controls.begin(), m_child_controls.end(),
					[&](Control::t_control_ptr entry)
					{
						return entry == control;
					});

				YELO_ASSERT_DISPLAY(existing_control == m_child_controls.end(), "Attempted to add a control instance to the canvas multiple times");

				if(existing_control == m_child_controls.end())
				{
					m_child_controls.push_back(control);
				}
			}

			void c_canvas_gwen::RemoveControl(Control::t_control_ptr control)
			{
				//Find the control to remove
				auto existing_control = std::find_if(m_child_controls.begin(), m_child_controls.end(),
					[&](Control::t_control_ptr entry)
					{
						return entry == control;
					});

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

			Control::t_control_list& c_canvas_gwen::Controls()
			{
				return m_child_controls;
			}
#pragma endregion
	};};};
};
#endif