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
		void		c_generic_shader_variable_node::SetVariableDatum(TagGroups::s_shader_postprocess_parameter* variable_datum)
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
			curr = m_shader_parameter_list_head;
			while(curr)
			{
				if(curr->m_variable_datum->value_type.type == Enums::_shader_variable_base_type_texture)
				{
					TagGroups::s_shader_postprocess_parameter* bitmap = CAST_PTR(TagGroups::s_shader_postprocess_parameter*, curr->m_variable_datum);
					hr |= bitmap->LoadCacheBitmap(pDevice);
				}
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
			curr = m_shader_parameter_list_head;

			while(curr)
			{
				switch(curr->m_variable_datum->value_type.type)
				{
				case Enums::_shader_variable_base_type_boolean:
					{
						curr->m_variable_datum->value.boolean.handle.Initialize(m_effect,
							curr->m_variable_datum->value_name,
							&curr->m_variable_datum->value.boolean.enabled);
					}
					break;
				case Enums::_shader_variable_base_type_integer:
					{
						curr->m_variable_datum->value.integer32.handle.Initialize(m_effect,
							curr->m_variable_datum->value_name,
							&curr->m_variable_datum->value.integer32.upper_bound);
					}
					break;
				case Enums::_shader_variable_base_type_float:
					{
						switch(curr->m_variable_datum->value_type.count)
						{
						case 1:
							curr->m_variable_datum->value.real32.handle.Initialize(m_effect,
								curr->m_variable_datum->value_name,
								&curr->m_variable_datum->value.real32.upper_bound);
							break;
						case 2:
							curr->m_variable_datum->value.vector2d.handle.Initialize(m_effect,
								curr->m_variable_datum->value_name,
								&curr->m_variable_datum->value.vector2d.upper_bound);
							break;
						case 3:
							curr->m_variable_datum->value.vector3d.handle.Initialize(m_effect,
								curr->m_variable_datum->value_name,
								&curr->m_variable_datum->value.vector3d.upper_bound);
							break;
						case 4:
							curr->m_variable_datum->value.vector4d.handle.Initialize(m_effect,
								curr->m_variable_datum->value_name,
								&curr->m_variable_datum->value.vector4d.upper_bound);
							break;
						};
					}
					break;
				case Enums::_shader_variable_base_type_argb_color:
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
					}
					break;
				case Enums::_shader_variable_base_type_texture:
					{
						TagGroups::s_shader_postprocess_parameter* bitmap = CAST_PTR(TagGroups::s_shader_postprocess_parameter*, curr->m_variable_datum);
						bitmap->value.bitmap.handle.Initialize(m_effect,
							bitmap->value_name,
							bitmap->GetTexture());
					}
					break;
				};
				curr = curr->m_next;
			}
			return hr;
		}
		void		c_generic_shader_base::UnloadCustomResources()
		{
			for (int32 i = 0; i < m_shader_generic->parameters.Count; ++i)
				m_shader_generic->parameters[i].ReleaseBitmap();
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

			for (int32 i = 0; i < m_shader_generic->parameters.Count; ++i)
				m_shader_generic->parameters[i].value.bitmap.handle.ClearHandles();

			return hr;
		}
		HRESULT		c_generic_shader_base::SetupVariables(IDirect3DDevice9* pDevice)
		{
			// Takes the variable blocks in m_shader_generic and creates the variable lists as necessary
			// This is where variable overrides are set up
			TagGroups::s_shader_postprocess_generic* shader_tag = m_shader_generic;
			
			for(int i = 0; i < shader_tag->parameters.Count; i++)
				AddVariable(&shader_tag->parameters[i]);
			return S_OK;
		}
		void		c_generic_shader_base::AddVariable(TagGroups::s_shader_postprocess_parameter* variable_datum)
		{
			// Adds a variable node to a variable node list
			if(!m_shader_parameter_list_head)
			{
				m_shader_parameter_list_head = new c_generic_shader_variable_node();
				(m_shader_parameter_list_head)->SetVariableDatum(variable_datum);
			}
			else
			{
				c_generic_shader_variable_node* curr = m_shader_parameter_list_head;
				while(curr->m_next)
					curr = curr->m_next;
				curr->m_next = new c_generic_shader_variable_node();
				curr->m_next->SetVariableDatum(variable_datum);
			}
		}
		/////////////////////////////////////////////////////////////////////
	}; };
};
#endif