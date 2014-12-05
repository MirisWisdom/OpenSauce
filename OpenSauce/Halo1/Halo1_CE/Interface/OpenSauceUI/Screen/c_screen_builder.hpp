/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include "Interface/OpenSauceUI/i_canvas.hpp"
#include "Interface/OpenSauceUI/Screen/c_screen.hpp"
#include "Interface/OpenSauceUI/Definitions/c_screen_definition.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace Screen
	{
		/// <summary>	The screen builder class. </summary>
		class c_screen_builder
		{
		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Builds a screen from a screen definition. </summary>
			///
			/// <param name="control_factory">  	[in] The control factory. </param>
			/// <param name="screen_definition">	[in] The screen definition. </param>
			/// <param name="target_canvas">		[in] Target canvas. </param>
			///
			/// <returns>	A new screen pointer. </returns>
			static t_screen_ptr BuildScreen(ControlFactory::c_control_factory& control_factory
				, Definitions::c_screen_definition& screen_definition
				, i_canvas& target_canvas)
			{
				// Build the screen's root control
				auto root_ptr = control_factory.BuildControl(target_canvas, screen_definition.m_control_definition);

				return std::make_shared<c_screen>(root_ptr);
			}
		};
	};};};
};
#endif