/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/ShaderSources/c_shader_data_postprocess_definition.hpp"

#if !PLATFORM_IS_DEDI

#include <YeloLib/Halo1/shaders/shader_postprocess_definitions.hpp>

namespace Yelo
{
    namespace Rasterizer
    {
        namespace PostProcessing
        {
            void c_shader_data_postprocess_definition::ClearMembers()
            {
                // TODO: null m_shader_name?

                m_shader_definition = nullptr;
            }

            void c_shader_data_postprocess_definition::SetShaderName(const char* shader_name)
            {
                m_shader_name = shader_name;
            }

            void c_shader_data_postprocess_definition::SetShaderDefinition(TagGroups::s_shader_postprocess_definition* definition)
            {
                m_shader_definition = definition;
            }

            void c_shader_data_postprocess_definition::Ctor()
            {
                ClearMembers();
            }

            void c_shader_data_postprocess_definition::Dtor()
            {
                ClearMembers();
            }

            const char* c_shader_data_postprocess_definition::DataSourceID()
            {
                uint32 name_length = strlen(m_shader_name) + 1;
                if (name_length > k_data_source_id_max_length)
                {
                    return &m_shader_name[name_length - k_data_source_id_max_length];
                }
                return m_shader_name;
            }

            const void* c_shader_data_postprocess_definition::GetData(uint32& data_size)
            {
                data_size = m_shader_definition->shader_code_binary.size;
                return m_shader_definition->shader_code_binary.address;
            }

            const bool c_shader_data_postprocess_definition::IsBinary()
            {
                return true;
            }

            const char* c_shader_data_postprocess_definition::GetIncludePath()
            {
                return nullptr;
            }

            const D3DXMACRO* c_shader_data_postprocess_definition::GetMacros()
            {
                return nullptr;
            }
        }
    }
}
#endif
