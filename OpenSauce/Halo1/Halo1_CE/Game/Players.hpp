/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/effects/player_effects.hpp>
#include <blamlib/Halo1/game/player_control.hpp>
#include <blamlib/Halo1/game/players.hpp>
#include <blamlib/Halo1/memory/data.hpp>

namespace Yelo
{
	namespace Objects
	{
		struct s_unit_datum;
	};

	namespace Players
	{
		void Initialize();
		void Dispose();
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Server side updates of players </summary>
		///
		/// <remarks>	called before core game update functions </remarks>
		void PLATFORM_API UpdateForServer();
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Client side updates of players </summary>
		///
		/// <remarks>	called before core game update functions </remarks>
		void PLATFORM_API UpdateForClient();
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <remarks>	called after core game update functions </remarks>
		void PLATFORM_API Update();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Get the player data of the currently active local player </summary>
		///
		/// <returns>	NULL if there is none </returns>
		datum_index LocalPlayerIndex();
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Get the player data of the currently active local player </summary>
		///
		/// <returns>	NULL if there is none </returns>
		s_player_datum* LocalPlayer();
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Get the player data from a player number, and optionally their player_index </summary>
		///
		/// <param name="player_number">		 	The player number </param>
		/// <param name="player_index_reference">	[out] (Optional) the player index </param>
		///
		/// <returns>	NULL if player number is invalid or unused </returns>
		s_player_datum* GetPlayerFromNumber(byte player_number, _Out_ datum_index* player_index_reference = nullptr);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Use the handle to a player to get the active unit object which it controls </summary>
		///
		/// <param name="player_index">	 </param>
		///
		/// <returns>	NULL if player handle or the unit handle for the player is NONE. </returns>
		Objects::s_unit_datum* GetPlayerUnit(datum_index player_index);
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// 	Use the handle to a player to get the weapons held by it slave unit which they are in control of.
		/// </summary>
		///
		/// <param name="player_index">		   	 </param>
		/// <param name="current_weapon_index">	[out] (Optional) the readied weapon index. </param>
		///
		/// <returns>	NULL if player handle or the unit handle for the player is NONE. </returns>
		datum_index* GetWeapons(datum_index player_index, _Out_ int16* current_weapon_index = nullptr);
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// 	Use the handle to a player to get the active unit object of the vehicle which the unit the player is
		/// 	controlling is currently in
		/// </summary>
		///
		/// <param name="player_index">		 	 </param>
		/// <param name="current_seat_index">	[out] (Optional) the player's vehicle seat index </param>
		///
		/// <returns>
		/// 	NULL if player handle or the unit handle for the player is NONE. Also returns NULL if player isn't in a
		/// 	vehicle NOTE: "vehicle" is a loose term here, could be another unit derived. NOTE: "vehicle" is a loose term
		/// 	here, could be another unit derived type, possibly even a biped, who knows!
		/// </returns>
		Objects::s_unit_datum* GetVehicle(datum_index player_index, _Out_ int16* current_seat_index = nullptr);
	};
};