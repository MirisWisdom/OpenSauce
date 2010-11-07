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
#include "Rasterizer/GBuffer.hpp"
#if !PLATFORM_IS_DEDI
#include <Common/Halo1/YeloSharedSettings.hpp>
#include "Memory/MemoryInterface.hpp"
#include "Rasterizer/Rasterizer.hpp"
#include "Rasterizer/ShaderExtension/ShaderExtension.hpp"
#include "Rasterizer/PostProcessing/PostProcessingDebug.hpp"
#include "Game/GameEngine.hpp"
#include "Game/GameState.hpp"
#include "Game/EngineFunctions.hpp"
#include "Objects/Objects.hpp"

namespace Yelo
{
	namespace DX9
	{
#define __EL_INCLUDE_ID			__EL_INCLUDE_RASTERIZER_DX9
#define __EL_INCLUDE_FILE_ID	__EL_RASTERIZER_DX9_GBUFFER
#include "Memory/_EngineLayout.inl"

		//////////////////////////////////////////////////////////////////////
		// s_gbuffer
		void		c_gbuffer::ReleaseTargets()
		{
			m_rt_depth.ReleaseTarget();
			m_rt_velocity.ReleaseTarget();
			m_rt_normals_index.ReleaseTarget();
		}

		void		c_gbuffer::ClearTargets(IDirect3DDevice9* pDevice)
		{
			m_rt_depth.ClearTarget(pDevice, NULL, 1);
			m_rt_velocity.ClearTarget(pDevice, NULL, 1);
			m_rt_normals_index.ClearTarget(pDevice, NULL, 1);
		}
		bool		c_gbuffer::SetEffectVar(LPD3DXEFFECT& effect,
			cstring texture_semantic,
			Rasterizer::s_render_target& target,
			cstring x_handle_semantic, const int x_index,
			cstring y_handle_semantic, const int y_index,
			cstring z_handle_semantic, const int z_index,
			cstring w_handle_semantic, const int w_index)
		{				
			if(!effect) return false;

			D3DXHANDLE tex_handle = effect->GetParameterBySemantic(NULL, texture_semantic);
			if(!tex_handle)								return true;
			else if(tex_handle && !target.IsEnabled())	return false;

			effect->SetTexture(tex_handle, target.texture);

			D3DXHANDLE index_handle = NULL;

			if(x_handle_semantic)
			{
				index_handle = effect->GetParameterBySemantic(NULL, x_handle_semantic);
				if(!index_handle)
					return false;
				effect->SetInt(index_handle, x_index);
			}
			if(y_handle_semantic)
			{
				index_handle = effect->GetParameterBySemantic(NULL, y_handle_semantic);
				if(!index_handle)
					return false;
				effect->SetInt(index_handle, y_index);
			}
			if(z_handle_semantic)
			{
				index_handle = effect->GetParameterBySemantic(NULL, z_handle_semantic);
				if(!index_handle)
					return false;
				effect->SetInt(index_handle, z_index);
			}
			if(w_handle_semantic)
			{
				index_handle = effect->GetParameterBySemantic(NULL, w_handle_semantic);
				if(!index_handle)
					return false;
				effect->SetInt(index_handle, w_index);
			}
			
			return true;

		}
		bool		c_gbuffer::SetDepth(LPD3DXEFFECT& effect)
		{
			return SetEffectVar(effect, "TEXDEPTH", m_rt_depth, 
				"GBUFFER_DEPTH_X", k_gbuffer_depth_x);
		}

		bool		c_gbuffer::SetVelocity(LPD3DXEFFECT& effect)
		{
			return SetEffectVar(effect, "TEXVELOCITY", m_rt_velocity, 
				"GBUFFER_VELOCITY_X", k_gbuffer_velocity_x, 
				"GBUFFER_VELOCITY_Y", k_gbuffer_velocity_y);
		}
		bool		c_gbuffer::SetNormals(LPD3DXEFFECT& effect)
		{
			return SetEffectVar(effect, "TEXNORMALS", m_rt_normals_index, 
				"GBUFFER_NORMALS_X", k_gbuffer_normals_x, 
				"GBUFFER_NORMALS_Y", k_gbuffer_normals_y);
		}
		bool		c_gbuffer::SetIndex(LPD3DXEFFECT& effect)
		{
			return SetEffectVar(effect, "TEXINDEX", m_rt_normals_index, 
				"GBUFFER_INDEX_X", k_gbuffer_index_x, 
				"GBUFFER_INDEX_Y", k_gbuffer_index_y);
		}
		//////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////
		// c_gbuffer_debug_effect
		static c_gbuffer_debug_effect g_gbuffer_debug;

		c_gbuffer_debug_effect& GBufferDebug() { return g_gbuffer_debug; }


		HRESULT		c_gbuffer_debug_effect::AllocateResources(IDirect3DDevice9* device, uint32 width, uint32 height)
		{			
			m_technique_single =	m_effect->GetTechniqueByName("DebugRTSingle");
			m_technique_all =		m_effect->GetTechniqueByName("DebugRTAll");

			m_target_handle =		m_effect->GetParameterByName(NULL, "RenderTarget");

			m_depth_set =			c_gbuffer_system::GBuffer().SetDepth(m_effect);
			m_velocity_set =		c_gbuffer_system::GBuffer().SetVelocity(m_effect);
			m_normals_set =			c_gbuffer_system::GBuffer().SetNormals(m_effect);
			m_index_set =			c_gbuffer_system::GBuffer().SetIndex(m_effect);

			TEXTURE_VERTEX quad[4] = 
			{
				{ -0.5f,			-0.5f,			1.0f, 1.0f, 0.0f, 0.0f },
				{ width - 0.5f,		-0.5,			1.0f, 1.0f, 1.0f, 0.0f },
				{ -0.5,				height - 0.5f,	1.0f, 1.0f, 0.0f, 1.0f },
				{ width - 0.5f,		height - 0.5f,	1.0f, 1.0f, 1.0f, 1.0f }
			};
			memcpy_s(m_vertices, sizeof(m_vertices), quad, sizeof(quad));

			return (IsAvailable() ? S_OK : E_FAIL);
		}
		void		c_gbuffer_debug_effect::ReleaseResources()
		{
			safe_release(m_effect);
			
			m_technique_single = NULL;
			m_technique_all = NULL;

			m_target_handle = NULL;

			m_depth_set = false;
			m_velocity_set = false;
			m_normals_set = false;
			m_index_set = false;
		}
		void		c_gbuffer_debug_effect::OnLostDevice()
		{
			if(m_effect)
				m_effect->OnLostDevice();
		}
		HRESULT		c_gbuffer_debug_effect::OnResetDevice()
		{
			HRESULT hr = S_OK;
			if(m_effect)
				hr = m_effect->OnResetDevice();
			return hr;
		}
		void		c_gbuffer_debug_effect::Render(IDirect3DDevice9* device, int16 debug_target)
		{
			if(!IsAvailable()) return;

			DWORD old_depthbias, old_fillmode, old_srcblend, old_dest_blend, old_zenable, old_zwriteenable, old_stencilenable;

			device->GetRenderState(D3DRS_DEPTHBIAS, &old_depthbias);
			device->GetRenderState(D3DRS_FILLMODE, &old_fillmode);
			device->GetRenderState(D3DRS_SRCBLEND, &old_srcblend);
			device->GetRenderState(D3DRS_DESTBLEND, &old_dest_blend);	
			device->GetRenderState(D3DRS_ZENABLE, &old_zenable);	
			device->GetRenderState(D3DRS_ZWRITEENABLE, &old_zwriteenable);	
			device->GetRenderState(D3DRS_STENCILENABLE, &old_stencilenable);

			device->SetRenderState(D3DRS_DEPTHBIAS, 0);
			device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
			device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
			device->SetRenderState(D3DRS_ZENABLE, false);
			device->SetRenderState(D3DRS_ZWRITEENABLE, false);
			device->SetRenderState(D3DRS_STENCILENABLE, false);

			UINT cPasses, p;
			device->SetFVF( D3DFVF_XYZRHW | D3DFVF_TEX1 );

			Rasterizer::s_render_target& rt = Rasterizer::GlobalRenderTargets()[Enums::_rasterizer_target_render_primary];
			device->SetRenderTarget(0, rt.surface);

			m_effect->SetTechnique( debug_target == NONE ? 
				m_technique_all : m_technique_single);

			m_effect->SetInt(m_target_handle, debug_target);

			m_effect->Begin( &cPasses, 0 );
			for( p = 0; p < cPasses; ++p )
			{	
				m_effect->BeginPass( p );
				device->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, m_vertices, sizeof( TEXTURE_VERTEX ) );
				m_effect->EndPass();
			}
			m_effect->End();

			device->SetRenderState(D3DRS_DEPTHBIAS, old_depthbias);
			device->SetRenderState(D3DRS_FILLMODE, old_fillmode);
			device->SetRenderState(D3DRS_SRCBLEND, old_srcblend);
			device->SetRenderState(D3DRS_DESTBLEND, old_dest_blend);
			device->SetRenderState(D3DRS_ZENABLE, old_zenable);	
			device->SetRenderState(D3DRS_ZWRITEENABLE, old_zwriteenable);	
			device->SetRenderState(D3DRS_STENCILENABLE, old_stencilenable);
		}
		bool		c_gbuffer_debug_effect::IsAvailable()
		{
			return (
				(m_effect != NULL) &&
				(m_technique_single != NULL) &&
				(m_technique_all != NULL) &&
				(m_target_handle != NULL) &&
				m_depth_set &&
				m_velocity_set &&
				m_normals_set &&
				m_index_set
				);
		}
		//////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////
		// c_gbuffer_system interface
		Enums::rasterizer_vertex_shader const c_gbuffer_system::kValidShaders[] = {
			Enums::_rasterizer_vs_environment_lightmap,
			Enums::_rasterizer_vs_model_fogged,
			Enums::_rasterizer_vs_model,
			Enums::_rasterizer_vs_model_fast,
			Enums::_rasterizer_vs_model_scenery,
		};

		API_FUNC_NAKED void c_gbuffer_system::Hook_RenderWindow()
		{ 
			static uint32 CALL_ADDRESS = GET_FUNC_PTR(RENDER_WINDOW);
			static uint32 RETN_ADDRESS = GET_FUNC_PTR(RENDER_WINDOW_CALL_RETN);

			__asm {
				mov			c_gbuffer_system::g_is_rendering_reflection, 0
				call		CALL_ADDRESS
				mov			c_gbuffer_system::g_is_rendering_reflection, 1
				jmp			RETN_ADDRESS
			}
		}

		API_FUNC_NAKED void c_gbuffer_system::Hook_RenderObjectList_GetObjectIndex()
		{ 
			static uint32 RETN_ADDRESS = GET_FUNC_PTR(RENDER_OBJECT_LIST_HOOK_RETN);

			__asm {
				mov		c_gbuffer_system::g_object_index, ax
				and     eax, 0FFFFh
				lea     edi, [eax+eax*2]
				jmp		RETN_ADDRESS
			}
		}
		API_FUNC_NAKED void c_gbuffer_system::Hook_RenderObjectList_ClearObjectIndex()
		{ 
			__asm {
				mov		c_gbuffer_system::g_object_index, 0FFFFh
				retn
			}
		}
		API_FUNC_NAKED void c_gbuffer_system::Hook_FirstPersonWeaponDraw_GetObjectIndex()
		{ 
			static uint32 RETN_ADDRESS = GET_FUNC_PTR(FIRST_PERSON_WEAPON_DRAW_HOOK_RETN);

			__asm {
				mov     edx, [edx+34h]
				mov     eax, esi
				mov		c_gbuffer_system::g_object_index, ax
				jmp		RETN_ADDRESS
			}
		}

		void		c_gbuffer_system::Ctor(cstring package_file)
		{
			m_shader_package.OpenFile(package_file);
		}
		void		c_gbuffer_system::Initialize()	
		{	
			char file_string[MAX_PATH];
			file_string[0] = '\0';

			strcat_s(file_string, MAX_PATH, Settings::OpenSauceProfilePath());
			strcat_s(file_string, MAX_PATH, "shaders\\gbuffer_shaders.shd");

			g_default_system.Ctor(file_string);
			

			Memory::WriteRelativeJmp(&Hook_RenderWindow, 
				GET_FUNC_VPTR(RENDER_WINDOW_CALL), true);

			Memory::WriteRelativeJmp(&Hook_RenderObjectList_GetObjectIndex, 
				GET_FUNC_VPTR(RENDER_OBJECT_LIST_HOOK), true);
			Memory::WriteRelativeJmp(&Hook_RenderObjectList_ClearObjectIndex, 
				GET_FUNC_VPTR(RENDER_OBJECT_LIST_END_HOOK), true);
			Memory::WriteRelativeJmp(&Hook_FirstPersonWeaponDraw_GetObjectIndex, 
				GET_FUNC_VPTR(FIRST_PERSON_WEAPON_DRAW_HOOK), true);

			char NOP = 0x90;
			byte* call_address;

			call_address = CAST_PTR(byte*, GET_FUNC_VPTR(RASTERIZER_DRAW_STATIC_TRIANGLES_STATIC_VERTICES__DRAW_INDEXED_PRIMITIVE_HOOK));
			Memory::WriteRelativeCall(&DrawIndexedPrimitive_Object, 
				call_address, true);
			Memory::WriteMemory(call_address + 5, &NOP, sizeof(NOP));

			call_address = CAST_PTR(byte*, GET_FUNC_VPTR(RASTERIZER_DRAW_DYNAMIC_TRIANGLES_STATIC_VERTICES2__DRAW_INDEXED_PRIMITIVE_HOOK));
			Memory::WriteRelativeCall(&DrawIndexedPrimitive_Structure, 
				call_address, true);
			Memory::WriteMemory(call_address + 5, &NOP, sizeof(NOP));

			call_address = CAST_PTR(byte*, GET_FUNC_VPTR(RASTERIZER_SET_WORLD_VIEW_PROJECTION_MATRIX_VERTEX_CONSTANT_HOOK));
			Memory::WriteRelativeCall(&SetVertexShaderConstantF_WVP, 
				call_address, true);
			Memory::WriteMemory(call_address + 5, &NOP, sizeof(NOP));
		}
		void		c_gbuffer_system::Dispose()		{}

		void		c_gbuffer_system::LoadSettings(TiXmlElement* dx9_element)
		{
			// setup default values
			g_system_enabled = true;

			if(dx9_element != NULL)
			{
				TiXmlElement* element = dx9_element->FirstChildElement("GBuffer");

				if(element != NULL)
					g_system_enabled = Settings::ParseBoolean( element->Attribute("enabled") );
			}
		}

		void		c_gbuffer_system::SaveSettings(TiXmlElement* dx9_element)
		{
			TiXmlElement* gbuffer_element = NULL;

			gbuffer_element = new TiXmlElement("GBuffer");
				dx9_element->LinkEndChild(gbuffer_element);

			gbuffer_element->SetAttribute("enabled", Settings::BooleanToString(g_system_enabled));
		}

		void		c_gbuffer_system::Initialize3D(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters)
		{
			g_debug_index = 0;
			g_default_system.AllocateResources(pDevice, pParameters);	
		}

		void		c_gbuffer_system::OnLostDevice()
		{
			g_default_system.OnLostDeviceImpl();
		}

		void		c_gbuffer_system::OnResetDevice(D3DPRESENT_PARAMETERS* pParameters)
		{
			g_default_system.OnResetDeviceImpl(pParameters);
		}

		void		c_gbuffer_system::Render()	{}

		void		c_gbuffer_system::Release() 
		{
			g_default_system.ReleaseResources();
		}


		void		c_gbuffer_system::VertexShaderChanged(IDirect3DVertexShader9* pShader)
		{
			g_default_system.VertexShaderChangedImpl(pShader);
		}

		HRESULT	 	c_gbuffer_system::DrawIndexedPrimitive(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE Type,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount)
		{
			HRESULT hr;
			// Get the current render target
			LPDIRECT3DSURFACE9 origTarget;
			pDevice->GetRenderTarget(0, &origTarget);

			LPDIRECT3DVERTEXSHADER9 origVShader;
			pDevice->GetVertexShader(&origVShader);

			// Using the transpose of the world view for normal output
			Rasterizer::s_render_frustum_matricies Matricies;
			Rasterizer::RenderGlobals()->frustum.GetMatricies(Matricies);

			D3DXMATRIX WorldViewT;
			Matricies.world_view_transpose->ConvertTo4x4(WorldViewT);
			pDevice->SetVertexShaderConstantF(96+4, WorldViewT, 4); 

			// We don't want to write to the Z-Buffer as it's already been done
			DWORD ZWriteEnable;
			pDevice->GetRenderState(D3DRS_ZWRITEENABLE, &ZWriteEnable);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);

			hr = g_default_system.DrawIndexedPrimitive_Impl(pDevice, Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);

			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, ZWriteEnable);

			pDevice->SetVertexShader(origVShader);
			Yelo::safe_release(origVShader); 

			pDevice->SetRenderTarget(0, origTarget);
			Yelo::safe_release(origTarget);

			return hr;
		}

		HRESULT	 	c_gbuffer_system::DrawIndexedPrimitive_Structure(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE Type,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount)
		{
			HRESULT hr = pDevice->DrawIndexedPrimitive(Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
			
			if(g_default_system.IsRenderable() && 
				!g_is_rendering_reflection && g_current_render_state == Enums::_render_progress_structure)
			{
				hr &= g_default_system.DrawIndexedPrimitive_StructureImpl(pDevice, Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
				hr &= DrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
			}

			return hr;
		}
		HRESULT	 	c_gbuffer_system::DrawIndexedPrimitive_Object(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE Type,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount)
		{
			HRESULT hr = pDevice->DrawIndexedPrimitive(Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);

			if(g_default_system.IsRenderable() && 
				!g_is_rendering_reflection && 
				(g_current_render_state == Enums::_render_progress_sky || g_current_render_state == Enums::_render_progress_objects))
			{
				hr &= g_default_system.DrawIndexedPrimitive_ObjectImpl(pDevice, Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
				hr &= DrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);				
			}

			return hr;
		}		
		
		HRESULT		c_gbuffer_system::SetVertexShaderConstantF_WVP(IDirect3DDevice9* device, UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount)
		{			
			if(g_is_rendering_reflection == false && g_system_enabled && !g_wvp_stored)	
			{
				device->SetVertexShaderConstantF(96, g_previous_worldviewproj, 4);
				memcpy_s(&g_previous_worldviewproj, sizeof(D3DMATRIX), pConstantData, sizeof(D3DMATRIX));
				g_wvp_stored = true;
			}

			return device->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
		}
		void		c_gbuffer_system::ClearGBuffer(IDirect3DDevice9* pDevice)
		{
			g_default_system.ClearGBufferImpl(pDevice);

			g_wvp_stored = false;
		}
		
		HRESULT		c_gbuffer_system::SetVertexShaderConstantF_All(IDirect3DDevice9* pDevice, UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount)
		{
			//eye position doesn't get updated in the object code so always pass it through so we have the most recent value
			if((StartRegister == 0) || (g_current_render_state == Enums::_render_progress_objects))
				return Rasterizer::ShaderExtension::SetVertexShaderConstantF(pDevice, StartRegister, pConstantData, Vector4fCount);
			else
				return pDevice->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
		}
		//////////////////////////////////////////////////////////////////////

		
		//////////////////////////////////////////////////////////////////////
		// c_gbuffer_system implementation
		void		c_gbuffer_system::OnLostDeviceImpl()
		{
			if(m_gbuffer_ps) m_gbuffer_ps->OnLostDevice();
			if(m_gbuffer_vs) m_gbuffer_vs->OnLostDevice();

			GBufferDebug().OnLostDevice();

			ReleaseResources();
		}

		void		c_gbuffer_system::OnResetDeviceImpl(D3DPRESENT_PARAMETERS* params)
		{
			AllocateResources(DX9::Direct3DDevice(), params);
			if(m_gbuffer_ps) m_gbuffer_ps->OnResetDevice();
			if(m_gbuffer_vs) m_gbuffer_vs->OnResetDevice();

			GBufferDebug().OnResetDevice();
		}

		void		c_gbuffer_system::AllocateResources(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* params)
		{
			if(FAILED(pDevice->TestCooperativeLevel()))
				return;

			HRESULT hr;
			D3DCAPS9 device_caps;
			hr = pDevice->GetDeviceCaps(&device_caps);

			if(FAILED(LoadEffect(pDevice, &m_gbuffer_ps,		"GBuffer_PS")))		return;
			if(FAILED(LoadEffect(pDevice, &m_gbuffer_vs,		"GBuffer_VS")))		return;

			if(FAILED(LoadEffect(pDevice, &GBufferDebug().GetEffect(),	"GBuffer_Debug")))	return;

			// Create the position texture
			m_gbuffer.m_rt_depth.CreateTarget(pDevice, params->BackBufferWidth, params->BackBufferHeight, D3DFMT_R32F);
			// Create the velocity texture
			m_gbuffer.m_rt_velocity.CreateTarget(pDevice, params->BackBufferWidth, params->BackBufferHeight, D3DFMT_G16R16F);
			// Create the normals texture
			m_gbuffer.m_rt_normals_index.CreateTarget(pDevice, params->BackBufferWidth, params->BackBufferHeight, D3DFMT_A8R8G8B8);

			if(FAILED(GBufferDebug().AllocateResources(pDevice, params->BackBufferWidth, params->BackBufferHeight)))
				return;

			m_structures.vs_technique_bsp = m_gbuffer_vs->GetTechniqueByName("BSP_VS");
			m_structures.vs_technique_object = m_gbuffer_vs->GetTechniqueByName("Object_VS");

			m_multi_rt.bsp_techniques[0] = m_gbuffer_ps->GetTechniqueByName("MRT2_BSP");
			m_multi_rt.bsp_techniques[1] = m_gbuffer_ps->GetTechniqueByName("MRT3_BSP");
			m_multi_rt.object_techniques[0] = m_gbuffer_ps->GetTechniqueByName("MRT2_Object");
			m_multi_rt.object_techniques[1] = m_gbuffer_ps->GetTechniqueByName("MRT3_Object");

			m_multi_rt.count = 2;//(device_caps.NumSimultaneousRTs > NUMBEROF(m_multi_rt.techniques) ? 
				//NUMBEROF(m_multi_rt.techniques) : device_caps.NumSimultaneousRTs);

			// cant alpha test and put data in the alpha channel with 1 render target
			if(m_multi_rt.count == 1)
				return;

			// TODO: Figure out a way to never set target 0 to NULL as it will end up rendering to the backbuffer
			switch (m_multi_rt.count)
			{
			case 2:
				m_multi_rt.output[0][0] = (m_gbuffer.m_rt_depth.IsEnabled()	?			m_gbuffer.m_rt_depth.surface : NULL);
				m_multi_rt.output[1][0] = (m_gbuffer.m_rt_velocity.IsEnabled() ?		m_gbuffer.m_rt_velocity.surface : NULL);
				m_multi_rt.output[1][1] = (m_gbuffer.m_rt_normals_index.IsEnabled() ?	m_gbuffer.m_rt_normals_index.surface : NULL);
			default:
				m_multi_rt.output[0][0] = (m_gbuffer.m_rt_depth.IsEnabled() ?			m_gbuffer.m_rt_depth.surface : NULL);
				m_multi_rt.output[0][1] = (m_gbuffer.m_rt_velocity.IsEnabled() ?		m_gbuffer.m_rt_velocity.surface : NULL);
				m_multi_rt.output[0][2] = (m_gbuffer.m_rt_normals_index.IsEnabled() ?	m_gbuffer.m_rt_normals_index.surface : NULL);
				break;
			}
			
			m_parameters.far_clip =					m_gbuffer_ps->GetParameterByName(NULL, "FarClipDistance");
			m_parameters.is_sky =					m_gbuffer_ps->GetParameterByName(NULL, "IsSky");
			m_parameters.sample_normal_texture =	m_gbuffer_ps->GetParameterByName(NULL, "SampleNormalTexture");
			m_parameters.do_velocity =				m_gbuffer_ps->GetParameterByName(NULL, "DoVelocity");
			m_parameters.velocity_multiplier =		m_gbuffer_ps->GetParameterByName(NULL, "VelocityMultiplier");

			m_parameters.mesh_type_index =			m_gbuffer_ps->GetParameterByName(NULL, "MeshTypeIndex");
			m_parameters.owner_team_index =			m_gbuffer_ps->GetParameterByName(NULL, "OwnerTeamIndex");

			g_current_render_state = Enums::_render_progress_none;
			m_is_loaded = true;			
		}

		void		c_gbuffer_system::ReleaseResources()
		{
			m_gbuffer.ReleaseTargets();

			Yelo::safe_release(m_gbuffer_ps);
			Yelo::safe_release(m_gbuffer_vs);

			GBufferDebug().ReleaseResources();

			m_is_loaded = false;
		}

		void		c_gbuffer_system::VertexShaderChangedImpl(IDirect3DVertexShader9* pShader)
		{
			// Sky can use many different vertex shaders so just say yes
			m_render_gbuffer = (g_current_render_state == Enums::_render_progress_sky);
			
			// Halo has a fixed vertex shader array so comparing pointers works ok
			int32 i = 0;
			while (!m_render_gbuffer && i < NUMBEROF(kValidShaders))
				m_render_gbuffer = GET_PTR(VSF_TABLE_START_REFERENCE)[kValidShaders[i++]].shader == pShader;
		}

		HRESULT	 	c_gbuffer_system::DrawIndexedPrimitive_Impl(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE Type,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount)
		{
			HRESULT hr;

			UINT cPasses, p;
			m_gbuffer_vs->Begin(NULL,0);
			m_gbuffer_vs->BeginPass(0);

			m_gbuffer_ps->Begin(&cPasses, 0);
			for( p = 0; p < cPasses; ++p )
			{ 
				for(uint32 i = 0; i < m_multi_rt.count; i++)
					pDevice->SetRenderTarget(i, m_multi_rt.output[p][i]);

				m_gbuffer_ps->BeginPass(p);
				hr = pDevice->DrawIndexedPrimitive(Type,BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
				m_gbuffer_ps->EndPass();

				for(uint32 i = 1; i < m_multi_rt.count; i++)
					pDevice->SetRenderTarget(i, NULL);
			}
			m_gbuffer_ps->End();

			m_gbuffer_vs->EndPass();	
			m_gbuffer_vs->End();


			return hr;
		}
		HRESULT	 	c_gbuffer_system::DrawIndexedPrimitive_StructureImpl(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE Type,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount)
		{
			if(!m_is_loaded || !m_render_gbuffer || !g_system_enabled)
				return S_OK;

			m_gbuffer_vs->SetTechnique(m_structures.vs_technique_bsp);
			m_gbuffer_ps->SetTechnique(m_multi_rt.bsp_techniques[m_multi_rt.count - 2]);
			real DT = GameState::MainGlobals()->delta_time;
			real BT = 1.0f / 30.0f;
			real Multiplier = BT / DT;
			m_gbuffer_ps->SetFloat(m_parameters.velocity_multiplier, Multiplier);
			m_gbuffer_ps->SetFloat(m_parameters.far_clip, Rasterizer::RenderGlobals()->frustum.z_far);
			m_gbuffer_ps->SetBool(m_parameters.is_sky, false);
			m_gbuffer_ps->SetBool(m_parameters.do_velocity, true);
			m_gbuffer_ps->SetBool(m_parameters.sample_normal_texture, true);
			m_gbuffer_ps->SetInt(m_parameters.mesh_type_index, 2);
			m_gbuffer_ps->SetInt(m_parameters.owner_team_index, 0);

			return S_OK;
		}
		HRESULT	 	c_gbuffer_system::DrawIndexedPrimitive_ObjectImpl(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE Type,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount)
		{
			int16 MeshIndex = 0;
			int16 TeamIndex = 0;

			m_gbuffer_vs->SetTechnique(m_structures.vs_technique_object);
			m_gbuffer_ps->SetTechnique(m_multi_rt.object_techniques[m_multi_rt.count - 2]);
			m_gbuffer_ps->SetFloat(m_parameters.far_clip, Rasterizer::RenderGlobals()->frustum.z_far);
			m_gbuffer_ps->SetBool(m_parameters.is_sky, g_current_render_state == Enums::_render_progress_sky);
			m_gbuffer_ps->SetBool(m_parameters.do_velocity, false);
			m_gbuffer_ps->SetBool(m_parameters.sample_normal_texture, false);

			if(g_current_render_state == Enums::_render_progress_sky)
				MeshIndex = 1;
			else if(g_object_index != 0xFFFF)
			{
				Objects::s_object_header_datum& object_header = (*Objects::ObjectHeader())[g_object_index];		
				
				TeamIndex = *object_header._object->GetOwnerTeamIndex() + 1;
				if(TeamIndex < 0)
					TeamIndex = 0;
				
				MeshIndex = object_header.object_type + 3;
				if(MeshIndex < 0)
					MeshIndex = 0;
				
				if(GameEngine::Current() != NULL)	// becomes non-null during multiplayer
					TeamIndex += 9;					// Offset TeamIndex by 9 for MP teams
			}

			m_gbuffer_ps->SetInt(m_parameters.mesh_type_index, MeshIndex);
			m_gbuffer_ps->SetInt(m_parameters.owner_team_index, TeamIndex);

			return S_OK;
		}

		void		c_gbuffer_system::ClearGBufferImpl(IDirect3DDevice9* pDevice)
		{
			if(!g_system_enabled)
				return;

			// Get the current render target
			LPDIRECT3DSURFACE9 origTarget;
			pDevice->GetRenderTarget(0, &origTarget);

			// Clear our outputs
			m_gbuffer.ClearTargets(pDevice);

			// Set render target back to original
			pDevice->SetRenderTarget(0, origTarget);
			Yelo::safe_release(origTarget);
		}

		void		c_gbuffer_system::RenderDebugQuad(IDirect3DDevice9* pDevice)
		{
			if(!g_system_enabled || !m_is_loaded)
			{
				g_debug_index = 0;
				return;
			}
			GBufferDebug().Render(pDevice, g_debug_index);
		}

		HRESULT		c_gbuffer_system::LoadEffect(IDirect3DDevice9* pDevice, LPD3DXEFFECT* pEffect, const char* EffectID)
		{
			HRESULT hr = S_OK;
			LPD3DXBUFFER error_buffer = NULL;
			uint32 data_size = 0;
			void* data_pointer = m_shader_package.GetDataPointer(EffectID, &data_size);

			if(!data_pointer || data_size == 0)
				return E_FAIL;

			hr = D3DXCreateEffect(
				pDevice, 
				data_pointer, 
				data_size, 
				NULL, 
				NULL, 
				NULL, 
				NULL, 
				pEffect, 
				&error_buffer
			);
			if (FAILED(hr))
			{
				// if loading the shader failed, and we are in devmode
				// print an error to the console
				if(GameState::DevmodeEnabled())
				{
					Postprocessing::Debug::Write("Error: failed to load gbuffer shaders");
					Postprocessing::Debug::WriteD3DXErrors(error_buffer, 1);
				}
			}
			Yelo::safe_release(error_buffer);	

			return hr;
		}
		c_gbuffer&	c_gbuffer_system::GBuffer() { return c_gbuffer_system::g_default_system.m_gbuffer; }
		/////////////////////////////////////////////////////////////////////
		// c_gbuffer_system		
		BOOL					c_gbuffer_system::g_is_rendering_reflection = true;
		uint16					c_gbuffer_system::g_object_index;
		c_gbuffer_system		c_gbuffer_system::g_default_system;
		int16					c_gbuffer_system::g_debug_index;
		bool					c_gbuffer_system::g_system_enabled;
		Enums::render_progress	c_gbuffer_system::g_current_render_state;	// What is halo currently rendering
		BOOL					c_gbuffer_system::g_wvp_stored;
		D3DXMATRIX				c_gbuffer_system::g_previous_worldviewproj;

		//////////////////////////////////////////////////////////////////////
		// c_gbuffer_system::render_progress
		void		c_gbuffer_system::render_progress::SkyPreProcess()
		{
			g_current_render_state = Enums::_render_progress_sky;
		}
		void		c_gbuffer_system::render_progress::SkyPostProcess()
		{
			g_current_render_state = Enums::_render_progress_none;
		}

		void		c_gbuffer_system::render_progress::ObjectsPreProcess()
		{
			g_current_render_state = Enums::_render_progress_objects;
		}
		void		c_gbuffer_system::render_progress::ObjectsPostProcess()
		{
			g_current_render_state = Enums::_render_progress_none;
		}

		void		c_gbuffer_system::render_progress::StructurePreProcess()
		{
			g_current_render_state = Enums::_render_progress_structure;
		}
		void		c_gbuffer_system::render_progress::StructurePostProcess()
		{
			g_current_render_state = Enums::_render_progress_none;
		}

		void		c_gbuffer_system::render_progress::UIPreProcess()
		{
			if(g_debug_index != 0)
				g_default_system.RenderDebugQuad(DX9::Direct3DDevice());
		}
		//////////////////////////////////////////////////////////////////////
	};
};
#endif