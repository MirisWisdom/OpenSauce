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

#include <Blam/Halo1/shader_postprocess_definitions.hpp>
#include "Common/YeloSettings.hpp"

namespace Yelo
{
	namespace Main
	{
		struct s_project_component
		{
			// Called from Main on startup
			proc_initialize				Initialize;
			// Called from Main on shutdown
			proc_dispose				Dispose;

			// Called from Yelo::GameState on init for new game state
			proc_initialize				InitializeForNewGameState;

			// Called from Yelo::GameState on init for a new map
			proc_initialize_for_new_map	InitializeForNewMap;
			// Called from Yelo::GameState on disposing from an old map
			proc_dispose_from_old_map	DisposeFromOldMap;

			// Called from Yelo::GameState on each game tick
			void						(*Update)(real delta_time);
		};

		//	[out_components] - Returns a list of components, or NULL if no components exist
		//	returns: Number of components in [out_components] minus 1, or just 0 if no components exist
		int32 GetProjectComponents(s_project_component*& out_components);


#if PLATFORM_IS_USER
		struct s_dx_component
		{
			// Called from [Yelo_]IDirect3D9::CreateDevice
			void (*Initialize)(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params);
			// Called from [Yelo_]IDirect3DDevice9::Reset
			void (*OnLostDevice)();
			// Called from [Yelo_]IDirect3DDevice9::Reset
			void (*OnResetDevice)(D3DPRESENT_PARAMETERS* params);
			// Called from [Yelo_]IDirect3DDevice9::EndScene
			void (*Render)();
			// Called from Rasterizer::Hook_RasterizerDispose
			void (*Release)();
		};

		//	[out_components] - Returns a list of components, or NULL if no components exist
		//	returns: Number of components in [out_components] minus 1, or just or 0 if no components exist
		int32 GetDXComponents(s_dx_component*& out_components);


		struct s_postprocessing_subsystem_component
		{
			void		(*Initialize)(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters);	
			void		(*OnLostDevice)();
			void		(*OnResetDevice)(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pParameters);
			void		(*Render)();

			HRESULT		(*AllocateResources)(IDirect3DDevice9* pDevice);
			void		(*ReleaseResources)();

			Settings::proc_settings_read LoadSettings;
			Settings::proc_settings_save SaveSettings;
			void		(*SetDefaultSettings)();

			void		(*Load)();
			void		(*Unload)();

			void		(*InitializeForNewMap)();
			void		(*DisposeFromOldMap)();
			void		(*Update)(real delta_time);
			// return true if any effects were applied
			bool		(*DoPostProcesses)(IDirect3DDevice9* pDevice, real frame_time, Enums::postprocess_render_stage render_point);
		};

		//	[out_components] - Returns a list of components, or NULL if no components exist
		//	returns: Number of components in [out_components] minus 1, or just or 0 if no components exist
		int32 GetPostprocessingSubsystemComponents(s_postprocessing_subsystem_component*& out_components);
#endif
	};
};