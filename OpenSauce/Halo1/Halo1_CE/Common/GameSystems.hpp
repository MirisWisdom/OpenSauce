/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
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
			proc_update					Update;
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
#endif

		void InsertHooks();
	};
};