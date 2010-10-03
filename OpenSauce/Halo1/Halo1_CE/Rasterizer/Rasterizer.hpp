/*
    Yelo: Open Sauce SDK
		Halo 1 (CE) Edition
    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#if !PLATFORM_IS_DEDI

namespace Yelo
{
	namespace Enums
	{
		enum rasterizer_target_render
		{
			_rasterizer_target_render_d3d,
			_rasterizer_target_render_primary,
			_rasterizer_target_render_secondary,
			_rasterizer_target_render
		};
	};

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
				int16 count;
				PAD16;
				uint32 triangles[0x4000];
			}dynamic_triangles;
		}; BOOST_STATIC_ASSERT( sizeof(s_render_globals) == 0x9D298 );
		s_render_globals* RenderGlobals();

		struct s_render_target
		{
			uint32 width, height;
			D3DFORMAT format;

			IDirect3DSurface9* surface;
			IDirect3DTexture9* texture;
			
			// returns whether the texture and surface are valid
			bool		IsEnabled() const;
			// Create the render target texture and surface. If either of those fails [enabled] will be false
			HRESULT		CreateTarget(IDirect3DDevice9* device, uint32 rt_width, uint32 rt_height, D3DFORMAT rt_format);
			// Release the texture and surface
			void		ReleaseTarget();
			// Set the device target to [surface] then clear it
			void		ClearTarget(IDirect3DDevice9* device);
		};
		s_render_target* GlobalRenderTargets();


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
		};
		s_rasterizer_debug_options* DebugOptions();

		struct s_rasterizer_frame_inputs
		{
			double elapsed_time; // just going off some documentation from FireScythe
			real UNKNOWN(0);
			real UNKNOWN(1);
		};
		s_rasterizer_frame_inputs* FrameInputs();


		void Initialize();
		void Dispose();

		void Update();

		void LoadSettings(TiXmlElement* dx9_element);
		void SaveSettings(TiXmlElement* dx9_element);

		void SetRenderStates();
	};
};
#endif