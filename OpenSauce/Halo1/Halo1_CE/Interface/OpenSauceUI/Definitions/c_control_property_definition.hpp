/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include <YeloLib/configuration/c_configuration_container.hpp>
#include <YeloLib/configuration/c_configuration_value.hpp>

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace Definitions
	{
		/// <summary>	A control property definition. </summary>
		class c_control_property_definition
			: public Configuration::c_configuration_container
		{
		public:
			Configuration::c_configuration_value<std::string> m_name;
			Configuration::c_configuration_value<std::string> m_value;

			/// <summary>	Default constructor. </summary>
			c_control_property_definition();

		protected:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets a vector containing pointers to the containers members. </summary>
			///
			/// <returns>	A vector containing pointers to the containers members. </returns>
			const std::vector<Configuration::i_configuration_value* const> GetMembers() final override;
		};
	};};};
};
#endif