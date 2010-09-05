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
#include "Rasterizer/PostProcessing/PostProcessingShader.hpp"
#if !PLATFORM_IS_DEDI
#include "Game/EngineFunctions.hpp"
#include "Game/GameState.hpp"
#include "Rasterizer/GBuffer.hpp"

namespace PP = Yelo::Postprocessing;

namespace Yelo
{
	namespace Postprocessing
	{
		void		c_postprocess_shader::Initialize(IDirect3DDevice9* pDevice)
		{
			UnloadStandardResources();				
			m_shader_base->runtime.flags.valid_shader_bit = false;
		}
		void		c_postprocess_shader::SetSource(void* pSource)
		{
			m_shader_base = CAST_PTR(TagGroups::s_shader_postprocess_definition*, pSource);
		}
		HRESULT		c_postprocess_shader::SetupShader()
		{
			// Set the shaders variables to default values
			m_shader_base->runtime.flags.valid_shader_bit = false;
			m_shader_base->runtime.flags.uses_gbuffer_bit = false;
			m_shader_base->runtime.dx_effect = NULL;
			m_shader_base->runtime.postprocess_handle = NULL;

			m_shader_base->ortho_wvp_matrix.ClearHandles();
			m_shader_base->wv_matrix.ClearHandles();
			m_shader_base->wvt_matrix.ClearHandles();
			m_shader_base->scene_size.ClearHandles();
			m_shader_base->hud_scale.ClearHandles();
			m_shader_base->pixel_size.ClearHandles();
			m_shader_base->near_clip_dist.ClearHandles();
			m_shader_base->far_clip_dist.ClearHandles();
			m_shader_base->frame_time.ClearHandles();
			m_shader_base->tex_source.ClearHandles();
			m_shader_base->tex_scene.ClearHandles();
			m_shader_base->tex_depth.ClearHandles();
			m_shader_base->tex_velocity.ClearHandles();
			m_shader_base->tex_normals.ClearHandles();
			m_shader_base->tex_index.ClearHandles();

			return S_OK;
		}

		HRESULT		c_postprocess_shader::LoadStandardResources(IDirect3DDevice9* pDevice)
		{
			// Loads the handles for base variables that are either necessary or globally available
			if(*m_effect == NULL)
				return E_FAIL;

			// Get the PostProcess technique handle
			// Shader must have a "PostProcess" technique
			m_shader_base->runtime.postprocess_handle = (*m_effect)->GetTechniqueByName("PostProcess");
			if (m_shader_base->runtime.postprocess_handle == NULL)
				return E_FAIL;
			(*m_effect)->SetTechnique(m_shader_base->runtime.postprocess_handle);

			m_shader_base->runtime.flags.uses_gbuffer_bit = false;

			PP::s_postprocess_globals& pp_globals = PP::Globals();
			s_per_frame_values& pf_values = PerFrameValues();

			// Shaders must have this for their vertex shader
			m_shader_base->ortho_wvp_matrix.Initialize(m_effect,		"ORTHOWORLDVIEWPROJECTION", &pp_globals.m_matricies.ortho_proj_matrix, true);
			if (!m_shader_base->ortho_wvp_matrix.IsUsed()) return E_FAIL;
			
			m_shader_base->wv_matrix.Initialize(m_effect,				"WORLDVIEW", &pf_values.m_matricies.world_view, true);
			m_shader_base->wvt_matrix.Initialize(m_effect,				"WORLDVIEWTRANSPOSE", &pf_values.m_matricies.world_view_transpose, true);
			
			m_shader_base->scene_size.Initialize(m_effect,				"SCENESIZE", &pp_globals.m_rendering.screen_dimensions, true);	
			m_shader_base->hud_scale.Initialize(m_effect,				"HUDSCALE", &pf_values.m_hud_scale.value, true);

			real pixel_size_temp[2] = {
				1.0f / pp_globals.m_rendering.screen_dimensions.x,
				1.0f / pp_globals.m_rendering.screen_dimensions.y
			};
			m_shader_base->pixel_size.Initialize(m_effect,				"PIXELSIZE", &pixel_size_temp[0], true);
			m_shader_base->near_clip_dist.Initialize(m_effect,			"NEARCLIPDISTANCE", &pf_values.m_clipping.near_clip, true);
			m_shader_base->far_clip_dist.Initialize(m_effect,			"FARCLIPDISTANCE", &pf_values.m_clipping.far_clip, true);

			// Made tex_scene optional, realized it's rarely used!
			m_shader_base->tex_scene.Initialize(m_effect,				"TEXSCENE", pp_globals.m_render_targets.scene_render_target.m_texture, true);
			if(!m_shader_base->tex_scene.IsUsed() && !pp_globals.m_render_targets.scene_render_target.m_enabled) return E_FAIL;

			m_shader_base->tex_source.Initialize(m_effect,				"TEXSOURCE", pp_globals.m_render_targets.scene_render_target.m_texture, true);
			if(m_shader_base->tex_source.IsUsed() && !pp_globals.m_render_targets.scene_buffer_chain.IsAvailable()) return E_FAIL;

			m_shader_base->tex_depth.Initialize(m_effect,				"TEXDEPTH", pp_globals.m_render_targets.gbuffer->m_rt_depth.m_texture, true);
			if(m_shader_base->tex_depth.IsUsed() && !pp_globals.m_render_targets.gbuffer->m_rt_depth.m_enabled) return E_FAIL;
			else m_shader_base->runtime.flags.uses_gbuffer_bit |= m_shader_base->tex_depth.IsUsed();

			m_shader_base->tex_velocity.Initialize(m_effect,			"TEXVELOCITY", pp_globals.m_render_targets.gbuffer->m_rt_velocity.m_texture, true);
			if(m_shader_base->tex_velocity.IsUsed() && !pp_globals.m_render_targets.gbuffer->m_rt_velocity.m_enabled) return E_FAIL;
			else m_shader_base->runtime.flags.uses_gbuffer_bit |= m_shader_base->tex_velocity.IsUsed();

			m_shader_base->tex_normals.Initialize(m_effect,				"TEXNORMALS", pp_globals.m_render_targets.gbuffer->m_rt_normals.m_texture, true);
			if(m_shader_base->tex_normals.IsUsed() && !pp_globals.m_render_targets.gbuffer->m_rt_normals.m_enabled) return E_FAIL;
			else m_shader_base->runtime.flags.uses_gbuffer_bit |= m_shader_base->tex_normals.IsUsed();

			m_shader_base->tex_index.Initialize(m_effect,				"TEXINDEX", pp_globals.m_render_targets.gbuffer->m_rt_index.m_texture, true);
			if(m_shader_base->tex_index.IsUsed() && !pp_globals.m_render_targets.gbuffer->m_rt_index.m_enabled) return E_FAIL;
			else m_shader_base->runtime.flags.uses_gbuffer_bit |= m_shader_base->tex_index.IsUsed();

			return S_OK;
		}
		HRESULT		c_postprocess_shader::AllocateResources(IDirect3DDevice9* pDevice)
		{
			// Loads the shader, sets up the standard variables and any custom variables in derived classes
			m_shader_base->runtime.flags.valid_shader_bit = false;
			if(FAILED(LoadShader(pDevice)))
			{		
				if(m_effect)
					Yelo::safe_release(*m_effect);
				return E_FAIL;
			}

			// Check that the shader can be used on this device
			D3DXEFFECT_DESC effect_description;
			LPD3DXEFFECT effect = *m_effect;
			effect->GetDesc(&effect_description);
			for (UINT i = 0; i < effect_description.Techniques; i++)
			{
				D3DXHANDLE technique = effect->GetTechnique(i);
				D3DXTECHNIQUE_DESC technique_description;
				effect->GetTechniqueDesc(technique, &technique_description);
				for (UINT j = 0; j < technique_description.Passes; j++)
				{
					D3DXHANDLE pass = effect->GetPass(technique, j);
					D3DXPASS_DESC pass_description;
					effect->GetPassDesc(pass, &pass_description);
					D3DCAPS9 device_caps;
					pDevice->GetDeviceCaps(&device_caps);
					union{
						DWORD version;
						struct{
							union{
								WORD major_minor_version;
								struct{
									char minor_version;
									char major_version;
								};
							};
							WORD type;
						};
					} pass_vs_version, pass_ps_version;
					pass_vs_version.version = D3DXGetShaderVersion(pass_description.pVertexShaderFunction);
					pass_ps_version.version = D3DXGetShaderVersion(pass_description.pPixelShaderFunction);
					// check that the graphics device supports the intended shader model
					bool ps_version_ok = !(pass_vs_version.version > device_caps.VertexShaderVersion);
					bool vs_version_ok = !(pass_ps_version.version > device_caps.PixelShaderVersion);
					if(!ps_version_ok || !vs_version_ok)
					{
						if(GameState::DevmodeEnabled())
						{
							byte previous_dev_mode = *GameState::DeveloperMode();
							*GameState::DeveloperMode() = 4;
							Engine::Console::TerminalPrintF(
								"Error: failed to load shader \"%s\", the device does not support %s shader %i.%i",
								m_shader_id, 
								(ps_version_ok ? "vertex" : "pixel"), 
								(ps_version_ok ? pass_vs_version.major_version : pass_ps_version.major_version),
								(ps_version_ok ? pass_vs_version.minor_version : pass_ps_version.minor_version)
							);
							*GameState::DeveloperMode() = previous_dev_mode;
						}
						return E_FAIL;
					}

					// if either the pixel or vertex shader version is 3.0 or above, then ensure that
					// they are both of the same version.
					if((pass_ps_version.version >= D3DPS_VERSION(3,0) || pass_vs_version.version >= D3DPS_VERSION(3,0))
						&& (pass_ps_version.major_minor_version != pass_vs_version.major_minor_version))
					{
						if(GameState::DevmodeEnabled())
						{
							byte previous_dev_mode = *GameState::DeveloperMode();
							*GameState::DeveloperMode() = 4;
							Engine::Console::TerminalPrintF(
								"Error: failed to load shader \"%s\", the vertex and pixel shader models are incompatible (VS: %i.%i PS: %i.%i)",
								m_shader_id, 
								pass_vs_version.major_version, pass_vs_version.minor_version,
								pass_ps_version.major_version, pass_ps_version.minor_version
							);
							*GameState::DeveloperMode() = previous_dev_mode;
						}
						return E_FAIL;
					}
				}
			}

			Yelo::Engine::TagGroups::PredictResources(m_shader_base->predicted_resources);

			HRESULT_ERETURN(LoadStandardResources(pDevice));
			HRESULT_ERETURN(LoadCustomResources(pDevice));
				
			m_shader_base->runtime.flags.valid_shader_bit = true;
			return S_OK;
		}
		void		c_postprocess_shader::UnloadStandardResources()
		{
			// Reset the standard variable handles
			m_shader_base->runtime.postprocess_handle = NULL;
			m_shader_base->ortho_wvp_matrix.ClearHandles();
			m_shader_base->wv_matrix.ClearHandles();
			m_shader_base->wvt_matrix.ClearHandles();
			m_shader_base->scene_size.ClearHandles();
			m_shader_base->hud_scale.ClearHandles();
			m_shader_base->pixel_size.ClearHandles();	
			m_shader_base->near_clip_dist.ClearHandles();
			m_shader_base->far_clip_dist.ClearHandles();
			m_shader_base->tex_source.ClearHandles();
			m_shader_base->tex_scene.ClearHandles();
			m_shader_base->tex_depth.ClearHandles();
			m_shader_base->tex_velocity.ClearHandles();
			m_shader_base->tex_normals.ClearHandles();
			m_shader_base->tex_index.ClearHandles();
		}
		void		c_postprocess_shader::ReleaseResources()		
		{
			UnloadShader();
			UnloadStandardResources();
			UnloadCustomResources();
			m_shader_base->runtime.flags.valid_shader_bit = false;
		}
		void		c_postprocess_shader::OnLostDevice()
		{		
			if(m_effect != NULL && *m_effect != NULL)
				(*m_effect)->OnLostDevice();

			m_shader_base->runtime.flags.valid_shader_bit = false;
		}
		void		c_postprocess_shader::OnResetDevice(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters)
		{
			if(m_effect != NULL && *m_effect != NULL)
				(*m_effect)->OnResetDevice();
		}
		HRESULT		c_postprocess_shader::DoPostProcess(IDirect3DDevice9* pDevice,
								double frame_time,
								TagGroups::s_shader_postprocess_effect_render_quad* quad)
		{
			if(!m_shader_base->runtime.flags.valid_shader_bit)		return E_FAIL;

			if(m_shader_base->runtime.flags.uses_gbuffer_bit && !DX9::c_gbuffer_system::g_system_enabled) return E_FAIL;
			
			// do any custom pre render process added by derived classes
			HRESULT_ERETURN(DoPreRender(pDevice, frame_time));

			// set the vertex declaration
			pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_TEX2);
			
			// set any standard variables that change per frame
			m_shader_base->wv_matrix.SetVariable(m_effect,		&PerFrameValues().m_matricies.world_view);
			m_shader_base->wvt_matrix.SetVariable(m_effect,		&PerFrameValues().m_matricies.world_view_transpose);
			m_shader_base->hud_scale.SetVariable(m_effect,		&PerFrameValues().m_hud_scale.value);
			m_shader_base->near_clip_dist.SetVariable(m_effect, &PerFrameValues().m_clipping.near_clip);
			m_shader_base->far_clip_dist.SetVariable(m_effect,	&PerFrameValues().m_clipping.far_clip);

			// draw the quad
			UINT num_passes;
			LPD3DXEFFECT effect = *m_effect;
			HRESULT_ERETURN(effect->Begin( &num_passes, 0 ));

			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);

			PP::s_postprocess_globals::s_render_targets& pp_globals_rts = PP::Globals().m_render_targets;
			for(UINT pass = 0; pass < num_passes; ++pass )
			{
				// do per pass code. if the shader is changed call CommitChanges.
				DoPerPass(pDevice, frame_time, pass);

				// Set tex_source to either the previous render target, or the saved scene if this is the first pass
				if(m_shader_base->tex_source.IsUsed())
					m_shader_base->tex_source.SetVariable(&effect,
						pp_globals_rts.scene_buffer_chain.m_first_render ? 
							pp_globals_rts.scene_render_target.m_texture : 
							pp_globals_rts.scene_buffer_chain.GetNextSource());
				
				pDevice->SetRenderTarget( 0, pp_globals_rts.scene_buffer_chain.GetNextTarget() );				
				
				pDevice->Clear( 0L, NULL, D3DCLEAR_TARGET, 0x00000000, 1.0f, 0L );
				effect->BeginPass( pass );
				{
					pDevice->SetIndices(quad->runtime.index_buffer);
					pDevice->SetStreamSource(0, quad->runtime.vertex_buffer, 0, sizeof( *quad ));

					pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, quad->vertex_count, 0, quad->primitive_count);
				};
				effect->EndPass();

				pp_globals_rts.scene_buffer_chain.m_first_render = false;
				pp_globals_rts.scene_buffer_chain.Flip();
			}
			effect->End();

			// do any custom post render process added by derived classes
			HRESULT_ERETURN(DoPostRender(pDevice, frame_time));

			return S_OK;
		}
	};
};
#endif