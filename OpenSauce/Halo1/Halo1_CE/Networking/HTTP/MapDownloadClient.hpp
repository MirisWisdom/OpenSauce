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

		void	LoadSettings(TiXmlElement* xml_element);
		void	SaveSettings(TiXmlElement* xml_element);
	};};};};
};
#endif