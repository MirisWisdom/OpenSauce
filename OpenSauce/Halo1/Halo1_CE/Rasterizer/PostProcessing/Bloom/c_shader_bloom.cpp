/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/Bloom/c_shader_bloom.hpp"

#if !PLATFORM_IS_DEDI

#include "Rasterizer/PostProcessing/Bloom/s_shader_bloom_definition.hpp"

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
            namespace Bloom
            {
#ifdef EXTERNAL_SUBSYSTEM_SHADERS
#define SUBSYSTEM_BLOOM
#include "Rasterizer/PostProcessing/PostProcessing_ExternalSubsystemPaths.inl"
#undef SUBSYSTEM_BLOOM
#endif

                ////////////////////////////////////////////////////////////////////////////////////////////////////
                /// <summary>Sets the shader definition.</summary>
                ///
                /// <param name="definition">[in] The shader definition.</param>
                void c_shader_bloom::SetShaderDefinition(TagGroups::s_shader_postprocess_definition* definition)
                {
                    m_members_bloom.definition = CAST_PTR(s_shader_bloom_definition*, definition);
                    c_shader_postprocess::SetShaderDefinition(definition);
                }

                void c_shader_bloom::Ctor()
                {
                    c_shader_postprocess::Ctor();

                    m_members_bloom.definition = nullptr;
                    m_members_bloom.shader_data = nullptr;
                }

                void c_shader_bloom::Dtor()
                {
                    c_shader_postprocess::Dtor();

                    m_members_bloom.definition = nullptr;

                    m_members_bloom.shader_data->Dtor();
                    delete m_members_bloom.shader_data;
                    m_members_bloom.shader_data = nullptr;
                }

                ////////////////////////////////////////////////////////////////////////////////////////////////////
                /// <summary>Sets up the shader.</summary>
                ///
                /// <remarks>Sets up the shader classes shader data object. When EXTERNAL_SUBSYSTEM_SHADERS is
                ///     define the shader data object will be a c_shader_data_external, otherwise it will be a
                ///     c_shader_data_pp_shaders.</remarks>
                void c_shader_bloom::SetupShader()
                {
                    YELO_ASSERT_DISPLAY(m_members_bloom.shader_data == nullptr, "Shader data object has not been deleted and NULLed before being set again.");

#ifdef EXTERNAL_SUBSYSTEM_SHADERS
                    m_members_bloom.shader_data = new c_shader_data_external();
                    m_members_bloom.shader_data->Ctor();
                    m_members_bloom.shader_data->SetFilePath(g_external_subsystem_shader_path, g_external_subsystem_include_path);
#else
                    m_members_bloom.shader_data = new c_shader_data_pp_shaders();
                    m_members_bloom.shader_data->Ctor();
                    m_members_bloom.shader_data->SetShaderID("PP_Bloom");
#endif
                    SetSourceData(m_members_bloom.shader_data);
                }

                /// <summary>Gets the bloom parameter handles.</summary>
                void c_shader_bloom::GetHandles()
                {
                    c_shader_postprocess::GetHandles();

                    m_members_bloom.definition->size_handle.Initialize(m_members.definition->runtime.dx_effect, "c_bloom_size");
                    m_members_bloom.definition->exposure_handle.Initialize(m_members.definition->runtime.dx_effect, "c_bloom_exposure");
                    m_members_bloom.definition->mix_amount_handle.Initialize(m_members.definition->runtime.dx_effect, "c_bloom_mix_amount");
                    m_members_bloom.definition->minimum_color_handle.Initialize(m_members.definition->runtime.dx_effect, "c_bloom_minimum_color");
                    m_members_bloom.definition->maximum_color_handle.Initialize(m_members.definition->runtime.dx_effect, "c_bloom_maximum_color");
                }

                /// <summary>Clears the bloom parameter handles.</summary>
                void c_shader_bloom::ClearHandles()
                {
                    c_shader_postprocess::ClearHandles();

                    m_members_bloom.definition->size_handle.ClearHandles();
                    m_members_bloom.definition->exposure_handle.ClearHandles();
                    m_members_bloom.definition->mix_amount_handle.ClearHandles();
                    m_members_bloom.definition->minimum_color_handle.ClearHandles();
                    m_members_bloom.definition->maximum_color_handle.ClearHandles();
                }

                ////////////////////////////////////////////////////////////////////////////////////////////////////
                /// <summary>Validates the implementation.</summary>
                ///
                /// <returns>true if it succeeds, false if it fails.</returns>
                bool c_shader_bloom::ValidateImpl()
                {
                    if (!c_shader_postprocess::ValidateImpl())
                    {
                        return false;
                    }

                    auto valid = false;
                    do
                    {
                        if (!m_members_bloom.definition->size_handle.IsUsed())
                        {
                            break;
                        }
                        if (!m_members_bloom.definition->exposure_handle.IsUsed())
                        {
                            break;
                        }
                        if (!m_members_bloom.definition->mix_amount_handle.IsUsed())
                        {
                            break;
                        }
                        if (!m_members_bloom.definition->minimum_color_handle.IsUsed())
                        {
                            break;
                        }
                        if (!m_members_bloom.definition->maximum_color_handle.IsUsed())
                        {
                            break;
                        }

                        valid = true;
                    } while (false);

                    return valid;
                }
            }
        }
    }
}
#endif
