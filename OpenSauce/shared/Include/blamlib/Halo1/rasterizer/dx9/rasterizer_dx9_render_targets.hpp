/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <d3d9.h>

namespace Yelo
{
	// yeah, I fucked up, this should have stayed in the Rasterizer namespace!
	namespace Render
	{
		struct s_render_target
		{
			uint32 width, height;
			D3DFORMAT format;

			IDirect3DSurface9* surface;
			IDirect3DTexture9* texture;

			// returns whether the texture and surface are valid
			bool		IsEnabled() const;
			// Create the render target texture and surface. If either of those fails [enabled] will be false
			HRESULT		CreateTarget(IDirect3DDevice9* device, uint32 rt_width, uint32 rt_height, D3DFORMAT rt_format);
			// Release the texture and surface
			void		ReleaseTarget();
			// Set the device target to [surface] then clear it
			void		ClearTarget(IDirect3DDevice9* device, D3DCOLOR color = 0x00000000, DWORD flags = D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL);
		};
		s_render_target* GlobalRenderTargets(); // defined in the project implementation
	};
};