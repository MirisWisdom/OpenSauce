/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI/Definitions/c_control_definition.hpp"

#if !PLATFORM_IS_DEDI

#include "Interface/OpenSauceUI/resource_id.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace Definitions
	{
#pragma region c_control_definition
		c_control_definition::c_control_definition()
			: Configuration::c_configuration_container("Control")
#ifdef DEBUG
			, m_id("IDStr", "")
#else
			, m_id("ID", RESOURCE_ID_NONE)
#endif
			, m_type("Type", "")
			, m_properties("Property", [](){ return c_control_property_definition(); })
			, m_controls("Control", [](){ return c_control_definition(); })
			, m_pages("Page", [](){ return c_page_definition(); })
		{ }

		const std::vector<Configuration::i_configuration_value* const> c_control_definition::GetMembers()
		{
			return std::vector<Configuration::i_configuration_value* const> { &m_id, &m_type, &m_properties, &m_controls, &m_pages };
		}

		const uint32 c_control_definition::GetID()
		{
#ifdef DEBUG
			if(m_id.Get().empty())
			{
				return RESOURCE_ID_NONE;
			}

			return RESOURCE_ID_DEBUG(m_id.Get().c_str());
#else
			return m_id;
#endif
		}
#pragma endregion
	};};};
};
#endif