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

		// Returns the index of the unit_external_upgrades block with the given tag index
		int32 DefinitionFindUnitUpgradeBlockIndex(datum_index unit_tag_index);
		// Returns the unit_external_upgrades boarding_seat block index with the given seat index
		int32 DefinitionFindUnitUpgradeBoardingSeatBlockIndex(TagGroups::s_unit_external_upgrades const* unit_upgrade_definition, int32 seat_index);
		// Returns the s_unit_external_upgrades block for the given unit_index
		TagGroups::s_unit_external_upgrades const* DefinitionFindUnitUpgradesBlock(datum_index unit_index);
		// Returns the s_unit_boarding_seat block for the given unit's seat_index
		TagGroups::s_unit_boarding_seat const* DefinitionFindUnitUpgradesBoardingSeatBlock(datum_index unit_index, int16 seat_index);
		// Returns the datum_index of the unit in the specified seat_index of the given vehicle
		datum_index GetUnitInSeat(datum_index vehicle_index, int32 seat_index);
	}; };
};