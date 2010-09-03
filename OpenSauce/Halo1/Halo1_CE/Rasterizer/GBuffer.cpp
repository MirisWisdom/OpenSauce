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

		//////////////////////////////////////////////////////////////////////////
		// s_render_target
		HRESULT		s_render_target::CreateTarget(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters, D3DFORMAT Format)
		{
			m_enabled = true;

			HRESULT hr = pDevice->CreateTexture(						
					pParameters->BackBufferWidth,
					pParameters->BackBufferHeight,
					1,
					D3DUSAGE_RENDERTARGET,
					Format,
					D3DPOOL_DEFAULT,
					&m_texture,
					NULL);

			if(SUCCEEDED(hr))
				hr = m_texture->GetSurfaceLevel(0, &m_surface);

			if(FAILED(hr))
			{
				this->ReleaseTarget();
				m_enabled = false;
			}
			m_format = Format;
			return hr;
		}

		void		s_render_target::ReleaseTarget()
		{
			Yelo::safe_release(m_surface);
			Yelo::safe_release(m_texture);
		}

		void		s_render_target::ClearTarget(IDirect3DDevice9* pDevice)
		{
			if(!m_enabled)	return;
			
			pDevice->SetRenderTarget(0, m_surface);
			pDevice->Clear( 0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
										0x00000000, 1.0f, 0L );
		}
		//////////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////////
		// s_gbuffer
		void s_gbuffer::ReleaseTargets()
		{
			m_rt_depth.ReleaseTarget();
			m_rt_velocity.ReleaseTarget();
			m_rt_normals.ReleaseTarget();
			m_rt_index.ReleaseTarget();
		}

		void s_gbuffer::ClearTargets(IDirect3DDevice9* pDevice)
		{
			m_rt_depth.ClearTarget(pDevice);
			m_rt_velocity.ClearTarget(pDevice);
			m_rt_normals.ClearTarget(pDevice);
			m_rt_index.ClearTarget(pDevice);
		}
		//////////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////
		// c_gbuffer_system interface
		Enums::rasterizer_vertex_shader const c_gbuffer_system::kValidShaders[] = {
			Enums::_rasterizer_vs_environment_lightmap,
			Enums::_rasterizer_vs_model_fogged,
			Enums::_rasterizer_vs_model,
			Enums::_rasterizer_vs_model_fast,
			Enums::_rasterizer_vs_model_scenery,
		};

		BOOL c_gbuffer_system::g_is_rendering_reflection = true;
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

		uint16 c_gbuffer_system::g_object_index;
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
		
		void		c_gbuffer_system::ClearGBuffer(IDirect3DDevice9* pDevice)
		{
			g_default_system.ClearGBufferImpl(pDevice);
		}
		//////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////
		// c_gbuffer_system implementation
		void		c_gbuffer_system::OnLostDeviceImpl()
		{
			if(gbuffer_ps) gbuffer_ps->OnLostDevice();
			if(gbuffer_vs) gbuffer_vs->OnLostDevice();
			if(gbuffer_debug) gbuffer_debug->OnLostDevice();
			ReleaseResources();
		}

		void		c_gbuffer_system::OnResetDeviceImpl(D3DPRESENT_PARAMETERS* params)
		{
			AllocateResources(DX9::Direct3DDevice(), params);
			if(gbuffer_ps) gbuffer_ps->OnResetDevice();
			if(gbuffer_vs) gbuffer_vs->OnResetDevice();
			if(gbuffer_debug) gbuffer_debug->OnResetDevice();
		}

		void		c_gbuffer_system::AllocateResources(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* params)
		{
			if(FAILED(pDevice->TestCooperativeLevel()))
				return;

			HRESULT hr;
			D3DCAPS9 device_caps;
			hr = pDevice->GetDeviceCaps(&device_caps);

			if(FAILED(LoadEffect(pDevice, &gbuffer_ps,		"GBuffer_PS")))		return;
			if(FAILED(LoadEffect(pDevice, &gbuffer_vs,		"GBuffer_VS")))		return;
			if(FAILED(LoadEffect(pDevice, &gbuffer_debug,	"GBuffer_Debug")))	return;			

			// Create the position texture
			gbuffer.m_rt_depth.CreateTarget(pDevice, params, D3DFMT_R32F);

			// Create the velocity texture
			gbuffer.m_rt_velocity.CreateTarget(pDevice, params, D3DFMT_G16R16F);
			// Create the normals texture
			gbuffer.m_rt_normals.CreateTarget(pDevice, params, D3DFMT_G16R16F);
			// Create the index texture
			gbuffer.m_rt_index.CreateTarget(pDevice, params, D3DFMT_A8R8G8B8);

			m_structures.vs_technique_bsp = gbuffer_vs->GetTechniqueByName("BSP_VS");
			m_structures.vs_technique_object = gbuffer_vs->GetTechniqueByName("Object_VS");

			m_multi_rt.techniques[0] = gbuffer_ps->GetTechniqueByName("MRT1");
			m_multi_rt.techniques[1] = gbuffer_ps->GetTechniqueByName("MRT2");
			m_multi_rt.techniques[2] = gbuffer_ps->GetTechniqueByName("MRT2");
			m_multi_rt.techniques[3] = gbuffer_ps->GetTechniqueByName("MRT4");

			m_multi_rt.count = (device_caps.NumSimultaneousRTs > NUMBEROF(m_multi_rt.techniques) ? 
				NUMBEROF(m_multi_rt.techniques) : device_caps.NumSimultaneousRTs);

			gbuffer_ps->SetTechnique(m_multi_rt.techniques[ m_multi_rt.count - 1 ]);

			// TODO: Figure out a way to never set target 0 to NULL as it will end up rendering to the backbuffer
			switch (m_multi_rt.count)
			{
			case 1:
				m_multi_rt.output[0][0] = (gbuffer.m_rt_depth.m_enabled ?	gbuffer.m_rt_depth.m_surface : NULL);
				m_multi_rt.output[1][0] = (gbuffer.m_rt_velocity.m_enabled ?gbuffer.m_rt_velocity.m_surface : NULL);
				m_multi_rt.output[2][0] = (gbuffer.m_rt_normals.m_enabled ?	gbuffer.m_rt_normals.m_surface : NULL);
				m_multi_rt.output[3][0] = (gbuffer.m_rt_index.m_enabled ?	gbuffer.m_rt_index.m_surface : NULL);
				break;
			case 2:
			case 3:
				m_multi_rt.output[0][0] = (gbuffer.m_rt_depth.m_enabled	?	gbuffer.m_rt_depth.m_surface : NULL);
				m_multi_rt.output[0][1] = (gbuffer.m_rt_velocity.m_enabled ?gbuffer.m_rt_velocity.m_surface : NULL);
				m_multi_rt.output[1][0] = (gbuffer.m_rt_normals.m_enabled ?	gbuffer.m_rt_normals.m_surface : NULL);
				m_multi_rt.output[1][1] = (gbuffer.m_rt_index.m_enabled ?	gbuffer.m_rt_index.m_surface : NULL);
				break;
			default:
				m_multi_rt.output[0][0] = (gbuffer.m_rt_depth.m_enabled ?	gbuffer.m_rt_depth.m_surface : NULL);
				m_multi_rt.output[0][1] = (gbuffer.m_rt_velocity.m_enabled ?gbuffer.m_rt_velocity.m_surface : NULL);
				m_multi_rt.output[0][2] = (gbuffer.m_rt_normals.m_enabled ?	gbuffer.m_rt_normals.m_surface : NULL);
				m_multi_rt.output[0][3] = (gbuffer.m_rt_index.m_enabled ?	gbuffer.m_rt_index.m_surface : NULL);
				break;
			}
			m_debug.rt_technique_single =	gbuffer_debug->GetTechniqueByName("DebugRTSingle");
			m_debug.rt_technique_all =		gbuffer_debug->GetTechniqueByName("DebugRTAll");

			D3DXHANDLE	DepthTex =							gbuffer_debug->GetParameterByName(NULL, "DepthTex");
			D3DXHANDLE	VelocityTex =						gbuffer_debug->GetParameterByName(NULL, "VelocityTex");
			D3DXHANDLE	NormalsTex =						gbuffer_debug->GetParameterByName(NULL, "NormalsTex");
			D3DXHANDLE	IndexTex =							gbuffer_debug->GetParameterByName(NULL, "IndexTex");
			if(gbuffer.m_rt_depth.m_enabled&&DepthTex)		gbuffer_debug->SetTexture(DepthTex,	gbuffer.m_rt_depth.m_texture);
			if(gbuffer.m_rt_velocity.m_enabled&&VelocityTex)gbuffer_debug->SetTexture(VelocityTex, gbuffer.m_rt_velocity.m_texture);
			if(gbuffer.m_rt_normals.m_enabled&&NormalsTex)	gbuffer_debug->SetTexture(NormalsTex, gbuffer.m_rt_normals.m_texture);
			if(gbuffer.m_rt_index.m_enabled&&IndexTex)		gbuffer_debug->SetTexture(IndexTex,	gbuffer.m_rt_index.m_texture);

			m_parameters.far_clip =					gbuffer_ps->GetParameterByName(NULL, "FarClipDistance");
			m_parameters.is_sky =					gbuffer_ps->GetParameterByName(NULL, "IsSky");
			m_parameters.sample_normal_texture =	gbuffer_ps->GetParameterByName(NULL, "SampleNormalTexture");
			m_parameters.do_velocity =				gbuffer_ps->GetParameterByName(NULL, "DoVelocity");
			m_parameters.velocity_multiplier =		gbuffer_ps->GetParameterByName(NULL, "VelocityMultiplier");

			m_parameters.mesh_type_index =	gbuffer_ps->GetParameterByName(NULL, "MeshTypeIndex");
			m_parameters.owner_team_index = gbuffer_ps->GetParameterByName(NULL, "OwnerTeamIndex");

			TEXTURE_VERTEX Quad[4] = 
			{
				{ -0.5f,								-0.5f,								1.0f, 1.0f, 0.0f, 0.0f },
				{ params->BackBufferWidth - 0.5f,		-0.5,								1.0f, 1.0f, 1.0f, 0.0f },
				{ -0.5,									params->BackBufferHeight - 0.5f,	1.0f, 1.0f, 0.0f, 1.0f },
				{ params->BackBufferWidth - 0.5f,		params->BackBufferHeight - 0.5f,	1.0f, 1.0f, 1.0f, 1.0f }
			};
			memcpy_s(m_debug.quad_vertices, sizeof(m_debug.quad_vertices), Quad, sizeof(Quad));

			g_current_render_state = Enums::_render_progress_none;
			m_is_loaded = true;			
		}

		void		c_gbuffer_system::ReleaseResources()
		{
			gbuffer.ReleaseTargets();

			Yelo::safe_release(gbuffer_ps);
			Yelo::safe_release(gbuffer_vs);
			Yelo::safe_release(gbuffer_debug);

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
			gbuffer_vs->Begin(NULL,0);
			gbuffer_vs->BeginPass(0);

			gbuffer_ps->Begin(&cPasses, 0);
			for( p = 0; p < cPasses; ++p )
			{ 
				for(uint32 i = 0; i < m_multi_rt.count; i++)
					pDevice->SetRenderTarget(i, m_multi_rt.output[p][i]);

				gbuffer_ps->BeginPass(p);
				hr = pDevice->DrawIndexedPrimitive(Type,BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
				gbuffer_ps->EndPass();
			}
			gbuffer_ps->End();

			gbuffer_vs->EndPass();	
			gbuffer_vs->End();

			for(uint32 i = 1; i < m_multi_rt.count; i++)
				pDevice->SetRenderTarget(i, NULL);

			return hr;
		}
		HRESULT	 	c_gbuffer_system::DrawIndexedPrimitive_StructureImpl(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE Type,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount)
		{
			if(!m_is_loaded || !m_render_gbuffer || !g_system_enabled)
				return S_OK;

			gbuffer_vs->SetTechnique(m_structures.vs_technique_bsp);
			real DT = GameState::MainGlobals()->delta_time;
			real BT = 1.0f / 30.0f;
			real Multiplier = BT / DT;
			gbuffer_ps->SetFloat(m_parameters.velocity_multiplier, Multiplier);
			gbuffer_ps->SetFloat(m_parameters.far_clip, Rasterizer::RenderGlobals()->frustum.z_far);
			gbuffer_ps->SetBool(m_parameters.is_sky, false);
			gbuffer_ps->SetBool(m_parameters.do_velocity, true);
			gbuffer_ps->SetBool(m_parameters.sample_normal_texture, true);
			gbuffer_ps->SetInt(m_parameters.mesh_type_index, 2);
			gbuffer_ps->SetInt(m_parameters.owner_team_index, 0);

			return S_OK;
		}
		HRESULT	 	c_gbuffer_system::DrawIndexedPrimitive_ObjectImpl(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE Type,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount)
		{
			int16 MeshIndex = 0;
			int16 TeamIndex = 0;

			gbuffer_vs->SetTechnique(m_structures.vs_technique_object);
			gbuffer_ps->SetFloat(m_parameters.far_clip, Rasterizer::RenderGlobals()->frustum.z_far);
			gbuffer_ps->SetBool(m_parameters.is_sky, g_current_render_state == Enums::_render_progress_sky);
			gbuffer_ps->SetBool(m_parameters.do_velocity, false);
			gbuffer_ps->SetBool(m_parameters.sample_normal_texture, false);

			if(g_current_render_state == Enums::_render_progress_sky)
				MeshIndex = 1;

			if(g_object_index != 0xFFFF)
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

			gbuffer_ps->SetInt(m_parameters.mesh_type_index, MeshIndex);
			gbuffer_ps->SetInt(m_parameters.owner_team_index, TeamIndex);

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
			gbuffer.ClearTargets(pDevice);

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

			D3DXHANDLE	targetToggle = gbuffer_debug->GetParameterByName(NULL, "RenderTarget");
			gbuffer_debug->SetInt(targetToggle, g_debug_index);

			DWORD old_depthbias, old_fillmode, old_srcblend, old_dest_blend, old_zenable, old_zwriteenable, old_stencilenable;
			pDevice->GetRenderState(D3DRS_DEPTHBIAS, &old_depthbias);
			pDevice->GetRenderState(D3DRS_FILLMODE, &old_fillmode);
			pDevice->GetRenderState(D3DRS_SRCBLEND, &old_srcblend);
			pDevice->GetRenderState(D3DRS_DESTBLEND, &old_dest_blend);	
			pDevice->GetRenderState(D3DRS_ZENABLE, &old_zenable);	
			pDevice->GetRenderState(D3DRS_ZWRITEENABLE, &old_zwriteenable);	
			pDevice->GetRenderState(D3DRS_STENCILENABLE, &old_stencilenable);

			pDevice->SetRenderState(D3DRS_DEPTHBIAS, 0);
			pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
			pDevice->SetRenderState(D3DRS_ZENABLE, false);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
			pDevice->SetRenderState(D3DRS_STENCILENABLE, false);

			UINT cPasses, p;
			pDevice->SetFVF( D3DFVF_XYZRHW | D3DFVF_TEX1 );

			Rasterizer::s_render_target& rt = Rasterizer::GlobalRenderTargets()[Enums::_rasterizer_target_render_primary];
			pDevice->SetRenderTarget(0, rt.surface);
			pDevice->Clear( 0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
				0xFF000000, 1.0f, 0L );
			gbuffer_debug->SetTechnique( g_debug_index == NONE ? 
				m_debug.rt_technique_all : m_debug.rt_technique_single);

			gbuffer_debug->Begin( &cPasses, 0 );
			for( p = 0; p < cPasses; ++p )
			{	
				gbuffer_debug->BeginPass( p );
				pDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, m_debug.quad_vertices, sizeof( TEXTURE_VERTEX ) );
				gbuffer_debug->EndPass();
			}
			gbuffer_debug->End();

			pDevice->SetRenderState(D3DRS_DEPTHBIAS, old_depthbias);
			pDevice->SetRenderState(D3DRS_FILLMODE, old_fillmode);
			pDevice->SetRenderState(D3DRS_SRCBLEND, old_srcblend);
			pDevice->SetRenderState(D3DRS_DESTBLEND, old_dest_blend);
			pDevice->SetRenderState(D3DRS_ZENABLE, old_zenable);	
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, old_zwriteenable);	
			pDevice->SetRenderState(D3DRS_STENCILENABLE, old_stencilenable);
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
		s_gbuffer*	c_gbuffer_system::GBuffer() { return &c_gbuffer_system::g_default_system.gbuffer; }
		/////////////////////////////////////////////////////////////////////
		// c_gbuffer_system
		c_gbuffer_system		c_gbuffer_system::g_default_system;
		int16					c_gbuffer_system::g_debug_index;
		bool					c_gbuffer_system::g_system_enabled;
		Enums::render_progress	c_gbuffer_system::g_current_render_state;	// What is halo currently rendering
		D3DXMATRIX				c_gbuffer_system::g_previous_worldviewproj;

		//////////////////////////////////////////////////////////////////////
		// c_gbuffer_system::render_progress
		void c_gbuffer_system::render_progress::SkyPreProcess()
		{
			g_current_render_state = Enums::_render_progress_sky;
		}
		void c_gbuffer_system::render_progress::SkyPostProcess()
		{
			g_current_render_state = Enums::_render_progress_none;
		}

		void c_gbuffer_system::render_progress::ObjectsPreProcess()
		{
			g_current_render_state = Enums::_render_progress_objects;
		}
		void c_gbuffer_system::render_progress::ObjectsPostProcess()
		{
			g_current_render_state = Enums::_render_progress_none;
		}

		void c_gbuffer_system::render_progress::StructurePreProcess()
		{
			g_current_render_state = Enums::_render_progress_structure;

			if(g_is_rendering_reflection == false)			
				DX9::Direct3DDevice()->SetVertexShaderConstantF(96, g_previous_worldviewproj, 4);
		}
		void c_gbuffer_system::render_progress::StructurePostProcess()
		{
			g_current_render_state = Enums::_render_progress_none;

			if(g_is_rendering_reflection == false)
				DX9::Direct3DDevice()->GetVertexShaderConstantF(0, g_previous_worldviewproj, 4);
		}

		void c_gbuffer_system::render_progress::UIPreProcess()
		{
			g_current_render_state = Enums::_render_progress_none; 
			// TODO: shouldn't this be set to ui? - this was originally set to make sure UI meshes are not rendered to the gbuffer. 
			//probably not needed anymore since vertex shaders define what is rendered.

			if(g_debug_index != 0)
				g_default_system.RenderDebugQuad(DX9::Direct3DDevice());
		}
		void c_gbuffer_system::render_progress::UIPostProcess()
		{
			g_current_render_state = Enums::_render_progress_none;
		}
		//////////////////////////////////////////////////////////////////////
	};
};
#endif