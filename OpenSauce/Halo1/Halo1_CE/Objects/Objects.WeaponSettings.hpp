/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum settings_adjustment_result : long_enum;
	};

	namespace Objects
	{
		namespace Weapon
		{
			void Initialize();
			void Dispose();

#if !PLATFORM_IS_DEDI
			Enums::settings_adjustment_result AdjustSettings();
			void LoadSettings(TiXmlElement* weapons_element);
			void SaveSettings(TiXmlElement* weapons_element);
#endif

#if defined(DX_WRAPPER)
			void Initialize3D(IDirect3DDevice9 *pDevice, D3DPRESENT_PARAMETERS *pPP);
			void OnLostDevice();
			void OnResetDevice(D3DPRESENT_PARAMETERS *pPP);
			void Render();
			void Release();
#elif PLATFORM_IS_USER
			DOC_TODO("Need fallback initialization for when we're not using the DX_WRAPPER")
#endif
		};
	};
};