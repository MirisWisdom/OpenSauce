/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include "Interface/OpenSauceUI/Control/i_control.hpp"
#include "Interface/OpenSauceUI/Definitions/c_control_definition.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace Control
	{
		/// <summary>	An event handler. </summary>
		class c_event_handler
			abstract
		{
		private:
			struct s_event_callback
			{
				uint32 id;
				void* userdata;
				t_event_callback function;

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
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Adds a callback. </summary>
			///
			/// <param name="callback_id">	Identifier for the callback. </param>
			/// <param name="function">   	The function. </param>
			/// <param name="userdata">   	[in] If non-null, the userdata. </param>
			void AddCallback(const uint32 callback_id, const t_event_callback& function, void* userdata);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Removes the callback described by callback_id. </summary>
			///
			/// <param name="callback_id">	Identifier for the callback. </param>
			void RemoveCallback(const uint32 callback_id);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Triggers the event. </summary>
			///
			/// <param name="event_data">	Information from the event. </param>
			void Trigger(const s_interface_value& event_data) const;

		private:
			/// <summary>	Attaches the handler to the event. </summary>
			virtual void Attach() = 0;

			/// <summary>	Detaches the handler from the event. </summary>
			virtual void Detach() = 0;
		};

		/// <summary>	Defines an alias representing event handler shared pointer. </summary>
		typedef std::shared_ptr<c_event_handler> t_event_handler_ptr;

		/// <summary>	Defines an alias representing an event_handler map. </summary>
		typedef std::map<uint32, t_event_handler_ptr> t_event_handler_map;

		/// <summary>	A property interface. </summary>
		class c_property_interface final
			: public i_property_interface
		{
			/// <summary>	Defines an alias representing the property get function. </summary>
			typedef std::function<s_interface_value(i_control&)> t_property_get_func;

			/// <summary>	Defines an alias representing the property set function. </summary>
			typedef std::function<void(i_control&, const s_interface_value&)> t_property_set_func;

			/// <summary>	Defines an alias representing the property set by string function. </summary>
			typedef std::function<void(i_control&, const char*)> t_property_set_by_string_func;

			const t_property_get_func m_get;
			const t_property_set_func m_set;
			const t_property_set_by_string_func m_set_string;

		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Constructor. </summary>
			///
			/// <param name="get">		 	The get function. </param>
			/// <param name="set">		 	The set function. </param>
			/// <param name="set_string">	The set by string function. </param>
			c_property_interface(const t_property_get_func& get
				, const t_property_set_func& set
				, const t_property_set_by_string_func& set_string);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets a property using a string representation. </summary>
			///
			/// <param name="control">	[in] The control to modify. </param>
			/// <param name="value">  	The value to set. </param>
			void Set(i_control& control, const char* value) override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets a property using the specified value. </summary>
			///
			/// <param name="control">	[in] The control to modify. </param>
			/// <param name="value">  	The value to set. </param>
			void Set(i_control& control, const s_interface_value& value) override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the value of a property. </summary>
			///
			/// <param name="control">	[in] The control to get the value from. </param>
			/// <param name="default">	The default value. </param>
			///
			/// <returns>	The properties value of the default value. </returns>
			s_interface_value Get(i_control& control, const s_interface_value& default_value) const override;
		};
		
#define DEFINE_PROPERTY_INTERFACE(name, get, set, set_string) static Control::c_property_interface g_property_interface_##name ((get), (set), (set_string))
#define GET_PROPERTY_INTERFACE(name) &g_property_interface_##name

		/// <summary>	The OpenSauceUI base control. </summary>
		class c_control_base
			abstract
			: public i_control
		{
		private:
			i_control& m_parent;
			uint32 m_resource_id;
			t_control_list m_child_controls;
			t_property_interface_map m_property_interfaces;
			t_event_handler_map m_event_handlers;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Searches for a matching control in the child control list. </summary>
			///
			/// <param name="control">	The control to find. </param>
			///
			/// <returns>	The found control. </returns>
			t_control_list::const_iterator FindControl(const t_control_ptr control) const;
			
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
			void AddEventHandler(const uint32 event_id, const t_event_handler_ptr& event_handler);

		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Default constructor. </summary>
			///
			/// <param name="parent">	[in] The parent. </param>
			c_control_base(i_control& parent);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Searches for the a property interface. </summary>
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
			void AddControl(t_control_ptr control) final override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Removes the specified control from the child control list. </summary>
			///
			/// <remarks>	If the specified control does not exist, the action is ignored. </remarks>
			///
			/// <param name="control">	The control to remove. </param>
			void RemoveControl(t_control_ptr control) final override;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the controls list. </summary>
			///
			/// <returns>	A list of controls. </returns>
			t_control_list& Controls() final override;

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
			void AddEventCallback(const uint32 event_id, const uint32 callback_id, const t_event_callback& function, void* userdata) final override;

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