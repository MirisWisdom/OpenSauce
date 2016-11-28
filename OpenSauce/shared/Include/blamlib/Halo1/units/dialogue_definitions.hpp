/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <YeloLib/tag_files/tag_groups_base_yelo.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum // dialogue_vocalization sub-types
		{
			_dialogue_vocalization_event,
			_dialogue_vocalization_response, // response/comment
		};

		enum dialogue_vocalization
		{
			//////////////////////////////////////////////////////////////////////////
			// idle
			_dialogue_vocalization_idle_noncombat,
			_dialogue_vocalization_idle_combat,
			_dialogue_vocalization_idle_flee,
			_dialogue_vocalization_unused3,
			_dialogue_vocalization_unused4,
			_dialogue_vocalization_unused5,
			//////////////////////////////////////////////////////////////////////////
			// involuntary
			_dialogue_vocalization_pain_body_minor,
			_dialogue_vocalization_pain_body_major,
			_dialogue_vocalization_pain_shield,
			_dialogue_vocalization_pain_falling,
			_dialogue_vocalization_scream_fear,
			_dialogue_vocalization_scream_pain,
			_dialogue_vocalization_maimed_limb,
			_dialogue_vocalization_maimed_head,
			_dialogue_vocalization_death_quiet,
			_dialogue_vocalization_death_violent,
			_dialogue_vocalization_death_falling,
			_dialogue_vocalization_death_agonizing,
			_dialogue_vocalization_death_instant,
			_dialogue_vocalization_death_flying,
			_dialogue_vocalization_unused20,
			//////////////////////////////////////////////////////////////////////////
			// hurting people
			_dialogue_vocalization_damaged_friend,
			_dialogue_vocalization_damaged_friend_player,
			_dialogue_vocalization_damaged_enemy,
			_dialogue_vocalization_damaged_enemy_comment,
			_dialogue_vocalization_unused25,
			_dialogue_vocalization_unused26,
			_dialogue_vocalization_unused27,
			_dialogue_vocalization_unused28,
			//////////////////////////////////////////////////////////////////////////
			// being hurt
			_dialogue_vocalization_hurt_friend,
			_dialogue_vocalization_hurt_friend_response,
			_dialogue_vocalization_hurt_friend_player,
			_dialogue_vocalization_hurt_enemy,
			_dialogue_vocalization_hurt_enemy_response,
			_dialogue_vocalization_hurt_enemy_comment,
			_dialogue_vocalization_hurt_enemy_bullet,
			_dialogue_vocalization_hurt_enemy_needler,
			_dialogue_vocalization_hurt_enemy_plasma,
			_dialogue_vocalization_hurt_enemy_sniper,
			_dialogue_vocalization_hurt_enemy_grenade,
			_dialogue_vocalization_hurt_enemy_explosion,
			_dialogue_vocalization_hurt_enemy_melee,
			_dialogue_vocalization_hurt_enemy_flame,
			_dialogue_vocalization_hurt_enemy_shotgun,
			_dialogue_vocalization_hurt_enemy_vehicle,
			_dialogue_vocalization_hurt_enemy_mounted_weapon,
			_dialogue_vocalization_unused46,
			_dialogue_vocalization_unused47,
			_dialogue_vocalization_unused48,
			//////////////////////////////////////////////////////////////////////////
			// killing people
			_dialogue_vocalization_killed_friend,
			_dialogue_vocalization_killed_friend_comment,
			_dialogue_vocalization_killed_friend_player,
			_dialogue_vocalization_killed_friend_player_comment,
			_dialogue_vocalization_killed_enemy,
			_dialogue_vocalization_killed_enemy_comment,
			_dialogue_vocalization_killed_enemy_player,
			_dialogue_vocalization_killed_enemy_player_comment,
			_dialogue_vocalization_killed_enemy_covenant,
			_dialogue_vocalization_killed_enemy_covenant_comment,
			_dialogue_vocalization_killed_enemy_flood_combat,
			_dialogue_vocalization_killed_enemy_flood_combat_comment,
			_dialogue_vocalization_killed_enemy_flood_carrier,
			_dialogue_vocalization_killed_enemy_flood_carrier_comment,
			_dialogue_vocalization_killed_enemy_sentinel,
			_dialogue_vocalization_killed_enemy_sentinel_comment,
			_dialogue_vocalization_killed_enemy_bullet,
			_dialogue_vocalization_killed_enemy_needler,
			_dialogue_vocalization_killed_enemy_plasma,
			_dialogue_vocalization_killed_enemy_sniper,
			_dialogue_vocalization_killed_enemy_grenade,
			_dialogue_vocalization_killed_enemy_explosion,
			_dialogue_vocalization_killed_enemy_melee,
			_dialogue_vocalization_killed_enemy_flame,
			_dialogue_vocalization_killed_enemy_shotgun,
			_dialogue_vocalization_killed_enemy_vehicle,
			_dialogue_vocalization_killed_enemy_mounted_weapon,
			_dialogue_vocalization_killing_spree,
			_dialogue_vocalization_unused77,
			_dialogue_vocalization_unused78,
			_dialogue_vocalization_unused79,
			//////////////////////////////////////////////////////////////////////////
			// player kill comments
			_dialogue_vocalization_player_kill_comment,
			_dialogue_vocalization_player_kill_bullet_comment,
			_dialogue_vocalization_player_kill_needler_comment,
			_dialogue_vocalization_player_kill_plasma_comment,
			_dialogue_vocalization_player_kill_sniper_comment,
			_dialogue_vocalization_anyone_kill_grenade_comment,
			_dialogue_vocalization_player_kill_explosion_comment,
			_dialogue_vocalization_player_kill_melee_comment,
			_dialogue_vocalization_player_kill_flame_comment,
			_dialogue_vocalization_player_kill_shotgun_comment,
			_dialogue_vocalization_player_kill_vehicle_comment,
			_dialogue_vocalization_player_kill_mounted_weapon_comment,
			_dialogue_vocalization_player_killing_spree_comment,
			_dialogue_vocalization_unused93,
			_dialogue_vocalization_unused94,
			_dialogue_vocalization_unused95,
			//////////////////////////////////////////////////////////////////////////
			// friends dying
			_dialogue_vocalization_friend_died,
			_dialogue_vocalization_friend_player_died,
			_dialogue_vocalization_friend_killed_by_friend,
			_dialogue_vocalization_friend_killed_by_friendly_player,
			_dialogue_vocalization_friend_killed_by_enemy,
			_dialogue_vocalization_friend_killed_by_enemy_player,
			_dialogue_vocalization_friend_killed_by_covenant,
			_dialogue_vocalization_friend_killed_by_flood,
			_dialogue_vocalization_friend_killed_by_sentinel,
			_dialogue_vocalization_friend_betrayed,
			_dialogue_vocalization_unused106,
			_dialogue_vocalization_unused107,
			//////////////////////////////////////////////////////////////////////////
			// shouting
			_dialogue_vocalization_new_combat_alone,
			_dialogue_vocalization_new_enemy_recent_combat,
			_dialogue_vocalization_old_enemy_sighted,
			_dialogue_vocalization_unexpected_enemy,
			_dialogue_vocalization_dead_friend_found,
			_dialogue_vocalization_alliance_broken,
			_dialogue_vocalization_alliance_reformed,
			_dialogue_vocalization_grenade_throwing,
			_dialogue_vocalization_grenade_sighted,
			_dialogue_vocalization_grenade_startle,
			_dialogue_vocalization_grenade_danger_enemy,
			_dialogue_vocalization_grenade_danager_self,
			_dialogue_vocalization_grenade_danger_friend,
			_dialogue_vocalization_unused121,
			_dialogue_vocalization_unused122,
			//////////////////////////////////////////////////////////////////////////
			// group communication
			_dialogue_vocalization_new_combat_group_response,
			_dialogue_vocalization_new_combat_nearby_response,
			_dialogue_vocalization_alert_friend,
			_dialogue_vocalization_alert_friend_response,
			_dialogue_vocalization_alert_lost_contact,
			_dialogue_vocalization_alert_lost_contact_response,
			_dialogue_vocalization_blocked,
			_dialogue_vocalization_blocked_response,
			_dialogue_vocalization_search_start,
			_dialogue_vocalization_search_query,
			_dialogue_vocalization_search_query_response,
			_dialogue_vocalization_search_report,
			_dialogue_vocalization_search_abandon,
			_dialogue_vocalization_search_group_abandon,
			_dialogue_vocalization_group_uncover,
			_dialogue_vocalization_group_uncover_response,
			_dialogue_vocalization_advance,
			_dialogue_vocalization_advance_response,
			_dialogue_vocalization_retreat,
			_dialogue_vocalization_retreat_response,
			_dialogue_vocalization_cover,
			_dialogue_vocalization_unused144,
			_dialogue_vocalization_unused145,
			_dialogue_vocalization_unused146,
			_dialogue_vocalization_unused147,
			//////////////////////////////////////////////////////////////////////////
			// actions
			_dialogue_vocalization_sighted_friend_player,
			_dialogue_vocalization_shooting,
			_dialogue_vocalization_shooting_vehicle,
			_dialogue_vocalization_shooting_berserk,
			_dialogue_vocalization_shooting_group,
			_dialogue_vocalization_shooting_traitor,
			_dialogue_vocalization_taunt,
			_dialogue_vocalization_taunt_response,
			_dialogue_vocalization_flee,
			_dialogue_vocalization_flee_response,
			_dialogue_vocalization_flee_leader_died,
			_dialogue_vocalization_attempted_flee,
			_dialogue_vocalization_attempted_flee_response,
			_dialogue_vocalization_lost_contact,
			_dialogue_vocalization_hiding_finished,
			_dialogue_vocalization_vehicle_entry,
			_dialogue_vocalization_vehicle_exit,
			_dialogue_vocalization_vehicle_woohoo,
			_dialogue_vocalization_vehicle_scared,
			_dialogue_vocalization_vehicle_collision,
			_dialogue_vocalization_partially_sighted,
			_dialogue_vocalization_nothing_there,
			_dialogue_vocalization_pleading,
			_dialogue_vocalization_unused171,
			_dialogue_vocalization_unused172,
			_dialogue_vocalization_unused173,
			_dialogue_vocalization_unused174,
			_dialogue_vocalization_unused175,
			_dialogue_vocalization_unused176,
			//////////////////////////////////////////////////////////////////////////
			// exclamations
			_dialogue_vocalization_surprise,
			_dialogue_vocalization_berserk,
			_dialogue_vocalization_melee_attack,
			_dialogue_vocalization_dive,
			_dialogue_vocalization_uncover_exclamation,
			_dialogue_vocalization_leap_attack,
			_dialogue_vocalization_resurrection,
			_dialogue_vocalization_unused184,
			_dialogue_vocalization_unused185,
			_dialogue_vocalization_unused186,
			_dialogue_vocalization_unused187,
			//////////////////////////////////////////////////////////////////////////
			// post-combat actions
			_dialogue_vocalization_celebration,
			_dialogue_vocalization_check_body_enemy,
			_dialogue_vocalization_check_body_friend,
			_dialogue_vocalization_shooting_dead_enemy,
			_dialogue_vocalization_shooting_dead_enemy_player,
			_dialogue_vocalization_unused193,
			_dialogue_vocalization_unused194,
			_dialogue_vocalization_unused195,
			_dialogue_vocalization_unused196,
			//////////////////////////////////////////////////////////////////////////
			// post-combat chatter
			_dialogue_vocalization_alone,
			_dialogue_vocalization_unscathed,
			_dialogue_vocalization_seriously_wounded,
			_dialogue_vocalization_seriously_wounded_response,
			_dialogue_vocalization_massacre,
			_dialogue_vocalization_massacre_response,
			_dialogue_vocalization_rout,
			_dialogue_vocalization_rout_response,
			_dialogue_vocalization_unused205,
			_dialogue_vocalization_unused206,
			_dialogue_vocalization_unused207,
			_dialogue_vocalization_unused208,

			k_number_of_dialogue_vocalizations, // NUMBER_OF_VOCALIZATION_TYPES
			k_maximum_dialogue_vocalizations = 256,

			// Halo2:
			// player_look
			// player_look_longtime
			// weapon_trade_better
			// weapon_trade_worse
			// weapon_trade_equal
		};
	};

	namespace TagGroups
	{
		struct s_dialogue_definition
		{
			enum { k_group_tag = 'udlg' };

			int16 skip;
			PAD16;
			TAG_PAD(tag_block, 1);
			TAG_FIELD(tag_reference, vocalizations, 'snd!')[Enums::k_number_of_dialogue_vocalizations];
			TAG_PAD(tag_reference, 
				Enums::k_maximum_dialogue_vocalizations - Enums::k_number_of_dialogue_vocalizations);
		}; BOOST_STATIC_ASSERT( sizeof(s_dialogue_definition) == 0x1010 );
	};
};