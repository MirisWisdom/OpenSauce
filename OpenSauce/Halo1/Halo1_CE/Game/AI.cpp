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
#include <blamlib/Halo1/objects/objects.hpp>
#include <blamlib/Halo1/objects/damage.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_global_definitions.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_scenario.hpp>
#include <YeloLib/Halo1/units/units_yelo.hpp>

#include "Memory/MemoryInterface.hpp"
#include "Objects/Objects.hpp"
#include "Objects/Units.hpp"

#include "Game/AI.Transform.inl"

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
			auto unit = blam::object_get_and_verify_type<Objects::s_unit_datum>(unit_index);

			datum_index parent_unit_index = unit->object.parent_object_index;
			auto parent_unit = blam::object_get_and_verify_type<Objects::s_unit_datum>(parent_unit_index);
			int16 seat_index = unit->unit.vehicle_seat_index;

			const auto* unit_definition = blam::tag_get<TagGroups::s_unit_definition>(parent_unit->object.definition_index);

			// TODO: Add tag data to get bidirectional references?
			// Get the boarding seat definition for the current seat
			int16 boarding_seat_index = 0;
			const TagGroups::unit_seat_boarding* boarding_seat = nullptr;
			for(auto& seat : unit_definition->unit.seats)
			{
				if(seat.HasBoardingTargetSeat() && (seat.GetSeatBoarding().target_seat_index == seat_index))
				{
					boarding_seat = &seat.GetSeatBoarding();
					break;
				}

				boarding_seat_index++;
			}

			// Exit the vehicle like normal if the seat can't be boarded
			if(!boarding_seat)
			{
				unit->unit.animation.state = Enums::_unit_animation_state_seat_exit;
				return;
			}

			// Check if the seat the actor is in is being boarded. If the seat is being boarded, prevent ai from exiting.
			datum_index boarding_unit = Objects::GetUnitInSeat(parent_unit_index, boarding_seat_index);
			if (!boarding_unit.IsNull())
			{
				return;
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
		
		void InitializeForNewMap()
		{
			Transform::InitializeForNewMap();
		}

		void DisposeFromOldMap()
		{
			Transform::DisposeFromOldMap();
		}

		void PLATFORM_API Update()
		{
		}

		void ObjectsUpdate()
		{
			Objects::c_object_iterator iter(Enums::_object_type_mask_unit);

			for(auto object_index : iter)
			{
				Transform::UnitUpdate(object_index.index);
			}
		}

		void UnitDamageAftermath(const datum_index object_index, const Objects::s_damage_data* damage_data)
		{
			Transform::UnitDamaged(object_index, damage_data);
		}
	};
};