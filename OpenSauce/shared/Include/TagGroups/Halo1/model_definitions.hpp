/*
    Yelo: Open Sauce SDK

    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
		};
	};

	namespace TagGroups
	{
		struct s_model_marker_instance
		{
			TAG_FIELD(byte, region_index);
			TAG_FIELD(byte, permutation_index);
			TAG_FIELD(byte, node_index);
			PAD8;
			TAG_FIELD(real_point3d, translation);
			TAG_FIELD(real_quaternion, rotation);
		}; BOOST_STATIC_ASSERT( sizeof(s_model_marker_instance) == 0x20 ); // max count: 32

		struct s_model_markers
		{
			TAG_FIELD(tag_string, name);
			TAG_FIELD(int16, magic_identifier);
			PAD16;
			TAG_PAD(int32, 4);
			TAG_TBLOCK(instances, s_model_marker_instance);
		}; BOOST_STATIC_ASSERT( sizeof(s_model_markers) == 0x40 ); // max count: 256

		struct s_model_node
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
		}; BOOST_STATIC_ASSERT( sizeof(s_model_node) == 0x9C ); // max count: 64

		struct s_model_region_permutation_marker
		{
			TAG_FIELD(tag_string, name);
			TAG_FIELD(int16, node_index);
			PAD16;
			TAG_FIELD(real_quaternion, rotation);
			TAG_FIELD(real_point3d, translation);
			TAG_PAD(int32, 4);
		}; BOOST_STATIC_ASSERT( sizeof(s_model_region_permutation_marker) == 0x50 ); // max count: 64

		struct s_gbxmodel_region_permutation
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
			TAG_TBLOCK(markers, s_model_region_permutation_marker);
		}; BOOST_STATIC_ASSERT( sizeof(s_gbxmodel_region_permutation) == 0x58 ); // max count: 32

		struct s_gbxmodel_region
		{
			TAG_FIELD(tag_string, name);
			TAG_PAD(int32, 8);
			TAG_TBLOCK(permutations, s_gbxmodel_region_permutation);
		}; BOOST_STATIC_ASSERT( sizeof(s_gbxmodel_region) == 0x4C ); // max count: 32


		struct s_model_triangle
		{
			TAG_FIELD(int16, vertex0_index);
			TAG_FIELD(int16, vertex1_index);
			TAG_FIELD(int16, vertex2_index);
		}; BOOST_STATIC_ASSERT( sizeof(s_model_triangle) == 0x6 ); // max count: 65535

		struct s_gbxmodel_geometry_part
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
			TAG_TBLOCK(triangles, s_model_triangle);
			TAG_PAD(int32, 5);
			TAG_PAD(int32, 4);
			TAG_PAD(byte, 1);
			TAG_PAD(byte, 1);
			TAG_PAD(byte, 1);
			TAG_PAD(byte, 1);
			TAG_PAD(int32, 6);
		}; BOOST_STATIC_ASSERT( sizeof(s_gbxmodel_geometry_part) == 0x84 ); // max count: 32

		struct s_gbxmodel_geometry
		{
			struct __flags
			{
				TAG_FLAG(unused);
			}; BOOST_STATIC_ASSERT( sizeof(__flags) == sizeof(long_flags) );

			TAG_FIELD(__flags, flags);
			TAG_PAD(int32, 8);
			TAG_TBLOCK(parts, s_gbxmodel_geometry_part);
		}; BOOST_STATIC_ASSERT( sizeof(s_gbxmodel_geometry) == 0x30 ); // max count: 256

		struct s_model_shader_reference
		{
			TAG_FIELD(tag_reference, shader, "shdr");
			TAG_FIELD(int16, permutation);
			PAD16;
			TAG_PAD(int32, 3);
		}; BOOST_STATIC_ASSERT( sizeof(s_model_shader_reference) == 0x20 ); // max count: 64

		struct s_gbxmodel_definition
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
			TAG_TBLOCK(markers, s_model_markers);
			TAG_TBLOCK(nodes, s_model_node);
			TAG_TBLOCK(regions, s_gbxmodel_region);
			TAG_TBLOCK(geometries, s_gbxmodel_geometry);
			TAG_TBLOCK(shaders, s_model_shader_reference);
		}; BOOST_STATIC_ASSERT( sizeof(s_gbxmodel_definition) == 0xE8 ); // max count: 1



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
			TAG_FIELD(long_flags, flags);
			PAD32;
			TAG_FIELD(real, damage_threshold);
			TAG_PAD(int32, 3);
			TAG_FIELD(tag_reference, destroyed_effect, 'effe');
			TAG_TBLOCK(permutations, tag_string);
		}; BOOST_STATIC_ASSERT( sizeof(collision_model_region) == 0x54 );

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
			TAG_BLOCK(units, animation_graph_unit_seat);
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
