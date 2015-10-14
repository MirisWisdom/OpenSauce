/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include <YeloLib/memory/linked_list.hpp>
#include <YeloLib/Halo1/time/interpolation/c_interp_linear.hpp>
#include <YeloLib/render/quad/c_quad_instance.hpp>

namespace Yelo
{
    namespace TagGroups
    {
        struct s_effect_postprocess_quad_definition;
    }

    namespace Rasterizer
    {
        namespace PostProcessing
        {
            class c_effect_postprocess;

            class c_effect_instance : public LinkedListNode<c_effect_instance>
            {
            protected:
                struct
                {
                    struct
                    {
                        bool is_valid;
                        bool is_active;
                        PAD16;
                    } m_flags;

                    c_effect_postprocess* definition;
                    TagGroups::s_effect_postprocess_quad_definition* quad_definition;

                    Render::c_quad_instance* render_quad;

                    struct
                    {
                        real start;
                        real end;
                        real current;

                        Time::Interpolation::c_interp_linear<1> interpolator;
                    } m_fade;
                } m_members;

            private:
                void ClearMembers();

            public:
                virtual void SetEffect(c_effect_postprocess* definition);
                void SetQuadDefinition(TagGroups::s_effect_postprocess_quad_definition* definition);
                bool IsValid();
                real GetCurrentFade();
                int16 GetFadeDirection();
                void SetIsActive(bool active);

                virtual void Ctor();
                virtual void Dtor();

                virtual void SetupEffectInstance() {}

                void Validate();

                HRESULT LoadEffectInstance();
                void UnloadEffectInstance();
            protected:
                virtual bool ValidateImpl();

                virtual bool IsActive();
            public:
                HRESULT Render(IDirect3DDevice9* render_device);

                virtual void UpdateEffectInstance(real delta_time);
                void SetEffectFade(real start, real end, real change_time);
            };
        }
    }
}
#endif
