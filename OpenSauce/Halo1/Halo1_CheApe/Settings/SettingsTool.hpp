/*
	Yelo: Open Sauce SDK
	Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

#include <YeloLib/configuration/c_configuration_container.hpp>
#include <YeloLib/configuration/c_configuration_value.hpp>
#include <YeloLib/open_sauce/settings/c_settings_singleton.hpp>

namespace Yelo
{
	namespace Settings
	{
		class c_tool_container final
			: public Configuration::c_configuration_container
		{
		public:
			Configuration::c_configuration_value<bool> m_disable_exception_handling;
			Configuration::c_configuration_value<bool> m_do_full_crashdump;

			c_tool_container();

		protected:
			const std::vector<i_configuration_value* const> GetMembers() override;
		};

		class c_settings_tool final
			: public c_settings_singleton<c_tool_container, c_settings_tool>
		{ };
	};
};