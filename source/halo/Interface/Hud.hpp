/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include <Game/Camera.hpp>

namespace Yelo
{
	namespace Hud
	{
		void Initialize();
		void Dispose();
		void Update();

		bool& ShowHUD();
		bool& ScaleHUD();
		bool& ShowCrosshair();
		real& HUDScaleY();
		real& HUDScaleX();
		void* HudGlobals();
		void UpdateChanges();

		Enums::settings_adjustment_result AdjustSettings();

		void EnableWidgetScale();
		void EnableHUDScale();
		void DisableScale();

		void Initialize3D(IDirect3DDevice9 *pDevice, D3DPRESENT_PARAMETERS *pPP);
		void OnLostDevice();
		void OnResetDevice(D3DPRESENT_PARAMETERS *pPP);
		void Render();
		void Release();
	}
}
#endif