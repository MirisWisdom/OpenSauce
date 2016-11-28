/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

namespace Yelo
{
	namespace Networking { namespace HTTP { namespace Client { namespace MapDownload
	{
		enum map_download_state : _enum
		{
			_map_download_state_searching,
			_map_download_state_downloading,
			_map_download_state_completed,
			_map_download_state_extracting,
			_map_download_state_reconnecting,
			_map_download_state_failed,

			_map_download_state_
		};

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
			map_download_state m_download_state;
			PAD16;

			real m_part_progress;
			real m_map_progress;
			real m_reconnect_time;

			char m_map_name_string[k_display_map_name_string_length];
			char m_provider_title_string[k_display_provider_title_string_length];
			char m_provider_description_string[k_display_provider_description_string_length];

		public:
			void SetMapName(const char* map_name);
			cstring GetMapName();

			void SetProviderTitle(const char* provider_title);
			cstring GetProviderTitle();

			void SetProviderDescription(const char* provider_description);
			cstring GetProviderDescription();

			void SetPartProgress(const real percent_complete);
			real GetPartProgress();

			void SetMapProgress(const real percent_complete);
			real GetMapProgress();
			
			void SetReconnectTime(const real time_remaining);
			real GetReconnectTime();

			void SetMapDownloadState(const map_download_state state);
			map_download_state GetMapDownloadState();

			void ResetDisplay();
		};

		extern c_map_download_display g_map_download_display;
	};};};};
};
#endif