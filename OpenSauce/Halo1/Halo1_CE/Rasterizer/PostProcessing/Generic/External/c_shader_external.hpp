/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include "Rasterizer/PostProcessing/Generic/c_shader_generic.hpp"

namespace Yelo
{
    namespace Rasterizer
    {
        namespace PostProcessing
        {
            class c_shader_data_external;

            namespace Generic
            {
                namespace External
                {
                    class c_shader_external : public c_shader_generic
                    {
                    protected:
                        struct
                        {
                            tag_string shader_id;
                            char shader_path[MAX_PATH];
                            char include_path[MAX_PATH];
                            c_shader_data_external* shader_source_data;
                        } m_members_external;

                    public:
                        void SetShaderPath(const char* path);
                        void SetShaderID(const char* id);
                        const char* GetShaderID();

                        void Ctor() override;
                        void Dtor() override;

                        void SetupShader();
                    };
                }
            }
        }
    }
}
#endif
