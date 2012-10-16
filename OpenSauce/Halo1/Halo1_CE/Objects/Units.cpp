/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Objects/Units.hpp"

#include "Game/GameState.hpp"
#include "Memory/MemoryInterface.hpp"
#include "Networking/MDP.hpp"
#include "Networking/MessageDeltas.hpp"

#include <TagGroups/Halo1/unit_definitions.hpp>
#include <TagGroups/Halo1/game_globals_definitions.hpp>
#include "Objects/Objects.hpp"

namespace Yelo
{
	namespace Objects {
		byte* s_unit_data::GetYeloGrenade2Count()
		{
			return !GameState::YeloGameStateEnabled() ? NULL : this->GetYeloGrenade2Count_();
		}
		byte* s_unit_data::GetYeloGrenade3Count()
		{
			return !GameState::YeloGameStateEnabled() ? NULL : this->GetYeloGrenade3Count_();
		}

		byte* s_unit_data::GetZoomLevel()
		{
			return !GameState::YeloGameStateEnabled() ? this->GetZoomLevel_() : this->GetYeloZoomLevel();
		}
		byte* s_unit_data::GetDesiredZoomLevel()
		{
			return !GameState::YeloGameStateEnabled() ? this->GetDesiredZoomLevel_() : this->GetYeloDesiredZoomLevel();
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
			if(yelo_header.flags.update_unit_grenade_types_count)
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