/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include "Interface/OpenSauceUI/Control/i_control.hpp"
#include "Interface/OpenSauceUI/Control/i_event_handler.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace Control
	{
		/// <summary>	An event handler. </summary>
		class c_event_handler
			abstract
			: public i_event_handler
		{
		private:
			struct s_event_callback
			{
				uint32 id;
				void* userdata;
				event_callback_t function;

				////////////////////////////////////////////////////////////////////////////////////////////////////
				/// <summary>	Assignment operator. </summary>
				///
				/// <param name="rhs">	The right hand side. </param>
				///
				/// <returns>	A shallow copy of this object. </returns>
				s_event_callback& operator=(const s_event_callback& rhs);

				////////////////////////////////////////////////////////////////////////////////////////////////////
				/// <summary>	Equality operator. </summary>
				///
				/// <param name="rhs">	The right hand side. </param>
				///
				/// <returns>	true if the parameters are considered equivalent. </returns>
				bool operator==(const s_event_callback& rhs) const;
			};

			std::vector<s_event_callback> m_callbacks;

		public:
			/// <summary>	Default constructor. </summary>
			c_event_handler();

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Adds a callback. </summary>
			///
			/// <param name="callback_id">	Identifier for the callback. </param>
			/// <param name="function">   	The function. </param>
			/// <param name="userdata">   	[in] If non-null, the userdata. </param>
			void AddCallback(const uint32 callback_id, const event_callback_t& function, void* userdata) final override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Removes the callback described by callback_id. </summary>
			///
			/// <param name="callback_id">	Identifier for the callback. </param>
			void RemoveCallback(const uint32 callback_id) final override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Triggers the event. </summary>
			///
			/// <param name="event_data">	Information from the event. </param>
			void Trigger(const s_interface_value& event_data) const final override;

		private:
			/// <summary>	Attaches the handler to the event. </summary>
			virtual void Attach() = 0;

			/// <summary>	Detaches the handler from the event. </summary>
			virtual void Detach() = 0;
		};
	};};};
};
#endif