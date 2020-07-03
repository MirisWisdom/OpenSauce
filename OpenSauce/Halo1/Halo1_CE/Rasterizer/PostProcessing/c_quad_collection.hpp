/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

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
            class c_quad_collection final
            {
                enum
                {
                    k_maximum_quads_per_axis = 20,
                    k_maxiumum_quad_instance_count = 10
                };

                std::array<Render::c_quad_instance, k_maxiumum_quad_instance_count> m_quad_array;
                IDirect3DVertexBuffer9* m_vertex_buffer;
                IDirect3DIndexBuffer9* m_index_buffer;

            public:
                c_quad_collection();

                bool CreateQuadBuffers(IDirect3DDevice9& render_device, const point2d screen_dimensions);
                void DestroyQuadBuffers();
                Render::c_quad_instance* CreateQuadInstance(const TagGroups::s_effect_postprocess_quad_definition&);
                bool SetRenderDeviceBuffers(IDirect3DDevice9& render_device);

            private:
                Render::c_quad_instance* GetExistingQuad(const point2d tesselation, const real_bounds& x_bounds, const real_bounds& y_bounds);
                Render::c_quad_instance* GetUnusedQuad();
                int32 GetTotalVertexCount();
                int32 GetTotalIndexCount();
            };
        }
    }
}
#endif
