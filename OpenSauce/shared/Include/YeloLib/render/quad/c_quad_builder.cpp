/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/render/quad/c_quad_builder.hpp>

#if !PLATFORM_IS_DEDI

#include <YeloLib/render/quad/s_quad.hpp>

#ifdef API_DEBUG
#include <YeloLib/render/quad/c_quad_instance.hpp>
#endif

namespace Yelo
{
    namespace Render
    {
        void c_quad_builder::BuildVertices(s_vertex*& buffer_pointer, const s_quad& quad, const point2d dimensions)
        {
            auto x_offset = quad.x_bounds.lower;
            auto x_range = quad.x_bounds.upper - quad.x_bounds.lower;
            auto x_increment = x_range / quad.tessellation.x;

            auto y_offset = quad.y_bounds.lower;
            auto y_range = quad.y_bounds.upper - quad.y_bounds.lower;
            auto y_increment = y_range / quad.tessellation.y;

            auto x_pos_offset = CAST(real, dimensions.x) * x_offset;
            auto x_pos_increment = CAST(real, dimensions.x) * x_increment;

            auto y_pos_offset = CAST(real, dimensions.y) * y_offset;
            auto y_pos_increment = CAST(real, dimensions.y) * y_increment;

            auto u_tex_increment = 1.0f / quad.tessellation.x;
            auto v_tex_increment = 1.0f / quad.tessellation.y;

            for (auto y = 0; y < quad.tessellation.y + 1; y++)
            {
                for (auto x = 0; x < quad.tessellation.x + 1; x++)
                {
                    // set the 3D position
                    // y is negative because screen position is from the top left (0,0) to bottom right (1,-1)
                    // z is an arbitrary number as its irrelevant in an orthographic projection
                    buffer_pointer->m_position = { x_pos_offset + x * x_pos_increment - 0.5f, -(y_pos_offset + y * y_pos_increment - 0.5f), 10.0f };
                    buffer_pointer->m_quad_space_coords = { x * u_tex_increment, y * v_tex_increment, 0, 0 };
                    buffer_pointer->m_screen_space_coords = { x_offset + x * x_increment, y_offset + y * y_increment,0,0 };
                    buffer_pointer->m_quad_bounds = { quad.x_bounds.lower, quad.x_bounds.upper, quad.y_bounds.lower, quad.y_bounds.upper };
                    buffer_pointer->m_unused = { 0, 0, 0, 0 };

                    buffer_pointer++;
                }
            }
        }

        void c_quad_builder::BuildIndices(s_face*& buffer_pointer, const s_quad& quad)
        {
            // creates a two dimensional array containing the indices of each vertex point
            auto counter = 0;
            std::vector<std::vector<int16>> index_array(quad.tessellation.y + 1, std::vector<int16>(quad.tessellation.x + 1));
            for (auto y = 0; y < quad.tessellation.y + 1; y++)
            {
                for (auto x = 0; x < quad.tessellation.x + 1; x++)
                {
                    index_array[y][x] = counter;
                    counter++;
                }
            }

            // loop through tess x and tess y setting the indices according to the index table
            for (auto y = 0; y < quad.tessellation.y; y++)
            {
                for (auto x = 0; x < quad.tessellation.x; x++)
                {
                    // the quad faces will always be:
                    /*
                        face 1

                        (x + 0, y + 0)------------(x + 1, y + 0)
                            |					 _______/
                            |			 _______/
                            |	 _______/
                            |	/
                        (x + 0, y + 1)

                        face 2
                                                  (x + 1, y + 0)
                                                    _______/	|
                                            _______/			|
                                    _______/					|
                                /							|
                        (x + 0, y + 1)------------(x + 1, y + 1)
                        */

                    buffer_pointer->m_indices[0] = index_array[y + 1][x + 0];
                    buffer_pointer->m_indices[1] = index_array[y + 0][x + 0];
                    buffer_pointer->m_indices[2] = index_array[y + 0][x + 1];
                    buffer_pointer++;

                    buffer_pointer->m_indices[0] = index_array[y + 0][x + 1];
                    buffer_pointer->m_indices[1] = index_array[y + 1][x + 1];
                    buffer_pointer->m_indices[2] = index_array[y + 1][x + 0];
                    buffer_pointer++;
                }
            }
        }

#ifdef API_DEBUG
#define YELO_UNIT_TEST_QUAD_BUILDER(name) YELO_UNIT_TEST(name, Yelo.Rasterizer.PostProcessing.c_quad_builder)

        YELO_UNIT_TEST_QUAD_BUILDER(BuildVertices_WithNullStartValue_Throws)
        {
            c_quad_builder::s_vertex* vertex_pointer = nullptr;

            AssertThrows(std::exception, c_quad_builder::BuildVertices(vertex_pointer, s_quad(), {1, 1}));
        }

        YELO_UNIT_TEST_QUAD_BUILDER(BuildVertices_WithQuad_CalculatesQuadVertices)
        {
            c_quad_builder::s_vertex vertex_array[6];
            auto quad_instance = c_quad_instance({ 2, 1 }, { 0.25, 0.75 }, { 0.25, 0.75 });

            auto vertex_pointer = &vertex_array[0];
            c_quad_builder::BuildVertices(vertex_pointer, quad_instance.Quad(), { 100, 100 });

            AssertThat(vertex_array[0], Equals(c_quad_builder::s_vertex { { 24.5, -24.5, 10 }, { 0.0, 0.0, 0, 0 }, { 0.25, 0.25, 0, 0 }, { 0.25, 0.75, 0.25, 0.75 }, { 0, 0, 0, 0 } }));
            AssertThat(vertex_array[1], Equals(c_quad_builder::s_vertex { { 49.5, -24.5, 10 }, { 0.5, 0.0, 0, 0 }, { 0.5 , 0.25, 0, 0 }, { 0.25, 0.75, 0.25, 0.75 }, { 0, 0, 0, 0 } }));
            AssertThat(vertex_array[2], Equals(c_quad_builder::s_vertex { { 74.5, -24.5, 10 }, { 1.0, 0.0, 0, 0 }, { 0.75, 0.25, 0, 0 }, { 0.25, 0.75, 0.25, 0.75 }, { 0, 0, 0, 0 } }));
            AssertThat(vertex_array[3], Equals(c_quad_builder::s_vertex { { 24.5, -74.5, 10 }, { 0.0, 1.0, 0, 0 }, { 0.25, 0.75, 0, 0 }, { 0.25, 0.75, 0.25, 0.75 }, { 0, 0, 0, 0 } }));
            AssertThat(vertex_array[4], Equals(c_quad_builder::s_vertex { { 49.5, -74.5, 10 }, { 0.5, 1.0, 0, 0 }, { 0.5 , 0.75, 0, 0 }, { 0.25, 0.75, 0.25, 0.75 }, { 0, 0, 0, 0 } }));
            AssertThat(vertex_array[5], Equals(c_quad_builder::s_vertex { { 74.5, -74.5, 10 }, { 1.0, 1.0, 0, 0 }, { 0.75, 0.75, 0, 0 }, { 0.25, 0.75, 0.25, 0.75 }, { 0, 0, 0, 0 } }));
        }

        YELO_UNIT_TEST_QUAD_BUILDER(BuildIndices_WithNullStartValue_Throws)
        {
            c_quad_builder::s_face* face_pointer = nullptr;

            AssertThrows(std::exception, c_quad_builder::BuildIndices(face_pointer, s_quad()));
        }

        YELO_UNIT_TEST_QUAD_BUILDER(BuildIndices_WithQuad_CalculatesQuadIndices)
        {
            c_quad_builder::s_face index_array[4];
            auto quad_instance = c_quad_instance({ 2, 1 }, { 0.25, 0.75 }, { 0.25, 0.75 });

            auto index_pointer = &index_array[0];
            c_quad_builder::BuildIndices(index_pointer, quad_instance.Quad());

            AssertThat(index_array[0], Equals(c_quad_builder::s_face { { 3, 0, 1 } } ));
            AssertThat(index_array[1], Equals(c_quad_builder::s_face { { 1, 4, 3 } } ));
            AssertThat(index_array[2], Equals(c_quad_builder::s_face { { 4, 1, 2 } } ));
            AssertThat(index_array[3], Equals(c_quad_builder::s_face { { 2, 5, 4 } } ));
        }

#undef YELO_UNIT_TEST_QUAD_BUILDER
#endif
    }
}
#endif
