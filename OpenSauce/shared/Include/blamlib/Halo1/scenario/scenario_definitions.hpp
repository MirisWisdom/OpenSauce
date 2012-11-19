/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/ai/ai_scenario_definitions.hpp>
#include <blamlib/Halo1/cutscene/recorded_animation_definitions.hpp>
#include <blamlib/Halo1/hs/hs_scenario_definitions.hpp>

#include <blamlib/Halo1/tag_files/tag_groups.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_maximum_skies_per_scenario = 32,
			k_maximum_structure_bsps_per_scenario = 16,

			// How much OS-upgrades [k_maximum_structure_bsps_per_scenario]
			k_maximum_structure_bsps_per_scenario_upgrade = k_maximum_structure_bsps_per_scenario * 2,

			k_maximum_object_names_per_scenario = 512,
		};

		enum scenario_netgame_type
		{
			_scenario_netgame_type_none,
			_scenario_netgame_type_ctf,
			_scenario_netgame_type_slayer,
			_scenario_netgame_type_oddball,
			_scenario_netgame_type_koth,
			_scenario_netgame_type_race,
			_scenario_netgame_type_terminator,
			_scenario_netgame_type_stub,
			_scenario_netgame_type_ignored1,
			_scenario_netgame_type_ignored2,
			_scenario_netgame_type_ignored3,
			_scenario_netgame_type_ignored4,
			_scenario_netgame_type_all_games,
			_scenario_netgame_type_all_except_ctf,
			_scenario_netgame_type_all_except_ctf_y_race,

			_scenario_netgame_type
		};

		enum scenario_netgame_flag_type
		{
			_scenario_netgame_flag_ctf_flag,
			_scenario_netgame_flag_ctf_vehicle,
			_scenario_netgame_flag_oddball_ball_spawn,
			_scenario_netgame_flag_race_track,
			_scenario_netgame_flag_race_vehicle,
			_scenario_netgame_flag_vegas_bank,
			_scenario_netgame_flag_teleporter_source,
			_scenario_netgame_flag_teleporter_target,
			_scenario_netgame_flag_hill,

			_scenario_netgame_flag_type,
		};
	};

	namespace TagGroups
	{
		struct scenario_starting_profile
		{
			TAG_FIELD(tag_string, name);
			TAG_FIELD(real_fraction, starting_health_damage);
			TAG_FIELD(real_fraction, starting_shield_damage);
			struct {
				TAG_FIELD(tag_reference, weapon, 'weap');
				TAG_FIELD(int16, rounds_loaded);
				TAG_FIELD(int16, rounds_total);
			}weapons[2];
			TAG_FIELD(byte, grenade_counts[4]);
			TAG_PAD(int32, 5);
		}; BOOST_STATIC_ASSERT( sizeof(scenario_starting_profile) == 0x68 );

		struct scenario_player
		{
			TAG_FIELD(real_point3d, position);
			TAG_FIELD(angle, facing);
			TAG_FIELD(int16, team_designator);
			TAG_FIELD(int16, bsp_index);
			TAG_ENUM(game_types, Enums::scenario_netgame_type)[4];

			TAG_PAD(int32, 6);
		}; BOOST_STATIC_ASSERT( sizeof(scenario_player) == 0x34 );

		struct scenario_trigger_volume
		{
			// _scenario_trigger_volume_type_? = 0
			// _scenario_trigger_volume_type_? = 1
			TAG_FIELD(_enum, type); // This is actually treated as a skip-4 field in Halo1's defs
			PAD16;
			TAG_FIELD(tag_string, name);
			TAG_FIELD(real_vector3d, data0);
			TAG_FIELD(real_vector3d, forward);
			TAG_FIELD(real_vector3d, up);
			TAG_FIELD(real_point3d, position);
			TAG_FIELD(real_vector3d, transform);
		}; BOOST_STATIC_ASSERT( sizeof(scenario_trigger_volume) == 0x60 );

		struct scenario_netpoint
		{
			TAG_FIELD(real_point3d, position);
			TAG_FIELD(angle, facing_degrees);
			TAG_ENUM(type, Enums::scenario_netgame_flag_type);
			TAG_FIELD(int16, team_index);
			TAG_FIELD(tag_reference, weapon_group, 'itmc');
			TAG_PAD(int32, 28);
		}; BOOST_STATIC_ASSERT( sizeof(scenario_netpoint) == 0x94 );

		struct scenario_netgame_equipment
		{
			TAG_FIELD(long_flags, flags);
			TAG_ENUM(game_types, Enums::scenario_netgame_type)[4];
			TAG_FIELD(int16, team_index);
			TAG_FIELD(int16, respawn_time);
			TAG_PAD(int32, 12);
			TAG_FIELD(real_point3d, position);
			TAG_FIELD(angle, facing_degrees);
			TAG_FIELD(tag_reference, item_collection, 'itmc');
			TAG_PAD(int32, 12);
		}; BOOST_STATIC_ASSERT( sizeof(scenario_netgame_equipment) == 0x90 );

		struct scenario_starting_equipment
		{
			TAG_FIELD(long_flags, flags);
			TAG_ENUM(game_types, Enums::scenario_netgame_type)[4];
			TAG_PAD(int32, 12);
			TAG_FIELD(tag_reference, item_collections, 'itmc')[6];
			TAG_PAD(int32, 12);
		}; BOOST_STATIC_ASSERT( sizeof(scenario_starting_equipment) == 0xCC );


		struct scenario_cutscene_flag
		{
			PAD32;
			TAG_FIELD(tag_string, name);
			TAG_FIELD(real_point3d, position);
			TAG_FIELD(real_euler_angles2d, facing);
			TAG_PAD(int32, 9);
		}; BOOST_STATIC_ASSERT( sizeof(scenario_cutscene_flag) == 0x5C );

		struct scenario_cutscene_camera_point
		{
			PAD32;
			TAG_FIELD(tag_string, name);
			PAD32;
			TAG_FIELD(real_point3d, position);
			TAG_FIELD(real_euler_angles3d, orientation);
			TAG_FIELD(angle, field_of_view);
			TAG_PAD(int32, 9);
		}; BOOST_STATIC_ASSERT( sizeof(scenario_cutscene_camera_point) == 0x68 );

		struct scenario_structure_bsps
		{
			PAD32;
			TAG_FIELD(int32, bsp_data_size);
			TAG_PAD(int32, 2);
			TAG_FIELD(tag_reference, structure_bsp, 'sbsp');
		}; BOOST_STATIC_ASSERT( sizeof(scenario_structure_bsps) == 0x20 ); // max count: 32


		struct scenario
		{
			enum { k_group_tag = 'scnr' };

			TAG_FIELD(tag_reference, _dont_use);
			TAG_FIELD(tag_reference, _wont_use);
			TAG_FIELD(tag_reference, _cant_use);

			TAG_TBLOCK_(skies, tag_reference);

			PAD16; // type
			PAD16; // flags

			TAG_PAD(tag_block, 1); // scenario_child_scenario_reference
			PAD32; // local north
			TAG_PAD(int32, 39); // tag_data + 136
			TAG_PAD(tag_block,
				1 + // predicted_resource_block
				1); // scenario_function
			TAG_PAD(tag_data, 1); // editor_scenario_data
			TAG_PAD(tag_block, 1); // editor_comment_definition
			TAG_PAD(tag_block, 1); // Halo2. scenario_environment_object
			TAG_PAD(int32, 53); // 212

			TAG_PAD(tag_block,
				1 + // scenario_object_name

				1 + // s_scenario_scenery
				1 + // scenario_object_palette_entry

				1 + // s_scenario_biped
				1 + // scenario_object_palette_entry

				1 + // s_scenario_vehicle
				1 + // scenario_object_palette_entry

				1 + // s_scenario_equipment
				1 + // scenario_object_palette_entry

				1 + // s_scenario_weapon
				1 + // scenario_object_palette_entry

				1 + // scenario_device_group

				1 + // s_scenario_machine
				1 + // scenario_object_palette_entry

				1 + // s_scenario_control
				1 + // scenario_object_palette_entry

				1 + // s_scenario_light_fixture
				1 + // scenario_object_palette_entry

				1 + // s_scenario_sound_scenery
				1 + // scenario_object_palette_entry

				1 + // Halo2. s_scenario_light
				1   // Halo2. scenario_object_palette_entry
				);

			TAG_PAD(int32, 15); // 60

			TAG_TBLOCK(player_starting_profiles, scenario_starting_profile);
			TAG_TBLOCK(player_starting_locations, scenario_player);
			TAG_TBLOCK(trigger_volumes, scenario_trigger_volume);
			TAG_TBLOCK(recorded_animations, recorded_animation_definition);
			TAG_TBLOCK(netgame_flags, scenario_netpoint);
			TAG_TBLOCK(netgame_equipment, scenario_netgame_equipment);
			TAG_TBLOCK(starting_equipment, scenario_starting_equipment);
			TAG_PAD(tag_block,
				1 + // scenario_bsp_switch_trigger_volume
				1 + // scenario_decal
				1 + // scenario_decal_palette_entry
				1   // scenario_detail_object_collection_palette_entry
				);

			TAG_PAD(int32, 9); // 36

			TAG_PAD(tag_block,
				1 + // Halo2. style_palette_entry
				1 + // Halo2. squad_group_definition
				1 + // Halo2. squad_definition
				1   // Halo2. zone_definition
				);
			TAG_PAD(tag_block,
				1 + // actor_palette_entry
				1 + // encounter_definition
				1 + // ai_command_list_definition
				1 + // ai_animation_reference_definition
				1 + // ai_script_reference_definition
				1 + // ai_recording_reference_definition
				1   // ai_conversation
				);

			TAG_FIELD(tag_data, hs_syntax_data);
			TAG_FIELD(tag_data, hs_string_data);

			TAG_TBLOCK(scripts, hs_script);
			TAG_TBLOCK(globals, hs_global_internal);
			TAG_TBLOCK(references, hs_tag_reference);
			TAG_TBLOCK(source_files, hs_source_file);
			
			TAG_PAD(tag_block, 1); // Halo2. cs_script_data
			TAG_PAD(tag_block, 1);

			TAG_TBLOCK(cutscene_flags, scenario_cutscene_flag);
			TAG_TBLOCK(cutscene_camera_points, scenario_cutscene_camera_point);
			TAG_PAD(tag_block,
				1 // s_scenario_cutscene_title
				);

			TAG_PAD(int32, 27); // 108
			TAG_PAD(tag_reference, 3);
			TAG_TBLOCK(structure_bsps, scenario_structure_bsps);

			// Get the tag reference we redefined for users to reference yelo definitions
			tag_reference& GetYeloReferenceHack()				{ return _dont_use; }
			tag_reference const& GetYeloReferenceHack() const	{ return _dont_use; }

		}; BOOST_STATIC_ASSERT( sizeof(scenario) == 0x5B0 );
	};
};