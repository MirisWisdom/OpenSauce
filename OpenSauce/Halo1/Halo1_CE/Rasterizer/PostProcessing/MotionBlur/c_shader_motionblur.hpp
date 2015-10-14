/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include "Rasterizer/PostProcessing/c_shader_postprocess.hpp"

namespace Yelo
{
    namespace Rasterizer
    {
        namespace PostProcessing
        {
#ifdef EXTERNAL_SUBSYSTEM_SHADERS
            class c_shader_data_external;
#else
            class c_shader_data_pp_shaders;
#endif
            namespace MotionBlur
            {
                struct s_shader_motionblur_definition;

                class c_shader_motionblur : public c_shader_postprocess
                {
                protected:
                    struct
                    {
                        s_shader_motionblur_definition* definition;

#ifdef EXTERNAL_SUBSYSTEM_SHADERS
                        c_shader_data_external* shader_data;
#else
                        c_shader_data_pp_shaders* shader_data;
#endif
                    } m_members_motionblur;

                public:
                    void SetShaderDefinition(TagGroups::s_shader_postprocess_definition* definition) override;

                    void Ctor() override;
                    void Dtor() override;

                    void SetupShader() override;
                protected:
                    void GetHandles() override;
                    void ClearHandles() override;
                    bool ValidateImpl() override;
                };
            }
        }
    }
}
#endif
