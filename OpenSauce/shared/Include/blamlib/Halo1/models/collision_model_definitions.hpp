/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <TagGroups/Halo1/TagGroupDefinitions.hpp>

#include <blamlib/Halo1/models/model_definitions.hpp>

namespace Yelo
{
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
		struct bsp3d_node
		{
			TAG_FIELD(int32, plane);
			TAG_FIELD(int32, back_child);
			TAG_FIELD(int32, front_child);
		};
		struct collision_leaf
		{
			TAG_FIELD(word_flags, flags);
			TAG_FIELD(int16, reference_count, "bsp2d");
			TAG_FIELD(int32, first_reference, "bsp2d");
		};
		struct bsp2d_reference
		{
			TAG_FIELD(int32, plane);
			TAG_FIELD(int32, bsp2d_node);
		};
		struct bsp2d_node
		{
			TAG_FIELD(real_plane2d, plane);
			TAG_FIELD(int32, left_child);
			TAG_FIELD(int32, rigt_child);
		};
		struct collision_surface
		{
			TAG_FIELD(int32, plane);
			TAG_FIELD(int32, first_edge);
			TAG_FIELD(byte_flags, flags);
			TAG_FIELD(sbyte, breakable_surface);
			TAG_FIELD(int16, material);
		};
		struct collision_edge
		{
			TAG_FIELD(int32, start_vertex);
			TAG_FIELD(int32, end_vertex);
			TAG_FIELD(int32, forward_edge);
			TAG_FIELD(int32, reverse_edge);
			TAG_FIELD(int32, left_surface);
			TAG_FIELD(int32, right_surface);
		};
		struct collision_vertex
		{
			TAG_FIELD(real_point3d, point);
			TAG_FIELD(int32, first_edge);
		};
		struct collision_bsp
		{
			TAG_TBLOCK(bsp3d_nodes, bsp3d_node);
			TAG_TBLOCK(planes, real_plane3d);
			TAG_TBLOCK(leaves, collision_leaf);
			struct {
				TAG_TBLOCK(references, bsp2d_reference);
				TAG_TBLOCK(nodes, bsp2d_node);
			}bsp2d;
			TAG_TBLOCK(surfaces, collision_surface);
			TAG_TBLOCK(edges, collision_edge);
			TAG_TBLOCK(vertices, collision_vertex);
		};


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
	};
};