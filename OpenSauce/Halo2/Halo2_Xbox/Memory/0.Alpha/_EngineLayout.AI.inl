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
	DATA_PTR(AI_INITIALIZE_FOR_NEW_MAP_MOD,	0x4AACD0);
	FUNC_PTR(AI_INITIALIZE_FOR_NEW_MAP,		0x127980);
};

namespace GameState
{
	ENGINE_DPTR(s_ai_globals, _AiGlobals,									0x660CB4);
	ENGINE_DPTR(t_actor_data, _Actors,										0x6B0D38);
	ENGINE_DPTR(s_ai_reference_frame_data, _AiReferenceFrameData,			0x6BAF0C);
	ENGINE_DPTR(s_actor_firing_position_owners, _ActorFiringPositionOwners,	0x74CAC8);
	ENGINE_DPTR(t_swarm_data, _Swarms,										0x6C7FD8);
	ENGINE_DPTR(t_prop_data, _Props,										0x6C7FE4);
	ENGINE_DPTR(t_prop_ref_data, _PropRefs,									0x6C7FE0);
	ENGINE_DPTR(t_tracking_data, _Tracking,									0x6C7FDC);
	ENGINE_DPTR(t_squad_data, _Squads,										0x6D9E5C);
	ENGINE_DPTR(t_squad_group_data, _SquadGroups,							0x6D9E60);
	ENGINE_DPTR(t_clump_data, _Clumps,										0x6C7FEC);
	ENGINE_DPTR(t_joint_state_data, _JointStates,							0x6C7FF0);
	ENGINE_DPTR(t_dynamic_firing_points_data, _DynamicFiringPoints,			0x6DA6C8);
	ENGINE_DPTR(t_command_scripts_data, _CommandScripts,					0x6C8000);
	ENGINE_DPTR(t_joint_command_scripts_data, _JointCommandScripts,			0x6C7FFC);
	ENGINE_DPTR(s_ai_dialogue_globals, _AiDialogueGlobals,					0x6BAF08);
	ENGINE_DPTR(t_vocalization_records_data, _VocalizationRecords,			0x6BAF04);
	ENGINE_DPTR(s_ai_orders_data, _AiOrdersData,							0x6C7FE8);
	ENGINE_DPTR(t_flocks_data, _Flocks,										0x6DA6CC);
	ENGINE_DPTR(s_gravemind_globals, _GravemindGlobals,						0x6C8334);
};

#else
	#error Undefined engine layout include for: __EL_INCLUDE_AI
#endif