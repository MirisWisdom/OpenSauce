/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/

//////////////////////////////////////////////////////////////////////////
// AI.cpp
#if __EL_INCLUDE_FILE_ID == __EL_AI_AI

namespace AI
{
	DATA_PTR(AI_INITIALIZE_FOR_NEW_MAP_MOD,	NULL);
	FUNC_PTR(AI_INITIALIZE_FOR_NEW_MAP,		NULL);
};

namespace GameState
{
	ENGINE_DPTR(s_ai_globals, _AiGlobals,									NULL);
	ENGINE_DPTR(t_actor_data, _Actors,										NULL);
	ENGINE_DPTR(s_ai_reference_frame_data, _AiReferenceFrameData,			NULL);
	ENGINE_DPTR(s_actor_firing_position_owners, _ActorFiringPositionOwners,	NULL);
	ENGINE_DPTR(t_swarm_data, _Swarms,										NULL);
	ENGINE_DPTR(t_prop_data, _Props,										NULL);
	ENGINE_DPTR(t_prop_ref_data, _PropRefs,									NULL);
	ENGINE_DPTR(t_tracking_data, _Tracking,									NULL);
	ENGINE_DPTR(t_squad_data, _Squads,										NULL);
	ENGINE_DPTR(t_squad_group_data, _SquadGroups,							NULL);
	ENGINE_DPTR(t_clump_data, _Clumps,										NULL);
	ENGINE_DPTR(t_joint_state_data, _JointStates,							NULL);
	ENGINE_DPTR(t_dynamic_firing_points_data, _DynamicFiringPoints,			NULL);
	ENGINE_DPTR(t_command_scripts_data, _CommandScripts,					NULL);
	ENGINE_DPTR(t_joint_command_scripts_data, _JointCommandScripts,			NULL);
	ENGINE_DPTR(s_ai_dialogue_globals, _AiDialogueGlobals,					NULL);
	ENGINE_DPTR(t_vocalization_records_data, _VocalizationRecords,			NULL);
	ENGINE_DPTR(s_ai_orders_data, _AiOrdersData,							NULL);
	ENGINE_DPTR(t_flocks_data, _Flocks,										NULL);
	ENGINE_DPTR(s_gravemind_globals, _GravemindGlobals,						NULL);
};

#else
	#error Undefined engine layout include for: __EL_INCLUDE_AI
#endif