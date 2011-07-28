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
#include <Blam/Halo1/shader_postprocess_definitions.hpp>
#include "Rasterizer/PostProcessing/PostProcessingInterpLinear.hpp"

namespace PP = Yelo::Postprocessing;

namespace Yelo
{
	namespace Postprocessing { namespace Subsystem { namespace Bloom {		

		static s_shader_postprocess_bloom							g_bloom_shader;
		static s_shader_postprocess_bloom_effect					g_bloom_effect;
		static c_shader_instance_node*								g_bloom_shader_instance;
		
		static TagGroups::s_shader_postprocess_globals_bloom		g_bloom_defaults;
		static TagGroups::s_shader_postprocess_globals_bloom*		g_bloom_current_globals;

		static TagGroups::s_shader_postprocess_bloom_definition		g_bloom_values_current;
		static c_postprocess_interp_linear							g_bloom_size_interp;
		static c_postprocess_interp_linear							g_bloom_exposure_interp;
		static c_postprocess_interp_linear							g_bloom_mix_amount_interp;
		static c_postprocess_interp_linear							g_bloom_minimum_color_interp;
		static c_postprocess_interp_linear							g_bloom_maximum_color_interp;

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
		void			c_bloom_subsystem::Update(real delta_time)
		{
			c_bloom_subsystem::g_instance.UpdateImpl(delta_time);
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

			g_bloom_defaults.bloom.size = 3.5f;
			g_bloom_defaults.bloom.exposure = 1.0f;
			g_bloom_defaults.bloom.mix_amount = 0.5f;

			g_bloom_defaults.bloom.minimum_color.red = g_bloom_defaults.bloom.minimum_color.green = g_bloom_defaults.bloom.minimum_color.blue = 0.55f;
			g_bloom_defaults.bloom.maximum_color.red = g_bloom_defaults.bloom.maximum_color.green = g_bloom_defaults.bloom.maximum_color.blue = 1.0f;

			g_bloom_size_interp.SetType(Enums::_shader_variable_base_type_float, 1);
			g_bloom_exposure_interp.SetType(Enums::_shader_variable_base_type_float, 1);
			g_bloom_mix_amount_interp.SetType(Enums::_shader_variable_base_type_float, 1);
			g_bloom_minimum_color_interp.SetType(Enums::_shader_variable_base_type_float, 3);
			g_bloom_maximum_color_interp.SetType(Enums::_shader_variable_base_type_float, 3);

			g_bloom_current_globals = &g_bloom_defaults;
			g_bloom_values_current = g_bloom_defaults.bloom;

			g_shader.SetBloomValues(&g_bloom_values_current);
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
				g_shader.Update();

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
			g_bloom_defaults.flags.is_enabled_bit = true;
		}

		void			c_bloom_subsystem::InitializeForNewMapImpl()
		{
			if(!g_subsystem_loaded)
				return;

			// get the maps bloom settings from the map
			if(PP::Globals().m_map_postprocess_globals && PP::Globals().m_map_postprocess_globals->bloom_globals.Count == 1)
			{
				g_bloom_current_globals = &PP::Globals().m_map_postprocess_globals->bloom_globals[0];
				g_bloom_values_current = PP::Globals().m_map_postprocess_globals->bloom_globals[0].bloom;
			}

			SetupInterps();
		}
		void			c_bloom_subsystem::DisposeFromOldMapImpl()
		{
			// set the bloom values to their defaults
			g_bloom_current_globals = &g_bloom_defaults;
			g_bloom_values_current = g_bloom_defaults.bloom;
		}
		void			c_bloom_subsystem::UpdateImpl(real delta_time)
		{
			// update animated values
			g_bloom_size_interp.Update(delta_time);
			g_bloom_exposure_interp.Update(delta_time);
			g_bloom_mix_amount_interp.Update(delta_time);
			g_bloom_minimum_color_interp.Update(delta_time);
			g_bloom_maximum_color_interp.Update(delta_time);

			g_bloom_values_current.size = g_bloom_size_interp.GetCurrent().real32;
			g_bloom_values_current.exposure = g_bloom_exposure_interp.GetCurrent().real32;
			g_bloom_values_current.mix_amount = g_bloom_mix_amount_interp.GetCurrent().real32;
			g_bloom_values_current.minimum_color = g_bloom_minimum_color_interp.GetCurrent().color3d;
			g_bloom_values_current.maximum_color = g_bloom_maximum_color_interp.GetCurrent().color3d;
		}
		bool			c_bloom_subsystem::DoPostProcessesImpl(IDirect3DDevice9* pDevice, double frame_time, 
			Enums::postprocess_render_stage render_point)
		{
			// Apply the bloom effect at the intended render stage
			if (!g_bloom_current_globals->flags.is_enabled_bit)
				return false;

			if((!g_bloom_current_globals->flags.apply_after_hud_bit && (render_point == Enums::_postprocess_render_stage_pre_hud)) ||
				(g_bloom_current_globals->flags.apply_after_hud_bit && (render_point == Enums::_postprocess_render_stage_pre_ui)))
			{
				g_shader.Update();
				return SUCCEEDED(g_effect.DoPostProcessEffect(pDevice, frame_time));
			}
			return false;
		}
		void			c_bloom_subsystem::SetupInterps()
		{
			g_bloom_size_interp.GetStart().real32 = g_bloom_values_current.size;
			g_bloom_size_interp.Reset();

			g_bloom_exposure_interp.GetStart().real32 = g_bloom_values_current.exposure;
			g_bloom_exposure_interp.Reset();

			g_bloom_mix_amount_interp.GetStart().real32 = g_bloom_values_current.mix_amount;
			g_bloom_mix_amount_interp.Reset();

			g_bloom_minimum_color_interp.GetStart().color3d = g_bloom_values_current.minimum_color;
			g_bloom_minimum_color_interp.Reset();

			g_bloom_maximum_color_interp.GetStart().color3d = g_bloom_values_current.maximum_color;
			g_bloom_maximum_color_interp.Reset();
		}

		void			c_bloom_subsystem::SetBloomSize(real size, real interp_time)
		{
			g_bloom_size_interp.GetStart().real32 = g_bloom_size_interp.GetCurrent().real32;
			g_bloom_size_interp.GetEnd().real32 = size;
			g_bloom_size_interp.Begin(interp_time);
		}

		void			c_bloom_subsystem::SetBloomExposure(real exposure, real interp_time)
		{
			g_bloom_exposure_interp.GetStart().real32 = g_bloom_exposure_interp.GetCurrent().real32;
			g_bloom_exposure_interp.GetEnd().real32 = exposure;
			g_bloom_exposure_interp.Begin(interp_time);
		}

		void			c_bloom_subsystem::SetBloomMixAmount(real mix_amount, real interp_time)
		{
			g_bloom_mix_amount_interp.GetStart().real32 = g_bloom_mix_amount_interp.GetCurrent().real32;
			g_bloom_mix_amount_interp.GetEnd().real32 = mix_amount;
			g_bloom_mix_amount_interp.Begin(interp_time);
		}

		void			c_bloom_subsystem::SetBloomMinimumColor(real_rgb_color minimum_color, real interp_time)
		{
			g_bloom_minimum_color_interp.GetStart().color3d = g_bloom_minimum_color_interp.GetCurrent().color3d;
			g_bloom_minimum_color_interp.GetEnd().color3d = minimum_color;
			g_bloom_minimum_color_interp.Begin(interp_time);
		}

		void			c_bloom_subsystem::SetBloomMaximumColor(real_rgb_color maximum_color, real interp_time)
		{
			g_bloom_maximum_color_interp.GetStart().color3d = g_bloom_maximum_color_interp.GetCurrent().color3d;
			g_bloom_maximum_color_interp.GetEnd().color3d = maximum_color;
			g_bloom_maximum_color_interp.Begin(interp_time);
		}
		/////////////////////////////////////////////////////////////////////
	}; }; };
};
#endif