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
#include "Rasterizer/PostProcessing/Internal/PostProcessingSubsystem_Internal.hpp"
#if !PLATFORM_IS_DEDI
#include "Common/YeloSettings.hpp"

namespace PP = Yelo::Postprocessing;

namespace Yelo
{
	namespace Postprocessing { namespace Subsystem { namespace Internal {

		s_internal_subsystem_globals s_internal_subsystem_globals::g_instance;
		s_internal_subsystem_globals& Globals() { return s_internal_subsystem_globals::g_instance; }


		/////////////////////////////////////////////////////////////////////
		// c_internal_subsystem
		bool c_internal_subsystem::g_subsystem_enabled;
		bool c_internal_subsystem::g_subsystem_loaded;
		c_internal_subsystem c_internal_subsystem::g_instance;

		void		c_internal_subsystem::Initialize(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters)
		{
			c_internal_subsystem::g_instance.InitializeImpl(pDevice, pParameters);
		}
		void		c_internal_subsystem::OnLostDevice()
		{
			c_internal_subsystem::g_instance.OnLostDeviceImpl();
		}
		void		c_internal_subsystem::OnResetDevice(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters)
		{
			// we don't want our resources allocated in the same way as other subsystems so we have to do it here
			c_internal_subsystem::g_instance.ResetRenderBlock();
			c_internal_subsystem::g_instance.AllocateResourcesImpl(pDevice);
			c_internal_subsystem::g_instance.OnResetDeviceImpl(pDevice, pParameters);
			c_internal_subsystem::g_instance.ValidateEffects();
		}
		void		c_internal_subsystem::ReleaseResources()
		{
			c_internal_subsystem::g_instance.ReleaseResourcesImpl();
		}
		void		c_internal_subsystem::LoadSettings(TiXmlElement* pp_element)
		{
			return c_internal_subsystem::g_instance.LoadSettingsImpl(pp_element);
		}
		void		c_internal_subsystem::SaveSettings(TiXmlElement* pp_element)
		{
			c_internal_subsystem::g_instance.SaveSettingsImpl(pp_element);
		}
		void		c_internal_subsystem::SetDefaultSettings()
		{
			c_internal_subsystem::g_instance.SetDefaultSettingsImpl();
		}
		void		c_internal_subsystem::InitializeForNewMap()
		{
			// the tags are internal, so we only want to be loading our shaders/effects on map load
			// load the new shaders, then allocate the resources and validate our effects
			c_internal_subsystem::g_instance.LoadShaders();		
			c_internal_subsystem::g_instance.AllocateResourcesImpl(PP::Globals().m_rendering.render_device);
			c_internal_subsystem::g_instance.ValidateEffects();
		}
		void		c_internal_subsystem::DisposeFromOldMap()
		{
			// Release shader resources then delete the memory allocated for them
			c_internal_subsystem::g_instance.ReleaseResourcesImpl();	
			c_internal_subsystem::g_instance.UnloadShaders();			
		};
		void		c_internal_subsystem::Update(real delta_time)
		{
			c_internal_subsystem::g_instance.UpdateImpl(delta_time);
		}
		bool		c_internal_subsystem::DoPostProcesses(IDirect3DDevice9* pDevice, real frame_time, Enums::postprocess_render_stage render_point)
		{
			return c_internal_subsystem::g_instance.DoPostProcessesImpl(pDevice, frame_time, render_point);
		}
		void		c_internal_subsystem::InitializeImpl(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters)
		{
			ZeroMemory(Globals().m_shader_array, sizeof(Globals().m_shader_array));
		}					
		void		c_internal_subsystem::OnLostDeviceImpl()
		{
			for(int32 i = 0; i < k_max_shader_count; i++)
				if(Globals().m_shader_array[i])Globals().m_shader_array[i]->OnLostDevice();
		}
		void		c_internal_subsystem::OnResetDeviceImpl(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters)
		{
			for(int32 i = 0; i < k_max_shader_count; i++)
				if(Globals().m_shader_array[i])Globals().m_shader_array[i]->OnResetDevice(pDevice, pParameters);
		}

		HRESULT		c_internal_subsystem::AllocateResourcesImpl(IDirect3DDevice9* pDevice)
		{
			HRESULT hr = S_OK;
			// for all of the currently loaded shaders, allocate all direct3D resources
			for(uint32 i = 0; i < Globals().m_shader_count; i++)
				hr |= Globals().m_shader_array[i]->AllocateResources(pDevice);
			// for all of the currently loaded effects, allocate all direct3D resources
			for(uint32 i = 0; i < Globals().m_effect_count; i++)
				Globals().m_effect_array[i]->AllocateResources(pDevice);

			// everything has been loaded
			g_subsystem_loaded = SUCCEEDED(hr) && PP::Globals().m_flags.loaded;
			return hr;
		}
		void		c_internal_subsystem::ReleaseResourcesImpl()
		{
			// for all of the currently loaded shaders, release all direct3D resources
			for(uint32 i = 0; i < Globals().m_shader_count; i++)
				Globals().m_shader_array[i]->ReleaseResources();
			// for all of the currently loaded effects, release all direct3D resources
			for(uint32 i = 0; i < Globals().m_effect_count; i++)
				Globals().m_effect_array[i]->ReleaseResources();

			// everything has been unloaded
			g_subsystem_loaded = false;
		}
		void		c_internal_subsystem::LoadSettingsImpl(TiXmlElement* pp_element)
		{
			if(pp_element != NULL)
			{
				TiXmlElement* element = pp_element->FirstChildElement("Internal");
				if(!element) return;

				g_subsystem_enabled = Settings::ParseBoolean( element->Attribute("enabled") );
			}
		}
		void		c_internal_subsystem::SaveSettingsImpl(TiXmlElement* pp_element)
		{
			TiXmlElement* element = NULL;

			element = new TiXmlElement("Internal");
				pp_element->LinkEndChild(element);

			element->SetAttribute("enabled", Settings::BooleanToString(g_subsystem_enabled));
		}
		void		c_internal_subsystem::SetDefaultSettingsImpl()
		{
			// in the event of not having an entry in the settings file, default to enabled
			g_subsystem_enabled = true;
		}
		void		c_internal_subsystem::Load() 
		{
			// because our shaders/effects are cache based we don't need to reload them from the cache, but the effect 
			// validity may change so clear out the render blocks, reallocate the resources and validate the effects
			if(!g_subsystem_loaded)
			{
				c_internal_subsystem::g_instance.ResetRenderBlock();
				c_internal_subsystem::g_instance.AllocateResourcesImpl(DX9::Direct3DDevice());
				c_internal_subsystem::g_instance.ValidateEffects();
			}
		}
		void		c_internal_subsystem::Unload() 
		{
			//if we are currently loaded, release all of our resources to unload
			if(g_subsystem_loaded)
				c_internal_subsystem::g_instance.ReleaseResourcesImpl();
		}
		bool		c_internal_subsystem::DoPostProcessesImpl(IDirect3DDevice9* pDevice, real frame_time, Enums::postprocess_render_stage render_point)
		{	
			// if this subsystem is disabled, do nothing
			if(!g_subsystem_enabled || !PP::Globals().FadeEffect().IsAvailable()) return false;
			// ...otherwise, apply the effects in the relevant render block
			return Globals().m_render_blocks[render_point].RenderEffects(pDevice, frame_time);
		}
		void		c_internal_subsystem::UnloadShaders()
		{
			// Shaders and effects should have been released before this function is called

			Globals().m_shader_collection = NULL;

			// delete all shaders
			for(int32 i = 0; i < k_max_shader_count; i++)
			{ 
				if(Globals().m_shader_array[i])
				{
					Globals().m_shader_array[i]->Dtor();
					delete Globals().m_shader_array[i];
					Globals().m_shader_array[i] = NULL;
				}
			}					
			// delete all effects
			for(int32 i = 0; i < k_max_effect_count; i++)
			{ 
				if(Globals().m_effect_array[i])
				{
					Globals().m_effect_array[i]->Dtor();
					delete Globals().m_effect_array[i];
					Globals().m_effect_array[i] = NULL;
				}
			}	
			// clear out all effect pointers in the render blocks
			ResetRenderBlock();

			Globals().m_shader_count = 0;
			Globals().m_effect_count = 0;
		}
		void		c_internal_subsystem::LoadShaders()
		{
			TagGroups::tag_iterator iter;
			// look for a shader_postprocess_collection tag in the cache
			tag_iterator_new(iter, TagGroups::s_shader_postprocess_collection::k_group_tag);
			// get the first instance (there should only be one, others will be ignored)
			datum_index tag_index = tag_iterator_next(iter);
			if(!tag_index.IsNull())
			{
				// set the current shader collection tag reference to the tag found
				Globals().m_shader_collection = TagGroups::Instances()[tag_index.index].Definition<TagGroups::s_shader_postprocess_collection>();

				// if the collection tag is the wrong version or the pointer is fubar'd, return
				if(!Globals().m_shader_collection)					
					return;
			}
			else
			{				
				Globals().m_shader_count = 0;
				Globals().m_effect_count = 0;
				return;
			}

			Globals().m_shader_count = Globals().m_shader_collection->shaders.Count;

			// loop through the tag collection shader block elements
			for(int32 i = 0; i < Globals().m_shader_collection->shaders.Count; ++i)
			{
				// get the shader_postprocess_generic tag referenced in the element
				datum_index shader_index = Globals().m_shader_collection->shaders[i].shader.tag_index;
				if(TagGroups::Instances()[shader_index.index].MatchesGroup(TagGroups::s_shader_postprocess_generic::k_group_tag))
				{
					TagGroups::s_shader_postprocess_generic* shpg = TagGroups::Instances()[shader_index.index].Definition<TagGroups::s_shader_postprocess_generic>();
					if(!shpg) continue;
					// used later for ease of reference when setting up shader variables
					Globals().m_shader_collection->shaders[i].runtime.shader_reference = shpg;

					// allocate a new shader interface class and set its source to the shader tag pointer
					Globals().m_shader_array[i] = new c_internal_shader();
					Globals().m_shader_array[i]->Ctor();
					Globals().m_shader_array[i]->SetID(TagGroups::Instances()[shader_index.index].name);
					Globals().m_shader_array[i]->SetSource((void*)shpg);
					Globals().m_shader_array[i]->SetupShader();
				}
			}

			Globals().m_effect_count = Globals().m_shader_collection->effects.Count;
			// loop through the tag collection shader block elements
			for(int32 i = 0; i < Globals().m_shader_collection->effects.Count; ++i)
			{
				// allocate a new effect interface class and set its source to the effect struct
				Globals().m_effect_array[i] = new c_internal_effect();
				Globals().m_effect_array[i]->Ctor();
				Globals().m_effect_array[i]->SetSource((void*)&Globals().m_shader_collection->effects[i]);
				Globals().m_effect_array[i]->SetupEffect();
			}
		}
		void		c_internal_subsystem::ResetRenderBlock()
		{
			// loop through all render blocks and clear out old pointers
			for(int32 i = 0; i < Enums::_postprocess_render_stage; ++i)
				Globals().m_render_blocks[i].ClearList();
		}
		void		c_internal_subsystem::ValidateEffects()
		{
			// loop through all the loaded effects and add them to the relevant render block if they are valid
			for(int32 i = 0; Globals().m_shader_collection && (i < Globals().m_shader_collection->effects.Count); ++i)
				if(Globals().m_effect_array[i]->ValidateEffect())
					Globals().m_render_blocks[Globals().m_effect_array[i]->m_effect_internal->render_stage].AddEffect(Globals().m_effect_array[i]);
		}
		void		c_internal_subsystem::UpdateImpl(real DeltaTime)
		{
			// if there are no internal effects then theres no point in going on
			if(!Globals().m_shader_collection || (Globals().m_shader_collection && (Globals().m_shader_collection->effects.Count == 0))) return;

			// update time reliant code in the effects
			for(uint32 i = 0; i < Globals().m_effect_count; i++)
				Globals().m_effect_array[i]->Update(DeltaTime);
		}
		bool		c_internal_subsystem::SetEffectActive(uint16 index, bool enabled)
		{
			if(!Globals().m_shader_collection) return false;
			if(index >= Globals().m_shader_collection->effects.Count) return false;

			Globals().m_shader_collection->effects[index].runtime.flags.is_active_bit = enabled;
			return true;
		}
		bool		c_internal_subsystem::SetEffectFade(uint16 index, real start, real end, real time)
		{
			if(!Globals().m_shader_collection) return false;
			if(index >= Globals().m_shader_collection->effects.Count) return false;

			Globals().m_effect_array[index]->FadeEffect(start, end, time);
			return true;
		}
		int16		c_internal_subsystem::GetEffectIndexByName(cstring test_string)
		{
			int16 return_id = NONE;
			if(!Globals().m_shader_collection) return return_id;

			// loop through all effects, looking for an effect called [TestString]
			for(int16 i = 0; i < Globals().m_shader_collection->effects.Count; i++)
			{
				if(strcmp(test_string, Globals().m_shader_collection->effects[i].name) == 0)
				{	
					return_id = i;
					break;
				}
			}
			return return_id;
		}
		real		c_internal_subsystem::GetEffectCurrentFade(uint16 index)
		{
			if(!Globals().m_shader_collection) return 0;

			if(index >= Globals().m_shader_collection->effects.Count) return 0;

			return Globals().m_effect_array[index]->m_fade.current;
		}
		int16		c_internal_subsystem::GetEffectFadeDirection(uint16 index)
		{
			if(!Globals().m_shader_collection) return 0;

			if(index >= Globals().m_shader_collection->effects.Count) return 0;

			return Globals().m_effect_array[index]->CurrentFadeDirection();
		}
		bool		c_internal_subsystem::GetEffectIsValid(uint16 index)
		{
			if(!Globals().m_shader_collection) return false;

			if(index >= Globals().m_shader_collection->effects.Count) return 0;

			return Globals().m_shader_collection->effects[index].runtime.flags.valid_effect_bit;
		}	
		int16		c_internal_subsystem::GetEffectShaderVariableIndexByName(uint16 effect_index, cstring name)
		{
			int16 return_id = NONE;
			if(effect_index >= Globals().m_effect_count) return return_id;

			//loop through all shader variables for an effect to see if one is called Name
			for(int32 i = 0; i < Globals().m_effect_array[effect_index]->m_effect_internal->script_variables.Count; i++)
			{
				if(strcmp(name, Globals().m_effect_array[effect_index]->m_effect_internal->script_variables[i].name) == 0)
				{
					return_id = (int16)i;
					break;
				}
			}
			return return_id;
		}
		void		c_internal_subsystem::SetEffectShaderVariableBoolean(uint16 effect_index, 
			uint16 variable_index, 
			bool value1, 
			float interp_time)
		{
			if(effect_index >= Globals().m_effect_count) return;
			if(variable_index >= Globals().m_effect_array[effect_index]->m_effect_internal->script_variables.Count) return;
			if(Globals().m_effect_array[effect_index]->m_effect_internal->script_variables[variable_index].value_type.type != 
				Enums::_shader_variable_base_type_boolean) return;

			c_internal_shader_variable_scripted_node* curr = Globals().m_effect_array[effect_index]->m_scripted_variables;
			int16 index = 0;
			while(index != variable_index)
			{
				curr = curr->m_next_scripted;
				index++;
			}
			curr->NextValue().boolean = value1;
			curr->SetInterpolationTime(interp_time);
		}
		void		c_internal_subsystem::SetEffectShaderVariableInteger(uint16 effect_index, 
			uint16 variable_index, 
			uint32 value1, 
			float interp_time)
		{
			if(effect_index >= Globals().m_effect_count) return;
			if(variable_index >= Globals().m_effect_array[effect_index]->m_effect_internal->script_variables.Count) return;
			if(Globals().m_effect_array[effect_index]->m_effect_internal->script_variables[variable_index].value_type.type != 
				Enums::_shader_variable_base_type_integer) return;

			c_internal_shader_variable_scripted_node* curr = Globals().m_effect_array[effect_index]->m_scripted_variables;
			int16 index = 0;
			while(index != variable_index)
			{
				curr = curr->m_next_scripted;
				index++;
			}
			curr->NextValue().integer32 = value1;
			curr->SetInterpolationTime(interp_time);
		}
		void		c_internal_subsystem::SetEffectShaderVariableReal(uint16 effect_index, 
			uint16 variable_index, 
			real value1, 
			real value2, 
			real value3, 
			real value4, 
			float interp_time)
		{
			if(effect_index >= Globals().m_effect_count) return;
			if(variable_index >= Globals().m_effect_array[effect_index]->m_effect_internal->script_variables.Count) return;
			if((Globals().m_effect_array[effect_index]->m_effect_internal->script_variables[variable_index].value_type.type != 
					Enums::_shader_variable_base_type_float) &&
				(Globals().m_effect_array[effect_index]->m_effect_internal->script_variables[variable_index].value_type.type != 
					Enums::_shader_variable_base_type_argb_color)) 
				return;

			c_internal_shader_variable_scripted_node* curr = Globals().m_effect_array[effect_index]->m_scripted_variables;
			int16 index = 0;
			while(index != variable_index)
			{
				curr = curr->m_next_scripted;
				index++;
			}

			// the available float variables can have up to 4 channels, may as well do it all in one function
			switch(curr->m_shader_variable->m_variable_datum->value_type.type)
			{
			case Enums::_shader_variable_base_type_float:
				switch(curr->m_shader_variable->m_variable_datum->value_type.count)
				{
				case 4: curr->NextValue().vector4d.w = value4;
				case 3: curr->NextValue().vector4d.k = value3;
				case 2: curr->NextValue().vector4d.j = value2;
				case 1: curr->NextValue().vector4d.i = value1;
				}
				break;

			case Enums::_shader_variable_base_type_argb_color:
				curr->NextValue().vector4d.k = value1;
				curr->NextValue().vector4d.i = value2;
				curr->NextValue().vector4d.j = value3;
				curr->NextValue().vector4d.w = value4;
				break;
			}
			curr->SetInterpolationTime(interp_time);
		}
		/////////////////////////////////////////////////////////////////////

	}; }; };
};
#endif