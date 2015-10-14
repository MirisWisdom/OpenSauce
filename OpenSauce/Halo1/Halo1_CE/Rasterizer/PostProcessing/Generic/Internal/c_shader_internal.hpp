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
            class c_shader_data_postprocess_definition;

            namespace Generic
            {
                namespace Internal
                {
                    class c_shader_internal : public c_shader_generic
                    {
                    protected:
                        struct
                        {
                            const char* shader_name;
                            c_shader_data_postprocess_definition* shader_source_data;
                            datum_index shader_tag_index;
                        } m_members_internal;

                    public:
                        void SetShaderDefinition(TagGroups::s_shader_postprocess_definition* definition) override;
                        void SetShaderName(const char* name);
                        void SetDatumIndex(datum_index shader_index);
                        datum_index GetDatumIndex();

                        void Ctor() override;
                        void Dtor() override;

                        void SetupShader() override;
                        void SetupBitmapParameters();
                    };
                };
            };
        };
    };
};
#endif
