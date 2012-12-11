/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include "Interface/TextBlock.hpp"

namespace Yelo
{
	namespace Networking { namespace HTTP { namespace Client { namespace MapDownload
	{
		enum
		{
			k_display_title_string_length = 13, // Map Download/0
			k_display_map_name_string_length = 32,
			k_display_progress_string_length = 32,// Connecting to server(0)/0
			k_display_cancel_string_length = 21,// Press [esc] to cancel/0
			k_display_provider_title_string_length = 64,// Press [esc] to cancel/0
			k_display_provider_description_string_length = 256,// Press [esc] to cancel/0
		};

		class c_map_download_display
		{
			// background
			TextBlock* m_background_tint;
			// container
			TextBlock* m_base_container;
			// inner container
			TextBlock* m_inner_container;
			// title
			TextBlock* m_title;
			// map name
			TextBlock* m_map_name;
			// part progress bar back
			TextBlock* m_part_progress_back;
			// part progress bar front
			TextBlock* m_part_progress_front;
			// part progress bar text
			TextBlock* m_part_progress_text;
			// map progress bar back
			TextBlock* m_map_progress_back;
			// map progress bar front
			TextBlock* m_map_progress_front;
			// map progress bar text
			TextBlock* m_map_progress_text;
			// cancel text
			TextBlock* m_cancel;
			// provider title
			TextBlock* m_provider_title;
			// provider text
			TextBlock* m_provider_description;

			bool m_map_extracting;
			bool m_reconnecting;
			bool m_failed;
			PAD8;

			real m_part_progress;
			real m_map_progress;
			real m_reconnect_time;

			wchar_t* m_title_string;
			wchar_t* m_cancel_string;

			wchar_t m_map_name_string[k_display_map_name_string_length];
			wchar_t m_part_progress_string[k_display_progress_string_length];
			wchar_t m_map_progress_string[k_display_progress_string_length];
			wchar_t m_provider_title_string[k_display_provider_title_string_length];
			wchar_t m_provider_description_string[k_display_provider_description_string_length];


			void SetupDisplay(D3DPRESENT_PARAMETERS* present_parameters);
		public:
			void SetMapName(const char* map_name);
			void SetProviderTitle(const char* provider_title);
			void SetProviderDescription(const char* provider_description);
			void SetPartProgress(const real percent_complete);
			void SetMapProgress(const real percent_complete);
			void SetMapExtracting(const bool is_extracting);
			void SetReconnecting(const bool is_reconnecting);
			void SetReconnectTime(const real time_remaining);
			void SetFailed(const bool is_failed);

			void ResetDisplay();
			void Initialize(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* present_parameters);
			void Dispose();
			void DeviceLost();
			void DeviceReset(D3DPRESENT_PARAMETERS* present_parameters);
			void Update();
			void Render();
		};

		extern c_map_download_display g_map_download_display;
	};};};};
};
#endif