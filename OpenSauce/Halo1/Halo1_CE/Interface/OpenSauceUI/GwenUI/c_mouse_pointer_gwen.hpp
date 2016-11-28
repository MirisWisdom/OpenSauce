/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include "Interface/OpenSauceUI/Control/i_mouse_pointer.hpp"
#include "Interface/OpenSauceUI/Control/i_property_interface.hpp"
#include "Interface/OpenSauceUI/Control/i_control.hpp"
#include "Interface/OpenSauceUI/Control/i_canvas.hpp"
#include "Interface/OpenSauceUI/ControlFactory/c_control_factory.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace GwenUI
	{
		/// <summary>	A mouse pointer using gwen. </summary>
		class c_mouse_pointer_gwen final
			: public Control::i_mouse_pointer
			, public Input::i_control_mouse_handler
		{
			ControlFactory::c_control_factory& m_control_factory;
			Control::i_canvas& m_canvas;
			Control::control_ptr_t m_mouse_control;
			Control::i_property_interface* m_position_property;

		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Builds the mouse. </summary>
			///
			/// <param name="control_input">	[in] The control input manager. </param>
			void BuildMouse(Input::i_control_input& control_input) override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Destroys the mouse. </summary>
			///
			/// <param name="control_input">	[in] The control input manager. </param>
			void DestroyMouse(Input::i_control_input& control_input) override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Constructor. </summary>
			///
			/// <param name="control_factory">	[in] The control factory to create the pointer with. </param>
			/// <param name="canvas">		  	[in] The target canvas. </param>
			c_mouse_pointer_gwen(ControlFactory::c_control_factory& control_factory, Control::i_canvas& canvas);

			/// <summary>	Shows the object. </summary>
			void Show() override;

			/// <summary>	Hides the object. </summary>
			void Hide() override;

#pragma region i_control_mouse_handler
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Executes the mouse position update action. </summary>
			///
			/// <param name="absolute">	The absolute position. </param>
			/// <param name="relative">	The relative position. </param>
			void OnMousePositionUpdate(const point2d& absolute, const point2d& relative) override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Not used. </summary>
			///
			/// <param name="button">	The button. </param>
			/// <param name="value"> 	The value. </param>
			void OnMouseButtonUpdate(const int button, const bool value) { }

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Not used. </summary>
			///
			/// <param name="value">	The value. </param>
			void OnMouseWheelUpdate(const int value) { }
#pragma endregion
		};
	};};};
};
#endif