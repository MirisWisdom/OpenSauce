/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

// TODO: move this into YeloLib, start using in CheApe

namespace Yelo
{
	// [enabled] - true when OS is initializing for OS-modified game states, false if we're shutting down said modifications
	typedef void (API_FUNC* proc_initialize_for_yelo_game_state)(bool enabled);
	// [game_state_life_cycle] - See: Enums::project_game_state_component_life_cycle
	typedef void (API_FUNC* proc_handle_game_state_life_cycle)(_enum game_state_life_cycle);
	namespace Main
	{
		struct s_project_component
		{
			// Called from Main on startup
			proc_initialize				Initialize;
			// Called from Main on shutdown
			proc_dispose				Dispose;

			// Called from Yelo::GameState on each game tick
			proc_update					Update;
		};

		struct s_project_map_component
		{
			// Called from Yelo::GameState on init for a new map
			proc_initialize_for_new_map	InitializeForNewMap;
			// Called from Yelo::GameState on disposing from an old map
			proc_dispose_from_old_map	DisposeFromOldMap;
		};

		struct s_project_bsp_component
		{
			// Called from Yelo::GameState on init after a bsp load
			proc_initialize_for_new_bsp	InitializeForNewBSP;
			// Called from Yelo::GameState on disposing after a bsp unload
			proc_dispose_from_old_bsp	DisposeFromOldBSP;
		};

		struct s_project_game_state_component
		{
			// Called from Yelo::GameState on init for new game state
			proc_initialize						InitializeForNewGameState;

			proc_initialize_for_yelo_game_state	InitializeForYeloGameState;

			proc_handle_game_state_life_cycle	HandleGameStateLifeCycle;
		};

#if PLATFORM_IS_USER
		struct s_dx_component
		{
			// Called from [Yelo_]IDirect3D9::CreateDevice
			void (API_FUNC* Initialize)(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params);
			// Called from [Yelo_]IDirect3DDevice9::Reset
			void (API_FUNC* OnLostDevice)();
			// Called from [Yelo_]IDirect3DDevice9::Reset
			void (API_FUNC* OnResetDevice)(D3DPRESENT_PARAMETERS* params);
			// Called from [Yelo_]IDirect3DDevice9::EndScene
			void (API_FUNC* Render)();
			// Called from Rasterizer::Hook_RasterizerDispose
			void (API_FUNC* Release)();
		};
#endif
	};
};