/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include <YeloLib/Halo1/time/interpolation/c_interp_function.hpp>

namespace Yelo
{
    namespace TagGroups
    {
        struct s_shader_postprocess_value_animation_function;
    }

    namespace Rasterizer
    {
        namespace PostProcessing
        {
            template<int ValueCount>
            class c_interp_function_pp_anim final : public Time::Interpolation::c_interp_function<ValueCount>
            {
            protected:
                struct
                {
                    TagGroups::s_shader_postprocess_value_animation_function* animation;
                } m_members_pp_anim;

            public:
                explicit c_interp_function_pp_anim(TagGroups::s_shader_postprocess_value_animation_function* animation);

                void Ctor() override;
                void Dtor() override;

                void Update(real delta_time) override;
            };
        }
    }
}
#endif
