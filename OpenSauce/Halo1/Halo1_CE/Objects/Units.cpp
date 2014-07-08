/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Objects/Units.hpp"

#include <blamlib/Halo1/game/game_globals.hpp>
#include <blamlib/Halo1/game/game_globals_definitions.hpp>
#include <blamlib/Halo1/saved_games/game_state_structures.hpp>
#include <blamlib/Halo1/units/unit_definitions.hpp>

#include <blamlib/Halo1/units/biped_structures.hpp>
#include <blamlib/Halo1/units/vehicle_structures.hpp>
#include <blamlib/Halo1/units/unit_structures.hpp>

#include <YeloLib/Halo1/open_sauce/project_yellow_global_cv_definitions.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_scenario.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_scenario_definitions.hpp>

#include "Game/Camera.hpp"
#include "Game/EngineFunctions.hpp"
#include "Game/GameState.hpp"
#include "Memory/MemoryInterface.hpp"

#include "Objects/Objects.hpp"

namespace Yelo
{
	namespace Objects { namespace Units {
#define __EL_INCLUDE_ID			__EL_INCLUDE_OBJECTS
#define __EL_INCLUDE_FILE_ID	__EL_OBJECTS_UNITS
#include "Memory/_EngineLayout.inl"
	}; };
};

#include "Objects/Units.Boarding.inl"
#include "Objects/Units.Animations.inl"
#include "Objects/Units.UnitInfections.inl"
#include "Objects/Units.GrenadeCounts.inl"

namespace Yelo
{
	namespace Objects { namespace Units {

		void Initialize()
		{
			Animations::Initialize();
			Infections::Initialize();
			Boarding::Initialize();
			
			static const byte opcode_null[] = { 
				Enums::_x86_opcode_nop, Enums::_x86_opcode_nop, Enums::_x86_opcode_nop, 
				Enums::_x86_opcode_nop, Enums::_x86_opcode_nop, Enums::_x86_opcode_nop 
			};

			Memory::WriteMemory(GET_FUNC_VPTR(BIPED_UPDATE_CHECK_PARENT_UNIT_TYPE), opcode_null);
		}

		void Dispose()
		{
			Infections::Dispose();
		}

		static void InitializeUnitGrenadeTypesForNewMap(GameState::s_yelo_header_data& yelo_header)
		{
			static bool g_already_enabled = false;

			if (!yelo_header.flags.update_unit_grenade_types_count)
				return;

			uint32 count = yelo_header.unit_grenade_types_count;

			bool enabled = count > Enums::k_unit_grenade_types_count;
			// HACK: if the grenade count changes are already in place, don't repeat or undo them
			// There was a problem with clients getting disconnected from the server during a join attempt.
			// It puts you back at the mainmenu, but when you try to join again, this would executed again
			// without the changes from last time being undo'd, resulting in a ASSERT() failing on code bytes mismatch
			if (enabled != g_already_enabled)
			{
				InitializeGrenadeCounts_UnitZoomLevelRefs(enabled);
				InitializeGrenadeCounts_UnitDesiredZoomLevelRefs(enabled);
				InitializeGrenadeCounts_NumberOfUnitGrenadeTypes(count);
				InitializeGrenadeCounts_UnitGrenadeCounts(enabled);
				InitializeGrenadeCounts_MessageDeltaGrenadeCounts(enabled);

				g_already_enabled = enabled;
			}

			yelo_header.flags.update_unit_grenade_types_count = false;
		}
		void InitializeForNewMap()
		{
			if(GameState::YeloGameStateEnabled())
			{
				GameState::s_yelo_header_data& yelo_header = GameState::GameStateGlobals()->header->yelo;

				// handle the grenade types upgrading, if needed
				InitializeUnitGrenadeTypesForNewMap(yelo_header);
			}
		}

		void DisposeFromOldMap()
		{
		}

		void InitializeForNewMapPrologue()
		{
			const TagGroups::s_game_globals* game_globals = GameState::GlobalGameGlobals();
			GameState::s_yelo_header_data& yelo_header = GameState::GameStateGlobals()->header->yelo;

			// check this map's grenade count vs the previous map's
			if (game_globals->grenades.Count != yelo_header.unit_grenade_types_count)
			{
				// inform the unit grenade counts code to run an assembly update
				yelo_header.flags.update_unit_grenade_types_count = true;
				// figure out the new grenade count
				// defaulting to the stock count if there is suspicious number
				if (game_globals->grenades.Count <= Enums::k_unit_grenade_types_count_yelo)
					yelo_header.unit_grenade_types_count = CAST(byte, game_globals->grenades.Count);
				else
					yelo_header.unit_grenade_types_count = Enums::k_unit_grenade_types_count;
			}
		}

		void InitializeForYeloGameState(bool enabled)
		{
		}

	}; };
};