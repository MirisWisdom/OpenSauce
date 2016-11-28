/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#pragma once

#include <blamlib/Halo2/memory/data.hpp>

namespace Yelo
{
	namespace AI
	{
		struct bit_vector
		{
			PAD_TYPE(int16); PAD_TYPE(int16);
			PAD_TYPE(int16); PAD_TYPE(int16);
			PAD_TYPE(int32);
			PAD_TYPE(int32);
			PAD_TYPE(int32);
		};

		struct ai_behavior
		{
			_enum Index; // index in the behavior list
			_enum Type; // ai_behavior_type
			PAD_TYPE(long_flags); PAD_TYPE(long_flags);
			PAD_TYPE(int32);
		};

		struct ai_behavior_type0 : public ai_behavior
		{
			void* proc;
		};

		struct ai_behavior_type1 : public ai_behavior
		{
			void* UNKNOWN(proc1)[4];
			int32 UNKNOWN(0)[8];
			void* UNKNOWN(proc2);

			struct _BitVector {
				int32 Count;
				bit_vector* Vector;
			}BitVector;
		};

		struct ai_behavior_type2 : public ai_behavior
		{
		};

		void Initialize();
		void Dispose();
		void Update();
	};

	namespace GameState
	{
		struct s_ai_globals : TStructImpl(884)
		{
			TStructGetPtrImpl(bool, Enabled, 0x0);
		};
		s_ai_globals* _AiGlobals();

		struct s_actor_datum : TStructImpl(2184)
		{
		};
		typedef Memory::DataArray<s_actor_datum, 256> t_actor_data;
		t_actor_data* _Actors();

		struct s_ai_reference_frame_data : TStructImpl(1600)
		{
		};
		s_ai_reference_frame_data* _AiReferenceFrameData();

		struct s_actor_firing_position_owners : TStructImpl(8252)
		{
		};
		s_actor_firing_position_owners* _ActorFiringPositionOwners();

		struct s_swarm_datum : TStructImpl(52)
		{
		};
		typedef Memory::DataArray<s_swarm_datum, 32> t_swarm_data;
		t_swarm_data* _Swarms();

		struct s_prop_datum : TStructImpl(196)
		{
		};
		typedef Memory::DataArray<s_prop_datum, 256> t_prop_data;
		t_prop_data* _Props();

		struct s_prop_ref_datum : TStructImpl(60)
		{
		};
		typedef Memory::DataArray<s_prop_ref_datum, 1024> t_prop_ref_data;
		t_prop_ref_data* _PropRefs();

		struct s_tracking_datum : TStructImpl(292)
		{
		};
		typedef Memory::DataArray<s_tracking_datum, 100> t_tracking_data;
		t_tracking_data* _Tracking();

		struct s_squad_datum : TStructImpl(152)
		{
		};
		typedef Memory::DataArray<s_squad_datum, 335> t_squad_data;
		t_squad_data* _Squads();

		struct s_squad_group_datum : TStructImpl(56)
		{
		};
		typedef Memory::DataArray<s_squad_group_datum, 100> t_squad_group_data;
		t_squad_group_data* _SquadGroups();

		struct s_clump_datum : TStructImpl(80)
		{
		};
		typedef Memory::DataArray<s_clump_datum, 20> t_clump_data;
		t_clump_data* _Clumps();

		struct s_joint_state_datum : TStructImpl(188)
		{
		};
		typedef Memory::DataArray<s_joint_state_datum, 20> t_joint_state_data;
		t_joint_state_data* _JointStates();

		struct s_dynamic_firing_points_datum : TStructImpl(1156)
		{
		};
		typedef Memory::DataArray<s_dynamic_firing_points_datum, 15> t_dynamic_firing_points_data;
		t_dynamic_firing_points_data* _DynamicFiringPoints();

		struct s_command_scripts_datum : TStructImpl(212)
		{
		};
		typedef Memory::DataArray<s_command_scripts_datum, 40> t_command_scripts_data;
		t_command_scripts_data* _CommandScripts();

		struct s_joint_command_scripts_datum : TStructImpl(140)
		{
		};
		typedef Memory::DataArray<s_joint_command_scripts_datum, 10> t_joint_command_scripts_data;
		t_joint_command_scripts_data* _JointCommandScripts();

		struct s_ai_dialogue_globals : TStructImpl(4024)
		{
		};
		s_ai_dialogue_globals* _AiDialogueGlobals();

		struct s_vocalization_records_datum : TStructImpl(84)
		{
		};
		typedef Memory::DataArray<s_vocalization_records_datum, 15> t_vocalization_records_data;
		t_vocalization_records_data* _VocalizationRecords();

		struct s_ai_orders_data : TStructImpl(16)
		{
		};
		s_ai_orders_data* _AiOrdersData();

		struct s_flocks_datum : TStructImpl(40)
		{
		};
		typedef Memory::DataArray<s_flocks_datum, 10> t_flocks_data;
		t_flocks_data* _Flocks();

		struct s_gravemind_globals : TStructImpl(36)
		{
		};
		s_gravemind_globals* _GravemindGlobals();
	};
};