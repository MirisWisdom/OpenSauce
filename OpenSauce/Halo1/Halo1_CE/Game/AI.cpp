/*
    Yelo: Open Sauce SDK
		Halo 1 (CE) Edition
    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
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