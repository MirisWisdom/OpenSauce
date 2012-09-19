/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Objects/Units.hpp"

#include "Game/GameState.hpp"
#include "Memory/MemoryInterface.hpp"

#include <TagGroups/Halo1/unit_definitions.hpp>
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
			return !GameState::YeloGameStateEnabled() ? this->GetYeloZoomLevel() : this->GetZoomLevel_();
		}
		byte* s_unit_data::GetDesiredZoomLevel()
		{
			return !GameState::YeloGameStateEnabled() ? this->GetYeloDesiredZoomLevel() : this->GetDesiredZoomLevel_();
		}
	};

	namespace Objects { namespace Units {
#define __EL_INCLUDE_ID			__EL_INCLUDE_OBJECTS
#define __EL_INCLUDE_FILE_ID	__EL_OBJECTS_UNITS
#include "Memory/_EngineLayout.inl"

		void Initialize()
		{
		}

#include "Objects/Units.GrenadeCounts.inl"
		void InitializeForYeloGameState(bool enabled)
		{
			InitializeForYeloGameState_UnitZoomLevelRefs(enabled);
			InitializeForYeloGameState_UnitDesiredZoomLevelRefs(enabled);
			InitializeForYeloGameState_NumberOfUnitGrenadeTypes(enabled);
			InitializeForYeloGameState_UnitGrenadeCounts(enabled);
		}

	}; };
};