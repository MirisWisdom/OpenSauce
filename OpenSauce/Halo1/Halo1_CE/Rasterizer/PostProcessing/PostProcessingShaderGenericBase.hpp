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

/*
	User Add-on
		Component: Post-processing
		Original Author: FireScythe
		Integration: kornman00
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include "Rasterizer/DX9/DX9.hpp"
#include "Rasterizer/PostProcessing/PostProcessing.hpp"
#include "Rasterizer/PostProcessing/PostProcessingShader.hpp"

namespace Yelo
{
	namespace Postprocessing { namespace Subsystem {

		enum { 
			k_max_shader_count = 32, 
			k_max_effect_count = 32,
			k_variable_count = 16,
			k_bitmap_count = 4,
		};

		/////////////////////////////////////////////////////////////////////
		// Interface class for shader variables
		/////////////////////////////////////////////////////////////////////
		class c_generic_shader_variable_node
		{
		public:
			c_generic_shader_variable_node* m_next;
			TagGroups::s_shader_postprocess_value_base* m_variable_datum;

			void				SetVariableDatum(TagGroups::s_shader_postprocess_value_base* variable_datum);

			c_generic_shader_variable_node() : 
				m_next(NULL),
				m_variable_datum(NULL)
			{
			}
			~c_generic_shader_variable_node()
			{
				delete m_next;
				m_next = NULL;
			}
		};

		/////////////////////////////////////////////////////////////////////
		// Generic shader class for variable accessibility
		/////////////////////////////////////////////////////////////////////
		class c_generic_shader_base 
			abstract : public c_postprocess_shader			
		{
		public:
			bool							m_shader_loaded;
			PAD24;
			TagGroups::s_shader_postprocess_generic* m_shader_generic;

			c_generic_shader_variable_node* m_shader_texture_variable_list_head;
			c_generic_shader_variable_node* m_shader_boolean_variable_list_head;
			c_generic_shader_variable_node* m_shader_integer_variable_list_head;
			c_generic_shader_variable_node* m_shader_float_variable_list_head;
			c_generic_shader_variable_node* m_shader_float2_variable_list_head;
			c_generic_shader_variable_node* m_shader_float3_variable_list_head;
			c_generic_shader_variable_node* m_shader_float4_variable_list_head;
			c_generic_shader_variable_node* m_shader_color_variable_list_head;
			
			virtual HRESULT		LoadShader(IDirect3DDevice9* pDevice);
			virtual HRESULT		LoadBitmaps(IDirect3DDevice9* pDevice);
			virtual void		UnloadShader();
			virtual HRESULT		LoadCustomResources(IDirect3DDevice9* pDevice);
			virtual HRESULT		SetupVariables(IDirect3DDevice9* pDevice);
			virtual void		UnloadCustomResources();
			virtual void		SetID(cstring pID);
			virtual void		SetSource(void* pSource);
			virtual HRESULT		SetupShader();
			void				AddVariable(c_generic_shader_variable_node** list_pointer, 
									TagGroups::s_shader_postprocess_value_base* variable_datum);
			bool				FindVariable(c_generic_shader_variable_node** list_pointer, tag_string var_name);
			
			virtual void		Ctor()
			{
				m_shader_loaded = false;
				m_shader_generic = NULL;

				c_postprocess_shader::Ctor();

				m_shader_texture_variable_list_head = NULL;
				m_shader_boolean_variable_list_head = NULL;
				m_shader_integer_variable_list_head = NULL;
				m_shader_float_variable_list_head = NULL;
				m_shader_float2_variable_list_head = NULL;
				m_shader_float3_variable_list_head = NULL;
				m_shader_float4_variable_list_head = NULL;
				m_shader_color_variable_list_head = NULL;
			}

			virtual void		Dtor()
			{
				c_postprocess_shader::Dtor();

				m_effect = NULL;

				delete m_shader_texture_variable_list_head;
				delete m_shader_boolean_variable_list_head;
				delete m_shader_integer_variable_list_head;
				delete m_shader_float_variable_list_head;
				delete m_shader_float2_variable_list_head;
				delete m_shader_float3_variable_list_head;
				delete m_shader_float4_variable_list_head;
				delete m_shader_color_variable_list_head;
				m_shader_texture_variable_list_head = NULL;
				m_shader_boolean_variable_list_head = NULL;
				m_shader_integer_variable_list_head = NULL;
				m_shader_float_variable_list_head = NULL;
				m_shader_float2_variable_list_head = NULL;
				m_shader_float3_variable_list_head = NULL;
				m_shader_float4_variable_list_head = NULL;
				m_shader_color_variable_list_head = NULL;
			}
		};
	}; };
};
#endif