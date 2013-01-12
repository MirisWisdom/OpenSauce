/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Game/AI.hpp"
#include "Objects/Objects.hpp"
#include "Objects/Units.hpp"
#include "TagGroups/TagGroups.hpp"

#include "TagGroups/project_yellow_definitions.hpp"

#include "Memory/MemoryInterface.hpp"

namespace Yelo
{
	namespace AI
	{
#define __EL_INCLUDE_ID			__EL_INCLUDE_GAME
#define __EL_INCLUDE_FILE_ID	__EL_GAME_AI
#include "Memory/_EngineLayout.inl"
		
		s_ai_globals_data* AIGlobals()								DPTR_IMP_GET(ai_globals);
		t_actor_data* Actors()										DPTR_IMP_GET(actors);
		t_swarm_data* Swarms()										DPTR_IMP_GET(swarms);
		t_swarm_component_data* SwarmComponents()					DPTR_IMP_GET(swarm_components);
		t_prop_data* Props()										DPTR_IMP_GET(props);
		t_encounter_data* Encounters()								DPTR_IMP_GET(encounters);
		s_squad_data* Squads()										DPTR_IMP_GET(squads);
		s_platoon_data* Platoons()									DPTR_IMP_GET(platoons);
		t_ai_pursuit_data* AIPursuits()								DPTR_IMP_GET(ai_pursuits);

		//ai_communication_dialogue_data

		s_ai_communication_replies_data* AICommunicationReplies()	DPTR_IMP_GET(ai_communication_replies);
		t_ai_conversation_data* AIConversations()					DPTR_IMP_GET(ai_conversations);


		void Initialize()
		{
#if !PLATFORM_DISABLE_UNUSED_CODE
			Memory::CreateHookRelativeCall(&AI::Update, GET_FUNC_VPTR(AI_UPDATE_HOOK), Enums::_x86_opcode_retn);
#endif
            static const byte k_null_bytes[7] = {
                    Enums::_x86_opcode_nop, Enums::_x86_opcode_nop,
                    Enums::_x86_opcode_nop, Enums::_x86_opcode_nop,
                    Enums::_x86_opcode_nop, Enums::_x86_opcode_nop,
                    Enums::_x86_opcode_nop
            };

            Memory::WriteMemory(GET_FUNC_VPTR(ACTOR_ACTION_HANDLE_VEHICLE_EXIT_HOOK), k_null_bytes, sizeof(k_null_bytes));
			Memory::WriteRelativeCall(&ActorActionHandleVehicleExitHook, GET_FUNC_VPTR(ACTOR_ACTION_HANDLE_VEHICLE_EXIT_HOOK), true);
		}

		void Dispose()
		{
		}

		void PLATFORM_API Update()
		{
		}

		API_FUNC_NAKED void PLATFORM_API ActorActionHandleVehicleExitHook()
		{
			__asm {
				pushad
				push	ecx		// datum_index unit_index
				call	ActorActionHandleVehicleExitBoardingSeat
				popad

				retn
			}
		}

		void ActorActionHandleVehicleExitBoardingSeat(datum_index unit_index)
		{
			Objects::s_unit_datum* unit = (*Objects::ObjectHeader())[unit_index]->_unit;

			datum_index parent_unit_index = unit->object.parent_object_index;
			int16 seat_index = unit->unit.vehicle_seat_index;

			TagGroups::s_unit_external_upgrades const* unit_upgrades_definition = 
				Objects::Units::DefinitionFindUnitUpgradesBlock(parent_unit_index);

			// Check if a unit upgrades definition exists for the vehicle the actor is in
			if (unit_upgrades_definition != NULL)
				return;
			// If a unit upgrades definition does not exist, exit the vehicle like normal
			else
				*unit->unit.animation.GetAnimationState() = Enums::_unit_animation_state_seat_exit;
		}
	};
};