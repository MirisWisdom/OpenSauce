/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI/Definitions/c_page_definition.hpp"

#if !PLATFORM_IS_DEDI

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace Definitions
	{
#pragma region c_page_definition
		c_page_definition::c_page_definition()
			: Configuration::c_configuration_container("Page")
			, m_controls("Control", [](){ return c_control_definition(); })
		{ }

		const std::vector<Configuration::i_configuration_value* const> c_page_definition::GetMembers()
		{
			return std::vector<Configuration::i_configuration_value* const> { &m_controls };
		}
#pragma endregion
	};};};
};
#endif