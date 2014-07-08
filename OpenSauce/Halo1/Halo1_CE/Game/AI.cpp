/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Game/AI.hpp"

#include <blamlib/Halo1/ai/actor_structures.hpp>
#include <blamlib/Halo1/ai/ai_structures.hpp>
#include <blamlib/Halo1/ai/prop_structures.hpp>
#include <blamlib/Halo1/units/unit_structures.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_global_definitions.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_global_cv_definitions.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_scenario.hpp>
#include <YeloLib/Halo1/units/units_yelo.hpp>

#include "Memory/MemoryInterface.hpp"
#include "Objects/Objects.hpp"
#include "Objects/Units.hpp"

namespace Yelo
{
	namespace AI
	{
#define __EL_INCLUDE_ID			__EL_INCLUDE_GAME
#define __EL_INCLUDE_FILE_ID	__EL_GAME_AI
#include "Memory/_EngineLayout.inl"
		
		s_ai_globals_data* AIGlobals()								DPTR_IMP_GET(ai_globals);
		actor_data_t& Actors()										DPTR_IMP_GET_BYREF(actors);
		swarm_data_t& Swarms()										DPTR_IMP_GET_BYREF(swarms);
		swarm_component_data_t& SwarmComponents()					DPTR_IMP_GET_BYREF(swarm_components);
		prop_data_t& Props()										DPTR_IMP_GET_BYREF(props);
		encounter_data_t& Encounters()								DPTR_IMP_GET_BYREF(encounters);
		squads_data_t& Squads()										DPTR_IMP_GET_BYREF(squads);
		platoons_data_t& Platoons()									DPTR_IMP_GET_BYREF(platoons);
		ai_pursuit_data_t& AIPursuits()								DPTR_IMP_GET_BYREF(ai_pursuits);

		//ai_communication_dialogue_events_t
		ai_communication_reply_events_t& AICommunicationReplies()	DPTR_IMP_GET_BYREF(ai_communication_replies);
		ai_conversation_data_t& AIConversations()					DPTR_IMP_GET_BYREF(ai_conversations);


		static void ActorActionHandleVehicleExitBoardingSeat(datum_index unit_index)
		{
			const TagGroups::project_yellow_globals_cv* cv_globals = Scenario::GetYeloCvGlobals();
			auto unit = blam::object_get_and_verify_type<Objects::s_unit_datum>(unit_index);

			// Exit the vehicle like normal if a globals tag doesn't exist
			if(cv_globals == nullptr)
			{
				unit->unit.animation.state = Enums::_unit_animation_state_seat_exit;
				return;
			}

			datum_index parent_unit_index = unit->object.parent_object_index;
			auto parent_unit = blam::object_get_and_verify_type<Objects::s_unit_datum>(parent_unit_index);
			int16 seat_index = unit->unit.vehicle_seat_index;

			TagGroups::s_unit_external_upgrades const* unit_upgrades_definition = 
				cv_globals->FindUnitExternalUpgradeBlock(parent_unit->object.definition_index);

			// Check if a unit upgrades definition exists for the vehicle the actor is in
			if (unit_upgrades_definition != nullptr)
			{
				// Check if the seat the actor is in is being boarded
				for(const auto& boarding_seat : unit_upgrades_definition->boarding_seats)
				{
					int16 boarding_seat_index = boarding_seat.seat_index;
					int16 target_seat_index = boarding_seat.target_seat_index;

					if (seat_index == target_seat_index)
					{
						datum_index boarding_unit = Objects::GetUnitInSeat(parent_unit_index, boarding_seat_index);

						// If the seat is being boarded, prevent ai from exiting
						if (boarding_unit != datum_index::null)
							return;
					}
				}
			}

			// Exit the vehicle like normal if conditions haven't been met
			unit->unit.animation.state = Enums::_unit_animation_state_seat_exit;
		}

		static API_FUNC_NAKED void PLATFORM_API ActorActionHandleVehicleExitHook()
		{
			__asm {
				push	eax
				push	edx
				push	ecx

				push	ecx		// datum_index unit_index
				call	AI::ActorActionHandleVehicleExitBoardingSeat
				
				pop		ecx
				pop		edx
				pop		eax

				retn
			}
		}

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

            Memory::WriteMemory(GET_FUNC_VPTR(ACTOR_ACTION_HANDLE_VEHICLE_EXIT_HOOK), k_null_bytes);
			Memory::WriteRelativeCall(&AI::ActorActionHandleVehicleExitHook, GET_FUNC_VPTR(ACTOR_ACTION_HANDLE_VEHICLE_EXIT_HOOK), true);
		}

		void Dispose()
		{
		}

		void PLATFORM_API Update()
		{
		}
	};
};