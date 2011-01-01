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

/*
	User Add-on
		Component: Post-processing
		Original Author: FireScythe
		Integration: kornman00

	This component adds a highly customizable post processing ability to 
	Halo Custom Edition.
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include <Common/PackedFile.hpp>

#include "TagGroups/TagGroups.hpp" // need to include this first for tag macros, etc
#include <Blam/Halo1/shader_postprocess_definitions.hpp>

#include "Rasterizer/DX9/DX9.hpp"
#include "Rasterizer/RenderTargetChain.hpp"
#include "Rasterizer/GBuffer.hpp"
#include "Rasterizer/PostProcessing/PostProcessingQuadManager.hpp"

namespace Yelo
{
	namespace Postprocessing
	{
		struct s_dx9_render_state_capture
		{
		private:
			DWORD old_cullmode, old_fillmode, old_srcblend, old_dest_blend, old_blendop,
				old_zenable, old_zwriteenable, old_stencilenable, old_colorwriteenable,
				old_alphablendenable, old_alphatestenable, old_fogenable,
				old_depthbias, old_slopescaledepthbias;
		public:
			void Capture();
			void Restore();
		};

		/////////////////////////////////////////////////////////////////////
		// For holding values that change each frame
		/////////////////////////////////////////////////////////////////////
		struct s_per_frame_values
		{
			static s_per_frame_values g_instance;

			struct
			{
				D3DXMATRIX	world_view;
				D3DXMATRIX	world_view_transpose;
			}m_matricies;
			struct
			{
				real_point2d	scaled_ratio;
				real_point2d	default_ratio;

				real_point2d	value;
			}m_hud_scale;
			struct
			{
				real		near_clip;
				real		far_clip;
			}m_clipping;
			real		m_delta_time;


			// Poll the engine state for value updates
			void PollUpdate();
		};
		s_per_frame_values& PerFrameValues();


		/////////////////////////////////////////////////////////////////////
		// Runtime information for controlling effects
		/////////////////////////////////////////////////////////////////////
		struct s_activation_variables 
		{
			static	s_activation_variables g_instance;

			bool		m_player_has_control;
			bool		m_in_cinematic;
			bool		m_local_player_in_vehicle;
			bool		m_local_player_zoomed;

			// Poll the engine state for value updates
			void PollUpdate();
		};
		s_activation_variables& ActivationVariables();

		/////////////////////////////////////////////////////////////////////
		// Fades the current scene texture and target by a specified amount
		/////////////////////////////////////////////////////////////////////
		class c_fade_effect
		{
			bool				m_available;
			PAD24;
			LPD3DXEFFECT		m_effect;
			D3DXHANDLE			m_result_texture;
			D3DXHANDLE			m_fade_amount;
			c_quad_instance*	m_quad_instance;
		public:
			HRESULT		FadeResult(IDirect3DDevice9* device, float fade_value);
			HRESULT		AllocateResources(IDirect3DDevice9* device);
			void		ReleaseResources();
			bool		IsAvailable();

			c_fade_effect() : m_available(false), 
				m_effect(NULL), 
				m_result_texture(NULL), 
				m_fade_amount(NULL), 
				m_quad_instance(NULL) {}
		};

		/////////////////////////////////////////////////////////////////////
		// Global runtime values accessible to all subsystems
		/////////////////////////////////////////////////////////////////////
		struct s_postprocess_globals
		{
			static s_postprocess_globals g_instance;

			c_quad_manager					m_quad_manager;
			c_fade_effect					m_fade_effect;

			TagGroups::s_shader_postprocess_globals* m_map_postprocess_globals;

			c_packed_file					m_shader_file;
			struct {
				bool loaded;
				PAD24;
			}m_flags;
			struct s_rendering {
				point2d						screen_dimensions;	// Dimensions of the current render area
				IDirect3DDevice9*			render_device;		// Pointer to the current direct3D device
				D3DPRESENT_PARAMETERS		creation_parameters;// Parameters the device was created with
			}m_rendering;
			struct s_render_targets {
				DX9::s_render_target_chain	scene_buffer_chain;
				DX9::c_gbuffer*				gbuffer;
			}m_render_targets;			
			struct s_matricies {
				D3DXMATRIX					ortho_proj_matrix;	// Orthographic projection matrix for rendering quads
			}m_matricies;

			c_quad_manager&					QuadManager() { return m_quad_manager; }
			c_fade_effect&					FadeEffect() { return m_fade_effect; }
		};
		s_postprocess_globals& Globals();

		void		Initialize();
		void		Dispose();

		void		Initialize3D(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters);
		void		OnLostDevice();
		void		OnResetDevice(D3DPRESENT_PARAMETERS* pParameters);
		void		Render();
		void		Release();

		void		LoadSettings(TiXmlElement* dx9_element);
		void		SaveSettings(TiXmlElement* dx9_element);

		void		InitializeForNewMap();
		void		DisposeFromOldMap();
		void		Update(real delta_time);

		HRESULT		AllocateResources(D3DPRESENT_PARAMETERS* pParameters);
		void		ReleaseResources();	

		void*		HS_Load();
		void*		HS_Unload();
		void*		HS_SetEffectActive(void** arguments);
		void*		HS_SetEffectFade(void** arguments);
		void*		HS_GetEffectIndexByName(void** arguments);
		void*		HS_GetEffectCurrentFade(void** arguments);
		void*		HS_GetEffectFadeDirection(void** arguments);
		void*		HS_GetEffectIsValid(void** arguments);
		void*		HS_GetEffectShaderVariableIndexByName(void** arguments);
		void*		HS_SetEffectShaderVariableBoolean(void** arguments);
		void*		HS_SetEffectShaderVariableInteger(void** arguments);
		void*		HS_SetEffectShaderVariableReal(void** arguments);

		void		RenderPreAlphaBlended();
		void		RenderPreHUD();
		void		RenderPreMenu();
		void		RenderPostMenu();
	};
};
#endif