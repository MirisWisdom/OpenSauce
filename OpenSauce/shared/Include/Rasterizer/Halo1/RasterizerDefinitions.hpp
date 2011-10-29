/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Rasterizer
	{
		struct s_render_camera
		{
			real_point3d point;
			real_vector3d forward;
			real_vector3d up;

			UNKNOWN_TYPE(bool); // controls the rendering of stuff related to certain objects
			PAD24;
			UNKNOWN_TYPE(real);
			UNKNOWN_TYPE(rectangle2d);
			UNKNOWN_TYPE(rectangle2d);
			PAD32;
			UNKNOWN_TYPE(real);
			PAD32; PAD32; PAD32; PAD32; // just looks like 16 bytes of unused poop
		}; BOOST_STATIC_ASSERT( sizeof(s_render_camera) == 0x54 );

		struct s_render_frustum_matricies
		{
			const real_matrix4x3*		world_view;
			const real_matrix4x3*		world_view_transpose;
		};
		struct s_render_frustum
		{
			PAD128; // 4 reals
			real_matrix4x3 world_to_view;
			real_matrix4x3 view_to_world;

			// clipping bounds
			PAD_TYPE(real_plane3d);
			PAD_TYPE(real_plane3d);
			PAD_TYPE(real_plane3d);
			PAD_TYPE(real_plane3d);

			PAD_TYPE(real_plane3d);
			PAD_TYPE(real_plane3d);

			real z_near, z_far;

			PAD_TYPE(real_point3d);	PAD_TYPE(real_point3d);
			PAD_TYPE(real_point3d);	PAD_TYPE(real_point3d);
			PAD_TYPE(real_point3d);	PAD_TYPE(real_point3d);
			PAD_TYPE(real_point3d);	PAD_TYPE(real_point3d);

			bool projection_valid;		PAD24;

			// projection matrix
			PAD128;	PAD128;	PAD128;	PAD128;

			PAD_TYPE(real_point2d);

			inline void GetMatricies(s_render_frustum_matricies& mats) const
			{
				mats.world_view = &world_to_view;
				mats.world_view_transpose = &view_to_world;
			}
		}; BOOST_STATIC_ASSERT( sizeof(s_render_frustum) == 0x18C );

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
			byte fog[0x4C];
			PAD32;
			int32 leaf_index;
			int32 cluster_index;
			PAD8; // probably an unused bool
			bool visible_sky_model;
			int16 visible_sky_index;

			struct {
				uint32 cluster_pvs[16];
				byte clusters[0x1A0][0x80];
				int16 count;
				PAD16;
			}rendered_clusters;

			struct {
				uint32 visibility_bitvector[0x20000];
				int32 count;
				uint32 triangles[Enums::k_rastizer_maximum_dynamic_triangles];
			}dynamic_triangles;
		}; BOOST_STATIC_ASSERT( sizeof(s_render_globals) == 0x9D298 );
	};
};