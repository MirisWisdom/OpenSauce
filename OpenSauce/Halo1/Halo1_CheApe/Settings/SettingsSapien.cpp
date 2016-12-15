/*
	Yelo: Open Sauce SDK
	Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Settings/SettingsSapien.hpp"

#include <YeloLib/configuration/c_configuration_container.hpp>
#include <YeloLib/configuration/c_configuration_value.hpp>

using namespace Yelo::Configuration;

namespace Yelo
{
	namespace Settings
	{
		c_sapien_container::c_sapien_container()
			: c_configuration_container("Sapien")
			, m_affinity_mask("AffinityMask", 1)
		{ }

		const std::vector<i_configuration_value* const> c_sapien_container::GetMembers()
		{
			return std::vector<i_configuration_value* const> { &m_affinity_mask };
		}
	};
};