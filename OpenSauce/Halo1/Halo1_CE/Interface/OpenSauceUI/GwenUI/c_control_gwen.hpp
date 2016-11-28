/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include <Gwen/Controls/Base.h>

#include "Interface/OpenSauceUI/Control/i_control.hpp"
#include "Interface/OpenSauceUI/Control/i_property_interface.hpp"
#include "Interface/OpenSauceUI/Control/i_event_handler.hpp"
#include "Interface/OpenSauceUI/Control/c_control_base.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace GwenUI
	{
		/// <summary>	An OpenSauceUI control using Gwen. </summary>
		class c_control_gwen final
			: public Control::c_control_base
		{
		private:
			Gwen::Controls::Base* m_control;

		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Constructor. </summary>
			///
			/// <param name="parent">			  	[in] The parent control. </param>
			/// <param name="gwen_control">		  	[in] The Gwen control. </param>
			/// <param name="property_interfaces">	[in] The control's property interfaces. </param>
			/// <param name="event_handlers">	  	[in] The control's event handlers. </param>
			c_control_gwen(Control::i_control& parent
				, Gwen::Controls::Base* gwen_control
				, const Control::property_interface_map_t& property_interfaces
				, const Control::event_handler_map_t& event_handlers);
			
#pragma region i_control
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the controls bounds. </summary>
			///
			/// <returns>	The controls bounds. </returns>
			rectangle2d GetBounds() const override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the Gwen base control pointer. </summary>
			///
			/// <returns>	The Gwen base control pointer. </returns>
			void* GetControlPtr() const override;

			/// <summary>	Shows the control. </summary>
			void Show() override;

			/// <summary>	Hides the control. </summary>
			void Hide() override;
#pragma endregion
		};
	};};};
};
#endif