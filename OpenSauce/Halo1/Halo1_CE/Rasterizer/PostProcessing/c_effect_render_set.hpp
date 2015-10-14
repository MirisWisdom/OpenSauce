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
        namespace PostProcessing
        {
            class c_effect_instance;

            class c_effect_render_set
            {
                c_effect_instance** m_effect_list;

            public:
                void Ctor();
                void Dtor();

                void SetCount(uint32 effect_count);
                void SetEffectInstance(uint32 index, c_effect_instance* effect);
                bool RenderSet(IDirect3DDevice9* render_device);
            };
        }
    }
}
#endif
