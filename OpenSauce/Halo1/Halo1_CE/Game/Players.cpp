/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Game/Players.hpp"

#include <blamlib/game/player_structures.hpp>
#include <blamlib/scenario/scenario.hpp>
#include <blamlib/scenario/scenario_definitions.hpp>
#include <blamlib/units/unit_structures.hpp>
#include <YeloLib/open_sauce/project_yellow_global_definitions.hpp>
#include <YeloLib/open_sauce/project_yellow_scenario.hpp>
#include <YeloLib/open_sauce/project_yellow_scenario_definitions.hpp>

#include "Game/GameEngine.hpp"
#include "Game/Scripting.hpp"
#include "Game/ScriptLibrary.hpp"
#include "Networking/Networking.hpp"
#include "Objects/Objects.hpp"
#include <Pointers/Game.Players.hpp>

namespace Yelo
{
	namespace Players
	{
		players_data_t& Players() DPTR_IMP_GET_BYREF(players);
		teams_data_t& Teams() DPTR_IMP_GET_BYREF(teams);
		s_players_globals_data* PlayersGlobals() DPTR_IMP_GET(players_globals);
		s_player_control_globals_data* PlayerControlGlobals() DPTR_IMP_GET(player_control_globals);
		s_player_effects_data* PlayerEffects() DPTR_IMP_GET(player_effects);
		datum_index* MachineToPlayerTable() PTR_IMP_GET2(machine_to_player_table);


		// NOTE: THE s_network_game_player_unit BLOCK IS CURRENTLY HIDDEN IN THE TAG DEFINITIONS
		// Looking at changing the struct, so consider this temporarily deprecated for now

		/*!
		* \brief
		* Determines the unit definition override for a player in Free For All games
		*/
		static datum_index PlayerFindUnitDefinitionOverride(
			const s_player_datum* player,
			datum_index unit_definition_index,
			const TAG_TBLOCK(&player_units, TagGroups::s_network_game_player_unit))
		{
			// NOTE: currently don't override FFA players
			return unit_definition_index;
		}

		/*!
		* \brief
		* Determines the unit definition override for a player in Team-based games
		*/
		static datum_index PlayerFindTeamUnitDefinitionOverride(
			const s_player_datum* player,
			datum_index unit_definition_index,
			const TAG_TBLOCK(&player_units, TagGroups::s_network_game_player_unit))
		{
			// HACK: not the best way of doing this (hate hard coding stuff), but for now it works
			static cstring k_team_names[Enums::k_number_of_multiplayer_teams] = {
				"red_team",
				"blue_team"
			};

			int32 player_team_index = player->team_index;
			if (player_team_index >= 0 && player_team_index < NUMBEROF(k_team_names))
			{
				cstring player_team_name = k_team_names[player_team_index];

				for (const auto& player_unit : player_units)
				{
					// Null definitions are removed by CheApe extensions, so tag_index should never be null (unless modified by non-OS tools)
					if (!strcmp(player_unit.name, player_team_name))
						unit_definition_index = player_unit.definition.tag_index;
				}
			}

			return unit_definition_index;
		}

		/*!
		* \brief
		* Determines the unit definition override to use for a player based on their state and if any overrides exist in the PY globals definition
		*
		* \param player_index
		* Player's index
		*
		* \param unit_definition_index
		* Default unit definition for multiplayer games
		*
		* \returns
		* The override unit definition, based on the player's state, or the default definition
		*/
		static datum_index PlayerSpawnCreateUnitMultiplayerGetDefinitionOverride(
			datum_index player_index,
			datum_index unit_definition_index)
		{
			const TAG_TBLOCK(&player_units, TagGroups::s_network_game_player_unit) = Scenario::GetYeloGlobals()->networking.player_units;
			if (player_units.count == 0)
				return unit_definition_index;

			const s_player_datum* player = Players()[player_index];

			unit_definition_index = GameEngine::GlobalVariant()->universal_variant.teams
				? PlayerFindTeamUnitDefinitionOverride(player, unit_definition_index, player_units)
				: PlayerFindUnitDefinitionOverride(player, unit_definition_index, player_units);

			return unit_definition_index;
		}

		static API_FUNC_NAKED void PLATFORM_API PlayerSpawnCreateUnitMultiplayerHook()
		{
			static const uintptr_t RETN_ADDRESS = GET_FUNC_PTR(PLAYER_SPAWN__CREATE_UNIT_MP_HOOK_RETN);

			__asm {
				push eax // unit_definition_index
				push ebx // player_index
				call PlayerSpawnCreateUnitMultiplayerGetDefinitionOverride

				jmp RETN_ADDRESS
			}
		}


		static void* scripting_volume_test_player_team_evaluate(
			void** arguments)
		{
			struct s_arguments
			{
				int16 trigger_volume;
				PAD16;
				int16 team_index;
				PAD16;
			}* args = CAST_PTR(s_arguments*, arguments);
			TypeHolder result;
			result.pointer = nullptr;
			result.boolean = false;

			if (args->team_index >= 0 && GameEngine::GlobalVariant()->universal_variant.teams)
			{
				for (auto player : Players())
				{
					if (player->team_index == args->team_index &&
						blam::scenario_trigger_volume_test_object(args->trigger_volume, player->slave_unit_index))
					{
						result.boolean = true;
						break;
					}
				}
			}

			return result.pointer;
		}

		static void* scripting_volume_test_player_team_all_evaluate(
			void** arguments)
		{
			struct s_arguments
			{
				int16 trigger_volume;
				PAD16;
				int16 team_index;
				PAD16;
			}* args = CAST_PTR(s_arguments*, arguments);
			TypeHolder result;
			result.pointer = nullptr;
			result.boolean = true;

			if (args->team_index >= 0 && GameEngine::GlobalVariant()->universal_variant.teams)
			{
				for (auto player : Players())
				{
					if (player->team_index == args->team_index &&
						!blam::scenario_trigger_volume_test_object(args->trigger_volume, player->slave_unit_index))
					{
						result.boolean = false;
						break;
					}
				}
			}
			else
				result.boolean = false;

			return result.pointer;
		}

		static void* scripting_player_team_teleport_evaluate(
			void** arguments)
		{
			struct s_arguments
			{
				int16 team_index;
				PAD16;
				int16 cutscene_flag;
				PAD16;
			}* args = CAST_PTR(s_arguments*, arguments);

			if (args->team_index >= 0 && GameEngine::GlobalVariant()->universal_variant.teams)
			{
				for (auto player : Players())
				{
					if (player->team_index == args->team_index)
					{
						TagGroups::scenario* scnr = Scenario::Scenario();
						blam::player_teleport(player.index, datum_index::null, scnr->cutscene_flags[args->cutscene_flag].position);
					}
				}
			}

			return nullptr;
		}

		static void* scripting_player_team_players_evaluate(
			void** arguments)
		{
			struct s_arguments
			{
				int16 team_index;
				PAD16;
			}* args = CAST_PTR(s_arguments*, arguments);
			TypeHolder result;
			result.datum = datum_index::null;

			if (args->team_index >= 0 && GameEngine::GlobalVariant()->universal_variant.teams)
			{
				// An object list will only be created if there is 1 or more players on the specified team
				datum_index object_list = datum_index::null;

				for (auto player : Players())
				{
					if (player->team_index == args->team_index)
					{
						if (object_list.IsNull())
							object_list = blam::object_list_new();

						blam::object_list_add(object_list, player->slave_unit_index);
					}
				}

				// list will either be null or a handle to a object-list on return
				result.datum = object_list;
			}

			return result.pointer;
		}


		static int32 scripting_player_data_get_integer_by_name(
			s_player_datum* player,
			cstring data_name,
			bool for_team_data = false)
		{
			cstring s = data_name; // alias for keeping the code width down

			if (!strcmp(s, "kills_this_lifetime"))
				return player->kills_this_lifetime;
			if (!strcmp(s, "friendly_fire_kills"))
				return player->friendly_fire_kills;
			if (!strcmp(s, "deaths"))
				return player->deaths;
			if (!strcmp(s, "suicides"))
				return player->suicides;

			//else if( !strcmp(s,"") )	return *player-;
			if (!for_team_data)
			{
				if (!strcmp(s, "team_index"))
					return player->team_index;
				if (!strcmp(s, "current_spree_count"))
					return player->current_spree_count;
				if (!strcmp(s, "last_kill_time"))
					return player->last_kill_time;
				if (!strcmp(s, "ping"))
					return player->ping;
				//else if( !strcmp(s,"") )	return *player-;
			}

			return NONE;
		}

		static void* scripting_player_data_get_integer_evaluate(
			void** arguments)
		{
			struct s_arguments
			{
				int16 player_list_index;
				PAD16;
				cstring data_name;
			}* args = CAST_PTR(s_arguments*, arguments);
			TypeHolder result;
			result.pointer = nullptr;
			result.int32 = NONE;

			if (args->player_list_index >= 0)
			{
				for (auto player : Players())
				{
					if (player->network_player.player_list_index == args->player_list_index)
					{
						result.int32 = scripting_player_data_get_integer_by_name(player.datum, args->data_name);
						break;
					}
				}
			}

			return result.pointer;
		}

		static void* scripting_player_team_data_get_integer_evaluate(
			void** arguments)
		{
			struct s_arguments
			{
				int16 team_index;
				PAD16;
				cstring data_name;
			}* args = CAST_PTR(s_arguments*, arguments);
			TypeHolder result;
			result.pointer = nullptr;
			result.int32 = NONE;

			if (args->team_index >= 0 && GameEngine::GlobalVariant()->universal_variant.teams)
			{
				for (auto player : Players())
				{
					if (player->team_index == args->team_index)
						result.int32 += scripting_player_data_get_integer_by_name(player.datum, args->data_name, true);
				}
			}

			return result.pointer;
		}

		static datum_index scripting_player_data_get_object_by_name(
			s_player_datum* player,
			cstring data_name)
		{
			datum_index object_index = player->slave_unit_index;

			if (!object_index.IsNull()) // only get object data fields whenever this player is alive
			{
				cstring s = data_name; // alias for keeping the code width down

				if (!strcmp(s, "nearest_object"))
					return player->nearest_object_action_result.action_object_index;
				if (!strcmp(s, "slave_unit"))
					return player->slave_unit_index;
				if (!strcmp(s, "last_slave_unit"))
					return player->last_slave_unit_index;
				if (!strcmp(s, "target_player"))
				{
					datum_index target_player = player->target_player_index;
					if (!target_player.IsNull())
						return Players()[target_player]->slave_unit_index;
				}
			}

			return datum_index::null;
		}

		static void* scripting_player_data_get_object_evaluate(
			void** arguments)
		{
			struct s_arguments
			{
				int16 player_list_index;
				PAD16;
				cstring data_name;
			}* args = CAST_PTR(s_arguments*, arguments);
			TypeHolder result;
			result.pointer = nullptr;
			result.datum = datum_index::null;

			if (args->player_list_index >= 0)
			{
				for (auto player : Players())
				{
					if (player->network_player.player_list_index == args->player_list_index)
					{
						result.datum = scripting_player_data_get_object_by_name(player.datum, args->data_name);
						break;
					}
				}
			}

			return result.pointer;
		}

		static real* scripting_player_data_get_real_by_name(
			s_player_datum* player,
			cstring data_name,
			Enums::hs_type& out_type)
		{
			cstring s = data_name; // alias for keeping the code width down

			out_type = HS_TYPE(real);
			if (!strcmp(s, "speed"))
				return &player->speed;
			//else if( !strcmp(s,"") )		return &player->;

			return nullptr;
		}

		static void* scripting_player_data_get_real_evaluate(
			void** arguments)
		{
			struct s_arguments
			{
				int16 player_list_index;
				PAD16;
				cstring data_name;
			}* args = CAST_PTR(s_arguments*, arguments);
			TypeHolder result;
			result.pointer = nullptr;
			result.real = -1.0f;

			if (args->player_list_index >= 0)
			{
				for (auto player : Players())
				{
					if (player->network_player.player_list_index == args->player_list_index)
					{
						Enums::hs_type result_type;
						result.ptr.real = scripting_player_data_get_real_by_name(player.datum, args->data_name, result_type);
						Scripting::UpdateTypeHolderFromPtrToData(result, result_type);
						break;
					}
				}
			}

			return result.pointer;
		}

		static void* scripting_player_data_set_real_evaluate(
			void** arguments)
		{
			struct s_arguments
			{
				int16 player_list_index;
				PAD16;
				cstring data_name;
				real data_value;
			}* args = CAST_PTR(s_arguments*, arguments);

			if (args->player_list_index >= 0)
			{
				for (auto player : Players())
				{
					if (player->network_player.player_list_index == args->player_list_index)
					{
						TypeHolder result;
						Enums::hs_type result_type;
						result.ptr.real = scripting_player_data_get_real_by_name(player.datum, args->data_name, result_type);
						Scripting::UpdateTypeHolderDataFromPtr(result, result_type, &args->data_value);
						break;
					}
				}
			}

			return nullptr;
		}


		static void* scripting_player_local_get_evaluate()
		{
			TypeHolder result;
			result.pointer = nullptr;
			result.int32 = NONE;

			auto* local_player = LocalPlayer();
			if (local_player != nullptr)
				result.int32 = local_player->network_player.player_list_index;

			return result.pointer;
		}

		namespace ActionResults
		{
			static void* player_handle_action_jmp_table_yelo[Enums::k_number_of_player_action_results_yelo];

			// Replaces PlayerHandleAction function's switch table with our own
			static void InitializePlayerHandleActionJmpTable()
			{
				// how much the stock jmp table is offset from the action_result enum
				const int32 offset = Enums::_player_action_result_swap_equipment;

				// write the game's default jump case into our jump table
				for (auto& ptr : player_handle_action_jmp_table_yelo)
					ptr = GET_PTR2(PLAYER_HANDLE_ACTION_JMP_TABLE)[1];

				// copy the game's jmp table to our own and account for the offset
				int32 handle_action_last_entry_type = GET_PTR(PLAYER_HANDLE_ACTION_LAST_ENTRY_TYPE);
				for (int32 x = offset; x <= handle_action_last_entry_type + offset; x++)
					player_handle_action_jmp_table_yelo[x] = GET_PTR2(PLAYER_HANDLE_ACTION_JMP_TABLE)[x - offset];

				static const byte k_null_opcode[] = {Enums::_x86_opcode_nop, Enums::_x86_opcode_nop, Enums::_x86_opcode_nop};

				// nullify jmp table size adjustment
				Memory::WriteMemory(GET_FUNC_VPTR(PLAYER_HANDLE_ACTION_JMP_TABLE_ADJUST_SIZE), k_null_opcode);
				// set the game's jump table count to our's
				GET_PTR(PLAYER_HANDLE_ACTION_LAST_ENTRY_TYPE) = Enums::k_number_of_player_action_results_yelo - 1;
				// set the game's jump table address to our's
				GET_PTR(player_handle_action_jmp_ptr) = player_handle_action_jmp_table_yelo;
			}

			// Initialize custom yelo action results
			static void InitializeYeloActionResults()
			{
				// Example:
				//player_handle_action_jmp_table_yelo[Enums::_player_action_result_yelo] = &JMPFunction;
				static_assert(Enums::k_number_of_player_action_results_yelo == Enums::k_number_of_player_action_results,
					"New player_action_results were defined, but I don't think you've added handlers for them yet");
			}

			void Initialize()
			{
				InitializePlayerHandleActionJmpTable();
				InitializeYeloActionResults();
			}
		};

		namespace NearbyObjects
		{
			static void* player_examine_nearby_objects_server_jmp_table_yelo[Enums::_object_type_sound_scenery + 1];
			static void* player_examine_nearby_objects_client_jmp_table_yelo[Enums::_object_type_sound_scenery + 1];

			// Replaces PlayerExamineNearbyObjectsServer function's switch table with our own
			static void InitializePlayerExamineNearbyObjectsServerJmpTable()
			{
				// copy the game's jmp table to our own
				for (int32 x = 0; x <= Enums::_object_type_control; x++)
					player_examine_nearby_objects_server_jmp_table_yelo[x] = GET_PTR2(PLAYER_EXAMINE_NEARBY_OBJECTS_SERVER_JMP_TABLE)[x];

				// write the game's default jump case into our jump table for cases 9-11
				for (int32 x = Enums::_object_type_light_fixture; x <= Enums::_object_type_sound_scenery; x++)
					player_examine_nearby_objects_server_jmp_table_yelo[x] = GET_PTR2(PLAYER_EXAMINE_NEARBY_OBJECTS_SERVER_JMP_TABLE)[0];

				// set the game's client jump table count to our's
				GET_PTR(PLAYER_EXAMINE_NEARBY_OBJECTS_SERVER_LAST_ENTRY_TYPE) = Enums::_object_type_sound_scenery;
				// set the game's client jump table address to our's
				GET_PTR(player_examine_nearby_objects_server_jmp_ptr) = player_examine_nearby_objects_server_jmp_table_yelo;
			}

			// Replaces PlayerExamineNearbyObjectsClient function's switch table with our own
			static void InitializePlayerExamineNearbyObjectsClientJmpTable()
			{
				// copy the game's jmp table to our own
				for (int32 x = 0; x <= Enums::_object_type_control; x++)
					player_examine_nearby_objects_client_jmp_table_yelo[x] = GET_PTR2(PLAYER_EXAMINE_NEARBY_OBJECTS_CLIENT_JMP_TABLE)[x];

				// write the game's default jump case into our jump table for cases 9-11
				for (int32 x = Enums::_object_type_light_fixture; x <= Enums::_object_type_sound_scenery; x++)
					player_examine_nearby_objects_client_jmp_table_yelo[x] = GET_PTR2(PLAYER_EXAMINE_NEARBY_OBJECTS_CLIENT_JMP_TABLE)[0];

				// set the game's client jump table count to our's
				GET_PTR(PLAYER_EXAMINE_NEARBY_OBJECTS_CLIENT_LAST_ENTRY_TYPE) = Enums::_object_type_sound_scenery;
				// set the game's client jump table address to our's
				GET_PTR(player_examine_nearby_objects_client_jmp_ptr) = player_examine_nearby_objects_client_jmp_table_yelo;
			}

			static void PlayerExamineNearbyBiped(
				datum_index player_index,
				datum_index biped_index)
			{
				s_player_datum* player = Players()[player_index];

				// Make sure we're not detecting ourself
				if (player->slave_unit_index != biped_index)
				{
					// Add check to see if biped is on our team to allow differentiation between unit seat entering and weapon swapping
					// Problem: when you enter a biped seat, their team changes to yours
					blam::player_examine_nearby_vehicle(player_index, biped_index);
				}
			}

			static API_FUNC_NAKED void PLATFORM_API PlayerExamineNearbyBipedServerJMP()
			{
				static const uintptr_t RETN_ADDRESS = GET_FUNC_PTR(PLAYER_EXAMINE_NEARBY_OBJECTS_SERVER_JMP_TABLE_RETN);

				__asm {
					push ecx
					push edx

					push ecx // datum_index biped_index
					push edi // datum_index player_index
					call PlayerExamineNearbyBiped

					pop edx
					pop ecx

					jmp RETN_ADDRESS
				}
			}

			static API_FUNC_NAKED void PLATFORM_API PlayerExamineNearbyBipedClientJMP()
			{
				static const uintptr_t RETN_ADDRESS = GET_FUNC_PTR(PLAYER_EXAMINE_NEARBY_OBJECTS_CLIENT_JMP_TABLE_RETN);

				__asm {
					push ecx
					push edx

					push ecx // datum_index biped_index
					push edi // datum_index player_index
					call PlayerExamineNearbyBiped

					pop edx
					pop ecx

					jmp RETN_ADDRESS
				}
			}

			// Allows us to add objects to the player_examine_nearby_objects switch tables
			static void InitializeYeloNearbyObjects()
			{
				// Detect nearby biped objects server side for seat entry
				player_examine_nearby_objects_server_jmp_table_yelo[Enums::_object_type_biped] =
					&PlayerExamineNearbyBipedServerJMP;
				// Detect nearby biped objects client side for seat entry
				player_examine_nearby_objects_client_jmp_table_yelo[Enums::_object_type_biped] =
					&PlayerExamineNearbyBipedClientJMP;
			}

			void Initialize()
			{
				InitializePlayerExamineNearbyObjectsServerJmpTable();
				InitializePlayerExamineNearbyObjectsClientJmpTable();

				InitializeYeloNearbyObjects();
			}
		};

		#if !PLATFORM_IS_DEDI
		void PLATFORM_API FollowingCameraGetUnitCameraInfoImpl(
			const int16 player_index,
			s_unit_camera_info& camera_info)
		{
			auto& local_player = PlayerControlGlobals()->local_players[player_index];
			auto& player_unit_datum = *blam::object_get_and_verify_type<Objects::s_unit_datum>(local_player.unit_index);

			// If the player has been mounted, see if a third person camera should be used
			bool info_set = false;
			if (player_unit_datum.unit.animation.state == Enums::_unit_animation_state_yelo_unit_mounted)
			{
				auto& unit_definition = *blam::tag_get<TagGroups::s_unit_definition>(player_unit_datum.object.definition_index);
				if ((unit_definition.unit.extensions.count == 1)
					&& (unit_definition.unit.extensions[0].mounted_state.count == 1))
				{
					auto& mounted_state_definition = unit_definition.unit.extensions[0].mounted_state[0];
					if (TEST_FLAG(mounted_state_definition.flags, Flags::_unit_mounted_state_flags_third_person_camera))
					{
						// Return the camera info for the mounted state camera
						Objects::s_object_marker marker;
						if (blam::object_get_marker_by_name(local_player.unit_index, mounted_state_definition.unit_camera.camera_marker_name, &marker, 1) == 1)
						{
							camera_info.unit_index = local_player.unit_index;
							camera_info.seat_index = NONE;
							camera_info.unit_camera_definition = &mounted_state_definition.unit_camera;
							camera_info.position = marker.transformed_matrix.Position;
							return;
						}
					}
				}
			}

			blam::player_control_get_unit_camera_info(player_index, camera_info);
		}

		API_FUNC_NAKED void FollowingCameraGetUnitCameraInfoHook()
		{
			API_FUNC_NAKED_START()
				push ecx

				xor ecx, ecx
				movsx cx, ax
				push esi
				push ecx
				call FollowingCameraGetUnitCameraInfoImpl
				add esp, 8

				pop ecx
				pop ebp
			API_FUNC_NAKED_END_()
		}
		#endif

		void Initialize()
		{
			Scripting::InitializeScriptFunctionWithParams(
				Enums::_hs_function_volume_test_player_team,
				scripting_volume_test_player_team_evaluate);
			Scripting::InitializeScriptFunctionWithParams(
				Enums::_hs_function_volume_test_player_team_all,
				scripting_volume_test_player_team_all_evaluate);
			Scripting::InitializeScriptFunctionWithParams(
				Enums::_hs_function_player_team_teleport,
				scripting_player_team_teleport_evaluate);
			Scripting::InitializeScriptFunctionWithParams(
				Enums::_hs_function_player_team_players,
				scripting_player_team_players_evaluate);

			Scripting::InitializeScriptFunctionWithParams(
				Enums::_hs_function_player_data_get_integer,
				scripting_player_data_get_integer_evaluate);
			Scripting::InitializeScriptFunctionWithParams(
				Enums::_hs_function_player_team_data_get_integer,
				scripting_player_team_data_get_integer_evaluate);
			Scripting::InitializeScriptFunctionWithParams(
				Enums::_hs_function_player_data_get_object,
				scripting_player_data_get_object_evaluate);
			Scripting::InitializeScriptFunctionWithParams(
				Enums::_hs_function_player_data_get_real,
				scripting_player_data_get_real_evaluate);
			Scripting::InitializeScriptFunctionWithParams(
				Enums::_hs_function_player_data_set_real,
				scripting_player_data_set_real_evaluate);
			Scripting::InitializeScriptFunction(
				Enums::_hs_function_player_local_get,
				scripting_player_local_get_evaluate);

			Memory::WriteRelativeJmp(PlayerSpawnCreateUnitMultiplayerHook, GET_FUNC_VPTR(PLAYER_SPAWN__CREATE_UNIT_MP_HOOK));
			Memory::WriteRelativeJmp(UpdateForServer, GET_FUNC_VPTR(PLAYERS_UPDATE_BEFORE_GAME_SERVER_HOOK), true);
			Memory::WriteRelativeJmp(UpdateForClient, GET_FUNC_VPTR(PLAYERS_UPDATE_BEFORE_GAME_CLIENT_HOOK), true);
			Memory::WriteRelativeJmp(Update, GET_FUNC_VPTR(PLAYERS_UPDATE_AFTER_GAME_HOOK), true);

			#if !PLATFORM_IS_DEDI
			Memory::WriteRelativeCall(&FollowingCameraGetUnitCameraInfoHook, GET_FUNC_VPTR(FOLLOWING_CAMERA_GET_UNIT_CAMERA_INFO_CALL), true);
			#endif

			NearbyObjects::Initialize();
			ActionResults::Initialize();
		}

		void Dispose() { }

		void PLATFORM_API UpdateForServer() { }

		void PLATFORM_API UpdateForClient() { }

		void PLATFORM_API Update() { }

		Objects::s_unit_datum* s_player_datum::GetPlayerUnit()
		{
			datum_index object_index = this->slave_unit_index;
			if (object_index.IsNull())
				return nullptr;

			return Objects::ObjectHeader()[object_index]->_unit;
		}

		datum_index s_player_datum::GetVehicleIndex()
		{
			datum_index vehicle_index = datum_index::null;

			Objects::s_unit_datum* unit = this->GetPlayerUnit();
			if (unit != nullptr)
			{
				datum_index parent_object_index = unit->object.parent_object_index;
				if (!parent_object_index.IsNull() && unit->unit.vehicle_seat_index == NONE)
					vehicle_index = parent_object_index;
			}

			return vehicle_index;
		}

		s_player_yelo_server_data& s_player_datum::GetYeloServerData()
		{
			assert(GameState::IsServer());

			return server.yelo_data;
		}

		datum_index LocalPlayerIndex()
		{
			for (auto player : Players())
			{
				if (player->local_player_index != NONE)
					return player.index;
			}

			return datum_index::null;
		}

		s_player_datum* LocalPlayer()
		{
			for (auto player : Players())
			{
				if (player->local_player_index != NONE)
					return player.datum;
			}

			return nullptr;
		}

		s_player_datum* GetPlayerFromNumber(
			byte player_number,
			datum_index* player_index_reference)
		{
			if (player_index_reference != nullptr)
				*player_index_reference = datum_index::null;
			if (player_number == NONE)
				return nullptr;

			for (auto player : Players())
			{
				if (player->network_player.player_list_index == player_number)
				{
					if (player_index_reference != nullptr)
						*player_index_reference = player.index;
					return player.datum;
				}
			}

			return nullptr;
		}


		Objects::s_unit_datum* GetPlayerUnit(
			datum_index player_index)
		{
			if (player_index.IsNull())
				return nullptr;

			return Players()[player_index]->GetPlayerUnit();
		}

		datum_index* GetWeapons(
			datum_index player_index,
			int16* current_weapon_index)
		{
			Objects::s_unit_datum* unit = GetPlayerUnit(player_index);

			if (unit == nullptr)
				return nullptr;

			if (current_weapon_index)
				*current_weapon_index = unit->unit.current_weapon_index;

			return unit->unit.weapon_object_indices;
		}

		Objects::s_unit_datum* GetVehicle(
			datum_index player_index,
			int16* current_seat_index)
		{
			Objects::s_unit_datum* unit = GetPlayerUnit(player_index);

			if (unit == nullptr)
				return nullptr;

			if (current_seat_index)
				*current_seat_index = unit->unit.vehicle_seat_index;

			datum_index parent_index = unit->object.parent_object_index;
			if (parent_index.IsNull())
				return nullptr;

			return Objects::ObjectHeader()[parent_index]->_unit;
		}
	};
};
