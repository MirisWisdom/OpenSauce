/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/c_quad_collection.hpp"

#if !PLATFORM_IS_DEDI

#include <YeloLib/Halo1/shaders/effect_postprocess_collection_definitions.hpp>
#include <YeloLib/render/quad/c_quad_builder.hpp>

namespace Yelo
{
    namespace Rasterizer
    {
        namespace PostProcessing
        {
            c_quad_collection::c_quad_collection()
                : m_quad_array(),
                  m_vertex_buffer(nullptr),
                  m_index_buffer(nullptr) { }

            bool c_quad_collection::CreateQuadBuffers(IDirect3DDevice9& render_device, const point2d screen_dimensions)
            {
                auto vertex_count = GetTotalVertexCount();
                auto index_count = GetTotalIndexCount();

                if (!vertex_count || !index_count)
                {
                    return true;
                }

                HRESULT result;
                result = render_device.CreateVertexBuffer(
                    vertex_count * sizeof(Render::c_quad_builder::s_vertex),
                    D3DUSAGE_WRITEONLY,
                    Render::c_quad_builder::k_vertex_format,
                    D3DPOOL_DEFAULT,
                    &m_vertex_buffer,
                    nullptr);
                YELO_ASSERT_DISPLAY(SUCCEEDED(result), "failed to create the post processing vertex buffer");
                if (FAILED(result))
                {
                    return false;
                }

                result = render_device.CreateIndexBuffer(
                    index_count * sizeof(WORD),
                    D3DUSAGE_WRITEONLY,
                    D3DFMT_INDEX16,
                    D3DPOOL_DEFAULT,
                    &m_index_buffer,
                    nullptr);
                YELO_ASSERT_DISPLAY(SUCCEEDED(result), "failed to create the post processing index buffer");
                if (FAILED(result))
                {
                    return false;
                }

                Render::c_quad_builder::s_vertex* vertices;
                result = m_vertex_buffer->Lock(0, 0, CAST_PTR(void**, &vertices), 0);
                if (FAILED(result))
                {
                    return false;
                }

                Render::c_quad_builder::s_face* indicies;
                result = m_index_buffer->Lock(0, 0, CAST_PTR(void**, &indicies), 0);
                if (FAILED(result))
                {
                    return false;
                }

                int32 vertex_offset = 0;
                int32 index_offset = 0;
                for (auto& quad_instance : m_quad_array)
                {
                    auto& quad = quad_instance.Quad();
                    Render::c_quad_builder::BuildVertices(vertices, quad, screen_dimensions);
                    Render::c_quad_builder::BuildIndices(indicies, quad);

                    // the start vertex and index variables indicate where in the data the render device should access
                    // to draw the quad
                    // this is so that we only need one vertex and index buffer to hold all of the quads
                    quad.start_vertex = vertex_offset;
                    quad.start_index = index_offset;

                    vertex_offset += quad.vertex_count;
                    index_offset += quad.primitive_count * 3;
                }

                m_vertex_buffer->Unlock();
                m_index_buffer->Unlock();
                return true;
            }

            void c_quad_collection::DestroyQuadBuffers()
            {
                safe_release(m_vertex_buffer);
                safe_release(m_index_buffer);
            }

            Render::c_quad_instance* c_quad_collection::CreateQuadInstance(const TagGroups::s_effect_postprocess_quad_definition& quad_definition)
            {
                point2d tesselation_final;
                tesselation_final.x = __max(quad_definition.tessellation.x, 1);
                tesselation_final.x = __min(tesselation_final.x, k_maximum_quads_per_axis);
                tesselation_final.y = __max(quad_definition.tessellation.y, 1);
                tesselation_final.y = __min(tesselation_final.y, k_maximum_quads_per_axis);

                auto quad_instance = GetExistingQuad(tesselation_final, quad_definition.x_bounds, quad_definition.y_bounds);
                if (!quad_instance)
                {
                    quad_instance = GetUnusedQuad();
                    if (!quad_instance)
                    {
                        LOG(Error, "Quad collection has run out of available quad instances");
                        return nullptr;
                    }
                    *quad_instance = Render::c_quad_instance(tesselation_final, quad_definition.x_bounds, quad_definition.y_bounds);
                }
                quad_instance->AddRef();
                return quad_instance;
            }

            bool c_quad_collection::SetRenderDeviceBuffers(IDirect3DDevice9& render_device)
            {
                auto result = render_device.SetStreamSource(0, m_vertex_buffer, 0, sizeof(Render::c_quad_builder::s_vertex));
                result |= render_device.SetIndices(m_index_buffer);
                result |= render_device.SetFVF(Render::c_quad_builder::k_vertex_format);
                return SUCCEEDED(result);
            }

            Render::c_quad_instance* c_quad_collection::GetExistingQuad(const point2d tesselation, const real_bounds& x_bounds, const real_bounds& y_bounds)
            {
                for (auto& quad_instance : m_quad_array)
                {
                    const auto& quad = quad_instance.Quad();
                    if (
                        quad.tessellation.x == tesselation.x &&
                        quad.tessellation.y == tesselation.y &&
                        quad.x_bounds.lower == x_bounds.lower &&
                        quad.x_bounds.upper == x_bounds.upper &&
                        quad.y_bounds.lower == y_bounds.lower &&
                        quad.y_bounds.upper == y_bounds.upper)
                    {
                        return &quad_instance;
                    }
                }
                return nullptr;
            }

            Render::c_quad_instance* c_quad_collection::GetUnusedQuad()
            {
                for (auto& quad_instance : m_quad_array)
                {
                    if (!quad_instance.GetRef())
                    {
                        return &quad_instance;
                    }
                }
                return nullptr;
            }

            int32 c_quad_collection::GetTotalVertexCount()
            {
                int32 count = 0;
                for (auto& quad_instance : m_quad_array)
                {
                    count += quad_instance.Quad().vertex_count;
                }
                return count;
            }

            int32 c_quad_collection::GetTotalIndexCount()
            {
                int32 count = 0;
                for (auto& quad_instance : m_quad_array)
                {
                    count += quad_instance.Quad().primitive_count * 3;
                }
                return count;
            }
        };
    };
};
#endif
