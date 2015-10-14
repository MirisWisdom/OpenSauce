/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/c_shader_instance.hpp"

#if !PLATFORM_IS_DEDI

#include <YeloLib/Halo1/shaders/shader_postprocess_definitions.hpp>

#include "Rasterizer/PostProcessing/c_shader_postprocess.hpp"

namespace Yelo
{
    namespace Rasterizer
    {
        namespace PostProcessing
        {
            void c_shader_instance::ClearMembers()
            {
                ClearNodeData();

                m_members.m_flags.is_valid = false;
                m_members.m_flags.is_active = true;
                m_members.definition = nullptr;
            }

            void c_shader_instance::SetShader(c_shader_postprocess* definition)
            {
                m_members.definition = definition;
            }

            c_shader_postprocess* c_shader_instance::GetShader()
            {
                return m_members.definition;
            }

            bool c_shader_instance::IsValid()
            {
                return m_members.m_flags.is_valid;
            }

            bool& c_shader_instance::IsActive()
            {
                return m_members.m_flags.is_active;
            }

            bool c_shader_instance::UsesGBuffer()
            {
                if (!GetShader() || !GetShader()->GetShaderDefinition<TagGroups::s_shader_postprocess_definition>())
                {
                    return false;
                }
                return GetShader()->GetShaderDefinition<TagGroups::s_shader_postprocess_definition>()->runtime.flags.uses_gbuffer_bit;
            }

            void c_shader_instance::Ctor()
            {
                ClearMembers();
            }

            void c_shader_instance::Dtor()
            {
                ClearMembers();
            }

            void c_shader_instance::Validate()
            {
                m_members.m_flags.is_valid = ValidateImpl();
            }

            bool c_shader_instance::ValidateImpl()
            {
                auto valid = false;
                do
                {
                    if (!m_members.definition)
                    {
                        break;
                    }

                    valid = true;
                } while (false);

                if (valid)
                {
                    valid &= m_members.definition->IsValid();
                }
                return valid;
            }
        }
    }
}
#endif
