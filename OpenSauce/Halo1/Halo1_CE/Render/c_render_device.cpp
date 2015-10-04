/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Render/c_render_device.hpp"

#if !PLATFORM_IS_DEDI

#include <blamLib/Halo1/rasterizer/dx9/rasterizer_dx9.hpp>
#include <blamlib/Halo1/bitmaps/bitmap_group.hpp>
#include "Rasterizer/DX9/DX9.hpp"

namespace Yelo
{
    namespace Render
    {
        void c_render_device::SetTextureSampler(const datum_index bitmap, const uint32 index, const _enum sampler, const uint32 uv_mode, const uint32 sample_mode)
        {
            auto bitmap_group = TagGroups::TagGetForModify<TagGroups::s_bitmap_group>(bitmap);
            auto bitmap_data = CAST_PTR(TagGroups::s_bitmap_data*, &bitmap_group->bitmaps[index]);
            blam::rasterizer_set_texture_bitmap_data(static_cast<_enum>(sampler), bitmap_data);

            auto& device = *DX9::Direct3DDevice();
            device.SetSamplerState(sampler, D3DSAMP_ADDRESSU, uv_mode);
            device.SetSamplerState(sampler, D3DSAMP_ADDRESSV, uv_mode);
            device.SetSamplerState(sampler, D3DSAMP_MAGFILTER, sample_mode);
            device.SetSamplerState(sampler, D3DSAMP_MINFILTER, sample_mode);
            device.SetSamplerState(sampler, D3DSAMP_MIPFILTER, sample_mode);
        }
    };
};
#endif
