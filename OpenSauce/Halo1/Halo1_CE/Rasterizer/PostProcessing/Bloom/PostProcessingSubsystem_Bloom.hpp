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
#pragma once

#if !PLATFORM_IS_DEDI
#include "Rasterizer/PostProcessing/Bloom/PostProcessingBloomEffect.hpp"
#include "Rasterizer/PostProcessing/Bloom/PostProcessingBloomShader.hpp"

namespace Yelo
{
	namespace Postprocessing { namespace Subsystem { namespace Bloom {		
		class c_bloom_subsystem
		{
			static c_bloom_subsystem		g_instance;

		public:
			static bool						g_subsystem_enabled;
			static bool						g_subsystem_loaded;
			static c_bloom_shader			g_shader;
			static c_bloom_effect			g_effect;

			static void			Initialize(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters);	
			static void			OnLostDevice();
			static void			OnResetDevice(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters);
			static void			Render()	{}

			static HRESULT		AllocateResources(IDirect3DDevice9* pDevice);
			static void			ReleaseResources();

			static void			LoadSettings(TiXmlElement* pp_element);
			static void			SaveSettings(TiXmlElement* pp_element);
			static void			SetDefaultSettings();

			static void			Load();
			static void			Unload();

			static void			InitializeForNewMap();
			static void			DisposeFromOldMap();
			static void			Update(real delta_time)	{}
			static void			DoPostProcesses(IDirect3DDevice9* pDevice, real frame_time, Enums::postprocess_render_stage render_point);


		private:
			void				InitializeImpl(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters);	
			void				OnLostDeviceImpl();
			void				OnResetDeviceImpl(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters);
			
			HRESULT				AllocateResourcesImpl(IDirect3DDevice9* pDevice);
			void				ReleaseResourcesImpl();

			void				LoadSettingsImpl(TiXmlElement* pp_element)			{}
			void				SaveSettingsImpl(TiXmlElement* pp_element)			{}
			void				SetDefaultSettingsImpl();

			void				InitializeForNewMapImpl();
			void				DisposeFromOldMapImpl();
			void				DoPostProcessesImpl(IDirect3DDevice9* pDevice, double frame_time, Enums::postprocess_render_stage render_stage);
		};
	}; }; };
};
#endif