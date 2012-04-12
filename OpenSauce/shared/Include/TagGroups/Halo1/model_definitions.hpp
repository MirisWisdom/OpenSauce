/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <TagGroups/Halo1/TagGroupDefinitions.hpp>
#include <TagGroups/Halo1/structure_bsp_definitions.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_maximum_regions_per_model = 8,

			k_maximum_nodes_per_model = 64,
			k_maximum_nodes_per_model_halo_stock = 44, // 'Cause Xbox1 has limits

			k_number_of_rows_per_node_matrix = 4,
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
			_weapon_type_animation
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
			_weapon_class_animation,

			// When used in a regular unit (e.g. elite) this is the 'infect-start' animation
			// When used in an infected unit (e.g. combat form elite) this is the 'infect-end' animation
			_weapon_class_animation_yelo_infect = _weapon_class_animation_unused52,
		};

		enum unit_seat_animation
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
			_unit_seat_animation
		};
	};

	namespace Flags
	{
		enum damage_region_flags : long_flags
		{
			_damage_region_lives_unit_object_dies_bit,
			_damage_region_forces_object_to_die_bit,
			_damage_region_dies_when_object_dies_bit,
			_damage_region_dies_when_object_is_damaged_bit,
			_damage_region_disappears_when_shield_is_off_bit,
			_damage_region_inhibits_melee_attack_bit,
			_damage_region_inhibits_weapon_attack_bit,
			_damage_region_inhibits_walking_bit,
			_damage_region_forces_drop_weapon_bit,
			_damage_region_causes_head_maimed_scream_bit,
		};
	};

	namespace TagGroups
	{
		struct model_marker_instance
		{
			TAG_FIELD(byte, region_index);
			TAG_FIELD(byte, permutation_index);
			TAG_FIELD(byte, node_index);
			PAD8;
			TAG_FIELD(real_point3d, translation);
			TAG_FIELD(real_quaternion, rotation);
		}; BOOST_STATIC_ASSERT( sizeof(model_marker_instance) == 0x20 ); // max count: 32
		struct model_markers
		{
			TAG_FIELD(tag_string, name);
			TAG_FIELD(int16, magic_identifier);
			PAD16;
			TAG_PAD(int32, 4);
			TAG_TBLOCK(instances, model_marker_instance);
		}; BOOST_STATIC_ASSERT( sizeof(model_markers) == 0x40 ); // max count: 256

		struct model_node
		{
			TAG_FIELD(tag_string, name);
			TAG_FIELD(int16, next_sibling_node_index);
			TAG_FIELD(int16, first_child_node_index);
			TAG_FIELD(int16, parent_node_index);
			PAD16;
			TAG_FIELD(real_point3d, default_translation);
			TAG_FIELD(real_quaternion, default_rotation);
			TAG_FIELD(real, node_distance_from_parent);
			TAG_PAD(int32, 8);
			TAG_PAD(int32, 13);
		}; BOOST_STATIC_ASSERT( sizeof(model_node) == 0x9C ); // max count: 64

		struct model_region_permutation_marker
		{
			TAG_FIELD(tag_string, name);
			TAG_FIELD(int16, node_index);
			PAD16;
			TAG_FIELD(real_quaternion, rotation);
			TAG_FIELD(real_point3d, translation);
			TAG_PAD(int32, 4);
		}; BOOST_STATIC_ASSERT( sizeof(model_region_permutation_marker) == 0x50 ); // max count: 64
		struct gbxmodel_region_permutation
		{
			struct __flags
			{
				TAG_FLAG(cannot_be_chosen_randomly);
			}; BOOST_STATIC_ASSERT( sizeof(__flags) == sizeof(long_flags) );

			TAG_FIELD(tag_string, name);
			TAG_FIELD(__flags, flags);
			TAG_PAD(int32, 7);
			TAG_FIELD(int16, super_low);
			TAG_FIELD(int16, low);
			TAG_FIELD(int16, medium);
			TAG_FIELD(int16, high);
			TAG_FIELD(int16, super_high);
			PAD16;
			TAG_TBLOCK(markers, model_region_permutation_marker);
		}; BOOST_STATIC_ASSERT( sizeof(gbxmodel_region_permutation) == 0x58 ); // max count: 32
		struct gbxmodel_region
		{
			TAG_FIELD(tag_string, name);
			TAG_PAD(int32, 8);
			TAG_TBLOCK(permutations, gbxmodel_region_permutation);
		}; BOOST_STATIC_ASSERT( sizeof(gbxmodel_region) == 0x4C ); // max count: 32


		struct model_triangle
		{
			TAG_FIELD(int16, vertex0_index);
			TAG_FIELD(int16, vertex1_index);
			TAG_FIELD(int16, vertex2_index);
		}; BOOST_STATIC_ASSERT( sizeof(model_triangle) == 0x6 ); // max count: 65535
		struct gbxmodel_geometry_part
		{
			struct __flags
			{
				TAG_FLAG(stripped_internal);
				TAG_FLAG(zoner);
			}; BOOST_STATIC_ASSERT( sizeof(__flags) == sizeof(long_flags) );

			TAG_FIELD(__flags, flags);
			TAG_FIELD(int16, shader_index);
			TAG_FIELD(byte, prev_filthy_part_index);
			TAG_FIELD(byte, next_filthy_part_index);
			TAG_FIELD(int16, centroid_primary_node);
			TAG_FIELD(int16, centroid_secondary_node);
			TAG_FIELD(real_fraction, centroid_primary_weight);
			TAG_FIELD(real_fraction, centroid_secondary_weight);
			TAG_FIELD(real_point3d, centroid);
			TAG_TBLOCK(uncompressed_vertices, Rasterizer::model_vertex_uncompressed); // max count: 65535
			TAG_TBLOCK(compressed_vertices, Rasterizer::model_vertex_compressed); // max count: 65535
			TAG_TBLOCK(triangles, model_triangle);
			Rasterizer::rasterizer_triangle_buffer triangle_buffer;
			Rasterizer::rasterizer_vertex_buffer vertex_buffer;
			TAG_PAD(byte, 1);
			TAG_PAD(byte, 1);
			TAG_PAD(byte, 1);
			sbyte num_nodes;
			TAG_PAD(int32, 6);
		}; BOOST_STATIC_ASSERT( sizeof(gbxmodel_geometry_part) == 0x84 ); // max count: 32
		struct gbxmodel_geometry
		{
			struct __flags
			{
				TAG_FLAG(unused);
			}; BOOST_STATIC_ASSERT( sizeof(__flags) == sizeof(long_flags) );

			TAG_FIELD(__flags, flags);
			TAG_PAD(int32, 8);
			TAG_TBLOCK(parts, gbxmodel_geometry_part);
		}; BOOST_STATIC_ASSERT( sizeof(gbxmodel_geometry) == 0x30 ); // max count: 256

		struct model_shader_reference
		{
			TAG_FIELD(tag_reference, shader, "shdr");
			TAG_FIELD(int16, permutation);
			PAD16;
			TAG_PAD(int32, 3);
		}; BOOST_STATIC_ASSERT( sizeof(model_shader_reference) == 0x20 ); // max count: 64
		struct gbxmodel_definition
		{
			enum { k_group_tag = 'mod2' };

			struct __flags
			{
				TAG_FLAG(blend_shared_normals);
				TAG_FLAG(parts_have_local_nodes);
				TAG_FLAG(ignore_skinning);
			}; BOOST_STATIC_ASSERT( sizeof(__flags) == sizeof(long_flags) );

			TAG_FIELD(__flags, flags);
			TAG_FIELD(int32, node_list_checksum);
			TAG_FIELD(real, super_high_detail_cutoff, "pixels");
			TAG_FIELD(real, high_detail_cutoff, "pixels");
			TAG_FIELD(real, medium_detail_cutoff, "pixels");
			TAG_FIELD(real, low_detail_cutoff, "pixels");
			TAG_FIELD(real, super_low_cutoff, "pixels");
			TAG_FIELD(int16, super_high_detail_node_count, "nodes");
			TAG_FIELD(int16, high_detail_node_count, "nodes");
			TAG_FIELD(int16, medium_detail_node_count, "nodes");
			TAG_FIELD(int16, low_detail_node_count, "nodes");
			TAG_FIELD(int16, super_low_detail_node_count, "nodes");
			PAD16;
			TAG_PAD(int32, 2);
			TAG_FIELD(real, base_map_u_scale, "", "0 defaults to 1");
			TAG_FIELD(real, base_map_v_scale, "", "0 defaults to 1");
			TAG_PAD(int32, 29);
			TAG_TBLOCK(markers, model_markers);
			TAG_TBLOCK(nodes, model_node);
			TAG_TBLOCK(regions, gbxmodel_region);
			TAG_TBLOCK(geometries, gbxmodel_geometry);
			TAG_TBLOCK(shaders, model_shader_reference);
		}; BOOST_STATIC_ASSERT( sizeof(gbxmodel_definition) == 0xE8 ); // max count: 1



		struct s_body_damage_resistance
		{
			TAG_FIELD(real, maximum_vitality);

			TAG_FIELD(real, system_shock);
			TAG_PAD(int32, 6);

			TAG_PAD(int32, 7);

			TAG_FIELD(real_fraction, friendly_damage_resistance);
			TAG_PAD(int32, 2);
			TAG_PAD(int32, 8);

			TAG_FIELD(tag_reference, localized_damage_effect, 'effe');
			TAG_FIELD(real, area_damage_effect_threshold);
			TAG_FIELD(tag_reference, area_damage_effect, 'effe');

			TAG_FIELD(real, damaged_threshold);
			TAG_FIELD(tag_reference, damaged_effect, 'effe');
			TAG_FIELD(tag_reference, depleted_effect, 'effe');
			TAG_FIELD(real, destroyed_threshold);
			TAG_FIELD(tag_reference, destroyed_effect, 'effe');
		}; BOOST_STATIC_ASSERT( sizeof(s_body_damage_resistance) == 0xC4 );
		struct s_shield_damage_resistance
		{
			TAG_FIELD(real, maximum_vitality);

			PAD16;
			TAG_ENUM(material_type, Enums::material_type);
			TAG_PAD(int32, 6);

			TAG_ENUM(failure_function, transition_function);
			PAD16;
			TAG_FIELD(real_fraction, failure_threshold);
			TAG_FIELD(real_fraction, failing_leak_fraction);
			TAG_PAD(int32, 4);

			TAG_FIELD(real, minimum_stun_damage);
			TAG_FIELD(real, stun_time, "seconds");
			TAG_FIELD(real, recharge_time, "seconds");
			TAG_PAD(int32, 4);
			TAG_PAD(int32, 24);

			TAG_FIELD(real, damaged_threshold);
			TAG_FIELD(tag_reference, damaged_effect, 'effe');
			TAG_FIELD(tag_reference, depleted_effect, 'effe');

			TAG_FIELD(tag_reference, recharging_effect, 'effe');
		}; BOOST_STATIC_ASSERT( sizeof(s_shield_damage_resistance) == 0xEC );
		struct s_damage_resistance
		{
			TAG_FIELD(long_flags, flags);
			TAG_FIELD(int16, indirect_damage_material, collision_model_material);
			PAD16;
			s_body_damage_resistance body_damage_resistance;
			s_shield_damage_resistance shield_damage_resistance;
			TAG_PAD(int32, 3);
			TAG_PAD(int32, 28);
		}; BOOST_STATIC_ASSERT( sizeof(s_damage_resistance) == 0x234 );

		struct collision_model_material
		{
			TAG_FIELD(tag_string, name);
			TAG_FIELD(long_flags, flags);
			TAG_ENUM(material_type, Enums::material_type);
			PAD16;
			struct {
				TAG_FIELD(real_fraction, leak_percentage); // unused for 'body'?
				TAG_FIELD(real, damaged_multiplier);

				// either an unused real field or a post-process field
				PAD32;
				PAD32;
			}shield, body;
		}; BOOST_STATIC_ASSERT( sizeof(collision_model_material) == 0x48 );
		struct collision_model_region
		{
			TAG_FIELD(tag_string, name);
			TAG_FIELD(Flags::damage_region_flags, flags);
			PAD32;
			TAG_FIELD(real, damage_threshold);
			TAG_PAD(int32, 3);
			TAG_FIELD(tag_reference, destroyed_effect, 'effe');
			TAG_TBLOCK(permutations, tag_string);
		}; BOOST_STATIC_ASSERT( sizeof(collision_model_region) == 0x54 ); // aka damage region
		struct collision_model_pathfinding_sphere
		{
			TAG_FIELD(int16, node, collision_model_node);
			PAD16;
			TAG_PAD(int32, 3);
			TAG_FIELD(real_point3d, center);
			TAG_FIELD(real, radius);
		}; BOOST_STATIC_ASSERT( sizeof(collision_model_pathfinding_sphere) == 0x20 );
		struct collision_model_node
		{
			TAG_FIELD(tag_string, name);
			TAG_FIELD(int16, region, collision_model_region);
			TAG_FIELD(int16, parent_node, collision_model_node);
			TAG_FIELD(int16, next_sibling_node, collision_model_node);
			TAG_FIELD(int16, first_child_node, collision_model_node);
			TAG_PAD(tag_block, 1);
			TAG_TBLOCK(bsp, collision_bsp);
		}; BOOST_STATIC_ASSERT( sizeof(collision_model_node) == 0x40 );
		struct collision_model_definition
		{
			enum { k_group_tag = 'coll' };

			s_damage_resistance damage_resistance;

			TAG_TBLOCK(materials, collision_model_material);
			TAG_TBLOCK(regions, collision_model_region);
			TAG_PAD(tag_block, 1); // damage_modifiers_block is unused

			TAG_PAD(int32, 4);
			struct s_pathfinding {
				struct s_box {
					TAG_FIELD(real_bounds, x);
					TAG_FIELD(real_bounds, y);
					TAG_FIELD(real_bounds, z);
				}box;
				TAG_TBLOCK(spheres, collision_model_pathfinding_sphere);
			}pathfinding;
			TAG_TBLOCK(nodes, collision_model_node);
		}; BOOST_STATIC_ASSERT( sizeof(collision_model_definition) == 0x298 );




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
			TAG_BLOCK(weapon_types, animation_graph_weapon_type);
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
			TAG_BLOCK(animations, unit_seat_animation);
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

			TAG_BLOCK(objects, animation_graph_object_overlay);
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
