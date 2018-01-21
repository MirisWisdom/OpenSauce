/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Game/GameState.hpp"

#include <blamlib/cache/physical_memory_map_structures.hpp>
#include <blamlib/game/game_allegiance_structures.hpp>
#include <blamlib/game/game_globals_definitions.hpp>
#include <blamlib/game/game_globals_structures.hpp>
#include <blamlib/game/game_time_structures.hpp>
#include <blamlib/main/console.hpp>
#include <blamlib/main/main_structures.hpp>
#include <blamlib/saved_games/game_state_structures.hpp>
#include <blamlib/scenario/scenario.hpp>

#include <Common/DebugDump.hpp>
#include <Common/GameSystems.hpp>
#include <Game/AI.hpp>
#include <Game/Effects.hpp>
#include <Game/EngineFunctions.hpp>

#include <Game/GameBuildNumber.hpp>
#include <Game/GameStateRuntimeData.hpp>
#include <Game/ScriptLibrary.hpp>
#include <Memory/MemoryInterface.hpp>
#include <Networking/GameSpyApi.hpp>
#include <Networking/HTTP/c_file_downloader.hpp>
#include <Networking/VersionCheck.hpp>
#include <Objects/Objects.hpp>
#include <Objects/Units.hpp>
#include <Rasterizer/GBuffer.hpp>
#include <Rasterizer/PostProcessing/PostProcessing.hpp>
#include <Rasterizer/Sky.hpp>
#include <Scenario/StructureBSP.hpp>
#include <Settings/Settings.hpp>
#include <TagGroups/TagGroups.hpp>
#include <yelolib/cseries/random.hpp>
#include <yelolib/open_sauce/blam_memory_upgrades.hpp>
#include <yelolib/open_sauce/project_yellow_scenario.hpp>
#include <yelolib/open_sauce/project_yellow_scenario_definitions.hpp>
#include <yelolib/shell/shell_windows_command_line.hpp>
#include <Networking/HTTP/HTTPServer.hpp>
#include <Memory/1.10/Game.GameState.hpp>

namespace Yelo
{
	namespace GameState
	{
		void DataArrayInfoDumpToConsole(cstring);

#include "Game/GameState.EventLog.inl"
#include "Game/GameState.Scripting.inl"

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


		TagGroups::s_game_globals* GlobalGameGlobals()					PTR_IMP_GET(global_game_globals);
		TagGroups::s_game_globals** GlobalGameGlobalsReference()		PTR_IMP_GET2(global_game_globals);
		s_physics_globals* Physics()									PTR_IMP_GET2(global_physics);
		s_point_physics_globals* PointPhysics()							PTR_IMP_GET2(global_point_physics);


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
		static bool PLATFORM_API InitializeForNewBSPHook()
		{
			static int16 bsp_index = 0;

			_asm
			{
				mov		bsp_index, si
			};

			bool return_value = blam::scenario_switch_structure_bsp((int16)bsp_index);

			InitializeForNewBSP();

			return return_value;
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
			strcpy_s(GET_PTR2(init_txt_filename), 12, "initc.txt"); // 10 characters (including '\0')
#endif
		}
		void Initialize()
		{
			InitializeProcs();
			MemoryUpgradesInitialize();
			if(!CMDLINE_GET_PARAM(no_os_gamestate).ParameterSet())
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

			for(auto& call : K_GAME_SCENARIO_SWITCH_BSP_CALLS)
				Memory::WriteRelativeCall(InitializeForNewBSPHook, call, true);

			Memory::CreateHookRelativeCall(&DisposeFromOldBSP, GET_FUNC_VPTR(GAME_DISPOSE_FROM_OLD_BSP_HOOK), Enums::_x86_opcode_ret);

#if !PLATFORM_IS_DEDI
			ServerListInitialize();
#endif
		}

		void Dispose()
		{
			EventLogDispose();
			MemoryUpgradesDispose();
		}

		void* GameStateMalloc(const bool k_update_allocation_crc, const size_t size_of)
		{
			s_game_state_globals* gsg = GameStateGlobals();

			byte* base_addr = CAST_PTR(byte*, gsg->base_address) + gsg->cpu_allocation_size;

			// Debug check that we don't allocate more memory than the game state has available
			YELO_ASSERT_DISPLAY((base_addr + size_of) <= blam::physical_memory_map_get_tag_cache_address(),
				"Bit off more game-state than the game could chew!");

			gsg->cpu_allocation_size += size_of;
			// If the allocation crc is updated, game states won't be loadable by stock games
			if (k_update_allocation_crc)
				Memory::CRC(gsg->header->allocation_crc, &size_of, sizeof(size_of));

			return base_addr;
		}

		void Update(real delta_time)
		{
			Debug::Update(delta_time);
#if PLATFORM_IS_USER
			DX9::c_gbuffer_system::Update(delta_time);
			Rasterizer::PostProcessing::Update(delta_time);
#endif
#if PLATFORM_IS_DEDI
			Networking::HTTP::Server::Update(delta_time);
#endif
			Networking::HTTP::Client::Update(delta_time);
			Networking::VersionCheck::Update(delta_time);
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

			Random::InitializeSeed(GameGlobals()->options.game_random_seed);

			Objects::Units::InitializeForNewMapPrologue();
		}
		static void InitializeForNewMapEpilogue()
		{
			// Update the gravity based on the scenario's yelo tag settings
			if(!Scenario::GetYelo()->IsNull())
			{
				real gravity_scale = Scenario::GetYelo()->physics.gravity_scale;
				if(gravity_scale > 0 && gravity_scale != 1.0f)
					Physics()->SetGravityScale(gravity_scale);
			}
		}
		void PLATFORM_API InitializeForNewMap()
		{
			InitializeForNewMapPrologue();

			BuildNumber::InitializeForNewMap();
			TagGroups::InitializeForNewMap();
			RuntimeData::InitializeForNewMap();
#if PLATFORM_IS_USER
			Rasterizer::PostProcessing::InitializeForNewMap();
			Render::Sky::InitializeForNewMap();
			StructureBSP::InitializeForNewMap();
#endif
			AI::InitializeForNewMap();
			Objects::InitializeForNewMap();
			Networking::VersionCheck::InitializeForNewMap();

			InitializeForNewMapEpilogue();
		}
		static void DisposeFromOldMapPrologue()
		{
		}
		void PLATFORM_API DisposeFromOldMap()
		{
			DisposeFromOldMapPrologue();

#if PLATFORM_IS_USER
			Rasterizer::PostProcessing::DisposeFromOldMap();
			Render::Sky::DisposeFromOldMap();
			StructureBSP::DisposeFromOldMap();
#endif
			AI::DisposeFromOldMap();
			Objects::DisposeFromOldMap();
		}

		void PLATFORM_API InitializeForNewBSP()
		{
#if PLATFORM_IS_USER
			StructureBSP::InitializeForNewBSP();
#endif
		}

		void PLATFORM_API DisposeFromOldBSP()
		{
#if PLATFORM_IS_USER
			StructureBSP::DisposeFromOldBSP();
#endif
		}

		void PLATFORM_API InitializeForNewGameState()
		{
			GameState::RuntimeData::InitializeForNewGameState();
#if PLATFORM_IS_USER
			StructureBSP::InitializeForNewGameState();
#endif
			AI::InitializeForNewGameState();
		}

		void InitializeForYeloGameState(bool enabled)
		{
			Objects::InitializeForYeloGameState(enabled);
		}
		static void HandleGameStateLifeCycle(Enums::game_state_life_cycle life_cycle)
		{
			YELO_ASSERT_DISPLAY( IN_RANGE_ENUM(life_cycle, Enums::k_number_of_game_state_life_cycles), "Unknown lifecycle");

#if PLATFORM_IS_USER
			StructureBSP::HandleGameStateLifeCycle(life_cycle);
#endif
			AI::HandleGameStateLifeCycle(life_cycle);
		}
		void PLATFORM_API HandleBeforeSaveLifeCycle()
		{
			HandleGameStateLifeCycle(Enums::_game_state_life_cycle_before_save);
		}
		void PLATFORM_API HandleBeforeLoadLifeCycle()
		{
			HandleGameStateLifeCycle(Enums::_game_state_life_cycle_before_load);
		}
		void PLATFORM_API HandleAfterLoadLifeCycle()
		{
			HandleGameStateLifeCycle(Enums::_game_state_life_cycle_after_load);
		}

		static void DataArrayInfoDumpToConsole(cstring data_array_name)
		{
#if PLATFORM_IS_USER // no printing for dedis
			std::string name(data_array_name);
			const Memory::s_data_array* array = nullptr;

				 if (!name.compare("part"))			array = &Effects::Particles().Header;
			else if (!name.compare("partsys"))		array = &Effects::ParticleSystems().Header;
			else if (!name.compare("partsysparts"))	array = &Effects::ParticleSystemParticles().Header;
			else if (!name.compare("effe"))			array = &Effects::Effects().Header;
			else if (!name.compare("effelocs"))		array = &Effects::EffectLocations().Header;
			else if (!name.compare("cont"))			array = &Effects::Contrails().Header;
			else if (!name.compare("contpoints"))	array = &Effects::ContrailPoints().Header;
			else if (!name.compare("lightvol"))		array = &Objects::LightVolumes().Header;
			else if (!name.compare("obje"))			array = &Objects::ObjectHeader().Header;
			else return; // user gave an unrecognized name

			int active_count = array->NumberOfValidDatums();
			float active_percentage = CAST(float, active_count);
			active_percentage /= array->max_datum;
			active_percentage *= 100.f;
			blam::console_response_printf(false, "%3.2f full; %d / %d",
				active_percentage, active_count, array->max_datum);
#endif
		}

		void InitializeScripting()
		{
			Scripting::InitializeScriptFunction(Enums::_hs_function_physics_get_gravity, 
				scripting_physics_get_gravity_evaluate);
			Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_physics_set_gravity, 
				scripting_physics_set_gravity_evaluate);
			Scripting::InitializeScriptFunction(Enums::_hs_function_physics_constants_reset, 
				scripting_physics_constants_reset_evaluate);

			Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_data_array_info,
				scripting_data_array_info_evaluate);
		}
	};
};