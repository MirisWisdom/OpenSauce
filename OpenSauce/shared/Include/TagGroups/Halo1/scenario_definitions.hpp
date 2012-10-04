/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/Halo1/scenario/scenario_definitions.hpp>

#include <TagGroups/Halo1/TagGroupDefinitions.hpp>

namespace Yelo
{
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


		struct hs_scenario_data_base
		{
			struct _script {
				TAG_ENUM(type, Enums::hs_script_type);
				TAG_ENUM(return_type, Enums::hs_type);
				TAG_FIELD(datum_index, root_expression_index);
			};
			struct _global {
				TAG_ENUM(type, Enums::hs_type);
				PAD16;
				PAD32;
				TAG_FIELD(datum_index, initialization_expression_index);
			};
			TAG_FIELD(tag_string, name);
			union {
				TAG_PAD(int32, 15); // 60
				_script script;
				_global global;
			};
		};
		struct hs_script			: hs_scenario_data_base	{}; BOOST_STATIC_ASSERT( sizeof(hs_script) == 0x5C );
		struct hs_global_internal	: hs_scenario_data_base	{}; BOOST_STATIC_ASSERT( sizeof(hs_global_internal) == 0x5C );
		struct hs_tag_reference
		{
			TAG_PAD(int32, 6); // 24
			TAG_FIELD(tag_reference, reference);
		}; BOOST_STATIC_ASSERT( sizeof(hs_tag_reference) == 0x28 );
		struct hs_source_file
		{
			TAG_FIELD(tag_string, name);
			TAG_FIELD(tag_data, source);
		}; BOOST_STATIC_ASSERT( sizeof(hs_source_file) == 0x34 );

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
			TAG_FIELD(tag_reference, structure_bsp, "sbsp");
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
			TAG_PAD(tag_block,
				1 + // scenario_trigger_volume
				1 + // recorded_animation_definition
				1 + // scenario_netpoint
				1 + // scenario_netgame_equipment
				1 + // scenario_starting_equipment
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
				1 + // ai_command_list
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