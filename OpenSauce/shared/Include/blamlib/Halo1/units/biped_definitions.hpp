/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/units/unit_definitions.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum biped_function_mode : _enum
		{
			_biped_function_mode_none,
			_biped_function_mode_flying_velocity,

			_biped_function_mode,
		};
	};

	namespace TagGroups
	{
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
				TAG_FLAG(uses_player_physics);				// Halo2: removed
				TAG_FLAG(flying);							// Halo2: removed
				TAG_FLAG(physics_pill_centered_at_origin);	// Halo2: removed
				TAG_FLAG(spherical);						// Halo2: removed
				TAG_FLAG(passes_through_other_bipeds);
				TAG_FLAG(can_climb_any_surface);			// Halo2: removed
				TAG_FLAG(immune_to_falling_damage);
				TAG_FLAG(rotate_while_airborne);
				TAG_FLAG(uses_limp_body_physics);
				TAG_FLAG(has_no_dying_airborne);			// Halo2: unused
				TAG_FLAG(random_speed_increase);
				TAG_FLAG(unit_uses_old_ntsc_player_physics);// Halo2: unused or removed
			}; BOOST_STATIC_ASSERT( sizeof(__flags) == sizeof(long_flags) );

			////////////////////////////////////////////////////////////////
			// $$$ BIPED $$$
			TAG_FIELD(angle, moving_turning_speed, "degrees per second");
			TAG_FIELD(__flags, flags);
			TAG_FIELD(angle, stationary_turning_threshold);
			TAG_PAD(int32, 4);
			_enum function_exports[Enums::k_number_of_incoming_object_functions]; // Enums::biped_function_mode
			TAG_FIELD(tag_reference, dont_use, 'jpt!');

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
			TAG_FIELD(tag_reference, footsteps, 'foot');
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
	};
};