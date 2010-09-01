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
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include "Rasterizer/DX9/DX9.hpp"
#include "Rasterizer/PostProcessing/PostProcessing.hpp"

namespace Yelo
{
	namespace Postprocessing
	{
		/////////////////////////////////////////////////////////////////////
		// Base post process shader. Must be derived to be used.
		/////////////////////////////////////////////////////////////////////
		class c_postprocess_shader
			abstract
		{
		public:
			TagGroups::s_shader_postprocess_definition* m_shader_base;
			char						m_shader_id[MAX_PATH];

		protected:
			LPD3DXEFFECT*				m_effect;		// D3DX Effect object for this shader
						
		public:
			LPD3DXEFFECT*		GetEffect() const	{ return m_effect; }

			virtual void		Ctor()
			{
				m_effect = NULL;
				m_shader_base = NULL;
			}

			virtual void		Dtor()
			{
				ReleaseResources();
			}

			bool				IsValidShader() const { return m_shader_base->runtime.flags.valid_shader_bit; }
			//IGNORE		NULLs all handles and sets shd_ValidShader to false.
			void				Initialize(IDirect3DDevice9* Device);
			virtual void		SetSource(void* pSource);
			virtual HRESULT		SetupShader();
			//ESSENTIAL		Loads the LPD3DXEFFECT shader. MUST be overriden by derived structs.
			virtual HRESULT		LoadShader(IDirect3DDevice9* pDevice){ return E_FAIL; }
			//OPTIONAL		Used for loading non standard resources such as bitmaps.
			virtual HRESULT		LoadCustomResources(IDirect3DDevice9* pDevice){ return S_OK; }
			//IGNORE		Loads the standard D3DXHANDLEs from the shader.
			HRESULT				LoadStandardResources(IDirect3DDevice9* pDevice);
			//IGNORE		Calls the above three Load* functions and sets shd_ValidShader to true if all is well.
			HRESULT				AllocateResources(IDirect3DDevice9* pDevice);
			//ESSENTIAL		Unloads the shader. MUST be overriden by derived structs.
			virtual void		UnloadShader()			{}
			//OPTIONAL		Optional unless resources have been loaded in LoadCustomResources. They should be released here.
			virtual void		UnloadCustomResources()	{}
			//IGNORE		NULLs all standard handles.
			void				UnloadStandardResources();
			//IGNORE		Calls the above three Unload* functions and sets shd_ValidShader to false.
			void				ReleaseResources();
			//IGNORE		If the shader is not null, tell it that the device has been lost.
			void				OnLostDevice();
			//IGNORE		If the shader is not null, tell it that the device has been reset.
			void				OnResetDevice(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters);
			//OPTIONAL		Things than need to be done before the shader is started go here.
			virtual HRESULT		DoPreRender(IDirect3DDevice9* pDevice, double frame_time)			{ return S_OK; }
			//OPTIONAL		Things than need to be done per pass go here.
			virtual HRESULT		DoPerPass(IDirect3DDevice9* pDevice, double frame_time, UINT pass)	{ return S_OK; }
			//OPTIONAL		Things than need to be done after the shader is complete go here.
			virtual HRESULT		DoPostRender(IDirect3DDevice9* pDevice, double frame_time)			{ return S_OK; }
			//IGNORE		Call this if your shader will be changing the size of the fullscreen quad.
			HRESULT				DoPostProcess(IDirect3DDevice9* pDevice,
											double frame_time,
											TagGroups::s_shader_postprocess_effect_render_quad* quad);
		};
	};
};
#endif