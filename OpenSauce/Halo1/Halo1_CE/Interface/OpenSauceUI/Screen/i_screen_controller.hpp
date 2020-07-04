/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include "Interface/OpenSauceUI/i_visibility_toggle.hpp"
#include "Interface/OpenSauceUI/Control/i_canvas.hpp"
#include "Interface/OpenSauceUI/ControlFactory/c_control_factory.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace Screen
	{
		/// <summary>	A controller for handling screens. </summary>
		class i_screen_controller
			abstract
			: public i_visibility_toggle
		{
		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Builds the controller's screen. </summary>
			///
			/// <param name="control_factory">	[in,out] The control factory. </param>
			/// <param name="target_canvas">  	[in,out] Target canvas. </param>
			virtual void BuildScreen(ControlFactory::c_control_factory& control_factory, Control::i_canvas& target_canvas) = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Destroys the controller's screen. </summary>
			///
			/// <param name="target_canvas">	[in] Target canvas. </param>
			virtual void DestroyScreen(Control::i_canvas& target_canvas) = 0;

			/// <summary>	Updates the controller. </summary>
			virtual void Update() = 0;

		protected:
			/// <summary>	Sets a screens static properties. </summary>
			virtual void SetStaticProperties() = 0;

			/// <summary>	Binds a screens dynamic properties. </summary>
			virtual void BindDynamicProperties() = 0;

			/// <summary>	Unbinds a screens dynamic properties. </summary>
			virtual void UnbindDynamicProperties() = 0;

			/// <summary>	Binds a screens events. </summary>
			virtual void BindEvents() = 0;

			/// <summary>	Unbinds a screens events. </summary>
			virtual void UnbindEvents() = 0;
		};

		/// <summary>	Defines an alias representing screen controller shared pointer. </summary>
		typedef std::shared_ptr<i_screen_controller> screen_controller_ptr_t;
	};};};
};
#endif