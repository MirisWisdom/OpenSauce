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
	DATA_PTR(AI_INITIALIZE_FOR_NEW_MAP_MOD,	0x441608);
	FUNC_PTR(AI_INITIALIZE_FOR_NEW_MAP,		0x1C79E0);
};

namespace GameState
{
	ENGINE_DPTR(s_ai_globals, _AiGlobals,									0x4F55D0);
	ENGINE_DPTR(t_actor_data, _Actors,										0x4F55F0);
	ENGINE_DPTR(s_ai_reference_frame_data, _AiReferenceFrameData,			0x4F93A0);
	ENGINE_DPTR(s_actor_firing_position_owners, _ActorFiringPositionOwners,	0x557C6C);
	ENGINE_DPTR(t_swarm_data, _Swarms,										0x5044C8);
	ENGINE_DPTR(t_prop_data, _Props,										0x50241C);
	ENGINE_DPTR(t_prop_ref_data, _PropRefs,									0x502418);
	ENGINE_DPTR(t_tracking_data, _Tracking,									0x502414);
	ENGINE_DPTR(t_squad_data, _Squads,										0x51E9D8);
	ENGINE_DPTR(t_squad_group_data, _SquadGroups,							0x51E9DC);
	ENGINE_DPTR(t_clump_data, _Clumps,										0x502420);
	ENGINE_DPTR(t_joint_state_data, _JointStates,							0x502424);
	ENGINE_DPTR(t_dynamic_firing_points_data, _DynamicFiringPoints,			0x51ECA4);
	ENGINE_DPTR(t_command_scripts_data, _CommandScripts,					0x502408);
	ENGINE_DPTR(t_joint_command_scripts_data, _JointCommandScripts,			0x502404);
	ENGINE_DPTR(s_ai_dialogue_globals, _AiDialogueGlobals,					0x4F939C);
	ENGINE_DPTR(t_vocalization_records_data, _VocalizationRecords,			0x4F9398);
	ENGINE_DPTR(s_ai_orders_data, _AiOrdersData,							0x5044CC);
	ENGINE_DPTR(t_flocks_data, _Flocks,										0x51ECB4);
	ENGINE_DPTR(s_gravemind_globals, _GravemindGlobals,						0x5047F4);
};

#else
	#error Undefined engine layout include for: __EL_INCLUDE_AI
#endif