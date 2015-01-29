/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Game/Players.hpp"

#include <blamlib/Halo1/game/player_structures.hpp>
#include <blamlib/Halo1/scenario/scenario.hpp>
#include <blamlib/Halo1/scenario/scenario_definitions.hpp>
#include <blamlib/Halo1/units/unit_structures.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_global_definitions.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_scenario.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_scenario_definitions.hpp>

#include "Game/EngineFunctions.hpp"
#include "Game/GameEngine.hpp"
#include "Game/GameState.hpp"
#include "Game/Scripting.hpp"
#include "Game/ScriptLibrary.hpp"
#include "Memory/MemoryInterface.hpp"
#include "Networking/Networking.hpp"
#include "Objects/Objects.hpp"
#include "Objects/Units.hpp"
#include "Scenario/Scenario.hpp"

namespace Yelo
{
	namespace Players
	{
#define __EL_INCLUDE_ID			__EL_INCLUDE_GAME
#define __EL_INCLUDE_FILE_ID	__EL_GAME_PLAYERS
#include "Memory/_EngineLayout.inl"

		players_data_t& Players()										DPTR_IMP_GET_BYREF(players);
		teams_data_t& Teams()											DPTR_IMP_GET_BYREF(teams);
		s_players_globals_data* PlayersGlobals()						DPTR_IMP_GET(players_globals);
		s_player_control_globals_data* PlayerControlGlobals()			DPTR_IMP_GET(player_control_globals);
		s_player_effects_data* PlayerEffects()							DPTR_IMP_GET(player_effects);
		datum_index* MachineToPlayerTable()								PTR_IMP_GET2(machine_to_player_table);


// For the player unit overrides implementation
#include "Game/Players.NetworkPlayerUnit.inl"

#include "Game/Players.Scripting.inl"
#include "Game/Players.ActionResults.inl"
#include "Game/Players.NearbyObjects.inl"

#if !PLATFORM_IS_DEDI
		void PLATFORM_API FollowingCameraGetUnitCameraInfoImpl(const int16 player_index, Players::s_unit_camera_info& camera_info)
		{
			auto& local_player = PlayerControlGlobals()->local_players[player_index];
			auto& player_unit_datum = *blam::object_get_and_verify_type<Objects::s_unit_datum>(local_player.unit_index);

			// If the player has been mounted, see if a third person camera should be used
			bool info_set = false;
			if(player_unit_datum.unit.animation.state == Enums::_unit_animation_state_yelo_unit_mounted)
			{
				auto& unit_definition = *blam::tag_get<TagGroups::s_unit_definition>(player_unit_datum.object.definition_index);
				if((unit_definition.unit.extensions.Count == 1)
					&& (unit_definition.unit.extensions[0].mounted_state.Count == 1))
				{
					auto& mounted_state_definition = unit_definition.unit.extensions[0].mounted_state[0];
					if(TEST_FLAG(mounted_state_definition.flags, Flags::_unit_mounted_state_flags_third_person_camera))
					{
						// Return the camera info for the mounted state camera
						Objects::s_object_marker marker;
						if(blam::object_get_marker_by_name(local_player.unit_index, mounted_state_definition.unit_camera.camera_marker_name, &marker, 1) == 1)
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
				push	ecx

				xor		ecx, ecx
				movsx	cx, ax
				push	esi
				push	ecx
				call	FollowingCameraGetUnitCameraInfoImpl
				add		esp, 8

				pop		ecx
				pop		ebp
			API_FUNC_NAKED_END_()
		}
#endif

		void Initialize()
		{
			Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_volume_test_player_team, 
				scripting_volume_test_player_team_evaluate);
			Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_volume_test_player_team_all, 
				scripting_volume_test_player_team_all_evaluate);
			Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_player_team_teleport, 
				scripting_player_team_teleport_evaluate);
			Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_player_team_players, 
				scripting_player_team_players_evaluate);

			Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_player_data_get_integer, 
				scripting_player_data_get_integer_evaluate);
			Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_player_team_data_get_integer, 
				scripting_player_team_data_get_integer_evaluate);
			Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_player_data_get_object, 
				scripting_player_data_get_object_evaluate);
			Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_player_data_get_real, 
				scripting_player_data_get_real_evaluate);
			Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_player_data_set_real, 
				scripting_player_data_set_real_evaluate);
 			Scripting::InitializeScriptFunction(Enums::_hs_function_player_local_get, 
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

		void Dispose()
		{
		}

		void PLATFORM_API UpdateForServer()
		{
		}

		void PLATFORM_API UpdateForClient()
		{
		}

		void PLATFORM_API Update()
		{
		}

		Objects::s_unit_datum* s_player_datum::GetPlayerUnit()
		{
			datum_index object_index = this->slave_unit_index;
			if(object_index.IsNull()) return nullptr;

			return Objects::ObjectHeader()[object_index]->_unit;
		}

		datum_index s_player_datum::GetVehicleIndex()
		{
			datum_index vehicle_index = datum_index::null;

			Objects::s_unit_datum* unit = this->GetPlayerUnit();
			if(unit != nullptr)
			{
				datum_index parent_object_index = unit->object.parent_object_index;
				if(	!parent_object_index.IsNull() && unit->unit.vehicle_seat_index == NONE )
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
			for(auto player : Players())
			{
				if(player->local_player_index != NONE)
					return player.index;
			}

			return datum_index::null;
		}

		s_player_datum* LocalPlayer()
		{
			for(auto player : Players())
			{
				if(player->local_player_index != NONE)
					return player.datum;
			}

			return nullptr;
		}

		s_player_datum* GetPlayerFromNumber(byte player_number, datum_index* player_index_reference)
		{
			if(player_index_reference != nullptr) *player_index_reference = datum_index::null;
			if(player_number == NONE) return nullptr;

			for(auto player : Players())
			{
				if(player->network_player.player_list_index == player_number)
				{
					if(player_index_reference != nullptr) *player_index_reference = player.index;
					return player.datum;
				}
			}

			return nullptr;
		}


		Objects::s_unit_datum* GetPlayerUnit(datum_index player_index)
		{
			if(player_index.IsNull()) return nullptr;
			
			return Players::Players()[player_index]->GetPlayerUnit();
		}

		datum_index* GetWeapons(datum_index player_index, int16* current_weapon_index)
		{
			Objects::s_unit_datum* unit = GetPlayerUnit(player_index);

			if(unit == nullptr) return nullptr;

			if(current_weapon_index)
				*current_weapon_index = unit->unit.current_weapon_index;

			return unit->unit.weapon_object_indices;
		}

		Objects::s_unit_datum* GetVehicle(datum_index player_index, int16* current_seat_index)
		{
			Objects::s_unit_datum* unit = GetPlayerUnit(player_index);

			if(unit == nullptr) return nullptr;

			if(current_seat_index)
				*current_seat_index = unit->unit.vehicle_seat_index;

			datum_index parent_index = unit->object.parent_object_index;
			if(parent_index.IsNull())
				return nullptr;

			return Objects::ObjectHeader()[parent_index]->_unit;
		}
	};
};