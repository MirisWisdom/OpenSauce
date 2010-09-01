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
#include "Rasterizer/PostProcessing/MotionBlur/PostProcessingMotionBlurShader.hpp"

#if !PLATFORM_IS_DEDI
#include "Rasterizer/PostProcessing/PostProcessingDebug.hpp"
#include "Game/EngineFunctions.hpp"
#include "Game/GameState.hpp"

namespace PP = Yelo::Postprocessing;

namespace Yelo
{
	namespace Postprocessing { namespace Subsystem { namespace MotionBlur {

		HRESULT		c_motionblur_shader::LoadShader(IDirect3DDevice9* pDevice)
		{
			uint32 data_size;
			void* data_pointer = PP::Globals().m_shader_file.GetDataPointer("PP_MotionBlur", &data_size);
			if(data_size == 0 || data_pointer == NULL)
				return E_FAIL;		

			// build the blur shader from the effect data
			LPD3DXBUFFER error_buffer;
			HRESULT hr = D3DXCreateEffect(
				pDevice, 
				data_pointer, 
				data_size, 
				NULL, 
				NULL, 
				NULL, 
				NULL, 
				&m_blur_shader->runtime.dx_effect, 
				&error_buffer
			);

			if (FAILED(hr))
			{
				if(GameState::DevmodeEnabled())
 				{
					Postprocessing::Debug::WriteLine(
						"Error: failed to load shader \"%s\"",
						"pp_shaders::PP_MotionBlur");
					Postprocessing::Debug::WriteD3DXErrors(error_buffer, 1);
 				}
			}
			m_effect = &m_blur_shader->runtime.dx_effect;

			Yelo::safe_release(error_buffer);
			return hr;
		}

		HRESULT		c_motionblur_shader::LoadCustomResources(IDirect3DDevice9* pDevice)
		{
			if(!PP::Globals().m_render_targets.gbuffer->m_rt_velocity.m_enabled)
				return E_FAIL;

			m_blur_shader->blur_amount_var.Initialize(m_effect, "c_blur_amount", &m_blur_amount, false);
			if(!m_blur_shader->blur_amount_var.IsUsed()) return E_FAIL;

			return S_OK;
		}

		void        c_motionblur_shader::UnloadShader()
		{
			if(m_effect != NULL)
				Yelo::safe_release(*m_effect);
		}

		void        c_motionblur_shader::UnloadCustomResources()
		{
			m_blur_shader->blur_amount_var.ClearHandles();
		}

		HRESULT		c_motionblur_shader::DoPreRender(IDirect3DDevice9* pDevice, double frame_time)
		{			
			m_blur_shader->blur_amount_var.SetVariable(m_effect, &m_blur_amount);

			return S_OK;
		}

		void		c_motionblur_shader::SetSource(void* pSource)
		{
			m_blur_shader = CAST_PTR(s_shader_postprocess_motionblur*, pSource);

			c_postprocess_shader::SetSource(pSource);
		}

		HRESULT		c_motionblur_shader::SetupShader()
		{
			HRESULT hr = c_postprocess_shader::SetupShader();

			m_blur_shader->blur_amount_var.ClearHandles();

			return hr;
		}

	}; }; };
};
#endif