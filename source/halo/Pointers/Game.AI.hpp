/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI
#include "Pointers/1.10/HaloCE_110_Runtime.Game.inl"
#include "Pointers/1.10/HaloCE_110_Runtime_Manual.Game.inl"
#else
#include "Pointers/1.10/HaloCE_110_Dedi.Game.inl"
#include "Pointers/1.10/HaloCE_110_Dedi_Manual.Game.inl"
#endif

#include <blam/ai/actor_structures.hpp>
#include <blam/ai/ai_communication.hpp>
#include <blam/ai/ai_structures.hpp>
#include <blam/ai/encounters.hpp>
#include <blam/ai/prop_structures.hpp>
#include <Memory/MemoryInterface.hpp>
#include <yelo/memory/memory_interface_base.hpp>

namespace Yelo
{
	namespace AI
	{
		ENGINE_DPTR(s_ai_globals_data, ai_globals, DUO_PTR(K_AI_GLOBALS));
		ENGINE_DPTR(actor_data_t, actors, DUO_PTR(K_ACTORS));
		ENGINE_DPTR(swarm_data_t, swarms, DUO_PTR(K_SWARMS));
		ENGINE_DPTR(swarm_component_data_t, swarm_components, DUO_PTR(K_SWARM_COMPONENTS));
		ENGINE_DPTR(prop_data_t, props, DUO_PTR(K_PROPS));
		ENGINE_DPTR(encounter_data_t, encounters, DUO_PTR(K_ENCOUNTERS));
		ENGINE_DPTR(squads_data_t, squads, DUO_PTR(K_SQUADS));
		ENGINE_DPTR(platoons_data_t, platoons, DUO_PTR(K_PLATOONS));
		ENGINE_DPTR(ai_pursuit_data_t, ai_pursuits, DUO_PTR(K_AI_PURSUITS));
		//ai_communication_dialogue_events_t
		ENGINE_DPTR(ai_communication_reply_events_t, ai_communication_replies, DUO_PTR(K_AI_COMMUNICATION_REPLIES));
		ENGINE_DPTR(ai_conversation_data_t, ai_conversations, DUO_PTR(K_AI_CONVERSATIONS));

		FUNC_PTR(AI_UPDATE_HOOK, DUO_PTR(K_AI_UPDATE_HOOK));
		FUNC_PTR(ACTOR_ACTION_HANDLE_VEHICLE_EXIT_HOOK, DUO_PTR(K_ACTOR_ACTION_HANDLE_VEHICLE_EXIT_HOOK));
		FUNC_PTR(ACTOR_ACTION_HANDLE_VEHICLE_EXIT_RETN, DUO_PTR(K_ACTOR_ACTION_HANDLE_VEHICLE_EXIT_RETN));
		FUNC_PTR(PROP_STATUS_REFRESH_HOOK, DUO_PTR(K_PROP_STATUS_REFRESH_HOOK));
		FUNC_PTR(PROP_STATUS_REFRESH_RETN, DUO_PTR(K_PROP_STATUS_REFRESH_RETN));
		FUNC_PTR(ACTOR_INPUT_UPDATE_HOOK, DUO_PTR(K_ACTOR_INPUT_UPDATE_HOOK));
		FUNC_PTR(ACTOR_INPUT_UPDATE_RETN, DUO_PTR(K_ACTOR_INPUT_UPDATE_RETN));
	}
}
