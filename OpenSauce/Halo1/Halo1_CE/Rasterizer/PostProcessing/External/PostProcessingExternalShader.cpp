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
#include "Rasterizer/PostProcessing/External/PostProcessingExternalShader.hpp"
#if !PLATFORM_IS_DEDI
#include <Common/Halo1/YeloSharedSettings.hpp>
#include <Blam/Halo1/shader_postprocess_definitions.hpp>

#include "Rasterizer/PostProcessing/External/PostProcessingSubsystem_External.hpp"

namespace PP = Yelo::Postprocessing;

namespace Yelo
{
	namespace Postprocessing { namespace Subsystem { namespace External {
		/////////////////////////////////////////////////////////////////////
		// c_external_shader
		template<class T>
		void		c_external_shader::SetupVariables_Base(cstring semantic, TagBlock<T>& block, 
			const uint16 variable_count, 
			const TagGroups::shader_variable_type& value_type
		)
		{
			// Allocate the block memory for the variables in the shader, and set the values accordingly
			char buffer[20];
			ZeroMemory(buffer, sizeof(buffer));
			int count = 0;
			for (int32 i = 0; i < variable_count; ++i)
			{
				ZeroMemory(buffer, sizeof(buffer));
				sprintf_s(buffer, sizeof(buffer), semantic, i+1);

				if((*m_effect)->GetParameterBySemantic(NULL, buffer) == NULL)
					continue;
				count++;
			}
			if(count == 0) return;

			ZeroMemory(buffer, sizeof(buffer));

			// allocate memory for bitmap block elements
			block.Address = new T[count];
			memset(block.Address, 0, sizeof(T) * count);
			block.Count = count;

			for (int32 i = 0; i < block.Count; ++i)
			{
				ZeroMemory(buffer, sizeof(buffer));
				sprintf_s(buffer, sizeof(buffer), semantic, i+1);

				D3DXHANDLE variable_handle = (*m_effect)->GetParameterBySemantic(NULL, buffer);
				if(!variable_handle)
					continue;
				D3DXPARAMETER_DESC variable_description;
				(*m_effect)->GetParameterDesc(variable_handle, &variable_description);

				strcpy_s(block[i].value_name, Yelo::Enums::k_tag_string_length+1, variable_description.Name);

				if(value_type.type == Enums::_shader_variable_base_type_texture)
					continue;

				D3DXHANDLE annotation;
				annotation = (*m_effect)->GetAnnotationByName(variable_handle, "value_lower_bound");
				if(annotation)
				{
					switch(value_type.type)
					{
					case Enums::_shader_variable_base_type_boolean:
						break;

					case Enums::_shader_variable_base_type_integer:
						(*m_effect)->GetInt(annotation,			CAST_PTR(INT*,&block[i].value.integer32.lower_bound));
						break;

					case Enums::_shader_variable_base_type_float:
						switch(value_type.count)
						{
						case 1: (*m_effect)->GetFloat(annotation,		CAST_PTR(FLOAT*,&block[i].value.real32.lower_bound)); break;
						case 2: (*m_effect)->GetFloatArray(annotation,	CAST_PTR(FLOAT*,&block[i].value.vector2d.lower_bound), 2); break;
						case 3: (*m_effect)->GetFloatArray(annotation,	CAST_PTR(FLOAT*,&block[i].value.vector3d.lower_bound), 3); break;
						case 4: (*m_effect)->GetFloatArray(annotation,	CAST_PTR(FLOAT*,&block[i].value.vector4d.lower_bound), 4); break;
						}
						break;

					case Enums::_shader_variable_base_type_argb_color:
						(*m_effect)->GetFloatArray(annotation,	CAST_PTR(FLOAT*,&block[i].value.color4d.lower_bound), 4); 
						break;
					}
				}

				annotation = (*m_effect)->GetAnnotationByName(variable_handle, "value_upper_bound");
				if(annotation)
				{
					switch(value_type.type)
					{
					case Enums::_shader_variable_base_type_boolean:
						{
							BOOL value;
							(*m_effect)->GetBool(annotation, &value); 
							block[i].value.boolean.enabled = value > 0;
						}
						break;

					case Enums::_shader_variable_base_type_integer:
						(*m_effect)->GetInt(annotation,			CAST_PTR(INT*,&block[i].value.integer32.upper_bound)); 
						break;

					case Enums::_shader_variable_base_type_float:
						switch(value_type.count)
						{
						case 1: (*m_effect)->GetFloat(annotation,		CAST_PTR(FLOAT*,&block[i].value.real32.upper_bound)); break;
						case 2: (*m_effect)->GetFloatArray(annotation,	CAST_PTR(FLOAT*,&block[i].value.vector2d.upper_bound), 2); break;
						case 3: (*m_effect)->GetFloatArray(annotation,	CAST_PTR(FLOAT*,&block[i].value.vector3d.upper_bound), 3); break;
						case 4: (*m_effect)->GetFloatArray(annotation,	CAST_PTR(FLOAT*,&block[i].value.vector4d.upper_bound), 4); break;
						}
						break;

					case Enums::_shader_variable_base_type_argb_color:
						(*m_effect)->GetFloatArray(annotation,	CAST_PTR(FLOAT*,&block[i].value.color4d.upper_bound), 4); 
						break;
					}
				}
				
				annotation = (*m_effect)->GetAnnotationByName(variable_handle, "animation_function");
				if(annotation)
				{
					INT value;
					(*m_effect)->GetInt(annotation,		&value);
					block[i].animation_function.function = CAST(_enum, value);
				}
				
				annotation = (*m_effect)->GetAnnotationByName(variable_handle, "animation_flags");
				if(annotation)
				{
					INT value;
					(*m_effect)->GetInt(annotation,		&value);
					*CAST_PTR(byte_flags*,&block[i].animation_function.flags) = CAST(byte_flags, value);
				}
				
				annotation = (*m_effect)->GetAnnotationByName(variable_handle, "animation_duration");
				if(annotation)
					(*m_effect)->GetFloat(annotation,	&block[i].animation_function.animation_duration);
				
				annotation = (*m_effect)->GetAnnotationByName(variable_handle, "animation_rate");
				if(annotation)
					(*m_effect)->GetFloat(annotation,	&block[i].animation_function.animation_rate);
			}
		}

		HRESULT		c_external_shader::SetupVariables(IDirect3DDevice9* pDevice)
		{
			TagGroups::shader_variable_type value_type;
			value_type.count = 1;

			value_type.type = Enums::_shader_variable_base_type_texture;
			SetupVariables_Base<TagGroups::s_shader_postprocess_bitmap>("VARTEXTURE_%i",
				m_shader_generic->implementation.bitmaps,
				k_bitmap_count, value_type);


			value_type.type = Enums::_shader_variable_base_type_boolean;
			SetupVariables_Base<TagGroups::s_shader_postprocess_value_base>("VARBOOL_%i",
				m_shader_generic->implementation.bools,
				k_variable_count, value_type);
			value_type.type = Enums::_shader_variable_base_type_integer;
			SetupVariables_Base<TagGroups::s_shader_postprocess_value_base>("VARINT_%i",
				m_shader_generic->implementation.integers,
				k_variable_count, value_type);
			
			value_type.type = Enums::_shader_variable_base_type_argb_color;
			SetupVariables_Base<TagGroups::s_shader_postprocess_value_base>("VARCOLOR_%i",
				m_shader_generic->implementation.colors,
				k_variable_count, value_type);


			value_type.type = Enums::_shader_variable_base_type_float;
			SetupVariables_Base<TagGroups::s_shader_postprocess_value_base>("VARFLOAT_%i",
				m_shader_generic->implementation.floats,
				k_variable_count, value_type);
			value_type.type = Enums::_shader_variable_base_type_float;
			value_type.count++;
			SetupVariables_Base<TagGroups::s_shader_postprocess_value_base>("VARFLOAT2_%i",
				m_shader_generic->implementation.float2s,
				k_variable_count, value_type);
			value_type.type = Enums::_shader_variable_base_type_float;
			value_type.count++;
			SetupVariables_Base<TagGroups::s_shader_postprocess_value_base>("VARFLOAT3_%i",
				m_shader_generic->implementation.float3s,
				k_variable_count, value_type);
			value_type.type = Enums::_shader_variable_base_type_float;
			value_type.count++;
			SetupVariables_Base<TagGroups::s_shader_postprocess_value_base>("VARFLOAT4_%i",
				m_shader_generic->implementation.float4s,
				k_variable_count, value_type);
						
			return c_generic_shader_base::SetupVariables(pDevice);
		}

		HRESULT		c_external_shader::LoadShader(IDirect3DDevice9* pDevice)
		{
			// external shaders can't inherit from other shaders
			m_shader_generic->base_shader.tag_index = datum_index::null;
			return c_generic_shader_base::LoadShader(pDevice);				
		}

		HRESULT		c_external_shader::LoadBitmaps(IDirect3DDevice9* pDevice)
		{
			// loads bitmaps that are stored externally, from a path defined in the variables annotations						
			c_generic_shader_variable_node* curr;

			// load bitmap textures
			curr = m_shader_texture_variable_list_head;
			while(curr)
			{
				TagGroups::s_shader_postprocess_bitmap* bitmap_var = CAST_PTR(TagGroups::s_shader_postprocess_bitmap*, curr->m_variable_datum); 
				
				// get the resource annotation for the variable
				D3DXHANDLE variable_handle = (*m_effect)->GetParameterByName(NULL, bitmap_var->value_name);
				if(!variable_handle )
					return E_FAIL;
				D3DXHANDLE resource_string = (*m_effect)->GetAnnotationByName(variable_handle, "texture_location");
				if(!resource_string )
					return E_FAIL;

				// get the file path to the texture, relative to the Halo CE directory
				LPCSTR tempstr;
				(*m_effect)->GetString(resource_string, &tempstr);

				char resource_buffer[MAX_PATH];
				resource_buffer[0] = '\0';
				sprintf_s(resource_buffer, "%s%s", c_external_subsystem::g_subsystem_shader_textures_path, tempstr);

				// set the bitmap elements source to the texture path
				bitmap_var->SetSource(resource_buffer);	
				
				curr = curr->m_next;
			}

			// setting the shader variables happens in c_generic_shader_base
			return c_generic_shader_base::LoadBitmaps(pDevice);
		}
		/////////////////////////////////////////////////////////////////////
	}; }; };
};
#endif