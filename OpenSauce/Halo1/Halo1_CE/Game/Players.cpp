/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Game/Players.hpp"

#include "Game/EngineFunctions.hpp"
#include "Game/GameEngine.hpp"
#include "Game/GameState.hpp"
#include "Game/Scripting.hpp"
#include "Game/ScriptLibrary.hpp"
#include "Memory/MemoryInterface.hpp"
#include "Networking/Networking.hpp"
#include "Objects/Objects.hpp"
#include "TagGroups/project_yellow_definitions.hpp"

namespace Yelo
{
	namespace Players
	{
#define __EL_INCLUDE_ID			__EL_INCLUDE_GAME
#define __EL_INCLUDE_FILE_ID	__EL_GAME_PLAYERS
#include "Memory/_EngineLayout.inl"

		t_players_data* Players()										DPTR_IMP_GET(players);
		t_teams_data* Teams()											DPTR_IMP_GET(teams);
		s_players_globals_data* PlayersGlobals()						DPTR_IMP_GET(players_globals);
		s_player_control_globals_data* PlayerControlGlobals()			DPTR_IMP_GET(player_control_globals);
		s_player_effects_data* PlayerEffects()							DPTR_IMP_GET(player_effects);
		datum_index* MachineToPlayerTable()								PTR_IMP_GET2(machine_to_player_table);


// For the player unit overrides implementation
#include "Game/Players.NetworkPlayerUnit.inl"

#include "Game/Players.Scripting.inl"
#include "Game/Players.ActionResults.inl"
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
			Memory::WriteRelativeJmp(UpdateServer, GET_FUNC_VPTR(PLAYERS_UPDATE_BEFORE_GAME_SERVER_HOOK), true);
			Memory::WriteRelativeJmp(UpdateClient, GET_FUNC_VPTR(PLAYERS_UPDATE_BEFORE_GAME_CLIENT_HOOK), true);
			Memory::WriteRelativeJmp(Update, GET_FUNC_VPTR(PLAYERS_UPDATE_AFTER_GAME_HOOK), true);
		}

		void Dispose()
		{
		}

		void PLATFORM_API UpdateServer()
		{
		}

		void PLATFORM_API UpdateClient()
		{
		}

		void PLATFORM_API Update()
		{
		}

		Objects::s_unit_datum* s_player_datum::GetPlayerUnit()
		{
			datum_index object_index = this->slave_unit_index;
			if(object_index.IsNull()) return NULL;

			return (*Objects::ObjectHeader())[object_index]->_unit;
		}

		datum_index s_player_datum::GetVehicleIndex()
		{
			datum_index vehicle_index = datum_index::null;

			Objects::s_unit_datum* unit = this->GetPlayerUnit();
			if(unit != NULL)
			{
				datum_index parent_object_index = unit->object.parent_object_index;
				if(	!parent_object_index.IsNull() && unit->unit.vehicle_seat_index == NONE )
					vehicle_index = parent_object_index;
			}

			return vehicle_index;
		}

		datum_index LocalPlayerIndex()
		{
			t_players_data::Iterator iter(Players::Players());
			s_player_datum* player;
			while( (player = iter.Next()) != NULL )
			{
				if(player->local_player_index != NONE)
					return iter.Current();
			}

			return datum_index::null;
		}

		s_player_datum* LocalPlayer()
		{
			t_players_data::Iterator iter(Players::Players());
			s_player_datum* player;
			while( (player = iter.Next()) != NULL )
			{
				if(player->local_player_index != NONE)
					return player;
			}

			return NULL;
		}

		s_player_datum* GetPlayerFromNumber(byte player_number, datum_index* player_index_reference)
		{
			if(player_index_reference != NULL) *player_index_reference = datum_index::null;
			if(player_number == NONE) return NULL;

			t_players_data::Iterator iter(Players::Players());
			s_player_datum* player;
			while( (player = iter.Next()) != NULL )
			{
				if(player->network_player.player_list_index == player_number)
				{
					if(player_index_reference != NULL) *player_index_reference = iter.Current();
					return player;
				}
			}

			return NULL;
		}


		Objects::s_unit_datum* GetPlayerUnit(datum_index player_index)
		{
			if(player_index.IsNull()) return NULL;
			
			return (*Players::Players())[player_index]->GetPlayerUnit();
		}

		datum_index* GetWeapons(datum_index player_index, int16* current_weapon_index)
		{
			Objects::s_unit_datum* unit = GetPlayerUnit(player_index);

			if(unit == NULL) return NULL;

			if(current_weapon_index)
				*current_weapon_index = unit->unit.current_weapon_index;

			return unit->unit.weapon_object_indices;
		}

		Objects::s_unit_datum* GetVehicle(datum_index player_index, int16* current_seat_index)
		{
			Objects::s_unit_datum* unit = GetPlayerUnit(player_index);

			if(unit == NULL) return NULL;

			if(current_seat_index)
				*current_seat_index = unit->unit.vehicle_seat_index;

			datum_index parent_index = unit->object.parent_object_index;
			if(parent_index.IsNull())
				return NULL;

			return (*Objects::ObjectHeader())[parent_index]->_unit;
		}
	};
};