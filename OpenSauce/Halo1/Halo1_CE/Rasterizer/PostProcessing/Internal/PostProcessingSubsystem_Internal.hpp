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
#include "Rasterizer/PostProcessing/PostProcessingRenderBlock.hpp"
#include "Rasterizer/PostProcessing/Internal/PostProcessingInternalShader.hpp"
#include "Rasterizer/PostProcessing/Internal/PostProcessingInternalEffect.hpp"

namespace Yelo
{
	namespace Postprocessing { namespace Subsystem { namespace Internal {

		/////////////////////////////////////////////////////////////////////
		// Global runtime values
		/////////////////////////////////////////////////////////////////////		
		struct s_internal_subsystem_globals 
		{
			static s_internal_subsystem_globals g_instance;

			TagGroups::s_shader_postprocess_collection* m_shader_collection;
			c_internal_shader*			m_shader_array[k_max_shader_count];
			uint32						m_shader_count;
			c_internal_effect*			m_effect_array[k_max_effect_count];
			uint32						m_effect_count;

			c_postprocess_render_block	m_render_blocks[Enums::_postprocess_render_stage];		
		};
		s_internal_subsystem_globals& Globals();

		/////////////////////////////////////////////////////////////////////
		// Subsystem that allows tag based shaders to be used
		/////////////////////////////////////////////////////////////////////
		class c_internal_subsystem
		{
		public:
			static bool g_subsystem_enabled;
			static bool g_subsystem_loaded;
			static c_internal_subsystem g_instance;

			static void			Initialize(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters);
			static void			OnLostDevice();
			static void			OnResetDevice(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters);
			static void			Render()	{}
			
			static HRESULT		AllocateResources(IDirect3DDevice9* pDevice) { return S_OK; }
			static void			ReleaseResources();

			static void			LoadSettings(TiXmlElement* pp_element);
			static void			SaveSettings(TiXmlElement* pp_element);
			static void			SetDefaultSettings();

			static void			Load();
			static void			Unload();

			static void			InitializeForNewMap();
			static void			DisposeFromOldMap();
			static void			Update(real delta_time);
			static void			DoPostProcesses(IDirect3DDevice9* pDevice, real frame_time, Enums::postprocess_render_stage render_point);

		private:
			void		InitializeImpl(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters);
			void		OnLostDeviceImpl();
			void		OnResetDeviceImpl(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters);

			void		LoadSettingsImpl(TiXmlElement* pp_element);
			void		SaveSettingsImpl(TiXmlElement* pp_element);
			void		SetDefaultSettingsImpl();

			HRESULT		AllocateResourcesImpl(IDirect3DDevice9* pDevice);
			void		ReleaseResourcesImpl();

			void		UpdateImpl(real DeltaTime);
			void		DoPostProcessesImpl(IDirect3DDevice9* pDevice, real frame_time, Enums::postprocess_render_stage render_point);
			
			void		UnloadShaders();
			void		LoadShaders();
			void		ResetRenderBlock();
			void		ValidateEffects();
			void		Update();

		public:
			bool		SetEffectActive(uint16 index, bool enabled);
			bool		SetEffectFade(uint16 index, real start, real end, real time);
			int16		GetEffectIndexByName(cstring test_string);
			real		GetEffectCurrentFade(uint16 index);
			bool		GetEffectIsValid(uint16 index);
			int16		GetEffectShaderVariableIndexByName(uint16 effect_index, cstring name);
			void		SetEffectShaderVariableBoolean(uint16 effect_index, uint16 variable_index, bool value1);
			void		SetEffectShaderVariableInteger(uint16 effect_index, uint16 variable_index, uint32 value1);
			void		SetEffectShaderVariableReal(uint16 effect_index, uint16 variable_index, real value1, real value2, real value3, real value4);
		};
	}; }; };
};
#endif