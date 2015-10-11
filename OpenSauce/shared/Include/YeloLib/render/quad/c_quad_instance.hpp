/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include <YeloLib/render/quad/s_quad.hpp>

namespace Yelo
{
    namespace Render
    {
        class c_quad_instance final
        {
            s_quad m_quad;
            uint32 m_reference_count;

        public:
            c_quad_instance();
            c_quad_instance(const point2d tesselation, const real_bounds& x_bounds, const real_bounds& y_bounds);

            s_quad& Quad();
            uint32 GetRef() const;
            void AddRef();
            void Release();
        };
    }
}
#endif
