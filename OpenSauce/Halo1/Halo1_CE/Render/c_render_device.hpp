/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include <YeloLib/render/i_render_device.hpp>

namespace Yelo
{
    namespace Render
    {
        class c_render_device final : public i_render_device
        {
        public:
            void SetTextureSampler(const datum_index bitmap, const uint32 index, const _enum sampler, const uint32 uv_mode, const uint32 sample_mode) override;
        };

        class c_render_device_singleton : public c_singleton<c_render_device> { };
    };
};
#endif
