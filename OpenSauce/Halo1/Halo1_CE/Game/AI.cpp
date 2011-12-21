/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Game/AI.hpp"

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
			Memory::CreateHookRelativeCall(&AI::Update, GET_FUNC_VPTR(AI_UPDATE_HOOK), Enums::_x86_opcode_retn);
		}

		void Dispose()
		{
		}

		void PLATFORM_API Update()
		{
		}
	};
};