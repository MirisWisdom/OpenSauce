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
#include "Rasterizer/PostProcessing/MotionBlur/PostProcessingSubsystem_MotionBlur.hpp"
#if !PLATFORM_IS_DEDI
#include "Common/YeloSettings.hpp"

namespace PP = Yelo::Postprocessing;

namespace Yelo
{
	namespace Postprocessing { namespace Subsystem { namespace MotionBlur {

		c_motionblur_subsystem c_motionblur_subsystem::g_instance;
		
		bool														c_motionblur_subsystem::g_subsystem_enabled;
		bool														c_motionblur_subsystem::g_subsystem_loaded;
		c_motionblur_shader											c_motionblur_subsystem::g_shader;
		c_motionblur_effect											c_motionblur_subsystem::g_effect;
		
		static s_shader_postprocess_motionblur						m_motionblur_shader;
		static TagGroups::s_shader_postprocess_effect_definition	m_motionblur_effect;

		void		c_motionblur_subsystem::Initialize(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters)
		{
			c_motionblur_subsystem::g_instance.InitializeImpl(pDevice, pParameters);
		}

		void		c_motionblur_subsystem::OnLostDevice()
		{
			c_motionblur_subsystem::g_instance.OnLostDeviceImpl();
		}

		void		c_motionblur_subsystem::OnResetDevice(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters)
		{
			c_motionblur_subsystem::g_instance.OnResetDeviceImpl(pDevice, pParameters);
		}

		HRESULT		c_motionblur_subsystem::AllocateResources(IDirect3DDevice9* pDevice)
		{
			return c_motionblur_subsystem::g_instance.AllocateResourcesImpl(pDevice);
		}

		void		c_motionblur_subsystem::ReleaseResources()
		{
			c_motionblur_subsystem::g_instance.ReleaseResourcesImpl();
		}

		void		c_motionblur_subsystem::LoadSettings(TiXmlElement* pp_element)
		{
			return c_motionblur_subsystem::g_instance.LoadSettingsImpl(pp_element);
		}

		void		c_motionblur_subsystem::SaveSettings(TiXmlElement* pp_element)
		{
			c_motionblur_subsystem::g_instance.SaveSettingsImpl(pp_element);
		}

		void		c_motionblur_subsystem::SetDefaultSettings()
		{
			c_motionblur_subsystem::g_instance.SetDefaultSettingsImpl();
		}

		void		c_motionblur_subsystem::Load() 
		{
			if(!g_subsystem_loaded)
				c_motionblur_subsystem::g_instance.AllocateResourcesImpl(DX9::Direct3DDevice());
		}

		void		c_motionblur_subsystem::Unload() 
		{
			if(g_subsystem_loaded)
				c_motionblur_subsystem::g_instance.ReleaseResourcesImpl();
		}

		bool		c_motionblur_subsystem::DoMotionBlurProcess(IDirect3DDevice9* pDevice, double frame_time)
		{
			return c_motionblur_subsystem::g_instance.DoMotionBlurProcessImpl(pDevice, frame_time);
		}


		void		c_motionblur_subsystem::InitializeImpl(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters)
		{
			g_shader.Ctor();
			g_shader.SetSource(&m_motionblur_shader);
			g_shader.SetupShader();

			m_motionblur_effect.quad_tesselation.x = 5;
			m_motionblur_effect.quad_tesselation.y = 5;
			g_effect.Ctor();
			g_effect.SetSource(&m_motionblur_effect);
			g_effect.SetupEffect();

			g_shader.Initialize(pDevice);
		}

		void		c_motionblur_subsystem::OnLostDeviceImpl()
		{
			g_shader.OnLostDevice();
		}

		void		c_motionblur_subsystem::OnResetDeviceImpl(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters)
		{
			g_shader.OnResetDevice(pDevice, pParameters);
		}

		HRESULT		c_motionblur_subsystem::AllocateResourcesImpl(IDirect3DDevice9* pDevice)
		{
			HRESULT hr;
			g_effect.ResetEffect();
			g_effect.SetSource(&m_motionblur_effect);
			g_effect.SetupEffect();

			g_effect.AllocateResources(pDevice);
			hr = g_shader.AllocateResources(pDevice);

			if(SUCCEEDED(hr))
			{
				c_shader_instance_node*	shader_instance = new c_shader_instance_node();
				shader_instance->Ctor();
				shader_instance->SetShaderSource(&g_shader);

				g_effect.AddProcess(shader_instance);
			}
			g_effect.ValidateEffect();
			g_subsystem_loaded = SUCCEEDED(hr) && PP::Globals().m_flags.loaded;
			return hr;
		}

		void		c_motionblur_subsystem::ReleaseResourcesImpl()
		{
			g_effect.ReleaseResources();
			g_shader.ReleaseResources();
			g_subsystem_loaded = false;
		}

		void		c_motionblur_subsystem::LoadSettingsImpl(TiXmlElement* pp_element)
		{
			if(pp_element != NULL)
			{
				TiXmlElement* element = pp_element->FirstChildElement("MotionBlur");
				if(!element) return;

				g_subsystem_enabled = Settings::ParseBoolean( element->Attribute("enabled") );
				element->QueryFloatAttribute("amount", &g_shader.m_blur_amount);
			}
		}

		void		c_motionblur_subsystem::SaveSettingsImpl(TiXmlElement* pp_element)
		{
			TiXmlElement* element = NULL;

			element = new TiXmlElement("MotionBlur");
				pp_element->LinkEndChild(element);

			element->SetAttribute("enabled", Settings::BooleanToString(g_subsystem_enabled));
			element->SetDoubleAttribute("amount", g_shader.m_blur_amount);
		}

		void		c_motionblur_subsystem::SetDefaultSettingsImpl()
		{
			g_subsystem_enabled = true;
			g_shader.m_blur_amount = 1.0f;
		}

		void		c_motionblur_subsystem::Update(real DeltaTime)
		{
			g_shader.m_vignette.enabled = ActivationVariables().m_player_has_control && !ActivationVariables().m_local_player_in_vehicle;
			g_shader.Update(DeltaTime);
		}

		bool		c_motionblur_subsystem::DoMotionBlurProcessImpl(IDirect3DDevice9* pDevice, double frame_time)
		{
			HRESULT hr = S_OK;
			if (!g_subsystem_enabled) return false;

			hr = g_effect.DoPostProcessEffect(pDevice, frame_time);

			return SUCCEEDED(hr);
		}

	}; }; };
};
#endif