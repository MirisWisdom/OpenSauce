/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <TagGroups/Halo1/TagGroupDefinitions.hpp>
#include <TagGroups/Halo1/object_definitions.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum {
			// Stock number of grenade types
			k_number_of_unit_grenade_types = 2,
			// Number of supported grenade types in OS-modified game states
			k_number_of_unit_grenade_types_yelo = k_number_of_unit_grenade_types + 2,
		};

		enum biped_function_mode : _enum
		{
			_biped_function_mode_none,
			_biped_function_mode_flying_velocity,

			_biped_function_mode,
		};
		enum vehicle_function_mode : _enum
		{
			_vehicle_function_mode_none,
			_vehicle_function_mode_speed_absolute,
			_vehicle_function_mode_speed_forward,
			_vehicle_function_mode_speed_backward,
			_vehicle_function_mode_slide_absolute,
			_vehicle_function_mode_slide_left,
			_vehicle_function_mode_slide_right,
			_vehicle_function_mode_speed_slide_maximum,
			_vehicle_function_mode_turn_absolute,
			_vehicle_function_mode_turn_left,
			_vehicle_function_mode_turn_right,
			_vehicle_function_mode_crouch,
			_vehicle_function_mode_jump,
			_vehicle_function_mode_walk,
			_vehicle_function_mode_velocity_air,
			_vehicle_function_mode_velocity_water,
			_vehicle_function_mode_velocity_ground,
			_vehicle_function_mode_velocity_forward,
			_vehicle_function_mode_velocity_left,
			_vehicle_function_mode_velocity_up,
			_vehicle_function_mode_left_tread_position,
			_vehicle_function_mode_right_tread_position,
			_vehicle_function_mode_left_tread_velocity,
			_vehicle_function_mode_right_tread_velocity,
			_vehicle_function_mode_front_left_tire_position,
			_vehicle_function_mode_front_right_tire_position,
			_vehicle_function_mode_back_left_tire_position,
			_vehicle_function_mode_back_right_tire_position,
			_vehicle_function_mode_front_left_tire_velocity,
			_vehicle_function_mode_front_right_tire_velocity,
			_vehicle_function_mode_back_left_tire_velocity,
			_vehicle_function_mode_back_right_tire_velocity,
			_vehicle_function_mode_wingtip_contrail,
			_vehicle_function_mode_hover,
			_vehicle_function_mode_thrust,
			_vehicle_function_mode_engine_hack,
			_vehicle_function_mode_wingtip_contrail_new,

			_vehicle_function_mode,
		};
		enum vehicle_type : _enum
		{
			_vehicle_type_human_tank,
			_vehicle_type_human_jeep,
			_vehicle_type_human_boat,
			_vehicle_type_human_plane,
			_vehicle_type_alien_scout,
			_vehicle_type_alien_fighter,
			_vehicle_type_turret,

			_vehicle_type,
		};
	};
	namespace TagGroups
	{
		struct s_unit_camera_track
		{
			TAG_FIELD(tag_reference, track, "trak");
			PAD32; PAD32; PAD32;
		};
		struct s_unit_camera
		{
			TAG_FIELD(tag_string, camera_marker_name);
			TAG_FIELD(tag_string, camera_submerged_marker_name);
			TAG_FIELD(real, pitch_autolevel, "degrees");
			TAG_FIELD(real_bounds, pitch_range, "degrees");
			TAG_TBLOCK(camera_tracks, s_unit_camera_track);
		};

		struct unit_seat_acceleration
		{
			TAG_FIELD(real_vector3d, seat_acceleration_scale);
			PAD32; PAD32; PAD32;
		};

		struct unit_hud_reference
		{
			TAG_FIELD(tag_reference, unit_hud_interface, "unhi");
			TAG_PAD(int32, 8); // 32
		};

		struct dialogue_variant_definition
		{
			TAG_FIELD(int16, varient_number, "", "variant number to use this dialogue with (must match the suffix in the permutations on the unit's model)");
			PAD16;
			PAD32;
			TAG_FIELD(tag_reference, dialogue, "udlg");
		};

		struct powered_seat_definition
		{
			PAD32;
			TAG_FIELD(real, driver_powerup_time, "seconds");
			TAG_FIELD(real, driver_powerdown_time, "seconds");
			TAG_PAD(int32, 14); // 56
		};

		struct unit_initial_weapon
		{
			TAG_FIELD(tag_reference, weapon, "weap");
			TAG_PAD(int32, 4); // 20
		};

		struct unit_seat
		{
			struct _Flags {
				TAG_FLAG(invisable);
				TAG_FLAG(locked);
				TAG_FLAG(driver);
				TAG_FLAG(gunner);
				TAG_FLAG(third_person);
				TAG_FLAG(camera);
				TAG_FLAG(allows_weapons);
				TAG_FLAG(third_person_on_enter);
				TAG_FLAG(first_person_camera_slaved_to_gun);
				TAG_FLAG(allow_vehicle_communication_anims);
				TAG_FLAG(not_valid_without_driver);
				TAG_FLAG(allow_ai_noncombatants);
			}flags;

			TAG_FIELD(tag_string, label);
			TAG_FIELD(tag_string, marker_name);
			TAG_PAD(tag_string, 1); // 32

			TAG_FIELD(unit_seat_acceleration, acceleration);

			TAG_FIELD(real, yaw_rate, "degrees per second");
			TAG_FIELD(real, pitch_rate, "degrees per second");

			TAG_FIELD(s_unit_camera, unit_camera);
			TAG_TBLOCK(unit_hud_interface, unit_hud_reference);

			TAG_PAD(int32, 1); // 4
			TAG_FIELD(int16, hud_text_message_index);
			PAD16;

			TAG_FIELD(real, yaw_min, "degrees");
			TAG_FIELD(real, yaw_max, "degrees");

			TAG_FIELD(tag_reference, built_in_gunner, "actv");
			TAG_PAD(int32, 5); // 20
		}; BOOST_STATIC_ASSERT( sizeof(unit_seat) == 284 );
		//////////////////////////////////////////////////////////////////////////
		struct _unit_definition
		{
			struct __unit_definition_flags {
				TAG_FLAG(circular_aiming);
				TAG_FLAG(destroyed_after_dying);
				TAG_FLAG(half_speed_interpolation);
				TAG_FLAG(fires_from_camera);
				TAG_FLAG(entrance_inside_bounding_sphere);
				TAG_FLAG(unused);
				TAG_FLAG(causes_passenger_dialogue);
				TAG_FLAG(resists_pings);
				TAG_FLAG(melee_attack_is_fatal);
				TAG_FLAG(dont_reface_during_pings);
				TAG_FLAG(has_no_aiming);
				TAG_FLAG(simple_creature);
				TAG_FLAG(impact_melee_attaches_to_unit);
				TAG_FLAG(impact_melee_dies_on_shields);
				TAG_FLAG(cannot_open_doors_automaticlly);
				TAG_FLAG(melee_attackers_cannot_attach);
				TAG_FLAG(not_instantly_killed_by_melee);
				TAG_FLAG(shield_sapping);
				TAG_FLAG(runs_around_flaming);
				TAG_FLAG(inconsequential);
				TAG_FLAG(special_cinematic_unit);
				TAG_FLAG(ignored_by_autoaiming);
				TAG_FLAG(shields_fry_infection_forms);
				TAG_FLAG(integrated_light_controls_weapon);
				TAG_FLAG(integrated_light_lasts_forever);
			}flags;

			TAG_ENUM(default_team, unit_default_team);
			TAG_ENUM(constant_sound_volume, ai_sound_volume);

			TAG_FIELD(real, rider_damage_fraction, "what percent damage applied to us gets applied to our children (i.e., riders)");

			TAG_FIELD(tag_reference, integrated_light_toggle, "effe");

			TAG_ENUM(a_in, unit_function_mode);
			TAG_ENUM(b_in, unit_function_mode);
			TAG_ENUM(c_in, unit_function_mode);
			TAG_ENUM(d_in, unit_function_mode);

			TAG_FIELD(real, camera_fov, "degrees");
			TAG_FIELD(real, camera_stiffness);
			TAG_FIELD(s_unit_camera, unit_camera);

			TAG_FIELD(unit_seat_acceleration, acceleration);

			TAG_FIELD(real, soft_ping_threshold);
			TAG_FIELD(real, soft_ping_interrupt_time, "seconds");

			TAG_FIELD(real, hard_ping_threshold);
			TAG_FIELD(real, hard_ping_interrupt_time, "seconds");

			TAG_FIELD(real, hard_death_threshold);
			TAG_FIELD(real, feign_death_threshold);
			TAG_FIELD(real, feign_death_time, "seconds");

			TAG_FIELD(real, distance_of_evade_anim, "world units", "this must be set to tell the AI how far it should expect our evade animation to move us");
			TAG_FIELD(real, distance_of_dive_anim, "world units", "this must be set to tell the AI how far it should expect our dive animation to move us");
			PAD32;

			TAG_FIELD(real, stunned_movement_threshold, "", "if we take this much damage in a short space of time we will play our 'stunned movement' animations");
			TAG_FIELD(real, feign_death_chance, "");
			TAG_FIELD(real, feign_repeat_chance, "");

			TAG_FIELD(tag_reference, spawned_actor, "actv", "actor variant which we spawn when we are destroyed or self-destruct");
			TAG_FIELD(short_bounds, spawned_actor_count, "", "number of actors which we spawn");
			TAG_FIELD(real, spawned_velocity, "", "velocity at which we throw spawned actors");

			TAG_FIELD(real, aiming_velocity_max, "degrees per second");
			TAG_FIELD(real, aiming_acceleration_max, "degrees per second");
			TAG_FIELD(real, casual_aiming_modifier, "[0,1]");

			TAG_FIELD(real, looking_velocity_max, "degrees per second");
			TAG_FIELD(real, looking_acceleration_max, "degrees per second");
			PAD32; PAD32;

			TAG_FIELD(real, ai_vehicle_radius, "", "radius around this unit that the AI tries to avoid when entering it as a vehicle (zero = use bounding sphere radius)");
			TAG_FIELD(real, ai_danger_radius, "", "danger radius around this unit that the AI tries to avoid");

			TAG_FIELD(tag_reference, melee_damage, "jpt!");

			TAG_ENUM(motion_sensor_blip_size);
			PAD16;

			PAD_TYPE(tag_block);

			TAG_TBLOCK(new_hud_interfaces, unit_hud_reference);
			TAG_TBLOCK(dialogue_variants, dialogue_variant_definition);

			TAG_FIELD(real, grenade_velocity, "world units per second");
			TAG_ENUM(grenade_type);
			TAG_FIELD(int16, grenade_count);
			PAD32;

			TAG_TBLOCK(powered_seats, powered_seat_definition);
			TAG_TBLOCK(weapons, unit_initial_weapon);
			TAG_TBLOCK(seats, unit_seat);

		}; BOOST_STATIC_ASSERT( sizeof(_unit_definition) == 0x174 );
		
		struct s_unit_definition : s_object_definition
		{
			_unit_definition unit;
		};
		//////////////////////////////////////////////////////////////////////////
		struct s_contact_point
		{
			TAG_PAD(int32, 8);
			TAG_FIELD(tag_string, marker_name);
		}; BOOST_STATIC_ASSERT( sizeof(s_contact_point) == 0x40 ); // max count: 2
		struct _biped_definition
		{
			struct __flags
			{
				TAG_FLAG(turns_without_animating);
				TAG_FLAG(uses_player_physics);
				TAG_FLAG(flying);
				TAG_FLAG(physics_pill_centered_at_origin);
				TAG_FLAG(spherical);
				TAG_FLAG(passes_through_other_bipeds);
				TAG_FLAG(can_climb_any_surface);
				TAG_FLAG(immune_to_falling_damage);
				TAG_FLAG(rotate_while_airborne);
				TAG_FLAG(uses_limp_body_physics);
				TAG_FLAG(has_no_dying_airborne);
				TAG_FLAG(random_speed_increase);
				TAG_FLAG(unit_uses_old_ntsc_player_physics);
			}; BOOST_STATIC_ASSERT( sizeof(__flags) == sizeof(long_flags) );

			////////////////////////////////////////////////////////////////
			// $$$ BIPED $$$
			TAG_FIELD(angle, moving_turning_speed, "degrees per second");
			TAG_FIELD(__flags, flags);
			TAG_FIELD(angle, stationary_turning_threshold);
			TAG_PAD(int32, 4);
			struct{
				TAG_ENUM(source, Enums::biped_function_mode);
			}inputs[4];
			TAG_FIELD(tag_reference, don_t_use, "jpt!");

			////////////////////////////////////////////////////////////////
			// flying
			TAG_FIELD(angle, bank_angle, "degrees", "angle at which we bank left/right when sidestepping or turning while moving forwards");
			TAG_FIELD(real, bank_apply_time, "seconds", "time it takes us to apply a bank");
			TAG_FIELD(real, bank_decay_time, "seconds", "time it takes us to recover from a bank");
			TAG_FIELD(real, pitch_ratio, "", "amount that we pitch up/down when moving up or down");
			TAG_FIELD(real, max_velocity, "world units per second", "max velocity when not crouching");
			TAG_FIELD(real, max_sidestep_velocity, "world units per second", "max sideways or up/down velocity when not crouching");
			TAG_FIELD(real, acceleration, "world units per second squared");
			TAG_FIELD(real, deceleration, "world units per second squared");
			TAG_FIELD(angle, angular_velocity_maximum, "degrees per second", "turn rate");
			TAG_FIELD(angle, angular_acceleration_maximum, "degrees per second squared", "turn acceleration rate");
			TAG_FIELD(real, crouch_velocity_modifier, "[0,1]", "how much slower we fly if crouching (zero = same speed)");
			TAG_PAD(int32, 2);

			////////////////////////////////////////////////////////////////
			// movement
			TAG_FIELD(angle, maximum_slope_angle, "degrees");
			TAG_FIELD(angle, downhill_falloff_angle, "degrees");
			TAG_FIELD(angle, downhill_cutoff_angle, "degrees");
			TAG_FIELD(real, downhill_velocity_scale);
			TAG_FIELD(angle, uphill_falloff_angle, "degrees");
			TAG_FIELD(angle, uphill_cutoff_angle, "degrees");
			TAG_FIELD(real, uphill_velocity_scale);
			TAG_PAD(int32, 6);
			TAG_FIELD(tag_reference, footsteps, "foot");
			TAG_PAD(int32, 6);

			////////////////////////////////////////////////////////////////
			// jumping and landing
			TAG_FIELD(real, jump_velocity, "world units per second");
			TAG_PAD(int32, 7);
			TAG_FIELD(real, maximum_soft_landing_time, "seconds", "the longest amount of time the biped can take to recover from a soft landing");
			TAG_FIELD(real, maximum_hard_landing_time, "seconds", "the longest amount of time the biped can take to recover from a hard landing");
			TAG_FIELD(real, minimum_soft_landing_velocity, "world units per second", "below this velocity the biped does not react when landing");
			TAG_FIELD(real, minimum_hard_landing_velocity, "world units per second", "below this velocity the biped will not do a soft landing when returning to the ground");
			TAG_FIELD(real, maximum_hard_landing_velocity, "world units per second", "the velocity corresponding to the maximum landing time");
			TAG_FIELD(real, death_hard_landing_velocity, "world units per second", "the maximum velocity with which a character can strike the ground and live");
			TAG_PAD(int32, 5);

			////////////////////////////////////////////////////////////////
			// camera, collision, and autoaim
			TAG_FIELD(real, standing_camera_height, "world units");
			TAG_FIELD(real, crouching_camera_height, "world units");
			TAG_FIELD(real, crouch_transition_time, "seconds");
			TAG_PAD(int32, 6);
			TAG_FIELD(real, standing_collision_height, "world units");
			TAG_FIELD(real, crouching_collision_height, "world units");
			TAG_FIELD(real, collision_radius, "world units");
			TAG_PAD(int32, 10);
			TAG_FIELD(real, autoaim_width, "world units");
			TAG_PAD(int32, 35);
			TAG_TBLOCK(contact_points, s_contact_point);
		};
		
		struct s_biped_definition : s_unit_definition
		{
			enum { k_group_tag = 'bipd' };

			_biped_definition biped;
		}; BOOST_STATIC_ASSERT( sizeof(s_biped_definition) == 0x4F4 );
		//////////////////////////////////////////////////////////////////////////
		struct _vehicle_definition
		{	
			struct __flags
			{
				TAG_FLAG(speed_wakes_physics);
				TAG_FLAG(turn_wakes_physics);
				TAG_FLAG(driver_power_wakes_physics);
				TAG_FLAG(gunner_power_wakes_physics);
				TAG_FLAG(control_opposite_speed_sets_brake);
				TAG_FLAG(slide_wakes_physics);
				TAG_FLAG(kills_riders_at_terminal_velocity);
				TAG_FLAG(causes_collision_damage);
				TAG_FLAG(ai_weapon_cannot_rotate);
				TAG_FLAG(ai_does_not_require_driver);
				TAG_FLAG(ai_unused);
				TAG_FLAG(ai_driver_enable);
				TAG_FLAG(ai_driver_flying);
				TAG_FLAG(ai_driver_can_sidestep);
				TAG_FLAG(ai_driver_hovering);
			}; BOOST_STATIC_ASSERT( sizeof(__flags) == sizeof(long_flags) );

			////////////////////////////////////////////////////////////////
			// $$$ VEHICLE $$$
			TAG_FIELD(__flags, flags);
			TAG_ENUM(type, Enums::vehicle_type);
			PAD16;
			TAG_FIELD(real, maximum_forward_speed);
			TAG_FIELD(real, maximum_reverse_speed);
			TAG_FIELD(real, speed_acceleration);
			TAG_FIELD(real, speed_deceleration);
			TAG_FIELD(real, maximum_left_turn);
			TAG_FIELD(real, maximum_right_turn);
			TAG_FIELD(real, wheel_circumference);
			TAG_FIELD(real, turn_rate);
			TAG_FIELD(real, blur_speed);
			struct{
				TAG_ENUM(source, Enums::vehicle_function_mode);
			}inputs[4];
			TAG_PAD(int32, 3);
			TAG_FIELD(real, maximum_left_slide);
			TAG_FIELD(real, maximum_right_slide);
			TAG_FIELD(real, slide_acceleration);
			TAG_FIELD(real, slide_deceleration);
			TAG_FIELD(real, minimum_flipping_angular_velocity);
			TAG_FIELD(real, maximum_flipping_angular_velocity);
			TAG_PAD(int32, 6);
			TAG_FIELD(real, fixed_gun_yaw);
			TAG_FIELD(real, fixed_gun_pitch);
			TAG_PAD(int32, 6);
			TAG_FIELD(real, ai_sideslip_distance);
			TAG_FIELD(real, ai_destination_radius);
			TAG_FIELD(real, ai_avoidance_distance);
			TAG_FIELD(real, ai_pathfinding_radius);
			TAG_FIELD(real, ai_charge_repeat_timeout);
			TAG_FIELD(real, ai_strafing_abort_range);
			TAG_FIELD(angle_bounds, ai_oversteering_bounds);
			TAG_FIELD(angle, ai_steering_maximum);
			TAG_FIELD(real, ai_throttle_maximum);
			TAG_FIELD(real, ai_move_position_time);
			PAD32;
			TAG_FIELD(tag_reference, suspension_sound, "snd!");
			TAG_FIELD(tag_reference, crash_sound, "snd!");
			TAG_FIELD(tag_reference, material_effects, "foot");
			TAG_FIELD(tag_reference, effect, "effe");
		};
		
		struct s_vehicle_definition : s_unit_definition
		{
			enum { k_group_tag = 'vehi' };

			_vehicle_definition vehicle;
		}; BOOST_STATIC_ASSERT( sizeof(s_vehicle_definition) == 0x3F0 );
		//////////////////////////////////////////////////////////////////////////
	};
};