/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include "Rasterizer/PostProcessing/c_shader_instance.hpp"

namespace Yelo
{
    namespace Rasterizer
    {
        namespace PostProcessing
        {
            namespace Generic
            {
                class c_shader_generic;
                class c_parameter_instance;

                class c_shader_instance_generic : public c_shader_instance
                {
                    struct
                    {
                        c_shader_generic* shader;

                        uint32 parameter_count;
                        c_parameter_instance* parameters;
                    } m_members_generic;

                public:
                    void SetShader(c_shader_postprocess* shader) override;

                    void Ctor() override;
                    void Dtor() override;

                    void SetupShaderInstance() override;
                    c_parameter_instance* GetParameterInstance(const char* name);
                private:
                    void CreateParameterInstances();
                    void DestroyParameterInstances();

                public:
                    void UpdateShaderInstance(real delta_time) override;
                    void SetShaderInstanceVariables() override;
                };
            }
        }
    }
}
#endif
