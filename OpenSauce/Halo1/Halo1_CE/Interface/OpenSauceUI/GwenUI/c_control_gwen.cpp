/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI/GwenUI/c_control_gwen.hpp"

#if !PLATFORM_IS_DEDI

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace GwenUI
	{
#pragma region c_event_handler_gwen_handler
		void c_event_handler_gwen_handler::Callback(Gwen::Event::Info& info)
		{
			Control::s_interface_value event_data;

			// If the data transform function is set use it to get the event data from the event info
			if(m_data_transform)
			{
				m_data_transform(info, event_data);
			}

			m_owner.Trigger(event_data);
		}

		c_event_handler_gwen_handler::c_event_handler_gwen_handler(const c_event_handler_gwen& owner
			, Gwen::Event::Caller& caller
			, const t_event_data_transform& data_transform)
			: m_owner(owner)
			, m_caller(caller)
			, is_attached(false)
			, m_data_transform(data_transform)
		{ }

		c_event_handler_gwen_handler::~c_event_handler_gwen_handler()
		{
			Detach();
		}

		void c_event_handler_gwen_handler::Attach()
		{
			// If the handler is not already attached, attach it
			if(!is_attached)
			{
				m_caller.Add(this, &c_event_handler_gwen_handler::Callback);
				is_attached = true;
			}
		}

		void c_event_handler_gwen_handler::Detach()
		{
			// If the handler is not already detached, detach it
			if(is_attached)
			{
				m_caller.RemoveHandler(this);
				is_attached = false;
			}
		}
#pragma endregion

#pragma region c_event_handler_gwen
		c_event_handler_gwen::c_event_handler_gwen(Gwen::Event::Caller& caller, const t_event_data_transform& data_transform)
			: Control::c_event_handler()
			, m_handler_thunk(*this, caller, data_transform)
		{ }

		void c_event_handler_gwen::Attach()
		{
			m_handler_thunk.Attach();
		}

		void c_event_handler_gwen::Detach()
		{
			m_handler_thunk.Detach();
		}
#pragma endregion

#pragma region c_control_gwen
		c_control_gwen::c_control_gwen(Control::i_control& parent
			, Gwen::Controls::Base* gwen_control
			, const Control::t_property_interface_map& property_interfaces
			, const Control::t_event_handler_map& event_handlers)
			: c_control_base(parent)
			, m_control(gwen_control)
		{
			// Add all property interfaces
			for(auto& property_interface : property_interfaces)
			{
				AddPropertyInterface(property_interface.first, property_interface.second);
			}

			// Add all event handlers
			for(auto& event_handler : event_handlers)
			{
				AddEventHandler(event_handler.first, event_handler.second);
			}
		}

#pragma region i_control
		rectangle2d c_control_gwen::GetBounds() const
		{
			auto& bounds = m_control->GetBounds();

			return rectangle2d { bounds.x, bounds.y, bounds.h, bounds.w };
		}

		void* c_control_gwen::GetControlPtr() const
		{
			return m_control;
		}

		void c_control_gwen::Show()
		{
			m_control->Show();
		}

		void c_control_gwen::Hide()
		{
			m_control->Hide();
		}
#pragma endregion
#pragma endregion
	};};};
};
#endif