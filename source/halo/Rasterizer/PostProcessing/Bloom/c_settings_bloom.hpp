/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include <yelo/configuration/c_configuration_container.hpp>
#include <yelo/configuration/c_configuration_value.hpp>
#include <yelo/open_sauce/settings/c_settings_singleton.hpp>

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing { namespace Bloom
	{
		class c_settings_container
			: public Configuration::c_configuration_container
		{
		public:
			Configuration::c_configuration_value<bool> m_enabled;

			c_settings_container();

		protected:
			std::vector<i_configuration_value*> GetMembers() final override;
		};

		class c_settings_bloom
			: public Settings::c_settings_singleton<c_settings_container, c_settings_bloom>
		{
		public:
			void PostLoad() final override;
			void PreSave() final override;
		};
	};};};
};
#endif