/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI/Definitions/c_screen_definition.hpp"

#if !PLATFORM_IS_DEDI

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace Definitions
	{
#pragma region c_screen_definition
		c_screen_definition::c_screen_definition()
			: Configuration::c_configuration_container("Screen")
			, m_screen_name("Name", "")
			, m_control_definition()
		{ }

		const std::vector<Configuration::i_configuration_value* const> c_screen_definition::GetMembers()
		{
			return std::vector<Configuration::i_configuration_value* const> { &m_screen_name, &m_control_definition };
		}
#pragma endregion
	};};};
};
#endif