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
#pragma once

#if !PLATFORM_IS_DEDI
#include "Rasterizer/GBuffer.hpp"
#include "Rasterizer/PostProcessing/PostProcessing.hpp"
#include "Rasterizer/PostProcessing/PostProcessingShader.hpp"
#include "Rasterizer/PostProcessing/PostProcessingShaderGenericBase.hpp"

namespace Yelo
{
	namespace Postprocessing { namespace Subsystem { namespace MotionBlur {

		struct s_shader_postprocess_motionblur : TagGroups::s_shader_postprocess_definition
		{
			TAG_FIELD(TagGroups::t_shader_variable_real,	blur_amount_var);
			TAG_FIELD(TagGroups::t_shader_variable_real,	vignette_amount_var);

			s_shader_postprocess_motionblur()	{}
		};

		struct c_motionblur_shader : public c_postprocess_shader
		{
			s_shader_postprocess_motionblur*				m_blur_shader;
			real											m_blur_amount;
			struct {
				bool enabled;
				PAD24;
				real current_transition;
			}m_vignette;
			
		public:
			HRESULT		LoadShader(IDirect3DDevice9* pDevice);
			HRESULT		LoadCustomResources(IDirect3DDevice9* pDevice);
			void		UnloadShader();
			void		UnloadCustomResources();
			HRESULT		DoPreRender(IDirect3DDevice9* pDevice, double frame_time);
			
			void		SetSource(void* pSource);
			HRESULT		SetupShader();

			void		Update(real delta_time);

			void		Ctor()
			{
				c_postprocess_shader::Ctor();

				m_blur_shader = NULL;
				m_blur_amount = 1.0f;
			}
		};

	};};};
};
#endif