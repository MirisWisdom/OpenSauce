/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include <Rasterizer/PostProcessing/c_effect_instance.hpp>

namespace Yelo
{
    namespace Enums
    {
        enum effect_activation_state : _enum;
        enum postprocess_render_stage : _enum;
    }

    namespace TagGroups
    {
        struct s_effect_postprocess_effect_activation_control;
        struct s_effect_postprocess_generic_effect_instance;
    };

    namespace Rasterizer
    {
        namespace PostProcessing
        {
            namespace Generic
            {
                class c_effect_instance_generic : public c_effect_instance
                {
                protected:
                    struct
                    {
                        TagGroups::s_effect_postprocess_generic_effect_instance* definition;
                    } m_members_generic;

                public:
                    void SetEffectInstanceDefinition(TagGroups::s_effect_postprocess_generic_effect_instance* definition);
                    const char* GetName();
                    Enums::postprocess_render_stage GetRenderStage();

                    void Ctor() override;
                    void Dtor() override;

                    void SetupEffectInstance() override;
                    bool GetInitiallyActive();

                    bool IsActive() override;
                    TagGroups::s_effect_postprocess_effect_activation_control* FindActivationControl(Enums::effect_activation_state state);
                    bool EvaluateActivationControls();
                    bool GetActivationValue(TagGroups::s_effect_postprocess_effect_activation_control& control);
                };
            };
        };
    }
};
#endif
