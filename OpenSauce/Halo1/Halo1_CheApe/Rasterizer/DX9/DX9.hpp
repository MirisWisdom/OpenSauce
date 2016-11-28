/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

#ifndef PLATFORM_NO_DX9
namespace Yelo
{
	namespace Rasterizer { namespace DX9 {
		void Initialize();
		void Dispose();

		// D3D Present Properties
		D3DPRESENT_PARAMETERS* D3DPresetParams();

		// D3D Properties
		IDirect3D9* Direct3D9();

		// D3D Device Properties
		IDirect3DDevice9* Direct3DDevice();
	}; };
};
#endif