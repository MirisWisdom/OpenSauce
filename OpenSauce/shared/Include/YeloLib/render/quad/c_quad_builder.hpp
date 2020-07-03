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
        struct s_quad;

        class c_quad_builder
        {
        public:
            // commented out texcoords are for vertex data which may or may not be used to pass custom values in future
            static const uint32 k_vertex_format =
            D3DFVF_XYZ | D3DFVF_TEX4 |
                         D3DFVF_TEXCOORDSIZE4(0) |
                         D3DFVF_TEXCOORDSIZE4(1) |
                         D3DFVF_TEXCOORDSIZE4(2) |
                         D3DFVF_TEXCOORDSIZE4(3)/* |
                         D3DFVF_TEXCOORDSIZE2(4) |
                         D3DFVF_TEXCOORDSIZE2(5) |
                         D3DFVF_TEXCOORDSIZE2(6) |
                         D3DFVF_TEXCOORDSIZE2(7)*/;

            struct s_face
            {
                int16 m_indices[3];

                friend bool operator==(const s_face& lhs, const s_face& rhs)
                {
                    return lhs.m_indices[0] == rhs.m_indices[0] &&
                        lhs.m_indices[1] == rhs.m_indices[1] &&
                        lhs.m_indices[2] == rhs.m_indices[2];
                }

                friend bool operator!=(const s_face& lhs, const s_face& rhs)
                {
                    return !(lhs == rhs);
                }
            };

            struct s_vertex
            {
                real_point3d m_position;
                real_quaternion m_quad_space_coords;
                real_quaternion m_screen_space_coords;
                real_quaternion m_quad_bounds;
                real_quaternion m_unused;

                friend bool operator==(const s_vertex& lhs, const s_vertex& rhs)
                {
                    return lhs.m_position == rhs.m_position
                        && lhs.m_quad_space_coords == rhs.m_quad_space_coords
                        && lhs.m_screen_space_coords == rhs.m_screen_space_coords
                        && lhs.m_quad_bounds == rhs.m_quad_bounds
                        && lhs.m_unused == rhs.m_unused;
                }

                friend bool operator!=(const s_vertex& lhs, const s_vertex& rhs)
                {
                    return !(lhs == rhs);
                }
            };

            static void BuildVertices(s_vertex*& buffer_pointer, const s_quad& quad, const point2d dimensions);
            static void BuildIndices(s_face*& buffer_pointer, const s_quad& quad);
        };
    }
}
#endif
