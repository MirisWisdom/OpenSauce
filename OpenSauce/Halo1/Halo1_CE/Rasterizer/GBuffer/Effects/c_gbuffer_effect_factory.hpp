/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include <YeloLib/Halo1/files/packed_file.hpp>

namespace Yelo
{
    namespace Rasterizer
    {
        namespace GBuffer
        {
            class c_gbuffer_effect_factory final
            {
                static c_gbuffer_effect_factory m_instance;

            public:
                static c_gbuffer_effect_factory& Get();

            private:
                c_packed_file m_shader_package;

            public:
                void Initialize();

                void Dispose();

                bool CreateEffect(IDirect3DDevice9& device, LPD3DXEFFECT* effect, cstring effect_id);

                static D3DXHANDLE GetGBufferTechnique(ID3DXEffect& effect,
                                               cstring mesh_type,
                                               cstring rt_support);

                static D3DXHANDLE GetGBufferTechnique(ID3DXEffect& effect,
                                               const int32 rt_count,
                                               cstring mesh_type,
                                               cstring rt_support);
            };
        };
    };
};
#endif
