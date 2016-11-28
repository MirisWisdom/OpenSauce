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
		enum {
			k_max_vehicle_suspensions = 8,
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
			_enum function_exports[Enums::k_number_of_incoming_object_functions]; // Enums::vehicle_function_mode
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
			TAG_FIELD(tag_reference, suspension_sound, 'snd!');
			TAG_FIELD(tag_reference, crash_sound, 'snd!');
			TAG_FIELD(tag_reference, material_effects, 'foot');
			TAG_FIELD(tag_reference, effect, 'effe');
		};

		struct s_vehicle_definition : s_unit_definition
		{
			enum { k_group_tag = 'vehi' };

			_vehicle_definition vehicle;
		}; BOOST_STATIC_ASSERT( sizeof(s_vehicle_definition) == 0x3F0 );
	};
};