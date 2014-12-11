/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include "Interface/OpenSauceUI/Screen/i_screen_controller.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace Screen
	{
		/// <summary>	Manager for screen visibility. </summary>
		class c_screen_display_manager
		{
		private:
			struct s_screen_controller_state
			{
				bool visible;
				PAD24;
				t_screen_controller_ptr controller;
			};

			std::map<uint32, s_screen_controller_state> m_screen_controllers;

		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Adds a screen controller. </summary>
			///
			/// <param name="screen_id"> 	Identifier for the screen. </param>
			/// <param name="controller">	The controller. </param>
			void AddScreenController(const uint32 screen_id, t_screen_controller_ptr controller);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Removes the screen controller described by screen_id. </summary>
			///
			/// <param name="screen_id">	Identifier for the screen. </param>
			void RemoveScreenController(const uint32 screen_id);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Shows a screen. </summary>
			///
			/// <param name="screen_id">	Identifier for the screen. </param>
			void ShowScreen(const uint32 screen_id);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Hides a screen. </summary>
			///
			/// <param name="screen_id">	Identifier for the screen. </param>
			void HideScreen(const uint32 screen_id);
		};
	};};};
};
#endif