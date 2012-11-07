/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Game/GameState.hpp"

#include <YeloLib/Halo1/open_sauce/blam_memory_upgrades.hpp>

#include "Memory/MemoryInterface.hpp"
#include "Common/GameSystems.hpp"
#include "Common/YeloSettings.hpp"
#include "Common/CmdLineSettings.hpp"
#include "Game/ScriptLibrary.hpp"
#include "Game/EngineFunctions.hpp"
#include "Game/GameBuildNumber.hpp"
#include "Rasterizer/PostProcessing/PostProcessing.hpp"

#include "TagGroups/TagGroups.hpp"
#include "TagGroups/project_yellow_definitions.hpp"

namespace Yelo
{
	namespace GameState
	{
#include "Game/GameState.EventLog.inl"
#include "Game/GameState.Scripting.inl"


#define __EL_INCLUDE_ID			__EL_INCLUDE_GAME
#define __EL_INCLUDE_FILE_ID	__EL_GAME_GAME_STATE
#include "Memory/_EngineLayout.inl"

#include "Game/GameState.Procs.inl"
#include "Game/GameState.MemoryUpgrades.inl"
#if !PLATFORM_IS_DEDI
	#include "Game/GameState.ServerList.inl"
#endif

		s_main_globals* MainGlobals()								PTR_IMP_GET2(main_globals);
		s_physical_memory_map_globals* PhysicalMemoryMapGlobals()	PTR_IMP_GET2(physical_memory_globals);


		s_game_state_globals* GameStateGlobals()						PTR_IMP_GET2(game_state_globals);
		s_game_globals* GameGlobals()									DPTR_IMP_GET(game_globals);
		s_game_time_globals* GameTimeGlobals()							DPTR_IMP_GET(game_time_globals);
		s_game_allegiance_globals* GameAllegianceGlobals()				DPTR_IMP_GET(game_allegiance_globals);
		Game::s_scenario_globals* ScenarioGlobals()						DPTR_IMP_GET(scenario_globals);


		TagGroups::scenario* Scenario()									PTR_IMP_GET(global_scenario);
		TagGroups::s_game_globals* GlobalGameGlobals()					PTR_IMP_GET(global_game_globals);
		TagGroups::collision_bsp* Bsp3d()								PTR_IMP_GET(global_bsp3d);
		TagGroups::collision_bsp* CollisionBsp()						PTR_IMP_GET(global_collision_bsp);
		TagGroups::structure_bsp* StructureBsp()						PTR_IMP_GET(global_structure_bsp);
		s_physics_globals* Physics()									PTR_IMP_GET2(global_physics);
		s_point_physics_globals* PointPhysics()							PTR_IMP_GET2(global_point_physics);


		datum_index ScenarioIndex()										PTR_IMP_GET(global_scenario_index);
		int16 StructureBspIndex()										PTR_IMP_GET(structure_bsp_index);
		byte* DeveloperMode()											PTR_IMP_GET2(developer_mode);
		static bool* TransportDumping()									PTR_IMP_GET2(transport_dumping);
		bool DevmodeEnabled()											PTR_IMP_GET(devmode_enabled);


		static bool g_yelo_game_state_enabled;
		bool YeloGameStateEnabled()
		{
			return g_yelo_game_state_enabled;
		}
		static void InitializeForYeloGameState()
		{
			// TODO: change game_state_init build-numbers string code to reference ours
			// TODO: change game save code to use our build-number checks
		}

		API_FUNC_NAKED static void PLATFORM_API InitializeForNewGameStateHook()
		{
			__asm {
				pop		esi
				pop		ebx
				pop		ecx
				jmp		GameState::InitializeForNewGameState
			}
		}
		API_FUNC_NAKED static void PLATFORM_API InitializeForNewMapHook()
		{
			__asm {
				pop		ecx
				jmp		GameState::InitializeForNewMap
			}
		}

		static void InitializeForDebug()
		{
			*GameState::DeveloperMode() = Enums::k_developer_mode_level_debug_output; // make console messages appear

			// increment the game build by one so all games (hosted or browsed) aren't
			// from the normal, non-Yelo, game pool.
			BuildNumber::GameBuildString()[Enums::k_game_build_string_build_offset+1] += 7;
		}
		static void InitializeInitTextFix()
		{
			// FFFFFFFFuck you. This enables the client to have it's own init.txt (also was causing crashes)
			// Obviously this isn't ran in Dedi builds
#if PLATFORM_IS_USER
			strcpy_s(GET_PTR2(init_txt_filename), 12, "init_c.txt"); // 11 characters (including '\0')
#endif
		}
		void Initialize()
		{
			InitializeProcs();
			MemoryUpgradesInitialize();
			if(CMDLINE_GET_PARAM(use_os_gamestate).ParameterSet())
			{
				InitializeForYeloGameState(g_yelo_game_state_enabled = true);
			}
			else
				g_yelo_game_state_enabled = false;

#ifdef API_DEBUG
			InitializeForDebug();
#endif
			*TransportDumping() = false;

			InitializeInitTextFix();

			EventLogInitialize();

			Memory::WriteRelativeJmp(&InitializeForNewGameStateHook, GET_FUNC_VPTR(GAME_INITIALIZE_HOOK), true);
			Memory::WriteRelativeJmp(&InitializeForNewMapHook, GET_FUNC_VPTR(GAME_INITIALIZE_FOR_NEW_MAP_HOOK), true);
			Memory::CreateHookRelativeCall(&DisposeFromOldMap, GET_FUNC_VPTR(GAME_DISPOSE_FROM_OLD_MAP_HOOK), Enums::_x86_opcode_ret);

#if !PLATFORM_IS_DEDI
			ServerListInitialize();
#endif
		}

		void Dispose()
		{
			EventLogDispose();
			MemoryUpgradesDispose();
		}

		void Update(real delta_time)
		{
			Yelo::Main::s_project_component* components;
			const int32 component_count = Yelo::Main::GetProjectComponents(components);

			for(int32 x = 0; x <= component_count; x++)
				if( components[x].Update != NULL )
					components[x].Update(delta_time);
		}

		static void InitializeForNewMapPrologue()
		{
			Physics()->Reset(); // Reset the physics constants on each new map load since these are engine globals, not game state globals.

			s_yelo_header_data& yelo_header = GameStateGlobals()->header->yelo;
			const TagGroups::s_game_globals* game_globals = GameState::GlobalGameGlobals();
			if(!yelo_header.flags.initialized)
			{
				yelo_header.flags.initialized = true;
				yelo_header.unit_grenade_types_count = Enums::k_unit_grenade_types_count;
			}

			yelo_header.flags.game_state_upgrades_on = YeloGameStateEnabled();

			// check this map's grenade count vs the previous map's
			if(game_globals->grenades.Count != yelo_header.unit_grenade_types_count)
			{
				// inform the unit grenade counts code to run an assembly update
				yelo_header.flags.update_unit_grenade_types_count = true;
				// figure out the new grenade count
				// defaulting to the stock count if there is suspicious number
				if(game_globals->grenades.Count <= Enums::k_unit_grenade_types_count_yelo)
					yelo_header.unit_grenade_types_count = CAST(byte, game_globals->grenades.Count);
				else
					yelo_header.unit_grenade_types_count = Enums::k_unit_grenade_types_count;
			}
		}
		static void InitializeForNewMapEpilogue()
		{
			// Update the gravity based on the scenario's yelo tag settings
			if(!TagGroups::_global_yelo->IsNull())
			{
				const real& gravity_scale = TagGroups::_global_yelo->physics.gravity_scale;
				if(gravity_scale > 0 && gravity_scale != 1.0f)
					Physics()->SetGravityScale(gravity_scale);
			}
		}
		void PLATFORM_API InitializeForNewMap()
		{
			InitializeForNewMapPrologue();

			Yelo::Main::s_project_map_component* components;
			const int32 component_count = Yelo::Main::GetProjectComponents(components);

			for(int32 x = 0; x <= component_count; x++)
				if( components[x].InitializeForNewMap != NULL )
					components[x].InitializeForNewMap();

			InitializeForNewMapEpilogue();
		}
		static void DisposeFromOldMapPrologue()
		{
		}
		void PLATFORM_API DisposeFromOldMap()
		{
			DisposeFromOldMapPrologue();

			Yelo::Main::s_project_map_component* components;
			const int32 component_count = Yelo::Main::GetProjectComponents(components);

			for(int32 x = component_count; x >= 0; x--)
				if( components[x].DisposeFromOldMap != NULL )
					components[x].DisposeFromOldMap();
		}

		void PLATFORM_API InitializeForNewGameState()
		{
			Yelo::Main::s_project_game_state_component* components;
			const int32 component_count = Yelo::Main::GetProjectComponents(components);

			for(int32 x = 0; x <= component_count; x++)
				if( components[x].InitializeForNewGameState != NULL )
					components[x].InitializeForNewGameState();
		}
		void InitializeForYeloGameState(bool enabled)
		{
			Yelo::Main::s_project_game_state_component* components;
			const int32 component_count = Yelo::Main::GetProjectComponents(components);

			for(int32 x = 0; x <= component_count; x++)
				if( components[x].InitializeForYeloGameState != NULL )
					components[x].InitializeForYeloGameState(enabled);
		}
		static void HandleGameStateLifeCycle(Enums::project_game_state_component_life_cycle life_cycle)
		{
			ASSERT( IN_RANGE_ENUM(life_cycle, Enums::k_number_of_project_game_state_component_life_cycles), "What fucking life cycle is this shit?");

			Yelo::Main::s_project_game_state_component* components;
			const int32 component_count = Yelo::Main::GetProjectComponents(components);

			for(int32 x = 0; x <= component_count; x++)
				if( components[x].HandleGameStateLifeCycle != NULL )
					components[x].HandleGameStateLifeCycle(life_cycle);
		}
		void PLATFORM_API HandleBeforeSaveLifeCycle()
		{
			HandleGameStateLifeCycle(Enums::_project_game_state_component_life_cycle_before_save);
		}
		void PLATFORM_API HandleBeforeLoadLifeCycle()
		{
			HandleGameStateLifeCycle(Enums::_project_game_state_component_life_cycle_before_load);
		}
		void PLATFORM_API HandleAfterLoadLifeCycle()
		{
			HandleGameStateLifeCycle(Enums::_project_game_state_component_life_cycle_after_load);
		}

		void InitializeScripting()
		{
			Scripting::InitializeScriptFunction(Enums::_hs_function_physics_get_gravity, 
				scripting_physics_get_gravity_evaluate);
			Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_physics_set_gravity, 
				scripting_physics_set_gravity_evaluate);
			Scripting::InitializeScriptFunction(Enums::_hs_function_physics_constants_reset, 
				scripting_physics_constants_reset_evaluate);
		}
	};
};