/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/game/game_globals.hpp>
#include <YeloLib/tag_files/tag_groups_base_yelo.hpp>

namespace Yelo
{
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

		struct s_game_globals_player_representation // game_globals_first_person_interface
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

			long_enum language;

			TAG_PAD(byte, 244);

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
			TAG_TBLOCK(materials, material_definition);
			TAG_BLOCK(playlist_members, playlist_autogenerate_choice);
		}; BOOST_STATIC_ASSERT( sizeof(s_game_globals) == 0x1AC );
	};
};