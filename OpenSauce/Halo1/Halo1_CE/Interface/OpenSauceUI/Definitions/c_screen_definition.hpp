/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include <YeloLib/configuration/c_configuration_container.hpp>
#include <YeloLib/configuration/c_configuration_value.hpp>

#include "Interface/OpenSauceUI/Definitions/c_control_definition.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace Definitions
	{
		/// <summary>	A screen definition. </summary>
		class c_screen_definition
			: public Configuration::c_configuration_container
		{
		public:
			Configuration::c_configuration_value<std::string> m_screen_name;
			c_control_definition m_control_definition;

			/// <summary>	Default constructor. </summary>
			c_screen_definition();

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