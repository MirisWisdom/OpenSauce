/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blam/cache/cache_files.hpp>
#include <blam/structures/structure_bsp_definitions.hpp>

#include <yelo/open_sauce/blam_memory_upgrades.hpp>

#include <yelo/configuration/c_configuration_container.hpp>
#include <yelo/configuration/c_configuration_value.hpp>
#include <yelo/open_sauce/settings/c_settings_singleton.hpp>

namespace Yelo
{
	namespace DataFiles
	{
		void Initialize();
		void Dispose();
	};

	namespace Cache
	{
		class c_settings_container
			: public Configuration::c_configuration_container
		{
		public:
			Configuration::c_configuration_value<bool> m_check_yelo_files_first;
		#if !PLATFORM_IS_DEDI
			Configuration::c_configuration_value<std::string> m_mainmenu_scenario;
		#endif

			c_settings_container();
			
		protected:
			std::vector<i_configuration_value*> GetMembers() final override;
		};

		class c_settings_cache
			: public Settings::c_settings_singleton<c_settings_container, c_settings_cache>
		{
		public:
			void PostLoad() final override;
		};

		void Initialize();
		void Dispose();
	};
};