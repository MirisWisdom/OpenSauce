/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/render/quad/c_quad_renderer.hpp>

#if !PLATFORM_IS_DEDI
#include <YeloLib/render/quad/s_quad.hpp>

namespace Yelo
{
    namespace Render
    {
        void c_quad_renderer::Draw(IDirect3DDevice9& render_device, i_quad_pass_handler& pass_handler, ID3DXEffect& effect, const s_quad& quad)
        {
            UINT pass_count;
            effect.Begin(&pass_count, 0);
            for (auto pass = 0; pass < pass_count; ++pass)
            {
                pass_handler.PrePass();
                effect.BeginPass(pass);
                {
                    render_device.DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
                                                       quad.start_vertex,
                                                       0,
                                                       quad.vertex_count,
                                                       quad.start_index,
                                                       quad.primitive_count);
                };
                effect.EndPass();
                pass_handler.PostPass();
            }
            effect.End();
        }
    }
}
#endif
