/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include <Gwen/Controls/Base.h>

#include "Interface/OpenSauceUI/GwenUI/ControlBuilders/c_control_builder_gwen_slider.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace GwenUI { namespace ControlBuilders
	{
		/// <summary>	A Gwen vertical slider control builder for OpenSauceUI. </summary>
		class c_control_builder_gwen_verticalslider
			: public c_control_builder_gwen_slider
		{
		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Creates an instance of a control. </summary>
			///
			/// <returns>	null if it fails, else the new instance. </returns>
			Gwen::Controls::Base* CreateInstance(Gwen::Controls::Base* parent) const override;
		};
	};};};};
};
#endif