/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/ai/actions.hpp>
#include <blamlib/Halo1/ai/actor_types.hpp>
#include <blamlib/Halo1/ai/ai_communication.hpp>
#include <blamlib/Halo1/memory/data.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum actor_default_state
		{
			_actor_default_state_none,
			_actor_default_state_sleeping,
			_actor_default_state_alert,
			_actor_default_state_moving_repeat_same_position,
			_actor_default_state_moving_loop,
			_actor_default_state_moving_loop_back_and_forth,
			_actor_default_state_moving_loop_randomly,
			_actor_default_state_moving_randomly,
			_actor_default_state_guarding,
			_actor_default_state_guarding_at_guard_position,
			_actor_default_state_searching,
			_actor_default_state_fleeing,

			k_number_of_actor_default_states,
		};

		enum actor_target_type : _enum
		{
			k_number_of_actor_target_types = 12
		};

		enum actor_danger_zone : _enum
		{
			_actor_danger_zone_none,
			_actor_danger_zone_suicide,
			_actor_danger_zone_projectile,
			_actor_danger_zone_vehicle,
		};

		enum actor_fire_target : _enum
		{
			_actor_fire_target_prop = 1,
			_actor_fire_target_manual_point = 2,
		};

		enum actor_acknowledgement : _enum
		{
			_actor_acknowledgement_never,
			_actor_acknowledgement_combat,
			_actor_acknowledgement_instant,
			_actor_acknowledgement_searching,
			_actor_acknowledgement_definite,
		};

		enum actor_knowledge_type : _enum
		{
			_actor_knowledge_type_noncombat0,
			_actor_knowledge_type_guard1,
			_actor_knowledge_type_guard2,
			_actor_knowledge_type_noncombat3,

			k_number_of_actor_knowledge_types,
		};

		enum actor_perception_type : _enum
		{
			_actor_perception_type_none,
			_actor_perception_type_partial,
			_actor_perception_type_full,
			_actor_perception_type_unmistakable,

			k_number_of_actor_perception_types,
		};

		enum actor_movement_state : _enum
		{
			_actor_movement_state_noncombat,
			_actor_movement_state_asleep,
			_actor_movement_state_combat,
			_actor_movement_state_flee,
		};
	};

	namespace AI
	{
		struct s_ai_globals_data : TStructImpl(2268)
		{
		};
		s_ai_globals_data*				AIGlobals();

		struct s_actor_datum : TStructImpl(1828)
		{
			TStructGetPtrImpl(bool,				MetaSwarm, 0x6);
			//TStructGetPtrImpl(bool,				Meta, 0x7);

			TStructGetPtrImpl(bool,				MetaEncounterless, 0x9);
			//TStructGetPtrImpl(bool,				Meta, 0xA);

			TStructGetPtrImpl(datum_index,		MetaUnitIndex, 0x18);
			// 0x1C ?
			TStructGetPtrImpl(int16,			MetaSwarmUnitCount, 0x1E); // MAXIMUM_NUMBER_OF_UNITS_PER_SWARM = 16

			TStructGetPtrImpl(datum_index,		MetaSwarmUnitIndex, 0x24);
			TStructGetPtrImpl(datum_index,		MetaSwarmCacheIndex, 0x28);
			//TStructGetPtrImpl(datum_index,		Meta, 0x2C); // an actor index
			//TStructGetPtrImpl(datum_index,		Meta, 0x30);
			TStructGetPtrImpl(datum_index,		MetaEncounterIndex, 0x34);

			TStructGetPtrImpl(int16,			MetaSquadIndex, 0x3A);
			TStructGetPtrImpl(int16,			MetaPlatoonIndex, 0x3C);

			TStructGetPtrImpl(bool,				MetaTimeslice, 0x4C);
			// 0x50 int32
			// 0x54 int32
			TStructGetPtrImpl(datum_index,		MetaActorDefinition, 0x58);
			TStructGetPtrImpl(datum_index,		MetaActorVariantDefinition, 0x5C);

			//TStructGetPtrImpl(_enum,		, 0x6A);
			TStructGetPtrImpl(Enums::actor_action,		StateAction, 0x6C);

			// 0x9C union state.action_data (total size: 0x84)

			TStructGetPtrImpl(bool,				StateActionDataChargeFinishedMeleeAttack, 0xA3);
			TStructGetPtrImpl(bool,				StateActionDataChargeAbortedMeleeAttack, 0xA4);

			TStructGetPtrImpl(bool,				StateActionDataChargeUnableToAdvance, 0xC5);

			//////////////////////////////////////////////////////////////////////////
			// input @ 0x120, sizeof(0xA8)
			
			TStructGetPtrImpl(bool,				InputVehiclePassenger, 0x160);
			//TStructGetPtrImpl(int32,			InputSurfaceIndex, 0x164);
			//TStructGetPtrImpl(real_vector3d,	InputPosition, 0x168);
			TStructGetPtrImpl(real_vector3d,	InputFacingVector, 0x174);
			TStructGetPtrImpl(real_vector3d,	InputAimingVector, 0x180);
			TStructGetPtrImpl(real_vector3d,	InputLookingVector, 0x18C);

			TStructGetPtrImpl(bool,				InputIsBurningToDeath, 0x1B5);
			//PAD16
			TStructGetPtrImpl(real,				InputUnitDamageBody, 0x1B8);
			TStructGetPtrImpl(real,				InputUnitDamageShield, 0x1BC);
			TStructGetPtrImpl(real,				InputUnitDamageBodyRecent, 0x1C0);
			TStructGetPtrImpl(real,				InputUnitDamageShieldRecent, 0x1C4);
			//////////////////////////////////////////////////////////////////////////

			// Not sure if this part of 'target'
			//TStructGetPtrImpl(_enum,			, 0x1E4);
			//TStructGetPtrImpl(datum_index,		, 0x1E8); // prop_index

			TStructGetPtrImpl(Enums::actor_target_type,		TargetTargetType, 0x268);

			TStructGetPtrImpl(datum_index,		TargetTargetPropIndex, 0x270);

			TStructGetPtrImpl(Enums::actor_danger_zone,		DangerZoneDangerType, 0x280);

			TStructGetPtrImpl(bool,				DangerZoneNoticedDanager, 0x287);

			TStructGetPtrImpl(datum_index,		DangerZoneObjectIndex, 0x28C);

			//TStructGetPtrImpl(real,				, 0x294);

			//TStructGetPtrImpl(real_point3d,		, 0x2B0);
			//TStructGetPtrImpl(real_vector3d,	, 0x2BC);

			//TStructGetPtrImpl(real,				, 0x2D8);
			//TStructGetPtrImpl(real_point3d,	, 0x2DC);

			TStructGetPtrImpl(_enum,			StimuliPanicType, 0x308);
			TStructGetPtrImpl(datum_index,		StimuliPanicPropIndex, 0x30C);

			TStructGetPtrImpl(_enum,			StimuliCombatTransition, 0x312);

			TStructGetPtrImpl(datum_index,		StimuliCombatTransitionPropIndex, 0x340);

			// 0x350, 0x68 byte structure
			TStructGetPtrImpl(int16,			FiringPositionsCurrentPositionIndex, 0x3B8);

			//TStructGetPtrImpl(int16,			FiringPositions, 0x3C6); // a count of sorts
			// 0x3C8, array of [4] elements, struct made of 2 int16
			//TStructGetPtrImpl(bool,			FiringPositions, 0x3D8);
			//TStructGetPtrImpl(bool,			FiringPositions, 0x3D9);
			//TStructGetPtrImpl(real_vector3d,	FiringPositions, 0x3DC);

			//////////////////////////////////////////////////////////////////////////
			// control
			struct s_direction_specification
			{
				// 1 = _direction_specification_prop
				_enum type;
				PAD16;
				datum_index prop_index;
				PAD32; PAD32;
			}; BOOST_STATIC_ASSERT( sizeof(s_direction_specification) == 0x10 );

			//TStructGetPtrImpl(_enum,			control.path.destination_orders, 0x46C);
			//TStructGetPtrImpl(int16,			control.path.destination_orders, 0x470);

			//TStructGetPtrImpl(datum_index,	control.path.destination_orders.ignore_target_object_index, 0x480);
			TStructGetPtrImpl(bool,				ControlPathDestinationOrdersComplete, 0x484); // true when at destination

			//TStructGetPtrImpl(int32,			control.path, 0x4A0);

			//TStructGetPtrImpl(,	control.path, 0x4A8); // 0x5C byte structure

			TStructGetPtrImpl(_enum,			ControlSecondaryLookType, 0x544);
			// 0x48 ?
			TStructGetPtrImpl(s_direction_specification,	ControlSecondaryLookDirection, 0x54C);

			TStructGetPtrImpl(bool,				ControlIdleMajorActive, 0x55C);
			// 0x55D ?
			//TStructGetPtrImpl(bool,				Control, 0x55E);
			TStructGetPtrImpl(bool,				ControlIdleMinorActive, 0x55F);
			//TStructGetPtrImpl(int32,	Control, 0x560); // timer. secondary look?
			TStructGetPtrImpl(int32,	ControlIdleMajorTimer, 0x564);
			TStructGetPtrImpl(int32,	ControlIdleMinorTimer, 0x568);
			TStructGetPtrImpl(s_direction_specification,	ControlIdleMajorDirection, 0x56C);
			TStructGetPtrImpl(s_direction_specification,	ControlIdleMinorDirection, 0x57C);

			//TStructGetPtrImpl(bool,				Control, 0x591);
			//PAD24
			//TStructGetPtrImpl(real,				Control, 0x594);

			TStructGetPtrImpl(real_vector3d,	ControlDesiredFacingVector, 0x5A4);
			TStructGetPtrImpl(real_vector3d,	ControlDesiredAimingVector, 0x5B0);
			TStructGetPtrImpl(real_vector3d,	ControlDesiredLookingVector, 0x5BC);
			// 0x5C8, 0x10 byte structure

			TStructGetPtrImpl(_enum,			ControlFireState, 0x5F2);

			TStructGetPtrImpl(Enums::actor_fire_target,		ControlCurrentFireTargetType, 0x60C);
			//////////////////////////////////////////////////////////////////////////
			// _actor_fire_target_prop
			TStructGetPtrImpl(datum_index,		ControlCurrentFireTargetPropIndex, 0x610);
			//////////////////////////////////////////////////////////////////////////
			// _actor_fire_target_manual_point
			//TStructGetPtrImpl(real_point3d,		ControlCurrentFireTargetManualPoint?, 0x610);
			//TStructGetPtrImpl(int32,				Control, 0x61C);

			//TStructGetPtrImpl(bool,				Control, 0x621);
			//TStructGetPtrImpl(bool,				Control, 0x622);
			//TStructGetPtrImpl(bool,				Control, 0x623);
			//TStructGetPtrImpl(sbyte,				Control, 0x624);
			//TStructGetPtrImpl(int16,				Control, 0x626);

			//TStructGetPtrImpl(real_vector3d,	Control, 0x62C);

			//TStructGetPtrImpl(real,				Control, 0x638);

			TStructGetPtrImpl(real_vector3d,	ControlBurstAimVector, 0x68C);

			TStructGetPtrImpl(word_flags,		ControlDataFlags, 0x6D0); // unit_control_flags
			// PAD16?
			TStructGetPtrImpl(int16,			ControlPersistentControlTicks, 0x6D4);
			// PAD16?
			TStructGetPtrImpl(long_flags,		ControlPersistentControlFlags, 0x6D8);

			TStructGetPtrImpl(Enums::actor_movement_state,			ControlMovementState, 0x6DC);
			TStructGetPtrImpl(real_vector3d,	ControlDataThrottle, 0x6E0);

			//TStructGetPtrImpl(int16,			ControlAnimation, 0x6EC);
			TStructGetPtrImpl(real_vector2d,	ControlAnimationFacing, 0x6F0);
			TStructGetPtrImpl(_enum,			ControlDataAimingType, 0x6F8); // 0=alert, 1=casual, casted to byte then set to ctrl_data's aiming_speed
			// PAD24
			TStructGetPtrImpl(real_vector3d,	ControlDataFacingVector, 0x6FC);
			TStructGetPtrImpl(real_vector3d,	ControlDataAimingVector, 0x708);
			TStructGetPtrImpl(real_vector3d,	ControlDataLookingVector, 0x714);
			TStructGetPtrImpl(real,				ControlDataPrimaryTrigger, 0x720);
		};
		typedef Memory::DataArray<s_actor_datum, 256> t_actor_data;
		t_actor_data*					Actors();


		struct s_swarm_datum : TStructImpl(152)
		{
			TStructGetPtrImpl(int16,				UnitCount, 0x2);
			TStructGetPtrImpl(datum_index,			ActorIndex, 0x4);
			TStructGetPtrImpl(datum_index,			UnitIndices, 0x18); // [16]
		};
		typedef Memory::DataArray<s_swarm_datum, 32> t_swarm_data;
		t_swarm_data*					Swarms();


		struct s_swarm_component_datum : TStructImpl(64)
		{
		};
		typedef Memory::DataArray<s_swarm_component_datum, 256> t_swarm_component_data;
		t_swarm_component_data*			SwarmComponents();


		struct s_prop_datum : TStructImpl(312)
		{
			TStructGetPtrImpl(datum_index,		OwnerActorIndex, 0x4);
			TStructGetPtrImpl(datum_index,		NextPropIndex, 0x4);
			TStructGetPtrImpl(datum_index,		ParentPropIndex, 0xC); // guess these are union'd?
			TStructGetPtrImpl(datum_index,		OrphanPropIndex, 0xC);

			//TStructGetPtrImpl(bool,				, 0x14);
			//TStructGetPtrImpl(datum_index,			, 0x18); // unit_index
			//TStructGetPtrImpl(datum_index,			, 0x1C); // actor_index (swarm?)
			TStructGetPtrImpl(_enum,			State, 0x24);

			TStructGetPtrImpl(Enums::actor_perception_type,			Perception, 0x30);

			TStructGetPtrImpl(bool,				Enemy, 0x60);

			TStructGetPtrImpl(int16,			UnopposableCasualtiesInflicted, 0xA6);
			//TStructGetPtrImpl(int16,			, 0xA8);

			TStructGetPtrImpl(bool,				Dead, 0x127);
		};
		typedef Memory::DataArray<s_prop_datum, 768> t_prop_data;
		t_prop_data*					Props();


		struct s_encounter_datum : TStructImpl(108)
		{
		};
		typedef Memory::DataArray<s_encounter_datum, 128> t_encounter_data;
		t_encounter_data*				Encounters();


		struct s_squad_data : TStructImpl(32768)
		{
		};
		s_squad_data*						Squads();

		struct s_platoon_data : TStructImpl(4096)
		{
		};
		s_platoon_data*					Platoons();

		struct s_ai_pursuit_datum : TStructImpl(40)
		{
		};
		typedef Memory::DataArray<s_ai_pursuit_datum, 256> t_ai_pursuit_data;
		t_ai_pursuit_data*				AIPursuits();


		struct s_ai_communication_dialogue_data : TStructImpl(1664)
		{
		};
		//s_ai_communication_dialogue_data

		struct s_ai_communication_replies_data : TStructImpl(720)
		{
		};
		s_ai_communication_replies_data*	AICommunicationReplies();


		struct s_ai_conversation_datum : TStructImpl(100)
		{
		};
		typedef Memory::DataArray<s_ai_conversation_datum, 8> t_ai_conversation_data;
		t_ai_conversation_data*			AIConversations();



		void Initialize();
		void Dispose();
		void PLATFORM_API Update();
	};
};