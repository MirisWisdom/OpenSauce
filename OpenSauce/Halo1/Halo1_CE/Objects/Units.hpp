/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Objects { namespace Units {
		void Initialize();
		void Dispose();
		void InitializeForNewMap();
		void DisposeFromOldMap();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>Initialize the Units system for a new map before any of the actual initialize_for_new_map procedures are
		/// 	executed This way we're able to ascertain state changes from the old map with the new map. Eg, differing unit
		/// 	grenade type counts.
		/// </summary>
		void InitializeForNewMapPrologue();

		void InitializeForYeloGameState(bool enabled);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Get the object_index of the unit in the specified seat_index of the given vehicle. </summary>
		///
		/// <param name="vehicle_index">	subject vehicle. </param>
		/// <param name="seat_index">   	subject vehicle seat. </param>
		///
		/// <returns>	The index of the unit in the seat. </returns>
		datum_index GetUnitInSeat(datum_index vehicle_index, int32 seat_index);
	}; };
};