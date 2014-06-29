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
		class c_sapien_container
			: public Configuration::c_configuration_container
		{
		public:
			Configuration::c_configuration_value<uint32> m_affinity_mask;

			c_sapien_container();

		protected:
			const std::vector<i_configuration_value* const> GetMembers() final override;
		};

		class c_settings_sapien
			: public c_settings_singleton<c_sapien_container, c_settings_sapien>
		{ };
	};
};