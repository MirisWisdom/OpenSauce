/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include "Interface/OpenSauceUI/i_visibility_toggle.hpp"
#include "Interface/OpenSauceUI/Control/s_interface_value.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace Control
	{
		class i_control;

		/// <summary>	Defines an alias representing the a shared control pointer. </summary>
		typedef std::shared_ptr<i_control> control_ptr_t;

		/// <summary>	Defines an alias representing a list of shared controls pointers. </summary>
		typedef std::vector<control_ptr_t> control_list_t;

#pragma region Property Interface
		/// <summary>	A property interface. </summary>
		class i_property_interface
			abstract
		{
		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets a property using a string representation. </summary>
			///
			/// <param name="control">	[in] The control to modify. </param>
			/// <param name="value">  	The value to set. </param>
			virtual void Set(i_control& control, const char* value) = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets a property using the specified value. </summary>
			///
			/// <param name="control">	[in] The control to modify. </param>
			/// <param name="value">  	The value to set. </param>
			virtual void Set(i_control& control, const s_interface_value& value) = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the value of a property. </summary>
			///
			/// <param name="control">	   	[in] The control to get the value from. </param>
			/// <param name="output_value">	[out] The output value. </param>
			///
			/// <returns>	true if the get function was executed. </returns>
			virtual bool Get(i_control& control, s_interface_value& output_value) const = 0;
		};
		
		/// <summary>	Defines an alias representing a property interface map. </summary>
		typedef std::map<uint32, i_property_interface*> property_interface_map_t;
#pragma endregion

#pragma region Event
		/// <summary>	Defines an alias for an event callback function. </summary>
		typedef std::function<void(const s_interface_value&, void*)> event_callback_t;
#pragma endregion

		/// <summary>	Base interface for an OpenSauceUI control. </summary>
		class i_control
			abstract
			: public i_visibility_toggle
		{
		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the controls parent. </summary>
			///
			/// <returns>	The controls parent. </returns>
			virtual i_control* Parent() = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the resource identifier of the control. </summary>
			///
			/// <returns>	The controls resource identifier. </returns>
			virtual uint32 GetResourceID() const = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the controls bounds. </summary>
			///
			/// <returns>	The controls bounds. </returns>
			virtual rectangle2d GetBounds() const = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Sets the resource identifier of the control. </summary>
			///
			/// <param name="resource_id">	Identifier for the control resource. </param>
			virtual void SetResourceID(const uint32 resource_id) = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets a raw pointer to an implementation specific control object. </summary>
			///
			/// <returns>	Returns the implementation specific control pointer. </returns>
			virtual void* GetControlPtr() const = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Adds a control to the child control list. </summary>
			///
			/// <remarks>	If the specified control has already been added the action is ignored. </remarks>
			///
			/// <param name="control">	The control to add. </param>
			virtual void AddControl(control_ptr_t control) = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Removes the specified control from the child control list. </summary>
			///
			/// <remarks>	If the specified control does not exist, the action is ignored. </remarks>
			///
			/// <param name="control">	The control to remove. </param>
			virtual void RemoveControl(control_ptr_t control) = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the controls list. </summary>
			///
			/// <returns>	A list of controls. </returns>
			virtual control_list_t& Controls() = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets a property interface. </summary>
			///
			/// <param name="interface_id">	The interface id. </param>
			///
			/// <returns>	null if it fails, else the property interface. </returns>
			virtual i_property_interface* GetPropertyInterface(const uint32 interface_id) const = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Adds an event callback. </summary>
			///
			/// <param name="event_id">   	Identifier for the event. </param>
			/// <param name="callback_id">	Identifier for the callback. </param>
			/// <param name="callback">   	The callback function. </param>
			/// <param name="userdata">   	[in] The userdata for the event. </param>
			virtual void AddEventCallback(const uint32 event_id, const uint32 callback_id, const event_callback_t& function, void* userdata) = 0;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Removes an event callback. </summary>
			///
			/// <param name="event_id">   	Identifier for the event. </param>
			/// <param name="callback_id">	The callback identifier. </param>
			virtual void RemoveEventCallback(const uint32 event_id, const uint32 callback_id) = 0;
		};
	};};};
};
#endif