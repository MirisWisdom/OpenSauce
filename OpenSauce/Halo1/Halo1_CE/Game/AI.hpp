/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include "Memory/Data.hpp"

namespace Yelo
{
	namespace Enums
	{
		enum ai_communication_type : _enum
		{
			_ai_communication_type_death,
			_ai_communication_type_killing_spree,
			_ai_communication_type_hurt,
			_ai_communication_type_damage,
			_ai_communication_type_sighted_enemy,
			_ai_communication_type_found_enemy,
			_ai_communication_type_unexpected_enemy,
			_ai_communication_type_found_dead_friend,
			_ai_communication_type_allegiance_changed,
			_ai_communication_type_grenade_throwing,
			_ai_communication_type_grenade_startle,
			_ai_communication_type_grenade_sighted,
			_ai_communication_type_grenade_danger,
			_ai_communication_type_lost_contact,
			_ai_communication_type_blocked,
			_ai_communication_type_alert_noncombat,
			_ai_communication_type_search_start,
			_ai_communication_type_search_query,
			_ai_communication_type_search_report,
			_ai_communication_type_search_abandon,
			_ai_communication_type_search_group_abandon,
			_ai_communication_type_uncover_start,
			_ai_communication_type_advance,
			_ai_communication_type_retreat,
			_ai_communication_type_cover,
			_ai_communication_type_sighted_friend_player,
			_ai_communication_type_shooting,
			_ai_communication_type_shooting_vehicle,
			_ai_communication_type_shooting_berserk,
			_ai_communication_type_shooting_group,
			_ai_communication_type_shooting_traitor,
			_ai_communication_type_flee,
			_ai_communication_type_flee_leader_died,
			_ai_communication_type_flee_idle,
			_ai_communication_type_attempted_flee,
			_ai_communication_type_hiding_finished,
			_ai_communication_type_vehicle_entry,
			_ai_communication_type_vehicle_exit,
			_ai_communication_type_vehicle_woohoo,
			_ai_communication_type_vehicle_scared,
			_ai_communication_type_vehicle_falling,
			_ai_communication_type_surprise,
			_ai_communication_type_berserk,
			_ai_communication_type_melee,
			_ai_communication_type_dive,
			_ai_communication_type_uncover_exclamation,
			_ai_communication_type_falling,
			_ai_communication_type_leap,
			_ai_communication_type_postcombat_alone,
			_ai_communication_type_postcombat_unscathed,
			_ai_communication_type_postcombat_wounded,
			_ai_communication_type_postcombat_massacre,
			_ai_communication_type_postcombat_triumph,
			_ai_communication_type_postcombat_check_enemy,
			_ai_communication_type_postcombat_check_friend,
			_ai_communication_type_postcombat_shoot_corpse,
			_ai_communication_type_postcombat_celebrate,

			_ai_communication_type,
		};

		enum ai_communication_priority
		{
			_ai_communication_priority_none,
			_ai_communication_priority_filler,
			_ai_communication_priority_chatter,
			_ai_communication_priority_talk,
			_ai_communication_priority_communicate,
			_ai_communication_priority_shout,
			_ai_communication_priority_yell,
			_ai_communication_priority_exclaim,

			_ai_communication_priority,
		};

		enum actor_action : _enum
		{
			_actor_action_none,
			_actor_action_sleep,
			_actor_action_alert,
			_actor_action_fight,
			_actor_action_flee,
			_actor_action_uncover,
			_actor_action_guard,
			_actor_action_search,
			_actor_action_wait,
			_actor_action_vehicle,
			_actor_action_charge,
			_actor_action_obey,
			_actor_action_converse,
			_actor_action_avoid,

			NUMBER_OF_ACTOR_ACTIONS
		};

		enum action_class : _enum
		{
			_action_class_passive = 1,
		};

		enum actor_target_type : _enum
		{
			NUMBER_OF_ACTOR_TARGET_TYPES = 12
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

			NUMBER_OF_ACTOR_KNOWLEDGE_TYPES,
		};

		enum actor_perception_type : _enum
		{
			_actor_perception_type_none,
			_actor_perception_type_partial,
			_actor_perception_type_full,
			_actor_perception_type_unmistakable,

			NUMBER_OF_ACTOR_PERCEPTION_TYPES,
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
		struct s_ai_communication_packet
		{
			UNKNOWN_TYPE(int32);
			UNKNOWN_TYPE(int16); // 4
			Enums::ai_communication_type dialogue_type_index; // 6
			UNKNOWN_TYPE(int16); // 8
			PAD16; // A ?

			UNKNOWN_TYPE(int16); // C
			PAD16; // E ?
			PAD32; // 10 ?
			UNKNOWN_TYPE(int16); // 14
			PAD16; // 16 ?
			UNKNOWN_TYPE(int16); // 18
			UNKNOWN_TYPE(int16); // 1A
			bool broken; // 1C false = reformed
			PAD24;
		}; BOOST_STATIC_ASSERT( sizeof(s_ai_communication_packet) == 0x20 );

		typedef void (PLATFORM_API* proc_action_function)(datum_index actor_index);
		typedef void (PLATFORM_API* proc_action_function_2C)(datum_index actor_index, datum_index, datum_index);
		struct s_action_function_definition
		{
			Enums::actor_action action;
			PAD16;
			cstring name;
			const real_argb_color debug_color;
			size_t action_data_size;
			Enums::action_class action_class;
			PAD16;
			proc_action_function begin;
			proc_action_function perform;
			proc_action_function update;
			proc_action_function control;
			proc_action_function end;
			proc_action_function func28;
			proc_action_function_2C func2C;
			proc_action_function func30;
			proc_action_function func34;
			proc_action_function func38;
		};


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

			// 0x4A8, 0x5C byte structure

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

			//TStructGetPtrImpl(datum_index,	control.path.destination_orders.ignore_target_object_index, 0x480);

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
			TStructGetPtrImpl(datum_index,		ControlCurrentFireTargetPropIndex, 0x610);

			TStructGetPtrImpl(real_vector3d,	ControlBurstAimVector, 0x68C);

			TStructGetPtrImpl(word_flags,		ControlDataFlags, 0x6D0); // unit_control_flags
			// PAD16?
			//TStructGetPtrImpl(int16,			Control, 0x6D4);
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