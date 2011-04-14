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
		/** A replacement variable node type for exposing shader variables 
			to halo script.*/
		class c_internal_shader_variable_scripted_node : public c_generic_shader_variable_instance_node
		{
			struct{
				/// The time the current value will take to change
				real			change_time;
				/// The current point in the values change from start to finish
				real			current_interpolation;
			} m_interpolation;

			typedef union 
			{
				bool				boolean;
				int32				integer32;
				real				real32;
				real_vector2d		vector2d;
				real_vector3d		vector3d;
				real_quaternion		vector4d;
				real_argb_color		color4d;
				real				reals[4];
			} t_value_union;

			/** m_values[0] is the start value, m_values[1] is the end value and 
				m_current_value is the current value.*/
			t_value_union m_values[2], m_current_value;

			/// Returns a value between the start and end values
			t_value_union		InterpolateValues(float interpolation);

		public:
			/// The next node in the scripted variables list
			c_internal_shader_variable_scripted_node*	m_next_scripted;

			c_internal_shader_variable_scripted_node() : c_generic_shader_variable_instance_node(),
				m_next_scripted(NULL)
			{
				memset(&m_interpolation, 0, sizeof(m_interpolation));
				memset(&m_values, 0, sizeof(m_values));
				memset(&m_current_value, 0, sizeof(m_current_value));
			}

			/** Stores a pointer to the source variables and sets the initial 
				value of the scripted variable.*/
			void				SetVariableSource(c_generic_shader_variable_node* shader_variable);
			/// Sets the variable in the effect to the value of the scripted variable.
			void				SetVariable(LPD3DXEFFECT* dx_effect);
			/** Causes the scripted variable to interpolate between the start
				and end values over a set time period.*/
			void				SetInterpolationTime(float interp_time);
			/// Updates the current value by interpolating between the start and end values
			void				UpdateVariable(real delta_time);

			virtual void		ClearReferences()
			{
				m_next_scripted = NULL;
			}

			/// Returns a reference to the target variable
			t_value_union&		NextValue() { return m_values[1]; }
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