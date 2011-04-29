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
#include "Common/Precompile.hpp"
#include "Rasterizer/PostProcessing/PostProcessing.hpp"
#if !PLATFORM_IS_DEDI
#include "Rasterizer/GBuffer.hpp"
#include "Rasterizer/Rasterizer.hpp"
#include "Rasterizer/PostProcessing/PostProcessingShaderGenericBase.hpp"
#include "Rasterizer/PostProcessing/PostProcessingDebug.hpp"
#include "Game/Camera.hpp"
#include "Game/EngineFunctions.hpp"
#include "Game/GameState.hpp"
#include "Game/Players.hpp"
#include "Interface/Controls.hpp"
#include "Interface/GameUI.hpp"

#include "Common/GameSystems.hpp"
#include "Rasterizer/PostProcessing/MotionBlur/PostProcessingSubsystem_MotionBlur.hpp"
#include "Rasterizer/PostProcessing/Internal/PostProcessingSubsystem_Internal.hpp"

namespace Yelo
{
	namespace Postprocessing
	{
		/////////////////////////////////////////////////////////////////////
		// s_dx9_render_state_capture
		/*!
		 * \brief
		 * Captures the render states from the global render device.
		 * 
		 * Captures the render states from the global render device.
		 */
		void s_dx9_render_state_capture::Capture()
		{
			Globals().m_rendering.render_device->GetRenderState(D3DRS_CULLMODE, &old_cullmode);
			Globals().m_rendering.render_device->GetRenderState(D3DRS_FILLMODE, &old_fillmode);
			Globals().m_rendering.render_device->GetRenderState(D3DRS_SRCBLEND, &old_srcblend);
			Globals().m_rendering.render_device->GetRenderState(D3DRS_DESTBLEND, &old_dest_blend);	
			Globals().m_rendering.render_device->GetRenderState(D3DRS_BLENDOP, &old_blendop);	
			Globals().m_rendering.render_device->GetRenderState(D3DRS_ZENABLE, &old_zenable);	
			Globals().m_rendering.render_device->GetRenderState(D3DRS_ZWRITEENABLE, &old_zwriteenable);	
			Globals().m_rendering.render_device->GetRenderState(D3DRS_STENCILENABLE, &old_stencilenable);
			Globals().m_rendering.render_device->GetRenderState(D3DRS_COLORWRITEENABLE, &old_colorwriteenable);
			Globals().m_rendering.render_device->GetRenderState(D3DRS_ALPHABLENDENABLE, &old_alphablendenable);
			Globals().m_rendering.render_device->GetRenderState(D3DRS_ALPHATESTENABLE, &old_alphatestenable);
			Globals().m_rendering.render_device->GetRenderState(D3DRS_FOGENABLE, &old_fogenable);
			Globals().m_rendering.render_device->GetRenderState(D3DRS_DEPTHBIAS, &old_depthbias);
			Globals().m_rendering.render_device->GetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, &old_slopescaledepthbias);
		}

		/*!
		 * \brief
		 * Restores the render states to the global render device.
		 * 
		 * Restores the render states to the global render device.
		 */
		void s_dx9_render_state_capture::Restore()
		{
			Globals().m_rendering.render_device->SetRenderState(D3DRS_CULLMODE, old_cullmode);
			Globals().m_rendering.render_device->SetRenderState(D3DRS_FILLMODE, old_fillmode);
			Globals().m_rendering.render_device->SetRenderState(D3DRS_SRCBLEND, old_srcblend);
			Globals().m_rendering.render_device->SetRenderState(D3DRS_DESTBLEND, old_dest_blend);
			Globals().m_rendering.render_device->SetRenderState(D3DRS_BLENDOP, old_blendop);
			Globals().m_rendering.render_device->SetRenderState(D3DRS_ZENABLE, old_zenable);	
			Globals().m_rendering.render_device->SetRenderState(D3DRS_ZWRITEENABLE, old_zwriteenable);	
			Globals().m_rendering.render_device->SetRenderState(D3DRS_STENCILENABLE, old_stencilenable);
			Globals().m_rendering.render_device->SetRenderState(D3DRS_COLORWRITEENABLE, old_colorwriteenable);
			Globals().m_rendering.render_device->SetRenderState(D3DRS_ALPHABLENDENABLE, old_alphablendenable);
			Globals().m_rendering.render_device->SetRenderState(D3DRS_ALPHATESTENABLE, old_alphatestenable);
			Globals().m_rendering.render_device->SetRenderState(D3DRS_FOGENABLE, old_fogenable);
			Globals().m_rendering.render_device->SetRenderState(D3DRS_DEPTHBIAS, old_depthbias);
			Globals().m_rendering.render_device->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, old_slopescaledepthbias);
		}
		/////////////////////////////////////////////////////////////////////


		/////////////////////////////////////////////////////////////////////
		// s_per_frame_values
		s_per_frame_values s_per_frame_values::g_instance;

		/*!
		 * \brief
		 * Poll the engine state for value updates.
		 * 
		 * Poll the engine state for value updates.
		 */
		void		s_per_frame_values::PollUpdate()
		{
			m_hud_scale.value = Hud::HudIsScaled() ? m_hud_scale.scaled_ratio : m_hud_scale.default_ratio;

			m_clipping.near_clip = Rasterizer::RenderGlobals()->frustum.z_near;
			m_clipping.far_clip = Rasterizer::RenderGlobals()->frustum.z_far;

			m_delta_time = GameState::MainGlobals()->delta_time;
		}

		/*!
		 * \brief
		 * Returns a static instance of s_per_frame_values.
		 * 
		 * \returns
		 * A reference to a static s_per_frame_values.

		 * Returns a static instance of s_per_frame_values.
		 */
		s_per_frame_values& PerFrameValues() { return s_per_frame_values::g_instance; }
		/////////////////////////////////////////////////////////////////////


		/////////////////////////////////////////////////////////////////////
		// c_fade_effect
		/*!
		 * \brief
		 * Fades the result of a post process effect.
		 * 
		 * \param device
		 * A pointer to the current render device.
		 * 
		 * \param fade_value
		 * The amount to interpolate between fully on (1.0) and fully off (0.0).
		 * 
		 * \returns
		 * Returns S_OK if drawing the fade was successful.
		 * 
		 * Takes the current effects result and draws it to the next effects
		 * scene texture, using alpha blending to fade it in/out.
		 * 
		 * \remarks
		 * When fade_value is 0.0 or 1.0 nothing is drawn, the surfaces are
		 * just swapped since this results in the same outcome.
		 */
		HRESULT		c_fade_effect::FadeResult(IDirect3DDevice9* device, float fade_value)
		{			
			// if the fade shader is not loaded, do nothing
			if(!IsAvailable()) return E_FAIL;

			// if the fade value is at either extent, then simply swap surfaces if needed
			if(fade_value == 0.0f)
			{
				Globals().m_render_targets.scene_buffer_chain.SetSceneToLast();
				return S_OK;
			}
			else if(fade_value == 1.0f)
				return S_OK;

			HRESULT hr;
			
			// set the effect result as the scene texture
			Globals().m_render_targets.scene_buffer_chain.SetSceneToLast();
			// set the scene prior to the effect as the render target
			Globals().m_render_targets.scene_buffer_chain.Flip();

			m_effect->SetTexture(m_result_texture, Globals().m_render_targets.scene_buffer_chain.GetSceneTexture());
			m_effect->SetFloat(m_fade_amount, fade_value);
			device->SetRenderTarget(0, Globals().m_render_targets.scene_buffer_chain.GetCurrentSurface());

			// use alpha blending so the effect just has to specify an alpha value
			device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
			device->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);			
			device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			
			// render the fade effect
			UINT num_passes;
			hr = m_effect->Begin( &num_passes, 0 );
			for(UINT pass = 0; pass < num_passes; ++pass )
			{	
				hr = m_effect->BeginPass( pass );

				device->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 
						m_quad_instance->m_quad.start_vertex,
						0, 
						m_quad_instance->m_quad.vertex_count, 
						m_quad_instance->m_quad.start_index, 
						m_quad_instance->m_quad.primitive_count);
				hr = m_effect->EndPass();
				Globals().m_render_targets.scene_buffer_chain.Flip();
			}
			hr = m_effect->End();

			device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
			device->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);
			device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
			device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
			return hr;
		}
		/*!
		 * \brief
		 * Allocates the Direct3D resources for the fade effect.
		 * 
		 * \param device
		 * A pointer to the current render device.
		 * 
		 * \returns
		 * Returns S_OK if allocating the necessary resources was successful.
		 * 
		 * Gets the fade effect from the packed file and creates the D3DX
		 * effect. Also registers a 4x4 quad with the quad manager.
		 */
		HRESULT		c_fade_effect::AllocateResources(IDirect3DDevice9* device)
		{				
			m_available = true;

			// get a pointer to the fade effects binary shader
			uint32 data_size;
			void* data_pointer = Globals().m_shader_file.GetDataPointer("PP_EffectFade", &data_size);
			if(data_size == 0 || data_pointer == NULL)
				m_available = false;
			else
			{
				// register a 4x4 quad used by the fade effect
				m_quad_instance = Globals().QuadManager().CreateQuad(4, 4);
				if(!m_quad_instance)
				{
					m_available = false;
					return E_FAIL;
				}

				// create the fade direct3d effect
				LPD3DXBUFFER error_buffer = NULL;
				HRESULT hr = D3DXCreateEffect(
					device,
					data_pointer, 
					data_size, 
					NULL, NULL, NULL, NULL, 
					&m_effect, 
					&error_buffer);	
				if (FAILED(hr))
				{
					if(GameState::DevmodeEnabled())
					{
						Postprocessing::Debug::WriteLine(
							"Error: failed to load shader \"%s\"",
							"pp_shaders.shd:PP_EffectFade");
						Postprocessing::Debug::WriteD3DXErrors(error_buffer, 1);
					}
					m_available = false;
				}
				else
				{
					// set up the fade effects variables
					m_effect->SetMatrix("c_ortho_wvp", &Globals().m_matricies.ortho_proj_matrix);
					m_result_texture = m_effect->GetParameterByName(NULL, "t_result");
					m_fade_amount = m_effect->GetParameterByName(NULL, "c_fade_amount");
					if(!m_result_texture || !m_fade_amount)
						m_available = false;
				}
				Yelo::safe_release(error_buffer);	
			}
			return m_available ? S_OK : E_FAIL;
		}
		/*!
		 * \brief
		 * Releases the Direct3D resources used by the fade effect.
		 * 
		 * Releases the Direct3D resources and also releases the quad instance.
		 */
		void		c_fade_effect::ReleaseResources()
		{
			// release all resources
			Yelo::safe_release(m_effect);
			Yelo::safe_release(m_quad_instance);
			m_result_texture = NULL;
			m_fade_amount = NULL;
			m_available = false;
		}
		/*!
		 * \brief
		 * Returns true if the fade effect is ready to use.
		 * 
		 * \returns
		 * Returns true if the fade effect is ready to use. 
		 * Otherwise, returns false.

		 * Returns true when the fade effect is loaded and 
		 * ready to use.
		 */
		bool		c_fade_effect::IsAvailable()
		{
			return m_available;
		}
		/////////////////////////////////////////////////////////////////////


		/////////////////////////////////////////////////////////////////////
		// s_postprocess_globals
		s_postprocess_globals s_postprocess_globals::g_instance;

		s_postprocess_globals& Globals() { return s_postprocess_globals::g_instance; }
		/////////////////////////////////////////////////////////////////////


		/////////////////////////////////////////////////////////////////////
		// s_activation_variables
		s_activation_variables s_activation_variables::g_instance;

		void		s_activation_variables::PollUpdate()
		{
			// does the player have camera control (camera_control 0)
			m_player_has_control = !Camera::DirectorScripting()->initialized;

			// are we currently in a cinematic?
			Camera::s_cinematic_globals_data* cinematic_globals = Camera::CinematicGlobals();
			m_in_cinematic = (cinematic_globals ? cinematic_globals->in_progress : false);

			// are we in a vehicle?
			datum_index player_index = Players::LocalPlayerIndex();
			m_local_player_in_vehicle = (!player_index.IsNull() ? Players::GetVehicle(player_index, NULL) != NULL : false);

			// is our weapon zoomed?
			m_local_player_zoomed = *(Players::PlayerControlGlobals()->local_players[0].GetZoomLevel()) != NONE;
		}

		s_activation_variables& ActivationVariables() { return s_activation_variables::g_instance; }
		/////////////////////////////////////////////////////////////////////


		void		Initialize()
		{
			char file_string[MAX_PATH];
			file_string[0] = '\0';

			// open the post processing shaders file for reading
			strcat_s(file_string, MAX_PATH, Settings::CommonAppDataPath());
			strcat_s(file_string, MAX_PATH, "shaders\\pp_shaders.shd");

			Globals().m_shader_file.OpenFile(file_string);
		}
		void		Dispose()
		{
			Globals().m_shader_file.CloseFile();
		}

		void		Initialize3D(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters)
		{
			// store a pointer to the device and a copy of its creation parameters
			Globals().m_rendering.render_device = pDevice;
			Globals().m_rendering.creation_parameters = *pParameters;

			// loop through all of the post processing systems and initialise them
			Yelo::Main::s_postprocessing_subsystem_component* subsystems;
			const int32 subsystem_count = Yelo::Main::GetPostprocessingSubsystemComponents(subsystems);

			for(int32 i = 0; i <= subsystem_count; i++)
				subsystems[i].Initialize(pDevice, pParameters);

			// allocate global resources, then allocate the resources of the subsystems
			AllocateResources(pParameters);

			for(int32 i = 0; i <= subsystem_count; i++)
				subsystems[i].AllocateResources(Globals().m_rendering.render_device);

			// build the current vertex and index buffers			
			Globals().QuadManager().BuildBuffers(
				Globals().m_rendering.render_device,
				Globals().m_rendering.screen_dimensions.x,
				Globals().m_rendering.screen_dimensions.y);
		}	
		void		OnLostDevice()
		{
			// perform all device lost logic for the subsystems, then relase all resources
			Yelo::Main::s_postprocessing_subsystem_component* subsystems;
			const int32 subsystem_count = Yelo::Main::GetPostprocessingSubsystemComponents(subsystems);

			for(int32 i = 0; i <= subsystem_count; i++)
				subsystems[i].OnLostDevice();

			ReleaseResources();
		}
		void		OnResetDevice(D3DPRESENT_PARAMETERS* pParameters)
		{
			// allocate global resources, then allocate the resources for all subsystems
			AllocateResources(pParameters);

			Yelo::Main::s_postprocessing_subsystem_component* subsystems;
			const int32 subsystem_count = Yelo::Main::GetPostprocessingSubsystemComponents(subsystems);

			for(int32 i = 0; i <= subsystem_count; i++)
			{
				subsystems[i].AllocateResources(Globals().m_rendering.render_device);
				// this might not be necessary considering all DX resources are released when the device is lost
				subsystems[i].OnResetDevice(Globals().m_rendering.render_device, pParameters);
			}
			
			// create the current vertex and index buffers
			Globals().QuadManager().BuildBuffers(
				Globals().m_rendering.render_device,
				Globals().m_rendering.screen_dimensions.x,
				Globals().m_rendering.screen_dimensions.y);
		}
		void		Render()
		{
			// render all of the subsystems
			Yelo::Main::s_postprocessing_subsystem_component* subsystems;
			const int32 subsystem_count = Yelo::Main::GetPostprocessingSubsystemComponents(subsystems);

			for(int32 i = 0; i <= subsystem_count; i++)
				subsystems[i].Render();
		}
		void		Release()
		{
			// release all resources
			ReleaseResources();
		}


		void		LoadSettings(TiXmlElement* dx9_element)
		{
			ZeroMemory(&Globals(), sizeof(Globals()));

			Yelo::Main::s_postprocessing_subsystem_component* subsystems;
			const int32 subsystem_count = Yelo::Main::GetPostprocessingSubsystemComponents(subsystems);

			// reset the settings for all subsystems to default values
			for(int32 i = 0; i <= subsystem_count; i++)
				subsystems[i].SetDefaultSettings();

			// load the settings for all subsystems
			if(dx9_element != NULL)
			{
				TiXmlElement* pp_element = dx9_element->FirstChildElement("postprocessing");

				if(pp_element != NULL)
					for(int32 i = 0; i <= subsystem_count; i++)
						subsystems[i].LoadSettings(pp_element);
			}
		}
		void		SaveSettings(TiXmlElement* dx9_element)
		{
			TiXmlElement* pp_element = NULL;

			pp_element = new TiXmlElement("postprocessing");
				dx9_element->LinkEndChild(pp_element);

			// save the subsystems current settings
			Yelo::Main::s_postprocessing_subsystem_component* subsystems;
			const int32 subsystem_count = Yelo::Main::GetPostprocessingSubsystemComponents(subsystems);

			for(int32 i = 0; i <= subsystem_count; i++)
				subsystems[i].SaveSettings(pp_element);
		}

		void		InitializeForNewMap()
		{
			TagGroups::tag_iterator iter;
			// look for a shader_postprocess_globals tag in the cache
			tag_iterator_new(iter, TagGroups::s_shader_postprocess_globals::k_group_tag);
			// get the first instance (there should only be one, others will be ignored)
			datum_index tag_index = tag_iterator_next(iter);
			if(!tag_index.IsNull())
			{
				// set the current shader globals tag reference to the tag found
				Globals().m_map_postprocess_globals = TagGroups::Instances()[tag_index.index].Definition<TagGroups::s_shader_postprocess_globals>();
			}
			else
				Globals().m_map_postprocess_globals = NULL;

			// initialise the subsystems that are map dependent
			Yelo::Main::s_postprocessing_subsystem_component* subsystems;
			const int32 subsystem_count = Yelo::Main::GetPostprocessingSubsystemComponents(subsystems);

			for(int32 i = 0; i <= subsystem_count; i++)
				subsystems[i].InitializeForNewMap();
			
			// quad buffers will have been released by DisposeFromOldMap
			// quads may have been added so create new buffers
			Globals().QuadManager().BuildBuffers(
				Globals().m_rendering.render_device,
				Globals().m_rendering.screen_dimensions.x,
				Globals().m_rendering.screen_dimensions.y);
		}
		void		DisposeFromOldMap()
		{
			Globals().m_map_postprocess_globals = NULL;

			// dispose the subsystems that are map dependent
			Yelo::Main::s_postprocessing_subsystem_component* subsystems;
			const size_t subsystem_count = Yelo::Main::GetPostprocessingSubsystemComponents(subsystems);

			for(size_t i = 0; i <= subsystem_count; i++)
				subsystems[i].DisposeFromOldMap();
			
			// quads will probably have been removed so release the buffers
			Globals().QuadManager().Release();
		}
		void		Update(real delta_time)
		{			
			ActivationVariables().PollUpdate();

			// update all subsystems
			Yelo::Main::s_postprocessing_subsystem_component* subsystems;
			const int32 subsystem_count = Yelo::Main::GetPostprocessingSubsystemComponents(subsystems);

			for(int32 i = 0; i <= subsystem_count; i++)
				subsystems[i].Update(delta_time);
		}

		HRESULT		AllocateResources(D3DPRESENT_PARAMETERS* pParameters)
		{
			// If already loaded do nothing
			if(Globals().m_flags.loaded) 
				return S_OK;

			HRESULT hr = Globals().m_rendering.render_device->TestCooperativeLevel();
			if (SUCCEEDED(hr))	{	
				Globals().m_rendering.screen_dimensions.x = pParameters->BackBufferWidth;
				Globals().m_rendering.screen_dimensions.y = pParameters->BackBufferHeight;
				Globals().m_rendering.creation_parameters = *pParameters;

				//Get the new ratio between screen size and HUD size
				real_point2d Scale = {1.0f, 1.0f};

				const real old_ratio = (real)4 / 3;
				real new_ratio = (real)Globals().m_rendering.screen_dimensions.x / Globals().m_rendering.screen_dimensions.y;

				if (new_ratio > old_ratio)
					Scale.x = old_ratio / new_ratio;
				else
					Scale.y = new_ratio / old_ratio;

				PerFrameValues().m_hud_scale.scaled_ratio = Scale;
				PerFrameValues().m_hud_scale.default_ratio.x = 1.0f;
				PerFrameValues().m_hud_scale.default_ratio.y = 1.0f;

				PerFrameValues().PollUpdate();

				// Allocate the resources for the renderchain
				Globals().m_render_targets.scene_buffer_chain.AllocateResources(Globals().m_rendering.render_device, 
					pParameters->BackBufferWidth, 
					pParameters->BackBufferHeight);

				// get the gbuffer render targets
				Globals().m_render_targets.gbuffer = &DX9::c_gbuffer_system::GBuffer();
				
				// Create the orthographic projection matrix for rendering the quads
				D3DXMATRIX OrthoTranslation;
				D3DXMatrixTranslation(&OrthoTranslation, -1.0f, 1.0f, 0);
				D3DXMatrixOrthoLH (&Globals().m_matricies.ortho_proj_matrix, 
					Globals().m_rendering.screen_dimensions.x, 
					Globals().m_rendering.screen_dimensions.y, 
					1.0f, 10.0f);
				D3DXMatrixMultiply(&Globals().m_matricies.ortho_proj_matrix, &Globals().m_matricies.ortho_proj_matrix, &OrthoTranslation);
				
				// Load and create the fade effect shader
				Globals().FadeEffect().AllocateResources(Globals().m_rendering.render_device);

				Globals().m_flags.loaded = true;
			}
			return S_OK;
		}
		void		ReleaseResources()
		{
			// release the subsystem's resources
			Yelo::Main::s_postprocessing_subsystem_component* subsystems;
			const int32 subsystem_count = Yelo::Main::GetPostprocessingSubsystemComponents(subsystems);

			for(int32 i = 0; i <= subsystem_count; i++)
				subsystems[i].ReleaseResources();

			// release global resources the subsystems might use
			Globals().QuadManager().Release();
			Globals().FadeEffect().ReleaseResources();
			Globals().m_render_targets.scene_buffer_chain.ReleaseResources();
			Globals().m_flags.loaded = false;
		}		
			
		void*		HS_Load()
		{		
			if(!Globals().m_flags.loaded) AllocateResources(&Globals().m_rendering.creation_parameters);

			Yelo::Main::s_postprocessing_subsystem_component* subsystems;
			const int32 subsystem_count = Yelo::Main::GetPostprocessingSubsystemComponents(subsystems);

			for(int32 i = 0; i <= subsystem_count; i++)
				subsystems[i].Load();
			
			// quads will have been added by the Load process so create the quad buffers
			Globals().QuadManager().BuildBuffers(
				Globals().m_rendering.render_device,
				Globals().m_rendering.screen_dimensions.x,
				Globals().m_rendering.screen_dimensions.y);

			return NULL;
		}
		void*		HS_Unload()
		{
			Yelo::Main::s_postprocessing_subsystem_component* subsystems;
			const int32 subsystem_count = Yelo::Main::GetPostprocessingSubsystemComponents(subsystems);

			for(int32 i = 0; i <= subsystem_count; i++)
				subsystems[i].Unload();

			// quads will have been removed by the Unload process so release the quad buffers
			if(Globals().m_flags.loaded) ReleaseResources();	
			return NULL;
		}

		void*		HS_SetEffectActive(void** arguments)
		{
			struct s_arguments {
				uint16 effect_index;
				PAD16;
				bool active_bool;
				PAD24;
			}* args = CAST_PTR(s_arguments*, arguments);
			TypeHolder result;

			result.boolean = Subsystem::Internal::c_internal_subsystem::g_instance.SetEffectActive(args->effect_index, args->active_bool);
			
			return result.pointer;
		}
		void*		HS_SetEffectFade(void** arguments)
		{
			struct s_arguments {
				uint16 effect_index;
				PAD16;
				real fade_start;
				real fade_end;
				real fade_time;
			}* args = CAST_PTR(s_arguments*, arguments);
			TypeHolder result;

			result.boolean = Subsystem::Internal::c_internal_subsystem::g_instance.SetEffectFade(args->effect_index, args->fade_start, args->fade_end, args->fade_time);
			
			return result.pointer;
		}
		void*		HS_GetEffectIndexByName(void** arguments)
		{
			struct s_arguments {
				const char* test_string;
			}* args = CAST_PTR(s_arguments*, arguments);
			TypeHolder result;

			result.uint32 = Subsystem::Internal::c_internal_subsystem::g_instance.GetEffectIndexByName(args->test_string);
			
			return result.pointer;
		}
		void*		HS_GetEffectCurrentFade(void** arguments)
		{
			struct s_arguments {
				uint16 effect_index;
				PAD16;
			}* args = CAST_PTR(s_arguments*, arguments);
			TypeHolder result;

			result.real = Subsystem::Internal::c_internal_subsystem::g_instance.GetEffectCurrentFade(args->effect_index);
			
			return result.pointer;
		}	
		void*		HS_GetEffectFadeDirection(void** arguments)
		{
			struct s_arguments {
				uint16 effect_index;
				PAD16;
			}* args = CAST_PTR(s_arguments*, arguments);
			TypeHolder result;

			result.int16 = Subsystem::Internal::c_internal_subsystem::g_instance.GetEffectFadeDirection(args->effect_index);
			
			return result.pointer;
		}	
		void*		HS_GetEffectIsValid(void** arguments)
		{
			struct s_arguments {
				uint16 effect_index;
				PAD16;
			}* args = CAST_PTR(s_arguments*, arguments);
			TypeHolder result;

			result.boolean = Subsystem::Internal::c_internal_subsystem::g_instance.GetEffectIsValid(args->effect_index);
			
			return result.pointer;
		}
		void*		HS_GetEffectShaderVariableIndexByName(void** arguments)
		{
			struct s_arguments {
				uint16 effect_index;
				PAD16;
				const char* test_string;
			}* args = CAST_PTR(s_arguments*, arguments);
			TypeHolder result;

			result.int16 = Subsystem::Internal::c_internal_subsystem::g_instance.GetEffectShaderVariableIndexByName(args->effect_index, args->test_string);
			
			return result.pointer;
		}
		void*		HS_SetEffectShaderVariableBoolean(void** arguments)
		{
			struct s_arguments {
				uint16 effect_index;
				PAD16;
				uint16 script_variable_index;
				PAD16;
				bool value;
				PAD24;
				real interp_time;
			}* args = CAST_PTR(s_arguments*, arguments);
			
			Subsystem::Internal::c_internal_subsystem::g_instance.SetEffectShaderVariableBoolean(
				args->effect_index,		args->script_variable_index, 
				args->value,
				args->interp_time);
			
			return NULL;
		}
		void*		HS_SetEffectShaderVariableInteger(void** arguments)
		{
			struct s_arguments {
				uint16 effect_index;
				PAD16;
				uint16 script_variable_index;
				PAD16;
				uint32 value;
				real interp_time;
			}* args = CAST_PTR(s_arguments*, arguments);
			
			Subsystem::Internal::c_internal_subsystem::g_instance.SetEffectShaderVariableInteger(
				args->effect_index,		args->script_variable_index, 
				args->value,
				args->interp_time);
			
			return NULL;
		}
		void*		HS_SetEffectShaderVariableReal(void** arguments)
		{
			struct s_arguments {
				uint16 effect_index;
				PAD16;
				uint16 script_variable_index;
				PAD16;
				real values[4];
				real interp_time;
			}* args = CAST_PTR(s_arguments*, arguments);
			
			Subsystem::Internal::c_internal_subsystem::g_instance.SetEffectShaderVariableReal(
				args->effect_index,		args->script_variable_index, 
				args->values[0], 
				args->values[1], 
				args->values[2], 
				args->values[3],
				args->interp_time);
			
			return NULL;
		}

		/*
			Fixed black screening by saving and restoring the D3DRS_COLORWRITEENABLE state.
			Fixed black screening when using the flashlight in SP by saving and restoring 
				the D3DRS_ALPHABLENDENABLE, D3DRS_ALPHATESTENABLE and D3DRS_FOGENABLE states.
		*/

		static void RenderingCaptureAndSetState(IDirect3DDevice9* render_device, s_dx9_render_state_capture& dx9_state_capture, Enums::postprocess_render_stage stage)
		{
			dx9_state_capture.Capture();

			render_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
			render_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			render_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
			render_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
			render_device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			render_device->SetRenderState(D3DRS_ZENABLE, false);
			render_device->SetRenderState(D3DRS_ZWRITEENABLE, false);
			render_device->SetRenderState(D3DRS_STENCILENABLE, false);
			render_device->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
			render_device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
			render_device->SetRenderState(D3DRS_ALPHATESTENABLE, false);
			render_device->SetRenderState(D3DRS_FOGENABLE, false);
			if(stage == Enums::_postprocess_render_stage_pre_blur)
			{
				render_device->SetRenderState(D3DRS_DEPTHBIAS, 0);
				render_device->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, 0);
			}
		}
		
		void		RenderPreAlphaBlended() 
		{
			PerFrameValues().PollUpdate();

			if (!Globals().m_flags.loaded) return;

			IDirect3DDevice9* render_device = Globals().m_rendering.render_device;

			// reset the targets so that halo's primary buffer is the scene buffer
			Globals().m_render_targets.scene_buffer_chain.ResetTargets();

			// the same buffers are used for all post processes, set them here
			HRESULT hr = Globals().QuadManager().SetBuffers(render_device);
			if(SUCCEEDED(hr))
			{
				s_dx9_render_state_capture dx9_state_capture;
				{
					RenderingCaptureAndSetState(render_device, dx9_state_capture, Enums::_postprocess_render_stage_pre_blur);

					Yelo::Main::s_postprocessing_subsystem_component* subsystems;
					const int32 subsystem_count = Yelo::Main::GetPostprocessingSubsystemComponents(subsystems);

					bool effects_applied = false;
					for(int32 i = 0; i <= subsystem_count; i++)
						effects_applied |= subsystems[i].DoPostProcesses(Globals().m_rendering.render_device, 
							PerFrameValues().m_delta_time, 
							Enums::_postprocess_render_stage_pre_blur);

					effects_applied |= Subsystem::MotionBlur::c_motionblur_subsystem::DoMotionBlurProcess(render_device, 
						PerFrameValues().m_delta_time);
					
					for(int32 i = 0; i <= subsystem_count; i++)
						effects_applied |= subsystems[i].DoPostProcesses(Globals().m_rendering.render_device, 
							PerFrameValues().m_delta_time, 
							Enums::_postprocess_render_stage_pre_alpha_blended);
				
					// it's probable that the last surface rendered to isn't halos primary buffer
					// in that case, stretchrect the result to the primary buffer
					if(effects_applied && 
						(Rasterizer::GlobalRenderTargets()[Enums::_rasterizer_target_render_primary].surface !=
							Globals().m_render_targets.scene_buffer_chain.GetSceneSurface()))
						render_device->StretchRect(
							Globals().m_render_targets.scene_buffer_chain.GetSceneSurface(), 
							NULL, 
							Rasterizer::GlobalRenderTargets()[Enums::_rasterizer_target_render_primary].surface,
							NULL, 
							D3DTEXF_NONE);
				};
				dx9_state_capture.Restore();

				render_device->SetRenderTarget(0, Rasterizer::GlobalRenderTargets()[Enums::_rasterizer_target_render_primary].surface);
			}
		}
		void		RenderPreHUD()
		{
			if (!Globals().m_flags.loaded) return;

			IDirect3DDevice9* render_device = Globals().m_rendering.render_device;

			// reset the targets so that halo's primary buffer is the scene buffer
			Globals().m_render_targets.scene_buffer_chain.ResetTargets();

			// the same buffers are used for all post processes, set them here
			HRESULT hr = Globals().QuadManager().SetBuffers(render_device);
			if(SUCCEEDED(hr))
			{
				s_dx9_render_state_capture dx9_state_capture;
				{
					RenderingCaptureAndSetState(render_device, dx9_state_capture, Enums::_postprocess_render_stage_pre_hud);

					Yelo::Main::s_postprocessing_subsystem_component* subsystems;
					const int32 subsystem_count = Yelo::Main::GetPostprocessingSubsystemComponents(subsystems);

					bool effects_applied = false;
					for(int32 i = 0; i <= subsystem_count; i++)
						effects_applied |= subsystems[i].DoPostProcesses(render_device, 
							PerFrameValues().m_delta_time, 
							Enums::_postprocess_render_stage_pre_hud);				
				
					// it's probable that the last surface rendered to isn't halos primary buffer
					// in that case, stretchrect the result to the primary buffer
					if(effects_applied &&
						(Rasterizer::GlobalRenderTargets()[Enums::_rasterizer_target_render_primary].surface !=
							Globals().m_render_targets.scene_buffer_chain.GetSceneSurface()))
						render_device->StretchRect(
							Globals().m_render_targets.scene_buffer_chain.GetSceneSurface(), 
							NULL, 
							Rasterizer::GlobalRenderTargets()[Enums::_rasterizer_target_render_primary].surface,
							NULL, 
							D3DTEXF_NONE);
				};
				dx9_state_capture.Restore();

				render_device->SetRenderTarget(0, Rasterizer::GlobalRenderTargets()[Enums::_rasterizer_target_render_primary].surface);
			}
		}
		void		RenderPreMenu() 
		{
			if (!Globals().m_flags.loaded) return;

			IDirect3DDevice9* render_device = Globals().m_rendering.render_device;

			// reset the targets so that halo's primary buffer is the scene buffer
			Globals().m_render_targets.scene_buffer_chain.ResetTargets();

			// the same buffers are used for all post processes, set them here
			HRESULT hr = Globals().QuadManager().SetBuffers(render_device);
			if(SUCCEEDED(hr))
			{
				s_dx9_render_state_capture dx9_state_capture;
				{
					RenderingCaptureAndSetState(render_device, dx9_state_capture, Enums::_postprocess_render_stage_pre_ui);

					Yelo::Main::s_postprocessing_subsystem_component* subsystems;
					const int32 subsystem_count = Yelo::Main::GetPostprocessingSubsystemComponents(subsystems);

					bool effects_applied = false;
					for(int32 i = 0; i <= subsystem_count; i++)
						effects_applied |= subsystems[i].DoPostProcesses(render_device, 
							PerFrameValues().m_delta_time, 
							Enums::_postprocess_render_stage_pre_ui);						
				
					// it's probable that the last surface rendered to isn't halos primary buffer
					// in that case, stretchrect the result to the primary buffer
					if(effects_applied &&
						(Rasterizer::GlobalRenderTargets()[Enums::_rasterizer_target_render_primary].surface !=
							Globals().m_render_targets.scene_buffer_chain.GetSceneSurface()))
						render_device->StretchRect(
							Globals().m_render_targets.scene_buffer_chain.GetSceneSurface(), 
							NULL, 
							Rasterizer::GlobalRenderTargets()[Enums::_rasterizer_target_render_primary].surface,
							NULL, 
							D3DTEXF_NONE);
				};
				dx9_state_capture.Restore();

				render_device->SetRenderTarget(0, Rasterizer::GlobalRenderTargets()[Enums::_rasterizer_target_render_primary].surface);
			}
		}
		void		RenderPostMenu() 
		{
			if (!Globals().m_flags.loaded) return;

			IDirect3DDevice9* render_device = Globals().m_rendering.render_device;

			// reset the targets so that halo's primary buffer is the scene buffer
			Globals().m_render_targets.scene_buffer_chain.ResetTargets();

			// the same buffers are used for all post processes, set them here
			HRESULT hr = Globals().QuadManager().SetBuffers(render_device);
			if(SUCCEEDED(hr))
			{
				s_dx9_render_state_capture dx9_state_capture;
				{
					RenderingCaptureAndSetState(render_device, dx9_state_capture, Enums::_postprocess_render_stage_post_ui);

					Yelo::Main::s_postprocessing_subsystem_component* subsystems;
					const int32 subsystem_count = Yelo::Main::GetPostprocessingSubsystemComponents(subsystems);

					bool effects_applied = false;
					for(int32 i = 0; i <= subsystem_count; i++)
						effects_applied |= subsystems[i].DoPostProcesses(render_device, 
							PerFrameValues().m_delta_time, 
							Enums::_postprocess_render_stage_post_ui);		
				
					// it's probable that the last surface rendered to isn't halos primary buffer
					// in that case, stretchrect the result to the primary buffer
					if(effects_applied &&
						(Rasterizer::GlobalRenderTargets()[Enums::_rasterizer_target_render_primary].surface !=
							Globals().m_render_targets.scene_buffer_chain.GetSceneSurface()))
						render_device->StretchRect(
							Globals().m_render_targets.scene_buffer_chain.GetSceneSurface(), 
							NULL, 
							Rasterizer::GlobalRenderTargets()[Enums::_rasterizer_target_render_primary].surface,
							NULL, 
							D3DTEXF_NONE);
				};
				dx9_state_capture.Restore();

				render_device->SetRenderTarget(0, Rasterizer::GlobalRenderTargets()[Enums::_rasterizer_target_render_primary].surface);
			}
		}
	};
};
#endif