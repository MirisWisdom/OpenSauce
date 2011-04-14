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
#include "Rasterizer/PostProcessing/PostProcessingEffectGenericBase.hpp"
#if !PLATFORM_IS_DEDI
#include <Blam/Halo1/shader_postprocess_definitions.hpp>
#include "Rasterizer/Rasterizer.hpp"
#include "Rasterizer/PostProcessing/PostProcessingDebug.hpp"

#include "Game/EngineFunctions.hpp"

namespace PP = Yelo::Postprocessing;

namespace Yelo
{
	namespace Postprocessing { namespace Subsystem {
		/////////////////////////////////////////////////////////////////////
		// c_generic_shader_variable_instance_node
		void		c_generic_shader_variable_instance_node::SetVariableSource(c_generic_shader_variable_node* shader_variable)
		{
			m_shader_variable = shader_variable;
		}
		/////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////
		// c_generic_shader_variable_animated_node
		void		c_generic_shader_variable_animated_node::UpdateVariable(real delta_time)
		{		
			double function_input = 0.0f;

			// if animation_rate != 0.0f then only update the variable when the update rate has been reached.
			bool update = false;
			if(m_shader_variable->m_variable_datum->animation_function.animation_rate != 0.0f)
			{
				m_animation.m_update_time += delta_time;
				while(m_animation.m_update_time > m_shader_variable->m_variable_datum->animation_function.animation_rate)
				{
					update = true;
					m_animation.m_update_time -= m_shader_variable->m_variable_datum->animation_function.animation_rate;
				}
			}
			else
				update = true;

			if(!update)
				return;

			if(m_shader_variable->m_variable_datum->animation_function.animation_duration == 0.0f)
				function_input = 0.0f;
			else
				function_input = Rasterizer::FrameInputs()->elapsed_time / m_shader_variable->m_variable_datum->animation_function.animation_duration;

			int channel_count = m_shader_variable->m_variable_datum->value_type.count;
			if(m_shader_variable->m_variable_datum->value_type.type == Enums::_shader_variable_base_type_argb_color)
				channel_count = 4;

			for(int32 i = 0; i < channel_count; i++)
			{
				// if the function is noise and we want multichannel noise, get a different value for each channel
				if((m_shader_variable->m_variable_datum->animation_function.function == Enums::_periodic_function_noise) &&
					(m_shader_variable->m_variable_datum->animation_function.flags.multichannel_noise_bit))
					function_input += (7 * i) * function_input;

				m_animation.m_interpolation.values[i] = (real)Engine::Math::PeriodicFunctionEvaluate(
					(Enums::periodic_function)m_shader_variable->m_variable_datum->animation_function.function, 
					function_input
				);
				// invert if necessary
				if(m_shader_variable->m_variable_datum->animation_function.flags.inverted_bit)
					m_animation.m_interpolation.values[i] = 1.0f - m_animation.m_interpolation.values[i];
			}

			// if we should ignore the alpha channel, set value1 to 1.0f.
			if(m_shader_variable->m_variable_datum->animation_function.flags.ignore_alpha_bit)
				m_animation.m_interpolation.value1 = 1.0f;
		}
		void		c_generic_shader_variable_animated_node::SetVariable(LPD3DXEFFECT* dx_effect)
		{
			void* lower;
			void* upper;
			m_shader_variable->m_variable_datum->GetBoundingValues(lower, upper);
			m_shader_variable->m_variable_datum->SetVariableInterp(dx_effect, lower, upper, m_animation.m_interpolation.values);
		}
		/////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////
		// c_generic_shader_variable_texture_node
		void		c_generic_shader_variable_texture_node::SetVariable(LPD3DXEFFECT* dx_effect)
		{
			// Set the texture variable to the loaded texture
			TagGroups::s_shader_postprocess_bitmap* bitmap_value = 
				CAST_PTR(TagGroups::s_shader_postprocess_bitmap*, m_shader_variable->m_variable_datum);

			if(bitmap_value->flags.is_external_bit)
				bitmap_value->value.bitmap.handle.SetVariable(dx_effect, bitmap_value->runtime.external.texture_2d);
			else
				bitmap_value->value.bitmap.handle.SetVariable(dx_effect, bitmap_value->runtime._internal.bitmap->hardware_format);
		}
		/////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////
		// c_generic_shader_instance_node
		void		c_generic_shader_instance_node::AddVariableInstance(c_generic_shader_variable_instance_node** variable_list, 
			c_generic_shader_variable_node* shader_variable)
		{			
			// Add a variable instance to a variable instance list
			c_generic_shader_variable_instance_node* instance = NULL;
			if(shader_variable->m_variable_datum->value_type.type == Enums::_shader_variable_base_type_texture)
				instance = new c_generic_shader_variable_texture_node();
			else
				instance = new c_generic_shader_variable_animated_node();
			instance->SetVariableSource(shader_variable);

			// add the variable to the list
			if(*variable_list == NULL)
			{
				*variable_list = instance;
				return;
			}
			
			c_generic_shader_variable_instance_node* curr = *variable_list;
			while(curr->m_next)
				curr = curr->m_next;
			curr->m_next = instance;
			instance->m_previous = curr;
		}
		void		c_generic_shader_instance_node::AddInstanceList(c_generic_shader_variable_instance_node** destination_list, c_generic_shader_variable_node** source_list)
		{
			// take a shaders variable list and build the variable instance list
			c_generic_shader_variable_node* curr = *source_list;
			while(curr)
			{
				AddVariableInstance(destination_list, curr);
				curr = curr->m_next;
			}
		}
		void		c_generic_shader_instance_node::SetInstanceList(c_generic_shader_variable_instance_node** list, LPD3DXEFFECT* dx_effect)
		{
			// set the shader variables for this shader instance variable list
			c_generic_shader_variable_instance_node* curr = *list;
			while(curr)
			{
				curr->SetVariable(dx_effect);
				curr = curr->m_next;
			}			
		}
		void		c_generic_shader_instance_node::UpdateInstanceList(c_generic_shader_variable_instance_node** list, real delta_time)
		{
			// update the shader variables for this shader instance variable list
			c_generic_shader_variable_instance_node* curr = *list;
			while(curr)
			{
				curr->UpdateVariable(delta_time);
				curr = curr->m_next;
			}			
		}
		void		c_generic_shader_instance_node::SetShaderSource(c_postprocess_shader* shader)
		{
			// Create the variables instance lists for this shader instance
			c_shader_instance_node::SetShaderSource(shader);
			c_generic_shader_base* generic_shader = CAST_PTR(c_generic_shader_base*, shader);

			AddInstanceList(&m_variable_instance_texture_head,	&generic_shader->m_shader_texture_variable_list_head);
			AddInstanceList(&m_variable_instance_boolean_head,	&generic_shader->m_shader_boolean_variable_list_head);
			AddInstanceList(&m_variable_instance_integer_head,	&generic_shader->m_shader_integer_variable_list_head);
			AddInstanceList(&m_variable_instance_float_head,	&generic_shader->m_shader_float_variable_list_head);
			AddInstanceList(&m_variable_instance_float2_head,	&generic_shader->m_shader_float2_variable_list_head);
			AddInstanceList(&m_variable_instance_float3_head,	&generic_shader->m_shader_float3_variable_list_head);
			AddInstanceList(&m_variable_instance_float4_head,	&generic_shader->m_shader_float4_variable_list_head);
			AddInstanceList(&m_variable_instance_color_head,	&generic_shader->m_shader_color_variable_list_head);
		}
		void		c_generic_shader_instance_node::UpdateInstance(real delta_time)
		{
			// Update the variables instance lists for this shader instance
			UpdateInstanceList(&m_variable_instance_boolean_head, delta_time);
			UpdateInstanceList(&m_variable_instance_integer_head, delta_time);
			UpdateInstanceList(&m_variable_instance_float_head, delta_time);
			UpdateInstanceList(&m_variable_instance_float2_head, delta_time);
			UpdateInstanceList(&m_variable_instance_float3_head, delta_time);
			UpdateInstanceList(&m_variable_instance_float4_head, delta_time);
			UpdateInstanceList(&m_variable_instance_color_head, delta_time);
		}
		void		c_generic_shader_instance_node::SetInstance()
		{
			// Set the variables instance lists for this shader instance
			SetInstanceList(&m_variable_instance_texture_head, m_shader->GetEffect());
			SetInstanceList(&m_variable_instance_boolean_head, m_shader->GetEffect());
			SetInstanceList(&m_variable_instance_integer_head, m_shader->GetEffect());
			SetInstanceList(&m_variable_instance_float_head, m_shader->GetEffect());
			SetInstanceList(&m_variable_instance_float2_head, m_shader->GetEffect());
			SetInstanceList(&m_variable_instance_float3_head, m_shader->GetEffect());
			SetInstanceList(&m_variable_instance_float4_head, m_shader->GetEffect());
			SetInstanceList(&m_variable_instance_color_head, m_shader->GetEffect());
		}
		void		c_generic_shader_instance_node::ReplaceVariableInstance(const TagGroups::shader_variable_type& value_type, cstring variable_name, c_generic_shader_variable_instance_node* shader_variable)
		{
			// Replace a variable instance will a different type if a matching variable is found
			c_generic_shader_variable_instance_node* current_variable;
			c_generic_shader_variable_instance_node** variable_list;

			switch(value_type.type)
			{
			default:	return;

			case Enums::_shader_variable_base_type_boolean:
				variable_list = &m_variable_instance_boolean_head;
				break;

			case Enums::_shader_variable_base_type_integer:
				variable_list = &m_variable_instance_integer_head;
				break;

			case Enums::_shader_variable_base_type_float:
				switch(value_type.count)
				{
				default:
					return;
				case 1: variable_list = &m_variable_instance_float_head;  break;
				case 2: variable_list = &m_variable_instance_float2_head; break;
				case 3: variable_list = &m_variable_instance_float3_head; break;
				case 4: variable_list = &m_variable_instance_float4_head; break;
				}
				break;

			case Enums::_shader_variable_base_type_argb_color:
				variable_list = &m_variable_instance_color_head;
				break;
			}
			current_variable = FindVariableInstance(variable_list, variable_name);

			if(!current_variable)
				return;

			// keep hold of the pointers to the next and previous variables
			c_generic_shader_variable_instance_node *previous_variable, *next_variable;
			previous_variable = current_variable->m_previous;
			next_variable = current_variable->m_next;

			// remove the current variable from the list
			if(previous_variable)
			{
				previous_variable->m_next = shader_variable;
				shader_variable->m_previous = previous_variable;
			}
			if(next_variable)
			{
				next_variable->m_previous = shader_variable;
				shader_variable->m_next = next_variable;
			}
			shader_variable->SetVariableSource(current_variable->m_shader_variable);

			// have to reset the list head if the first entry is being replaced
			if(*variable_list == current_variable)
				*variable_list = shader_variable;

			// clear the list node references, thus preventing it from deleting its old siblings
			current_variable->ClearReferences();

			// delete the current variable
			delete current_variable;
		}
		c_generic_shader_variable_instance_node* 
					c_generic_shader_instance_node::FindVariableInstance(c_generic_shader_variable_instance_node** variable_list, const char* variable_name)
		{
			// search through the variables in the list, looking for a variable with a matching name
			c_generic_shader_variable_instance_node* curr = *variable_list;
			while(curr)
			{
				if(strcmp(curr->m_shader_variable->m_variable_datum->value_name, variable_name) == 0)
					return curr;
				curr = curr->m_next;
			}	
			return NULL;
		}
		/////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////
		// c_generic_effect_base
		bool		c_generic_effect_base::ValidateEffect()
		{
			c_postprocess_effect::ValidateEffect();

			return m_effect_generic->runtime.flags.valid_effect_bit;
		}		
		void		c_generic_effect_base::SetSource(void* pSource)
		{
			m_effect_generic = CAST_PTR(TagGroups::s_shader_postprocess_effect_generic*, pSource);

			c_postprocess_effect::SetSource(pSource);
		}
		HRESULT		c_generic_effect_base::SetupEffect()
		{
			c_postprocess_effect::SetupEffect();

			m_effect_generic->runtime.flags.is_active_bit = !m_effect_generic->effect_activation_flags.not_initially_active_bit;
			m_effect_generic->runtime.flags.valid_effect_bit = false;

			return S_OK;
		}
		HRESULT		c_generic_effect_base::DoPreEffect(IDirect3DDevice9* pDevice, double frame_time)
		{
			HRESULT hr = S_OK;
			// check that the conditions have been met for this effect to be applied
			if(PP::ActivationVariables().m_in_cinematic && m_effect_generic->effect_activation_flags.not_active_during_cutscenes_bit)
				hr = E_FAIL;
			if(PP::ActivationVariables().m_local_player_in_vehicle && m_effect_generic->effect_activation_flags.disabled_when_using_a_vehicle_bit)
				hr = E_FAIL;
			if(PP::ActivationVariables().m_local_player_zoomed && m_effect_generic->effect_activation_flags.disabled_when_zoomed_bit)
				hr = E_FAIL;			
			return hr;
		}
		HRESULT		c_generic_effect_base::DoPostEffect(IDirect3DDevice9* pDevice, double frame_time)
		{	
			// if the fade shader is not loaded, do nothing
			if(!m_fade.apply_fade || !PP::Globals().FadeEffect().IsAvailable()) return S_OK;

			return PP::Globals().FadeEffect().FadeResult(pDevice, m_fade.current);
		}
		void		c_generic_effect_base::AllocateResources(IDirect3DDevice9* pDevice)
		{
			c_postprocess_effect::AllocateResources(pDevice);

			// if the effect is already loaded theres no need to reload
			if(m_effect_loaded)
				return;

			AllocateCustomResources(pDevice);

			m_effect_loaded = true;
		}
		void		c_generic_effect_base::Update(real delta_time)
		{
			c_postprocess_effect::Update(delta_time);

			if(!PP::Globals().FadeEffect().IsAvailable() || !m_fade.apply_fade) return;

			// update the fade amount according to the game delta time
			if(m_fade.fade_time == 0)
				m_fade.current = m_fade.end;
			else
				m_fade.current += CAST(real, ((m_fade.end - m_fade.start) / m_fade.fade_time) * delta_time);

			// true = increase; false = decrease
			if(m_fade.direction)
			{
				if(m_fade.current > m_fade.end)
					m_fade.current = m_fade.end;
			}
			else
			{
				if(m_fade.current < m_fade.end)
					m_fade.current = m_fade.end;
			}
		}
		void		c_generic_effect_base::FadeEffect(real start, real end, real time)
		{
			// called through halo script to set up the fade variables
			m_fade.apply_fade = true;
			m_fade.start = (start > 1 ? 1 : (start < 0 ? 0 : start));
			m_fade.end = (end > 1 ? 1 : (end < 0 ? 0 : end));
			m_fade.direction = m_fade.start < m_fade.end;
			m_fade.current = m_fade.start;
			m_fade.fade_time = time;
		}

		int16		c_generic_effect_base::CurrentFadeDirection()
		{
			// not applying fade
			if(!m_fade.apply_fade || (m_fade.current == m_fade.end))
				return -1;
			// fading in = 1, fading out = 0
			return (m_fade.direction ? 1 : 0);
		}
		/////////////////////////////////////////////////////////////////////
	}; };
};
#endif