/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI/Screen/c_screen_controller_mainmenubottombar.hpp"

#if !PLATFORM_IS_DEDI

#include "Interface/OpenSauceUI/Control/control_property_ids.hpp"
#include "Interface/OpenSauceUI/Control/control_event_ids.hpp"

#include "Networking/VersionCheckClient.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace Screen
	{
#pragma region Resource ID Defines
#define K_LBL_OPENSAUCE_NEW_VERSION_ID											RESOURCE_ID("#LBL_opensauce_new_version",			0x878BB692)
#define K_LBL_OPENSAUCE_CURRENT_VERSION_ID										RESOURCE_ID("#LBL_opensauce_current_version",		0xACB12271)
#define K_LBL_OPENSAUCE_SETTINGS_BUTTON_TEXT_ID									RESOURCE_ID("#LBL_opensauce_settings_button_text",	0x6D501BA7)
#pragma endregion
		
		c_screen_controller_mainmenubottombar::c_screen_controller_mainmenubottombar(Definitions::c_screen_definition& definition)
			: c_screen_controller_base(definition)
			, m_available_version_set(false)
		{ }

		void c_screen_controller_mainmenubottombar::SetStaticProperties()
		{
			auto& version_checker = Networking::VersionCheck::c_version_check_manager_base::VersionChecker();
			auto& version = version_checker.CurrentVersion();

			char version_string[36];
			sprintf_s(version_string, "OpenSauce v%i.%i.%i (%s)",
				version.m_major,
				version.m_minor,
				version.m_build,
				version.m_build_date);

			// Set all static text strings in the screen
			SetControlProperty(K_LBL_OPENSAUCE_NEW_VERSION_ID,				K_PROPERTY_TEXT_ID, "");
			SetControlProperty(K_LBL_OPENSAUCE_CURRENT_VERSION_ID,			K_PROPERTY_TEXT_ID, version_string);
			SetControlProperty(K_LBL_OPENSAUCE_SETTINGS_BUTTON_TEXT_ID,		K_PROPERTY_TEXT_ID, "Press F7 for OS settings");
		}

		void c_screen_controller_mainmenubottombar::BindDynamicProperties()
		{
			AddDynamicProperty(K_LBL_OPENSAUCE_NEW_VERSION_ID,				K_PROPERTY_TEXT_ID,
				[this](Control::i_control& control, Control::i_property_interface& property)
				{
					if(m_available_version_set)
					{
						return;
					}

					auto& version_checker = Networking::VersionCheck::c_version_check_manager_base::VersionChecker();

					if(version_checker.IsNewVersionAvailable())
					{
						auto& version = version_checker.AvailableVersion();

						char version_string[36];
						if(is_null_or_empty(version.m_build_date))
						{
							sprintf_s(version_string, "v%i.%i.%i available!",
								version.m_major,
								version.m_minor,
								version.m_build);
						}
						else
						{
							sprintf_s(version_string, "v%i.%i.%i (%s) available!",
								version.m_major,
								version.m_minor,
								version.m_build,
								version.m_build_date);
						}

						property.Set(control, version_string);

						m_available_version_set = true;
					}
				});
		}
	};};};
};
#endif