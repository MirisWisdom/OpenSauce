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
    namespace TagGroups
    {
        struct s_shader_postprocess_parameter;
        struct s_shader_postprocess_generic;
    }

    namespace Rasterizer
    {
        namespace PostProcessing
        {
            namespace Generic
            {
                enum
                {
                    k_max_shader_count = 32,
                    k_max_effect_count = 32,
                    k_variable_count = 16,
                    k_bitmap_count = 4,
                };

                class c_shader_generic : public c_shader_postprocess
                {
                protected:
                    struct
                    {
                        TagGroups::s_shader_postprocess_generic* definition;
                    } m_members_generic;

                public:
                    void SetShaderDefinition(TagGroups::s_shader_postprocess_definition* definition) override;
                    TagGroups::s_shader_postprocess_generic* GetShader();

                    void Ctor() override;
                    void Dtor() override;

                    HRESULT LoadBitmaps(IDirect3DDevice9* render_device);
                    void UnloadBitmaps();
                    void SetupShader() override;
                protected:
                    void GetHandles() override;
                    void ClearHandles() override;
                    bool ValidateImpl() override;
                private:
                    void GetParameterHandle(LPD3DXEFFECT effect, TagGroups::s_shader_postprocess_parameter& parameter);
                    void ClearParameterHandle(TagGroups::s_shader_postprocess_parameter& parameter);
                    void SetParameterVariable(LPD3DXEFFECT effect, TagGroups::s_shader_postprocess_parameter& parameter);

                public:
                    void SetVariables() override;
                };
            }
        }
    }
}
#endif
