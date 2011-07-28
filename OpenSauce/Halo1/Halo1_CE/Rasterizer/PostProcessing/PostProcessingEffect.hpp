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
#include "Rasterizer/PostProcessing/PostProcessingShader.hpp"

namespace Yelo
{
	namespace Postprocessing
	{
		/////////////////////////////////////////////////////////////////////
		// An interface for a single instance of a shader
		/////////////////////////////////////////////////////////////////////
		class c_shader_instance_node
		{
		public:
			BOOL						m_is_active;
			c_postprocess_shader*		m_shader;
			c_shader_instance_node*		m_next;

			virtual void		Ctor()
			{
				m_is_active = true;
				m_shader = NULL;
				m_next = NULL;
			}

			virtual ~c_shader_instance_node()
			{
				m_is_active = true;
				delete m_next;
				m_next = NULL;
			}

			virtual void		UpdateInstance(real delta_time) {}
			virtual void		SetInstance() {}
			virtual void		SetShaderSource(c_postprocess_shader* shader);
		};

		/////////////////////////////////////////////////////////////////////
		// Base post process effect. Must be derived to be used.
		// The effect allows you to chain together shaders for more complex
		// effects.
		/////////////////////////////////////////////////////////////////////
		class c_postprocess_effect
			abstract
		{
		protected:
			TagGroups::s_shader_postprocess_effect_definition* m_base_effect;

			c_shader_instance_node*			m_shader_list_head;
			c_quad_instance*				m_render_quad;

		public:
			c_shader_instance_node* ShaderList() { return m_shader_list_head; }

			HRESULT				AddProcess(c_shader_instance_node* shader_instance);
			void				ResetEffect();
			virtual bool		ValidateEffect();
			virtual void		SetSource(void* pSource);
			virtual HRESULT		SetupEffect() { return S_OK; }
			virtual HRESULT		DoPreShader(IDirect3DDevice9* pDevice, double frame_time, int32 shader_index)	{ return S_OK; }
			virtual HRESULT		DoPostShader(IDirect3DDevice9* pDevice, double frame_time, int32 shader_index)	{ return S_OK; }
			virtual HRESULT		DoPreEffect(IDirect3DDevice9* pDevice,  double frame_time)	{ return S_OK; }
			virtual HRESULT		DoPostEffect(IDirect3DDevice9* pDevice,  double frame_time)	{ return S_OK; }
			HRESULT				DoPostProcessEffect(IDirect3DDevice9* pDevice,  double frame_time);
			virtual void		ReleaseResources();	
			virtual void		AllocateResources(IDirect3DDevice9* pDevice);
			virtual void		Update(real delta_time);

			virtual	void		Ctor()
			{
				m_base_effect = NULL;
				m_shader_list_head = NULL;
				m_render_quad = NULL;
			}

			virtual	void		Dtor()
			{
				delete m_shader_list_head;
				m_shader_list_head = NULL;
			}
		};
	};
};
#endif