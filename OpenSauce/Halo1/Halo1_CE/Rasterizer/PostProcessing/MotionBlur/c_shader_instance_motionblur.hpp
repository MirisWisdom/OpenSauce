/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include <YeloLib/Halo1/time/interpolation/c_interp_linear.hpp>

#include "Rasterizer/PostProcessing/c_shader_instance.hpp"

namespace Yelo
{
    namespace Rasterizer
    {
        namespace PostProcessing
        {
            class c_shader_postprocess;

            namespace MotionBlur
            {
                class c_shader_motionblur;

                class c_shader_instance_motionblur : public c_shader_instance
                {
                protected:
                    struct
                    {
                        struct
                        {
                            bool vignette_enabled_prev;
                            bool vignette_enabled;
                            PAD16;
                        } m_flags;

                        c_shader_motionblur* definition;

                        real blur_amount;

                        real start_vignette;
                        real target_vignette;
                        Time::Interpolation::c_interp_linear<1> vignette_interp;
                    } m_members_motionblur;

                public:
                    void SetShader(c_shader_postprocess* shader) override;
                    real& BlurAmount();

                    void Ctor() override;
                    void Dtor() override;

                    void UpdateShaderInstance(real delta_time) override;
                    void SetShaderInstanceVariables() override;
                };
            }
        }
    }
}
#endif
