/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Objects/Units.hpp"

#include <blamlib/Halo1/game/game_globals.hpp>
#include <blamlib/Halo1/units/unit_definitions.hpp>
#include <blamlib/Halo1/models/model_animation_definitions.hpp>

#include "Game/EngineFunctions.hpp"
#include "Game/GameState.hpp"
#include "Memory/MemoryInterface.hpp"
#include "Networking/MDP.hpp"
#include "Networking/MessageDeltas.hpp"
#include "TagGroups/TagGroups.hpp"

#include "TagGroups/project_yellow_definitions.hpp"

namespace Yelo
{
	namespace Objects {
		byte* s_unit_data::GetYeloGrenade2Count()
		{
			GameState::s_yelo_header_data& yelo_header = GameState::GameStateGlobals()->header->yelo;

			return GameState::YeloGameStateEnabled() && yelo_header.unit_grenade_types_count >= Enums::_unit_grenade_type_yelo2+1
				? &this->grenade_counts[Enums::_unit_grenade_type_yelo2] : NULL;
		}
		byte* s_unit_data::GetYeloGrenade3Count()
		{
			GameState::s_yelo_header_data& yelo_header = GameState::GameStateGlobals()->header->yelo;

			return GameState::YeloGameStateEnabled() && yelo_header.unit_grenade_types_count >= Enums::_unit_grenade_type_yelo3+1
				? &this->grenade_counts[Enums::_unit_grenade_type_yelo3] : NULL;
		}

		byte* s_unit_data::GetZoomLevel()
		{
			GameState::s_yelo_header_data& yelo_header = GameState::GameStateGlobals()->header->yelo;

			return GameState::YeloGameStateEnabled() && yelo_header.unit_grenade_types_count > Enums::k_unit_grenade_types_count
				? &this->zoom_level_yelo : &this->zoom_level;
		}
		byte* s_unit_data::GetDesiredZoomLevel()
		{
			GameState::s_yelo_header_data& yelo_header = GameState::GameStateGlobals()->header->yelo;

			return GameState::YeloGameStateEnabled() && yelo_header.unit_grenade_types_count > Enums::k_unit_grenade_types_count
				? &this->desired_zoom_level_yelo : &this->desired_zoom_level;
		}
	};

	namespace Objects { namespace Units {
#define __EL_INCLUDE_ID			__EL_INCLUDE_OBJECTS
#define __EL_INCLUDE_FILE_ID	__EL_OBJECTS_UNITS
#include "Memory/_EngineLayout.inl"
#include "Objects/Units.Boarding.inl"
#include "Objects/Units.Animations.inl"
#include "Objects/Units.GrenadeCounts.inl"

		void Initialize()
		{
			Animations::Initialize();
			Boarding::Initialize();
			
			static const byte opcode_null[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };

			Memory::WriteMemory(GET_FUNC_VPTR(BIPED_UPDATE_CHECK_PARENT_UNIT_TYPE), opcode_null, 6);
		}

		void Dispose()
		{
		}

		void InitializeForNewMap()
		{
			GameState::s_yelo_header_data& yelo_header = GameState::GameStateGlobals()->header->yelo;
			// handle the grenade types upgrading, if needed
			if(	GameState::YeloGameStateEnabled() && 
				yelo_header.flags.update_unit_grenade_types_count)
			{
				uint32 count = yelo_header.unit_grenade_types_count;

				bool enabled = count > Enums::k_unit_grenade_types_count;

				InitializeGrenadeCounts_UnitZoomLevelRefs(enabled);
				InitializeGrenadeCounts_UnitDesiredZoomLevelRefs(enabled);
				InitializeGrenadeCounts_NumberOfUnitGrenadeTypes(count);
				InitializeGrenadeCounts_UnitGrenadeCounts(enabled);
				InitializeGrenadeCounts_MessageDeltaGrenadeCounts(enabled);

				yelo_header.flags.update_unit_grenade_types_count = false;
			}
		}

		void DisposeFromOldMap()
		{
		}

		void InitializeForYeloGameState(bool enabled)
		{
		}

		int32 DefinitionFindUnitUpgradeBlockIndex(datum_index unit_tag_index)
		{
			const TagGroups::project_yellow_globals_cv* cv_globals = TagGroups::CvGlobals();

			if(cv_globals == NULL)
				return NONE;

			int32 unit_external_upgrades_count = cv_globals->unit_external_upgrades.Count;

			for(int32 x = 0; x < unit_external_upgrades_count; x++)
			{
				TagGroups::s_unit_external_upgrades const& unit_upgrade = cv_globals->unit_external_upgrades[x];

				if(unit_tag_index == unit_upgrade.unit.tag_index)
					return x;
			}
			return NONE;
		}
		
		int32 DefinitionFindUnitUpgradeBoardingSeatBlockIndex(TagGroups::s_unit_external_upgrades const* unit_upgrade_definition,
			int32 seat_index)
		{
			if (unit_upgrade_definition != NULL)
			{
				int32 boarding_seats_count = unit_upgrade_definition->boarding_seats.Count;

				for(int32 x = 0; x < boarding_seats_count; x++)
				{
					if(seat_index == unit_upgrade_definition->boarding_seats[x].seat_index)
						return x;
				}
			}
			return NONE;
		}

		TagGroups::s_unit_external_upgrades const* DefinitionFindUnitUpgradesBlock(datum_index unit_index)
		{
			const TagGroups::project_yellow_globals_cv* cv_globals = TagGroups::CvGlobals();

			if(cv_globals == NULL)
				return NULL;

			if (unit_index != datum_index::null)
			{
				// Check if a unit upgrades block exists in yelo_globals
				if (cv_globals->unit_external_upgrades.Count > 0)
				{
					Objects::s_unit_datum* unit = (*Objects::ObjectHeader())[unit_index]->_unit;
					datum_index unit_tag_index = unit->object.definition_index;
					int32 unit_upgrade_index = Objects::Units::DefinitionFindUnitUpgradeBlockIndex(unit_tag_index);

					// Check if an upgrades block exists for this unit
					if (unit_upgrade_index != NONE)
						return &cv_globals->unit_external_upgrades[unit_upgrade_index];
				}
			}
			return NULL;
		}

		TagGroups::s_unit_boarding_seat const* DefinitionFindUnitUpgradesBoardingSeatBlock(datum_index unit_index, int16 seat_index)
		{
			if (unit_index != datum_index::null)
			{
				TagGroups::s_unit_external_upgrades const* unit_external_upgrades_definition = DefinitionFindUnitUpgradesBlock(unit_index);

				// Check if a unit upgrades block exists in yelo_globals
				if (unit_external_upgrades_definition != NULL)
				{
					int32 boarding_seat_block_index = 
						DefinitionFindUnitUpgradeBoardingSeatBlockIndex(unit_external_upgrades_definition, seat_index);

					// Check if boarding seat block exists for this seat index
					if (boarding_seat_block_index != NONE)
						return &unit_external_upgrades_definition->boarding_seats[boarding_seat_block_index];
				}
			}
			return NULL;
		}

		datum_index GetUnitInSeat(datum_index vehicle_index, int32 seat_index)
		{
			s_unit_datum* vehicle = (*Objects::ObjectHeader())[vehicle_index]->_unit;
			datum_index unit = datum_index::null;
			datum_index next_object = vehicle->object.first_object_index;

			for (;;)
			{
				if (next_object == datum_index::null) break;

				int16 unit_seat_index = (*Objects::ObjectHeader())[next_object]->_unit->unit.vehicle_seat_index;
				byte object_type = (*Objects::ObjectHeader())[next_object]->_object->type;

				if (object_type == Yelo::Enums::_object_type_biped)
					if (unit_seat_index == seat_index) unit = next_object;

				next_object = (*Objects::ObjectHeader())[next_object]->_object->next_object_index;
			}

			return unit;
		}

	}; };
};