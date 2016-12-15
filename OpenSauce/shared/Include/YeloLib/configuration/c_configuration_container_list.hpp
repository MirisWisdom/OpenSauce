/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <YeloLib\configuration\i_configuration_container.hpp>
#include <YeloLib\configuration\c_configuration_list.hpp>

namespace Yelo
{
	namespace Configuration
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Configuration list variant that handles configuration containers. </summary>
		///
		/// <typeparam name="T">	Generic type parameter. </typeparam>
		template<typename T>
		class c_configuration_container_list
			: public c_configuration_list<T>
		{
			//static_assert(std::is_base_of<i_configuration_container, T>::value, "Configuration container lists are for types that derive from i_configuration_container only");
			
		public:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Initializes a new instance of the c_configuration_container_list class. </summary>
			///
			/// <param name="node_name">   	Name of the containers node. </param>
			/// <param name="create_entry">	The function to use when creating a new container. </param>
			c_configuration_container_list(const std::string& node_name, const std::function<T()>& create_entry)
				: c_configuration_list<T>(node_name, create_entry)
			{ }
		};
	};
};