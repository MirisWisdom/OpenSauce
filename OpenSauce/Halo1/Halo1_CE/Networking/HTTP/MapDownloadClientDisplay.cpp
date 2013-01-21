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
			m_map_extracting = false;
			m_reconnecting = false;
			m_failed = false;

			m_part_progress = 0.0f;
			m_map_progress = 0.0f;
			m_reconnect_time = 0.0f;

			m_title_string = L"Map Download";
			m_cancel_string = L"Press [esc] to cancel";

			m_map_name_string[0] = 0;
			m_part_progress_string[0] = 0;
			m_map_progress_string[0] = 0;
			m_provider_title_string[0] = 0;
			m_provider_description_string[0] = 0;
		}

		void c_map_download_display::SetupDisplay(D3DPRESENT_PARAMETERS* present_parameters)
		{
			// full screen black tint
			m_background_tint->SetIgnoreTextHeight(true);
			m_background_tint->SetDimensions(present_parameters->BackBufferWidth, present_parameters->BackBufferHeight);
			m_background_tint->SetBackColor(0xCC000000);
			m_background_tint->Attach(Enums::_attach_method_top_left, 0, 0, 0, 0);

			// opaque background for the whole display
			m_base_container->SetIgnoreTextHeight(true);
			m_base_container->SetDimensions(517, 244);
			m_base_container->SetBackColor(0xFF333333);
			m_base_container->Attach(Enums::_attach_method_center, 0, 0, 0, 0);

			// inner background for the whole display
			m_inner_container->SetIgnoreTextHeight(true);
			m_inner_container->SetDimensions(512, 240);
			m_inner_container->SetBackColor(0xFF555555);
			m_inner_container->Attach(Enums::_attach_method_center, 0, 0, 0, 0);

			// title bar for the display title
			m_title->SetIgnoreTextHeight(true);
			m_title->SetDimensions(512, 32);
			m_title->SetBackColor(0xFF777777);
			m_title->Attach(Enums::_attach_method_center, 0, 0, 0, -104);

			m_title->SetFont("Lucida Sans Unicode", 24, FW_NORMAL, false, 6);
			m_title->SetText(m_title_string);
			m_title->SetTextAlign(DT_CENTER | DT_VCENTER);
			m_title->SetTextColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.75f));

			// text block for the map name
			m_map_name->SetIgnoreTextHeight(true);
			m_map_name->SetDimensions(512, 32);
			m_map_name->SetBackColor(0x00000000);
			m_map_name->Attach(Enums::_attach_method_center, 0, 0, 0, -72);

			m_map_name->SetFont("Lucida Sans Unicode", 20, FW_NORMAL, false, 6);
			m_map_name->SetText(m_map_name_string);
			m_map_name->SetTextAlign(DT_CENTER | DT_VCENTER);
			m_map_name->SetTextColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.75f));

			// text block for the part progress bar back
			m_part_progress_back->SetIgnoreTextHeight(true);
			m_part_progress_back->SetDimensions(508, 26);
			m_part_progress_back->SetBackColor(0xFF444444);
			m_part_progress_back->Attach(Enums::_attach_method_center, 0, 0, 0, -41);

			// text block for the part progress bar front
			m_part_progress_front->SetIgnoreTextHeight(true);
			m_part_progress_front->SetDimensions(0, 26);
			m_part_progress_front->SetBackColor(0xFF666666);
			m_part_progress_front->Attach(Enums::_attach_method_center, 0, 0, 0, -41);

			// text block for the part progress bar text
			m_part_progress_text->SetIgnoreTextHeight(true);
			m_part_progress_text->SetDimensions(508, 26);
			m_part_progress_text->SetBackColor(0x00000000);
			m_part_progress_text->Attach(Enums::_attach_method_center, 0, 0, 0, -41);

			m_part_progress_text->SetFont("Lucida Sans Unicode", 16, FW_NORMAL, false, 6);
			m_part_progress_text->SetText(m_part_progress_string);
			m_part_progress_text->SetTextAlign(DT_CENTER | DT_VCENTER);
			m_part_progress_text->SetTextColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.75f));

			// text block for the map progress bar back
			m_map_progress_back->SetIgnoreTextHeight(true);
			m_map_progress_back->SetDimensions(508, 26);
			m_map_progress_back->SetBackColor(0xFF444444);
			m_map_progress_back->Attach(Enums::_attach_method_center, 0, 0, 0, -15);

			// text block for the map progress bar front
			m_map_progress_front->SetIgnoreTextHeight(true);
			m_map_progress_front->SetDimensions(0, 26);
			m_map_progress_front->SetBackColor(0xFF666666);
			m_map_progress_front->Attach(Enums::_attach_method_center, 0, 0, 0, -15);

			// text block for the map progress bar text
			m_map_progress_text->SetIgnoreTextHeight(true);
			m_map_progress_text->SetDimensions(508, 26);
			m_map_progress_text->SetBackColor(0x00000000);
			m_map_progress_text->Attach(Enums::_attach_method_center, 0, 0, 0, -15);

			m_map_progress_text->SetFont("Lucida Sans Unicode", 16, FW_NORMAL, false, 6);
			m_map_progress_text->SetText(m_map_progress_string);
			m_map_progress_text->SetTextAlign(DT_CENTER | DT_VCENTER);
			m_map_progress_text->SetTextColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.75f));

			// text block for the cancel text
			m_cancel->SetIgnoreTextHeight(true);
			m_cancel->SetDimensions(512, 24);
			m_cancel->SetBackColor(0xFF888888);
			m_cancel->Attach(Enums::_attach_method_center, 0, 0, 0, 12);

			m_cancel->SetFont("Lucida Sans Unicode", 16, FW_NORMAL, false, 6);
			m_cancel->SetText(m_cancel_string);
			m_cancel->SetTextAlign(DT_CENTER | DT_VCENTER);
			m_cancel->SetTextColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.75f));

			// text block for the provider title
			m_provider_title->SetIgnoreTextHeight(true);
			m_provider_title->SetDimensions(512, 32);
			m_provider_title->SetBackColor(0xFF666666);
			m_provider_title->Attach(Enums::_attach_method_center, 0, 0, 0, 40);

			m_provider_title->SetFont("Lucida Sans Unicode", 18, FW_NORMAL, false, 6);
			m_provider_title->SetText(m_provider_title_string);
			m_provider_title->SetTextAlign(DT_CENTER | DT_VCENTER);
			m_provider_title->SetTextColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.75f));

			// text block for the provider description
			m_provider_description->SetIgnoreTextHeight(true);
			m_provider_description->SetDimensions(512, 64);
			m_provider_description->SetBackColor(0xFF666666);
			m_provider_description->Attach(Enums::_attach_method_center, 0, 0, 0, 88);

			m_provider_description->SetFont("Lucida Sans Unicode", 15, FW_NORMAL, false, 6);
			m_provider_description->SetText(m_provider_description_string);
			m_provider_description->SetTextAlign(DT_CENTER | DT_VCENTER | DT_WORDBREAK);
			m_provider_description->SetTextColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.75f));


			m_background_tint->Refresh();
			m_base_container->Refresh();
			m_inner_container->Refresh();
			m_title->Refresh();
			m_map_name->Refresh();
			m_part_progress_back->Refresh();
			m_part_progress_front->Refresh();
			m_part_progress_text->Refresh();
			m_map_progress_back->Refresh();
			m_map_progress_front->Refresh();
			m_map_progress_text->Refresh();
			m_cancel->Refresh();
			m_provider_title->Refresh();
			m_provider_description->Refresh();
		}

		void c_map_download_display::SetMapName(const char* map_name)
		{
			uint32 string_length = strlen(map_name);
			m_map_name_string[0] = 0;

			if(string_length >= k_display_map_name_string_length)
			{
				string_length = k_display_map_name_string_length - 4;
				wchar_t map_name_wide[MAX_PATH];
				swprintf_s(&map_name_wide[0], MAX_PATH, L"%S", map_name);

				wcsncpy_s(&m_map_name_string[0], k_display_map_name_string_length, map_name_wide, string_length);
				wcscat_s(&m_map_name_string[0], k_display_map_name_string_length, L"...");
			}
			else
			{
				swprintf_s(&m_map_name_string[0], k_display_map_name_string_length, L"%S", map_name);
			}
		}

		void c_map_download_display::SetProviderTitle(const char* provider_title)
		{
			uint32 string_length = strlen(provider_title);
			m_provider_title_string[0] = 0;

			if(string_length >= k_display_provider_title_string_length)
			{
				wcscat_s(&m_provider_title_string[0], k_display_provider_title_string_length, L"ERROR:PROVIDER TITLE STRING TOO LONG");
			}
			else
			{
				swprintf_s(&m_provider_title_string[0], k_display_provider_title_string_length, L"%S", provider_title);
			}
		}

		void c_map_download_display::SetProviderDescription(const char* provider_description)
		{
			uint32 string_length = strlen(provider_description);
			m_provider_description_string[0] = 0;

			if(string_length >= k_display_provider_description_string_length)
			{
				wcscat_s(&m_provider_description_string[0], k_display_provider_description_string_length, L"ERROR:PROVIDER DESCRIPTION STRING TOO LONG");
			}
			else
			{
				swprintf_s(&m_provider_description_string[0], k_display_provider_description_string_length, L"%S", provider_description);
			}
		}

		void c_map_download_display::SetPartProgress(const real percent_complete) { m_part_progress = percent_complete; }
		void c_map_download_display::SetMapProgress(const real percent_complete) { m_map_progress = percent_complete; }
		void c_map_download_display::SetMapExtracting(const bool is_extracting) { m_map_extracting = is_extracting; }
		void c_map_download_display::SetReconnecting(const bool is_reconnecting) { m_reconnecting = is_reconnecting; }
		void c_map_download_display::SetReconnectTime(const real time_remaining) { m_reconnect_time = time_remaining; }
		void c_map_download_display::SetFailed(const bool is_failed) { m_failed = is_failed; }

		void c_map_download_display::Initialize(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* present_parameters)
		{
			m_background_tint = new TextBlock(device, present_parameters);
			m_base_container = new TextBlock(device, present_parameters);
			m_inner_container = new TextBlock(device, present_parameters);
			m_title = new TextBlock(device, present_parameters);
			m_map_name = new TextBlock(device, present_parameters);
			m_part_progress_back = new TextBlock(device, present_parameters);
			m_part_progress_front = new TextBlock(device, present_parameters);
			m_part_progress_text = new TextBlock(device, present_parameters);
			m_map_progress_back = new TextBlock(device, present_parameters);
			m_map_progress_front = new TextBlock(device, present_parameters);
			m_map_progress_text = new TextBlock(device, present_parameters);
			m_cancel = new TextBlock(device, present_parameters);
			m_provider_title = new TextBlock(device, present_parameters);
			m_provider_description = new TextBlock(device, present_parameters);

			ResetDisplay();
			SetupDisplay(present_parameters);
		}

		void c_map_download_display::Dispose()
		{
			m_background_tint->Release();
			m_base_container->Release();
			m_inner_container->Release();
			m_title->Release();
			m_map_name->Release();
			m_part_progress_back->Release();
			m_part_progress_front->Release();
			m_part_progress_text->Release();
			m_map_progress_back->Release();
			m_map_progress_front->Release();
			m_map_progress_text->Release();
			m_cancel->Release();
			m_provider_title->Release();
			m_provider_description->Release();

			delete m_background_tint; m_background_tint = NULL;
			delete m_base_container; m_base_container = NULL;
			delete m_inner_container; m_inner_container = NULL;
			delete m_title; m_title = NULL;
			delete m_map_name; m_map_name = NULL;
			delete m_part_progress_back; m_part_progress_back = NULL;
			delete m_part_progress_front; m_part_progress_front = NULL;
			delete m_part_progress_text; m_part_progress_text = NULL;
			delete m_map_progress_back; m_map_progress_back = NULL;
			delete m_map_progress_front; m_map_progress_front = NULL;
			delete m_map_progress_text; m_map_progress_text = NULL;
			delete m_cancel; m_cancel = NULL;
			delete m_provider_title; m_provider_title = NULL;
			delete m_provider_description; m_provider_description = NULL;
		}

		void c_map_download_display::DeviceLost()
		{
			m_background_tint->OnLostDevice();
			m_base_container->OnLostDevice();
			m_inner_container->OnLostDevice();
			m_title->OnLostDevice();
			m_map_name->OnLostDevice();
			m_part_progress_back->OnLostDevice();
			m_part_progress_front->OnLostDevice();
			m_part_progress_text->OnLostDevice();
			m_map_progress_back->OnLostDevice();
			m_map_progress_front->OnLostDevice();
			m_map_progress_text->OnLostDevice();
			m_cancel->OnLostDevice();
			m_provider_title->OnLostDevice();
			m_provider_description->OnLostDevice();
		}

		void c_map_download_display::DeviceReset(D3DPRESENT_PARAMETERS* present_parameters)
		{
			m_background_tint->OnResetDevice(present_parameters);
			m_base_container->OnResetDevice(present_parameters);
			m_inner_container->OnResetDevice(present_parameters);
			m_title->OnResetDevice(present_parameters);
			m_map_name->OnResetDevice(present_parameters);
			m_part_progress_back->OnResetDevice(present_parameters);
			m_part_progress_front->OnResetDevice(present_parameters);
			m_part_progress_text->OnResetDevice(present_parameters);
			m_map_progress_back->OnResetDevice(present_parameters);
			m_map_progress_front->OnResetDevice(present_parameters);
			m_map_progress_text->OnResetDevice(present_parameters);
			m_cancel->OnResetDevice(present_parameters);
			m_provider_title->OnResetDevice(present_parameters);
			m_provider_description->OnResetDevice(present_parameters);

			SetupDisplay(present_parameters);
		}

		void c_map_download_display::Update()
		{
			m_part_progress_string[0] = 0;
			if(m_map_extracting)
			{
				wcscpy_s(&m_part_progress_string[0], k_display_progress_string_length, L"Extracting map...");
				m_part_progress_front->SetDimensions(512, 26);
			}
			else if(m_reconnecting)
			{
				swprintf_s(m_part_progress_string, k_display_progress_string_length, L"Connecting to server (%1.f)", m_reconnect_time + 1);
				m_part_progress_front->SetDimensions(512, 26);
			}
			else if(m_failed)
			{
				m_part_progress_string[0] = 0;
				wcscpy_s(&m_part_progress_string[0], k_display_progress_string_length, L"Download failed");
				m_part_progress_front->SetDimensions(0, 26);
			}
			else
			{
				swprintf_s(m_part_progress_string, k_display_progress_string_length, L"%3.f%%", m_part_progress * 100);
				m_part_progress_front->SetDimensions((uint32)(m_part_progress * 512), 26);
			}
			m_part_progress_front->Refresh();

			swprintf_s(m_map_progress_string, k_display_progress_string_length, L"%3.f%%", m_map_progress * 100);
			m_map_progress_front->SetDimensions((uint32)(m_map_progress * 512), 26);
			m_map_progress_front->Refresh();
		}

		void c_map_download_display::Render()
		{
			m_background_tint->Render();
			m_base_container->Render();
			m_inner_container->Render();
			m_title->Render(m_title_string);
			m_map_name->Render(m_map_name_string);
			m_part_progress_back->Render();
			m_part_progress_front->Render();
			m_part_progress_text->Render(m_part_progress_string);
			m_map_progress_back->Render();
			m_map_progress_front->Render();
			m_map_progress_text->Render(m_map_progress_string);
			m_cancel->Render(m_cancel_string);
			m_provider_title->Render(m_provider_title_string);
			m_provider_description->Render(m_provider_description_string);
		}
	};};};};
};
#endif