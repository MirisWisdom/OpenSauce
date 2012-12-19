/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/tag_files/tag_groups.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum unit_grenade_type : _enum
		{
			_unit_grenade_type_frag,
			_unit_grenade_type_plasma,

			// Stock number of grenade types
			k_unit_grenade_types_count,

			_unit_grenade_type_yelo2 = k_unit_grenade_types_count,	// ie, "custom 2"
			_unit_grenade_type_yelo3,								// ie, "custom 3"

			// Number of supported grenade types in OS-modified game states
			k_unit_grenade_types_count_yelo
		};

		enum game_difficulty_value
		{
			//////////////////////////////////////////////////////////////////////////
			// health
			_game_difficulty_value_enemy_damage,
			_game_difficulty_value_enemy_body_vitality,
			_game_difficulty_value_enemy_shield_vitality,
			_game_difficulty_value_enemy_shield_recharge,
			_game_difficulty_value_friend_damage,
			_game_difficulty_value_friend_body_vitality,
			_game_difficulty_value_friend_shield_vitality,
			_game_difficulty_value_friend_shield_recharge,
			_game_difficulty_value_infection_forms_toughness,
			_game_difficulty_value_health_unused9,
			//////////////////////////////////////////////////////////////////////////
			// ranged fire
			_game_difficulty_value_rate_of_fire,
			_game_difficulty_value_projectile_error,
			_game_difficulty_value_burst_error,
			_game_difficulty_value_new_target_delay,
			_game_difficulty_value_burst_separation,
			_game_difficulty_value_target_tracking,
			_game_difficulty_value_target_leading,
			_game_difficulty_value_overcharge_chance,
			_game_difficulty_value_special_fire_delay,
			_game_difficulty_value_projectile_guidance_velocity_scale,
			_game_difficulty_value_melee_delay_scale,
			_game_difficulty_value_combat_unused21, // 0x15
			//////////////////////////////////////////////////////////////////////////
			// grenades
			_game_difficulty_value_grenade_chance_scale,
			_game_difficulty_value_grenade_timer_scale,
			_game_difficulty_value_grenades_unused24, // 0x18
			_game_difficulty_value_grenades_unused25, // 0x19
			_game_difficulty_value_grenades_unused26, // 0x1A
			//////////////////////////////////////////////////////////////////////////
			// placement
			_game_difficulty_value_actor_major_upgrade_normal,
			_game_difficulty_value_actor_major_upgrade_few,
			_game_difficulty_value_actor_major_upgrade_many,

			_game_difficulty_value_unused30, // 0x1E, in Halo2, this is the vehicle ram chance
			_game_difficulty_value_unused31, // 0x1F
			_game_difficulty_value_unused32, // 0x20
			_game_difficulty_value_unused33, // 0x21
			_game_difficulty_value_unused34, // 0x22

			k_number_of_game_difficulty_values
		};
	};

	namespace GameState
	{
		struct s_game_options
		{
			UNKNOWN_TYPE(int16);
			UNKNOWN_TYPE(int16); // ?, not sure about this field
			UNKNOWN_TYPE(int16); // ?, not sure about this field
			_enum difficulty_level;
			int32 random_seed;
			char map_name[0x7F+1];
			byte unknown[0x7F+1]; // pretty sure this is just an unused string
		}; BOOST_STATIC_ASSERT( sizeof(s_game_options) == 0x10C );

		struct s_game_globals
		{
			bool map_loaded;
			bool active;
			bool players_are_double_speed;
			bool map_loading_in_progress;
			real map_loading_precentage;
			s_game_options options;
		}; BOOST_STATIC_ASSERT( sizeof(s_game_globals) == 0x114 );
	};

	namespace TagGroups
	{
		struct s_game_globals_difficulty_information
		{
			struct {
				real easy, normal, hard, impossible;
			}values[Enums::k_number_of_game_difficulty_values];
			TAG_PAD(int32, 17); // could potentially have 4 more difficulty values, with 4 bytes left over
		}; BOOST_STATIC_ASSERT( sizeof(s_game_globals_difficulty_information) <= 0x284 );

		struct s_game_globals_grenade
		{
			TAG_FIELD(int16, maximum_count);
			TAG_FIELD(int16, multiplayer_spawn_count); // PC only
			TAG_FIELD(tag_reference, throwing_effect, 'effe');
			TAG_FIELD(tag_reference, hud_interface, 'grhi');
			TAG_FIELD(tag_reference, equipment, 'eqip');
			TAG_FIELD(tag_reference, projectile, 'proj');
		}; BOOST_STATIC_ASSERT( sizeof(s_game_globals_grenade) == 0x44 );

		struct s_game_globals_tag_reference
		{
			tag_reference reference;
		};

		struct s_game_globals_multiplayer_information
		{
			TAG_FIELD(tag_reference, flag, 'item');
			TAG_FIELD(tag_reference, unit, 'unit');
			TAG_TBLOCK(vehicles, s_game_globals_tag_reference); // 20
			TAG_FIELD(tag_reference, hill_shader, 'shdr');
			TAG_FIELD(tag_reference, flag_shader, 'shdr');
			TAG_FIELD(tag_reference, ball, 'item');
			TAG_TBLOCK(sounds, s_game_globals_tag_reference); // 60
			TAG_PAD(int32, 14);
		}; BOOST_STATIC_ASSERT( sizeof(s_game_globals_multiplayer_information) == 0xA0 );

		struct s_game_globals_player_information
		{
			TAG_FIELD(tag_reference, unit, 'unit');
			PAD_TYPE(tag_reference);
			struct {
				PAD32;
				PAD32;
				PAD32;
				TAG_FIELD(real, speed);
			}walk;
			TAG_FIELD(real, double_speed_multiplier);
			struct {
				TAG_FIELD(real, forward);
				TAG_FIELD(real, backward);
				TAG_FIELD(real, sideways);
				TAG_FIELD(real, acceleration);
			}	run,
				sneak;
			TAG_FIELD(real, airborne_acceleration);
			PAD_TYPE(tag_reference);
			TAG_FIELD(real_point3d, grenade_origin);
			PAD_TYPE(real_point3d);
			struct _stun {
				TAG_FIELD(real, movement_penalty);
				TAG_FIELD(real, turning_penalty);
				TAG_FIELD(real, jumping_penalty);
				TAG_FIELD(real, minimum_time);
				TAG_FIELD(real, maximum_time);
			}stun;													// 0x80
			PAD_TYPE(real_bounds);
			struct {
				TAG_FIELD(real_bounds, idle_time);
				TAG_FIELD(real_fraction, skip_fraction);
			}first_person;
			PAD_TYPE(tag_reference);
			TAG_FIELD(tag_reference, coop_respawn_effect, 'effe'); // 0xB8
			TAG_PAD(int32, 11);
		}; BOOST_STATIC_ASSERT( sizeof(s_game_globals_player_information) == 0xF4 );

		struct s_game_globals_player_representation
		{
			TAG_FIELD(tag_reference, fp_hands, 'mod2');
			TAG_FIELD(tag_reference, base_bitmap, 'bitm');
			struct {
				TAG_FIELD(tag_reference, meter, 'metr');
				TAG_FIELD(point2d, meter_origin);
			}shield, body;
			struct {
				TAG_FIELD(tag_reference, off_on_effect, 'effe');
				TAG_FIELD(tag_reference, on_off_effect, 'effe');
			}night_vision;
			TAG_PAD(int32, 22);
		}; BOOST_STATIC_ASSERT( sizeof(s_game_globals_player_representation) == 0xC0 );

		struct s_game_globals_falling_damage
		{
			PAD64;
			TAG_FIELD(real_bounds, harmful_falling_distance);
			TAG_FIELD(tag_reference, falling_damage, 'jpt!');
			PAD64;
			TAG_FIELD(real, maximum_falling_distance);
			TAG_FIELD(tag_reference, distance_damage, 'jpt!');
			TAG_FIELD(tag_reference, vehicle_environment_collision_damage_effect, 'jpt!');
			TAG_FIELD(tag_reference, vehicle_killed_unit_damage_effect, 'jpt!');
			TAG_FIELD(tag_reference, vehicle_collision_damage, 'jpt!');
			TAG_FIELD(tag_reference, flaming_death_damage, 'jpt!');
			PAD_TYPE(tag_reference);

			struct { // multiply the tag values by 0.0071303584f, and you get these runtime values
				real maximum_falling_distance;
				real_bounds harmful_falling_distance;
			}runtime;
		}; BOOST_STATIC_ASSERT( sizeof(s_game_globals_falling_damage) == 0x98 );

		struct material_definition
		{
			TAG_PAD(int32, 25);
			TAG_PAD(int32, 12);
			struct {
				TAG_FIELD(real, ground_friction_scale, "fraction of original velocity parallel to the ground after one tick");
				TAG_FIELD(real, ground_friction_normal_k1_scale, "cosine of angle at which friction falls off");
				TAG_FIELD(real, ground_friction_normal_k0_scale, "cosine of angle at which friction is zero");
				TAG_FIELD(real, ground_depth_scale, "depth a point mass rests in the ground");
				TAG_FIELD(real, ground_damp_fraction_scale, "fraction of original velocity perpendicular to the ground after one tick");
			}vehicle_terrain_params;
			TAG_PAD(int32, 19);
			TAG_PAD(int32, 120);
			struct {
				TAG_FIELD(real, maximum_vitality);
				PAD64;
				PAD32;
				TAG_FIELD(tag_reference, effect, 'effe');
				TAG_FIELD(tag_reference, sound, 'snd!');
				TAG_PAD(int32, 6);
				TAG_BLOCK(particle_effects, breakable_surface_particle_effect);
			}breakable_surface_params;
			TAG_PAD(int32, 15);
			TAG_FIELD(tag_reference, melee_hit_sound, 'snd!');
		}; BOOST_STATIC_ASSERT( sizeof(material_definition) == 0x374 );

		struct s_game_globals
		{
			enum { k_group_tag = 'matg' };

			TAG_PAD(byte, 248);

			TAG_TBLOCK(sounds, tag_reference);
			TAG_TBLOCK(camera, tag_reference);

			TAG_PAD(tag_block,
				1); // s_game_globals_player_control

			TAG_TBLOCK(difficulty_info, s_game_globals_difficulty_information);
			TAG_TBLOCK(grenades, s_game_globals_grenade);

			TAG_PAD(tag_block,
				1 + // s_game_globals_rasterizer_data
				1); // s_game_globals_interface_tag_references

			TAG_TBLOCK(weapons_list, s_game_globals_tag_reference);
			TAG_TBLOCK(cheat_powerups, s_game_globals_tag_reference);
			TAG_TBLOCK(multiplayer_info, s_game_globals_multiplayer_information);
			TAG_TBLOCK(player_info, s_game_globals_player_information);
			TAG_TBLOCK(player_representation, s_game_globals_player_representation);
			TAG_TBLOCK(falling_damage, s_game_globals_falling_damage);
			TAG_BLOCK(materials, material_definition);
			TAG_BLOCK(playlist_members, playlist_autogenerate_choice);
		}; BOOST_STATIC_ASSERT( sizeof(s_game_globals) == 0x1AC );
	};
};