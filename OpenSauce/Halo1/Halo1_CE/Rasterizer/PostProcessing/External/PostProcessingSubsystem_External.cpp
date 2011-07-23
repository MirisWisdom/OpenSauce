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
#include "Rasterizer/PostProcessing/External/PostProcessingSubsystem_External.hpp"
#if !PLATFORM_IS_DEDI
#include <Common/Halo1/YeloSharedSettings.hpp>

namespace PP = Yelo::Postprocessing;

namespace Yelo
{
	namespace Postprocessing { namespace Subsystem { namespace External {

		s_external_subsystem_globals s_external_subsystem_globals::g_instance;
		s_external_subsystem_globals& Globals() { return s_external_subsystem_globals::g_instance; }

		/////////////////////////////////////////////////////////////////////
		// c_external_subsystem
		c_external_subsystem c_external_subsystem::g_instance;

		cstring c_external_subsystem::g_subsystem_shader_settings_name = "OS_Settings.PostProcessing.xml";
		char c_external_subsystem::g_subsystem_shaders_path[MAX_PATH];
		char c_external_subsystem::g_subsystem_shader_textures_path[MAX_PATH];
		bool c_external_subsystem::g_subsystem_enabled;
		bool c_external_subsystem::g_subsystem_loaded;

		void		c_external_subsystem::Initialize(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters)
		{
			// empty the shader and texture paths
			g_subsystem_shaders_path[0] = 0;
			g_subsystem_shader_textures_path[0] = 0;

			c_external_subsystem::g_instance.InitializeImpl(pDevice, pParameters);
		}
		void		c_external_subsystem::OnLostDevice()
		{
			c_external_subsystem::g_instance.OnLostDeviceImpl();
		}
		void		c_external_subsystem::OnResetDevice(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters)
		{
			c_external_subsystem::g_instance.OnResetDeviceImpl(pDevice, pParameters);
		}
		HRESULT		c_external_subsystem::AllocateResources(IDirect3DDevice9* pDevice)
		{
			// the shaders are external, and so likely to change, so reload them, 
			// then allocate resources and validate effects
			c_external_subsystem::g_instance.LoadShaders();
			HRESULT hr = c_external_subsystem::g_instance.AllocateResourcesImpl(pDevice);
			c_external_subsystem::g_instance.ValidateEffects();
			return hr;
		}
		void		c_external_subsystem::ReleaseResources()
		{
			c_external_subsystem::g_instance.ReleaseResourcesImpl();
		}
		void		c_external_subsystem::LoadSettings(TiXmlElement* pp_element)
		{
			return c_external_subsystem::g_instance.LoadSettingsImpl(pp_element);
		}
		void		c_external_subsystem::SaveSettings(TiXmlElement* pp_element)
		{
			c_external_subsystem::g_instance.SaveSettingsImpl(pp_element);
		}
		void		c_external_subsystem::SetDefaultSettings()
		{
			c_external_subsystem::g_instance.SetDefaultSettingsImpl();
		}
		void		c_external_subsystem::Update(real DeltaTime)
		{
			c_external_subsystem::g_instance.UpdateImpl(DeltaTime);
		}
		bool		c_external_subsystem::DoPostProcesses(IDirect3DDevice9* pDevice, real frame_time, Enums::postprocess_render_stage render_point)
		{
			return c_external_subsystem::g_instance.DoPostProcessesImpl(pDevice, frame_time, render_point);
		}
		void		c_external_subsystem::InitializeImpl(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters)
		{
			ZeroMemory(Globals().m_shader_array, sizeof(Globals().m_shader_array));
		}	
		void		c_external_subsystem::OnLostDeviceImpl()
		{
			for(int32 i = 0; i < Globals().m_shader_count; i++)
				Globals().m_shader_array[i]->OnLostDevice();
		}
		void		c_external_subsystem::OnResetDeviceImpl(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters)
		{
			for(int32 i = 0; i < Globals().m_shader_count; i++)
				Globals().m_shader_array[i]->OnResetDevice(pDevice, pParameters);
		}
		HRESULT		c_external_subsystem::AllocateResourcesImpl(IDirect3DDevice9* pDevice)
		{
			HRESULT hr = S_OK;
			// for all of the currently loaded shaders, allocate all direct3D resources
			for(int32 i = 0; i < Globals().m_shader_count; i++)
				hr |= Globals().m_shader_array[i]->AllocateResources(pDevice);
			// for all of the currently loaded effects, allocate all direct3D resources
			for(int32 i = 0; i < Globals().m_effect_count; i++)
				Globals().m_effect_array[i]->AllocateResources(pDevice);

			// everything has been loaded
			g_subsystem_loaded = SUCCEEDED(hr) && PP::Globals().m_flags.loaded;
			return hr;
		}
		void		c_external_subsystem::ReleaseResourcesImpl()
		{
			// delete all shaders. for external shaders this will also delete any manually allocated memory
			for(int32 i = 0; i < k_max_shader_count; i++)
			{ 
				if(Globals().m_shader_array[i])
				{
					Globals().m_shader_array[i]->ReleaseResources();
					Globals().m_shader_array[i]->Dtor();
					delete Globals().m_shader_array[i];
					Globals().m_shader_array[i] = NULL;
				}
			}		
			// delete all effects. for external effects this will also delete any manually allocated memory			
			for(int32 i = 0; i < k_max_effect_count; i++)
			{ 
				if(Globals().m_effect_array[i])
				{
					Globals().m_effect_array[i]->ReleaseResources();
					Globals().m_effect_array[i]->Dtor();
					delete Globals().m_effect_array[i];
					Globals().m_effect_array[i] = NULL;
				}
			}
			// everything has been unloaded
			g_subsystem_loaded = false;
		}
		void		c_external_subsystem::LoadSettingsImpl(TiXmlElement* pp_element)
		{
			if(pp_element != NULL)
			{
				TiXmlElement* element = pp_element->FirstChildElement("External");
				if(!element) return;

				g_subsystem_enabled = Settings::ParseBoolean( element->Attribute("enabled") );
			}
		}
		void		c_external_subsystem::SaveSettingsImpl(TiXmlElement* pp_element)
		{
			TiXmlElement* element = NULL;

			element = new TiXmlElement("External");
				pp_element->LinkEndChild(element);

			element->SetAttribute("enabled", Settings::BooleanToString(g_subsystem_enabled));
		}
		void		c_external_subsystem::SetDefaultSettingsImpl()
		{
			// in the event of not having an entry in the settings file, default to enabled
			g_subsystem_enabled = true;
		}
		void		c_external_subsystem::Load() 
		{
			// if we are currently not loaded, load the shaders, then allocate our resources and validate our effects
			if(!g_subsystem_loaded)
			{
				c_external_subsystem::g_instance.LoadShaders();
				c_external_subsystem::g_instance.AllocateResourcesImpl(DX9::Direct3DDevice());
				c_external_subsystem::g_instance.ValidateEffects();
			}
		}
		void		c_external_subsystem::Unload() 
		{
			// if we are currently loaded, release all of our resources to unload
			if(g_subsystem_loaded)
				c_external_subsystem::g_instance.ReleaseResourcesImpl();
		}
		bool		c_external_subsystem::DoPostProcessesImpl(IDirect3DDevice9* pDevice, real frame_time, Enums::postprocess_render_stage render_point)
		{
			// if this subsystem is disabled, do nothing
			if(!g_subsystem_enabled) return false;
			// ...otherwise, apply the effects in the relevant render block
			return Globals().m_render_blocks[render_point].RenderEffects(pDevice, frame_time);
		}
		void		c_external_subsystem::LoadShaders()
		{
			// clear out all render block entries
			ResetRenderBlock();

			TiXmlDocument pp_setup = TiXmlDocument();
			TiXmlElement* root_element = NULL;
			{
				char file_path[MAX_PATH];
				if( !Settings::GetSettingsFilePath(g_subsystem_shader_settings_name, file_path) || 
					(root_element = Settings::GenericSettingsParse(pp_setup, file_path, "postprocessing")) == NULL)
					return;
			}

			// get the shader and texture directories from the settings file
			const char* shader_directory = root_element->Attribute("shader_directory");
			const char* texture_directory = root_element->Attribute("texture_directory");
			if(!shader_directory || !texture_directory) return;

			// copy the paths into memory
			g_subsystem_shaders_path[0] = 0;
			g_subsystem_shader_textures_path[0] = 0;

			strcat_s(g_subsystem_shaders_path, sizeof(g_subsystem_shaders_path), shader_directory);
			strcat_s(g_subsystem_shader_textures_path, sizeof(g_subsystem_shader_textures_path), texture_directory);

			// make sure they end with a backslash
			int shader_path_len = strlen(g_subsystem_shaders_path);
			int texture_path_len = strlen(g_subsystem_shader_textures_path);

			if(g_subsystem_shaders_path[shader_path_len - 1] != '\\')
			{
				g_subsystem_shaders_path[shader_path_len] = '\\';
				g_subsystem_shaders_path[shader_path_len + 1] = 0;
			}
			if(g_subsystem_shader_textures_path[texture_path_len - 1] != '\\')
			{
				g_subsystem_shader_textures_path[texture_path_len] = '\\';
				g_subsystem_shader_textures_path[texture_path_len + 1] = 0;
			}			

			Globals().m_shader_count = 0;
			Globals().m_effect_count = 0;

			// load each shader entry
			TiXmlElement* shader_list_element = NULL,
						* shader_element = NULL;

			shader_list_element = root_element->FirstChildElement("shaders");
			if(shader_list_element)
				shader_element = shader_list_element->FirstChildElement("shader");

			do
			{
				if(!shader_element)
					break;

				// build the fx file path
				cstring filename = shader_element->GetText();
				if(!filename)
					continue;
				char file_path[MAX_PATH];
				sprintf_s(file_path, "%s%s.fx", g_subsystem_shaders_path, filename);

				// get the files local directory for the include path
				char file_include_path[MAX_PATH];
				file_include_path[0] = 0;
				strcat_s(file_include_path, MAX_PATH, g_subsystem_shaders_path);

				//search for the last backslash in the filename
				int index = strlen(filename);
				while((index != 0) && (filename[index] != '\\'))
					index--;
				// if found, append the directory path to the include path
				if(index != 0)
				{
					char* relative_directory = new char[index + 2];
					memcpy_s(relative_directory, index + 2, filename, index + 1);
					relative_directory[index + 1] = 0;
					strcat_s(file_include_path, MAX_PATH, relative_directory);
					delete [] relative_directory;
				}

				// open the shader file and get it's length
				HANDLE file_handle = CreateFile(file_path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				if(file_handle == (HANDLE)NULL_HANDLE)
					continue;

				DWORD file_length = GetFileSize(file_handle, NULL);
				// if the file has no length there is no point loading it
				if(file_length == 0)
				{
					CloseHandle(file_handle);
					continue;
				}						

				// create a fake shader_postprocess_generic tag and NULL its variables
				TagGroups::s_shader_postprocess_generic* shader = new TagGroups::s_shader_postprocess_generic();
				memset(shader, 0, sizeof(TagGroups::s_shader_postprocess_generic));

				// allocate memory for the ASCII shader code
				shader->shader_code_text.address = new char[file_length];
				shader->shader_code_text.size = file_length;

				// read the entire shader file into the allocated memory
				DWORD bytes_read;
				BOOL success = ReadFile(file_handle, 
					shader->shader_code_text.address, 
					file_length,
					&bytes_read,
					NULL);
				CloseHandle(file_handle);

				// if the read failed or an incorrect number of bytes were read, move on to the next shader
				if(!success || (bytes_read != file_length))
				{
					delete [] shader->shader_code_text.address;
					delete shader;
					continue;
				}

				// create a new shader interface class, then sets its source to the fake tag that was just created
				Globals().m_shader_array[Globals().m_shader_count] = new c_external_shader();
				Globals().m_shader_array[Globals().m_shader_count]->Ctor();
				Globals().m_shader_array[Globals().m_shader_count]->SetID(filename);
				Globals().m_shader_array[Globals().m_shader_count]->SetSource(shader);
				Globals().m_shader_array[Globals().m_shader_count]->SetupShader();
				strcpy_s(Globals().m_shader_array[Globals().m_shader_count]->IncludePath(), MAX_PATH, file_include_path);

				Globals().m_shader_count++;
			}
			while((shader_element = shader_element->NextSiblingElement("shader")) &&
				(Globals().m_shader_count < k_max_shader_count));

			TiXmlElement* effects_element = NULL,
						* effect_element = NULL;

			effects_element = root_element->FirstChildElement("effects");
			if(effects_element)
				effect_element = effects_element->FirstChildElement("effect");

			do
			{
				if(!effect_element)
					break;

				cstring name = effect_element->Attribute("name");

				int render_stage;
				if(!effect_element->Attribute("render_stage", &render_stage) != TIXML_SUCCESS)
					render_stage = Enums::_postprocess_render_stage_pre_hud;

				int tessellation_x, tessellation_y;
				if(effect_element->QueryIntAttribute("quad_tessellation_x", &tessellation_x) != TIXML_SUCCESS)
					tessellation_x = 4;
				if(effect_element->QueryIntAttribute("quad_tessellation_y", &tessellation_y) != TIXML_SUCCESS)
					tessellation_y = 4;

				s_shader_postprocess_external_effect::_effect_activation_flags flags;
				flags.not_initially_active_bit = false;
				flags.not_active_during_cutscenes_bit = false;
				flags.disabled_when_zoomed_bit = false;
				flags.disabled_when_using_a_vehicle_bit = false;

				TiXmlElement* flags_element = effect_element->FirstChildElement("flags");
				if(flags_element)
				{
					TiXmlElement* flag_element = flags_element->FirstChildElement("flag");
					while(flag_element)
					{
						cstring flag_name = flag_element->Attribute("name");
						bool flag_value = Settings::ParseBoolean( flag_element->Attribute("value") );
						if(strcmp(flag_name, "not_active_during_cutscenes") == 0)
							flags.not_active_during_cutscenes_bit = flag_value;
						else if(strcmp(flag_name, "disabled_when_zoomed") == 0)
							flags.disabled_when_zoomed_bit = flag_value;
						else if(strcmp(flag_name, "disabled_when_using_a_vehicle") == 0)
							flags.disabled_when_using_a_vehicle_bit = flag_value;

						flag_element = flag_element->NextSiblingElement("flag");
					}
				}

				int32 shader_count = 0;
				int indices[k_max_shader_count];
				memset(&indices, 0, sizeof(indices));

				// get the effects "shaders" node
				TiXmlElement* effect_shaders_element = effect_element->FirstChildElement("shaders");
				if(effect_shaders_element && shader_list_element)
				{
					// iterate through the effects shader elements
					TiXmlElement* effect_shader_instance_element = effect_shaders_element->FirstChildElement("shader");
					do
					{
						if(!effect_shader_instance_element)
							break;

						int index = 0;
						// compare the elements text with the ids in the main shader list
						TiXmlElement* shader_list_entry_element = shader_list_element->FirstChildElement("shader");
						const char* effect_shader_instance_id = effect_shader_instance_element->GetText();

						do
						{
							if(!shader_list_entry_element || !effect_shader_instance_id)
								break;

							const char* id = shader_list_entry_element->Attribute("id");
							// if the text matches, set the index and break out
							if(id && (strcmp(effect_shader_instance_id, id) == 0))
							{
								indices[shader_count] = index;
								shader_count++;
								break;
							}
							// ... otherwise move on to the next shader in the main shader list
							index++;
						}
						while(shader_list_entry_element = shader_list_entry_element->NextSiblingElement("shader"));

						// if a shader with a matching filename wasnt found, then disable this effect
						// by setting shader_count to 0
						if(!shader_list_entry_element)
						{
							shader_count = 0;
							break;
						}
					}
					while(effect_shader_instance_element = effect_shader_instance_element->NextSiblingElement("shader"));
				}

				s_shader_postprocess_external_effect* effect = new s_shader_postprocess_external_effect();
				memset(effect, 0, sizeof(s_shader_postprocess_external_effect));

				if(name != NULL)
					strcpy_s(effect->name, Enums::k_tag_string_length, name);

				effect->render_stage = CAST(Enums::postprocess_render_stage, render_stage);

				effect->quad_tesselation.x = tessellation_x;
				effect->quad_tesselation.y = tessellation_y;

				// these options are not available to external shaders so default them to false
				effect->effect_activation_flags.not_initially_active_bit = flags.not_initially_active_bit;
				effect->effect_activation_flags.not_active_during_cutscenes_bit = flags.not_active_during_cutscenes_bit;
				effect->effect_activation_flags.disabled_when_zoomed_bit = flags.disabled_when_zoomed_bit;
				effect->effect_activation_flags.disabled_when_using_a_vehicle_bit = flags.disabled_when_using_a_vehicle_bit;

				// if the effects shader index count is 1 or more, set up the shader_indices block data in the fake tag 
				if(shader_count > 0)
				{
					// allocate the memory for the shader index elements
					effect->shader_indices.Address = new TagGroups::s_shader_postprocess_collection_shader_index[shader_count];
					memset(effect->shader_indices.Address, 0, sizeof(TagGroups::s_shader_postprocess_collection_shader_index) * shader_count);

					// iterate through the loaded indices, setting up the elements of the shader_indices block
					effect->shader_indices.Count = shader_count;
					for(int32 j = 0; j < effect->shader_indices.Count; j++)
						effect->shader_indices[j].shader_index = indices[j];
				}			

				// create a new effect interface class, then sets its source to the fake tag that was just created						
				Globals().m_effect_array[Globals().m_effect_count] = new c_external_effect();
				Globals().m_effect_array[Globals().m_effect_count]->Ctor();
				Globals().m_effect_array[Globals().m_effect_count]->SetSource(effect);
				Globals().m_effect_array[Globals().m_effect_count]->SetupEffect();

				Globals().m_effect_count++;				
			}
			while((effect_element = effect_element->NextSiblingElement("effect")) &&
				(Globals().m_effect_count < k_max_effect_count));
		}		
		void		c_external_subsystem::ResetRenderBlock()
		{
			// loop through all the render blocks and clear out all the effect pointers
			for(int32 i = 0; i < NUMBEROF(Globals().m_render_blocks); i++)
				Globals().m_render_blocks[i].ClearList();
		}
		void		c_external_subsystem::ValidateEffects()
		{
			// loop through all of the loaded shaders and validate them to make sure there are no problems
			// then add a pointer to the effect to the relevant render block
			for(int32 i = 0; i < Globals().m_effect_count; ++i)
				if(Globals().m_effect_array[i]->ValidateEffect())
					Globals().m_render_blocks[Globals().m_effect_array[i]->m_effect_external->render_stage].AddEffect(Globals().m_effect_array[i]);
		}		
		void		c_external_subsystem::UpdateImpl(real delta_time)
		{
			// if there are no external effects then theres no point in going on
			if(Globals().m_effect_count == 0) return;

			// update time reliant code in the effects
			for(int32 i = 0; i < Globals().m_effect_count; i++)
				Globals().m_effect_array[i]->Update(delta_time);
		}
		/////////////////////////////////////////////////////////////////////

	}; }; };
};
#endif