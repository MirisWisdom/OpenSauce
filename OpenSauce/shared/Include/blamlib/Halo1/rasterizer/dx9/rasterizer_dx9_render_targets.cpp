/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#if !PLATFORM_IS_EDITOR
#include <blamlib/Halo1/rasterizer/dx9/rasterizer_dx9_render_targets.hpp>

namespace Yelo
{
	namespace Render
	{
		bool s_render_target::IsEnabled() const { return surface && texture; }

		HRESULT s_render_target::CreateTarget(IDirect3DDevice9* device, uint32 rt_width, uint32 rt_height, D3DFORMAT rt_format)
		{
			width = rt_width;
			height = rt_height;
			format = rt_format;
			HRESULT hr = device->CreateTexture(						
				width,
				height,
				1,
				D3DUSAGE_RENDERTARGET,
				format,
				D3DPOOL_DEFAULT,
				&texture,
				nullptr);

			if(SUCCEEDED(hr))
				hr = texture->GetSurfaceLevel(0, &surface);

			if(FAILED(hr))
				this->ReleaseTarget();
			return hr;
		}

		void s_render_target::ReleaseTarget()
		{
			Yelo::safe_release(surface);
			Yelo::safe_release(texture);
		}

		void s_render_target::ClearTarget(IDirect3DDevice9* device, D3DCOLOR color, DWORD flags)
		{
			if(!IsEnabled())	return;
			
			device->SetRenderTarget(0, surface);
			device->Clear( 0L, nullptr, flags, color, 1.0f, 0L );
		}
	};
};

#endif