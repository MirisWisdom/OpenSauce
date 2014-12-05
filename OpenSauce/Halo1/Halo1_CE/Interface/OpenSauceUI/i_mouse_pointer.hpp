/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include "Interface/OpenSauceUI/i_canvas.hpp"
#include "Interface/OpenSauceUI/ControlFactory/c_control_factory.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI
	{
		/// <summary>	The OpenSauceUI mouse pointer interface. </summary>
		class i_mouse_pointer
			abstract
		{
		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Initializes the mouse pointer. </summary>
			///
			/// <param name="control_factory">	[in] The control factory to create the pointer with. </param>
			/// <param name="canvas">		  	[in] The target canvas. </param>
			virtual void Initialize(ControlFactory::c_control_factory& control_factory, i_canvas& canvas) = 0;

			/// <summary>	Releases the mouse pointer. </summary>
			virtual void Release() = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets the pointer's position. </summary>
			///
			/// <param name="x">	The x screen coordinate. </param>
			/// <param name="y">	The y screen coordinate. </param>
			virtual void SetPosition(const int x, const int y) = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the pointer's position. </summary>
			///
			/// <param name="x">	[out] The output x coordinate. </param>
			/// <param name="y">	[out] The output y coordinate. </param>
			virtual void GetPosition(int& x, int& y) = 0;
		};

		/// <summary>	Defines an alias representing a shared mouse pointer pointer. </summary>
		typedef std::shared_ptr<i_mouse_pointer> t_mouse_pointer_ptr;
	};};
};
#endif