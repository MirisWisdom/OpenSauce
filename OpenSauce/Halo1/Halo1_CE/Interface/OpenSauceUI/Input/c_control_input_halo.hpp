/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include <Gwen/Controls/Canvas.h>

#include "Interface/Controls.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace Input
	{
		/// <summary>	The control input class for the halo runtime. </summary>
		class c_control_input_halo final
		{
			Gwen::Controls::Canvas*	m_canvas;

			struct
			{
				Yelo::rectangle2d bounds;
				Yelo::point2d position;
				bool mouse_button_states[3];
				PAD8;
			}m_mouse;

			struct
			{
				bool key_states[Yelo::Enums::_Key];
			}m_keyboard;
		
		public:
			/// <summary>	Default constructor. </summary>
			c_control_input_halo()
				: m_canvas(nullptr)
			{
				ZeroMemory(&m_mouse, sizeof(m_mouse));
				ZeroMemory(&m_keyboard, sizeof(m_keyboard));
			}

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Initializes this object. </summary>
			///
			/// <param name="canvas">	[in] The gwen canvas to use. </param>
			void Initialize(Gwen::Controls::Canvas* canvas);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets the extents of the mouses movement. </summary>
			///
			/// <param name="min_x">	The minimum x coordinate. </param>
			/// <param name="max_x">	The maximum x coordinate. </param>
			/// <param name="min_y">	The minimum y coordinate. </param>
			/// <param name="max_y">	The maximum y coordinate. </param>
			void SetMouseBounds(const int min_x, const int max_x, const int min_y, const int max_y);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets the mouse's position. </summary>
			///
			/// <param name="pos_x">	The position x coordinate. </param>
			/// <param name="pos_y">	The position y coordinate. </param>
			void SetMousePosition(const int pos_x, const int pos_y);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the mouse's position. </summary>
			///
			/// <param name="pos_x">	[out] The position x coordinate. </param>
			/// <param name="pos_y">	[out] The position y coordinate. </param>
			void GetMousePosition(int& pos_x, int& pos_y);

			/// <summary>	Updates the ui input. </summary>
			void Update();
		};
	};};};
};
#endif