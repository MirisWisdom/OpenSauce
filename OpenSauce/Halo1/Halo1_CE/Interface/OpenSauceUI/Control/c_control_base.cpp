/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI/Control/c_control_base.hpp"

#if !PLATFORM_IS_DEDI

#include "Interface/OpenSauceUI/resource_id.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace Control
	{
#pragma region c_event_handler
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

		void c_event_handler::AddCallback(const uint32 callback_id, const t_event_callback& function, void* userdata)
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
#pragma endregion

#pragma region c_property_interface
		c_property_interface::c_property_interface(const t_property_get_func& get
			, const t_property_set_func& set
			, const t_property_set_by_string_func& set_string)
			: m_get(get)
			, m_set(set)
			, m_set_string(set_string)
		{ }

		void c_property_interface::Set(i_control& control, const char* value)
		{
			if(m_set_string)
			{
				m_set_string(control, value);
			}
		}

		void c_property_interface::Set(i_control& control, const s_interface_value& value)
		{
			if(m_set)
			{
				m_set(control, value);
			}
		}

		s_interface_value c_property_interface::Get(i_control& control, const s_interface_value& default_value) const
		{
			if(m_get)
			{
				return m_get(control);
			}

			return default_value;
		}
#pragma endregion

#pragma region c_control_base
		c_control_base::c_control_base()
			: m_resource_id(RESOURCE_ID_NONE)
			, m_child_controls()
			, m_property_interfaces()
		{ }

		t_control_list::const_iterator c_control_base::FindControl(const t_control_ptr control) const
		{
			// Find the control using a pointer comparison
			return std::find_if(m_child_controls.begin(), m_child_controls.end(),
				[&](const t_control_ptr& entry)
				{
					return entry == control;
				}
			);
		}

		void c_control_base::AddPropertyInterface(const uint32 interface_id, i_property_interface* property_interface)
		{
			YELO_ASSERT_DISPLAY(m_property_interfaces.find(interface_id) == m_property_interfaces.end(), "Existing property interface ID");

			m_property_interfaces[interface_id] = property_interface;
		}

		i_property_interface* c_control_base::FindPropertyInterface(const uint32 interface_id) const
		{
			auto& found_interface = m_property_interfaces.find(interface_id);
			
			if(found_interface == m_property_interfaces.end())
			{
				return nullptr;
			}

			return found_interface->second;
		}

		void c_control_base::AddEventHandler(const uint32 event_id, const t_event_handler_ptr& event_handler)
		{
			YELO_ASSERT_DISPLAY(m_event_handlers.find(event_id) == m_event_handlers.end(), "Attempted to add duplicate event handler to a control");

			m_event_handlers[event_id] = event_handler;
		}

#pragma region i_control
		uint32 c_control_base::GetResourceID() const
		{
			return m_resource_id;
		}

		void c_control_base::SetResourceID(const uint32 resource_id)
		{
			m_resource_id = resource_id;
		}

		void c_control_base::AddControl(t_control_ptr control)
		{
			// Add the control to the list if it does not already exist
			auto found_entry = FindControl(control);

			YELO_ASSERT_DISPLAY(found_entry == m_child_controls.end(), "Attempted to add a control instance to the control multiple times");

			if(found_entry == m_child_controls.end())
			{
				m_child_controls.push_back(control);
			}
		}

		void c_control_base::RemoveControl(t_control_ptr control)
		{
			// Remove the control from the list if it exists
			auto found_entry = FindControl(control);

			YELO_ASSERT_DISPLAY(found_entry != m_child_controls.end(), "Attempted to remove a non-existant control from a control");

			if(found_entry != m_child_controls.end())
			{
				m_child_controls.erase(found_entry);
			}
		}

		t_control_list& c_control_base::Controls()
		{
			return m_child_controls;
		}

		i_property_interface* c_control_base::GetPropertyInterface(const uint32 interface_id) const
		{
			auto property_interface = FindPropertyInterface(interface_id);

			YELO_ASSERT_DISPLAY(property_interface != nullptr, "Failed to find a property interface in a control");

			return property_interface;
		}

		void c_control_base::AddEventCallback(const uint32 event_id, const uint32 callback_id, const t_event_callback& function, void* userdata = nullptr)
		{
			YELO_ASSERT_DISPLAY(m_event_handlers.find(event_id) != m_event_handlers.end(), "Attempted to add an event callback to a non-existant handler");

			m_event_handlers[event_id]->AddCallback(callback_id, function, userdata);
		}

		void c_control_base::RemoveEventCallback(const uint32 event_id, const uint32 callback_id)
		{
			YELO_ASSERT_DISPLAY(m_event_handlers.find(event_id) != m_event_handlers.end(), "Attempted to remove an event callback from a non-existant handler");

			m_event_handlers[event_id]->RemoveCallback(callback_id);
		}
#pragma endregion
#pragma endregion
	};};};
};
#endif