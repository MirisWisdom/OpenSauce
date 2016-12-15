/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include "Interface/OpenSauceUI/Control/i_control.hpp"
#include "Interface/OpenSauceUI/Control/c_event_handler.hpp"
#include "Interface/OpenSauceUI/Control/c_property_interface.hpp"
#include "Interface/OpenSauceUI/Definitions/c_control_definition.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace Control
	{
		/// <summary>	The OpenSauceUI base control. </summary>
		class c_control_base
			abstract
			: public i_control
		{
		private:
			i_control& m_parent;
			uint32 m_resource_id;
			control_list_t m_child_controls;
			property_interface_map_t m_property_interfaces;
			event_handler_map_t m_event_handlers;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Searches for a matching control in the child control list. </summary>
			///
			/// <param name="control">	The control to find. </param>
			///
			/// <returns>	The found control. </returns>
			control_list_t::const_iterator FindControl(const control_ptr_t control) const;
			
		protected:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Adds a property interface to the control. </summary>
			///
			/// <param name="interface_id">		 	Identifier for the interface. </param>
			/// <param name="property_interface">	[in] The property interface object. </param>
			void AddPropertyInterface(const uint32 interface_id, i_property_interface* property_interface);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Adds an event handler to the control. </summary>
			///
			/// <param name="event_id">			Identifier for the event. </param>
			/// <param name="event_handler">	[in] The event handler. </param>
			void AddEventHandler(const uint32 event_id, const event_handler_ptr_t& event_handler);

		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Default constructor. </summary>
			///
			/// <param name="parent">	[in] The parent. </param>
			c_control_base(i_control& parent);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Searches for a property interface. </summary>
			///
			/// <param name="interface_id">	Identifier for the interface. </param>
			///
			/// <returns>	null if it fails, else the found property interface. </returns>
			i_property_interface* FindPropertyInterface(const uint32 interface_id) const;
			
#pragma region i_control
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the controls parent. </summary>
			///
			/// <returns>	The controls parent. </returns>
			i_control* Parent() final override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the resource identifier of the control. </summary>
			///
			/// <returns>	The controls resource identifier. </returns>
			uint32 GetResourceID() const final override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets the resource identifier of the control. </summary>
			///
			/// <param name="resource_id">	Identifier for the control resource. </param>
			void SetResourceID(const uint32 resource_id) final override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Adds a control to the child control list. </summary>
			///
			/// <remarks>	If the specified control has already been added the action is ignored. </remarks>
			///
			/// <param name="control">	The control to add. </param>
			void AddControl(control_ptr_t control) final override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Removes the specified control from the child control list. </summary>
			///
			/// <remarks>	If the specified control does not exist, the action is ignored. </remarks>
			///
			/// <param name="control">	The control to remove. </param>
			void RemoveControl(control_ptr_t control) final override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the child controls list. </summary>
			///
			/// <returns>	A list of controls. </returns>
			control_list_t& Controls() final override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets a property interface. </summary>
			///
			/// <param name="interface_id">	The interface id. </param>
			///
			/// <returns>	null if it fails, else the property interface. </returns>
			i_property_interface* GetPropertyInterface(const uint32 interface_id) const final override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Adds an event callback. </summary>
			///
			/// <param name="event_id">   	Identifier for the event. </param>
			/// <param name="callback_id">	Identifier for the callback. </param>
			/// <param name="callback">   	The callback function. </param>
			/// <param name="userdata">   	[in] The userdata for the event. </param>
			void AddEventCallback(const uint32 event_id, const uint32 callback_id, const event_callback_t& function, void* userdata) final override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Removes an event callback. </summary>
			///
			/// <param name="event_id">   	Identifier for the event. </param>
			/// <param name="callback_id">	The callback identifier. </param>
			void RemoveEventCallback(const uint32 event_id, const uint32 callback_id) final override;
#pragma endregion
		};
	};};};
};
#endif