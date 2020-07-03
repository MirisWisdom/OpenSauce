/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

namespace Yelo
{
    namespace Render
    {
        struct s_quad
        {
            point2d tessellation;
            real_bounds x_bounds;
            real_bounds y_bounds;
            int32 vertex_count;
            int32 primitive_count;
            int32 start_vertex;
            int32 start_index;
        };
    }
}
#endif
