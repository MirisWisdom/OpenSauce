/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include "Interface/OpenSauceUI/Control/i_control.hpp"
#include "Interface/OpenSauceUI/ControlFactory/i_control_builder.hpp"
#include "Interface/OpenSauceUI/ControlFactory/c_control_factory.hpp"
#include "Interface/OpenSauceUI/Definitions/c_control_definition.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace ControlFactory
	{
		/// <summary>	The control builder base class. </summary>
		class c_control_builder_base
			abstract
			: public i_control_builder
		{
		protected:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Builds a list of controls and adds them to a parent control. </summary>
			///
			/// <param name="control_registry">   	[in] The control registry. </param>
			/// <param name="control_definitions">	[in] The control definitions. </param>
			/// <param name="parent">			  	[in] The parent control. </param>
			void BuildChildren(c_control_factory& control_registry
				, Definitions::control_definition_list_t& control_definitions
				, Control::i_control& parent) const;
		};
	};};};
};
#endif