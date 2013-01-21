/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/objects/object_definitions.hpp>

#include <blamlib/Halo1/tag_files/tag_groups.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_maximum_weapons_per_unit = 4,
		};

		enum powered_seat {
			_powered_seat_driver,
			_powered_seat_gunner,

			k_number_of_powered_seats
		};

		enum unit_base_seat {
			_unit_base_seat_asleep,
			_unit_base_seat_alert,
			_unit_base_seat_stand,
			_unit_base_seat_crouch,
			_unit_base_seat_flee,
			_unit_base_seat_flaming,

			k_number_of_unit_base_seats,
		};

		enum unit_base_weapon {
			_unit_base_weapon_unarmed,

			k_number_of_unit_base_weapons,
		};
	};

	namespace TagGroups
	{
		struct s_unit_camera_track
		{
			TAG_FIELD(tag_reference, track, 'trak');
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
			TAG_FIELD(tag_reference, unit_hud_interface, 'unhi');
			TAG_PAD(int32, 8); // 32
		};

		struct dialogue_variant_definition
		{
			TAG_FIELD(int16, varient_number, "variant number to use this dialogue with (must match the suffix in the permutations on the unit's model)");
			PAD16;
			PAD32;
			TAG_FIELD(tag_reference, dialogue, 'udlg');
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
			TAG_FIELD(tag_reference, weapon, 'weap');
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

			TAG_FIELD(tag_reference, built_in_gunner, 'actv');
			TAG_PAD(int32, 5); // 20
		}; BOOST_STATIC_ASSERT( sizeof(unit_seat) == 284 );
		//////////////////////////////////////////////////////////////////////////
		struct _unit_definition
		{
			struct _flags {
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

			TAG_FIELD(tag_reference, integrated_light_toggle, 'effe');

			_enum function_exports[Enums::k_number_of_incoming_object_functions]; // Enums::unit_function_mode

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

			TAG_FIELD(real, stunned_movement_threshold, "if we take this much damage in a short space of time we will play our 'stunned movement' animations");
			TAG_FIELD(real, feign_death_chance);
			TAG_FIELD(real, feign_repeat_chance);

			TAG_FIELD(tag_reference, spawned_actor, "actv", "actor variant which we spawn when we are destroyed or self-destruct");
			TAG_FIELD(short_bounds, spawned_actor_count, "", "number of actors which we spawn");
			TAG_FIELD(real, spawned_velocity, "", "velocity at which we throw spawned actors");

			TAG_FIELD(real, aiming_velocity_max, "degrees per second");
			TAG_FIELD(real, aiming_acceleration_max, "degrees per second");
			TAG_FIELD(real, casual_aiming_modifier, "[0,1]");

			TAG_FIELD(real, looking_velocity_max, "degrees per second");
			TAG_FIELD(real, looking_acceleration_max, "degrees per second");
			PAD32; PAD32;

			TAG_FIELD(real, ai_vehicle_radius, "radius around this unit that the AI tries to avoid when entering it as a vehicle (zero = use bounding sphere radius)");
			TAG_FIELD(real, ai_danger_radius, "danger radius around this unit that the AI tries to avoid");

			TAG_FIELD(tag_reference, melee_damage, 'jpt!');

			TAG_ENUM(motion_sensor_blip_size);
			PAD16;

			PAD_TYPE(tag_block);

			TAG_TBLOCK(new_hud_interfaces, unit_hud_reference);
			TAG_TBLOCK(dialogue_variants, dialogue_variant_definition);

			TAG_FIELD(real, grenade_velocity, "world units per second");
			TAG_ENUM(grenade_type);
			TAG_FIELD(int16, grenade_count);

			struct { // tag value * 30f
				int16 soft;
				int16 hard;
			}runtime_ping_interrupt_time;

			TAG_TBLOCK(powered_seats, powered_seat_definition);
			TAG_TBLOCK(weapons, unit_initial_weapon);
			TAG_TBLOCK(seats, unit_seat);

		}; BOOST_STATIC_ASSERT( sizeof(_unit_definition) == 0x174 );

		struct s_unit_definition : s_object_definition
		{
			_unit_definition unit;
		};
	};
};