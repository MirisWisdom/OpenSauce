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
#include "Rasterizer/PostProcessing/PostProcessingShaderGenericBase.hpp"

namespace Yelo
{
	namespace Postprocessing { namespace Subsystem { namespace Bloom {
		/////////////////////////////////////////////////////////////////////
		// Tag struct for holding runtime values
		/////////////////////////////////////////////////////////////////////
		struct s_shader_postprocess_bloom : TagGroups::s_shader_postprocess_definition
		{
			TAG_FIELD(TagGroups::t_shader_variable_real,		bloom_size_handle);
			TAG_FIELD(TagGroups::t_shader_variable_real,		bloom_exposure_handle);
			TAG_FIELD(TagGroups::t_shader_variable_real,		bloom_mix_amount_handle);

			TAG_FIELD(TagGroups::t_shader_variable_real3d,		bloom_minimum_color_handle);
			TAG_FIELD(TagGroups::t_shader_variable_real3d,		bloom_maximum_color_handle);

			s_shader_postprocess_bloom()	{}
		};

		/////////////////////////////////////////////////////////////////////
		// Implements the bloom shader
		/////////////////////////////////////////////////////////////////////
		class c_bloom_shader : public c_postprocess_shader
		{			
			s_shader_postprocess_bloom*								m_bloom_shader;			
		public:
			TagGroups::s_shader_postprocess_bloom_definition*		m_bloom_definition;

			HRESULT		LoadShader(IDirect3DDevice9* pDevice);
			HRESULT		LoadCustomResources(IDirect3DDevice9* pDevice);
			void		UnloadShader();
			void		UnloadCustomResources();
			
			void		SetSource(void* pSource);
			HRESULT		SetupShader();

			void		Update();

			void		SetBloomValues(TagGroups::s_shader_postprocess_bloom_definition* values);

			void		Ctor()
			{				
				m_bloom_shader = NULL;
				m_bloom_definition = NULL;

				c_postprocess_shader::Ctor();
			}
		};
	}; }; };
};
#endif