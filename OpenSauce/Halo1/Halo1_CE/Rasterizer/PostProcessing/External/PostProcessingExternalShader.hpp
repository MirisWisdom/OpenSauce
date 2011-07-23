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
#include "Rasterizer/PostProcessing/PostProcessing.hpp"
#include "Rasterizer/PostProcessing/PostProcessingShaderGenericBase.hpp"

namespace Yelo
{
	namespace Postprocessing { namespace Subsystem { namespace External {

		/////////////////////////////////////////////////////////////////////
		// Class for loading and setting up non tag based shaders
		/////////////////////////////////////////////////////////////////////		
		class c_external_shader : public c_generic_shader_base
		{
			D3DXMACRO			m_shader_defines[2];

			uint16				GetUsedVariableCount(cstring semantic, const uint16 variable_count);
			void				SetupVariables_Base(
				cstring semantic, 
				TagBlock<TagGroups::s_shader_postprocess_parameter>& block,
				const uint16 index_start,
				const uint16 variable_count,
				const TagGroups::shader_variable_type& value_type);

		public:
			HRESULT				LoadShader(IDirect3DDevice9* pDevice);
			HRESULT				LoadBitmaps(IDirect3DDevice9* pDevice);
			HRESULT				SetupVariables(IDirect3DDevice9* pDevice);

			virtual void		Ctor()
			{
				c_generic_shader_base::Ctor();

				m_shader_defines[0].Name = "SHADER_EXTERNAL";
				m_shader_defines[0].Definition = NULL;

				m_shader_defines[1].Name = NULL;
				m_shader_defines[1].Definition = NULL;

				m_define_macros = &m_shader_defines[0];
			}
			virtual void		Dtor()
			{
				c_generic_shader_base::Dtor();
				// delete the texture path string.
				for(int32 i = 0; i < m_shader_generic->parameters.Count; i++)
					delete [] m_shader_generic->parameters[i].bitmap_value.runtime.external.source;
				
				// delete any manually allocated block memory
				delete [] m_shader_generic->parameters.Address;

				// delete any manually allocated shader data
				delete[] m_shader_generic->shader_code_text.address;
				delete[] m_shader_generic->shader_code_binary.address;

				delete m_shader_generic;
				m_shader_generic = NULL;
			}
		};	

	};};};
};
#endif