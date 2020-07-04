/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include <Gwen/Controls/Base.h>

#include "Interface/OpenSauceUI/resource_id.hpp"
#include "Interface/OpenSauceUI/Control/i_control.hpp"
#include "Interface/OpenSauceUI/ControlFactory/c_control_builder_base.hpp"
#include "Interface/OpenSauceUI/ControlFactory/c_control_factory.hpp"
#include "Interface/OpenSauceUI/Definitions/c_control_definition.hpp"
#include "Interface/OpenSauceUI/GwenUI/c_control_gwen.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace GwenUI { namespace ControlBuilders
	{
#define GWEN_CTRL_PTR(type, ptr) (CAST_PTR(Gwen::Controls::##type##*, ptr##))
#define GWEN_CTRL_REF(type, ptr) (*CAST_PTR(Gwen::Controls::##type##*, ptr##))

		/// <summary>	A Gwen control builder for OpenSauceUI. </summary>
		class c_control_builder_gwen
			abstract
			: public ControlFactory::c_control_builder_base
		{
		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Creates an instance of a control. </summary>
			///
			/// <returns>	null if it fails, else the new instance. </returns>
			virtual Gwen::Controls::Base* CreateInstance(Gwen::Controls::Base* parent) const = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets a controls default values. </summary>
			///
			/// <param name="control">	[in] The gwen control. </param>
			virtual void SetDefaultValues(Gwen::Controls::Base* control) const = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the property interfaces for the control type. </summary>
			///
			/// <remarks>	Derived overrides of this function must call the base function as well. </remarks>
			///
			/// <param name="property_interfaces">	[out] The property interfaces. </param>
			virtual void GetPropertyInterfaces(Control::property_interface_map_t& property_interfaces) const = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the event handlers for the control type. </summary>
			///
			/// <remarks>	Derived overrides of this function must call the base function as well. </remarks>
			///
			/// <param name="control">		 	[in] The control. </param>
			/// <param name="event_handlers">	[out] The event handlers. </param>
			virtual void GetEventHandlers(Gwen::Controls::Base* control, Control::event_handler_map_t& event_handlers) const = 0;

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