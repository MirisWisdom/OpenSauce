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

namespace Yelo
{
	namespace Scripting
	{
		#pragma region script functions related
		// possible overwrite ables
		// game_is_cooperative
		// core_load_at_startup
		// net_graph_clear
		// net_graph_show
		// play_update_history
		// message_metrics_dump
		// error_overflow_suppression
		// set_gamma

		// null params
		// ai_select
		// core_load_name
		// core_load_name_at_startup
		// core_save_name
		// sound_impulse_predict
		// player_effect_set_max_vibrate
		// config_one_control
		// set_gamepad_yaw_scale
		// set_gamepad_pitch_scale
		// sv_log_note (client only)
		static const uint16 hs_null_functions[] = {
			// these indices are ordered in a way that they won't interfere with 
			// development when replacing the HEK's scripting, unless the user 
			// adds a bunch of new functions

			0x104, // profile_service_clear_timers
			0x105, // profile_service_dump_timers
			0x110, // texture_cache_flush
			0x111, // sound_cache_flush
			0x113, // debug_memory
			0x114, // debug_memory_by_file
			0x115, // debug_memory_for_file
			0x116, // debug_tags
			0x117, // profile_reset
			0x118, // profile_dump
			0x119, // profile_activate
			0x11A, // profile_deactivate
			0x11B, // profile_graph_toggle
			0x134, // attract_mode_start
			0x184, // hammer_begin
			0x185, // hammer_stop
			0x154, // debug_sounds_wet
			0x186, // network_server_dump
			0x187, // network_client_dump
			0x18B, // show_player_update_stats
			0x18C, // message_metrics_clear
			0x1A5, // time_code_show
			0x1A6, // time_code_start
			0x1A7, // time_code_reset
			0x1B8, // delete_save_game_files

			//////////////////////////////////////////////////////////////////////////
			// these aren't null functions, but highly doubt they are used in anyone's scripts, so we're fine
			0x18A, // play_update_history*
			0x18D, // message_metrics_dump*
			0x1A8, // reload_shader_transparent_chicago*
			0x1BA, // profile_unlock_solo_levels*
			0x20B, // track_remote_player_position_updates*
			0x20D, // sv_get_player_action_queue_length*
			0x21C, // oid_watch*
			0x21D, // oid_dump*
			0x21E, // oid_status*
			//////////////////////////////////////////////////////////////////////////

			0x5C, // render_effects
			0x9C, // cheats_load
			0xA8, // ai_deselect
			0xDC, // ai_reconnect
			0x11F, // ai_lines
			0x120, // ai_debug_sound_point_set
			0x121, // ai_debug_vocalize
			0x122, // ai_debug_teleport_to
			0x123, // ai_debug_speak
			0x124, // ai_debug_speak_list
			0x153, // debug_sounds_distances
			0x18F, // structure_lens_flares_place
			0x1AC, // rasterizer_decals_flush
			0x1AD, // rasterizer_fps_accumulate


			//////////////////////////////////////////////////////////////////////////
			// these aren't null functions, but highly doubt they are used in anyone's scripts, so we're fine
			0x152, // debug_sounds_enable*
			0x18E, // error_overflow_suppression*
			0x1A9, // rasterizer_reload_effects*
			//////////////////////////////////////////////////////////////////////////
		};
		static const int32 HS_NULL_FUNCTION_COUNT = NUMBEROF(hs_null_functions);
		#pragma endregion

		#pragma region script globals related
		static const uint16 hs_null_globals[] = {
			0x67, // debug_bink
			0x68, // screenshot_size
			0x69, // screenshot_count
			0x71, // temporary_hud
			0x95, // model_animation_data_compressed_size
			0x96, // model_animation_data_uncompressed_size
			0x97, // model_animation_data_compression_savings_in_bytes
			0x98, // model_animation_data_compression_savings_in_bytes_at_import
			0x99, // model_animation_data_compression_savings_in_percent
			0x9A, // model_animation_bullshit0
			0x9B, // model_animation_bullshit1
			0x9C, // model_animation_bullshit2
			0x9D, // model_animation_bullshit3
			0x10C, // profile_graph
			0x10D, // profile_display
			0x10E, // profile_timebase_ticks
			0x10F, // profile_dump_frames
			0x110, // profile_dump_lost_frames
			0x117, // debug_texture_cache
			0x1B2, // texture_cache_graph
			0x1B3, // texture_cache_list
			0x1B8, // debug_player_color
			0x1BE, // debug_game_save
			0x1BF, // allow_out_of_sync
			0x1C0, // global_connection_dont_timeout
			0x1C1, // slow_server_startup_safety_zone_in_seconds
			0x1C2, // find_all_fucked_up_shit

			0x6A, // terminal_render
			0x6B, // console_dump_to_file
			0x6D, // debug_object_garbage_collection
			0x6E, // debug_render_freeze
			0x6F, // debug_no_drawing
			0x70, // debug_input_target
			0x72, // debug_leaf_index
			0x73, // debug_leaf_portal_index
			0x74, // debug_leaf_portals
			0x75, // debug_unit_all_animations
			0x76, // debug_unit_animations
			0x77, // debug_unit_illumination
			0x78, // debug_damage_taken
			0x84, // debug_effects_nonviolent
			0x8F, // debug_sound_environment
			0xA0, // debug_sprites
			0xA1, // debug_portals
			0xA2, // debug_inactive_objects
			0xA3, // render_contrails
			0xA4, // render_particles
			0xA5, // render_psystems
			0xA6, // render_wsystems
			0xA7, // debug_objects
			0xA8, // debug_objects_position_velocity
			0xA9, // debug_objects_root_node
			0xAA, // debug_objects_bounding_spheres
			0xAB, // debug_objects_collision_models
			0xAC, // debug_objects_physics
			0xAD, // debug_objects_names
			0xAE, // debug_objects_pathfinding_spheres
			0xAF, // debug_objects_unit_vectors
			0xB0, // debug_objects_unit_seats
			0xB1, // debug_objects_unit_mouth_apeture
			0xB2, // debug_objects_biped_physics_pills
			0xB3, // debug_objects_biped_autoaim_pills
			0xB4, // debug_objects_vehicle_powered_mass_points
			0xB5, // debug_objects_devices
			0xB6, // debug_objects_biped_messages
			0xB7, // debug_objects_vehicle_messages
			0xB8, // debug_objects_weapon_messages
			0xB9, // debug_objects_projectile_messages
			0xBA, // debug_objects_equipment_messages
			0xBB, // render_model_nodes
			0xBC, // render_model_vertex_counts
			0xBD, // render_model_index_counts
			0xBE, // render_model_markers
			0xBF, // render_model_no_geometry
			0xC0, // render_shadows
			0xC1, // debug_damage
			0xC2, // debug_scripting
			0xC3, // debug_trigger_volumes
			0xC4, // debug_point_physics
			0xC5, // debug_physics_disable_penetration_freeze
			0xC7, // collision_debug
			0xC8, // collision_debug_spray
			0xC9, // collision_debug_features
			0xCA, // collision_debug_repeat
			0xCB, // collision_debug_flag_front_facing_surfaces
			0xCC, // collision_debug_flag_back_facing_surfaces
			0xCD, // collision_debug_flag_ignore_two_sided_surfaces
			0xCE, // collision_debug_flag_ignore_invisible_surfaces
			0xCF, // collision_debug_flag_ignore_breakable_surfaces
			0xD0, // collision_debug_flag_structure
			0xD1, // collision_debug_flag_media
			0xD2, // collision_debug_flag_objects
			0xD3, // collision_debug_flag_objects_bipeds
			0xD4, // collision_debug_flag_objects_vehicles
			0xD5, // collision_debug_flag_objects_weapons
			0xD6, // collision_debug_flag_objects_equipment
			0xD7, // collision_debug_flag_objects_projectiles
			0xD8, // collision_debug_flag_objects_scenery
			0xD9, // collision_debug_flag_objects_machines
			0xDA, // collision_debug_flag_objects_controls
			0xDB, // collision_debug_flag_objects_light_fixtures
			0xDC, // collision_debug_flag_objects_placeholders
			0xDD, // collision_debug_flag_try_to_keep_location_valid
			0xDE, // collision_debug_flag_skip_passthrough_bipeds
			0xDF, // collision_debug_flag_use_vehicle_physics
			0xE0, // collision_debug_point_x
			0xE1, // collision_debug_point_y
			0xE2, // collision_debug_point_z
			0xE3, // collision_debug_vector_i
			0xE4, // collision_debug_vector_j
			0xE5, // collision_debug_vector_k
			0xE6, // collision_debug_length
			0xE7, // collision_debug_width
			0xE8, // collision_debug_height
			0xE9, // collision_debug_phantom_bsp
			0xEA, // collision_log_render
			0xEB, // collision_log_detailed
			0xEC, // collision_log_extended
			0xED, // collision_log_totals_only
			0xEE, // collision_log_time
			0xEF, // debug_obstacle_path
			0xF0, // debug_obstacle_path_on_failure
			0xF1, // debug_obstacle_path_start_point_x
			0xF2, // debug_obstacle_path_start_point_y
			0xF3, // debug_obstacle_path_start_surface_index
			0xF4, // debug_obstacle_path_goal_point_x
			0xF5, // debug_obstacle_path_goal_point_y
			0xF6, // debug_obstacle_path_goal_surface_index
			0xF7, // debug_camera
			0xF8, // debug_player
			0xF9, // debug_structure
			0xFA, // debug_bsp
			0xFB, // debug_input
			0xFC, // debug_permanent_decals
			0xFD, // debug_fog_planes
			0xFF, // debug_decals
			0x100, // debug_object_lights
			0x101, // debug_lights
			0x102, // debug_biped_physics
			0x103, // debug_biped_skip_update
			0x104, // debug_biped_skip_collision
			0x105, // debug_biped_limp_body_disable
			0x106, // debug_collision_skip_objects
			0x107, // debug_collision_skip_vectors
			0x108, // debug_material_effects
			0x118, // debug_detail_objects
			0x119, // ai_render
			0x11A, // ai_render_all_actors
			0x11B, // ai_render_inactive_actors
			0x11C, // ai_render_lineoffire_crouching
			0x11D, // ai_render_lineoffire
			0x11E, // ai_render_lineofsight
			0x11F, // ai_render_ballistic_lineoffire
			0x120, // ai_render_encounter_activeregion
			0x121, // ai_render_vision_cones
			0x122, // ai_render_current_state
			0x123, // ai_render_detailed_state
			0x124, // ai_render_props
			0x125, // ai_render_props_web
			0x126, // ai_render_props_no_friends
			0x127, // ai_render_props_target_weight
			0x128, // ai_render_props_unreachable
			0x129, // ai_render_props_unopposable
			0x12A, // ai_render_idle_look
			0x12B, // ai_render_support_surfaces
			0x12C, // ai_render_recent_damage
			0x12D, // ai_render_threats
			0x12E, // ai_render_emotions
			0x12F, // ai_render_audibility
			0x130, // ai_render_aiming_vectors
			0x131, // ai_render_secondary_looking
			0x132, // ai_render_targets
			0x133, // ai_render_targets_last_visible
			0x134, // ai_render_states
			0x135, // ai_render_vitality
			0x136, // ai_render_active_cover_seeking
			0x137, // ai_render_evaluations
			0x138, // ai_render_pursuit
			0x139, // ai_render_shooting
			0x13A, // ai_render_trigger
			0x13B, // ai_render_projectile_aiming
			0x13C, // ai_render_aiming_validity
			0x13D, // ai_render_speech
			0x13E, // ai_render_teams
			0x13F, // ai_render_player_ratings
			0x140, // ai_render_spatial_effects
			0x141, // ai_render_firing_positions
			0x142, // ai_render_gun_positions
			0x143, // ai_render_burst_geometry
			0x144, // ai_render_vehicle_avoidance
			0x145, // ai_render_vehicles_enterable
			0x146, // ai_render_melee_check
			0x147, // ai_render_dialogue_variants
			0x148, // ai_render_grenade_decisions
			0x149, // ai_render_danger_zones
			0x14A, // ai_render_charge_decisions
			0x14B, // ai_render_control
			0x14C, // ai_render_activation
			0x14D, // ai_render_paths
			0x14E, // ai_render_paths_selected_only
			0x14F, // ai_render_paths_destination
			0x150, // ai_render_paths_current
			0x151, // ai_render_paths_failed
			0x152, // ai_render_paths_raw
			0x153, // ai_render_paths_smoothed
			0x154, // ai_render_paths_avoided
			0x155, // ai_render_paths_avoidance_segment
			0x156, // ai_render_paths_avoidance_obstacles
			0x157, // ai_render_paths_avoidance_search
			0x158, // ai_render_paths_nodes
			0x159, // ai_render_paths_nodes_all
			0x15A, // ai_render_paths_nodes_polygons
			0x15B, // ai_render_paths_nodes_costs
			0x15C, // ai_render_paths_nodes_closest
			0x15D, // ai_render_player_aiming_blocked
			0x15E, // ai_render_vector_avoidance
			0x15F, // ai_render_vector_avoidance_rays
			0x160, // ai_render_vector_avoidance_sense_t
			0x161, // ai_render_vector_avoidance_avoid_t
			0x162, // ai_render_vector_avoidance_clear_time
			0x163, // ai_render_vector_avoidance_weights
			0x164, // ai_render_vector_avoidance_objects
			0x165, // ai_render_vector_avoidance_intermediate
			0x166, // ai_render_postcombat
			0x167, // ai_print_pursuit_checks
			0x168, // ai_print_rules
			0x169, // ai_print_rule_values
			0x16A, // ai_print_major_upgrade
			0x16B, // ai_print_respawn
			0x16C, // ai_print_evaluation_statistics
			0x16D, // ai_print_communication
			0x16E, // ai_print_communication_player
			0x16F, // ai_print_vocalizations
			0x170, // ai_print_placement
			0x171, // ai_print_speech
			0x172, // ai_print_speech_timers
			0x173, // ai_print_allegiance
			0x174, // ai_print_lost_speech
			0x175, // ai_print_migration
			0x176, // ai_print_automatic_migration
			0x177, // ai_print_scripting
			0x178, // ai_print_surprise
			0x179, // ai_print_command_lists
			0x17A, // ai_print_damage_modifiers
			0x17B, // ai_print_secondary_looking
			0x17C, // ai_print_oversteer
			0x17D, // ai_print_conversations
			0x17E, // ai_print_killing_sprees
			0x17F, // ai_print_acknowledgement
			0x180, // ai_print_unfinished_paths
			0x181, // ai_print_bsp_transition
			0x182, // ai_print_uncovering
			0x183, // ai_profile_disable
			0x184, // ai_profile_random
			0x185, // ai_show
			0x186, // ai_show_stats
			0x187, // ai_show_actors
			0x188, // ai_show_swarms
			0x189, // ai_show_paths
			0x18A, // ai_show_line_of_sight
			0x18B, // ai_show_prop_types
			0x18C, // ai_show_sound_distance
			0x18D, // ai_debug_fast_los
			0x18E, // ai_debug_oversteer_disable
			0x18F, // ai_debug_evaluate_all_positions
			0x190, // ai_debug_path
			0x191, // ai_debug_path_start_freeze
			0x192, // ai_debug_path_end_freeze
			0x193, // ai_debug_path_flood
			0x194, // ai_debug_path_maximum_radius
			0x195, // ai_debug_path_attractor
			0x196, // ai_debug_path_attractor_radius
			0x197, // ai_debug_path_attractor_weight
			0x198, // ai_debug_path_accept_radius
			0x199, // ai_debug_ballistic_lineoffire_freeze
			0x19A, // ai_debug_communication_random_disabled
			0x19B, // ai_debug_communication_timeout_disabled
			0x19C, // ai_debug_communication_unit_repeat_disabled
			0x19D, // ai_debug_communication_focus_enable
			0x19E, // ai_debug_blind
			0x19F, // ai_debug_deaf
			0x1A0, // ai_debug_invisible_player
			0x1A1, // ai_debug_ignore_player
			0x1A2, // ai_debug_flee_always
			0x1A3, // ai_debug_force_all_active
			0x1A4, // ai_debug_disable_wounded_sounds
			0x1A5, // ai_debug_force_vocalizations
			0x1A6, // ai_debug_force_crouch
			0x1A7, // ai_debug_path_disable_smoothing
			0x1A8, // ai_debug_path_disable_obstacle_avoidance
			0x1A9, // ai_fix_defending_guard_firing_positions
			0x1AA, // ai_fix_actor_variants
			0x1B1, // debug_player_teleport
			0x1B6, // debug_recording
			0x1B7, // debug_recording_newlines
		};
		static const int32 HS_NULL_GLOBALS_COUNT = NUMBEROF(hs_null_globals);
		#pragma endregion
	};
};