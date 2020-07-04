/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include "Interface/OpenSauceUI/Control/i_control.hpp"
#include "Interface/OpenSauceUI/Control/s_interface_value.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace Control
	{
		/// <summary>	Defines an alias for an event callback function. </summary>
		typedef std::function<void(const s_interface_value&, void*)> event_callback_t;

		/// <summary>	An event handler interface. </summary>
		class i_event_handler
			abstract
		{
		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Adds a callback. </summary>
			///
			/// <param name="callback_id">	Identifier for the callback. </param>
			/// <param name="function">   	The function. </param>
			/// <param name="userdata">   	[in] If non-null, the userdata. </param>
			virtual void AddCallback(const uint32 callback_id, const event_callback_t& function, void* userdata) = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Removes the callback described by callback_id. </summary>
			///
			/// <param name="callback_id">	Identifier for the callback. </param>
			virtual void RemoveCallback(const uint32 callback_id) = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Triggers the event. </summary>
			///
			/// <param name="event_data">	Information from the event. </param>
			virtual void Trigger(const s_interface_value& event_data) const = 0;
		};

		/// <summary>	Defines an alias representing event handler shared pointer. </summary>
		typedef std::shared_ptr<i_event_handler> event_handler_ptr_t;

		/// <summary>	Defines an alias representing an event_handler map. </summary>
		typedef std::map<uint32, event_handler_ptr_t> event_handler_map_t;
	};};};
};
#endif