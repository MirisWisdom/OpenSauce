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


		struct s_ai_globals_data : TStructImpl(2268)
		{
		};
		s_ai_globals_data*				AIGlobals();

		struct s_actor_datum : TStructImpl(1828)
		{
			TStructGetPtrImpl(bool,				MetaSwarm, 0x6);

			TStructGetPtrImpl(bool,				MetaEncounterless, 0x9);
			//TStructGetPtrImpl(bool,				Meta, 0xA);

			TStructGetPtrImpl(datum_index,		MetaUnitIndex, 0x18);
			// ?
			TStructGetPtrImpl(int16,			MetaUnitCount, 0x1E);

			TStructGetPtrImpl(datum_index,		MetaSwarmUnitIndex, 0x24);
			TStructGetPtrImpl(datum_index,		MetaSwarmCacheIndex, 0x28);
			//TStructGetPtrImpl(datum_index,		Meta, 0x2C); // an actor index

			TStructGetPtrImpl(datum_index,		MetaEncounterIndex, 0x34);

			TStructGetPtrImpl(int16,			MetaSquadIndex, 0x3A);
			TStructGetPtrImpl(int16,			MetaPlatoonIndex, 0x3C);

			TStructGetPtrImpl(bool,				MetaTimeslice, 0x4C);

			TStructGetPtrImpl(datum_index,		MetaActorDefinition, 0x58);


			//////////////////////////////////////////////////////////////////////////
			// input @ 0x120, sizeof(0xA8)
			
			TStructGetPtrImpl(bool,				InputVehiclePassenger, 0x160);
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