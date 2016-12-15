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
		enum {
			k_max_sound_references_per_graph = 257,
			k_max_sound_references_per_graph_upgrade = k_max_sound_references_per_graph * 2,

			// Halo2's unit_seat block was refactored to animation_mode
			k_max_unit_seat_modes_per_graph = 32,
			k_max_unit_seat_modes_per_graph_upgrade = 512, // Halo2's amount

			k_max_animations_per_graph = 256,
			// must be <= k_maximum_tool_import_files_upgrade
			k_max_animations_per_graph_upgrade = 2048,
		};

		enum model_animation_type
		{
			_model_animation_type_base,			// _animation_base
			_model_animation_type_overlay,
			_model_animation_type_replacement,

			k_number_of_model_animation_types
		};

		enum weapon_type_animation
		{
			_weapon_type_animation_reload1,
			_weapon_type_animation_reload2,
			_weapon_type_animation_chamber1,
			_weapon_type_animation_chamber2,
			_weapon_type_animation_fire1,
			_weapon_type_animation_fire2,
			_weapon_type_animation_charged1,
			_weapon_type_animation_chareged2,
			_weapon_type_animation_melee,
			_weapon_type_animation_overheat,

			k_number_of_weapon_type_animations,
		};
		enum weapon_class_animation
		{
			_weapon_class_animation_idle,
			_weapon_class_animation_gesture,
			_weapon_class_animation_turn_left,
			_weapon_class_animation_turn_right,
			_weapon_class_animation_dive_front,
			_weapon_class_animation_dive_back,
			_weapon_class_animation_dive_left,
			_weapon_class_animation_dive_right,
			_weapon_class_animation_move_front,
			_weapon_class_animation_move_back,
			_weapon_class_animation_move_left,
			_weapon_class_animation_move_right,
			_weapon_class_animation_slide_front,
			_weapon_class_animation_slide_back,
			_weapon_class_animation_slide_left,
			_weapon_class_animation_slide_right,
			_weapon_class_animation_airborne,
			_weapon_class_animation_land_soft,
			_weapon_class_animation_land_hard,
			_weapon_class_animation_unused19,
			_weapon_class_animation_throw_grenade,
			_weapon_class_animation_disarm,
			_weapon_class_animation_drop,
			_weapon_class_animation_ready,
			_weapon_class_animation_put_away,
			_weapon_class_animation_aim_still,
			_weapon_class_animation_aim_move,
			_weapon_class_animation_surprise_front,
			_weapon_class_animation_surprise_back,
			_weapon_class_animation_berserk,
			_weapon_class_animation_evade_left,
			_weapon_class_animation_evade_right,
			_weapon_class_animation_signal_move,
			_weapon_class_animation_signal_attack,
			_weapon_class_animation_warn,
			_weapon_class_animation_stunned_front,
			_weapon_class_animation_stunned_back,
			_weapon_class_animation_stunned_left,
			_weapon_class_animation_stunned_right,
			_weapon_class_animation_melee,
			_weapon_class_animation_celebrate,
			_weapon_class_animation_panic,
			_weapon_class_animation_melee_airborne,
			_weapon_class_animation_flaming,
			_weapon_class_animation_resurrect_front,
			_weapon_class_animation_resurrect_back,
			_weapon_class_animation_melee_continuous,
			_weapon_class_animation_feeding,
			_weapon_class_animation_leap_start,
			_weapon_class_animation_leap_airborne,
			_weapon_class_animation_leap_melee,
			_weapon_class_animation_zapping,
			_weapon_class_animation_unused52,
			_weapon_class_animation_unused53,
			_weapon_class_animation_unused54,

			k_number_of_weapon_class_animations,
		};

		enum unit_seat_animation : _enum
		{
			_unit_seat_animation_airborne_dead,
			_unit_seat_animation_landing_dead,
			_unit_seat_animation_acc_front_back,
			_unit_seat_animation_acc_left_right,
			_unit_seat_animation_acc_up_down,
			_unit_seat_animation_push,
			_unit_seat_animation_twist,
			_unit_seat_animation_enter,
			_unit_seat_animation_exit,
			_unit_seat_animation_look,
			_unit_seat_animation_talk,
			_unit_seat_animation_emotions,
			_unit_seat_animation_unused12,
			_unit_seat_animation_user0,
			_unit_seat_animation_user1,
			_unit_seat_animation_user2,
			_unit_seat_animation_user3,
			_unit_seat_animation_user4,
			_unit_seat_animation_user5,
			_unit_seat_animation_user6,
			_unit_seat_animation_user7,
			_unit_seat_animation_user8,
			_unit_seat_animation_user9,
			_unit_seat_animation_flying_front,
			_unit_seat_animation_flying_back,
			_unit_seat_animation_flying_left,
			_unit_seat_animation_flying_right,
			_unit_seat_animation_opening,
			_unit_seat_animation_closing,
			_unit_seat_animation_hovering,

			k_number_of_unit_seat_animations,

			// yelo animation states
			_unit_seat_animation_yelo_boarding = k_number_of_unit_seat_animations,
			_unit_seat_animation_yelo_ejecting,
			_unit_seat_animation_yelo_mounted,

			k_number_of_unit_seat_animations_yelo,
		};

		enum vehicle_animation
		{
			_vehicle_animation_steering,
			_vehicle_animation_roll,
			_vehicle_animation_throttle,
			_vehicle_animation_velocity,
			_vehicle_animation_braking,
			_vehicle_animation_ground_speed,
			_vehicle_animation_occupied,
			_vehicle_animation_unoccupied,

			k_number_of_vehicle_animations,
		};

		enum device_animation
		{
			_device_animation_position,
			_device_animation_power,

			k_number_of_device_animations,
		};

		enum first_person_weapon_animation
		{
			_first_person_weapon_animation_idle,
			_first_person_weapon_animation_posing,
			_first_person_weapon_animation_fire1,
			_first_person_weapon_animation_moving,
			_first_person_weapon_animation_overlays,
			_first_person_weapon_animation_light_off,
			_first_person_weapon_animation_light_on,
			_first_person_weapon_animation_reload_empty,
			_first_person_weapon_animation_reload_full,
			_first_person_weapon_animation_overheated,
			_first_person_weapon_animation_ready,
			_first_person_weapon_animation_put_away,
			_first_person_weapon_animation_overcharged,
			_first_person_weapon_animation_melee,
			_first_person_weapon_animation_fire2,
			_first_person_weapon_animation_overcharged_jitter,
			_first_person_weapon_animation_throw_grenade,
			_first_person_weapon_animation_ammunition,
			_first_person_weapon_animation_misfire1,
			_first_person_weapon_animation_misfire2,
			_first_person_weapon_animation_throw_overheated,
			_first_person_weapon_animation_overheating,
			_first_person_weapon_animation_overheating_again,
			_first_person_weapon_animation_enter,
			_first_person_weapon_animation_exit_empty,
			_first_person_weapon_animation_exit_full,
			_first_person_weapon_animation_overheating_exit, // o-h-exit
			_first_person_weapon_animation_overheating_enter,// o-h-s-enter

			k_number_of_first_person_weapon_animations,
		};
	};

	namespace TagGroups
	{
		struct animation_graph_weapon_type
		{
			TAG_FIELD(tag_string, label);
			TAG_PAD(int32, 4);
			TAG_TBLOCK(animations, int16); // block index to model_animation
		}; BOOST_STATIC_ASSERT( sizeof(animation_graph_weapon_type) == 0x3C ); // max count: 16
		struct animation_graph_weapon
		{
			TAG_FIELD(tag_string, name);
			TAG_FIELD(tag_string, grip_marker, "", "the marker name on the weapon to which the hand is attached (leave blank to use origin)");
			TAG_FIELD(tag_string, hand_marker, "", "the marker name on the unit to which the weapon is attached");

			////////////////////////////////////////////////////////////////
			// aiming screen bounds
			TAG_FIELD(angle, right_yaw_per_frame);
			TAG_FIELD(angle, left_yaw_per_frame);
			TAG_FIELD(int16, right_frame_count);
			TAG_FIELD(int16, left_frame_count);
			TAG_FIELD(angle, down_pitch_per_frame);
			TAG_FIELD(angle, up_pitch_per_frame);
			TAG_FIELD(int16, down_pitch_frame_count);
			TAG_FIELD(int16, up_pitch_frame_count);
			TAG_PAD(int32, 8);
			TAG_TBLOCK(animations, int16); // block index to model_animation
			TAG_BLOCK(ik_points, animation_graph_unit_seat_ik_point);
			TAG_TBLOCK(weapon_types, animation_graph_weapon_type);
		}; BOOST_STATIC_ASSERT( sizeof(animation_graph_weapon) == 0xBC ); // max count: 16
		struct animation_graph_unit_seat
		{
			TAG_FIELD(tag_string, label);

			////////////////////////////////////////////////////////////////
			// looking screen bounds
			TAG_FIELD(angle, right_yaw_per_frame);
			TAG_FIELD(angle, left_yaw_per_frame);
			TAG_FIELD(int16, right_frame_count);
			TAG_FIELD(int16, left_frame_count);
			TAG_FIELD(angle, down_pitch_per_frame);
			TAG_FIELD(angle, up_pitch_per_frame);
			TAG_FIELD(int16, down_pitch_frame_count);
			TAG_FIELD(int16, up_pitch_frame_count);
			TAG_PAD(int32, 2);
			TAG_TBLOCK(animations, int16); // block index to model_animation
			TAG_BLOCK(ik_points, animation_graph_unit_seat_ik_point);
			TAG_TBLOCK(weapons, animation_graph_weapon);
		}; BOOST_STATIC_ASSERT( sizeof(animation_graph_unit_seat) == 0x64 ); // max count: 32
		struct model_animation
		{
			TAG_FIELD(tag_string, name);
			TAG_ENUM(type);
			TAG_FIELD(int16, frame_count);
			TAG_FIELD(int16, frame_size);
			TAG_ENUM(frame_info_type);
			TAG_FIELD(int32, node_list_checksum);
			TAG_FIELD(int16, node_count);
			TAG_FIELD(int16, loop_frame_index);
			TAG_FIELD(real_fraction, weight);
			TAG_FIELD(int16, key_frame_index);
			TAG_FIELD(int16, second_key_frame_index);
			TAG_FIELD(int16, next_animation);
			struct _flags {
				TAG_FLAG16(compressed_data);
				TAG_FLAG16(world_relative);
				TAG_FLAG16(_25Hz_PAL);
			}flags; BOOST_STATIC_ASSERT( sizeof(_flags) == sizeof(word_flags) );
			TAG_FIELD(int16, sound);
			TAG_FIELD(int16, sound_frame_index);
			TAG_FIELD(sbyte, left_foot_frame_index);
			TAG_FIELD(sbyte, right_foot_frame_index);
			//////////////////////////////////////////////////////////////////////////
			// postprocessed fields (thus, not exposed to the editor)
			int16 first_animation; // if this is a permutated animation, this represents the first animation (first animation will also have this set)
			real_fraction random_fraction;
			//////////////////////////////////////////////////////////////////////////
			TAG_FIELD(tag_data, frame_info);
			TAG_FIELD(int32, node_trans_flag_data1);
			TAG_FIELD(int32, node_trans_flag_data2);
			TAG_PAD(byte, 8);
			TAG_FIELD(int32, node_rotation_flag_data1);
			TAG_FIELD(int32, node_rotation_flag_data2);
			TAG_PAD(byte, 8);
			TAG_FIELD(int32, node_scale_flag_data1);
			TAG_FIELD(int32, node_scale_flag_data2);
			TAG_PAD(byte, 4);
			TAG_FIELD(int32, offset_to_compressed_data);
			TAG_FIELD(tag_data, default_data);
			TAG_FIELD(tag_data, frame_data);
		}; BOOST_STATIC_ASSERT( sizeof(model_animation) == 0xB4 );
		struct model_animation_graph
		{
			enum { k_group_tag = 'antr' };

			TAG_BLOCK(objects, s_object_overlay);
			TAG_TBLOCK(units, animation_graph_unit_seat);
			TAG_BLOCK(weapons, animation_graph_weapon_animation);
			TAG_BLOCK(vehicles, animation_graph_vehicle_animation);
			TAG_BLOCK(devices, device_animation);
			TAG_BLOCK(unit_damage, unit_damage_animation);
			TAG_BLOCK(first_person_weapons, animation_graph_first_person_weapon_animation);
			TAG_BLOCK(sound_references, animation_graph_sound_reference);
			TAG_FIELD(real, limp_body_node_radius);
			struct _flags {
				TAG_FLAG16(compress_all_animations);
				TAG_FLAG16(force_idle_compression);
			}flags; BOOST_STATIC_ASSERT( sizeof(_flags) == sizeof(word_flags) );
			PAD16;
			TAG_BLOCK(nodes, animation_graph_node);
			TAG_TBLOCK(animations, model_animation);
		}; BOOST_STATIC_ASSERT( sizeof(model_animation_graph) == 0x80 );
	};
};