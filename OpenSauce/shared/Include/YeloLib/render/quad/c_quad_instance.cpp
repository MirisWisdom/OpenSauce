/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/render/quad/c_quad_instance.hpp>

#if !PLATFORM_IS_DEDI

namespace Yelo
{
    namespace Render
    {
        c_quad_instance::c_quad_instance(const point2d tesselation, const real_bounds& x_bounds, const real_bounds& y_bounds)
            : m_quad(),
              m_reference_count(0)
        {
            m_quad.tessellation = tesselation;
            m_quad.x_bounds = x_bounds;
            m_quad.y_bounds = y_bounds;
            m_quad.vertex_count = (m_quad.tessellation.x + 1) * (m_quad.tessellation.y + 1);
            m_quad.primitive_count = (m_quad.tessellation.x * m_quad.tessellation.y) * 2;
            m_quad.start_vertex = 0;
            m_quad.start_index = 0;
            m_reference_count = 0;
        }

        c_quad_instance::c_quad_instance() : c_quad_instance({ 0, 0 }, { 0, 0 }, { 0, 0 }) { }

        s_quad& c_quad_instance::Quad()
        {
            return m_quad;
        }

        uint32 c_quad_instance::GetRef() const
        {
            return m_reference_count;
        }

        void c_quad_instance::AddRef()
        {
            m_reference_count++;
        }

        void c_quad_instance::Release()
        {
            m_reference_count--;
        }

#ifdef API_DEBUG
#define YELO_UNIT_TEST_QUAD_INSTANCE(name) YELO_UNIT_TEST(name, Yelo.Rasterizer.PostProcessing.c_quad_instance)

        YELO_UNIT_TEST_QUAD_INSTANCE(Ctor_WithQuadDimensions_InitializesQuad)
        {
            auto quad_instance = c_quad_instance({ 1, 2 }, { 0.25, 0.75 }, { 0.25, 0.75 });

            auto& result = quad_instance.Quad();

            AssertThat(result.tessellation, Equals(point2d { 1, 2 }));
            AssertThat(result.x_bounds, Equals(real_bounds { 0.25, 0.75 }));
            AssertThat(result.y_bounds, Equals(real_bounds { 0.25, 0.75 }));
            AssertThat(result.vertex_count, Equals(6));
            AssertThat(result.primitive_count, Equals(4));
            AssertThat(result.start_vertex, Equals(0));
            AssertThat(result.start_index, Equals(0));
        }

        YELO_UNIT_TEST_QUAD_INSTANCE(GetRef_AfterAddRef_ReturnsIncreasedReferenceCount)
        {
            auto quad = c_quad_instance(point2d(), real_bounds(), real_bounds());

            quad.AddRef();
            auto result = quad.GetRef();

            AssertThat(result, Equals(1));
        }

        YELO_UNIT_TEST_QUAD_INSTANCE(GetRef_AfterRelease_ReturnsReducedReferenceCount)
        {
            auto quad = c_quad_instance(point2d(), real_bounds(), real_bounds());

            quad.AddRef();
            AssertThat(quad.GetRef(), Equals(1));
            quad.Release();
            auto result = quad.GetRef();

            AssertThat(result, Equals(0));
        }

        YELO_UNIT_TEST_QUAD_INSTANCE(Release_WhenReferencesIsZero_DoesNotDecrementReferenceCount)
        {
            auto quad = c_quad_instance(point2d(), real_bounds(), real_bounds());

            quad.Release();
            auto result = quad.GetRef();

            AssertThat(result, Equals(0));
        }

#undef YELO_UNIT_TEST_QUAD_INSTANCE
#endif
    }
}
#endif
