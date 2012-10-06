/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		// Only used for documentation purposes since we now have project component definitions spread across multiple .inl's
		// TODO: Document more 'hard' orderings, in that X system MUST come before Y.
		// These orderings should be followed in the respective .inl files
		// TODO: Is there anyway to use template magic to get around the need for .inl barf?
		// Guess we COULD make it a hard requirement for any system which defines one component type to also define functions for ALL parts (eg, init, dispose, and update)
		enum project_component {
			// This must come first, many systems use the Report filing system
			_project_component_settings,
			_project_component_file_io,
			// ...like debugging for example uses the report filing system
#ifdef API_DEBUG
			_project_component_debug_file,
#endif
			// compile in release too to get dumps from users
			_project_component_debug_dump,

			_project_component_game_state,
			_project_component_game_build_number,
			_project_component_cache,
			_project_component_data_files,
			_project_component_tag_groups,

			_project_component_scripting,				// No active code in Update game hook currently
			_project_component_game_state_runtime_data,
			_project_component_scenario_faux_zones,

#if PLATFORM_IS_USER
			_project_component_rasterizer_device_hooks,
			_project_component_dx9,
			_project_component_dx9_gbuffer,
			_project_component_rasterizer,
			_project_component_rasterizer_shader_draw,
			_project_component_rasterizer_shader_extension,
			_project_component_rasterizer_postprocessing,

			_project_component_camera,
#endif
			_project_component_console,					// No active code currently

			_project_component_game_engine,
			_project_component_networking,
			_project_component_players,					// No active code in Update game hook currently
			_project_component_effects,					// No active code currently
			_project_component_ai,						// No active code currently
			_project_component_objects,					// No active code in Update game hook currently

#if PLATFORM_IS_USER
			_project_component_game_ui,
			_project_component_input,					// No active code currently
			_project_component_keystone,
#endif

#if PLATFORM_IS_DEDI
			_project_component_server,
			_project_component_networking_http_server,
			_project_component_networking_http_server_map_download,
#endif
			_project_component_networking_http_client,
			_project_component_networking_version_check,

			_project_component_memory_function_interface_system,

			//_project_component_,
		};

		enum project_game_state_component_life_cycle {
			_project_game_state_component_life_cycle_before_save,
			_project_game_state_component_life_cycle_before_load,
			_project_game_state_component_life_cycle_after_load,

			k_number_of_project_game_state_component_life_cycles
		};
	};

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
#endif
	};
};