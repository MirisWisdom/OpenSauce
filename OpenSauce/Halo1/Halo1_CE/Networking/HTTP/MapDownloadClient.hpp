/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include <YeloLib/configuration/c_configuration_container.hpp>
#include <YeloLib/configuration/c_configuration_value.hpp>
#include <YeloLib/open_sauce/settings/c_settings_singleton.hpp>

namespace Yelo
{
	namespace Networking { namespace HTTP { namespace Client { namespace MapDownload
	{
#pragma region Settings
		class c_settings_container
			: public Configuration::c_configuration_container
		{
		public:
			Configuration::c_configuration_value<bool> m_enabled;

			c_settings_container();
			
		protected:
			const std::vector<i_configuration_value* const> GetMembers() final override;
		};

		class c_settings_mapdownload
			: public Settings::c_settings_singleton<c_settings_container, c_settings_mapdownload>
		{ };
#pragma endregion

		void	AddMapForDownload(const char* map_name);
		void*	RequestCompleted_Callback(const bool download_succeeded, const char* buffer, const GHTTPByteCount buffer_length, void* component_data);
		void*	RequestCancelled_Callback(void* component_data);

		void	Initialize3D(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters);
		void	OnLostDevice();
		void	OnResetDevice(D3DPRESENT_PARAMETERS* pParameters);
		void	Render();
		void	Release();

		void	Initialize();
		void	Dispose();

		void	Update(real delta);
	};};};};
};
#endif