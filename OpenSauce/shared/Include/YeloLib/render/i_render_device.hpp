/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
    namespace Render
    {
        class i_render_device
        {
        public:
            virtual ~i_render_device() {}

            virtual void SetTextureSampler(const datum_index bitmap, const uint32 index, const _enum sampler, const uint32 uv_mode, const uint32 sample_mode) = 0;
        };
    };
};
