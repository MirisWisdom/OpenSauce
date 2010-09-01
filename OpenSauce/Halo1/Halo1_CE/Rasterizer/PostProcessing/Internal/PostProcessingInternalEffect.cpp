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
#include "Rasterizer/PostProcessing/Internal/PostProcessingInternalEffect.hpp"
#if !PLATFORM_IS_DEDI
#include "Rasterizer/PostProcessing/Internal/PostProcessingSubsystem_Internal.hpp"

namespace PP = Yelo::Postprocessing;

namespace Yelo
{
	namespace Postprocessing { namespace Subsystem { namespace Internal {		
		/////////////////////////////////////////////////////////////////////
		// c_internal_shader_variable_scripted_node
		void		c_internal_shader_variable_scripted_node::SetVariable(LPD3DXEFFECT* dx_effect)
		{	
			// Set the scripted variable to it's current value
			m_shader_variable->m_variable_datum->SetVariable(dx_effect, &m_value);
		}
		void		c_internal_shader_variable_scripted_node::SetVariableSource(c_generic_shader_variable_node* shader_variable)
		{
			// Store the pointer to the the shader variable node	then set the scripted variables initial value
			// to the shader variables upper value.

			c_generic_shader_variable_instance_node::SetVariableSource(shader_variable);

			m_shader_variable->m_variable_datum->CopyDefaultVariable(&m_value);
		}
		/////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////
		// c_internal_effect
		bool		c_internal_effect::ValidateEffect()
		{
			c_generic_effect_base::ValidateEffect();

			// if we have no shader indices then this effect is invalid
			if(m_effect_internal->shader_indices.Count < 1)
				m_effect_internal->runtime.flags.valid_effect_bit = false;

			return m_effect_internal->runtime.flags.valid_effect_bit;
		}
		void		c_internal_effect::SetSource(void* pSource)
		{
			m_effect_internal = CAST_PTR(TagGroups::s_shader_postprocess_collection_effect*, pSource);

			c_generic_effect_base::SetSource(pSource);
		}
		void		c_internal_effect::AllocateCustomResources(IDirect3DDevice9* pDevice)
		{
			// loop through the shader indices, adding them to our render list
			for(int32 i = 0; i < m_effect_internal->shader_indices.Count; ++i)
			{
				c_generic_shader_instance_node* shader_node = new c_generic_shader_instance_node();
				shader_node->Ctor();
				shader_node->SetShaderSource(Globals().m_shader_array[m_effect_internal->shader_indices[i].shader_index]);

				AddProcess(shader_node);
			}	

			SetupScriptedVariables();
		}
		void		c_internal_effect::AddScriptedVariable(c_internal_shader_variable_scripted_node* scripted_variable)
		{
			// add a scripted variable to the scripted variable list
			if(m_scripted_variables == NULL)				
			{
				m_scripted_variables = scripted_variable;
				return;
			}

			c_internal_shader_variable_scripted_node* curr = m_scripted_variables;
			while(curr->m_next_scripted)
				curr = curr->m_next_scripted;
			curr->m_next_scripted = scripted_variable;
		}
		void		c_internal_effect::SetupScriptedVariables()
		{	
			// no shaders :(
			if(m_shader_list_head == NULL) 
				return;

			for(int32 i = 0; i < m_effect_internal->script_variables.Count; i++)
			{
				// get the shader instance this variable is intended for
				c_generic_shader_instance_node* shader = CAST_PTR(c_generic_shader_instance_node*, m_shader_list_head);
				int32 current_index = 0;
				while(current_index != m_effect_internal->script_variables[i].shader_index)
				{
					shader = CAST_PTR(c_generic_shader_instance_node*, shader->m_next);
					current_index++;
				}

				c_internal_shader_variable_scripted_node* new_variable = new c_internal_shader_variable_scripted_node();

				// replace the variable instance in the shader instance
				shader->ReplaceVariableInstance(m_effect_internal->script_variables[i].value_type,
					m_effect_internal->script_variables[i].shader_variable_name,
					new_variable);

				// add the variable to the scripted variable list
				AddScriptedVariable(new_variable);
			}			
		}
		/////////////////////////////////////////////////////////////////////

	}; }; };
}
#endif