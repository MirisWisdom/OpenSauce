/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Render
	{
		struct s_render_camera
		{
			real_point3d point;
			real_vector3d forward;
			real_vector3d up;

			UNKNOWN_TYPE(bool); // controls the rendering of stuff related to certain objects
			PAD24;
			real vertical_field_of_view;
			rectangle2d viewport_bounds;
			rectangle2d window_bounds;
			real z_near ,z_far;
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
	};
};