/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include "Interface/OpenSauceUI/i_control.hpp"
#include "Interface/OpenSauceUI/ControlFactory/c_control_builder_base.hpp"
#include "Interface/OpenSauceUI/ControlFactory/c_control_factory.hpp"
#include "Interface/OpenSauceUI/Definitions/c_control_definition.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace GwenUI
	{
		/// <summary>	A Gwen control builder for OpenSauceUI. </summary>
		class c_control_builder_gwen final
			: public ControlFactory::c_control_builder_base
		{
		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Builds a Gwen control from the specified definition. </summary>
			///
			/// <param name="control_registry">  	[in] The control registry. </param>
			/// <param name="control_definition">	[in] The control definition. </param>
			/// <param name="parent">			 	[in] The parent. </param>
			///
			/// <returns>	A shared control pointer. </returns>
			t_control_ptr Build(ControlFactory::c_control_factory& control_registry
				, Definitions::c_control_definition& control_definition
				, i_control& parent) const override;
		};
	};};};
};
#endif