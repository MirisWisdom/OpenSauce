/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/ai/actions.hpp>
#include <blamlib/Halo1/ai/actor_types.hpp>
#include <blamlib/Halo1/ai/ai.hpp>
#include <blamlib/Halo1/ai/ai_communication.hpp>
#include <blamlib/Halo1/ai/encounters.hpp>
#include <blamlib/Halo1/ai/props.hpp>
#include <blamlib/Halo1/memory/data.hpp>

namespace Yelo
{
	namespace AI
	{
		encounter_data_t&				Encounters();
		squads_data_t&					Squads();
		platoons_data_t&				Platoons();
		ai_pursuit_data_t&				Pursuits();

		//ai_communication_dialogue_events_t*	AICommunicationDialogue();
		ai_communication_reply_events_t&	AICommunicationReplies();
		ai_conversation_data_t&				AIConversations();



		void Initialize();
		void Dispose();
		void PLATFORM_API Update();
	};
};