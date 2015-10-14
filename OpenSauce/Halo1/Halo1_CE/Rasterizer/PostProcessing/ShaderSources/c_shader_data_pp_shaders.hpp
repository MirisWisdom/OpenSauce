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
    namespace Rasterizer
    {
        namespace PostProcessing
        {
            class c_shader_data_pp_shaders : public c_shader_data_base
            {
                char m_shader_id[MAX_PATH];

                void ClearMembers();

            public:
                void SetShaderID(const char* id);

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
