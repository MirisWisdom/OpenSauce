/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/Fade/c_shader_fade.hpp"

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
            namespace Fade
            {
#ifdef EXTERNAL_SUBSYSTEM_SHADERS
#define SUBSYSTEM_FADE
#include "Rasterizer/PostProcessing/PostProcessing_ExternalSubsystemPaths.inl"
#undef SUBSYSTEM_FADE
#endif
                ////////////////////////////////////////////////////////////////////////////////////////////////////
                /// <summary>Sets the shader definition.</summary>
                ///
                /// <param name="definition">[in,out] If non-null, the definition.</param>
                void c_shader_fade::SetShaderDefinition(TagGroups::s_shader_postprocess_definition* definition)
                {
                    m_members_fade.definition = CAST_PTR(s_shader_fade_definition*, definition);
                    c_shader_postprocess::SetShaderDefinition(definition);
                }

                void c_shader_fade::Ctor()
                {
                    c_shader_postprocess::Ctor();

                    m_members_fade.definition = nullptr;
                    m_members_fade.shader_data = nullptr;
                }

                void c_shader_fade::Dtor()
                {
                    c_shader_postprocess::Dtor();

                    m_members_fade.definition = nullptr;
                    m_members_fade.shader_data->Dtor();
                    delete m_members_fade.shader_data;
                    m_members_fade.shader_data = nullptr;
                }

                /// <summary>Sets up the shader.</summary>
                void c_shader_fade::SetupShader()
                {
                    YELO_ASSERT_DISPLAY(m_members_fade.shader_data == nullptr, "Shader data object has not been deleted and NULLed before being set again.");

#ifdef EXTERNAL_SUBSYSTEM_SHADERS
                    m_members_fade.shader_data = new c_shader_data_external();
                    m_members_fade.shader_data->Ctor();
                    m_members_fade.shader_data->SetFilePath(g_external_subsystem_shader_path, g_external_subsystem_include_path);
#else
                    m_members_fade.shader_data = new c_shader_data_pp_shaders();
                    m_members_fade.shader_data->Ctor();
                    m_members_fade.shader_data->SetShaderID("PP_EffectFade");
#endif
                    SetSourceData(m_members_fade.shader_data);
                }

                /// <summary>Gets the parameter handles.</summary>
                void c_shader_fade::GetHandles()
                {
                    c_shader_postprocess::GetHandles();

                    m_members_fade.definition->fade_amount_handle.Initialize(m_members.definition->runtime.dx_effect, "c_fade_amount");
                }

                /// <summary>Clears the parameter handles.</summary>
                void c_shader_fade::ClearHandles()
                {
                    c_shader_postprocess::ClearHandles();

                    m_members_fade.definition->fade_amount_handle.ClearHandles();
                }

                ////////////////////////////////////////////////////////////////////////////////////////////////////
                /// <summary>Validates the implementation.</summary>
                ///
                /// <returns>true if it succeeds, false if it fails.</returns>
                bool c_shader_fade::ValidateImpl()
                {
                    if (!c_shader_postprocess::ValidateImpl())
                    {
                        return false;
                    }

                    auto valid = false;
                    do
                    {
                        if (!m_members_fade.definition->fade_amount_handle.IsUsed())
                        {
                            break;
                        }

                        valid = true;
                    } while (false);

                    return valid;
                }
            };
        };
    };
};
#endif
