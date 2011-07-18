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
#include "Rasterizer/PostProcessing/PostProcessingEffect.hpp"
#include "Rasterizer/PostProcessing/PostProcessingShaderGenericBase.hpp"

namespace Yelo
{
	namespace Postprocessing { namespace Subsystem {		
		/////////////////////////////////////////////////////////////////////
		// Base type for adding functionality to variable instances
		/////////////////////////////////////////////////////////////////////
		class c_generic_shader_variable_instance_node
		{
		public:
			c_generic_shader_variable_node*				m_shader_variable;
			c_generic_shader_variable_instance_node*	m_previous;
			c_generic_shader_variable_instance_node*	m_next;			
			
			virtual void		SetVariableSource(c_generic_shader_variable_node* shader_variable);
			virtual void		UpdateVariable(real delta_time)		{}
			virtual void		SetVariable(LPD3DXEFFECT* dx_effect){}

			virtual void		ClearReferences()
			{
				m_previous = NULL;
				m_next = NULL;
			}
			
			c_generic_shader_variable_instance_node() : 
				m_previous(NULL),
				m_next(NULL),
				m_shader_variable(NULL)
			{
			}
			virtual ~c_generic_shader_variable_instance_node()
			{
				delete m_next;
				m_next = NULL;
			}
		};
		/////////////////////////////////////////////////////////////////////
		// Animated variable instance type
		/////////////////////////////////////////////////////////////////////
		class c_generic_shader_variable_animated_node : public c_generic_shader_variable_instance_node
		{
		public:
			struct _animation {
				union _interpolation {
					real values[4];
					struct{
						real value1;
						real value2;
						real value3;
						real value4;
					};

					_interpolation() : 
						value1(1.0f),
						value2(1.0f),
						value3(1.0f),
						value4(1.0f)
					{}

				}m_interpolation;
				real m_update_time;

				_animation() : 
					m_interpolation(),
					m_update_time(0.0f)
				{}

			}m_animation;

			void				UpdateVariable(real delta_time);
			void				SetVariable(LPD3DXEFFECT* dx_effect);

			c_generic_shader_variable_animated_node() : c_generic_shader_variable_instance_node(),
				m_animation()
			{
			}
		};	
		
		/////////////////////////////////////////////////////////////////////
		// Texture variable instance type
		/////////////////////////////////////////////////////////////////////
		class c_generic_shader_variable_texture_node : public c_generic_shader_variable_instance_node
		{
			void				SetVariable(LPD3DXEFFECT* dx_effect);
		};

		/////////////////////////////////////////////////////////////////////
		// Shader instance node that provides variable instance functionality
		/////////////////////////////////////////////////////////////////////
		class c_generic_shader_instance_node : public c_shader_instance_node
		{
			c_generic_shader_variable_instance_node*	m_variable_instance_head;

		public:
			virtual void		UpdateInstance(real delta_time);
			void				SetInstanceList(LPD3DXEFFECT* dx_effect);
			void				UpdateInstanceList(real delta_time);
			void				AddInstanceList(c_generic_shader_variable_node** source_list);
			virtual void		SetInstance();
			virtual void		SetShaderSource(c_postprocess_shader* shader);
			void				AddVariableInstance(c_generic_shader_variable_node* shader_variable);
			void				ReplaceVariableInstance(cstring variable_name, c_generic_shader_variable_instance_node* shader_variable);
			c_generic_shader_variable_instance_node* 
								FindVariableInstance(cstring variable_name);
			
			c_generic_shader_instance_node() : c_shader_instance_node(),
				m_variable_instance_head(NULL)
			{
			}

			virtual ~c_generic_shader_instance_node()
			{
				delete m_variable_instance_head;

				m_variable_instance_head = NULL;
			}

		};

		/////////////////////////////////////////////////////////////////////
		// Generic effect class that provides fading
		/////////////////////////////////////////////////////////////////////
		class c_generic_effect_base : public c_postprocess_effect
		{	
		public:
			bool	m_effect_loaded;
			PAD24;
			TagGroups::s_shader_postprocess_effect_generic*	m_effect_generic;

			struct _fade {
				bool			apply_fade;
				bool			direction;
				PAD16;
				real			start;
				real			end;
				real			current;
				real			fade_time;

				_fade() : 
					apply_fade(false),
					direction(false),
					start(1.0f),
					end(1.0f),
					current(1.0f),
					fade_time(1.0f)
				{}

			}m_fade;

			virtual bool		ValidateEffect();
			virtual void		SetSource(void* pSource);
			virtual HRESULT		SetupEffect();
			virtual HRESULT		DoPreEffect(IDirect3DDevice9* pDevice, double frame_time);
			virtual HRESULT		DoPostEffect(IDirect3DDevice9* pDevice, double frame_time);
			virtual void		AllocateResources(IDirect3DDevice9* pDevice);
			virtual void		AllocateCustomResources(IDirect3DDevice9* pDevice)	{}
			virtual void		Update(real delta_time);		
			virtual void		FadeEffect(real start, real end, real time);

			int16				CurrentFadeDirection();

			virtual void		Ctor()
			{
				m_effect_loaded = false;

				c_postprocess_effect::Ctor();
			}
			
			virtual	void		Dtor()
			{
				c_postprocess_effect::Dtor();
			}
		};
	};};
};
#endif