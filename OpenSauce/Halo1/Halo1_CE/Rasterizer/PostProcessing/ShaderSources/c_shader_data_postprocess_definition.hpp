/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include "Rasterizer/PostProcessing/ShaderSources/c_shader_data_base.hpp"

namespace Yelo
{
    namespace TagGroups
    {
        struct s_shader_postprocess_definition;
    }

    namespace Rasterizer
    {
        namespace PostProcessing
        {
            class c_shader_data_postprocess_definition : public c_shader_data_base
            {
                const char* m_shader_name;
                TagGroups::s_shader_postprocess_definition* m_shader_definition;

                void ClearMembers();

            public:
                void SetShaderName(const char* shader_name);

                void SetShaderDefinition(TagGroups::s_shader_postprocess_definition* definition);

                void Ctor() override;
                void Dtor() override;

                const char* DataSourceID() override;
                const void* GetData(uint32& data_size) override;

                void Dispose() override {}

                const bool IsBinary() override;

                const char* GetIncludePath() override;

                const D3DXMACRO* GetMacros() override;
            };
        }
    }
}
#endif
