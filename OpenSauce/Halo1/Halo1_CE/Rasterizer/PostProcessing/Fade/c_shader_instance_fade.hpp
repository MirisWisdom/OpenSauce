/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include <Rasterizer/PostProcessing/c_shader_instance.hpp>

namespace Yelo
{
    namespace Render
    {
        class c_quad_instance;
    }

    namespace Rasterizer
    {
        namespace PostProcessing
        {
            namespace Fade
            {
                class c_shader_fade;

                class c_shader_instance_fade : public c_shader_instance
                {
                protected:
                    struct
                    {
                        c_shader_fade* definition;

                        real fade_amount;
                    } m_members_fade;

                public:
                    void SetShader(c_shader_postprocess* definition) override;

                    void Ctor() override;
                    void Dtor() override;

                    void SetShaderInstanceVariables() override;
                    HRESULT Render(IDirect3DDevice9* render_device, Render::c_quad_instance* quad_instance, real fade_amount);
                };
            }
        }
    }
}
#endif
