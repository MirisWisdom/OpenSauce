/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include <YeloLib/configuration/c_configuration_container.hpp>
#include <YeloLib/configuration/c_configuration_container_list.hpp>
#include <YeloLib/configuration/c_configuration_value.hpp>

#include "Interface/OpenSauceUI/Definitions/c_control_property_definition.hpp"
#include "Interface/OpenSauceUI/Definitions/c_page_definition.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace Definitions
	{
		class c_page_definition;

		/// <summary>	A control definition. </summary>
		class c_control_definition
			: public Configuration::c_configuration_container
		{
		public:
#ifdef DEBUG
			Configuration::c_configuration_value<std::string> m_id;
#else
			Configuration::c_configuration_value<uint32> m_id;
#endif
			Configuration::c_configuration_value<std::string> m_type;
			Configuration::c_configuration_container_list<c_control_property_definition> m_properties;
			Configuration::c_configuration_container_list<c_control_definition> m_controls;
			Configuration::c_configuration_container_list<c_page_definition> m_pages;

			/// <summary>	Default constructor. </summary>
			c_control_definition();

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets the resource id for the control. </summary>
			///
			/// <returns>	The controls resource id. </returns>
			const uint32 GetID();

		protected:
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Gets a vector containing pointers to the containers members. </summary>
			///
			/// <returns>	A vector containing pointers to the containers members. </returns>
			const std::vector<Configuration::i_configuration_value* const> GetMembers() final override;
		};

		/// <summary>	Defines an alias representing a list of control definitions. </summary>
		typedef std::vector<Definitions::c_control_definition> control_definition_list_t;
	};};};
};
#endif