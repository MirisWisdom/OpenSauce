/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include <Gwen/Controls/Base.h>

#include "Interface/OpenSauceUI/GwenUI/ControlBuilders/c_control_builder_gwen_textbox.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace GwenUI { namespace ControlBuilders
	{
		/// <summary>	A Gwen numeric textbox control builder for OpenSauceUI. </summary>
		class c_control_builder_gwen_textboxnumeric
			: public c_control_builder_gwen_textbox
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
		};
	};};};};
};
#endif