/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/cseries/cseries_base.hpp>
#include <YeloLib/cseries/cseries_yelo_base.hpp>
#include <YeloLib/memory/data_yelo.hpp>

namespace Yelo
{
	namespace AI
	{
		namespace e_ai_communication_type
		{
			typedef enum : _enum
			{
				death,
				killing_spree,
				hurt,
				damage,
				sighted_enemy,
				found_enemy,
				unexpected_enemy,
				found_dead_friend,
				allegiance_changed,
				grenade_throwing,
				grenade_startle,
				grenade_sighted,
				grenade_danger,
				lost_contact,
				blocked,
				alert_noncombat,
				search_start,
				search_query,
				search_report,
				search_abandon,
				search_group_abandon,
				uncover_start,
				advance,
				retreat,
				cover,
				sighted_friend_player,
				shooting,
				shooting_vehicle,
				shooting_berserk,
				shooting_group,
				shooting_traitor,
				flee,
				flee_leader_died,
				flee_idle,
				attempted_flee,
				hiding_finished,
				vehicle_entry,
				vehicle_exit,
				vehicle_woohoo,
				vehicle_scared,
				vehicle_falling,
				surprise,
				berserk,
				melee,
				dive,
				uncover_exclamation,
				falling,
				leap,
				postcombat_alone,
				postcombat_unscathed,
				postcombat_wounded,
				postcombat_massacre,
				postcombat_triumph,
				postcombat_check_enemy,
				postcombat_check_friend,
				postcombat_shoot_corpse,
				postcombat_celebrate,

				k_count
			} type_t;
		}

		namespace e_ai_communication_priority
		{
			typedef enum : _enum
			{
				none,
				filler,
				chatter,
				talk,
				communicate,
				shout,
				yell,
				exclaim,

				k_count
			} type_t;
		}

		namespace e_ai_communication_team
		{
			typedef enum : _enum
			{
				unteamed = NONE,
				human = 0,
				covenant,

				k_count
			} type_t;
		}

		namespace e_ai_communication_hostility_type
		{
			typedef enum : _enum
			{
				none,
				self,
				friendly,
				enemy,
				traitor,

				k_count
			} type_t;
		}

		namespace e_ai_communication_status_type
		{
			typedef enum : _enum
			{
				never,
				dead,
				lost,
				not_visual,
				no_danger,
				visual,

				k_count
			} type_t;
		}

		struct s_ai_communication_event : TStructImpl(16) { };

		typedef s_ai_communication_event ai_communication_dialogue_events_t[105];
		typedef s_ai_communication_event ai_communication_reply_events_t[46];

		struct s_ai_conversation_datum : TStructImpl(100) { };

		typedef Memory::DataArray<s_ai_conversation_datum, 8> ai_conversation_data_t;

		struct s_ai_communication_packet
		{
			UNKNOWN_TYPE(int32);
			UNKNOWN_TYPE(int16); // 4
			e_ai_communication_type::type_t dialogue_type_index; // 6
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
		};

		BOOST_STATIC_ASSERT( sizeof(s_ai_communication_packet) == 0x20 );
	}
}
