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

		void		s_per_frame_values::PollUpdate()
		{
			Rasterizer::s_render_frustum_matricies render_matricies;
			Rasterizer::RenderGlobals()->frustum.GetMatricies(render_matricies);

			render_matricies.world_view->ConvertTo4x4(m_matricies.world_view);
			render_matricies.world_view_transpose->ConvertTo4x4(m_matricies.world_view_transpose);

			m_hud_scale.value = Hud::HudIsScaled() ? m_hud_scale.scaled_ratio : m_hud_scale.default_ratio;

			m_clipping.near_clip = Rasterizer::RenderGlobals()->frustum.z_near;
			m_clipping.far_clip = Rasterizer::RenderGlobals()->frustum.z_far;

			m_delta_time = GameState::MainGlobals()->delta_time;
		}

		s_per_frame_values& PerFrameValues() { return s_per_frame_values::g_instance; }
		/////////////////////////////////////////////////////////////////////


		/////////////////////////////////////////////////////////////////////
		// s_postprocess_quad
		void		s_postprocess_quad::AllocateResources(IDirect3DDevice9* pDevice)
		{
			SetupQuad(pDevice,  m_quad->x_segs, m_quad->y_segs);
		}
		void		s_postprocess_quad::ReleaseResources()
		{
			Yelo::safe_release(m_quad->runtime.vertex_buffer);
			Yelo::safe_release(m_quad->runtime.index_buffer);
		}
		void		s_postprocess_quad::SetSource(TagGroups::s_shader_postprocess_effect_render_quad* quad)
		{
			m_quad = quad;
		}
		bool		s_postprocess_quad::SetupQuad(IDirect3DDevice9* pDevice, int32 XSegments, int32 YSegments)
		{
			this->ReleaseResources();

			m_quad->x_segs = (XSegments < 1 ? 1 : XSegments);
			m_quad->y_segs = (YSegments < 1 ? 1 : YSegments);
			m_quad->x_segs = (m_quad->x_segs > k_maximum_quads_per_axis ? k_maximum_quads_per_axis : m_quad->x_segs);
			m_quad->y_segs = (m_quad->y_segs > k_maximum_quads_per_axis ? k_maximum_quads_per_axis : m_quad->y_segs);

			m_quad->quad_count = (m_quad->x_segs * m_quad->y_segs);
			m_quad->vertex_count = m_quad->quad_count * 4;
			m_quad->primitive_count = m_quad->quad_count * 2;

			pDevice->CreateVertexBuffer(
				m_quad->vertex_count * sizeof(s_postprocess_vertex), 
				D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, 
				D3DFVF_XYZ | D3DFVF_TEX2, 
				D3DPOOL_DEFAULT, 
				&m_quad->runtime.vertex_buffer,
				NULL);
			pDevice->CreateIndexBuffer(
				(m_quad->primitive_count * 3) * sizeof(WORD),
				D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, 
				D3DFMT_INDEX16, 
				D3DPOOL_DEFAULT, 
				&m_quad->runtime.index_buffer, 
				NULL);

			s_postprocess_vertex* verticies;
			m_quad->runtime.vertex_buffer->Lock( 0, 0, (void**)&verticies, D3DLOCK_DISCARD );

			WORD* indicies;
			m_quad->runtime.index_buffer->Lock( 0, 0, (void**)&indicies, D3DLOCK_DISCARD );

			const s_postprocess_globals::s_rendering& k_rendering = Globals().m_rendering;

			real quad_width = k_rendering.screen_dimensions.x / m_quad->x_segs;
			real quad_height = k_rendering.screen_dimensions.y / m_quad->y_segs;

			// The quads are treated as numerous small quads with 4 verts per quad. This isn't
			// the most efficient way as there are alot of duplicate verticies. But this is simpler.
			int32 quad_row = 0;
			int32 quad_column = 0;
			int32 quad_vertex = 0;
			for(int32 k = 0; k < m_quad->vertex_count; )
			{
				int32 x = 0;
				int32 y = 0;
				int32 z = 0;
				while (x < 4)
				{	
					real PointX = (quad_column * quad_width) + (quad_width * y);
					real PointY = (quad_row * quad_height) + (quad_height * z);
					verticies[k + x].x = PointX - 0.5f;
					verticies[k + x].y = -(PointY - 0.5f);
					verticies[k + x].z = 10.0f;

					verticies[k + x].tu0 = PointX / k_rendering.screen_dimensions.x;
 					verticies[k + x].tv0 = PointY / k_rendering.screen_dimensions.y;
 					verticies[k + x].tu1 = PointX / k_rendering.screen_dimensions.x;
 					verticies[k + x].tv1 = PointY / k_rendering.screen_dimensions.y;

					y++;
					if(y > 1)
					{	
						z++;
						y = 0;
					}
					x++;
				}

				int32 a = 0;
				while (a < 2)
				{
					indicies[(quad_row * (m_quad->x_segs * 6)) + (quad_column * 6) + (a * 3)]		= CAST(WORD, k + (a * 3));
					indicies[(quad_row * (m_quad->x_segs * 6)) + (quad_column * 6) + (a * 3) + 1]	= CAST(WORD, k + 1 + (a * 1));
					indicies[(quad_row * (m_quad->x_segs * 6)) + (quad_column * 6) + (a * 3) + 2]	= CAST(WORD, k + 2 - (a * 1));
					a++;
				}

				quad_column++;
				if(quad_column > m_quad->x_segs - 1)
				{
					quad_column = 0;
					quad_row++;
				}
				k += x;
			}

			m_quad->runtime.vertex_buffer->Unlock();
			m_quad->runtime.index_buffer->Unlock();

			return true;
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

			strcat_s(file_string, MAX_PATH, Settings::OpenSauceProfilePath());
			strcat_s(file_string, MAX_PATH, "shaders\\pp_shaders.shd");

			Globals().m_shader_file.OpenFile(file_string);
		}
		void		Dispose()
		{
			Globals().m_shader_file.CloseFile();
		}

		void		Initialize3D(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters)
		{
			Globals().m_rendering.render_device = pDevice;
			Globals().m_rendering.creation_parameters = *pParameters;

			Yelo::Main::s_postprocessing_subsystem_component* subsystems;
			const int32 subsystem_count = Yelo::Main::GetPostprocessingSubsystemComponents(subsystems);

			for(int32 i = 0; i <= subsystem_count; i++)
				subsystems[i].Initialize(pDevice, pParameters);

			AllocateResources(pParameters);

			for(int32 i = 0; i <= subsystem_count; i++)
				subsystems[i].AllocateResources(Globals().m_rendering.render_device);
		}	
		void		OnLostDevice()
		{
			Yelo::Main::s_postprocessing_subsystem_component* subsystems;
			const int32 subsystem_count = Yelo::Main::GetPostprocessingSubsystemComponents(subsystems);

			for(int32 i = 0; i <= subsystem_count; i++)
				subsystems[i].OnLostDevice();

			ReleaseResources();
		}
		void		OnResetDevice(D3DPRESENT_PARAMETERS* pParameters)
		{
			AllocateResources(pParameters);

			Yelo::Main::s_postprocessing_subsystem_component* subsystems;
			const int32 subsystem_count = Yelo::Main::GetPostprocessingSubsystemComponents(subsystems);

			for(int32 i = 0; i <= subsystem_count; i++)
			{
				subsystems[i].AllocateResources(Globals().m_rendering.render_device);
				// this might not be necessary considering all DX resources are released when the device is lost
				subsystems[i].OnResetDevice(Globals().m_rendering.render_device, pParameters);
			}
		}
		void		Render()
		{
			Yelo::Main::s_postprocessing_subsystem_component* subsystems;
			const int32 subsystem_count = Yelo::Main::GetPostprocessingSubsystemComponents(subsystems);

			for(int32 i = 0; i <= subsystem_count; i++)
				subsystems[i].Render();
		}
		void		Release()
		{
			ReleaseResources();
		}


		void		LoadSettings(TiXmlElement* dx9_element)
		{
			ZeroMemory(&Globals(), sizeof(Globals()));

			Yelo::Main::s_postprocessing_subsystem_component* subsystems;
			const int32 subsystem_count = Yelo::Main::GetPostprocessingSubsystemComponents(subsystems);

			for(int32 i = 0; i <= subsystem_count; i++)
				subsystems[i].SetDefaultSettings();

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

			Yelo::Main::s_postprocessing_subsystem_component* subsystems;
			const int32 subsystem_count = Yelo::Main::GetPostprocessingSubsystemComponents(subsystems);

			for(int32 i = 0; i <= subsystem_count; i++)
				subsystems[i].InitializeForNewMap();
		}
		void		DisposeFromOldMap()
		{
			Globals().m_map_postprocess_globals = NULL;

			Yelo::Main::s_postprocessing_subsystem_component* subsystems;
			const size_t subsystem_count = Yelo::Main::GetPostprocessingSubsystemComponents(subsystems);

			for(size_t i = 0; i <= subsystem_count; i++)
				subsystems[i].DisposeFromOldMap();
		}
		void		Update(real delta_time)
		{			
			ActivationVariables().PollUpdate();

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
				Globals().m_rendering.screen_dimensions.x = (real)pParameters->BackBufferWidth;
				Globals().m_rendering.screen_dimensions.y = (real)pParameters->BackBufferHeight;
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
					pParameters, pParameters->BackBufferFormat);

				// Create the original scene buffer and get the gbuffer render targets
				Globals().m_render_targets.scene_render_target.CreateTarget(Globals().m_rendering.render_device, 
					pParameters, pParameters->BackBufferFormat);
				Globals().m_render_targets.gbuffer = DX9::c_gbuffer_system::GBuffer();
				
				// Create the orthographic projection matrix for rendering the quads
				D3DXMATRIX OrthoTranslation;
				D3DXMatrixTranslation(&OrthoTranslation, -1.0f, 1.0f, 0);
				D3DXMatrixOrthoLH (&Globals().m_matricies.ortho_proj_matrix, 
					Globals().m_rendering.screen_dimensions.x, 
					Globals().m_rendering.screen_dimensions.y, 
					1.0f, 10.0f);
				D3DXMatrixMultiply(&Globals().m_matricies.ortho_proj_matrix, &Globals().m_matricies.ortho_proj_matrix, &OrthoTranslation);
				
				// Load and create the fade effect shader
				Globals().m_fade.fade_loaded = true;
				uint32 data_size;
				void* data_pointer = Globals().m_shader_file.GetDataPointer("PP_EffectFade", &data_size);
				if(data_size == 0 || data_pointer == NULL)
					Globals().m_fade.fade_loaded = false;
				else
				{
					LPD3DXBUFFER error_buffer = NULL;
					hr = D3DXCreateEffect(
						Globals().m_rendering.render_device, 
						data_pointer, 
						data_size, 
						NULL, NULL, NULL, NULL, 
						&Globals().m_fade.fade_shader, 
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
						Globals().m_fade.fade_loaded = false;
					}
					else
						Globals().m_fade.fade_shader->SetMatrix("c_ortho_wvp", &Globals().m_matricies.ortho_proj_matrix);
					Yelo::safe_release(error_buffer);	
				}

				Globals().m_flags.loaded = true;
			}
			return S_OK;
		}
		void		ReleaseResources()
		{
			Yelo::Main::s_postprocessing_subsystem_component* subsystems;
			const int32 subsystem_count = Yelo::Main::GetPostprocessingSubsystemComponents(subsystems);

			for(int32 i = 0; i <= subsystem_count; i++)
				subsystems[i].ReleaseResources();

			// Release global resources the subsystems might use
			Yelo::safe_release(Globals().m_fade.fade_shader);
			Globals().m_render_targets.scene_render_target.ReleaseTarget();
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

			return NULL;
		}
		void*		HS_Unload()
		{
			Yelo::Main::s_postprocessing_subsystem_component* subsystems;
			const int32 subsystem_count = Yelo::Main::GetPostprocessingSubsystemComponents(subsystems);

			for(int32 i = 0; i <= subsystem_count; i++)
				subsystems[i].Unload();

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
			}* args = CAST_PTR(s_arguments*, arguments);
			
			Subsystem::Internal::c_internal_subsystem::g_instance.SetEffectShaderVariableBoolean(
				args->effect_index,		args->script_variable_index, 
				args->value);
			
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
			}* args = CAST_PTR(s_arguments*, arguments);
			
			Subsystem::Internal::c_internal_subsystem::g_instance.SetEffectShaderVariableInteger(
				args->effect_index,		args->script_variable_index, 
				args->value);
			
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
			}* args = CAST_PTR(s_arguments*, arguments);
			
			Subsystem::Internal::c_internal_subsystem::g_instance.SetEffectShaderVariableReal(
				args->effect_index,		args->script_variable_index, 
				args->values[0], 
				args->values[1], 
				args->values[2], 
				args->values[3]);
			
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
			render_device->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);
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

			LPDIRECT3DSURFACE9 orig_target;
			render_device->GetRenderTarget(0, &orig_target);

			Rasterizer::s_render_target& rt = Rasterizer::GlobalRenderTargets()[Enums::_rasterizer_target_render_primary];
			if(rt.surface == NULL)
				return;	
			Globals().m_rendering.render_surface = rt.surface;

			s_dx9_render_state_capture dx9_state_capture;
			{
				RenderingCaptureAndSetState(render_device, dx9_state_capture, Enums::_postprocess_render_stage_pre_blur);

				Yelo::Main::s_postprocessing_subsystem_component* subsystems;
				const int32 subsystem_count = Yelo::Main::GetPostprocessingSubsystemComponents(subsystems);

				for(int32 i = 0; i <= subsystem_count; i++)
					subsystems[i].DoPostProcesses(Globals().m_rendering.render_device, PerFrameValues().m_delta_time, Enums::_postprocess_render_stage_pre_blur);
				Subsystem::MotionBlur::c_motionblur_subsystem::DoMotionBlurProcess(render_device, PerFrameValues().m_delta_time);
				for(int32 i = 0; i <= subsystem_count; i++)
					subsystems[i].DoPostProcesses(Globals().m_rendering.render_device, PerFrameValues().m_delta_time, Enums::_postprocess_render_stage_pre_alpha_blended);
			};
			dx9_state_capture.Restore();
			
			render_device->SetRenderTarget(0, orig_target);
			Yelo::safe_release(orig_target);
		}
		void		RenderPreHUD()
		{
			if (!Globals().m_flags.loaded) return;

			IDirect3DDevice9* render_device = Globals().m_rendering.render_device;

			LPDIRECT3DSURFACE9 orig_target;
			render_device->GetRenderTarget(0, &orig_target);

			Rasterizer::s_render_target& rt = Rasterizer::GlobalRenderTargets()[Enums::_rasterizer_target_render_primary];
			if(rt.surface == NULL)
				return;		
			Globals().m_rendering.render_surface = rt.surface;	

			s_dx9_render_state_capture dx9_state_capture;
			{
				RenderingCaptureAndSetState(render_device, dx9_state_capture, Enums::_postprocess_render_stage_pre_hud);

				Yelo::Main::s_postprocessing_subsystem_component* subsystems;
				const int32 subsystem_count = Yelo::Main::GetPostprocessingSubsystemComponents(subsystems);

				for(int32 i = 0; i <= subsystem_count; i++)
					subsystems[i].DoPostProcesses(render_device, PerFrameValues().m_delta_time, Enums::_postprocess_render_stage_pre_hud);
			};
			dx9_state_capture.Restore();

			Globals().m_rendering.render_device->SetRenderTarget(0, orig_target);
			Yelo::safe_release(orig_target);
		}
		void		RenderPreMenu() 
		{
			if (!Globals().m_flags.loaded) return;

			IDirect3DDevice9* render_device = Globals().m_rendering.render_device;

			LPDIRECT3DSURFACE9 orig_target;
			render_device->GetRenderTarget(0, &orig_target);		

			Rasterizer::s_render_target& rt = Rasterizer::GlobalRenderTargets()[Enums::_rasterizer_target_render_primary];
			if(rt.surface == NULL)
				return;			
			Globals().m_rendering.render_surface = rt.surface;

			s_dx9_render_state_capture dx9_state_capture;
			{
				RenderingCaptureAndSetState(render_device, dx9_state_capture, Enums::_postprocess_render_stage_pre_ui);

				Yelo::Main::s_postprocessing_subsystem_component* subsystems;
				const int32 subsystem_count = Yelo::Main::GetPostprocessingSubsystemComponents(subsystems);

				for(int32 i = 0; i <= subsystem_count; i++)
					subsystems[i].DoPostProcesses(render_device, PerFrameValues().m_delta_time, Enums::_postprocess_render_stage_pre_ui);
			};
			dx9_state_capture.Restore();

			render_device->SetRenderTarget(0, orig_target);
			Yelo::safe_release(orig_target);
		}
		void		RenderPostMenu() 
		{
			if (!Globals().m_flags.loaded) return;

			IDirect3DDevice9* render_device = Globals().m_rendering.render_device;

			LPDIRECT3DSURFACE9 orig_target;
			render_device->GetRenderTarget(0, &orig_target);

			Rasterizer::s_render_target& rt = Rasterizer::GlobalRenderTargets()[Enums::_rasterizer_target_render_primary];
			if(rt.surface == NULL)
				return;		
			Globals().m_rendering.render_surface = rt.surface;			

			s_dx9_render_state_capture dx9_state_capture;
			{
				RenderingCaptureAndSetState(render_device, dx9_state_capture, Enums::_postprocess_render_stage_post_ui);

				Yelo::Main::s_postprocessing_subsystem_component* subsystems;
				const int32 subsystem_count = Yelo::Main::GetPostprocessingSubsystemComponents(subsystems);

				for(int32 i = 0; i <= subsystem_count; i++)
					subsystems[i].DoPostProcesses(render_device, PerFrameValues().m_delta_time, Enums::_postprocess_render_stage_post_ui);
			};
			dx9_state_capture.Restore();
			
			render_device->SetRenderTarget(0, orig_target);
			Yelo::safe_release(orig_target);
		}
	};
};
#endif