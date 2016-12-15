/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/ai/actor_types.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum danger_trigger : _enum
		{
			_danger_trigger_never,
			_danger_trigger_visible,
			_danger_trigger_shooting,
			_danger_trigger_shooting_near_us,
			_danger_trigger_damaging_us,
			_danger_trigger_unused,
			_danger_trigger_unused_1,
			_danger_trigger_unused_2,
			_danger_trigger_unused_3,
			_danger_trigger_unused_4,

			_danger_trigger,
		};
		enum defensive_crouch_type : _enum
		{
			_defensive_crouch_type_never,
			_defensive_crouch_type_danger,
			_defensive_crouch_type_low_shields,
			_defensive_crouch_type_hide_behind_shield,
			_defensive_crouch_type_any_target,
			_defensive_crouch_type_flood_shamble,

			_defensive_crouch_type,
		};
		enum actor_movement_type : _enum
		{
			_actor_movement_type_always_run,
			_actor_movement_type_always_crouch,
			_actor_movement_type_switch_types,

			_actor_movement_type,
		};
		enum actor_special_fire_mode : _enum
		{
			_actor_special_fire_mode_none,
			_actor_special_fire_mode_overcharge,
			_actor_special_fire_mode_secondary_trigger,

			_actor_special_fire_mode,
		};
		enum actor_special_fire_situation : _enum
		{
			_actor_special_fire_situation_never,
			_actor_special_fire_situation_enemy_visible,
			_actor_special_fire_situation_enemy_out_of_sight,
			_actor_special_fire_situation_strafing,

			_actor_special_fire_situation,
		};
		enum grenade_type : _enum
		{
			_grenade_type_human_fragmentation,
			_grenade_type_covenant_plasma,
			_grenade_type_custom_2,
			_grenade_type_custom_3,

			_grenade_type,
		};
		enum trajectory_type : _enum
		{
			_trajectory_type_toss,
			_trajectory_type_lob,
			_trajectory_type_bounce,

			_trajectory_type,
		};
		enum grenade_stimulus : _enum
		{
			_grenade_stimulus_never,
			_grenade_stimulus_visible_target,
			_grenade_stimulus_seek_cover,

			_grenade_stimulus,
		};
	};


	namespace TagGroups
	{
		struct s_actor_definition
		{
			enum { k_group_tag = 'actr' };

			struct __flags
			{
				TAG_FLAG(can_see_in_darkness);
				TAG_FLAG(sneak_uncovering_target);
				TAG_FLAG(sneak_uncovering_pursuit_position);
				TAG_FLAG(unused);
				TAG_FLAG(shoot_at_target_s_last_location);
				TAG_FLAG(try_to_stay_still_when_crouched);
				TAG_FLAG(crouch_when_not_in_combat);
				TAG_FLAG(crouch_when_guarding);
				TAG_FLAG(unused_1);
				TAG_FLAG(must_crouch_to_shoot);
				TAG_FLAG(panic_when_surprised);
				TAG_FLAG(always_charge_at_enemies);
				TAG_FLAG(gets_in_vehicles_with_player);
				TAG_FLAG(start_firing_before_aligned);
				TAG_FLAG(standing_must_move_forward);
				TAG_FLAG(crouching_must_move_forward);
				TAG_FLAG(defensive_crouch_while_charging);
				TAG_FLAG(use_stalking_behavior);
				TAG_FLAG(stalking_freeze_if_exposed);
				TAG_FLAG(always_berserk_in_attacking_mode);
				TAG_FLAG(berserking_uses_panicked_movement);
				TAG_FLAG(flying);
				TAG_FLAG(panicked_by_unopposable_enemy);
				TAG_FLAG(crouch_when_hiding_from_unopposable);
				TAG_FLAG(always_charge_in_attacking_mode);
				TAG_FLAG(dive_off_ledges);
				TAG_FLAG(swarm);
				TAG_FLAG(suicidal_melee_attack);
				TAG_FLAG(cannot_move_while_crouching);
				TAG_FLAG(fixed_crouch_facing);
				TAG_FLAG(crouch_when_in_line_of_fire);
				TAG_FLAG(avoid_friends_line_of_fire);
			}; BOOST_STATIC_ASSERT( sizeof(__flags) == sizeof(long_flags) );

			struct __more_flags
			{
				TAG_FLAG(avoid_all_enemy_attack_vectors);
				TAG_FLAG(must_stand_to_fire);
				TAG_FLAG(must_stop_to_fire);
				TAG_FLAG(disallow_vehicle_combat);
				TAG_FLAG(pathfinding_ignores_danger);
				TAG_FLAG(panic_in_groups);
				TAG_FLAG(no_corpse_shooting);
			}; BOOST_STATIC_ASSERT( sizeof(__more_flags) == sizeof(long_flags) );

			TAG_FIELD(__flags, flags);
			TAG_FIELD(__more_flags, more_flags);
			TAG_PAD(int32, 3);
			TAG_ENUM(type, Enums::actor_type);
			PAD16;

			////////////////////////////////////////////////////////////////
			// perception
			TAG_FIELD(real, max_vision_distance, "world units", "maximum range of sight");
			TAG_FIELD(angle, central_vision_angle, "degrees", "horizontal angle within which we see targets out to our maximum range");
			TAG_FIELD(angle, max_vision_angle, "degrees", "maximum horizontal angle within which we see targets at range");
			PAD32;
			TAG_FIELD(angle, peripheral_vision_angle, "degrees", "maximum horizontal angle within which we can see targets out of the corner of our eye");
			TAG_FIELD(real, peripheral_distance, "world units", "maximum range at which we can see targets our of the corner of our eye");
			PAD32;
			TAG_FIELD(real_vector3d, standing_gun_offset, "", "offset of gun from feet when standing (x=forward, y=left, z=up)");
			TAG_FIELD(real_vector3d, crouching_gun_offset, "", "offset of gun from feet when crouch (x=forward, y=left, z=up)");
			TAG_FIELD(real, hearing_distance, "world units", "maximum range at which sounds can be heard");
			TAG_FIELD(real, notice_projectile_chance, "[0,1]", "random chance of noticing a dangerous enemy projectile (e.g. grenade)");
			TAG_FIELD(real, notice_vehicle_chance, "[0,1]", "random chance of noticing a dangerous vehicle");
			TAG_PAD(int32, 2);
			TAG_FIELD(real, combat_perception_time, "seconds", "time required to acknowledge a visible enemy when we are already in combat or searching for them");
			TAG_FIELD(real, guard_perception_time, "seconds", "time required to acknowledge a visible enemy when we have been alerted");
			TAG_FIELD(real, non_combat_perception_time, "seconds", "time required to acknowledge a visible enemy when we are not alerted");
			TAG_PAD(int32, 3);
			TAG_PAD(int32, 2);

			////////////////////////////////////////////////////////////////
			// movement
			TAG_FIELD(real, dive_into_cover_chance, "[0,1]", "chance of running a dive animation when moving into cover");
			TAG_FIELD(real, emerge_from_cover_chance, "[0,1]", "chance of running an emerge animation when uncovering a target");
			TAG_FIELD(real, dive_from_grenade_chance, "[0,1]", "chance of running a dive animation when moving away from a grenade");
			TAG_FIELD(real, pathfinding_radius, "world units");
			TAG_FIELD(real, glass_ignorance_chance, "[0,1]", "chance of not noticing that breakable surfaces have been destroyed");
			TAG_FIELD(real, stationary_movement_dist, "world units", "movement distance which is considered 'stationary' for considering whether we crouch");
			TAG_FIELD(real, free_flying_sidestep, "world units", "distance which we allow sidestepping for flying units");
			TAG_FIELD(angle, begin_moving_angle, "degrees", "we must be facing this close to our target before we start applying the throttle (default: 180 degrees)");
			PAD32;

			////////////////////////////////////////////////////////////////
			// looking
			TAG_FIELD(real_euler_angles2d, maximum_aiming_deviation, "degrees", "how far we can turn our weapon");
			TAG_FIELD(real_euler_angles2d, maximum_looking_deviation, "degrees", "how far we can turn our head");
			TAG_FIELD(angle, noncombat_look_delta_l, "degrees", "how far we can turn our head left away from our aiming vector when not in combat");
			TAG_FIELD(angle, noncombat_look_delta_r, "degrees", "how far we can turn our head right away from our aiming vector when not in combat");
			TAG_FIELD(angle, combat_look_delta_l, "degrees", "how far we can turn our head left away from our aiming vector when in combat");
			TAG_FIELD(angle, combat_look_delta_r, "degrees", "how far we can turn our head right away from our aiming vector when in combat");
			TAG_FIELD(real_euler_angles2d, idle_aiming_range, "degrees", "range in which we select random directions to aim in");
			TAG_FIELD(real_euler_angles2d, idle_looking_range, "degrees", "range in which we select random directions to look at");
			TAG_FIELD(real_bounds, event_look_time_modifier, "", "multiplier for how long we look at interesting events (zero = unchanged)");
			TAG_FIELD(real_bounds, noncombat_idle_facing, "seconds", "rate at which we change facing when looking around randomly when not in combat");
			TAG_FIELD(real_bounds, noncombat_idle_aiming, "seconds", "rate at which we change aiming directions when looking around randomly when not in combat");
			TAG_FIELD(real_bounds, noncombat_idle_looking, "seconds", "rate at which we change look around randomly when not in combat");
			TAG_FIELD(real_bounds, guard_idle_facing, "seconds", "rate at which we change facing when looking around randomly when guarding");
			TAG_FIELD(real_bounds, guard_idle_aiming, "seconds", "rate at which we change aiming directions when looking around randomly when guarding");
			TAG_FIELD(real_bounds, guard_idle_looking, "seconds", "rate at which we change look around randomly when guarding");
			TAG_FIELD(real_bounds, combat_idle_facing, "seconds", "rate at which we change facing when looking around randomly when searching or in combat");
			TAG_FIELD(real_bounds, combat_idle_aiming, "seconds", "rate at which we change aiming directions when looking around randomly when searching or in combat");
			TAG_FIELD(real_bounds, combat_idle_looking, "seconds", "rate at which we change look around randomly when searching or in combat");
			TAG_PAD(int32, 2);
			TAG_PAD(int32, 4);
			TAG_FIELD(tag_reference, do_not_use, 'weap');
			TAG_PAD(int32, 67);
			TAG_FIELD(tag_reference, do_not_use_1, 'proj');

			////////////////////////////////////////////////////////////////
			// unopposable
			TAG_ENUM(unreachable_danger_trigger, Enums::danger_trigger, "danger level of an unreachable enemy which will trigger a retreat if it continues over time");
			TAG_ENUM(vehicle_danger_trigger, Enums::danger_trigger, "danger level of a vehicle-based enemy which will trigger a retreat if it continues over time");
			TAG_ENUM(player_danger_trigger, Enums::danger_trigger, "danger level of an enemy player which will trigger a retreat if it continues over time");
			PAD16;
			TAG_FIELD(real_bounds, danger_trigger_time, "seconds", "how long it takes for an unopposable enemy that has the above danger level to trigger a retreat");
			TAG_FIELD(int16, friends_killed_trigger, "", "if this many of our friends are killed by an unopposable enemy, we trigger a retreat (zero = never use this as a retreat condition)");
			TAG_FIELD(int16, friends_retreating_trigger, "", "if this many of our friends are retreating from an unopposable enemy, we retreat as well (zero = never use this as a retreat condition)");
			TAG_PAD(int32, 3);
			TAG_FIELD(real_bounds, retreat_time, "seconds", "how long we retreat from an unopposable enemy for");
			TAG_PAD(int32, 2);

			////////////////////////////////////////////////////////////////
			// panic
			TAG_FIELD(real_bounds, cowering_time, "seconds", "how long we hide in cover after being panicked");
			TAG_FIELD(real, friend_killed_panic_chance, "[0,1]", "chance of panicking when we see a friend killed near us and the enemy is looking at us too");
			TAG_ENUM(leader_type, Enums::actor_type, "if we see a friend of this type killed we have a chance of panicking");
			PAD16;
			TAG_FIELD(real, leader_killed_panic_chance, "[0,1]", "chance of panicking when we see a leader killed");
			TAG_FIELD(real, panic_damage_threshold, "[0,1]", "panic if we take this much body damage in a short period of time");
			TAG_FIELD(real, surprise_distance, "world units", "the distance at which newly acknowledged props or weapon impacts are considered 'close' for surprise purposes");
			TAG_PAD(int32, 7);

			////////////////////////////////////////////////////////////////
			// defensive
			// danger values: being aimed at: 0.7
			// an enemy shooting in our general direction: 1.2
			// an enemy shooting directly at us: 1.6
			// an enemy damaging us: 2.0
			TAG_FIELD(real_bounds, hide_behind_cover_time, "seconds", "how long we stay behind cover after seeking cover");
			TAG_FIELD(real, hide_target_not_visible_time, "seconds", "if this is non-zero then we will only seek cover if our target has not been visible recently");
			TAG_FIELD(real, hide_shield_fraction, "[0,1]", "elites and jackals only seek cover if their shield falls below this value");
			TAG_FIELD(real, attack_shield_fraction, "[0,1]", "elites and jackals only come out from cover to attack if they have this much shields");
			TAG_FIELD(real, pursue_shield_fraction, "[0,1]", "elites and jackals only come out from cover to pursue if they have this much shields");
			TAG_PAD(int32, 4);
			TAG_ENUM(defensive_crouch_type, Enums::defensive_crouch_type);
			PAD16;
			TAG_FIELD(real, attacking_crouch_threshold, "", "when in attacking mode, if our crouch type is based on shields, we crouch when our shields are below this number; if our crouch type is based on danger, we crouch when our danger is above this number");
			TAG_FIELD(real, defending_crouch_threshold, "", "when in defending mode, if our crouch type is based on shields, we crouch when our shields are below this number; if our crouch type is based on danger, we crouch when our danger is above this number");
			TAG_FIELD(real, min_stand_time, "seconds", "minimum time to remain standing (zero = default)");
			TAG_FIELD(real, min_crouch_time, "seconds", "minimum time to remain crouching (zero = default)");
			TAG_FIELD(real, defending_hide_time_modifier, "", "how much longer we hide behind cover for when in the defending state (zero = unchanged)");
			TAG_FIELD(real, attacking_evasion_threshold, "", "when in attacking mode, we consider seeking cover or evading when our danger gets this high");
			TAG_FIELD(real, defending_evasion_threshold, "", "when in defending mode, we consider seeking cover or evading when our danger gets this high");
			TAG_FIELD(real, evasion_seek_cover_chance, "[0,1]", "chance of seeking cover (otherwise we just evade)");
			TAG_FIELD(real, evasion_delay_time, "seconds", "minimum time period between evasion moves");
			TAG_FIELD(real, max_seek_cover_distance, "world units", "maximum distance we will consider going to find cover (zero = default)");
			TAG_FIELD(real, cover_damage_threshold, "[0,1]", "how much damage we must take before we are allowed to seek cover (zero = always allowed to)");
			TAG_FIELD(real, stalking_discovery_time, "seconds", "if our target sees us for this long while we are stalking them, our cover is blown and we do something else (zero = never stop stalking)");
			TAG_FIELD(real, stalking_max_distance, "world units", "distance outside of which we don't bother stalking");
			TAG_FIELD(angle, stationary_facing_angle, "angle", "angle outside of which we must abandon a stationary facing direction and suffer any penalties");
			TAG_FIELD(real, change_facing_stand_time, "seconds", "how long we must stand up for after changing our fixed stationary facing");
			PAD32;

			////////////////////////////////////////////////////////////////
			// pursuit
			TAG_FIELD(real_bounds, uncover_delay_time, "seconds", "time to look at target's position after it becomes visible");
			TAG_FIELD(real_bounds, target_search_time, "seconds", "time we search at target's position");
			TAG_FIELD(real_bounds, pursuit_position_time, "seconds", "time we search at a pursuit position");
			TAG_FIELD(int16, num_positions, "[0,n]", "number of pursuit positions to check when in coordinated group search mode");
			TAG_FIELD(int16, num_positions_1, "[0,n]", "number of pursuit positions to check when in normal search mode");
			TAG_PAD(int32, 8);

			////////////////////////////////////////////////////////////////
			// berserk
			TAG_FIELD(real, melee_attack_delay, "seconds", "how long we must wait between attempting melee attacks");
			TAG_FIELD(real, melee_fudge_factor, "world units", "fudge factor that offsets how far in front of the target we start our attack (negative = we try to time our attack so that we go _through_ the target). this should be close to zero, but might be bigger for suiciding units");
			TAG_FIELD(real, melee_charge_time, "seconds", "how long we can stay in the charging state trying to reach our target before we give up");
			TAG_FIELD(real_bounds, melee_leap_range, "world units", "we can launch leaping melee attacks at targets within these ranges (zero = can't leap)");
			TAG_FIELD(real, melee_leap_velocity, "world units per tick", "how fast we spring at targets when launching a leaping melee attack");
			TAG_FIELD(real, melee_leap_chance, "[0,1]", "chance of launching a leaping melee attack at a ground-based target (we always leap at flying targets)");
			TAG_FIELD(real, melee_leap_ballistic, "[0,1]", "fraction that controls how ballistic our leaping melee trajectory is");
			TAG_FIELD(real, berserk_damage_amount, "[0,1]", "amount of body damage in a short time that makes us berserk");
			TAG_FIELD(real, berserk_damage_threshold, "[0,1]", "how low our body health must get before we will consider berserking");
			TAG_FIELD(real, berserk_proximity, "world units", "if we ever get this close to a target, we berserk");
			TAG_FIELD(real, suicide_sensing_dist, "world units", "when we are this close to a target, we check to see if they're getting away and if so blow up");
			TAG_FIELD(real, berserk_grenade_chance, "[0,1]", "chance of berserking when we have a dangerous projectile stuck to us");
			TAG_PAD(int32, 3);

			////////////////////////////////////////////////////////////////
			// firing positions
			TAG_FIELD(real_bounds, guard_position_time, "seconds", "time after which we decide to change guard positions (zero = never)");
			TAG_FIELD(real_bounds, combat_position_time, "seconds", "time after which we change combat firing positions");
			TAG_FIELD(real, old_position_avoid_dist, "world units", "distance we try and stay from our last discarded firing position");
			TAG_FIELD(real, friend_avoid_dist, "world units", "distance we try and stay from any friends");
			TAG_PAD(int32, 10);

			////////////////////////////////////////////////////////////////
			// communication
			TAG_FIELD(real_bounds, noncombat_idle_speech_time, "seconds", "time between idle vocalizations when we are not in combat");
			TAG_FIELD(real_bounds, combat_idle_speech_time, "seconds", "time between idle vocalizations when we are in combat or searching");
			TAG_PAD(int32, 12);
			TAG_PAD(int32, 32);
			TAG_FIELD(tag_reference, do_not_use_2, 'actr');
			TAG_PAD(int32, 12);
		}; BOOST_STATIC_ASSERT( sizeof(s_actor_definition) == 0x4F8 ); // max count: 1

		struct s_actor_variant_change_colors
		{
			TAG_FIELD(real_rgb_color, color_lower_bound);
			TAG_FIELD(real_rgb_color, color_upper_bound);
			TAG_PAD(int32, 2);
		}; BOOST_STATIC_ASSERT( sizeof(s_actor_variant_change_colors) == 0x20 ); // max count: 4

		struct s_actor_variant_definition
		{
			enum { k_group_tag = 'actv' };

			struct __flags
			{
				TAG_FLAG(can_shoot_while_flying);
				TAG_FLAG(interpolate_color_in_hsv);
				TAG_FLAG(has_unlimited_grenades);
				TAG_FLAG(moveswitch_stay_w_friends);
				TAG_FLAG(active_camouflage);
				TAG_FLAG(super_active_camouflage);
				TAG_FLAG(cannot_use_ranged_weapons);
				TAG_FLAG(prefer_passenger_seat);
			}; BOOST_STATIC_ASSERT( sizeof(__flags) == sizeof(long_flags) );

			TAG_FIELD(__flags, flags);
			TAG_FIELD(tag_reference, actor_definition, 'actr');
			TAG_FIELD(tag_reference, unit, 'unit');
			TAG_FIELD(tag_reference, major_variant, 'actv');
			TAG_PAD(int32, 6);

			////////////////////////////////////////////////////////////////
			// movement switching
			// note: only the flood combat forms will ever try to switch movement types voluntarily during combat
			TAG_ENUM(movement_type, Enums::actor_movement_type, "when we have a choice of movement types, which type we will use");
			PAD16;
			TAG_FIELD(real, initial_crouch_chance, "[0,1]", "actors that start their movement try to maintain this fraction of crouched actors");
			TAG_FIELD(real_bounds, crouch_time, "seconds", "when switching movement types, how long we will stay crouched for before running");
			TAG_FIELD(real_bounds, run_time, "seconds", "when switching movement types, how long we will run for before slowing to a crouch");

			////////////////////////////////////////////////////////////////
			// ranged combat
			TAG_FIELD(tag_reference, weapon, 'weap');
			TAG_FIELD(real, maximum_firing_distance, "world units", "we can only fire our weapon at targets within this distance");
			TAG_FIELD(real, rate_of_fire, "", "how many times per second we pull the trigger (zero = continuously held down)");
			TAG_FIELD(angle, projectile_error, "degrees", "error added to every projectile we fire");
			TAG_FIELD(real_bounds, first_burst_delay_time, "seconds");
			TAG_FIELD(real, new_target_firing_pattern_time, "seconds");
			TAG_FIELD(real, surprise_delay_time, "seconds");
			TAG_FIELD(real, surprise_fire_wildly_time, "seconds");
			TAG_FIELD(real, death_fire_wildly_chance, "[0,1]");
			TAG_FIELD(real, death_fire_wildly_time, "seconds");
			TAG_FIELD(real_bounds, desired_combat_range, "world units");
			TAG_FIELD(real_vector3d, custom_stand_gun_offset, "", "custom standing gun offset for overriding the default in the base actor");
			TAG_FIELD(real_vector3d, custom_crouch_gun_offset, "", "custom crouching gun offset for overriding the default in the base actor");
			TAG_FIELD(real, target_tracking, "[0,1]", "how well our bursts track moving targets. 0.0= fire at the position they were standing when we started the burst. 1.0= fire at current position");
			TAG_FIELD(real, target_leading, "[0,1]", "how much we lead moving targets. 0.0= no prediction. 1.0= predict completely.");
			TAG_FIELD(real, weapon_damage_modifier, "", "what fraction of its normal damage our weapon inflicts (zero = no modifier)");
			TAG_FIELD(real, damage_per_second, "", "only used if weapon damage modifier is zero... how much damage we should deliver to the target per second while firing a burst at them (zero = use weapon default)");

			////////////////////////////////////////////////////////////////
			// burst geometry
			// at the start of every burst we pick a random point near the target to fire at, on either the left or the right side.
			// the burst origin angle controls whether this error is exactly horizontal or might have some vertical component.
			// 
			// over the course of the burst we move our projectiles back in the opposite direction towards the target. this return motion is also controlled by an angle that specifies how close to the horizontal it is.
			// 
			// for example if the burst origin angle and the burst return angle were both zero, and the return length was the same as the burst length, every burst would start the same amount away from the target (on either the left or right) and move back to exactly over the target at the end of the burst.
			TAG_FIELD(real, burst_origin_radius, "world units", "how far away from the target the starting point is");
			TAG_FIELD(angle, burst_origin_angle, "degrees", "the range from the horizontal that our starting error can be");
			TAG_FIELD(real_bounds, burst_return_length, "world units", "how far the burst point moves back towards the target (could be negative)");
			TAG_FIELD(angle, burst_return_angle, "degrees", "the range from the horizontal that the return direction can be");
			TAG_FIELD(real_bounds, burst_duration, "seconds", "how long each burst we fire is");
			TAG_FIELD(real_bounds, burst_separation, "seconds", "how long we wait between bursts");
			TAG_FIELD(angle, burst_angular_velocity, "degrees per second", "the maximum rate at which we can sweep our fire (zero = unlimited)");
			PAD32;
			TAG_FIELD(real, special_damage_modifier, "[0,1]", "damage modifier for special weapon fire (applied in addition to the normal damage modifier. zero = no change)");
			TAG_FIELD(angle, special_projectile_error, "degrees", "projectile error angle for special weapon fire (applied in addition to the normal error)");

			////////////////////////////////////////////////////////////////
			// firing patterns
			// a firing pattern lets you modify the properties of an actor's burst geometry. actors choose which firing pattern to use based on their current state:
			//      'new-target' when the target just appeared
			//      'moving' when the actor is moving
			//      'berserk' if the actor is berserk
			// if none of these apply, no firing pattern is used.
			// 
			// the default values in the burst geometry are multiplied by any non-zero modifiers in the firing pattern.
			TAG_FIELD(real, new_target_burst_duration, "", "burst duration multiplier for newly appeared targets (zero = unchanged)");
			TAG_FIELD(real, new_target_burst_separation, "", "burst separation multiplier for newly appeared targets (zero = unchanged)");
			TAG_FIELD(real, new_target_rate_of_fire, "", "rate-of-fire multiplier for newly appeared targets (zero = unchanged)");
			TAG_FIELD(real, new_target_projectile_error, "", "error multiplier for newly appeared targets (zero = unchanged)");
			TAG_PAD(int32, 2);
			TAG_FIELD(real, moving_burst_duration, "", "burst duration multiplier when the actor is moving (zero = unchanged)");
			TAG_FIELD(real, moving_burst_separation, "", "burst separation multiplier when the actor is moving (zero = unchanged)");
			TAG_FIELD(real, moving_rate_of_fire, "", "rate-of-fire multiplier when the actor is moving (zero = unchanged)");
			TAG_FIELD(real, moving_projectile_error, "", "error multiplier when the actor is moving (zero = unchanged)");
			TAG_PAD(int32, 2);
			TAG_FIELD(real, berserk_burst_duration, "", "burst duration multiplier when the actor is berserk (zero = unchanged)");
			TAG_FIELD(real, berserk_burst_separation, "", "burst separation multiplier when the actor is berserk (zero = unchanged)");
			TAG_FIELD(real, berserk_rate_of_fire, "", "rate-of-fire multiplier when the actor is berserk (zero = unchanged)");
			TAG_FIELD(real, berserk_projectile_error, "", "error multiplier when the actor is berserk (zero = unchanged)");
			TAG_PAD(int32, 2);

			////////////////////////////////////////////////////////////////
			// special-case firing properties
			TAG_FIELD(real, super_ballistic_range, "", "we try to aim our shots super-ballistically if target is outside this range (zero = never)");
			TAG_FIELD(real, bombardment_range, "", "we offset our burst targets randomly by this range when firing at non-visible enemies (zero = never)");
			TAG_FIELD(real, modified_vision_range, "", "any custom vision range that this actor variant has (zero = normal)");
			TAG_ENUM(special_fire_mode, Enums::actor_special_fire_mode, "the type of special weapon fire that we can use");
			TAG_ENUM(special_fire_situation, Enums::actor_special_fire_situation, "when we will decide to use our special weapon fire mode");
			TAG_FIELD(real, special_fire_chance, "[0,1]", "how likely we are to use our special weapon fire mode");
			TAG_FIELD(real, special_fire_delay, "seconds", "how long we must wait between uses of our special weapon fire mode");

			////////////////////////////////////////////////////////////////
			// berserking and melee
			TAG_FIELD(real, melee_range, "world units", "how close an enemy target must get before triggering a melee charge");
			TAG_FIELD(real, melee_abort_range, "world units", "if our target gets this far away from us, we stop trying to melee them");
			TAG_FIELD(real_bounds, berserk_firing_ranges, "world units", "if we are outside maximum range, we advance towards target, stopping when we reach minimum range");
			TAG_FIELD(real, berserk_melee_range, "world units", "while berserking, how close an enemy target must get before triggering a melee charge");
			TAG_FIELD(real, berserk_melee_abort_range, "world units", "while berserking, if our target gets this far away from us, we stop trying to melee them");
			TAG_PAD(int32, 2);

			////////////////////////////////////////////////////////////////
			// grenades
			TAG_ENUM(grenade_type, Enums::grenade_type, "type of grenades that we throw");
			TAG_ENUM(trajectory_type, Enums::trajectory_type, "how we throw our grenades");
			TAG_ENUM(grenade_stimulus, Enums::grenade_stimulus, "what causes us to consider throwing a grenade");
			TAG_FIELD(int16, minimum_enemy_count, "", "how many enemies must be within the radius of the grenade before we will consider throwing there");
			TAG_FIELD(real, enemy_radius, "world units", "we consider enemies within this radius when determining where to throw");
			PAD32;
			TAG_FIELD(real, grenade_velocity, "world units per second", "how fast we can throw our grenades");
			TAG_FIELD(real_bounds, grenade_ranges, "world units", "ranges within which we will consider throwing a grenade");
			TAG_FIELD(real, collateral_damage_radius, "world units", "we won't throw if there are friendlies around our target within this range");
			TAG_FIELD(real_fraction, grenade_chance, "[0,1]", "how likely we are to throw a grenade");
			TAG_FIELD(real, grenade_check_time, "seconds", "for continuous stimuli (e.g. visible target), how often we check to see if we want to throw a grenade");
			TAG_FIELD(real, encounter_grenade_timeout, "seconds", "we cannot throw grenades if someone else in our encounter threw one this recently");
			TAG_PAD(int32, 5);

			////////////////////////////////////////////////////////////////
			// items
			TAG_FIELD(tag_reference, equipment, 'eqip');
			TAG_FIELD(short_bounds, grenade_count, "", "number of grenades that we start with");
			TAG_FIELD(real, dont_drop_grenades_chance, "[0,1]", "how likely we are not to drop any grenades when we die, even if we still have some");
			TAG_FIELD(real_bounds, drop_weapon_loaded, "", "amount of ammo loaded into the weapon that we drop (in fractions of a clip, e.g. 0.3 to 0.5)");
			TAG_FIELD(short_bounds, drop_weapon_ammo, "", "total number of rounds in the weapon that we drop (ignored for energy weapons)");
			TAG_PAD(int32, 3);
			TAG_PAD(int32, 4);

			////////////////////////////////////////////////////////////////
			// unit
			TAG_FIELD(real, body_vitality, "", "maximum body vitality of our unit");
			TAG_FIELD(real, shield_vitality, "", "maximum shield vitality of our unit");
			TAG_FIELD(real, shield_sapping_radius, "world units", "how far away we can drain the player's shields");
			TAG_FIELD(int16, forced_shader_permutation, "", "if nonzero, overrides the unit's shader permutation");
			PAD16;
			TAG_PAD(int32, 4);
			TAG_PAD(tag_block, 1);
			TAG_TBLOCK(change_colors, s_actor_variant_change_colors);
		}; BOOST_STATIC_ASSERT( sizeof(s_actor_variant_definition) == 0x238 ); // max count: 1		
	};
};