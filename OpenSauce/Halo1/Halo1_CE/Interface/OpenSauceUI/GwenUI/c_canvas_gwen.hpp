/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include <Gwen/Gwen.h>
#include <Gwen/Skins/TexturedBase.h>

#include "Interface/OpenSauceUI/Control/i_canvas.hpp"
#include "Interface/OpenSauceUI/Input/i_control_input.hpp"
#include "Interface/OpenSauceUI/GwenUI/c_gwen_renderer_halo.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace GwenUI
	{
		/// <summary>	The canvas for rendering the UI with Gwen. </summary>
		class c_canvas_gwen final
			: public Control::i_canvas
			, public Input::i_control_mouse_handler
			, public Input::i_control_keyboard_handler
		{
			std::unique_ptr<c_gwen_renderer_halo>		m_renderer;
			std::unique_ptr<Gwen::Skin::TexturedBase>	m_skin;
			std::unique_ptr<Gwen::Controls::Canvas>		m_canvas;

			Control::control_list_t m_child_controls;

		private:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Searches for a child control. </summary>
			///
			/// <param name="control">	The control to find. </param>
			///
			/// <returns>	Iterator for the found child control. </returns>
			Control::control_list_t::iterator FindChildControl(Control::control_ptr_t control);

#pragma region i_canvas
		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Initializes the Gwen DX9 renderer and skin. </summary>
			///
			/// <param name="device">			[in] The current render device. </param>
			/// <param name="ui_package">   	[in] The ui package. </param>
			/// <param name="control_input">	The control input. </param>
			void Initialize(IDirect3DDevice9* device, c_packed_file& ui_package, Input::i_control_input& control_input) override;

			/// <summary>	Releases the Gwen DX9 renderer. </summary>
			void Release(Input::i_control_input& control_input) override;

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
		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the controls parent. </summary>
			///
			/// <returns>	The controls parent. </returns>
			Control::i_control* Parent() override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the resource identifier of the control. </summary>
			///
			/// <returns>	The controls resource identifier. </returns>
			uint32 GetResourceID() const override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the controls bounds. </summary>
			///
			/// <returns>	The controls bounds. </returns>
			rectangle2d GetBounds() const override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Not applicable for a canvas. </summary>
			void SetResourceID(const uint32 resource_id) override { }

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
			void AddControl(Control::control_ptr_t control) override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Removes the specified control from the child control list. </summary>
			///
			/// <remarks>	If the specified control does not exist, the action is ignored. </remarks>
			///
			/// <param name="control">	The control to remove. </param>
			void RemoveControl(Control::control_ptr_t control) override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the controls list. </summary>
			///
			/// <returns>	A list of controls. </returns>
			Control::control_list_t& Controls() override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Not applicable for a canvas. </summary>
			Control::i_property_interface* GetPropertyInterface(const uint32 interface_id) const override { return nullptr; };
			
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Not applicable for a canvas. </summary>
			void AddEventCallback(const uint32 event_id, const uint32 callback_id, const Control::event_callback_t& function, void* userdata) override { }
			
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Not applicable for a canvas. </summary>
			void RemoveEventCallback(const uint32 event_id, const uint32 callback_id) override { }
#pragma endregion

#pragma region i_visibility_toggle
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Not applicable for a canvas. </summary>
			void Show() { }

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Not applicable for a canvas. </summary>
			void Hide() { }
#pragma endregion

#pragma region i_control_mouse_handler
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Executes the mouse position update action. </summary>
			///
			/// <param name="absolute">	The absolute position. </param>
			/// <param name="relative">	The relative position. </param>
			void OnMousePositionUpdate(const point2d& absolute, const point2d& relative) override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Executes the mouse button update action. </summary>
			///
			/// <param name="button">	The button. </param>
			/// <param name="value"> 	The button state. </param>
			void OnMouseButtonUpdate(const int button, const bool value) override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Executes the mouse wheel update action. </summary>
			///
			/// <param name="value">	The wheel value. </param>
			void OnMouseWheelUpdate(const int value) override;
#pragma endregion

#pragma region i_control_keyboard_handler
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Executes the keyboard character pressed action. </summary>
			///
			/// <param name="character">	The pressed character. </param>
			void OnKeyboardCharacterPressed(const wchar_t character) override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Executes the keyboard button update action. </summary>
			///
			/// <param name="key">  	The key. </param>
			/// <param name="value">	The button state. </param>
			void OnKeyboardButtonUpdate(const Enums::key_code key, const bool value) override;
#pragma endregion
		};
	};};};
};
#endif