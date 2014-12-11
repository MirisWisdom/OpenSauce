/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include "Interface/OpenSauceUI/Control/i_mouse_pointer.hpp"
#include "Interface/OpenSauceUI/Control/i_control.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace GwenUI
	{
		/// <summary>	A mouse pointer using gwen. </summary>
		class c_mouse_pointer_gwen final
			: public Control::i_mouse_pointer
		{
			Control::t_control_ptr m_mouse_control;

		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Initializes the mouse pointer. </summary>
			///
			/// <param name="control_factory">	[in] The control factory to create the pointer with. </param>
			/// <param name="canvas">		  	[in] The target canvas. </param>
			void Initialize(ControlFactory::c_control_factory& control_factory, Control::i_canvas& canvas) override;

			/// <summary>	Releases the mouse pointer. </summary>
			void Release() override;
			
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets the pointer's position. </summary>
			///
			/// <param name="x">	The x screen coordinate. </param>
			/// <param name="y">	The y screen coordinate. </param>
			void SetPosition(const int x, const int y) override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the pointer's position. </summary>
			///
			/// <param name="x">	[out] The output x coordinate. </param>
			/// <param name="y">	[out] The output y coordinate. </param>
			void GetPosition(int& x, int& y) override;

			/// <summary>	Brings the pointer control to the front. </summary>
			void InterfaceChanged() override;
		};
	};};};
};
#endif