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
		/// <summary>	A Gwen pointer control builder for OpenSauceUI. </summary>
		class c_control_builder_gwen_pointer
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
		};
	};};};};
};
#endif