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
		c_control_base::c_control_base(i_control& parent)
			: m_parent(parent)
			, m_resource_id(RESOURCE_ID_NONE)
			, m_child_controls()
			, m_property_interfaces()
			, m_event_handlers()
		{ }

		control_list_t::const_iterator c_control_base::FindControl(const control_ptr_t control) const
		{
			// Find the control using a pointer comparison
			return std::find_if(m_child_controls.begin(), m_child_controls.end(),
				[&](const control_ptr_t& entry)
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

		void c_control_base::AddEventHandler(const uint32 event_id, const event_handler_ptr_t& event_handler)
		{
			YELO_ASSERT_DISPLAY(m_event_handlers.find(event_id) == m_event_handlers.end(), "Attempted to add duplicate event handler to a control");

			m_event_handlers[event_id] = event_handler;
		}

#pragma region i_control
		i_control* c_control_base::Parent()
		{
			return &m_parent;
		}

		uint32 c_control_base::GetResourceID() const
		{
			return m_resource_id;
		}

		void c_control_base::SetResourceID(const uint32 resource_id)
		{
			m_resource_id = resource_id;
		}

		void c_control_base::AddControl(control_ptr_t control)
		{
			// Add the control to the list if it does not already exist
			auto found_entry = FindControl(control);

			YELO_ASSERT_DISPLAY(found_entry == m_child_controls.end(), "Attempted to add a control instance to the control multiple times");

			if(found_entry == m_child_controls.end())
			{
				m_child_controls.push_back(control);
			}
		}

		void c_control_base::RemoveControl(control_ptr_t control)
		{
			// Remove the control from the list if it exists
			auto found_entry = FindControl(control);

			YELO_ASSERT_DISPLAY(found_entry != m_child_controls.end(), "Attempted to remove a non-existant control from a control");

			if(found_entry != m_child_controls.end())
			{
				m_child_controls.erase(found_entry);
			}
		}

		control_list_t& c_control_base::Controls()
		{
			return m_child_controls;
		}

		i_property_interface* c_control_base::GetPropertyInterface(const uint32 interface_id) const
		{
			auto property_interface = FindPropertyInterface(interface_id);

			YELO_ASSERT_DISPLAY(property_interface != nullptr, "Failed to find a property interface in a control");

			return property_interface;
		}

		void c_control_base::AddEventCallback(const uint32 event_id, const uint32 callback_id, const event_callback_t& function, void* userdata = nullptr)
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
	};};};
};
#endif