/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/Generic/c_shader_instance_generic.hpp"

#if !PLATFORM_IS_DEDI

#include <YeloLib/Halo1/shaders/shader_postprocess_generic_definitions.hpp>

#include "Rasterizer/PostProcessing/Generic/c_parameter_instance.hpp"
#include "Rasterizer/PostProcessing/Generic/c_shader_generic.hpp"

namespace Yelo
{
    namespace Rasterizer
    {
        namespace PostProcessing
        {
            namespace Generic
            {
                void c_shader_instance_generic::SetShader(c_shader_postprocess* shader)
                {
                    m_members_generic.shader = CAST_PTR(c_shader_generic*, shader);
                    c_shader_instance::SetShader(shader);
                }

                void c_shader_instance_generic::Ctor()
                {
                    c_shader_instance::Ctor();

                    m_members_generic.shader = nullptr;
                    m_members_generic.parameter_count = 0;
                    m_members_generic.parameters = nullptr;
                }

                void c_shader_instance_generic::Dtor()
                {
                    c_shader_instance::Dtor();

                    DestroyParameterInstances();
                    m_members_generic.shader = nullptr;
                }

                /// <summary>Sets up the shader instance.</summary>
                void c_shader_instance_generic::SetupShaderInstance()
                {
                    c_shader_instance::SetupShaderInstance();

                    CreateParameterInstances();
                }

                c_parameter_instance* c_shader_instance_generic::GetParameterInstance(const char* name)
                {
                    c_parameter_instance* instance = nullptr;

                    uint32 index = 0;
                    while (index < m_members_generic.parameter_count && !instance)
                    {
                        auto parameter = m_members_generic.parameters[index].GetParameter();

                        YELO_ASSERT_DISPLAY(parameter != nullptr, "parameter instance has no assigned parameter");

                        // compare the parameter instances name with the requested name
                        if (strcmp(parameter->value_name, name) == 0)
                        {
                            instance = &m_members_generic.parameters[index];
                        }

                        index++;
                    }
                    return instance;
                }

                /// <summary>Creates parameter instances.</summary>
                void c_shader_instance_generic::CreateParameterInstances()
                {
                    auto definition = m_members_generic.shader->GetShader();

                    // if the shader has no parameters no further set up is needed
                    if (definition->parameters.Count == 0)
                    {
                        return;
                    }

                    // create the parameter instances
                    m_members_generic.parameter_count = definition->parameters.Count;
                    m_members_generic.parameters = new c_parameter_instance[m_members_generic.parameter_count];

                    for (uint32 i = 0; i < m_members_generic.parameter_count; i++)
                    {
                        // initialise each parameter instance and run its set up logic
                        m_members_generic.parameters[i].Ctor();
                        m_members_generic.parameters[i].SetParameter(&definition->parameters[i]);
                        m_members_generic.parameters[i].SetupParameterInstance();
                    }
                }
                
                /// <summary>Destroys the parameter instances.</summary>
                void c_shader_instance_generic::DestroyParameterInstances()
                {
                    // if there are no parameters no further action is needed
                    if (m_members_generic.parameter_count == 0)
                    {
                        return;
                    }

                    // run the destruction logic on each instance to delete memory and reset to defaults
                    for (uint32 i = 0; i < m_members_generic.parameter_count; i++)
                    {
                        m_members_generic.parameters[i].Dtor();
                    }

                    // delete the parameter instance array
                    delete [] m_members_generic.parameters;
                    m_members_generic.parameters = nullptr;

                    m_members_generic.parameter_count = 0;
                }

                ////////////////////////////////////////////////////////////////////////////////////////////////////
                /// <summary>Updates the shader instance described by delta_time.</summary>
                ///
                /// <param name="delta_time">The delta time.</param>
                void c_shader_instance_generic::UpdateShaderInstance(real delta_time)
                {
                    for (uint32 i = 0; i < m_members_generic.parameter_count; i++)
                    {
                        m_members_generic.parameters[i].Update(delta_time);
                    }
                }

                /// <summary>Sets shader instance variables.</summary>
                void c_shader_instance_generic::SetShaderInstanceVariables()
                {
                    for (uint32 i = 0; i < m_members_generic.parameter_count; i++)
                    {
                        m_members_generic.parameters[i].SetParameterVariable(m_members_generic.shader->GetEffect());
                    }
                }
            }
        }
    }
}
#endif
