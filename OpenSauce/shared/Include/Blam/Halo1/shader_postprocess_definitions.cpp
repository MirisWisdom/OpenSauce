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
#include <Common/Precompile.hpp>
#include <Blam/Halo1/shader_postprocess_definitions.hpp>

#if !PLATFORM_IS_EDITOR
	#include "Game/EngineFunctions.hpp"
#elif PLATFORM_IS_EDITOR
	#include "TagGroups/TagGroups.hpp"
#endif

namespace Yelo
{
	namespace TagGroups
	{
#if !PLATFORM_IS_EDITOR && !PLATFORM_IS_DEDI
		/////////////////////////////////////////////////////////////////////
		// s_shader_postprocess_parameter
		void s_shader_postprocess_parameter::GetBoundingValues(void*& lower_ref, void*& upper_ref)
		{
			switch(value_type.type)
			{
			case Enums::_shader_variable_base_type_boolean:
				value.boolean.inverse = !value.boolean.enabled;

				lower_ref = &value.boolean.enabled;
				upper_ref = &value.boolean.inverse;
				break;

			case Enums::_shader_variable_base_type_integer:
				lower_ref = &value.integer32.lower_bound;
				upper_ref = &value.integer32.upper_bound;
				break;

			case Enums::_shader_variable_base_type_float:
				switch(value_type.count)
				{
				case 1:
					lower_ref = &value.real32.lower_bound;
					upper_ref = &value.real32.upper_bound;
					break;
				case 2:
					lower_ref = &value.vector2d.lower_bound;
					upper_ref = &value.vector2d.upper_bound;
					break;
				case 3:
					lower_ref = &value.vector3d.lower_bound;
					upper_ref = &value.vector3d.upper_bound;
					break;
				case 4:
					lower_ref = &value.vector4d.lower_bound;
					upper_ref = &value.vector4d.upper_bound;
					break;
				}
				break;

			case Enums::_shader_variable_base_type_argb_color:
				lower_ref = &value.color4d.lower_bound;
				upper_ref = &value.color4d.upper_bound;
				break;
			}
		}

		void s_shader_postprocess_parameter::SetVariableInterp(LPD3DXEFFECT* effect, const void* lower_data, const void* upper_data, const real* interp_values)
		{
			switch(value_type.type)
			{
			case Enums::_shader_variable_base_type_boolean:
				value.boolean.handle.SetVariableInterp(effect, lower_data, upper_data, interp_values);
				break;

			case Enums::_shader_variable_base_type_integer:
				value.integer32.handle.SetVariableInterp(effect, lower_data, upper_data, interp_values);
				break;

			case Enums::_shader_variable_base_type_float:
				switch(value_type.count)
				{
				case 1: value.real32.handle.SetVariableInterp(effect, lower_data, upper_data, interp_values);
					break;
				case 2: value.vector2d.handle.SetVariableInterp(effect, lower_data, upper_data, interp_values);
					break;
				case 3: value.vector3d.handle.SetVariableInterp(effect, lower_data, upper_data, interp_values);
					break;
				case 4: value.vector4d.handle.SetVariableInterp(effect, lower_data, upper_data, interp_values);
					break;
				}
				break;

			case Enums::_shader_variable_base_type_argb_color:
				value.color4d.handle.SetVariableInterp(effect, lower_data, upper_data, interp_values);
				break;
			}
		}

		void s_shader_postprocess_parameter::SetVariable(LPD3DXEFFECT* effect, void* data, const bool fixup_argb_color_hack)
		{
			switch(value_type.type)
			{
			case Enums::_shader_variable_base_type_boolean:
				value.boolean.handle.SetVariable(effect, data, fixup_argb_color_hack);
				break;

			case Enums::_shader_variable_base_type_integer:
				value.integer32.handle.SetVariable(effect, data, fixup_argb_color_hack);
				break;

			case Enums::_shader_variable_base_type_float:
				switch(value_type.count)
				{
				case 1: value.real32.handle.SetVariable(effect, data, fixup_argb_color_hack);
					break;
				case 2: value.vector2d.handle.SetVariable(effect, data, fixup_argb_color_hack);
					break;
				case 3: value.vector3d.handle.SetVariable(effect, data, fixup_argb_color_hack);
					break;
				case 4: value.vector4d.handle.SetVariable(effect, data, fixup_argb_color_hack);
					break;
				}
				break;

			case Enums::_shader_variable_base_type_argb_color:
				value.color4d.handle.SetVariable(effect, data, fixup_argb_color_hack);
				break;
			}
		}

		void s_shader_postprocess_parameter::CopyDefaultVariable(void* dst)
		{
			switch(value_type.type)
			{
			case Enums::_shader_variable_base_type_boolean:
				*CAST_PTR(bool*, dst) = this->value.boolean.enabled;
				break;

			case Enums::_shader_variable_base_type_integer:
				*CAST_PTR(int32*, dst) = this->value.integer32.upper_bound;
				break;

			case Enums::_shader_variable_base_type_float:
				switch(value_type.count)
				{
				case 1: *CAST_PTR(real*, dst) = this->value.real32.upper_bound;
					break;
				case 2: *CAST_PTR(real_vector2d*, dst) = this->value.vector2d.upper_bound;
					break;
				case 3: *CAST_PTR(real_vector3d*, dst) = this->value.vector3d.upper_bound;
					break;
				case 4: *CAST_PTR(real_quaternion*, dst) = this->value.vector4d.upper_bound;
					break;
				}
				break;

			case Enums::_shader_variable_base_type_argb_color:
				*CAST_PTR(real_argb_color*, dst) = this->value.color4d.upper_bound;
				break;
			}
		}
		
		void s_shader_postprocess_parameter::SetSource(TagGroups::s_bitmap_data* source_bitmap)
		{
			bitmap_value.flags.is_external_bit = false;
			bitmap_value.runtime._internal.bitmap = source_bitmap;
		}
		void s_shader_postprocess_parameter::SetSource(cstring source_bitmap)
		{
			bitmap_value.flags.is_external_bit = true;

			size_t length = strlen(source_bitmap) + 1;
			char* source_name = new char[length];
			ZeroMemory(source_name, length);
			memcpy_s(source_name, length, source_bitmap, length);

			bitmap_value.runtime.external.source = source_name;
		}

		HRESULT s_shader_postprocess_parameter::LoadCacheBitmap(IDirect3DDevice9* pDevice)
		{
			HRESULT hr = E_FAIL;
			if(value_type.type != Enums::_shader_variable_base_type_texture)
				return hr;

			if(!bitmap_value.flags.is_external_bit)
			{
				if(bitmap_value.runtime._internal.bitmap != NULL)
				{
					// Add the bitmap to the pc texture cache, and create the direct3d texture
					Yelo::Engine::TextureCacheRequestTexture(bitmap_value.runtime._internal.bitmap, true, true);

					hr = (bitmap_value.runtime._internal.bitmap->hardware_format == NULL ? E_FAIL : S_OK);
				}
			}
			else
			{
				if(bitmap_value.runtime.external.source != NULL)
					hr = D3DXCreateTextureFromFile(pDevice, bitmap_value.runtime.external.source, &bitmap_value.runtime.external.texture_2d);
			}
			bitmap_value.flags.is_loaded_bit = SUCCEEDED(hr);
			return hr;
		}

		void s_shader_postprocess_parameter::ReleaseBitmap()
		{
			if(value_type.type != Enums::_shader_variable_base_type_texture)
				return;

			// No need to release the bitmap if it is internal, Halo handles that itself
			if(bitmap_value.flags.is_external_bit)
				Yelo::safe_release(bitmap_value.runtime.external.texture_2d);
		}

		IDirect3DTexture9* s_shader_postprocess_parameter::GetTexture()
		{
			if(value_type.type != Enums::_shader_variable_base_type_texture)
				return NULL;

			if(!bitmap_value.flags.is_loaded_bit)
				return NULL;
			return bitmap_value.flags.is_external_bit ? bitmap_value.runtime.external.texture_2d : CAST_PTR(IDirect3DTexture9*,bitmap_value.runtime._internal.bitmap->hardware_format);
		}


#elif PLATFORM_IS_EDITOR && !PLATFORM_IS_DEDI
		void s_shader_postprocess_parameter::SetParameter(s_shader_postprocess_value_base* value_source)
		{
			memcpy_s(&value_name, sizeof(tag_string), &value_source->value_name, sizeof(tag_string));
			memcpy_s(&value_type, sizeof(value_type), &value_source->value_type, sizeof(value_type));
			memcpy_s(&value, sizeof(value), &value_source->value, sizeof(value));
			memcpy_s(&animation_function, sizeof(animation_function), &value_source->animation_function, sizeof(animation_function));
		}
		void s_shader_postprocess_parameter::SetParameter(s_shader_postprocess_bitmap* value_source)
		{
			SetParameter(CAST_PTR(s_shader_postprocess_value_base*, value_source));

			bitmap_value.bitmap.tag_index = value_source->bitmap.tag_index;
			tag_reference_set(&bitmap_value.bitmap, value_source->bitmap.group_tag, value_source->bitmap.name);
		}
#endif
	};
};