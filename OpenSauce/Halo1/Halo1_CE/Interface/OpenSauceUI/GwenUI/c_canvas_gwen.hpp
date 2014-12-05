/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include <Gwen/Gwen.h>
#include <Gwen/Skins/TexturedBase.h>

#include "Interface/OpenSauceUI/i_canvas.hpp"
#include "Interface/OpenSauceUI/GwenUI/c_gwen_renderer_halo.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace GwenUI
	{
		/// <summary>	The canvas for rendering the UI with Gwen. </summary>
		class c_canvas_gwen final
			: public i_canvas
		{
			std::unique_ptr<c_gwen_renderer_halo>	m_renderer;
			std::unique_ptr<Gwen::Skin::TexturedBase>	m_skin;
			std::unique_ptr<Gwen::Controls::Canvas>		m_canvas;

			t_control_list m_child_controls;

		public:
#pragma region i_canvas
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Initializes the Gwen DX9 renderer and skin. </summary>
			///
			/// <param name="device">	[in] The current render device. </param>
			void Initialize(IDirect3DDevice9* device) override;

			/// <summary>	Releases the Gwen DX9 renderer. </summary>
			void Release() override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets the size of the canvas. </summary>
			///
			/// <param name="width"> 	The width of the canvas. </param>
			/// <param name="height">	The height of the canvas. </param>
			void SetSize(const int32 width, const int32 height) override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets the scale of the canvas. </summary>
			///
			/// <param name="scale">	The scale of the canvas. </param>
			void SetScale(const real scale) override;

			/// <summary>	Draws the canvas. </summary>
			void Draw() const override;
#pragma endregion

#pragma region i_control
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the resource identifier of the control. </summary>
			///
			/// <returns>	The controls resource identifier. </returns>
			virtual uint32 GetResourceID() const override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets the resource identifier of the control. </summary>
			///
			/// <param name="resource_id">	Identifier for the control resource. </param>
			void SetResourceID(const uint32 resource_id) final override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the Gwen base control pointer. </summary>
			///
			/// <returns>	The Gwen base control pointer. </returns>
			void* GetControlPtr() const override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Adds a control to the child control list. </summary>
			///
			/// <remarks>	If the specified control has already been added the action is ignored. </remarks>
			///
			/// <param name="control">	The control to add. </param>
			void AddControl(t_control_ptr control) override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Removes the specified control from the child control list. </summary>
			///
			/// <remarks>	If the specified control does not exist, the action is ignored. </remarks>
			///
			/// <param name="control">	The control to remove. </param>
			void RemoveControl(t_control_ptr control) override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the controls list. </summary>
			///
			/// <returns>	A list of controls. </returns>
			t_control_list& Controls() override;
#pragma endregion
		};
	};};};
};
#endif