/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

namespace Yelo
{
	namespace Interface { namespace OpenSauceUI
	{
		/// <summary>	Initializes the OpenSauceUI. </summary>
		void Initialize();

		/// <summary>	Disposes of the OpenSauceUI. </summary>
		void Dispose();

		/// <summary>	Updates the OpenSauceUI. </summary>
		void Update();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Queries if a OpenSauce user interface is available. </summary>
		///
		/// <returns>	true if a user interface is available, false if not. </returns>
		bool IsUIAvailable();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Shows a specific screen. </summary>
		///
		/// <param name="screen_id">	Identifier for the screen. </param>
		void ShowScreen(const uint32 screen_id);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Hides a specific screen. </summary>
		///
		/// <param name="screen_id">	Identifier for the screen. </param>
		void HideScreen(const uint32 screen_id);

		void Initialize3D(IDirect3DDevice9 *pDevice, D3DPRESENT_PARAMETERS *pPP);
		void OnLostDevice();
		void OnResetDevice(D3DPRESENT_PARAMETERS *pPP);
		void Render();
		void Release();
	};};
};
#endif