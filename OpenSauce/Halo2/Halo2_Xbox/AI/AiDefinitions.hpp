/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum ai_behavior_type
		{
			//_ai_behavior_type_impulse,
			//_ai_behavior_type,
			//_ai_behavior_type,
			_ai_behavior_type,
		};

		enum ai_behavior
		{
			_ai_behavior_GENERAL,
			_ai_behavior_root,
			_ai_behavior_null,
			_ai_behavior_null_discrete,
			_ai_behavior_obey,
			_ai_behavior_guard,
			_ai_behavior_follow_behavior,
			_ai_behavior_ready,
			_ai_behavior_smash_obstacle,
			_ai_behavior_perch,
			_ai_behavior_cover_friend,
			_ai_behavior_blind_panic,

			_ai_behavior_ENGAGE,
			_ai_behavior_engage,
			_ai_behavior_fight,
			_ai_behavior_melee_charge,
			_ai_behavior_melee_leaping_charge,
			_ai_behavior_surprise,
			_ai_behavior_grenade_impulse,
			_ai_behavior_anti_vehicle_grenade,
			_ai_behavior_stalk,
			_ai_behavior_berserk_wander_impulse,

			_ai_behavior_BERSERK,
			_ai_behavior_last_man_berserk,
			_ai_behavior_struck_with_grenade_berserk,

			_ai_behavior_PRESEARCH,
			_ai_behavior_presearch,
			_ai_behavior_presearch_uncover,
			_ai_behavior_destroy_cover,
			_ai_behavior_suppressing_fire,
			_ai_behavior_grenade_uncover,
			_ai_behavior_leap_on_cover,

			_ai_behavior_SEARCH,
			_ai_behavior_search,
			_ai_behavior_uncover,
			_ai_behavior_investigate,
			_ai_behavior_pursuit_sync,
			_ai_behavior_pursuit,
			_ai_behavior_postsearch,
			_ai_behavior_coverme_investigate,

			_ai_behavior_SELF_DEFENSE,
			_ai_behavior_self_perservation,
			_ai_behavior_cover,
			_ai_behavior_cover_peek,
			_ai_behavior_avoid,
			_ai_behavior_evasion_impulse,
			_ai_behavior_dive_impulse,
			_ai_behavior_danger_cover_impulse,
			_ai_behavior_danger_crouch_impulse,
			_ai_behavior_proximity_melee,
			_ai_behavior_proximity_self_preservation,
			_ai_behavior_unreachable_enemy_cover,
			_ai_behavior_scary_target_cover,
			_ai_behavior_group_emerge,

			_ai_behavior_RETREAT,
			_ai_behavior_retreat,
			_ai_behavior_retreat_grenade,
			_ai_behavior_flee,
			_ai_behavior_cower,
			_ai_behavior_low_shield_retreat,
			_ai_behavior_scary_target_retreat,
			_ai_behavior_leader_dead_retreat,
			_ai_behavior_peer_dead_retreat,
			_ai_behavior_danger_retreat,
			_ai_behavior_proximity_retreat,
			_ai_behavior_charge_when_cornered,
			_ai_behavior_surprise_retreat,
			_ai_behavior_overheated_weapon_retreat,

			_ai_behavior_AMBUSH,
			_ai_behavior_ambush,
			_ai_behavior_coordinated_ambush,
			_ai_behavior_proximity_ambush,
			_ai_behavior_vulnerable_enemy_ambush,
			_ai_behavior_nowhere_to_run_ambush,

			_ai_behavior_VEHICLE,
			_ai_behavior_vehicle,
			_ai_behavior_enter_friendly_vehicle,
			_ai_behavior_re_enter_flipped_vehicle,
			_ai_behavior_vehicle_entry_engage_impulse,
			_ai_behavior_vehicle_board,
			_ai_behavior_vehicle_fight,
			_ai_behavior_vehicle_charge,
			_ai_behavior_vehicle_ram_behavior,
			_ai_behavior_vehicle_cover,
			_ai_behavior_damage_vehicle_cover,
			_ai_behavior_exposed_rear_cover_impulse,
			_ai_behavior_player_endagered_cover_impulse,
			_ai_behavior_vehicle_avoid,
			_ai_behavior_vehicle_pickup,
			_ai_behavior_vehicle_player_pickup,
			_ai_behavior_vehicle_exit_impulse,
			_ai_behavior_danger_vehicle_exit_impulse,
			_ai_behavior_vehicle_flip,
			_ai_behavior_vehicle_turtle,
			_ai_behavior_vehicle_engage_patrol_impulse,
			_ai_behavior_vehicle_engage_wander_impulse,

			_ai_behavior_POSTCOMBAT,
			_ai_behavior_postcombat,
			_ai_behavior_post_postcombat,
			_ai_behavior_check_friend,
			_ai_behavior_shoot_corpse,
			_ai_behavior_postcombat_approach,

			_ai_behavior_ALERT,
			_ai_behavior_alert,

			_ai_behavior_IDLE,
			_ai_behavior_idle,
			_ai_behavior_wander_behavior,
			_ai_behavior_flight_wander,
			_ai_behavior_patrol,
			_ai_behavior_fall_sleep,

			_ai_behavior_BUGGERS,
			_ai_behavior_bugger_ground_uncover,

			_ai_behavior_SWARMS,
			_ai_behavior_swarm_root,
			_ai_behavior_swarm_attack,
			_ai_behavior_support_attack,
			_ai_behavior_infect,
			_ai_behavior_scatter,
			_ai_behavior_eject_parasite,
			_ai_behavior_flood_self_preservation,
			_ai_behavior_juggernaut_flurry,

			_ai_behavior_SENTINELS,
			_ai_behavior_enforcer_weapon_control,
			_ai_behavior_grapple,

			_ai_behavior_SPECIAL,
			_ai_behavior_formation,
			_ai_behavior_grunt_scared_by_elite,
			_ai_behavior_stunned,
			_ai_behavior_cure_isolation,
			_ai_behavior_deploy_turret,

			_ai_behavior,
		};

		enum actor_type
		{
			_actor_type_elite,
			_actor_type_jackal,
			_actor_type_grunt,
			_actor_type_hunter,
			_actor_type_engineer,
			_actor_type_assassin,
			_actor_type_player,
			_actor_type_marine,
			_actor_type_crew,
			_actor_type_combat_form,
			_actor_type_infection_form,
			_actor_type_carrier_form,
			_actor_type_monintor,
			_actor_type_sentinel,
			_actor_type_none,
			_actor_type_mounted_weapon,
			_actor_type_brute,
			_actor_type_prophet,
			_actor_type_bugger,
			_actor_type_juggernaut,
			_actor_type,
		};
	};

	namespace AI
	{
	};
};