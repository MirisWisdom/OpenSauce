/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include <Gwen/Controls/Base.h>

#include "Interface/OpenSauceUI/Control/i_control.hpp"
#include "Interface/OpenSauceUI/Control/c_control_base.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace GwenUI
	{
		class c_event_handler_gwen;

		/// <summary>	Defines an alias representing an event data transform function. </summary>
		typedef std::function<void(Gwen::Event::Info&, Control::s_interface_value&)> event_data_transform_t;

		/// <summary>	An event handler for gwen controls. </summary>
		class c_event_handler_gwen_handler final
			: public Gwen::Event::Handler
		{
		private:
			const c_event_handler_gwen& m_owner;
			Gwen::Event::Caller& m_caller;
			bool is_attached;
			const event_data_transform_t m_data_transform;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Callback for attaching to a gwen control event. </summary>
			///
			/// <param name="info">	[in] The event information. </param>
			void Callback(Gwen::Event::Info& info);

		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Constructor. </summary>
			///
			/// <param name="owner">		 	[in] The handler owner. </param>
			/// <param name="caller">		 	[in] The caller to attach to. </param>
			/// <param name="data_transform">	The data transform function. </param>
			c_event_handler_gwen_handler( const c_event_handler_gwen& owner
				, Gwen::Event::Caller& caller
				, const event_data_transform_t& data_transform);

			/// <summary>	Destructor. </summary>
			~c_event_handler_gwen_handler();

			/// <summary>	Attaches the handler to the gwen event caller. </summary>
			void Attach();

			/// <summary>	Detaches the handler from the gwen event caller. </summary>
			void Detach();
		};

		/// <summary>	An event handler for gwen. </summary>
		class c_event_handler_gwen final
			: public Control::c_event_handler
		{
		private:
			c_event_handler_gwen_handler m_handler_thunk;

		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Constructor. </summary>
			///
			/// <param name="caller">		 	[in] The caller to attach to. </param>
			/// <param name="data_transform">	(Optional) the data transform function. </param>
			c_event_handler_gwen(Gwen::Event::Caller& caller, const event_data_transform_t& data_transform = nullptr);

		private:
			/// <summary>	Attaches the handler to the event. </summary>
			void Attach() override;

			/// <summary>	Detaches the handler from the event. </summary>
			void Detach() override;
		};

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