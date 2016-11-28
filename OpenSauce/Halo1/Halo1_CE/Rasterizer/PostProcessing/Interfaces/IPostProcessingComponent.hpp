/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

namespace Yelo
{
	namespace Rasterizer { namespace PostProcessing
	{
		interface IPostProcessingComponent
			abstract
		{
			virtual void Initialize() {}
			virtual void Dispose() {}

			virtual void InitializeResources_Base(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* parameters) {}
			virtual void OnLostDevice_Base() {}
			virtual void OnResetDevice_Base(D3DPRESENT_PARAMETERS* parameters) {}
			virtual void ReleaseResources_Base() {}

			virtual bool IsReady() { return false; }
			virtual bool IsUnloaded() { return true; }

			virtual void Unload() {}
			virtual void Load() {}
		};
	};};
};
#endif