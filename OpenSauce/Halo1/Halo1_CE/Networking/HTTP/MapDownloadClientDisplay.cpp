/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Networking/HTTP/MapDownloadClientDisplay.hpp"

#if !PLATFORM_IS_DEDI
namespace Yelo
{
	namespace Networking { namespace HTTP { namespace Client { namespace MapDownload
	{
		c_map_download_display g_map_download_display;

		void c_map_download_display::ResetDisplay()
		{
			m_download_state = _map_download_state_downloading;

			m_part_progress = 0.0f;
			m_map_progress = 0.0f;
			m_reconnect_time = 0.0f;

			m_map_name_string[0] = 0;
			m_provider_title_string[0] = 0;
			m_provider_description_string[0] = 0;
		}

		void c_map_download_display::SetMapName(const char* map_name)
		{
			uint32 string_length = strlen(map_name);
			m_map_name_string[0] = 0;

			if(string_length >= k_display_map_name_string_length)
			{
				string_length = k_display_map_name_string_length - 4;
				strncpy_s(m_map_name_string, map_name, string_length);
				strcat_s(m_map_name_string, "...");
			}
			else
			{
				strcpy_s(m_map_name_string, map_name);
			}
		}

		cstring c_map_download_display::GetMapName()
		{
			return m_map_name_string;
		}

		void c_map_download_display::SetProviderTitle(const char* provider_title)
		{
			uint32 string_length = strlen(provider_title);
			m_provider_title_string[0] = 0;

			if(string_length >= k_display_provider_title_string_length)
			{
				strcpy_s(m_provider_title_string, "ERROR:PROVIDER TITLE STRING TOO LONG");
			}
			else
			{
				strcpy_s(m_provider_title_string, provider_title);
			}
		}

		cstring c_map_download_display::GetProviderTitle()
		{
			return m_provider_title_string;
		}

		void c_map_download_display::SetProviderDescription(const char* provider_description)
		{
			uint32 string_length = strlen(provider_description);
			m_provider_description_string[0] = 0;

			if(string_length >= k_display_provider_description_string_length)
			{
				strcpy_s(m_provider_description_string, "ERROR:PROVIDER DESCRIPTION STRING TOO LONG");
			}
			else
			{
				strcpy_s(m_provider_description_string, provider_description);
			}
		}

		cstring c_map_download_display::GetProviderDescription()
		{
			return m_provider_description_string;
		}

		void c_map_download_display::SetPartProgress(const real percent_complete)
		{
			m_part_progress = percent_complete;
		}

		real c_map_download_display::GetPartProgress()
		{
			return m_part_progress;
		}

		void c_map_download_display::SetMapProgress(const real percent_complete)
		{
			m_map_progress = percent_complete;
		}

		real c_map_download_display::GetMapProgress()
		{
			return m_map_progress;
		}

		void c_map_download_display::SetReconnectTime(const real time_remaining)
		{
			m_reconnect_time = time_remaining;
		}

		real c_map_download_display::GetReconnectTime()
		{
			return m_reconnect_time;
		}

		void c_map_download_display::SetMapDownloadState(const map_download_state state)
		{
			m_download_state = state;
		}

		map_download_state c_map_download_display::GetMapDownloadState()
		{
			return m_download_state;
		}
	};};};};
};
#endif