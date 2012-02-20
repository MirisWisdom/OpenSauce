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