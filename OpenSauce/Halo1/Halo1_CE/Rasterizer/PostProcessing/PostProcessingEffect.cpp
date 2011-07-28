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
#include "Rasterizer/PostProcessing/PostProcessingEffect.hpp"
#if !PLATFORM_IS_DEDI
#include "Rasterizer/Rasterizer.hpp"
#include "Rasterizer/PostProcessing/PostProcessingQuadManager.hpp"

namespace PP = Yelo::Postprocessing;

namespace Yelo
{
	namespace Postprocessing
	{
		/////////////////////////////////////////////////////////////////////
		// c_shader_instance_node
		void		c_shader_instance_node::SetShaderSource(c_postprocess_shader* shader) 
		{
			m_shader = shader;
		}
		/////////////////////////////////////////////////////////////////////


		/////////////////////////////////////////////////////////////////////
		// c_postprocess_effect
		HRESULT		c_postprocess_effect::AddProcess(c_shader_instance_node* shader_instance)
		{
			// if there is no list the new entry will be the list head
			if(m_shader_list_head == NULL)
				m_shader_list_head = shader_instance;
			else
			{
				// add the new shader instance to the end of the list
				c_shader_instance_node* curr_node = m_shader_list_head;
				while (curr_node->m_next)
					curr_node = curr_node->m_next;
				curr_node->m_next = shader_instance;
			}
			return S_OK;
		}
		void		c_postprocess_effect::ResetEffect()
		{
			m_base_effect = NULL;

			// release the quad instance to reduce its reference count
			safe_release<c_quad_instance>(m_render_quad);

			// deleting the head node will recursively delete all child nodes
			delete m_shader_list_head;
			m_shader_list_head = NULL;
		}
		bool		c_postprocess_effect::ValidateEffect()
		{
			m_base_effect->runtime.flags.valid_effect_bit = false;

			// if there is no quad instance or no shaders then this effect is invalid
			if(!m_render_quad || m_shader_list_head == NULL)
				return m_base_effect->runtime.flags.valid_effect_bit;

			// if any of the shaders in the effect are invalid, then the effect is also invalid
			c_shader_instance_node* curr_node = m_shader_list_head;
			while(curr_node)
			{
				if(!curr_node->m_shader || !curr_node->m_shader->IsValidShader())
					return m_base_effect->runtime.flags.valid_effect_bit;

				curr_node = curr_node->m_next;
			}

			m_base_effect->runtime.flags.valid_effect_bit = true;
			return m_base_effect->runtime.flags.valid_effect_bit;
		}
		void		c_postprocess_effect::SetSource(void* pSource)
		{
			m_base_effect = CAST_PTR(TagGroups::s_shader_postprocess_effect_definition*, pSource);
		}

		HRESULT		c_postprocess_effect::DoPostProcessEffect(IDirect3DDevice9* pDevice,  double frame_time)
		{
			HRESULT hr = E_FAIL;
			if(!m_base_effect->runtime.flags.valid_effect_bit || !m_base_effect->runtime.flags.is_active_bit)
				return hr;		

			// An effect can be canceled by returning E_FAIL in DoPreEffect
			HRESULT_ERETURN(DoPreEffect(pDevice, frame_time));

			PP::Globals().m_render_targets.scene_buffer_chain.m_first_render = true;

			// Apply each shader in order
			int32 index = 0;
			c_shader_instance_node* curr_node = m_shader_list_head;
			while(curr_node)
			{
				if(!curr_node->m_shader)
				{
					hr = E_FAIL;
					break;
				}
				DoPreShader(pDevice, frame_time, index);
				curr_node->SetInstance();
				if(curr_node->m_is_active)
					hr = curr_node->m_shader->DoPostProcess(pDevice, frame_time, m_render_quad);
				DoPostShader(pDevice, frame_time, index);

				curr_node = curr_node->m_next;
				index++;
			}

			HRESULT_ERETURN(DoPostEffect(pDevice, frame_time));

			// if errors occurred leave the render-target as-is
			// otherwise swap the targets so that the result of 
			// this effect becomes the scene texture for the next
			if (SUCCEEDED(hr))				
				PP::Globals().m_render_targets.scene_buffer_chain.SetSceneToLast();
			return hr;
		}

		void		c_postprocess_effect::ReleaseResources()
		{
			// release the quad instance to reduce its reference count
			safe_release<c_quad_instance>(m_render_quad);
		}	
		void		c_postprocess_effect::AllocateResources(IDirect3DDevice9* pDevice)
		{
			m_render_quad = Globals().QuadManager().CreateQuad(
				m_base_effect->quad_tesselation.x,
				m_base_effect->quad_tesselation.y);
		}
		void		c_postprocess_effect::Update(real delta_time)
		{
			c_shader_instance_node* curr_node = m_shader_list_head;
			while(curr_node)
			{
				curr_node->UpdateInstance(delta_time);
				curr_node = curr_node->m_next;
			}
		}
		/////////////////////////////////////////////////////////////////////
	};
};
#endif