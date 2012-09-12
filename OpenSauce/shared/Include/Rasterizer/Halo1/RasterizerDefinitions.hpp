/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

interface IDirect3DBaseTexture9;

namespace Yelo
{
	namespace Rasterizer
	{
		struct s_rasterizer_globals
		{
			bool initialized;
			PAD8;
			UNKNOWN_TYPE(int16);
			UNKNOWN_TYPE(rectangle2d);
			UNKNOWN_TYPE(rectangle2d);
			PAD32;
			UNKNOWN_TYPE(int32);
			UNKNOWN_TYPE(int32);
			PAD32; PAD32; PAD32;
			UNKNOWN_TYPE(real); UNKNOWN_TYPE(real);
			UNKNOWN_TYPE(real); UNKNOWN_TYPE(real);
			IDirect3DBaseTexture9** default_texture_white;
			IDirect3DBaseTexture9** default_texture_2d_texture;
			IDirect3DBaseTexture9** default_texture_3d_texture; // engine actually uses 2d for 3d cases
			IDirect3DBaseTexture9** default_texture_cubemap;
			UNKNOWN_TYPE(int16);
			int16 maximum_nodes_per_model;
			bool using_software_vertex_processing;
			PAD8;
			UNKNOWN_TYPE(int16); // only set, never read (in any meaningful way)
			uint32 fixed_function_ambient; // ambient light value for FF
			UNKNOWN_TYPE(bool); // active camo related
			UNKNOWN_TYPE(bool); // shadows related
			UNKNOWN_TYPE(bool); // render targets related
			PAD8; PAD32;
		}; BOOST_STATIC_ASSERT( sizeof(s_rasterizer_globals) == 0x60 );
		s_rasterizer_globals* RasterizerGlobals(); // defined in the implementing extension's code

		void InitializeMaximumNodesPerModelFixes(); // defined in the implementing extension's code

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

		struct s_render_window
		{
			int16 local_player_index;
			UNKNOWN_TYPE(bool);
			PAD8;
			s_render_camera render_camera, rasterizer_camera;
		}; BOOST_STATIC_ASSERT( sizeof(s_render_window) == 0xAC );

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
		s_render_globals* RenderGlobals(); // defined in the implementing extension's code

		struct s_player_screen_flash // this header file isn't the best place for this...
		{
			_enum type;
			PAD16;
			real intensity;
			UNKNOWN_TYPE(real);
			UNKNOWN_TYPE(real); UNKNOWN_TYPE(real); UNKNOWN_TYPE(real);
			UNKNOWN_TYPE(int32); // unused?
			UNKNOWN_TYPE(int32); // unused?
		}; BOOST_STATIC_ASSERT( sizeof(s_player_screen_flash) == 0x20 );
		struct s_rasterizer_window_parameters
		{
			_enum rasterizer_target;
			int16 window_index;
			UNKNOWN_TYPE(bool); // mirror rendering related
			UNKNOWN_TYPE(bool);
			PAD16;
			s_render_camera camera;
			s_render_frustum frustum;
			s_render_fog fog;
			s_player_screen_flash screen_flash;
		}; BOOST_STATIC_ASSERT( sizeof(s_rasterizer_window_parameters) == 0x258 );

		struct s_rasterizer_debug_options
		{
			bool fps;
			PAD8;
			int16 stats;
			int16 mode;
			bool wireframe;
			bool debug_model_vertices;
			int16 debug_model_lod;
			bool debug_transparents;
			bool debug_meter_shader;
			bool models;
			bool model_transparents;
			bool draw_first_person_weapon_first;
			bool stencil_mask;
			bool environment;
			bool environment_lightmaps;
			bool environment_shadows;
			bool environment_diffuse_lights;
			bool environment_diffuse_textures;
			bool environment_decals;
			bool environment_specular_lights;
			bool environment_specular_lightmaps;
			bool environment_reflection_lightmap_mask;
			bool environment_reflection_mirrors;
			bool environment_reflections;
			bool environment_transparents;
			bool environment_fog;
			bool environment_fog_screen;
			bool water;
			bool lens_flares;
			bool dynamic_unlit_geometry;
			bool dynamic_lit_geometry;
			bool dynamic_screen_geometry;
			bool hud_motion_sensor;
			bool detail_objects;
			bool debug_geometry;
			bool debug_geometry_multipass;
			bool fog_atmosphere;
			bool fog_plane;
			bool bump_mapping;
			PAD16;
			real lightmap_ambient;
			PAD16;
			int16 pad3;
			bool lightmaps_incident;
			bool lightmaps_filtering;
			PAD16;
			real model_lightning_ambient;
			bool environment_alpha_testing;
			bool environment_specular_mask;
			bool shadows_convolution;
			bool shadows_debug;
			bool water_mipmapping;
			bool active_camouflage;
			bool active_camouflage_multipass;
			bool plasma_energy;
			bool lens_flares_occlusion;
			bool lens_flares_occlusion_debug;
			bool ray_of_buddha;
			bool screen_flashes;
			bool screen_effects;
			bool DXTC_noise;
			bool soft_filter;
			bool secondary_render_target_debug;
			bool profile_log;
			PAD24;
			real detail_objects_offset_multiplier;
			real zbias;
			real zoffset;
			bool force_all_player_views_to_default_player;
			bool safe_frame_bounds;
			int16 freeze_flying_camera;
			bool zsprites;
			bool filthy_decal_fog_hack;
			bool smart;
			bool splitscreen_VB_optimization;
			bool profile_print_locks;
			PAD24;
			int32 profile_objectlook_time;
			int16 effects_level;
			int16 _unk1;
			int16 _unk2;
			PAD16;
			real pad3_scale;
			real f0;
			real f1;
			real f2;
			real f3;
			real f4;
			real f5;
		}; BOOST_STATIC_ASSERT( sizeof(s_rasterizer_debug_options) == 0x90 );
		s_rasterizer_debug_options* DebugOptions(); // defined in the implementing extension's code
	};
};