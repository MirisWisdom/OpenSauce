/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/ai/actions.hpp>
#include <blamlib/ai/ai_communication.hpp>
#include <blamlib/ai/encounters.hpp>

namespace Yelo
{
	namespace Objects
	{
		struct s_damage_data;
	};

	namespace AI
	{
		encounter_data_t& Encounters();
		squads_data_t& Squads();
		platoons_data_t& Platoons();
		ai_pursuit_data_t& Pursuits();

		//ai_communication_dialogue_events_t*	AICommunicationDialogue();
		ai_communication_reply_events_t& AICommunicationReplies();
		ai_conversation_data_t& AIConversations();

		void Initialize();
		void Dispose();

		void InitializeForNewGameState();

		void InitializeForNewMap();
		void DisposeFromOldMap();

		void PLATFORM_API Update();

		void HandleGameStateLifeCycle(
			_enum life_state);

		void ObjectsUpdate();
		void UnitDamageAftermath(
			const datum_index object_index,
			const Objects::s_damage_data* const damage_data);
	};
};
