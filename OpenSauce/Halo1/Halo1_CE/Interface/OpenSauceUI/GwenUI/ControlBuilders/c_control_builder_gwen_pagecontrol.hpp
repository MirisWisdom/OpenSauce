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
		/// <summary>	A Gwen page control builder for OpenSauceUI. </summary>
		class c_control_builder_gwen_pagecontrol
			: public c_control_builder_gwen_base
		{
		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Creates an instance of a control. </summary>
			///
			/// <returns>	null if it fails, else the new instance. </returns>
			Gwen::Controls::Base* CreateInstance(Gwen::Controls::Base* parent) const override;

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

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Builds a Gwen control from the specified definition. </summary>
			///
			/// <param name="control_registry">  	[in] The control registry. </param>
			/// <param name="control_definition">	[in] The control definition. </param>
			/// <param name="parent">			 	[in] The parent. </param>
			///
			/// <returns>	A shared control pointer. </returns>
			Control::control_ptr_t Build(ControlFactory::c_control_factory& control_registry
				, Definitions::c_control_definition& control_definition
				, Control::i_control& parent) const override;
		};
	};};};};
};
#endif