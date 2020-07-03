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
		class i_control;

		/// <summary>	A property interface interfcae. </summary>
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
	};};}
};
#endif