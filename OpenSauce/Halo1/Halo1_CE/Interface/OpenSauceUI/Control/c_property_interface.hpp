/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include "Interface/OpenSauceUI/Control/i_control.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace Control
	{
		/// <summary>	A property interface. </summary>
		class c_property_interface final
			: public i_property_interface
		{
			/// <summary>	Defines an alias representing the property get function. </summary>
			typedef std::function<void(i_control&, s_interface_value&)> property_get_func_t;

			/// <summary>	Defines an alias representing the property set function. </summary>
			typedef std::function<void(i_control&, const s_interface_value&)> property_set_func_t;

			/// <summary>	Defines an alias representing the property set by string function. </summary>
			typedef std::function<void(i_control&, const char*)> property_set_by_string_func_t;

			const property_get_func_t m_get;
			const property_set_func_t m_set;
			const property_set_by_string_func_t m_set_string;

		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Constructor. </summary>
			///
			/// <param name="get">		 	The get function. </param>
			/// <param name="set">		 	The set function. </param>
			/// <param name="set_string">	The set by string function. </param>
			c_property_interface(const property_get_func_t& get
				, const property_set_func_t& set
				, const property_set_by_string_func_t& set_string);

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
			/// <param name="control">	   	[in] The control to get the value from. </param>
			/// <param name="output_value">	[out] The output value. </param>
			///
			/// <returns>	true if the get function was executed. </returns>
			bool Get(i_control& control, s_interface_value& output_value) const override;
		};

#define DEFINE_PROPERTY_INTERFACE(name, get, set, set_string) static Control::c_property_interface g_property_interface_##name ((get), (set), (set_string))
#define GET_PROPERTY_INTERFACE(name) &g_property_interface_##name

	};};};
};
#endif