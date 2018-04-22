/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include <halo/halo.h>
#include "Game/GameState.hpp"

#include <Pointers/Game.GameState.hpp>
#include <blam/cache/physical_memory_map_structures.hpp>
#include <blam/game/game_allegiance_structures.hpp>
#include <blam/game/game_globals_definitions.hpp>
#include <blam/game/game_globals_structures.hpp>
#include <blam/game/game_time_structures.hpp>
#include <blam/main/console.hpp>
#include <blam/main/main_structures.hpp>
#include <blam/saved_games/game_state_structures.hpp>
#include <blam/scenario/scenario.hpp>
#include <Common/DebugDump.hpp>
#include <Game/AI.hpp>
#include <Game/Effects.hpp>
#include <Game/GameBuildNumber.hpp>
#include <Game/GameStateRuntimeData.hpp>
#include <Game/ScriptLibrary.hpp>
#include <Networking/GameSpyApi.hpp>
#include <Networking/HTTP/HTTPServer.hpp>
#include <Networking/VersionCheck.hpp>
#include <Objects/Objects.hpp>
#include <Objects/Units.hpp>
#include <Rasterizer/GBuffer.hpp>
#include <Rasterizer/PostProcessing/PostProcessing.hpp>
#include <Rasterizer/Sky.hpp>
#include <Scenario/StructureBSP.hpp>
#include <Settings/Settings.hpp>
#include <TagGroups/TagGroups.hpp>
#include <yelo/cseries/random.hpp>
#include <yelo/open_sauce/blam_memory_upgrades.hpp>
#include <yelo/open_sauce/project_yellow_scenario.hpp>
#include <yelo/open_sauce/project_yellow_scenario_definitions.hpp>
#include <yelo/shell/shell_windows_command_line.hpp>

namespace Yelo
{
	namespace GameState
	{
		void DataArrayInfoDumpToConsole(
			cstring);

		static struct s_event_log_globals
		{
			cstring filename;
			FILE* file_handle;
		} event_log_globals;

		static void EventLogInitialize()
		{
			event_log_globals.filename = Settings::K_EVENT_LOG_FILENAME;
			event_log_globals.file_handle = Settings::CreateReport(event_log_globals.filename, true, true);
		}

		static void EventLogDispose()
		{
			if (event_log_globals.file_handle != nullptr)
				fclose(event_log_globals.file_handle);
		}

		static void WriteEventFirstLine()
		{
			static const char format[] = "\n%s\n";

			tag_string time_str;
			GetTimeStampString(time_str);

			fprintf(event_log_globals.file_handle, format, time_str);
			fflush(event_log_globals.file_handle);
		}

		void WriteEvent(
			cstring str,
			bool write_time_stamp)
		{
			static const char format[] = "%s%s%c";

			if (event_log_globals.file_handle == nullptr)
				return;

			static bool first_line = true;
			if (first_line)
			{
				first_line = false;
				WriteEventFirstLine();
			}

			tag_string time_str;
			if (write_time_stamp)
				GetTimeStampString(time_str);

			char time_buffer[16];
			memset(time_buffer, 0, sizeof(time_buffer));

			if (write_time_stamp)
				strncpy(time_buffer, time_str + 11, 9); // copy the time only, leave out the date and year

			if (write_time_stamp)
				fprintf(event_log_globals.file_handle, format, time_buffer, str, '\n');
			else
				fprintf(event_log_globals.file_handle, format + 2, str, '\n');
			fflush(event_log_globals.file_handle);
		}

		static void* scripting_data_array_info_evaluate(
			void** arguments)
		{
			struct s_arguments
			{
				cstring data_array_name;
			}* args = CAST_PTR(s_arguments*, arguments);

			DataArrayInfoDumpToConsole(args->data_array_name);

			return nullptr;
		}

		//////////////////////////////////////////////////////////////////////////
		// physics constants

		static void* scripting_physics_get_gravity_evaluate()
		{
			TypeHolder result;
			result.pointer = nullptr;

			result.real = Physics()->gravity;

			return result.pointer;
		}

		static void* scripting_physics_set_gravity_evaluate(
			void** arguments)
		{
			struct s_arguments
			{
				real gravity_fraction;
			}* args = CAST_PTR(s_arguments*, arguments);

			Physics()->SetGravityScale(args->gravity_fraction);

			return nullptr;
		}

		static void* scripting_physics_constants_reset_evaluate()
		{
			Physics()->Reset();

			return nullptr;
		}


		typedef void (PLATFORM_API* game_state_proc)();

		// stock engine numbers
		enum
		{
			k_number_of_game_state_before_save_procs = 1,
			k_number_of_game_state_before_load_procs = 1,
			k_number_of_game_state_after_load_procs = 13,
		};

		//////////////////////////////////////////////////////////////////////////
		// before save
		static game_state_proc game_state_before_save_procs_yelo[] = {
			HandleBeforeSaveLifeCycle,
		};
		static game_state_proc game_state_before_save_procs_new[k_number_of_game_state_before_save_procs + NUMBEROF(game_state_before_save_procs_yelo)] = { };

		//////////////////////////////////////////////////////////////////////////
		// before load
		static game_state_proc game_state_before_load_procs_yelo[] = {
			HandleBeforeLoadLifeCycle,
		};
		static game_state_proc game_state_before_load_procs_new[k_number_of_game_state_before_load_procs + NUMBEROF(game_state_before_load_procs_yelo)] = { };

		//////////////////////////////////////////////////////////////////////////
		// after load
		static game_state_proc game_state_after_load_procs_yelo[] = {
			HandleAfterLoadLifeCycle,
		};
		static game_state_proc game_state_after_load_procs_new[k_number_of_game_state_after_load_procs + NUMBEROF(game_state_after_load_procs_yelo)] = { };

		//////////////////////////////////////////////////////////////////////////
		// impl
		static void PLATFORM_API game_state_call_before_save_procs_new()
		{
			for (auto proc : game_state_before_save_procs_new)
				proc();
		}

		static void PLATFORM_API game_state_call_before_load_procs_new()
		{
			for (auto proc : game_state_before_load_procs_new)
				proc();
		}

		//////////////////////////////////////////////////////////////////////////
		// initialization
		static void InitializeNewProcsArrays()
		{
			using namespace Enums;
			//////////////////////////////////////////////////////////////////////////
			// before save
			{
				// Get the one-and-only save proc from the first call address
				game_state_proc* before_save_proc = *CAST_PTR(game_state_proc**,
					K_GAME_STATE_BEFORE_SAVE_PROCS_CALLS[0] + sizeof(_x86_opcode_call_abs));

				// Copy the game's proc to the 'new' proc array
				game_state_before_save_procs_new[0] = *before_save_proc;
				// Copy our Yelo procs to the 'new' proc array
				for (int x = 0; x < NUMBEROF(game_state_before_save_procs_yelo); x++)
					game_state_before_save_procs_new[x + k_number_of_game_state_before_save_procs] =
						game_state_before_save_procs_yelo[x];
			}
			//////////////////////////////////////////////////////////////////////////
			// before load
			{
				// Get the one-and-only load proc from the first call address
				game_state_proc* before_load_proc = *CAST_PTR(game_state_proc**,
					K_GAME_STATE_BEFORE_LOAD_PROCS_CALLS[0] + sizeof(_x86_opcode_call_abs));

				// Copy the game's proc to the 'new' proc array
				game_state_before_load_procs_new[0] = *before_load_proc;
				// Copy our Yelo procs to the 'new' proc array
				for (int x = 0; x < NUMBEROF(game_state_before_load_procs_yelo); x++)
					game_state_before_load_procs_new[x + k_number_of_game_state_before_load_procs] =
						game_state_before_load_procs_yelo[x];
			}
			//////////////////////////////////////////////////////////////////////////
			// after load
			{
				// Get the address of the first element of the game's after load procs
				game_state_proc* after_load_procs = *CAST_PTR(game_state_proc**,
					K_GAME_STATE_AFTER_LOAD_PROCS_REFS[0]);

				// Copy the game's procs to the 'new' proc array
				for (int x = 0; x < k_number_of_game_state_after_load_procs; x++)
					game_state_after_load_procs_new[x] = after_load_procs[x];
				// Copy our Yelo procs to the 'new' proc array
				for (int x = 0; x < NUMBEROF(game_state_after_load_procs_yelo); x++)
					game_state_after_load_procs_new[x + k_number_of_game_state_after_load_procs] =
						game_state_after_load_procs_yelo[x];
			}
		}

		static void InitializeProcs()
		{
			InitializeNewProcsArrays();
			//////////////////////////////////////////////////////////////////////////
			// before save
			{
				for (auto ptr : K_GAME_STATE_BEFORE_SAVE_PROCS_CALLS)
					Memory::CreateHookRelativeCall(
						&game_state_call_before_save_procs_new,
						CAST_PTR(void*, ptr),
						Enums::_x86_opcode_nop);
			}
			//////////////////////////////////////////////////////////////////////////
			// before load
			{
				for (auto ptr : K_GAME_STATE_BEFORE_LOAD_PROCS_CALLS)
					Memory::CreateHookRelativeCall(
						&game_state_call_before_load_procs_new,
						CAST_PTR(void*, ptr),
						Enums::_x86_opcode_nop);
			}
			//////////////////////////////////////////////////////////////////////////
			// after load
			{
				STATIC_ASSERT(NUMBEROF(K_GAME_STATE_AFTER_LOAD_PROCS_REFS) == NUMBEROF(K_GAME_STATE_AFTER_LOAD_PROCS_COUNTS));

				for (int x = 0; x < NUMBEROF(K_GAME_STATE_AFTER_LOAD_PROCS_REFS); x++)
				{
					game_state_proc** procs_ref = CAST_PTR(game_state_proc**, K_GAME_STATE_AFTER_LOAD_PROCS_REFS[x]);
					size_t* count_ref = CAST_PTR(size_t*, K_GAME_STATE_AFTER_LOAD_PROCS_COUNTS[x]);

					*procs_ref = game_state_after_load_procs_new;
					*count_ref = NUMBEROF(game_state_after_load_procs_new);
				}
			}
		}

		// This memory upgrade design can only work with a DLL override system
		#if defined(DX_WRAPPER) || PLATFORM_IS_DEDI
		static void PhysicalMemoryReInitialize()
		{
			static const uintptr_t INITIALIZE = GET_FUNC_PTR(PHYSICAL_MEMORY_INITIALIZE);

			s_physical_memory_map_globals* globals = PhysicalMemoryMapGlobals();

			VirtualFree(globals->sound_cache_base_address, 0, MEM_RELEASE);
			VirtualFree(globals->texture_cache_base_address, 0, MEM_RELEASE);
			VirtualFree(globals->game_state_base_address, 0, MEM_RELEASE);

			__asm call INITIALIZE
		}

		static void MemoryUpgradesInitialize()
		{
			const uint32 k_allocation_size = Enums::k_physical_memory_map_allocation_size_upgrade;
			Memory::WriteMemory(GET_DATA_VPTR(PHYSICAL_MEMORY_ALLOCATION_SIZE), CAST_PTR(void*, k_allocation_size));

			PhysicalMemoryReInitialize();
		}

		static void MemoryUpgradesDispose() { }
		#else
		DOC_TODO_DEBUG("Game state memory upgrades not implemented...");
		static void MemoryUpgradesInitialize() {}
		static void MemoryUpgradesDispose() {}
		#endif


		#if !PLATFORM_IS_DEDI
		static cstring g_forced_mp_version = nullptr;

		static BOOL PLATFORM_API ServerVersionIsValid(cstring server_version)
		{
			if (g_forced_mp_version)
			{
				// compare with the forced server version
				if (strcmp(g_forced_mp_version, server_version) == 0)
					return true;
				return false;
			}

			// compare with the games version
			if (strcmp(BuildNumber::GameBuildString(), server_version) == 0)
				return true;

			// compare all known official versions
			return BuildNumber::StringIsValid(server_version);
		}

		static void SetVersionFromServer()
		{
			Networking::s_gamespy_server* server = Networking::GsServerBrowserGlobals()->selected_server.server;
			if (server)
			{
				const char* server_version = Networking::GameSpy::SBServerGetStringValue(server, "gamever", "");

				// use the current version if the queried server_version is invalid as far as we know
				if (server_version == nullptr || !BuildNumber::StringIsValid(server_version))
					server_version = BOOST_PP_STRINGIZE(PLATFORM_VERSION_VALUE);

				BuildNumber::ChangeAdvertisedVersion(server_version, false);
			}
		}

		static void SetVersionToCurrent()
		{
			BuildNumber::ChangeAdvertisedVersion(BOOST_PP_STRINGIZE(PLATFORM_VERSION_VALUE), false);
		}

		static void* CreateNetworkServerHook()
		{
			typedef void* (PLATFORM_API* create_network_game_t)();
			static const create_network_game_t network_game_server_create =
				CAST_PTR(create_network_game_t, GET_FUNC_VPTR(GAME_CREATE_NETWORK_SERVER));

			SetVersionToCurrent();

			return network_game_server_create();
		}

		static void* CreateNetworkClientHook()
		{
			typedef void* (PLATFORM_API* create_network_game_t)();
			static const create_network_game_t network_game_client_create =
				CAST_PTR(create_network_game_t, GET_FUNC_VPTR(GAME_CREATE_NETWORK_CLIENT));

			SetVersionFromServer();

			return network_game_client_create();
		}

		static void ServerListInitialize()
		{
			if (CMDLINE_GET_PARAM(mp_version).ParameterSet())
			{
				cstring mp_version = CMDLINE_GET_PARAM(mp_version).GetValue();
				if (mp_version && strcmp(mp_version, "disable") == 0)
					return;

				g_forced_mp_version = BuildNumber::ShortStringToBuildNumberString(mp_version);
			}

			// override the function call that omits servers of a different version from the server browser
			Memory::WriteRelativeCall(ServerVersionIsValid, GET_FUNC_VPTR(GAME_SERVER_QR2_STRING_MATCHES_GAMEVER_CALL), true);

			Memory::WriteRelativeCall(CreateNetworkServerHook, GET_FUNC_VPTR(GAME_CREATE_NETWORK_SERVER_CALL), true);
			for (auto ptr : K_GAME_CREATE_NETWORK_CLIENT_CALL)
				Memory::WriteRelativeCall(CreateNetworkClientHook, ptr, true);
		}
		#endif

		s_main_globals* MainGlobals() PTR_IMP_GET2(main_globals);
		s_physical_memory_map_globals* PhysicalMemoryMapGlobals() PTR_IMP_GET2(physical_memory_globals);


		s_game_state_globals* GameStateGlobals() PTR_IMP_GET2(game_state_globals);
		s_game_globals* GameGlobals() DPTR_IMP_GET(game_globals);
		s_game_time_globals* GameTimeGlobals() DPTR_IMP_GET(game_time_globals);
		s_game_allegiance_globals* GameAllegianceGlobals() DPTR_IMP_GET(game_allegiance_globals);


		TagGroups::s_game_globals* GlobalGameGlobals() PTR_IMP_GET(global_game_globals);
		TagGroups::s_game_globals** GlobalGameGlobalsReference() PTR_IMP_GET2(global_game_globals);
		s_physics_globals* Physics() PTR_IMP_GET2(global_physics);
		s_point_physics_globals* PointPhysics() PTR_IMP_GET2(global_point_physics);


		byte* DeveloperMode() PTR_IMP_GET2(developer_mode);
		static bool* TransportDumping() PTR_IMP_GET2(transport_dumping);
		bool DevmodeEnabled() PTR_IMP_GET(devmode_enabled);


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
				pop esi
				pop ebx
				pop ecx
				jmp GameState::InitializeForNewGameState
			}
		}

		API_FUNC_NAKED static void PLATFORM_API InitializeForNewMapHook()
		{
			__asm {
				pop ecx
				jmp GameState::InitializeForNewMap
			}
		}

		static bool PLATFORM_API InitializeForNewBSPHook()
		{
			static int16 bsp_index = 0;

			_asm
			{
				mov bsp_index, si
			}

			bool return_value = blam::scenario_switch_structure_bsp((int16)bsp_index);

			InitializeForNewBSP();

			return return_value;
		}

		static void InitializeForDebug()
		{
			*DeveloperMode() = Enums::k_developer_mode_level_debug_output; // make console messages appear

			// increment the game build by one so all games (hosted or browsed) aren't
			// from the normal, non-Yelo, game pool.
			BuildNumber::GameBuildString()[Enums::k_game_build_string_build_offset + 1] += 7;
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
			if (!CMDLINE_GET_PARAM(no_os_gamestate).ParameterSet())
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

			for (auto& call : K_GAME_SCENARIO_SWITCH_BSP_CALLS)
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

		void* GameStateMalloc(
			const bool k_update_allocation_crc,
			const size_t size_of)
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

		void Update(
			real delta_time)
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
			const TagGroups::s_game_globals* game_globals = GlobalGameGlobals();
			if (!yelo_header.flags.initialized)
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
			if (!Scenario::GetYelo()->IsNull())
			{
				real gravity_scale = Scenario::GetYelo()->physics.gravity_scale;
				if (gravity_scale > 0 && gravity_scale != 1.0f)
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

		static void DisposeFromOldMapPrologue() { }

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
			RuntimeData::InitializeForNewGameState();
			#if PLATFORM_IS_USER
			StructureBSP::InitializeForNewGameState();
			#endif
			AI::InitializeForNewGameState();
		}

		void InitializeForYeloGameState(
			bool enabled)
		{
			Objects::InitializeForYeloGameState(enabled);
		}

		static void HandleGameStateLifeCycle(
			Enums::game_state_life_cycle life_cycle)
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

		static void DataArrayInfoDumpToConsole(
			cstring data_array_name)
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
			Scripting::InitializeScriptFunction(
				Enums::_hs_function_physics_get_gravity,
				scripting_physics_get_gravity_evaluate);
			Scripting::InitializeScriptFunctionWithParams(
				Enums::_hs_function_physics_set_gravity,
				scripting_physics_set_gravity_evaluate);
			Scripting::InitializeScriptFunction(
				Enums::_hs_function_physics_constants_reset,
				scripting_physics_constants_reset_evaluate);

			Scripting::InitializeScriptFunctionWithParams(
				Enums::_hs_function_data_array_info,
				scripting_data_array_info_evaluate);
		}
	};
};
