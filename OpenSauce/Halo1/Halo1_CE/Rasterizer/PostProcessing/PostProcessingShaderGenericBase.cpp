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
#include "Rasterizer/PostProcessing/PostProcessingShaderGenericBase.hpp"
#if !PLATFORM_IS_DEDI
#include <TagGroups/Halo1/bitmap_definitions.hpp>
#include <Blam/Halo1/shader_postprocess_definitions.hpp>

#include "Game/EngineFunctions.hpp"
#include "Game/GameState.hpp"
#include "TagGroups/TagGroups.hpp"
#include "Rasterizer/PostProcessing/PostProcessingDebug.hpp"

namespace PP = Yelo::Postprocessing;

namespace Yelo
{
	namespace Postprocessing { namespace Subsystem {
		/////////////////////////////////////////////////////////////////////
		// c_generic_shader_variable_node
		void		c_generic_shader_variable_node::SetVariableDatum(TagGroups::s_shader_postprocess_value_base* variable_datum)
		{
			m_variable_datum = variable_datum;
		}
		/////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////
		// c_generic_shader_variable_node
		HRESULT		c_generic_shader_base::LoadShader(IDirect3DDevice9* pDevice)
		{
			// Compile the shader using the data in the m_shader_generic tag struct
			if (!m_shader_generic)
				return E_FAIL;

			// iterate through the base shaders to get the shader data in the bottom most tag
			TagGroups::s_shader_postprocess_generic* shader = m_shader_generic;
			while(!shader->base_shader.tag_index.IsNull())
				shader = TagGroups::Instances()[shader->base_shader.tag_index.index].Definition<TagGroups::s_shader_postprocess_generic>();
			
			TagGroups::s_shader_postprocess_definition* shader_definition = shader;
			return LoadShader_Impl(pDevice, shader_definition);
		}
		HRESULT		c_generic_shader_base::LoadBitmaps(IDirect3DDevice9* pDevice)
		{
			HRESULT hr = S_OK;

			c_generic_shader_variable_node* curr;
			// load bitmap textures
			curr = m_shader_texture_variable_list_head;
			while(curr)
			{
				TagGroups::s_shader_postprocess_bitmap* bitmap = CAST_PTR(TagGroups::s_shader_postprocess_bitmap*, curr->m_variable_datum);
				hr |= bitmap->LoadCacheBitmap(pDevice);
				curr = curr->m_next;
			}
			return hr;
		}
		void		c_generic_shader_base::UnloadShader()
		{
			if(m_effect)
				Yelo::safe_release(*m_effect);
		}
		HRESULT		c_generic_shader_base::LoadCustomResources(IDirect3DDevice9* pDevice)
		{
			// Set up all of the variables in the shader
			HRESULT hr = S_OK;		

			// SetupVariables will take the variable blocks in the tag and create c_generic_shader_variable_node's for them
			// this can be overloaded by subsystems so that variables can be manually allocated
			if(!m_shader_loaded)
			{
				SetupVariables(pDevice);
				m_shader_loaded = true;
			}

			hr = LoadBitmaps(pDevice);

			c_generic_shader_variable_node* curr;
			// load bitmap handles
			curr = m_shader_texture_variable_list_head;
			while(curr)
			{
				TagGroups::s_shader_postprocess_bitmap* bitmap = CAST_PTR(TagGroups::s_shader_postprocess_bitmap*, curr->m_variable_datum);
				bitmap->value.bitmap.handle.Initialize(m_effect,
					bitmap->value_name,
					bitmap->GetTexture());
				curr = curr->m_next;
			}
			// load boolean handles
			curr = m_shader_boolean_variable_list_head;
			while(curr)
			{
				curr->m_variable_datum->value.boolean.handle.Initialize(m_effect,
					curr->m_variable_datum->value_name,
					&curr->m_variable_datum->value.boolean.enabled);
				curr = curr->m_next;
			}
			// load integer handles
			curr = m_shader_integer_variable_list_head;
			while(curr)
			{
				curr->m_variable_datum->value.integer32.handle.Initialize(m_effect,
					curr->m_variable_datum->value_name,
					&curr->m_variable_datum->value.integer32.upper_bound);
				curr = curr->m_next;
			}
			// load float handles
			curr = m_shader_float_variable_list_head;
			while(curr)
			{
				curr->m_variable_datum->value.real32.handle.Initialize(m_effect,
					curr->m_variable_datum->value_name,
					&curr->m_variable_datum->value.real32.upper_bound);
				curr = curr->m_next;
			}
			// load float2 handles
			curr = m_shader_float2_variable_list_head;
			while(curr)
			{
				curr->m_variable_datum->value.vector2d.handle.Initialize(m_effect,
					curr->m_variable_datum->value_name,
					&curr->m_variable_datum->value.vector2d.upper_bound);
				curr = curr->m_next;
			}
			// load float3 handles
			curr = m_shader_float3_variable_list_head;
			while(curr)
			{
				curr->m_variable_datum->value.vector3d.handle.Initialize(m_effect,
					curr->m_variable_datum->value_name,
					&curr->m_variable_datum->value.vector3d.upper_bound);
				curr = curr->m_next;
			}
			// load float4 handles
			curr = m_shader_float4_variable_list_head;
			while(curr)
			{
				curr->m_variable_datum->value.vector4d.handle.Initialize(m_effect,
					curr->m_variable_datum->value_name,
					&curr->m_variable_datum->value.vector4d.upper_bound);
				curr = curr->m_next;
			}	
			// load color handles
			curr = m_shader_color_variable_list_head;
			while(curr)
			{
				// the shader wants rgba, but we store argb, so a switcheroo is needed
				D3DXCOLOR temp_color;
				temp_color.a = curr->m_variable_datum->value.color4d.upper_bound.alpha;
				temp_color.r = curr->m_variable_datum->value.color4d.upper_bound.red;
				temp_color.g = curr->m_variable_datum->value.color4d.upper_bound.green;
				temp_color.b = curr->m_variable_datum->value.color4d.upper_bound.blue;

				curr->m_variable_datum->value.color4d.handle.Initialize(m_effect,
					curr->m_variable_datum->value_name,
					&temp_color, false);
				curr = curr->m_next;
			}
			return hr;
		}
		void		c_generic_shader_base::UnloadCustomResources()
		{
			for (int32 i = 0; i < m_shader_generic->implementation.additional_bitmaps.Count; ++i)
				m_shader_generic->implementation.additional_bitmaps[i].ReleaseBitmap();			
		}
		void		c_generic_shader_base::SetID(cstring pID)
		{
			// A string ID for the shader for debugging purposes
			strcpy_s(m_shader_id, MAX_PATH, pID);
		}
		void		c_generic_shader_base::SetSource(void* pSource)
		{
			// Set the tag source for the shader
			m_shader_generic = CAST_PTR(TagGroups::s_shader_postprocess_generic*, pSource);

			c_postprocess_shader::SetSource(pSource);
		}
		HRESULT		c_generic_shader_base::SetupShader()
		{
			// Initialize the shader to default values
			HRESULT hr = c_postprocess_shader::SetupShader();

			TagGroups::s_shader_postprocess_implementation& impl = m_shader_generic->implementation;

			int32 i;
			for (i = 0; i < impl.additional_bitmaps.Count; ++i)	impl.additional_bitmaps[i].value.bitmap.handle.ClearHandles();
			for (i = 0; i < impl.bools.Count; ++i)				impl.bools[i].value.boolean.handle.ClearHandles();
			for (i = 0; i < impl.integers.Count; ++i)			impl.integers[i].value.integer32.handle.ClearHandles();
			for (i = 0; i < impl.floats.Count; ++i)				impl.floats[i].value.real32.handle.ClearHandles();
			for (i = 0; i < impl.float2s.Count; ++i)			impl.float2s[i].value.vector2d.handle.ClearHandles();
			for (i = 0; i < impl.float3s.Count; ++i)			impl.float3s[i].value.vector3d.handle.ClearHandles();
			for (i = 0; i < impl.float4s.Count; ++i)			impl.float4s[i].value.vector4d.handle.ClearHandles();
			for (i = 0; i < impl.colors.Count; ++i)				impl.colors[i].value.color4d.handle.ClearHandles();

			return hr;
		}
		HRESULT		c_generic_shader_base::SetupVariables(IDirect3DDevice9* pDevice)
		{
			// Takes the variable blocks in m_shader_generic and creates the variable lists as necessary
			// This is where variable overrides are set up
			TagGroups::s_shader_postprocess_generic* shader_tag = m_shader_generic;
			while(shader_tag)
			{
				TagGroups::s_shader_postprocess_implementation& impl = shader_tag->implementation;
				int32 i;

				for(i = 0; i < impl.additional_bitmaps.Count; i++)
				{					
					if(FindVariable(&m_shader_texture_variable_list_head, impl.additional_bitmaps[i].value_name))
						continue;
					AddVariable(&m_shader_texture_variable_list_head, &impl.additional_bitmaps[i]);
					impl.additional_bitmaps[i].value_type.type = Enums::_shader_variable_base_type_texture;
					impl.additional_bitmaps[i].value_type.count = 1;
				}

				for(i = 0; i < impl.bools.Count; i++)
				{
					if(FindVariable(&m_shader_boolean_variable_list_head, impl.bools[i].value_name))
						continue;
					AddVariable(&m_shader_boolean_variable_list_head, &impl.bools[i]);
					impl.bools[i].value_type.type = Enums::_shader_variable_base_type_boolean;
					impl.bools[i].value_type.count = 1;
				}

				for(i = 0; i < impl.integers.Count; i++)
				{
					if(FindVariable(&m_shader_integer_variable_list_head, impl.integers[i].value_name))
						continue;
					AddVariable(&m_shader_integer_variable_list_head, &impl.integers[i]);
					impl.integers[i].value_type.type = Enums::_shader_variable_base_type_integer;
					impl.integers[i].value_type.count = 1;
				}

				for(i = 0; i < impl.floats.Count; i++)
				{
					if(FindVariable(&m_shader_float_variable_list_head, impl.floats[i].value_name))
						continue;
					AddVariable(&m_shader_float_variable_list_head, &impl.floats[i]);
					impl.floats[i].value_type.type = Enums::_shader_variable_base_type_float;
					impl.floats[i].value_type.count = 1;
				}

				for(i = 0; i < impl.float2s.Count; i++)
				{
					if(FindVariable(&m_shader_float2_variable_list_head, impl.float2s[i].value_name))
						continue;
					AddVariable(&m_shader_float2_variable_list_head, &impl.float2s[i]);
					impl.float2s[i].value_type.type = Enums::_shader_variable_base_type_float;
					impl.float2s[i].value_type.count = 2;
				}

				for(i = 0; i < impl.float3s.Count; i++)
				{
					if(FindVariable(&m_shader_float3_variable_list_head, impl.float3s[i].value_name))
						continue;
					AddVariable(&m_shader_float3_variable_list_head, &impl.float3s[i]);
					impl.float3s[i].value_type.type = Enums::_shader_variable_base_type_float;
					impl.float3s[i].value_type.count = 3;
				}

				for(i = 0; i < impl.float4s.Count; i++)
				{
					if(FindVariable(&m_shader_float4_variable_list_head, impl.float4s[i].value_name))
						continue;
					AddVariable(&m_shader_float4_variable_list_head, &impl.float4s[i]);
					impl.float4s[i].value_type.type = Enums::_shader_variable_base_type_float;
					impl.float4s[i].value_type.count = 4;
				}

				for(i = 0; i < impl.colors.Count; i++)
				{
					if(FindVariable(&m_shader_color_variable_list_head, impl.colors[i].value_name))
						continue;
					AddVariable(&m_shader_color_variable_list_head, &impl.colors[i]);
					impl.colors[i].value_type.type = Enums::_shader_variable_base_type_argb_color;
					impl.colors[i].value_type.count = 1;
				}

				if(shader_tag->base_shader.tag_index.IsNull())
					shader_tag = NULL;
				else
					shader_tag = TagGroups::Instances()[shader_tag->base_shader.tag_index.index].Definition<TagGroups::s_shader_postprocess_generic>();
			}
			return S_OK;
		}
		void		c_generic_shader_base::AddVariable(c_generic_shader_variable_node** list_pointer, 
						TagGroups::s_shader_postprocess_value_base* variable_datum)
		{
			// Adds a variable node to a variable node list
			if(!*list_pointer)
			{
				*list_pointer = new c_generic_shader_variable_node();
				(*list_pointer)->SetVariableDatum(variable_datum);
			}
			else
			{
				c_generic_shader_variable_node* curr = *list_pointer;
				while(curr->m_next)
					curr = curr->m_next;
				curr->m_next = new c_generic_shader_variable_node();
				curr->m_next->SetVariableDatum(variable_datum);
			}
		}	
		bool		c_generic_shader_base::FindVariable(c_generic_shader_variable_node** list_pointer, tag_string var_name)
		{
			// Searches a variable node list to see if one with the specified name alrady exists
			c_generic_shader_variable_node* curr = *list_pointer;
			while(curr)
			{
				if(strncmp(curr->m_variable_datum->value_name, var_name, Enums::k_tag_string_length) == 0)
					return true;
				curr = curr->m_next;
			}
			return false;
		}
		/////////////////////////////////////////////////////////////////////
	}; };
};
#endif