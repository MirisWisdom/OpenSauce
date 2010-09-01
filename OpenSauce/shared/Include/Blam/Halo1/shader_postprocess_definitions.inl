/*
    Yelo: Open Sauce SDK

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

#include <string.h>
#include <utility>

namespace Yelo
{
	namespace TagGroups
	{
#if !PLATFORM_IS_EDITOR && !PLATFORM_IS_DEDI
		template<Enums::shader_variable_base_type TType, size_t TCount>
		void s_shader_postprocess_shader_variable<TType, TCount>::
			Initialize(LPD3DXEFFECT* effect, const char* id, void* data, bool semantic)
		{		
			if(effect == NULL || id == NULL || *id == '\0')
				return;

			if(semantic)
				runtime.dx_handle = (*effect)->GetParameterBySemantic(NULL, id);
			else					
				runtime.dx_handle = (*effect)->GetParameterByName(NULL, id);

			if(runtime.dx_handle != NULL)
				SetVariable(effect, data);
		}

		template<Enums::shader_variable_base_type TType, size_t TCount>
		template<typename TValueType>
		void s_shader_postprocess_shader_variable<TType, TCount>::
			GetValues(TValueType* values_out, const TValueType* values1, const TValueType* values2, const size_t count, const real* interp_values)
		{
			for(size_t i = 0; i < count; i++)
			{
				real range = CAST(real, values2[i] - values1[i]);
				values_out[i] = CAST(TValueType, (range * interp_values[i]) + values1[i]);
			}
		}

		template<Enums::shader_variable_base_type TType, size_t TCount>
		void s_shader_postprocess_shader_variable<TType, TCount>::
			GetValues(BOOL* values_out, const bool* values1, const bool* values2, const size_t count, const real* interp_values)
		{
			for(size_t i = 0; i < count; i++)
				values_out[i] = (interp_values[i] > 0.5 ? values1[i] : values2[i]);
		}
		template<Enums::shader_variable_base_type TType, size_t TCount>
		void s_shader_postprocess_shader_variable<TType, TCount>::
			GetValues(D3DXCOLOR* values_out, const FLOAT* values1, const FLOAT* values2, const size_t count, const real* interp_values)
		{
			for(size_t i = 0; i < count; i+=4, values_out++)
			{
				for(size_t x = 0; x < 4; x++)
				{
					FLOAT range = values2[i+x] - values1[i+x];
					FLOAT value = (range * interp_values[i+x]) + values1[i+x];

					switch(x)
					{
					case 0: values_out->a = value; break;
					case 1: values_out->r = value; break;
					case 2: values_out->g = value; break;
					case 3: values_out->b = value; break;
					}
				}
			}
		}

		template<Enums::shader_variable_base_type TType, size_t TCount>
		void s_shader_postprocess_shader_variable<TType, TCount>::
			SetVariableInterp(LPD3DXEFFECT* effect, const void* data1, const void* data2, const real* interp_values)
		{
			if(!IsUsed() || data1 == NULL || data2 == NULL)
				return;

			union {
				BOOL boolean[TCount];
				INT integer[TCount];
				FLOAT single[TCount];
				/*D3DXCOLOR*/FLOAT argb_color[TCount*4];
			}value;

			switch (TType)
			{
			default: return;

			case Enums::_shader_variable_base_type_boolean:
				GetValues(value.boolean, CAST_PTR(const bool*,data1), CAST_PTR(const bool*,data2), TCount, interp_values);
				break;

			case Enums::_shader_variable_base_type_integer:
				GetValues(value.integer, CAST_PTR(const INT*,data1), CAST_PTR(const INT*,data2), TCount, interp_values);
				break;

			case Enums::_shader_variable_base_type_float:
				GetValues(value.single, CAST_PTR(const FLOAT*,data1), CAST_PTR(const FLOAT*,data2), TCount, interp_values);
				break;

			case Enums::_shader_variable_base_type_argb_color:
				GetValues(CAST_PTR(D3DXCOLOR*, &value.argb_color), CAST_PTR(const FLOAT*,data1), CAST_PTR(const FLOAT*,data2), TCount * 4, interp_values);
				break;
			}

			SetVariable(effect, &value, false);
		}

		template<Enums::shader_variable_base_type TType, size_t TCount>
		void s_shader_postprocess_shader_variable<TType, TCount>::
			SetVariable(LPD3DXEFFECT* effect, void* data, const bool fixup_argb_color_hack)
		{
			if(!IsUsed() || data == NULL)
				return;

			switch (TType)
			{
			default: return;

			case Enums::_shader_variable_base_type_boolean:
				if(TCount == 1)	(*effect)->SetBool(runtime.dx_handle, *CAST_PTR(BOOL*, data));
				else			(*effect)->SetBoolArray(runtime.dx_handle, CAST_PTR(BOOL*, data), TCount);
				break;

			case Enums::_shader_variable_base_type_integer:
				if(TCount == 1)	(*effect)->SetInt(runtime.dx_handle, *CAST_PTR(INT*, data));
				else			(*effect)->SetIntArray(runtime.dx_handle, CAST_PTR(INT*, data), TCount);
				break;

			case Enums::_shader_variable_base_type_float:
				if(TCount == 1)	(*effect)->SetFloat(runtime.dx_handle, *CAST_PTR(FLOAT*, data));
				else			(*effect)->SetFloatArray(runtime.dx_handle, CAST_PTR(FLOAT*, data), TCount);
				break;

			case Enums::_shader_variable_base_type_argb_color:
				if(fixup_argb_color_hack)
				{
					D3DXCOLOR values_out_data[TCount];
					D3DXCOLOR* values_out = values_out_data;

					for(size_t i = 0; i < TCount; i++, values_out++)
					{
						memcpy_s(values_out, sizeof(*values_out), CAST_PTR(D3DXCOLOR*, data)+i, sizeof(*values_out));
						std::swap(values_out->b, values_out->a);
					}

					data = values_out_data;
				}

				(*effect)->SetFloatArray(runtime.dx_handle, CAST_PTR(FLOAT*, data), TCount * 4);
				break;

			case Enums::_shader_variable_base_type_matrix:
				(*effect)->SetMatrix(runtime.dx_handle, CAST_PTR(D3DXMATRIX*,data));
				break;

			case Enums::_shader_variable_base_type_texture:
				(*effect)->SetTexture(runtime.dx_handle, CAST_PTR(IDirect3DBaseTexture9*,data));
				break;
			}
		}
#endif
	};
};