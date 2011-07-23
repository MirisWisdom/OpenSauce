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
		uint16		c_external_shader::GetUsedVariableCount(cstring semantic, const uint16 variable_count)
		{
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
			return count;
		}
		void		c_external_shader::SetupVariables_Base(cstring semantic, 
			TagBlock<TagGroups::s_shader_postprocess_parameter>& block,
			const uint16 index_start,
			const uint16 variable_count,
			const TagGroups::shader_variable_type& value_type
		)
		{
			// Allocate the block memory for the variables in the shader, and set the values accordingly
			char buffer[20];
			for (uint32 i = 0; i < variable_count; ++i)
			{
				int index = index_start + i;

				ZeroMemory(buffer, sizeof(buffer));
				sprintf_s(buffer, sizeof(buffer), semantic, i + 1);

				D3DXHANDLE variable_handle = (*m_effect)->GetParameterBySemantic(NULL, buffer);
				if(!variable_handle)
					continue;
				D3DXPARAMETER_DESC variable_description;
				(*m_effect)->GetParameterDesc(variable_handle, &variable_description);

				strcpy_s(block[index].value_name, Yelo::Enums::k_tag_string_length + 1, variable_description.Name);

				block[index].value_type = value_type;

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
						(*m_effect)->GetInt(annotation,			CAST_PTR(INT*,&block[index].value.integer32.lower_bound));
						break;

					case Enums::_shader_variable_base_type_float:
						switch(value_type.count)
						{
						case 1: (*m_effect)->GetFloat(annotation,		CAST_PTR(FLOAT*,&block[index].value.real32.lower_bound)); break;
						case 2: (*m_effect)->GetFloatArray(annotation,	CAST_PTR(FLOAT*,&block[index].value.vector2d.lower_bound), 2); break;
						case 3: (*m_effect)->GetFloatArray(annotation,	CAST_PTR(FLOAT*,&block[index].value.vector3d.lower_bound), 3); break;
						case 4: (*m_effect)->GetFloatArray(annotation,	CAST_PTR(FLOAT*,&block[index].value.vector4d.lower_bound), 4); break;
						}
						break;

					case Enums::_shader_variable_base_type_argb_color:
						(*m_effect)->GetFloatArray(annotation,	CAST_PTR(FLOAT*,&block[index].value.color4d.lower_bound), 4); 
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
							block[index].value.boolean.enabled = value > 0;
						}
						break;

					case Enums::_shader_variable_base_type_integer:
						(*m_effect)->GetInt(annotation,			CAST_PTR(INT*,&block[index].value.integer32.upper_bound)); 
						break;

					case Enums::_shader_variable_base_type_float:
						switch(value_type.count)
						{
						case 1: (*m_effect)->GetFloat(annotation,		CAST_PTR(FLOAT*,&block[index].value.real32.upper_bound)); break;
						case 2: (*m_effect)->GetFloatArray(annotation,	CAST_PTR(FLOAT*,&block[index].value.vector2d.upper_bound), 2); break;
						case 3: (*m_effect)->GetFloatArray(annotation,	CAST_PTR(FLOAT*,&block[index].value.vector3d.upper_bound), 3); break;
						case 4: (*m_effect)->GetFloatArray(annotation,	CAST_PTR(FLOAT*,&block[index].value.vector4d.upper_bound), 4); break;
						}
						break;

					case Enums::_shader_variable_base_type_argb_color:
						(*m_effect)->GetFloatArray(annotation,	CAST_PTR(FLOAT*,&block[index].value.color4d.upper_bound), 4); 
						break;
					}
				}
				
				annotation = (*m_effect)->GetAnnotationByName(variable_handle, "animation_function");
				if(annotation)
				{
					INT value;
					(*m_effect)->GetInt(annotation,		&value);
					block[index].animation_function.function = CAST(_enum, value);
				}
				
				annotation = (*m_effect)->GetAnnotationByName(variable_handle, "animation_flags");
				if(annotation)
				{
					INT value;
					(*m_effect)->GetInt(annotation,		&value);
					*CAST_PTR(byte_flags*,&block[index].animation_function.flags) = CAST(byte_flags, value);
				}
				
				annotation = (*m_effect)->GetAnnotationByName(variable_handle, "animation_duration");
				if(annotation)
					(*m_effect)->GetFloat(annotation,	&block[index].animation_function.animation_duration);
				
				annotation = (*m_effect)->GetAnnotationByName(variable_handle, "animation_rate");
				if(annotation)
					(*m_effect)->GetFloat(annotation,	&block[index].animation_function.animation_rate);
			}
		}

		HRESULT		c_external_shader::SetupVariables(IDirect3DDevice9* pDevice)
		{
			uint16 count_texture, count_bool, count_int, count_color,
				count_float, count_float2, count_float3, count_float4,
				count_total;

			count_texture = GetUsedVariableCount("VARTEXTURE_%i", k_bitmap_count);
			count_bool = GetUsedVariableCount("VARBOOL_%i", k_bitmap_count);
			count_int = GetUsedVariableCount("VARINT_%i", k_bitmap_count);
			count_color = GetUsedVariableCount("VARCOLOR_%i", k_bitmap_count);
			count_float = GetUsedVariableCount("VARFLOAT_%i", k_bitmap_count);
			count_float2 = GetUsedVariableCount("VARFLOAT2_%i", k_bitmap_count);
			count_float3 = GetUsedVariableCount("VARFLOAT3_%i", k_bitmap_count);
			count_float4 = GetUsedVariableCount("VARFLOAT4_%i", k_bitmap_count);

			count_total = count_texture + count_bool + count_int + count_color +
				count_float + count_float2 + count_float3 + count_float4;

			if(count_total == 0)
				return c_generic_shader_base::SetupVariables(pDevice);

			m_shader_generic->parameters.Address = new TagGroups::s_shader_postprocess_parameter[count_total];
			memset(&m_shader_generic->parameters[0], 0, sizeof(TagGroups::s_shader_postprocess_parameter) * count_total);
			m_shader_generic->parameters.Count = count_total;

			uint16 index_offset = 0;
			TagGroups::shader_variable_type value_type;
			value_type.count = 1;

			value_type.type = Enums::_shader_variable_base_type_texture;
			SetupVariables_Base("VARTEXTURE_%i",
				m_shader_generic->parameters,
				index_offset, count_texture, value_type);
			index_offset += count_texture;

			value_type.type = Enums::_shader_variable_base_type_boolean;
			SetupVariables_Base("VARBOOL_%i",
				m_shader_generic->parameters,
				index_offset, count_bool, value_type);
			index_offset += count_bool;

			value_type.type = Enums::_shader_variable_base_type_integer;
			SetupVariables_Base("VARINT_%i",
				m_shader_generic->parameters,
				index_offset, count_int, value_type);
			index_offset += count_int;
			
			value_type.type = Enums::_shader_variable_base_type_argb_color;
			SetupVariables_Base("VARCOLOR_%i",
				m_shader_generic->parameters,
				index_offset, count_color, value_type);
			index_offset += count_color;

			value_type.type = Enums::_shader_variable_base_type_float;
			SetupVariables_Base("VARFLOAT_%i",
				m_shader_generic->parameters,
				index_offset, count_float, value_type);
			index_offset += count_float;

			value_type.count++;
			SetupVariables_Base("VARFLOAT2_%i",
				m_shader_generic->parameters,
				index_offset, count_float2, value_type);
			index_offset += count_float2;

			value_type.count++;
			SetupVariables_Base("VARFLOAT3_%i",
				m_shader_generic->parameters,
				index_offset, count_float3, value_type);
			index_offset += count_float3;

			value_type.count++;
			SetupVariables_Base("VARFLOAT4_%i",
				m_shader_generic->parameters,
				index_offset, count_float4, value_type);
						
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
			curr = m_shader_parameter_list_head;
			while(curr)
			{
				do
				{
					if(curr->m_variable_datum->value_type.type != Enums::_shader_variable_base_type_texture)
						break;

					TagGroups::s_shader_postprocess_parameter* bitmap_var = CAST_PTR(TagGroups::s_shader_postprocess_parameter*, curr->m_variable_datum); 
					
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
				}while(false);
				
				curr = curr->m_next;
			}

			// setting the shader variables happens in c_generic_shader_base
			return c_generic_shader_base::LoadBitmaps(pDevice);
		}
		/////////////////////////////////////////////////////////////////////
	}; }; };
};
#endif