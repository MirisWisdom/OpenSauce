/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#include "Common/Precompile.hpp"
#include "AI/AI.hpp"

#include "Memory/Runtime.hpp"

namespace Yelo
{
#define __EL_INCLUDE_ID			__EL_INCLUDE_AI
#define __EL_INCLUDE_FILE_ID	__EL_AI_AI
#include "Memory/_EngineLayout.inl"

	namespace AI
	{
		void Initialize()
		{
			YELO_MEM_WLIST_BEGIN()
				// Allow AI code to execute in MP games
				*CAST_PTR(void**, GET_DATA_PTR(AI_INITIALIZE_FOR_NEW_MAP_MOD)) =
					CAST_PTR(void*, GET_FUNC_PTR(AI_INITIALIZE_FOR_NEW_MAP) + 22);
			YELO_MEM_WLIST_END();
		}

		void Dispose()
		{
			YELO_MEM_WLIST_BEGIN()
				*CAST_PTR(void**, GET_DATA_PTR(AI_INITIALIZE_FOR_NEW_MAP_MOD)) =
					CAST_PTR(void*, GET_FUNC_PTR(AI_INITIALIZE_FOR_NEW_MAP));
			YELO_MEM_WLIST_END();
		}

		void Update()
		{
		}
	};

	namespace GameState
	{
		s_ai_globals* _AiGlobals()									DPTR_IMP_GET(_AiGlobals);
		t_actor_data* _Actors()										DPTR_IMP_GET(_Actors);
		s_ai_reference_frame_data* _AiReferenceFrameData()			DPTR_IMP_GET(_AiReferenceFrameData);
		s_actor_firing_position_owners* _ActorFiringPositionOwners()DPTR_IMP_GET(_ActorFiringPositionOwners);
		t_swarm_data* _Swarms()										DPTR_IMP_GET(_Swarms);
		t_prop_data* _Props()										DPTR_IMP_GET(_Props);
		t_prop_ref_data* _PropRefs()								DPTR_IMP_GET(_PropRefs);
		t_tracking_data* _Tracking()								DPTR_IMP_GET(_Tracking);
		t_squad_data* _Squads()										DPTR_IMP_GET(_Squads);
		t_squad_group_data* _SquadGroups()							DPTR_IMP_GET(_SquadGroups);
		t_clump_data* _Clumps()										DPTR_IMP_GET(_Clumps);
		t_joint_state_data* _JointStates()							DPTR_IMP_GET(_JointStates);
		t_dynamic_firing_points_data* _DynamicFiringPoints()		DPTR_IMP_GET(_DynamicFiringPoints);
		t_command_scripts_data* _CommandScripts()					DPTR_IMP_GET(_CommandScripts);
		t_joint_command_scripts_data* _JointCommandScripts()		DPTR_IMP_GET(_JointCommandScripts);
		s_ai_dialogue_globals* _AiDialogueGlobals()					DPTR_IMP_GET(_AiDialogueGlobals);
		t_vocalization_records_data* _VocalizationRecords()			DPTR_IMP_GET(_VocalizationRecords);
		s_ai_orders_data* _AiOrdersData()							DPTR_IMP_GET(_AiOrdersData);
		t_flocks_data* _Flocks()									DPTR_IMP_GET(_Flocks);
		s_gravemind_globals* _GravemindGlobals()					DPTR_IMP_GET(_GravemindGlobals);
	};
};