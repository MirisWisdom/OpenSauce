/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/OpenSauceUI/Screen/c_screen_controller_mapdownload.hpp"

#if !PLATFORM_IS_DEDI

#include "Interface/OpenSauceUI/Control/control_property_ids.hpp"
#include "Networking/HTTP/MapDownloadClientDisplay.hpp"

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI { namespace Screen
	{
#pragma region Resource ID Defines
#define K_LBL_MAP_DOWNLOAD_TITLE_ID									RESOURCE_ID_DEBUG("#LBL_map_download_title")
		
#define K_LBL_MAP_NAME_ID											RESOURCE_ID_DEBUG("#LBL_map_name")
#define K_PRG_MAP_PROGRESS_ID										RESOURCE_ID_DEBUG("#PRG_map_progress")
#define K_LBL_ESC_TO_CANCEL_TEXT_ID									RESOURCE_ID_DEBUG("#LBL_esc_to_cancel_text")

#define K_DOWNLOAD_SOURCE_TITLE_ID									RESOURCE_ID_DEBUG("#LBL_download_source_title")
#define K_DOWNLOAD_SOURCE_DESCRIPTION_ID							RESOURCE_ID_DEBUG("#LBL_download_source_description")
#pragma endregion

		c_screen_controller_mapdownload::c_screen_controller_mapdownload(Definitions::c_screen_definition& definition)
			: c_screen_controller_base(definition)
		{ }

		void c_screen_controller_mapdownload::ResetScreen()
		{
			SetControlProperty(K_LBL_MAP_DOWNLOAD_TITLE_ID,				K_PROPERTY_TEXT_ID, "Map Download");
			SetControlProperty(K_LBL_MAP_NAME_ID,						K_PROPERTY_TEXT_ID, "");
			SetControlProperty(K_PRG_MAP_PROGRESS_ID,					K_PROPERTY_TEXT_ID, "0%");
			SetControlProperty(K_PRG_MAP_PROGRESS_ID,					K_PROPERTY_VALUE_ID, 0.0f);
			SetControlProperty(K_LBL_ESC_TO_CANCEL_TEXT_ID,				K_PROPERTY_TEXT_ID, "Press [esc] to cancel");
			SetControlProperty(K_DOWNLOAD_SOURCE_TITLE_ID,				K_PROPERTY_TEXT_ID, "");
			SetControlProperty(K_DOWNLOAD_SOURCE_DESCRIPTION_ID,		K_PROPERTY_TEXT_ID, "");
		}

		void c_screen_controller_mapdownload::SetStaticProperties()
		{
			// Set the screen to be central
			AnchorScreen(
				(screen_anchor)(_screen_anchor_center_v | _screen_anchor_center_h),
				(screen_anchor)(_screen_anchor_center_v | _screen_anchor_center_h));

			ResetScreen();
		}

		void c_screen_controller_mapdownload::BindDynamicProperties()
		{
			// Set all dynamic properties in the UI controls
			AddDynamicProperty(K_LBL_MAP_NAME_ID,					K_PROPERTY_TEXT_ID,
				[](Control::i_control& control, Control::i_property_interface& property)
				{
					auto& map_download_display = Networking::HTTP::Client::MapDownload::g_map_download_display;

					property.Set(control, map_download_display.GetMapName());
				});

			AddDynamicProperty(K_PRG_MAP_PROGRESS_ID,				K_PROPERTY_TEXT_ID,
				[](Control::i_control& control, Control::i_property_interface& property)
				{
					auto& map_download_display = Networking::HTTP::Client::MapDownload::g_map_download_display;

					switch(map_download_display.GetMapDownloadState())
					{
					case Networking::HTTP::Client::MapDownload::_map_download_state_searching:
						property.Set(control, "0%");
						break;
					case Networking::HTTP::Client::MapDownload::_map_download_state_downloading:
						{
							char text[10] = "";
							sprintf_s(text, "%3.f%%", map_download_display.GetMapProgress() * 100);
							property.Set(control, text);
						}
						break;
					case Networking::HTTP::Client::MapDownload::_map_download_state_completed:
						property.Set(control, "100%");
						break;
					case Networking::HTTP::Client::MapDownload::_map_download_state_reconnecting:
						property.Set(control, "Connecting to server");
						break;
					case Networking::HTTP::Client::MapDownload::_map_download_state_extracting:
						property.Set(control, "Extracting map...");
						break;
					case Networking::HTTP::Client::MapDownload::_map_download_state_failed:
						property.Set(control, "Download failed");
						break;
					}
				});

			AddDynamicProperty(K_PRG_MAP_PROGRESS_ID,				K_PROPERTY_VALUE_ID,
				[](Control::i_control& control, Control::i_property_interface& property)
				{
					auto& map_download_display = Networking::HTTP::Client::MapDownload::g_map_download_display;

					real value = 0.0f;
					switch(map_download_display.GetMapDownloadState())
					{
					case Networking::HTTP::Client::MapDownload::_map_download_state_downloading:
						value = map_download_display.GetMapProgress();
						break;
					case Networking::HTTP::Client::MapDownload::_map_download_state_extracting:
					case Networking::HTTP::Client::MapDownload::_map_download_state_completed:
						value = 1.0f;
						break;
					case Networking::HTTP::Client::MapDownload::_map_download_state_reconnecting:
						value = 1.0f - (map_download_display.GetReconnectTime() / 3);
						break;
					case Networking::HTTP::Client::MapDownload::_map_download_state_searching:
					case Networking::HTTP::Client::MapDownload::_map_download_state_failed:
						value = 0.0f;
						break;
					}
					property.Set(control, value);
				});

			AddDynamicProperty(K_DOWNLOAD_SOURCE_TITLE_ID,			K_PROPERTY_TEXT_ID,
				[](Control::i_control& control, Control::i_property_interface& property)
				{
					auto& map_download_display = Networking::HTTP::Client::MapDownload::g_map_download_display;

					cstring text = "";
					switch(map_download_display.GetMapDownloadState())
					{
					case Networking::HTTP::Client::MapDownload::_map_download_state_searching:
						text = "Searching";
						break;
					case Networking::HTTP::Client::MapDownload::_map_download_state_downloading:
					case Networking::HTTP::Client::MapDownload::_map_download_state_completed:
					case Networking::HTTP::Client::MapDownload::_map_download_state_reconnecting:
					case Networking::HTTP::Client::MapDownload::_map_download_state_extracting:
						text = map_download_display.GetProviderTitle();
						break;
					case Networking::HTTP::Client::MapDownload::_map_download_state_failed:
						break;
					}

					property.Set(control, text);
				});

			AddDynamicProperty(K_DOWNLOAD_SOURCE_DESCRIPTION_ID,	K_PROPERTY_TEXT_ID,
				[](Control::i_control& control, Control::i_property_interface& property)
				{
					auto& map_download_display = Networking::HTTP::Client::MapDownload::g_map_download_display;

					cstring text = "";
					switch(map_download_display.GetMapDownloadState())
					{
					case Networking::HTTP::Client::MapDownload::_map_download_state_searching:
						text = "Looking for a map download provider...";
						break;
					case Networking::HTTP::Client::MapDownload::_map_download_state_downloading:
					case Networking::HTTP::Client::MapDownload::_map_download_state_completed:
					case Networking::HTTP::Client::MapDownload::_map_download_state_reconnecting:
					case Networking::HTTP::Client::MapDownload::_map_download_state_extracting:
						text = map_download_display.GetProviderDescription();
						break;
					case Networking::HTTP::Client::MapDownload::_map_download_state_failed:
						break;
					}

					property.Set(control, text);
				});
		}

		void c_screen_controller_mapdownload::Show()
		{
			// Reset UI text before showing the control
			ResetScreen();

			c_screen_controller_base::Show();
		}
	};};};
};
#endif