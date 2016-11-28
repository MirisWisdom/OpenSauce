/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/render/render_cameras.hpp>

namespace Yelo
{
	namespace Render
	{
		struct s_render_window
		{
			int16 local_player_index;
			UNKNOWN_TYPE(bool);
			PAD8;
			s_render_camera render_camera, rasterizer_camera;
		}; BOOST_STATIC_ASSERT( sizeof(s_render_window) == 0xAC );
		s_render_window* RenderWindow(); // defined in the implementing extension's code

		struct s_render_fog
		{
			word_flags flags; // same flags as in the tag definition
			PAD16;
			real_rgb_color atmospheric_color;
			real atmospheric_maximum_density,
				atmospheric_minimum_distance,
				atmospheric_maximum_distance;
			UNKNOWN_TYPE(int16);
			PAD16;
			real_plane3d plane; // copied from the bsp's fog planes
			real_rgb_color color; // copied from tag definition
			real max_density; // copied from tag definition
			real planar_maximum_distance;
			real opaque_depth; // copied from tag definition
			const void* screen_layers_definition; // pointer to the tag definition data
			UNKNOWN_TYPE(real);
		}; BOOST_STATIC_ASSERT( sizeof(s_render_fog) == 0x50 );

		struct s_render_globals
		{
			UNKNOWN_TYPE(int32);
			UNKNOWN_TYPE(int32);
			int16 local_player_index;
			UNKNOWN_TYPE(int16);
			UNKNOWN_TYPE(int32);
			UNKNOWN_TYPE(real);
			s_render_camera camera;
			s_render_frustum frustum;
			s_render_fog fog;
			int32 leaf_index;
			int32 cluster_index;
			PAD8; // probably an unused bool
			bool visible_sky_model;
			int16 visible_sky_index;

			struct {
				long_flags cluster_pvs[BIT_VECTOR_SIZE_IN_DWORDS(512)];
				byte clusters[0x1A0][Enums::k_maximum_rendered_clusters];
				int16 count;
				PAD16;
			}rendered_clusters;

			struct {
				uint32 visibility_bitvector[0x20000];
				int32 count;
				uint32 triangles[Enums::k_maximum_rendered_triangles];
			}rendered_triangles;
		}; BOOST_STATIC_ASSERT( sizeof(s_render_globals) == 0x9D298 );
		s_render_globals* RenderGlobals(); // defined in the implementing extension's code

		struct s_structure_render_globals
		{
			bool render_bsp; PAD24;
			int32 dynamic_triangle_buffer_index;
			UNKNOWN_TYPE(bool); PAD24;
			UNKNOWN_TYPE(real_vector3d);
		}; BOOST_STATIC_ASSERT( sizeof(s_structure_render_globals) == 0x18 );
		s_structure_render_globals* StructureRenderGlobals(); // defined in the implementing extension's code
	};

	namespace blam
	{
		void PLATFORM_API render_window(const uint16 local_player_index
			, void* render_camera
			, void* render_frustum
			, void* rasterizer_camera
			, void* rasterizer_frustum
			, void* rasterizer_target
			, const bool is_mirror);
	};
};