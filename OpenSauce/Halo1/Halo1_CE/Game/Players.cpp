/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Game/Players.hpp"

#include <blamlib/Halo1/scenario/scenario.hpp>

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
#include "TagGroups/project_yellow_definitions.hpp"

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
			assert(Networking::IsServer());

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