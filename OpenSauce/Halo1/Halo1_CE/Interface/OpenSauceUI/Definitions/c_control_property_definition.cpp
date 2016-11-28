/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI/Definitions/c_control_property_definition.hpp"

#if !PLATFORM_IS_DEDI

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace Definitions
	{
#pragma region c_control_property_definition
		c_control_property_definition::c_control_property_definition()
			: Configuration::c_configuration_container("Property")
			, m_name("Name", "")
			, m_value("Value", "")
		{ }

		const std::vector<Configuration::i_configuration_value* const> c_control_property_definition::GetMembers()
		{
			return std::vector<Configuration::i_configuration_value* const> { &m_name, &m_value };
		}
#pragma endregion
	};};};
};
#endif