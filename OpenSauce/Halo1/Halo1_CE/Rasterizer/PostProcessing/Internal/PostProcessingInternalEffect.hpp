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
#include "Rasterizer/PostProcessing/PostProcessingEffect.hpp"
#include "Rasterizer/PostProcessing/PostProcessingShaderGenericBase.hpp"
#include "Rasterizer/PostProcessing/PostProcessingEffectGenericBase.hpp"
#include "Rasterizer/PostProcessing/Internal/PostProcessingInternalShader.hpp"

namespace Yelo
{
	namespace Postprocessing { namespace Subsystem { namespace Internal {
		/////////////////////////////////////////////////////////////////////
		// A replacement variable instance for exposing shader variables 
		// to halo script
		/////////////////////////////////////////////////////////////////////
		class c_internal_shader_variable_scripted_node : public c_generic_shader_variable_instance_node
		{
		public:
			union {
				bool				boolean;
				int32				integer32;
				real				real32;
				real_vector2d		vector2d;
				real_vector3d		vector3d;
				real_quaternion		vector4d;
				real_argb_color		color4d;
			}											m_value;
			c_internal_shader_variable_scripted_node*	m_next_scripted;

			c_internal_shader_variable_scripted_node() : c_generic_shader_variable_instance_node(),
				m_next_scripted(NULL)
			{
				memset(&m_value, 0, sizeof(m_value));
			}

			void				SetVariableSource(c_generic_shader_variable_node* shader_variable);
			void				SetVariable(LPD3DXEFFECT* dx_effect);

			virtual void		ClearReferences()
			{
				m_next_scripted = NULL;
			}
		};


		/////////////////////////////////////////////////////////////////////
		// Effect interface for applying tag based effects
		/////////////////////////////////////////////////////////////////////
		class c_internal_effect : public c_generic_effect_base
		{	
		public:
			bool	m_effect_loaded;
			PAD24;
			TagGroups::s_shader_postprocess_collection_effect*	m_effect_internal;

			c_internal_shader_variable_scripted_node*			m_scripted_variables;

			virtual bool		ValidateEffect();
			virtual void		SetSource(void* pSource);
			virtual void		AllocateCustomResources(IDirect3DDevice9* pDevice);

			void				AddScriptedVariable(c_internal_shader_variable_scripted_node* scripted_variable);
			void				SetupScriptedVariables();		

			virtual void		Ctor()
			{
				m_scripted_variables = NULL;
				m_effect_internal = NULL;

				c_generic_effect_base::Ctor();
			}

			virtual void		Dtor()
			{
				//TODO: The array members have already been deleted by the shader instances
				//so don't need deleting here, is this bad? If so a flag could be added
				//to stop the shader instance from deleting them so that they can be deleted here

				//delete [] m_scripted_variables;
				m_scripted_variables = NULL;

				c_generic_effect_base::Dtor();
			}
		};

	}; }; };
};
#endif