/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include "Interface/OpenSauceUI/i_visibility_toggle.hpp"
#include "Interface/OpenSauceUI/Input/i_control_input.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace Control
	{
		/// <summary>	The OpenSauceUI mouse pointer interface. </summary>
		class i_mouse_pointer
			abstract
			: public i_visibility_toggle
		{
		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Builds the mouse. </summary>
			///
			/// <param name="control_input">	[in] The control input manager. </param>
			virtual void BuildMouse(Input::i_control_input& control_input) = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Destroys the mouse. </summary>
			///
			/// <param name="control_input">	[in] The control input manager. </param>
			virtual void DestroyMouse(Input::i_control_input& control_input) = 0;

#pragma region i_visibility_toggle
			/// <summary>	Shows the object. </summary>
			virtual void Show() = 0;

			/// <summary>	Hides the object. </summary>
			virtual void Hide() = 0;
#pragma endregion
		};

		/// <summary>	Defines an alias representing a shared mouse pointer pointer. </summary>
		typedef std::shared_ptr<i_mouse_pointer> mouse_pointer_ptr_t;
	};};};
};
#endif