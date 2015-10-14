/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include "Rasterizer/PostProcessing/c_effect_postprocess.hpp"

namespace Yelo
{
    namespace TagGroups
    {
        struct s_effect_postprocess_generic;
    }

    namespace Rasterizer
    {
        namespace PostProcessing
        {
            namespace Generic
            {
                class c_parameter_instance;

                class c_effect_generic : public c_effect_postprocess
                {
                protected:
                    struct
                    {
                        TagGroups::s_effect_postprocess_generic* definition;
                    } m_members_generic;

                public:
                    void SetEffectDefinition(TagGroups::s_effect_postprocess_definition* definition) override;

                    void Ctor() override;
                    void Dtor() override;

                    void SetupEffect() override;
                private:
                    void SetupExposedParameters();
                protected:
                    c_parameter_instance* GetExposedParameter(const char* exposed_name);
                };
            }
        }
    }
}
#endif
