/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/objects/object_definitions.hpp>
#include <blamlib/Halo1/units/unit_camera.hpp>

#include <YeloLib/Halo1/units/unit_definitions_yelo.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum tag_postprocess_mode : byte_enum;

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
#if PLATFORM_IS_STUBBS
			_unit_base_seat_aware,
#endif
			_unit_base_seat_stand,
			_unit_base_seat_crouch,
			_unit_base_seat_flee,
			_unit_base_seat_flaming,
#if PLATFORM_IS_STUBBS
			_unit_base_seat_maimed,
#endif

			k_number_of_unit_base_seats,
		};

		enum unit_base_weapon {
			_unit_base_weapon_unarmed,

			k_number_of_unit_base_weapons,
		};
	};

	namespace Flags
	{
		enum unit_definition_flags {
			_unit_circular_aiming_bit,
			_unit_destroyed_after_dying_bit,
			_unit_half_speed_interpolation_bit,
			_unit_fires_from_camera_bit,
			_unit_entrance_inside_bounding_sphere_bit,
			_unit_unused5_bit,							// Halo2: 'doesn't show readied weapon'
			_unit_causes_passenger_dialogue_bit,
			_unit_resists_pings_bit,
			_unit_melee_attack_is_fatal_bit,
			_unit_dont_reface_during_pings_bit,
			_unit_has_no_aiming_bit,
			_unit_simple_creature_bit,
			_unit_impact_melee_attaches_to_unit_bit,
			_unit_impact_melee_dies_on_shields_bit,
			_unit_cannot_open_doors_automaticlly_bit,
			_unit_melee_attackers_cannot_attach_bit,
			_unit_not_instantly_killed_by_melee_bit,
			_unit_shield_sapping_bit,
			_unit_runs_around_flaming_bit,
			_unit_inconsequential_bit,
			_unit_special_cinematic_unit_bit,
			_unit_ignored_by_autoaiming_bit,
			_unit_shields_fry_infection_forms_bit,
			_unit_integrated_light_controls_weapon_bit,	// Halo2: unused
			_unit_integrated_light_lasts_forever_bit,	// Halo2: unused
			_unit_reserved25_bit,						// Halo2: acts as gunner for parent
			_unit_reserved26_bit,						// Halo2: controlled by parent gunner
			_unit_reserved27_bit,						// Halo2: parent's primary weapon
			_unit_reserved28_bit,						// Halo2: unit has boost
			_unit_unused29_bit,
			_unit_unused30_bit,
			_unit_has_boarding_seats_yelo_bit,			// YELO: set by postprocess proc, for runtime

			k_number_of_unit_definition_flags
		};
		BOOST_STATIC_ASSERT(k_number_of_unit_definition_flags <= BIT_COUNT(long_flags));

		enum unit_seat_definition_flags {
			_unit_seat_invisable_bit,
			_unit_seat_locked_bit,
			_unit_seat_driver_bit,
			_unit_seat_gunner_bit,
			_unit_seat_third_person_camera_bit,
			_unit_seat_allows_weapons_bit,
			_unit_seat_third_person_on_enter_bit,
			_unit_seat_first_person_camera_slaved_to_gun_bit,
			_unit_seat_allow_vehicle_communication_anims_bit,
			_unit_seat_not_valid_without_driver_bit,
			_unit_seat_allow_ai_noncombatants_bit,

			k_number_of_unit_seat_definition_flags,

			_unit_seat_reserved11_bit =				// Halo2
				k_number_of_unit_seat_definition_flags,
			_unit_seat_reserved12_bit,					// Halo2: ai firing disabled by max acceleration
			_unit_seat_reserved13_bit,		// Halo2
			_unit_seat_reserved14_bit,	// Halo2
			_unit_seat_reserved15_bit,		// Halo2
			_unit_seat_reserved16_bit,	// Halo2: invalid for player
			_unit_seat_reserved17_bit,	// Halo2: invalid for non-player
			_unit_seat_reserved18_bit,	// Halo2: gunner (player only)
			_unit_seat_reserved19_bit,	// Halo2: invisible under major damage
			_unit_seat_allows_melee_bit,	// YELO

			k_number_of_unit_seat_definition_flags_yelo,
		};
		BOOST_STATIC_ASSERT(k_number_of_unit_seat_definition_flags_yelo <= BIT_COUNT(long_flags));
	};

	namespace TagGroups
	{
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
			TAG_FIELD(long_flags, flags, Flags::unit_seat_definition_flags);
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

			TAG_TBLOCK(seat_extensions, unit_seat_extensions);
			TAG_PAD(int32, 2); // 8

		private:
			bool Postprocess(Enums::tag_postprocess_mode mode,
				datum_index tag_index);
		public:
			static bool PLATFORM_API Postprocess(void* element, Enums::tag_postprocess_mode mode,
				datum_index tag_index);
		}; BOOST_STATIC_ASSERT( sizeof(unit_seat) == 284 );
		//////////////////////////////////////////////////////////////////////////
		struct _unit_definition
		{
			TAG_FIELD(long_flags, flags, Flags::unit_definition_flags);
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

			TAG_ENUM(blip_type);
			PAD16;

			TAG_TBLOCK(extensions, unit_extensions);

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

			bool Postprocess(Enums::tag_postprocess_mode mode,
				datum_index tag_index);

		}; BOOST_STATIC_ASSERT( sizeof(_unit_definition) == 0x174 );

		struct s_unit_definition : s_object_definition
		{
			enum { k_group_tag = 'unit' };

			_unit_definition unit;

			bool Postprocess(Enums::tag_postprocess_mode mode,
				datum_index tag_index)
			{
				return unit.Postprocess(mode, tag_index);
			}
		};
	};
};