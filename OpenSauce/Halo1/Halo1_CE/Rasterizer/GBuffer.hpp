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
		Component: GBuffer
		Original Author: FireScythe
		Integration: kornman00

	This component adds a GBuffer to Halo's rendering pipeline, providing
	access to the screen space Depth, Normals and Velocity.
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include <Common/PackedFile.hpp>
#include "Rasterizer/Rasterizer.hpp"
#include "Rasterizer/DX9/DX9.hpp"
#include "Rasterizer/DX9/rasterizer_dx9_shaders_vshader9.hpp"

namespace Yelo
{
	namespace Enums
	{
		enum render_progress {
			_render_progress_sky,
			_render_progress_objects,
			_render_progress_structure,
			_render_progress_none,
		};
	};

	namespace DX9
	{
		enum {
			k_maximum_multi_render_target = 3,
		};
		//////////////////////////////////////////////////////////////////////
		// Container for holding all members of the GBuffer					//
		//////////////////////////////////////////////////////////////////////
		struct c_gbuffer
		{
			enum {
				k_gbuffer_depth_x = 0,		//R32_F		X

				k_gbuffer_velocity_x = 0,	//R16_F		X
				k_gbuffer_velocity_y = 1,	//R16_F			Y

				k_gbuffer_normals_x = 0,	//R8G8B8A8	X
				k_gbuffer_normals_y = 1,	//R8G8B8A8		Y

				k_gbuffer_index_x = 2,		//R8G8B8A8			Z
				k_gbuffer_index_y = 3,		//R8G8B8A8				W
			};
			Rasterizer::s_render_target					m_rt_depth;
			Rasterizer::s_render_target					m_rt_velocity;
			Rasterizer::s_render_target					m_rt_normals_index;

			void ReleaseTargets();
			void ClearTargets(IDirect3DDevice9* pDevice);

			bool SetEffectVar(LPD3DXEFFECT& effect,
				bool& variable_used,
				cstring texture_semantic,
				Rasterizer::s_render_target& target,
				cstring x_handle_semantic = NULL, const int x_index = 0,
				cstring y_handle_semantic = NULL, const int y_index = 0,
				cstring z_handle_semantic = NULL, const int z_index = 0,
				cstring w_handle_semantic = NULL, const int w_index = 0);
			bool SetDepth(LPD3DXEFFECT& effect, bool& variable_used);
			bool SetVelocity(LPD3DXEFFECT& effect, bool& variable_used);
			bool SetNormals(LPD3DXEFFECT& effect, bool& variable_used);
			bool SetIndex(LPD3DXEFFECT& effect, bool& variable_used);
		};

		struct s_render_target_output
		{
			enum { k_target_count = 4, };

			LPDIRECT3DSURFACE9 m_targets[k_target_count];

			LPDIRECT3DSURFACE9& s_render_target_output::operator [] (int32 index)
			{
				return m_targets[index];
			}
		};

		class c_gbuffer_fullscreen_effect
		{
		protected:
			LPD3DXEFFECT	m_effect;
			TEXTURE_VERTEX	m_vertices[4];

		public:
			virtual HRESULT	AllocateResources(IDirect3DDevice9* device, uint32 width, uint32 height);
			void			ReleaseResources();

			void			OnLostDevice();
			HRESULT			OnResetDevice();
			virtual void	Render(IDirect3DDevice9* device) {}

			virtual bool	IsAvailable();
			LPD3DXEFFECT&	GetEffect() { return m_effect; }	
		};

		class c_gbuffer_debug_effect : public c_gbuffer_fullscreen_effect
		{
			D3DXHANDLE		m_technique_single;
			D3DXHANDLE		m_technique_all;

			D3DXHANDLE		m_far_clip_handle;
			D3DXHANDLE		m_target_handle;

			bool			m_depth_set;
			bool			m_velocity_set;
			bool			m_normals_set;
			bool			m_index_set;
		public:
			HRESULT			AllocateResources(IDirect3DDevice9* device, uint32 width, uint32 height);
			void			ReleaseResources();

			void			Render(IDirect3DDevice9* device, int16 debug_target);

			bool			IsAvailable();	
		};
		static c_gbuffer_debug_effect& GBufferDebug();

		class c_gbuffer_rtclear_effect : public c_gbuffer_fullscreen_effect
		{
		public:
			struct {
				D3DXHANDLE clear_technique;
				s_render_target_output output[k_maximum_multi_render_target];
				uint32 count;
			}m_multi_rt;

			HRESULT			AllocateResources(IDirect3DDevice9* device, uint32 width, uint32 height);
			void			ReleaseResources();

			void			Render(IDirect3DDevice9* device);

			bool			IsAvailable();	
		};
		static c_gbuffer_rtclear_effect& GBufferClear();
		
		class c_gbuffer_system
		{
		public:
			static int16 g_debug_index; // which render target to display
			static bool g_system_enabled; // Configured from the user settings

		private:
			static Enums::rasterizer_vertex_shader const kValidShaders[];

			static Enums::render_progress	g_current_render_state;						// What is halo currently rendering
			
			static BOOL						g_output_object_tbn;
			static BOOL						g_output_object_velocity;
			static BOOL						g_output_velocity;
			static D3DXVECTOR4				g_pixel_shader_input;
			
			static BOOL						g_wvp_stored;
			static D3DXMATRIX				g_previous_worldviewproj;					// WVP for the previous frame (for BSP velocity)

			static BOOL						g_is_rendering_reflection;					// Is halo rendering the reflection geometry?
			static void Hook_RenderWindow();

			static uint16					g_object_index;
			static void Hook_RenderObjectList_GetObjectIndex();
			static void Hook_RenderObjectList_ClearObjectIndex();
			static void Hook_FirstPersonWeaponDraw_GetObjectIndex();
			
			static uint32					g_current_object_lod;						// the LOD of the current object being rendered
			static void Hook_RenderObject_GetCurrentLOD();

			static void Hook_CommandCameraSet();
			static void Hook_CommandSwitchBSP();
			static void Hook_CommandGameSave();
		private:
			c_packed_file			m_shader_package;
			bool					m_is_loaded;
			bool					m_render_gbuffer;								// The GBuffer is only rendered to when certain vertex shaders are used
			PAD16;
			LPD3DXEFFECT			m_gbuffer_ps, m_gbuffer_vs;
			c_gbuffer				m_gbuffer;

			struct {
				s_render_target_output output[k_maximum_multi_render_target];
				uint32 count;
			}m_multi_rt;

			struct {
				struct {
					D3DXHANDLE n;
					D3DXHANDLE n_v;
				}ps_bsp_techniques;
				struct {
					D3DXHANDLE n;
					D3DXHANDLE n_v;
				}vs_bsp_techniques;

				struct {
					D3DXHANDLE n;
					D3DXHANDLE tbn;
					D3DXHANDLE n_v;
					D3DXHANDLE tbn_v;
					D3DXHANDLE none;
				}ps_object_techniques;
				struct {
					D3DXHANDLE n;
					D3DXHANDLE tbn;
					D3DXHANDLE n_v;
					D3DXHANDLE tbn_v;
					D3DXHANDLE none;
				}vs_object_techniques;
			}m_structures;

			bool IsRenderable() const
			{
				return m_is_loaded && m_render_gbuffer && g_system_enabled;
			}
		public:
			void Ctor(cstring package_file);
			// Hooks the non-reflection render call and opens the shader file 
			static void	Initialize();
			// Nothing
			static void	Dispose();
			// Load any user settings for the gbuffer
			static void LoadSettings(TiXmlElement* dx9_element);
			// Save any user settings for the gbuffer
			static void SaveSettings(TiXmlElement* dx9_element);
					
			// Calls AllocateResources
			static void	Initialize3D(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params);
			// Calls on lost device for the gbuffer shaders then releases resources
			static void	OnLostDevice();
			// Allocates resources, then calls on reset device for the GBuffer shaders
			static void	OnResetDevice(D3DPRESENT_PARAMETERS* params);
			// Nothing
			static void	Render();
			// Release direct3D resources when quitting
			static void Release();
			// Exposes g_output_object_tbn
			static BOOL& OutputObjectTBN();


		public:
			// The vertex shader is used to determine if a mesh should be rendered to the GBuffer
			static void			VertexShaderChanged(IDirect3DVertexShader9* pShader);
			// Called for each mesh drawn to render to the GBuffer
			static HRESULT	 	DrawIndexedPrimitive(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE Type,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount);
			static HRESULT	 	DrawIndexedPrimitive_Structure(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE Type,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount);
			static HRESULT	 	DrawIndexedPrimitive_Object(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE Type,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount);			
			static HRESULT		SetVertexShaderConstantF_WVP(IDirect3DDevice9* device, UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount);
			// Clears all GBuffer surfaces, ready for the next frame
			static void			ClearGBuffer(IDirect3DDevice9* pDevice);
			static HRESULT		SetVertexShaderConstantF_All(IDirect3DDevice9* device, UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount);

		private:
			static c_gbuffer_system			g_default_system;

			void		OnLostDeviceImpl();
			void		OnResetDeviceImpl(D3DPRESENT_PARAMETERS* params);
			// Creates GBuffer textures and shaders
			void		AllocateResources(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* params);
			// Releases GBuffer textures and shaders
			void		ReleaseResources();
			// The vertex shader is used to determine if a mesh should be rendered to the GBuffer
			void		VertexShaderChangedImpl(IDirect3DVertexShader9* pShader);
			// Called for each mesh drawn to render to the GBuffer
			HRESULT	 	DrawIndexedPrimitive_Impl(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE Type,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount);
			HRESULT	 	DrawIndexedPrimitive_StructureImpl(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE Type,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount);
			HRESULT	 	DrawIndexedPrimitive_ObjectImpl(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE Type,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount);
			// Clears all GBuffer surfaces, ready for the next frame
			void		ClearGBufferImpl(IDirect3DDevice9* pDevice);
			// Renders a full screen quad with one or all GBuffer textures
			void		RenderDebugQuad(IDirect3DDevice9* pDevice);
			// Loads an effect from a packed file
			HRESULT		LoadEffect(IDirect3DDevice9* pDevice, LPD3DXEFFECT* pEffect, const char* EffectID);

		public:
			static c_gbuffer&				GBuffer();

			struct render_progress
			{
				static void SkyPreProcess();
				static void SkyPostProcess();

				static void ObjectsPreProcess();
				static void ObjectsPostProcess();

				static void StructurePreProcess();
				static void StructurePostProcess();

				static void UIPreProcess();
			};
		};
	};
};
#endif