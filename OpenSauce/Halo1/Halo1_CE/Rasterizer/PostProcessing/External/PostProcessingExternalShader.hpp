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
			template<typename T>
			void				SetupVariables_Base(
				cstring semantic, 
				TagBlock<T>& block, 
				const uint16 variable_count, 
				const TagGroups::shader_variable_type& value_type);

		public:
			HRESULT				LoadShader(IDirect3DDevice9* pDevice);
			HRESULT				LoadBitmaps(IDirect3DDevice9* pDevice);
			HRESULT				SetupVariables(IDirect3DDevice9* pDevice);

			virtual void		Dtor()
			{
				c_generic_shader_base::Dtor();
				// delete the texture path string.
				for(int32 i = 0; i < m_shader_generic->implementation.bitmaps.Count; i++)
					delete [] m_shader_generic->implementation.bitmaps[i].runtime.external.source;
				
				// delete any manually allocated block memory
				delete[] m_shader_generic->implementation.bitmaps.Address;				
				delete[] m_shader_generic->implementation.bools.Address;
				delete[] m_shader_generic->implementation.integers.Address;
				delete[] m_shader_generic->implementation.floats.Address;
				delete[] m_shader_generic->implementation.float2s.Address;
				delete[] m_shader_generic->implementation.float3s.Address;
				delete[] m_shader_generic->implementation.float4s.Address;
				delete[] m_shader_generic->implementation.colors.Address;

				// delete any manually allocated shader data
				delete[] m_shader_generic->shader_code_text.address;
				delete[] m_shader_generic->shader_code_binary.address;
			}
		};	

	};};};
};
#endif