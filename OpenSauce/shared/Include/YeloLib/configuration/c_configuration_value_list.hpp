/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <YeloLib\configuration\c_configuration_value.hpp>
#include <YeloLib\configuration\c_configuration_list.hpp>

namespace Yelo
{
	namespace Configuration
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Configuration list variant that handles integral variable types and std::strings. </summary>
		///
		/// <typeparam name="T">	Generic type parameter. </typeparam>
		template<typename T>
		class c_configuration_value_list final
			: public c_configuration_list<c_configuration_value<T>>
		{
			static_assert(std::is_integral<T>::value || std::is_same<T, std::string>::value, "Configuration value lists are for integral types and std::string's only, use c_configuration_container_list for containers");
			
		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Initializes a new instance of the c_configuration_value_list class. </summary>
			///
			/// <param name="node_name">		Name of the configuration node. </param>
			/// <param name="default_value">	The default value for the node. </param>
			c_configuration_value_list(const std::string& node_name, const T default_value)
				: c_configuration_list<c_configuration_value<T>>(node_name, [node_name, default_value]() { return c_configuration_value<T>(node_name, default_value); })
			{ }
		};
	};
};