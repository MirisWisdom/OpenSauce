/*
    Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition
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