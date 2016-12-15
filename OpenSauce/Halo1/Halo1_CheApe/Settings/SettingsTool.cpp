/*
	Yelo: Open Sauce SDK
	Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Settings/SettingsTool.hpp"

#include <YeloLib/configuration/c_configuration_container.hpp>
#include <YeloLib/configuration/c_configuration_value.hpp>

using namespace Yelo::Configuration;

namespace Yelo
{
	namespace Settings
	{
		c_tool_container::c_tool_container()
			: c_configuration_container("Tool")
			, m_disable_exception_handling("DisableExceptionHandling", false)
			, m_do_full_crashdump("DoFullCrashdump", false)
		{ }

		const std::vector<i_configuration_value* const> c_tool_container::GetMembers()
		{
			return std::vector<i_configuration_value* const>
			{
				&m_disable_exception_handling,
				&m_do_full_crashdump
			};
		}
	};
};