/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include "Interface/OpenSauceUI/i_visibility_toggle.hpp"
#include "Interface/OpenSauceUI/Control/i_control.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace Screen
	{
		/// <summary>	The OpenSauceUI screen interface. </summary>
		class i_screen
			abstract
			: public i_visibility_toggle
		{
		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the root control of the screen. </summary>
			///
			/// <returns>	The root control of the screen. </returns>
			virtual Control::control_ptr_t GetRootControl() const = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets a control based on it's resource id. </summary>
			///
			/// <param name="resource_id">	Identifier for the control resource. </param>
			///
			/// <returns>	The found control or nullptr if not found. </returns>
			virtual Control::control_ptr_t GetControl(const uint32 resource_id) = 0;
		};

		/// <summary>	Defines an alias representing a shared screen pointer. </summary>
		typedef std::shared_ptr<i_screen> screen_ptr_t;
	};};};
};
#endif