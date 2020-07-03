/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI/Control/c_event_handler.hpp"

#if !PLATFORM_IS_DEDI

#include "Interface/OpenSauceUI/resource_id.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace Control
	{
		c_event_handler::s_event_callback& c_event_handler::s_event_callback::operator=(const s_event_callback& rhs)
		{
			id = rhs.id;
			userdata = rhs.userdata;
			function = rhs.function;

			return *this;
		}

		bool c_event_handler::s_event_callback::operator==(const s_event_callback& rhs) const
		{
			return id == rhs.id;
		}

		c_event_handler::c_event_handler()
			: m_callbacks()
		{ }

		void c_event_handler::AddCallback(const uint32 callback_id, const event_callback_t& function, void* userdata)
		{
			// Check that the callback has not already been added
			auto entry = std::find_if(m_callbacks.begin(), m_callbacks.end(),
				[&](const s_event_callback& entry)
				{
					return entry.id == callback_id;
				}
			);

			YELO_ASSERT_DISPLAY(entry == m_callbacks.end(), "Attempted to add a duplicate callback to an event handler");

			// Add the callback to the list
			if(entry == m_callbacks.end())
			{
				m_callbacks.push_back(s_event_callback { callback_id, userdata, function });

				// If this is the first callback to be added, attach to the controls handler
				if(m_callbacks.size() == 1)
				{
					Attach();
				}
			}
		}

		void c_event_handler::RemoveCallback(const uint32 callback_id)
		{
			// Check that the callback exists
			auto entry = std::find_if(m_callbacks.begin(), m_callbacks.end(),
				[&](const s_event_callback& entry)
				{
					return entry.id == callback_id;
				}
			);

			YELO_ASSERT_DISPLAY(entry != m_callbacks.end(), "Attempted to remove a non existant callback from an event handler");
			
			// Remove the callback from the list
			if(entry != m_callbacks.end())
			{
				m_callbacks.erase(entry);

				// If no callbacks are left, detach from the control's handler
				if(m_callbacks.size() == 0)
				{
					Detach();
				}
			}
		}

		void c_event_handler::Trigger(const s_interface_value& event_data) const
		{
			// Call all of the callbacks
			auto callbacks = m_callbacks;

			for(auto& entry : callbacks)
			{
				entry.function(event_data, entry.userdata);
			}
		}
	};};};
};
#endif