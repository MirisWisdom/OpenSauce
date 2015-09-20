/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

namespace Yelo
{
    namespace Rasterizer
    {
        namespace GBuffer
        {
            class c_gbuffer;

            void Initialize();
            void Dispose();

            void Initialize3D(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params);
            void OnLostDevice();
            void OnResetDevice(D3DPRESENT_PARAMETERS* params);
            void Render();
            void Release();

            void Update(real delta_time);
            void Clear(IDirect3DDevice9* device);
            void SetWorldViewProjection(const D3DMATRIX& matrix);

            void SkyPreProcess();
            void SkyPostProcess();
            void ObjectsPreProcess();
            void ObjectsPostProcess();
            void StructurePreProcess();
            void StructurePostProcess();
            void UIPreProcess();

            bool& OutputTbn();
            bool Available();
            bool& Enabled();
            void SetRendered(const bool rendered);
            c_gbuffer& GetGBuffer();
            int16& DebugIndex();
        };
    };
};
#endif
