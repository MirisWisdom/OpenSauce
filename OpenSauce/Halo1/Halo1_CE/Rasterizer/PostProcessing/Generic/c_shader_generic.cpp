/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/Generic/c_shader_generic.hpp"

#if !PLATFORM_IS_DEDI

#include <YeloLib/Halo1/shaders/shader_postprocess_generic_definitions.hpp>

namespace Yelo
{
    namespace Rasterizer
    {
        namespace PostProcessing
        {
            namespace Generic
            {
                /////////////////////////////////////////////////
                // member accessors
                void c_shader_generic::SetShaderDefinition(TagGroups::s_shader_postprocess_definition* definition)
                {
                    m_members_generic.definition = CAST_PTR(TagGroups::s_shader_postprocess_generic*, definition);
                    c_shader_postprocess::SetShaderDefinition(definition);
                }

                TagGroups::s_shader_postprocess_generic* c_shader_generic::GetShader()
                {
                    return m_members_generic.definition;
                }

                void c_shader_generic::Ctor()
                {
                    c_shader_postprocess::Ctor();

                    m_members_generic.definition = nullptr;
                }

                void c_shader_generic::Dtor()
                {
                    c_shader_postprocess::Dtor();

                    m_members_generic.definition = nullptr;
                }

                HRESULT c_shader_generic::LoadBitmaps(IDirect3DDevice9* render_device)
                {
                    auto success = S_OK;
                    // checks for whether the parameter is in fact a texture happen in LoadBitmap so just call LoadBitmap on every parameter
                    for (auto i = 0; i < m_members_generic.definition->parameters.Count; i++)
                    {
                        success &= m_members_generic.definition->parameters[i].LoadBitmap(render_device);
                    }
                    return success;
                }

                void c_shader_generic::UnloadBitmaps()
                {
                    for (auto i = 0; i < m_members_generic.definition->parameters.Count; i++)
                    {
                        m_members_generic.definition->parameters[i].ReleaseBitmap();
                    }
                }

                void c_shader_generic::SetupShader()
                {
                    // if the source data is null this function is being called directly
                    YELO_ASSERT_DISPLAY(m_members.source_data != nullptr, "c_shader_generic setup function does not create the source data, it must be created in a derived class");

                    c_shader_postprocess::SetupShader();
                }

                /// <summary>Gets the shader handles.</summary>
                void c_shader_generic::GetHandles()
                {
                    c_shader_postprocess::GetHandles();

                    for (auto i = 0; i < m_members_generic.definition->parameters.Count; i++)
                    {
                        GetParameterHandle(m_members.definition->runtime.dx_effect, m_members_generic.definition->parameters[i]);
                    }
                }

                /// <summary>Clears the shaders handles.</summary>
                void c_shader_generic::ClearHandles()
                {
                    c_shader_postprocess::ClearHandles();

                    for (auto i = 0; i < m_members_generic.definition->parameters.Count; i++)
                    {
                        ClearParameterHandle(m_members_generic.definition->parameters[i]);
                    }
                }

                ////////////////////////////////////////////////////////////////////////////////////////////////////
                /// <summary>Validates the shader.</summary>
                ///
                /// <returns>true if it succeeds, false if it fails.</returns>
                bool c_shader_generic::ValidateImpl()
                {
                    if (!c_shader_postprocess::ValidateImpl())
                    {
                        return false;
                    }

                    auto success = true;
                    for (auto i = 0; i < m_members_generic.definition->parameters.Count; i++)
                    {
                        success &= m_members_generic.definition->parameters[i].value.base.handle.IsUsed();
                    }

                    return success;
                }

                void c_shader_generic::GetParameterHandle(LPD3DXEFFECT effect, TagGroups::s_shader_postprocess_parameter& parameter)
                {
                    parameter.value.base.handle.Initialize(effect, parameter.value_name, false);
                }

                void c_shader_generic::ClearParameterHandle(TagGroups::s_shader_postprocess_parameter& parameter)
                {
                    parameter.value.base.handle.ClearHandles();
                }

                void c_shader_generic::SetParameterVariable(LPD3DXEFFECT effect, TagGroups::s_shader_postprocess_parameter& parameter)
                {
                    // TODO: grab a (const?) reference to parameter.value and use it instead of parameter.value. directly
                    switch (parameter.value_type.type)
                    {
                        case Enums::_shader_variable_base_type_boolean:
                            parameter.value.boolean.handle.SetVariable(effect, &parameter.value.boolean.enabled);
                            break;
                        case Enums::_shader_variable_base_type_integer:
                            parameter.value.integer32.handle.SetVariable(effect, &parameter.value.integer32.upper_bound);
                            break;
                        case Enums::_shader_variable_base_type_float:
                        {
                            switch (parameter.value_type.count)
                            {
                                case 1:
                                    parameter.value.real32.handle.SetVariable(effect, &parameter.value.real32.upper_bound);
                                    break;
                                case 2:
                                    parameter.value.vector2d.handle.SetVariable(effect, &parameter.value.vector2d.upper_bound);
                                    break;
                                case 3:
                                    parameter.value.vector3d.handle.SetVariable(effect, &parameter.value.vector3d.upper_bound);
                                    break;
                                case 4:
                                    parameter.value.vector4d.handle.SetVariable(effect, &parameter.value.vector4d.upper_bound);
                                    break;
                            }
                        }
                            break;
                        case Enums::_shader_variable_base_type_argb_color:
                            parameter.value.color4d.handle.SetVariable(effect, &parameter.value.color4d.upper_bound, false);
                            break;
                        case Enums::_shader_variable_base_type_texture:
                            parameter.value.bitmap.handle.SetVariable(effect, parameter.GetTexture());
                            break;
                    }
                }

                /// <summary>Sets the variables.</summary>
                void c_shader_generic::SetVariables()
                {
                    c_shader_postprocess::SetVariables();

                    // TODO: should be able to use a ranged for loop here on ->parameters
                    for (auto i = 0; i < m_members_generic.definition->parameters.Count; i++)
                    {
                        SetParameterVariable(m_members.definition->runtime.dx_effect, m_members_generic.definition->parameters[i]);
                    }
                }
            }
        }
    }
}
#endif
