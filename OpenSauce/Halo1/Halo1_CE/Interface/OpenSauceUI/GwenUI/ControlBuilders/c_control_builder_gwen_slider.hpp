/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include <Gwen/Controls/Base.h>

#include "Interface/OpenSauceUI/GwenUI/ControlBuilders/c_control_builder_gwen_base.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace GwenUI { namespace ControlBuilders
	{
		/// <summary>	A Gwen slider control builder for OpenSauceUI. </summary>
		class c_control_builder_gwen_slider
			abstract
			: public c_control_builder_gwen_base
		{
		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets a controls default values. </summary>
			///
			/// <param name="control">	[in] The gwen control. </param>
			void SetDefaultValues(Gwen::Controls::Base* control) const override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the property interfaces for the control type. </summary>
			///
			/// <remarks>	Derived overrides of this function must call the base function as well. </remarks>
			///
			/// <param name="property_interfaces">	[out] The property interfaces. </param>
			void GetPropertyInterfaces(Control::property_interface_map_t& property_interfaces) const override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the event handlers for the control type. </summary>
			///
			/// <remarks>	Derived overrides of this function must call the base function as well. </remarks>
			///
			/// <param name="control">		 	[in] The control. </param>
			/// <param name="event_handlers">	[out] The event handlers. </param>
			void GetEventHandlers(Gwen::Controls::Base* control, Control::event_handler_map_t& event_handlers) const override;
		};
	};};};};
};
#endif