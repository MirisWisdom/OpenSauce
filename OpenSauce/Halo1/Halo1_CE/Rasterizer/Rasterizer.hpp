/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

#include <Rasterizer/Halo1/RasterizerDefinitions.hpp>

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
		struct s_rasterizer_config
		{
			const BOOL linear_texture_addressing;
			const BOOL linear_texture_addressing_zoom;
			const BOOL linear_texture_addressing_sun;
			const BOOL use_fixed_function;
			const BOOL disable_driver_management;
			const BOOL unsupported_card;
			const BOOL prototype_card;
			const BOOL old_driver;
			const BOOL old_sound_driver;
			const BOOL invalid_driver;
			const BOOL invalid_sound_driver;
			const BOOL disable_buffering;
			const BOOL enable_stop_start;
			const BOOL head_relative_speech;
			const BOOL safe_mode;
			const DWORD force_shader;
			const BOOL use_anisotropic_filter;
			const BOOL disable_specular;
			const BOOL disable_render_targets;
			const BOOL disable_alpha_render_targets;
			const BOOL use_alternative_convolve_mask;
			const BOOL min_max_blend_op_is_broken;
			const FLOAT decal_z_bias_value;
			const FLOAT transparent_decal_z_bias_value;
			const FLOAT decal_slope_z_bias_value;
			const FLOAT transparent_decal_slope_z_bias_value;
		};
		s_rasterizer_config* RasterizerConfig();

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
			void		ClearTarget(IDirect3DDevice9* device, D3DCOLOR color = 0x00000000, DWORD flags = D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL);
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

		bool IsRenderingReflection();

		struct s_rasterizer_resolution
		{
			DWORD			width;
			DWORD			height;
			wchar_t			resolution_string[16];
			DWORD			refresh_rate_count;
			DWORD			refresh_rates[8];
		}; BOOST_STATIC_ASSERT(sizeof(s_rasterizer_resolution) == 0x4C);

		void Initialize();
		void Dispose();

		void Update();

		void LoadSettings(TiXmlElement* dx9_element);
		void SaveSettings(TiXmlElement* dx9_element);

		void SetRenderStates();
	};
};
#endif