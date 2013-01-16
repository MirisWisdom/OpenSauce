/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Objects/Units.hpp"

#include <blamlib/Halo1/game/game_globals.hpp>
#include <blamlib/Halo1/units/unit_definitions.hpp>

#include "Game/GameState.hpp"
#include "Memory/MemoryInterface.hpp"
#include "Networking/MDP.hpp"
#include "Networking/MessageDeltas.hpp"

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
#include "Objects/Units.GrenadeCounts.inl"

		void Initialize()
		{
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

	}; };
};