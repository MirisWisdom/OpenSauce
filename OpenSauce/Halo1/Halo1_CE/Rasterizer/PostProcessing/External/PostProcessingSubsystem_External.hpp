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
#include "Rasterizer/PostProcessing/PostProcessing.hpp"
#include "Rasterizer/PostProcessing/PostProcessingEffect.hpp"
#include "Rasterizer/PostProcessing/PostProcessingShaderGenericBase.hpp"
#include "Rasterizer/PostProcessing/PostProcessingRenderBlock.hpp"
#include "Rasterizer/PostProcessing/External/PostProcessingExternalShader.hpp"
#include "Rasterizer/PostProcessing/External/PostProcessingExternalEffect.hpp"

namespace Yelo
{
	namespace Postprocessing { namespace Subsystem { namespace External {
						
		/////////////////////////////////////////////////////////////////////
		// Global runtime values
		/////////////////////////////////////////////////////////////////////
		struct s_external_subsystem_globals
		{
			static s_external_subsystem_globals g_instance;

			c_external_shader*			m_shader_array[k_max_shader_count];
			int32						m_shader_count;
			c_external_effect*			m_effect_array[k_max_effect_count];
			int32						m_effect_count;
			c_postprocess_render_block	m_render_blocks[Enums::_postprocess_render_stage];
		};
		s_external_subsystem_globals& Globals();

		/////////////////////////////////////////////////////////////////////
		// Subsystem that allows non tag based shaders to be used
		/////////////////////////////////////////////////////////////////////
		class c_external_subsystem
		{
			static c_external_subsystem g_instance;

		public:
			static cstring g_subsystem_shader_settings_name;
			static char g_subsystem_shaders_path[MAX_PATH];
			static char g_subsystem_shader_textures_path[MAX_PATH];
			static bool g_subsystem_enabled;
			static bool g_subsystem_loaded;

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

			static void			InitializeForNewMap()	{}
			static void			DisposeFromOldMap()		{}
			static void			Update(real delta_time);
			static void			DoPostProcesses(IDirect3DDevice9* pDevice, real frame_time, Enums::postprocess_render_stage render_point);


		private:
			void				InitializeImpl(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters);
			void				OnLostDeviceImpl();
			void				OnResetDeviceImpl(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters);

			HRESULT				AllocateResourcesImpl(IDirect3DDevice9* pDevice);
			void				ReleaseResourcesImpl();

			void				LoadSettingsImpl(TiXmlElement* pp_element);
			void				SaveSettingsImpl(TiXmlElement* pp_element);
			void				SetDefaultSettingsImpl();

			void				DoPostProcessesImpl(IDirect3DDevice9* pDevice, real frame_time, Enums::postprocess_render_stage render_point);

			void				LoadShaders();
			void				ResetRenderBlock();
			void				ValidateEffects();
			void				UpdateImpl(real delta_time);
		};

	}; }; };
};
#endif