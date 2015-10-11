/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/FXAA/c_shader_fxaa.hpp"

#if !PLATFORM_IS_DEDI

#ifdef EXTERNAL_SUBSYSTEM_SHADERS
#include "Rasterizer/PostProcessing/ShaderSources/c_shader_data_external.hpp"
#else
#include "Rasterizer/PostProcessing/ShaderSources/c_shader_data_pp_shaders.hpp"
#endif

namespace Yelo
{
    namespace Rasterizer
    {
        namespace PostProcessing
        {
            namespace FXAA
            {
#ifdef EXTERNAL_SUBSYSTEM_SHADERS
#define SUBSYSTEM_FXAA
#include "Rasterizer/PostProcessing/PostProcessing_ExternalSubsystemPaths.inl"
#undef SUBSYSTEM_FXAA
#endif
                void c_shader_fxaa::Ctor()
                {
                    c_shader_postprocess::Ctor();

                    m_members_fxaa.shader_data = nullptr;
                }

                void c_shader_fxaa::Dtor()
                {
                    c_shader_postprocess::Dtor();

                    m_members_fxaa.shader_data->Dtor();
                    delete m_members_fxaa.shader_data;
                    m_members_fxaa.shader_data = nullptr;
                }

                /// <summary>Sets up the shader.</summary>
                void c_shader_fxaa::SetupShader()
                {
                    YELO_ASSERT_DISPLAY(m_members_fxaa.shader_data == nullptr, "Shader data object has not been deleted and NULLed before being set again.");

#ifdef EXTERNAL_SUBSYSTEM_SHADERS
                    m_members_fxaa.shader_data = new c_shader_data_external();
                    m_members_fxaa.shader_data->Ctor();
                    m_members_fxaa.shader_data->SetFilePath(g_external_subsystem_shader_path, g_external_subsystem_include_path);
#else
                    m_members_fxaa.shader_data = new c_shader_data_pp_shaders();
                    m_members_fxaa.shader_data->Ctor();
                    m_members_fxaa.shader_data->SetShaderID("PP_FXAA");
#endif
                    SetSourceData(m_members_fxaa.shader_data);
                }
            }
        }
    }
}
#endif
