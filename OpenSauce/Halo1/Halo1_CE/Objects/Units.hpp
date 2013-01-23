/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include "Objects/Objects.hpp"
#include "TagGroups/TagGroups.hpp"

#include "TagGroups/project_yellow_definitions.hpp"

namespace Yelo
{
	namespace Objects { namespace Units {
		void Initialize();
		void Dispose();
		void InitializeForNewMap();
		void DisposeFromOldMap();

		void InitializeForYeloGameState(bool enabled);

		// Returns the datum_index of the unit in the specified seat_index of the given vehicle
		datum_index GetUnitInSeat(datum_index vehicle_index, int32 seat_index);
	}; };
};