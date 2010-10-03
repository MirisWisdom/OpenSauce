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
#include "Rasterizer/PostProcessing/Internal/PostProcessingInternalShader.hpp"
#if !PLATFORM_IS_DEDI
#include <Blam/Halo1/shader_postprocess_definitions.hpp>

namespace PP = Yelo::Postprocessing;

namespace Yelo
{
	namespace Postprocessing { namespace Subsystem { namespace Internal {
		/////////////////////////////////////////////////////////////////////
		// c_internal_shader 
		HRESULT		c_internal_shader::LoadBitmaps(IDirect3DDevice9* pDevice)
		{			
			// Allocates the bitmap D3D textures			
			c_generic_shader_variable_node* curr;

			// load bitmap textures
			curr = m_shader_texture_variable_list_head;
			while(curr)
			{
				TagGroups::s_shader_postprocess_bitmap* bitmap_var = CAST_PTR(TagGroups::s_shader_postprocess_bitmap*, curr->m_variable_datum); 
								
				//if a valid tag index is referenced continue
				if(!bitmap_var->bitmap.tag_index.IsNull())
				{
					// get the bitmap tag pointer
					datum_index bitmap_index = bitmap_var->bitmap.tag_index;
					TagGroups::s_bitmap_definition* bitm = TagGroups::Instances()[bitmap_index.index].Definition<TagGroups::s_bitmap_definition>();
					if(bitm == NULL)
						return E_FAIL;

					// set the bitmap source to a bitmap datum from the bitmap tag
					bitmap_var->SetSource(&bitm->bitmaps[bitmap_var->value.bitmap.bitmap_index]);
				}
				curr = curr->m_next;
			}

			// setting the shader variables happens in c_generic_shader_base
			return c_generic_shader_base::LoadBitmaps(pDevice);
		}
		/////////////////////////////////////////////////////////////////////
	}; }; };
}
#endif