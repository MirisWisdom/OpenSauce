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
#include "Rasterizer/PostProcessing/Bloom/PostProcessingBloomShader.hpp"
#if !PLATFORM_IS_DEDI
#include "Rasterizer/PostProcessing/PostProcessingDebug.hpp"
#include "Game/EngineFunctions.hpp"
#include "Game/GameState.hpp"

namespace PP = Yelo::Postprocessing;

namespace Yelo
{
	namespace Postprocessing { namespace Subsystem { namespace Bloom {
		/////////////////////////////////////////////////////////////////////
		// c_bloom_shader
		HRESULT		c_bloom_shader::LoadShader(IDirect3DDevice9* pDevice)
		{
			// Loads the bloom shader from the pp_shaders file

			uint32 data_size;
			void* data_pointer = PP::Globals().m_shader_file.GetDataPointer("PP_Bloom", &data_size);
			if(data_size == 0 || data_pointer == NULL)
				return E_FAIL;		

			// build the bloom shader from the effect data
			LPD3DXBUFFER error_buffer;
			HRESULT hr = D3DXCreateEffect(pDevice, 
				data_pointer, 
				data_size, 
				NULL, 
				NULL,
				NULL, 
				NULL, 
				&m_bloom_shader->runtime.dx_effect, 
				&error_buffer);	

			// print errors, else set resource
			if (FAILED(hr))
			{
				if(GameState::DevmodeEnabled())
 				{
					Postprocessing::Debug::WriteLine(
						"Error: failed to load shader \"%s\"",
						"pp_shaders::PP_Bloom");
					Postprocessing::Debug::WriteD3DXErrors(error_buffer, 1);
 				}
			}
			m_effect = &m_bloom_shader->runtime.dx_effect;

			Yelo::safe_release(error_buffer);
			return hr;
		}
		HRESULT		c_bloom_shader::LoadCustomResources(IDirect3DDevice9* pDevice)
		{
			// get the handles to the values for this specific shader			
			m_bloom_shader->bloom_size_handle.Initialize(m_effect, "c_bloom_size", NULL);
			m_bloom_shader->bloom_exposure_handle.Initialize(m_effect, "c_bloom_exposure", NULL);
			m_bloom_shader->bloom_mix_amount_handle.Initialize(m_effect, "c_bloom_mix_amount", NULL);

			m_bloom_shader->bloom_minimum_color_handle.Initialize(m_effect, "c_bloom_minimum_color", NULL);
			m_bloom_shader->bloom_maximum_color_handle.Initialize(m_effect, "c_bloom_maximum_color", NULL);

			return S_OK;
		}
		void		c_bloom_shader::UnloadShader()
		{
			if(m_effect != NULL)
				Yelo::safe_release(*m_effect);
		}
		void		c_bloom_shader::UnloadCustomResources()
		{
			// clear all handles
			m_bloom_shader->bloom_size_handle.ClearHandles();
			m_bloom_shader->bloom_exposure_handle.ClearHandles();
			m_bloom_shader->bloom_mix_amount_handle.ClearHandles();

			m_bloom_shader->bloom_minimum_color_handle.ClearHandles();
			m_bloom_shader->bloom_maximum_color_handle.ClearHandles();
		}			
		void		c_bloom_shader::SetSource(void* pSource)
		{		
			m_bloom_shader = CAST_PTR(s_shader_postprocess_bloom*, pSource);

			c_postprocess_shader::SetSource(pSource);
		}
		HRESULT		c_bloom_shader::SetupShader()
		{
			HRESULT hr = c_postprocess_shader::SetupShader();

			m_bloom_shader->bloom_size_handle.ClearHandles();
			m_bloom_shader->bloom_exposure_handle.ClearHandles();
			m_bloom_shader->bloom_mix_amount_handle.ClearHandles();

			m_bloom_shader->bloom_minimum_color_handle.ClearHandles();
			m_bloom_shader->bloom_maximum_color_handle.ClearHandles();
			return hr;
		}
		void		c_bloom_shader::SetBloomValues(TagGroups::s_shader_postprocess_bloom_definition* values)
		{
			m_bloom_definition = values;
		}
		void		c_bloom_shader::Update()
		{
			m_bloom_shader->bloom_size_handle.SetVariable(m_effect, &m_bloom_definition->size);
			m_bloom_shader->bloom_exposure_handle.SetVariable(m_effect, &m_bloom_definition->exposure);
			m_bloom_shader->bloom_mix_amount_handle.SetVariable(m_effect, &m_bloom_definition->mix_amount);

			m_bloom_shader->bloom_minimum_color_handle.SetVariable(m_effect, &m_bloom_definition->minimum_color);
			m_bloom_shader->bloom_maximum_color_handle.SetVariable(m_effect, &m_bloom_definition->maximum_color);
		}
		/////////////////////////////////////////////////////////////////////
	}; }; };
};
#endif