/*
	Yelo: Open Sauce SDK
	Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if PLATFORM_IS_USER

#include <blamlib/cseries/cseries_base.hpp>
#include <vector>
#include <yelolib/configuration/c_configuration_value.hpp>
#include <yelolib/open_sauce/settings/c_settings_singleton.hpp>

namespace Yelo
{
	namespace Fov
	{
		class c_settings_container
			: public Configuration::c_configuration_container
		{
		public:
			Configuration::c_configuration_value<real> m_field_of_view;
			Configuration::c_configuration_value<bool> m_ignore_fov_change_in_cinematics;
			Configuration::c_configuration_value<bool> m_ignore_fov_change_in_main_menu;

			c_settings_container();

		protected:
			std::vector<i_configuration_value*> GetMembers() final override;
		};

		class c_settings_fov
			: public Settings::c_settings_singleton<c_settings_container, c_settings_fov>
		{
		public:
			void PostLoad() final override;

			void PreSave() final override;
		};

		void Initialize();
		void Dispose();

		real GetFieldOfView();
		void SetFieldOfView(
			real fov);
		void ResetFieldOfView();
	};
}
#endif
