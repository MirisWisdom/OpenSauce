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
		//////////////////////////////////////////////////////////////////////
		// Container for a texture and surface pair							//
		//////////////////////////////////////////////////////////////////////
		struct s_render_target
		{
			bool							m_enabled; PAD24;
			LPDIRECT3DTEXTURE9				m_texture;
			LPDIRECT3DSURFACE9				m_surface;
			D3DFORMAT						m_format;

			s_render_target() : m_enabled(true), m_format(D3DFMT_UNKNOWN) {}

			// Create the render target texture and surface. If either of those fails [enabled] will be false
			HRESULT		CreateTarget(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters, D3DFORMAT Format);
			// Release the texture and surface
			void		ReleaseTarget();
			// Set the device target to [surface] then clear it
			void		ClearTarget(IDirect3DDevice9* pDevice);
		};

		//////////////////////////////////////////////////////////////////////
		// Container for holding all members of the GBuffer					//
		//////////////////////////////////////////////////////////////////////
		struct s_gbuffer
		{
			s_render_target					m_rt_depth;
			s_render_target					m_rt_velocity;
			s_render_target					m_rt_normals;
			s_render_target					m_rt_index;

			void ReleaseTargets();
			void ClearTargets(IDirect3DDevice9* pDevice);
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

		class c_gbuffer_system
		{
		public:
			static int16 g_debug_index; // which render target to display
			static bool g_system_enabled; // Configured from the user settings

		private:
			enum {
				k_maximum_multi_render_target = 4,
			};

			static Enums::rasterizer_vertex_shader const kValidShaders[];

			static Enums::render_progress	g_current_render_state;						// What is halo currently rendering
			static D3DXMATRIX				g_previous_worldviewproj;					// WVP for the previous frame (for BSP velocity)

			static BOOL						g_is_rendering_reflection;					// Is halo rendering the reflection geometry?
			static void Hook_RenderWindow();

			static uint16					g_object_index;
			static void Hook_RenderObjectList_GetObjectIndex();
			static void Hook_RenderObjectList_ClearObjectIndex();
			static void Hook_FirstPersonWeaponDraw_GetObjectIndex();
		private:
			c_packed_file		m_shader_package;
			bool				m_is_loaded;
			bool				m_render_gbuffer;								// The GBuffer is only rendered to when certain vertex shaders are used
			PAD16;
			LPD3DXEFFECT		gbuffer_ps, gbuffer_vs, gbuffer_debug;
			s_gbuffer			gbuffer;

			struct {
				D3DXHANDLE techniques[k_maximum_multi_render_target];
				s_render_target_output output[k_maximum_multi_render_target];
				uint32 count;
			}m_multi_rt;

			struct {
				D3DXHANDLE rt_technique_single;
				D3DXHANDLE rt_technique_all;
				TEXTURE_VERTEX quad_vertices[4];
			}m_debug;

			struct {
				D3DXHANDLE vs_technique_bsp;
				D3DXHANDLE vs_technique_object;
			}m_structures;

			struct {
				D3DXHANDLE far_clip,
					is_sky,
					sample_normal_texture,
					do_velocity,
					velocity_multiplier,
					
					mesh_type_index,
					owner_team_index;
			}m_parameters;

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
		
		public:
			// The vertex shader is used to determine if a mesh should be rendered to the GBuffer
			static void			VertexShaderChanged(IDirect3DVertexShader9* pShader);
			// Called for each mesh drawn to render to the GBuffer
			static HRESULT	 	DrawIndexedPrimitive(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE Type,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount);
			static HRESULT	 	DrawIndexedPrimitive_Structure(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE Type,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount);
			static HRESULT	 	DrawIndexedPrimitive_Object(IDirect3DDevice9* pDevice, D3DPRIMITIVETYPE Type,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount);			
			// Clears all GBuffer surfaces, ready for the next frame
			static void			ClearGBuffer(IDirect3DDevice9* pDevice);

		private:
			static c_gbuffer_system g_default_system;

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
			static s_gbuffer*	GBuffer();

			struct render_progress
			{
				static void SkyPreProcess();
				static void SkyPostProcess();

				static void ObjectsPreProcess();
				static void ObjectsPostProcess();

				static void StructurePreProcess();
				static void StructurePostProcess();

				static void UIPreProcess();
				static void UIPostProcess();
			};
		};
	};
};
#endif