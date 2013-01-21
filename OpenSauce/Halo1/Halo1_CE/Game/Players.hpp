/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/effects/player_effects.hpp>
#include <blamlib/Halo1/game/player_control.hpp>
#include <blamlib/Halo1/game/player_structures.hpp>
#include <blamlib/Halo1/game/players.hpp>
#include <blamlib/Halo1/memory/data.hpp>

namespace Yelo
{
	namespace Players
	{
		typedef Memory::DataArray<s_player_datum, Enums::k_multiplayer_maximum_players_upgrade> t_players_data;
		t_players_data*					Players();
		typedef Memory::DataArray<s_team_datum, Enums::k_multiplayer_maximum_teams> t_teams_data;
		t_teams_data*					Teams();
		s_players_globals_data*			PlayersGlobals();
		s_player_control_globals_data*	PlayerControlGlobals();
		s_player_effects_data*			PlayerEffects();


		datum_index*					MachineToPlayerTable(); // [k_multiplayer_maximum_players][k_maximum_number_of_local_players]

		void Initialize();
		void Dispose();

		void PLATFORM_API UpdateServer();	// server & client updates are called before core game update functions
		void PLATFORM_API UpdateClient();
		void PLATFORM_API Update();			// called after core game update functions

		// Get the handle to the currently active local player
		// Returns a null handle if there is none.
		datum_index LocalPlayerIndex();
		// Get the player data of the currently active local player
		// Returns NULL if there is none.
		s_player_datum* LocalPlayer();
		// Get the player data from a player number, and optionally their player_index
		// Returns NULL if player number is invalid or unused
		s_player_datum* GetPlayerFromNumber(byte player_number, __out datum_index* player_index_reference = NULL);


		// Use the handle to a player to get the active unit object which it controls
		// Returns NULL if player handle or the unit handle for the player is NONE
		Objects::s_unit_datum* GetPlayerUnit(datum_index player_index);

		// Use the handle to a player to get the weapons held by it's active unit which 
		// they are in control of.
		// Returns NULL if player handle or the unit handle for the player is NONE
		datum_index* GetWeapons(datum_index player_index, int16* current_weapon_index = NULL);

		// Use the handle to a player to get the active unit object of the vehicle 
		// which the unit the player is controlling is currently in.
		// Returns NULL if player handle or the unit handle for the player is NONE
		// Also returns NULL if player isn't in a vehicle
		// NOTE: "vehicle" is a loose term here, could be another unit derived type, 
		// possibly even a biped, who knows!
		Objects::s_unit_datum* GetVehicle(datum_index player_index, int16* current_seat_index = NULL);
	};
};