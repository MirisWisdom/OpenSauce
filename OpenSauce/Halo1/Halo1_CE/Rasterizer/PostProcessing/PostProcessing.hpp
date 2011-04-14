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
		/*!
		 * \brief
		 * Used to store and restore a number of render device states that 
		 * are necessary for post processing.
		 * 
		 * This class is used to store a number of render device states before 
		 * the postprocess systems runs, and then to restore the state values 
		 * after the post processing is complete. This is necessary to prevent
		 * any rendering issues that could be caused if Halo does not reset the
		 * state before it continues its own drawing.
		 *
		 * It currently captures and restores the following Direct3D 
		 * render states:
		 * 	D3DRS_CULLMODE
		 * 	D3DRS_FILLMODE
		 * 	D3DRS_SRCBLEND
		 * 	D3DRS_DESTBLEND
		 * 	3DRS_BLENDOP
		 * 	D3DRS_ZENABLE
		 * 	D3DRS_ZWRITEENABLE
		 * 	D3DRS_STENCILENABLE
		 * 	D3DRS_COLORWRITEENABLE
		 * 	D3DRS_ALPHABLENDENABLE
		 * 	D3DRS_ALPHATESTENABLE
		 * 	D3DRS_FOGENABLE
		 * 	D3DRS_DEPTHBIAS
		 * 	D3DRS_SLOPESCALEDEPTHBIAS
		 */
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

		/**	Stored values that change on a per frame basis. */
		struct s_per_frame_values
		{
			/// A static instance of the struct.
			static s_per_frame_values g_instance;

			struct
			{
				/// The ratio use to make the HUD 4:3 relative to the screen ratio
				real_point2d	scaled_ratio;
				/// Ratio of an unchanged HUD (1:1)
				real_point2d	default_ratio;

				/// The current HUD scale ratio
				real_point2d	value;
			}m_hud_scale;
			struct
			{
				/// The rasterizers current near clip distance
				real		near_clip;
				/// The rasterizers current far clip distance
				real		far_clip;
			}m_clipping;
			/// The time in seconds that has passed since the last frame
			real		m_delta_time;


			/** Poll the engine state for value updates. */
			void PollUpdate();
		};
		/** Returns a static instance of s_per_frame_values. */
		s_per_frame_values& PerFrameValues();

		/*!
		 * \brief
		 * Runtime information for controlling post processing effects.
		 * 
		 * Retrieves and stored runtime information that can be used to
		 * control when a post process effect is rendered.
		 */
		struct s_activation_variables 
		{
			/*!
			 * \brief
			 * A static instance of s_activation_variables.
			 * 
			 * A static instance of s_activation_variables.
			 */
			static	s_activation_variables g_instance;

			/// True when the player is in first person
			bool		m_player_has_control;
			/// True when the cinematic black bars are present
			bool		m_in_cinematic;
			/// True when the player is in any seat of any vehicle
			bool		m_local_player_in_vehicle;
			/// Ture when the player is using the zoom function of any weapon that has it
			bool		m_local_player_zoomed;

			/** Poll the engine state for value updates. */
			void PollUpdate();
		};
		/** Returns a static instance of s_activation_variables. */
		s_activation_variables& ActivationVariables();

		/*!
		 * \brief
		 * Used to fade a post process effect in and/or out.
		 * 
		 * This class draws the current scene texture onto the current target
		 * using alpha blending to create a simple fade effect. The fade
		 * variables are set by the script function pp_set_effect_fade.
		 * 
		 * Because the post processing system uses a three surface render chain,
		 * when the fade amount is at either extent nothing has to be rendered 
		 * since when the fade amount is 1.0 nothing needs to be done, and at 
		 * 0.0f the current scene and target textures can just be swapped.
		 */
		class c_fade_effect
		{
		private:
			/// True when the effect loaded correctly
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

		/** Global runtime values accessible to all of the post processing subsystems. */
		struct s_postprocess_globals
		{
			/// A static instance of this struct
			static s_postprocess_globals g_instance;

			/// The post process quad manager
			c_quad_manager					m_quad_manager;
			/// The post process fading effect
			c_fade_effect					m_fade_effect;

			/// A pointer to the current post process globals struct
			TagGroups::s_shader_postprocess_globals* m_map_postprocess_globals;

			/**	A packed file containing the compiled shaders for the built
				in subsystems. */
			c_packed_file					m_shader_file;

			struct {
				/// True if the global postprocess resources have loaded correctly
				bool loaded;
				PAD24;
			}m_flags;
			struct s_rendering {
				/// Dimensions of the current render area
				point2d						screen_dimensions;
				/// Pointer to the current Direct3D device
				IDirect3DDevice9*			render_device;
				/// Parameters the device was created with
				D3DPRESENT_PARAMETERS		creation_parameters;
			}m_rendering;
			struct s_render_targets {
				/// 3 target render chain for the scene, source and target textures
				DX9::s_render_target_chain	scene_buffer_chain;
				/// Pointer to the GBuffer
				DX9::c_gbuffer*				gbuffer;
			}m_render_targets;			
			struct s_matricies {
				/// Orthographic projection matrix for rendering quads
				D3DXMATRIX					ortho_proj_matrix;
			}m_matricies;

			/** Returns a reference to the post processing quad manager. */
			c_quad_manager&					QuadManager() { return m_quad_manager; }
			/** Returns a reference to the fade effect instance. */
			c_fade_effect&					FadeEffect() { return m_fade_effect; }
		};
		/** Retruns a reference to the post processing globals. */
		s_postprocess_globals& Globals();

		/** Opens the packed file containing the compiled shaders for the built in subsystems. */
		void		Initialize();
		/** Closes the packed file containing the subsystem shaders. */
		void		Dispose();

		/**	Stores the device pointer and creation parameters, then initialises
			all of the subsystems. The global resources and subsystem resources
			are then allocated. Finally the quad manager creates the vertex and
			index buffers for the quads currently in use. */
		void		Initialize3D(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters);
		/**	Calls the lost device function for all of the subsystems, then
			releases all allocated resources. */
		void		OnLostDevice();
		/**	First the global resources are allocated, then the resource allocation
			and device reset functions are called for each subsystem. Finally
			the quad manager creates the vertex and index buffers for the quads
			currently in use. */
		void		OnResetDevice(D3DPRESENT_PARAMETERS* pParameters);
		/** Calls the render function for each subsystem. */
		void		Render();
		/** Releases all currently allocated resources. */
		void		Release();

		/** Loads the global post processing settings, then loads the settings
			for each subsystem. The subsystem settings are reset to defaults
			prior to looking for them in the xml settings file so if something
			is missing the subsystems will still function with expected values. */
		void		LoadSettings(TiXmlElement* dx9_element);
		/** Saves the current global settings and those of the subsystems. */
		void		SaveSettings(TiXmlElement* dx9_element);

		/**	Gets the pointer to a post process globals tag if present in the
			current map cache, then runs map dependent code in each subsystem.
			Lastly, the quad manager recreates its vertex and index buffers
			since the subsystems may have added or removed quads when
			initialising for a new map. */
		void		InitializeForNewMap();
		/** Runs subsystem code that handles a map being unloaded then releases
			the quad managers buffers. */
		void		DisposeFromOldMap();
		/** Updates per frame values and runs time dependent code in the
			subsystems.*/
		void		Update(real delta_time);

		/** Allocates the Direct3D resources used globally in the post
			processing system. 
			
			This function does the following:
				Sets global values that depend on the device settings.
				Gets initial values for the per frame variables.
				Allocates the render chain textures.
				Stores a pointer to the GBuffer.
				Creates the orthographic projection matrix.
				Allocates the resources for the fade effect.
		*/
		HRESULT		AllocateResources(D3DPRESENT_PARAMETERS* pParameters);
		/** Releases all resources allocated by the post process system.

			This function does the following:
				Release the resources of all subsystems.
				Release the quad manager buffers.
				Release the fade effects resources.
				Release the render chain resources
		*/
		void		ReleaseResources();	

		/** Console command to fully load the entire post processing system. */
		void*		HS_Load();
		/** Console command to fully unload the entire post processing system. */
		void*		HS_Unload();
		/**	Halo script function to enable or disable an internal post process effect. */
		void*		HS_SetEffectActive(void** arguments);
		/**	Halo script function to set the fade paremeters for an internal post process effect. */
		void*		HS_SetEffectFade(void** arguments);
		/** Halo script function to get the index of an effect by searching for it by name. */
		void*		HS_GetEffectIndexByName(void** arguments);
		/** Halo script function to get the current fade amount. */
		void*		HS_GetEffectCurrentFade(void** arguments);
		/** Halo script function to get the current fade direction of an effect. */
		void*		HS_GetEffectFadeDirection(void** arguments);
		/** Halo script function to find out whether an effect has loaded correctly and can be used. */
		void*		HS_GetEffectIsValid(void** arguments);
		/** Halo script function to find the index a scripted variable in a specific effect. */
		void*		HS_GetEffectShaderVariableIndexByName(void** arguments);
		/** Halo script function to set the value of a scripted boolean variable. */
		void*		HS_SetEffectShaderVariableBoolean(void** arguments);
		/** Halo script function to set the value of a scripted integer variable. */
		void*		HS_SetEffectShaderVariableInteger(void** arguments);
		/** Halo script function to set the value of a scripted real variable. */
		void*		HS_SetEffectShaderVariableReal(void** arguments);
		
		/** Renders all post processes that occur before alpha blended meshes. */
		void		RenderPreAlphaBlended();
		/** Renders all post processes that occur before the HUD is drawn. */
		void		RenderPreHUD();
		/** Renders all post processes that occur before the menu system is drawn. */
		void		RenderPreMenu();
		/** Renders all post processes that occur after the menu system is drawn. */
		void		RenderPostMenu();
	};
};
#endif