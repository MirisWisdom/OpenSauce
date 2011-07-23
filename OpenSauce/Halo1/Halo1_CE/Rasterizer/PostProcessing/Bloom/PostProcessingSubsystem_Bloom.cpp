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
#include "Rasterizer/PostProcessing/Bloom/PostProcessingSubsystem_Bloom.hpp"
#if !PLATFORM_IS_DEDI

namespace PP = Yelo::Postprocessing;

namespace Yelo
{
	namespace Postprocessing { namespace Subsystem { namespace Bloom {		

		static s_shader_postprocess_bloom							g_bloom_shader;
		static s_shader_postprocess_bloom_effect					g_bloom_effect;
		static TagGroups::s_shader_postprocess_globals_bloom		g_bloom_defaults;
		static c_shader_instance_node*								g_bloom_shader_instance;

		/////////////////////////////////////////////////////////////////////
		// c_bloom_subsystem
		c_bloom_subsystem											c_bloom_subsystem::g_instance;

		bool														c_bloom_subsystem::g_subsystem_enabled;
		bool														c_bloom_subsystem::g_subsystem_loaded;
		c_bloom_shader												c_bloom_subsystem::g_shader;
		c_bloom_effect												c_bloom_subsystem::g_effect;

		void			c_bloom_subsystem::Initialize(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters)
		{
			c_bloom_subsystem::g_instance.InitializeImpl(pDevice, pParameters);
		}	
		void			c_bloom_subsystem::OnLostDevice()
		{
			c_bloom_subsystem::g_instance.OnLostDeviceImpl();
		}
		void			c_bloom_subsystem::OnResetDevice(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters)
		{
			c_bloom_subsystem::g_instance.OnResetDeviceImpl(pDevice, pParameters);
		}

		HRESULT			c_bloom_subsystem::AllocateResources(IDirect3DDevice9* pDevice)
		{
			return c_bloom_subsystem::g_instance.AllocateResourcesImpl(pDevice);
		}
		void			c_bloom_subsystem::ReleaseResources()
		{
			c_bloom_subsystem::g_instance.ReleaseResourcesImpl();
		}

		void			c_bloom_subsystem::LoadSettings(TiXmlElement* pp_element)
		{
			c_bloom_subsystem::g_instance.LoadSettingsImpl(pp_element);
		}
		void			c_bloom_subsystem::SaveSettings(TiXmlElement* pp_element)
		{
			c_bloom_subsystem::g_instance.SaveSettingsImpl(pp_element);
		}
		void			c_bloom_subsystem::SetDefaultSettings()
		{
			c_bloom_subsystem::g_instance.SetDefaultSettingsImpl();
		}

		void			c_bloom_subsystem::Load()
		{
			if(!g_subsystem_loaded)
				c_bloom_subsystem::g_instance.AllocateResourcesImpl(DX9::Direct3DDevice());
		}
		void			c_bloom_subsystem::Unload()
		{
			if(g_subsystem_loaded)
				c_bloom_subsystem::g_instance.ReleaseResourcesImpl();
		}

		void			c_bloom_subsystem::InitializeForNewMap() 
		{
			c_bloom_subsystem::g_instance.InitializeForNewMapImpl();
		}
		void			c_bloom_subsystem::DisposeFromOldMap() 
		{
			c_bloom_subsystem::g_instance.DisposeFromOldMapImpl();
		}
		bool			c_bloom_subsystem::DoPostProcesses(IDirect3DDevice9* pDevice, real frame_time, Enums::postprocess_render_stage render_point)
		{
			return c_bloom_subsystem::g_instance.DoPostProcessesImpl(pDevice, frame_time, render_point);
		}


		void			c_bloom_subsystem::InitializeImpl(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters)
		{			
			// initialize shader
			g_shader.Ctor();
			g_shader.SetSource(&g_bloom_shader);
			g_shader.SetupShader();

			// initialize effect
			g_bloom_effect.quad_tesselation.x = 5;
			g_bloom_effect.quad_tesselation.y = 5;

			g_effect.Ctor();
			g_effect.SetSource(&g_bloom_effect);
			g_effect.SetupEffect();

			g_shader.Initialize(pDevice);

			// set defaults
			g_bloom_defaults.flags.is_enabled_bit = true;
			g_bloom_defaults.flags.apply_after_hud_bit = true;

			g_bloom_defaults.size = 3.5f;
			g_bloom_defaults.exposure = 1.0f;
			g_bloom_defaults.mix_amount = 0.5f;

			g_bloom_defaults.minimum_color.red = g_bloom_defaults.minimum_color.green = g_bloom_defaults.minimum_color.blue = 0.55f;
			g_bloom_defaults.maximum_color.red = g_bloom_defaults.maximum_color.green = g_bloom_defaults.maximum_color.blue = 1.0f;
		}	
		void			c_bloom_subsystem::OnLostDeviceImpl()
		{
			g_shader.OnLostDevice();
		}
		void			c_bloom_subsystem::OnResetDeviceImpl(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters)
		{
			g_shader.OnResetDevice(pDevice, pParameters);
		}
		HRESULT			c_bloom_subsystem::AllocateResourcesImpl(IDirect3DDevice9* pDevice)
		{
			g_effect.ResetEffect();
			g_effect.SetSource(&g_bloom_effect);
			g_effect.SetupEffect();

			g_effect.AllocateResources(pDevice);
			HRESULT hr = g_shader.AllocateResources(pDevice);

			if(SUCCEEDED(hr))
			{
				g_shader.SetBloomValues(&g_bloom_defaults);

				g_bloom_shader_instance = new c_shader_instance_node();
				g_bloom_shader_instance->Ctor();
				g_bloom_shader_instance->SetShaderSource(&g_shader);

				g_effect.AddProcess(g_bloom_shader_instance);
			}
			g_subsystem_loaded = g_effect.ValidateEffect() && PP::Globals().m_flags.loaded;

			InitializeForNewMapImpl();

			return hr;
		}
		void			c_bloom_subsystem::ReleaseResourcesImpl()
		{
			g_effect.ReleaseResources();
			g_effect.Dtor();
			g_shader.ReleaseResources();
			g_shader.Dtor();
			g_subsystem_loaded = false;
		}

		void			c_bloom_subsystem::SetDefaultSettingsImpl()
		{
			g_subsystem_enabled = true;
		}

		void			c_bloom_subsystem::InitializeForNewMapImpl()
		{
			if(!g_subsystem_loaded)
				return;

			// get the maps bloom settings from the map an set them
			if(PP::Globals().m_map_postprocess_globals && PP::Globals().m_map_postprocess_globals->bloom_globals.Count == 1)
			{
				g_subsystem_enabled = PP::Globals().m_map_postprocess_globals->bloom_globals[0].flags.is_enabled_bit;
				g_shader.SetBloomValues(&PP::Globals().m_map_postprocess_globals->bloom_globals[0]);
			}
		}
		void			c_bloom_subsystem::DisposeFromOldMapImpl()
		{
			// set the bloom values to their defaults
			g_subsystem_enabled = g_bloom_defaults.flags.is_enabled_bit;
			g_shader.SetBloomValues(&g_bloom_defaults);
		}
		bool			c_bloom_subsystem::DoPostProcessesImpl(IDirect3DDevice9* pDevice, double frame_time, 
			Enums::postprocess_render_stage render_point)
		{
			// Apply the bloom effect at the intended render stage
			if (!g_subsystem_enabled) 
				return false;

			if((!g_shader.m_bloom_globals->flags.apply_after_hud_bit && (render_point == Enums::_postprocess_render_stage_pre_hud)) ||
				(g_shader.m_bloom_globals->flags.apply_after_hud_bit && (render_point == Enums::_postprocess_render_stage_pre_ui)))
				return SUCCEEDED(g_effect.DoPostProcessEffect(pDevice, frame_time));
			return false;
		}
		/////////////////////////////////////////////////////////////////////
	}; }; };
};
#endif