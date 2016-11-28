/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

#ifndef __SCRIPTLIBRARY_INCLUDE_INL__
#define __SCRIPTLIBRARY_INCLUDE_INL__
	#define __SCRIPTLIBRARY_ENUMERATIONS \
		(0x00 + 0)
	#define __SCRIPTLIBRARY_DEFINITIONS \
		(0x00 + 1)
	#define __SCRIPTLIBRARY_DEFINITION_LISTING \
		(0x00 + 2)
#endif

#include <boost/preprocessor/iteration/iterate.hpp>

#if !BOOST_PP_IS_ITERATING

	#if defined(__SCRIPTLIBRARY_INCLUDE_ID)
		#define BOOST_PP_ITERATION_PARAMS_1 (3, (__SCRIPTLIBRARY_INCLUDE_ID, __SCRIPTLIBRARY_INCLUDE_ID, "Game/ScriptLibrary.inl"))
		??=include BOOST_PP_ITERATE()
		#undef __SCRIPTLIBRARY_INCLUDE_ID
	#endif

#elif BOOST_PP_ITERATION() == __SCRIPTLIBRARY_ENUMERATIONS

	//////////////////////////////////////////////////////////////////////////
	// Functions
	enum hs_function_enumeration : int16 {
		//_hs_function_null,

		_hs_function_volume_test_player_team,
		_hs_function_volume_test_player_team_all,
		_hs_function_player_team_teleport,
		_hs_function_player_team_players,

		_hs_function_player_data_get_integer,
		_hs_function_player_team_data_get_integer,
		_hs_function_player_data_get_object,
		_hs_function_player_data_get_real,
		_hs_function_player_data_set_real,
		_hs_function_player_local_get,

		_hs_function_objects_distance_to_object,
		_hs_function_object_data_get_real,
		_hs_function_object_data_set_real,
		_hs_function_weapon_data_get_real,
		_hs_function_weapon_data_set_real,
		_hs_function_weapon_data_magazine_get_integer,
		_hs_function_weapon_data_magazine_set_integer,
		_hs_function_weapon_data_trigger_set_real,
		_hs_function_unit_data_get_object,
		_hs_function_unit_data_get_integer,
		_hs_function_unit_data_set_integer,
		_hs_function_unit_data_get_real,
		_hs_function_unit_data_set_real,

		_hs_function_physics_get_gravity,
		_hs_function_physics_set_gravity,
		_hs_function_physics_constants_reset,

		_hs_function_runtime_integers_reset,
		_hs_function_runtime_integer_get,
		_hs_function_runtime_integer_set,
		_hs_function_runtime_integer_inc,
		_hs_function_runtime_integer_dec,

		_hs_function_game_change_version_id,
		_hs_function_game_engine_data_get_integer,

		_hs_function_machine_is_host,
		_hs_function_machine_is_dedi,

		_hs_function_abs_integer,
		_hs_function_abs_real,

		_hs_function_pp_load,
		_hs_function_pp_unload,

		_hs_function_pp_get_effect_instance_index_by_name,
		_hs_function_pp_set_effect_instance_active,
		_hs_function_pp_set_effect_instance_fade,
		_hs_function_pp_get_effect_instance_current_fade,
		_hs_function_pp_get_effect_instance_fade_direction,
		_hs_function_pp_get_effect_index_by_name,
		_hs_function_pp_get_effect_is_valid,
		_hs_function_pp_get_effect_shader_variable_index_by_name,
		_hs_function_pp_set_effect_shader_variable_boolean,
		_hs_function_pp_set_effect_shader_variable_integer,
		_hs_function_pp_set_effect_shader_variable_real,
		_hs_function_pp_set_effect_shader_instance_active,
		_hs_function_pp_bloom_set_size,
		_hs_function_pp_bloom_set_exposure,
		_hs_function_pp_bloom_set_mix_amount,
		_hs_function_pp_bloom_set_minimum_color,
		_hs_function_pp_bloom_set_maximum_color,

		_hs_function_bitwise_and,
		_hs_function_bitwise_or,
		_hs_function_bitwise_xor,
		_hs_function_bitwise_lhs,
		_hs_function_bitwise_rhs,
		_hs_function_bit_test,
		_hs_function_bit_toggle,
		_hs_function_bit_flags_test,
		_hs_function_bit_flags_toggle,
		_hs_function_hex_string_to_long,

		_hs_function_runtime_integer_operation,
		_hs_function_runtime_vectors_reset,
		_hs_function_runtime_vector_get_element,
		_hs_function_runtime_vector_set_element,
		_hs_function_runtime_vector_set,
		_hs_function_runtime_vector_operation,
		_hs_function_runtime_vector_to_string,
		_hs_function_object_data_set_vector,
		_hs_function_object_data_save_vector,

		_hs_function_display_scripted_ui_widget,
		_hs_function_play_bink_movie,

		_hs_function_scenario_faux_zones_reset,					// depreceated
		_hs_function_scenario_faux_zone_current_switch_variant,	// depreceated
		_hs_function_scenario_faux_zone_switch_variant,			// depreceated
		_hs_function_scenario_faux_zone_switch_sky,				// depreceated
		
		_hs_function_structure_bsp_set_lightmap_set,
		_hs_function_structure_bsp_set_sky_set,
		
		_hs_function_ai_transform_actor,
		_hs_function_ai_transform_actors,
		_hs_function_ai_transform_actors_by_type,
		_hs_function_ai_actor_is_transforming,

		//////////////////////////////////////////////////////////////////////////
		// everything after is runtime-only, ie not defined in the CheApe scripting definitions

		_hs_function_vehicle_remapper_enabled,

		_hs_function_sv_httpserver_set_thread_count,
		_hs_function_sv_httpserver_set_root,
		_hs_function_sv_httpserver_set_throttle,
		_hs_function_sv_httpserver_set_ports,
		_hs_function_sv_httpserver_show_config,
		_hs_function_sv_httpserver_start,
		_hs_function_sv_httpserver_stop,
		_hs_function_sv_httpserver_log_enable,
		_hs_function_sv_httpserver_set_connection_ban,
		_hs_function_sv_httpserver_banlist,
		_hs_function_sv_httpserver_banlist_file,
		_hs_function_sv_httpserver_ban_ip,
		_hs_function_sv_httpserver_unban_ip,

		_hs_function_sv_mapdownload_start_server,
		_hs_function_sv_mapdownload_stop_server,
		_hs_function_sv_mapdownload_set_part_definitions_path,
		_hs_function_sv_mapdownload_set_host,
		_hs_function_sv_mapdownload_reload_map_part_definitions,

		_hs_function_data_array_info,

	// debug functions
	#ifdef API_DEBUG
		_hs_function_dump_view_state,
		_hs_function_test_networking,
	#endif

		k_hs_function_enumeration_count,
	};


	//////////////////////////////////////////////////////////////////////////
	// Globals
	enum hs_global_enumeration : int16 {

		_hs_global_rasterizer_rt_display,
		_hs_global_rasterizer_gbuffer_enabled,

		_hs_global_pp_external_post_processes_enabled,
		_hs_global_pp_internal_post_processes_enabled,
		_hs_global_pp_fxaa_enabled,
		_hs_global_pp_motion_blur_enabled,
		_hs_global_pp_motion_blur_amount,

		_hs_global_ai_transforms_enabled,
		
		_hs_global_rasterizer_model_normal_mapping,
		_hs_global_rasterizer_model_detail_normal_mapping,
		_hs_global_rasterizer_model_specular_lights,
		_hs_global_rasterizer_model_specular_map,
		_hs_global_rasterizer_environment_dlm_diffuse,
		_hs_global_rasterizer_environment_dlm_specular,
		_hs_global_rasterizer_effect_depth_fade,
		
	// debug globals
	#ifdef API_DEBUG
	#endif

		k_hs_global_enumeration_count,
	};

#elif BOOST_PP_ITERATION() == __SCRIPTLIBRARY_DEFINITIONS

	//////////////////////////////////////////////////////////////////////////
	// Functions
	//HS_FUNCTION(null, void, "", "");

	#include "Game/ScriptLibrary.Definitions.Players.inl"
	#include "Game/ScriptLibrary.Definitions.Objects.inl"


	HS_FUNCTION(physics_get_gravity, real, "");
	HS_FUNCTION_WITH_PARAMS(physics_set_gravity, void, "", 
			"gravity fraction", 1,
		HS_TYPE(real)
	);
	HS_FUNCTION(physics_constants_reset, void, "");


	#include "Game/ScriptLibrary.Definitions.RuntimeData.inl"
	

	HS_FUNCTION_WITH_PARAMS(game_change_version_id, bool, "returns whether the change was successful or not", 
			"<also-change-game-build-string> <version-string> ", 2,
		HS_TYPE(bool),
		HS_TYPE(string)
	);
	HS_FUNCTION_WITH_PARAMS(game_engine_data_get_integer, long, "", 
			"<data-name>", 1,
		HS_TYPE(string)
	);


	HS_FUNCTION(machine_is_host, bool, "returns whether or not the local machine is the host");
	HS_FUNCTION(machine_is_dedi, bool, "returns whether or not the local machine is a dedicated server");


	#include "Game/ScriptLibrary.Definitions.Numbers.inl"
	#include "Game/ScriptLibrary.Definitions.Postprocessing.inl"
	#include "Game/ScriptLibrary.Definitions.HTTPServer.inl"
	#include "Game/ScriptLibrary.Definitions.HTTPServer.MapDownload.inl"

	HS_FUNCTION_WITH_PARAMS(display_scripted_ui_widget, bool, "", 
			"<name>", 1,
		HS_TYPE(string)
	);
	HS_FUNCTION_WITH_PARAMS(play_bink_movie, void, "", 
			"<filename>", 1,
		HS_TYPE(string)
	);

	HS_FUNCTION(scenario_faux_zones_reset, void, "depreceated, do not use");
	HS_FUNCTION_WITH_PARAMS(scenario_faux_zone_current_switch_variant, bool, "depreceated, do not use", 
			"<variant-name>", 1,
		HS_TYPE(string)
	);
	HS_FUNCTION_WITH_PARAMS(scenario_faux_zone_switch_variant, bool, "depreceated, do not use", 
			"<zone-name> <variant-name>", 2,
		HS_TYPE(string),
		HS_TYPE(string)
	);
	HS_FUNCTION_WITH_PARAMS(scenario_faux_zone_switch_sky, bool, "depreceated, do not use", 
			"<zone-sky-name>", 1,
		HS_TYPE(string)
	);
	
	HS_FUNCTION_WITH_PARAMS(structure_bsp_set_lightmap_set, bool, "Sets a bsp's lightmap set", 
			"<bsp-index> <set-name>", 2,
		HS_TYPE(short),
		HS_TYPE(string)
	);

	HS_FUNCTION_WITH_PARAMS(structure_bsp_set_sky_set, bool, "Sets a bsp's sky set", 
			"<bsp-index> <set-name>", 2,
		HS_TYPE(short),
		HS_TYPE(string)
	);

	HS_FUNCTION_WITH_PARAMS(vehicle_remapper_enabled, bool, "returns the remapper state before the function call", 
			"state_name", 1,
		HS_TYPE(string)
	);

	HS_FUNCTION_WITH_PARAMS(data_array_info, void, "", 
			"<data-array-name>", 1,
		HS_TYPE(string)
	);

	HS_FUNCTION_WITH_PARAMS(ai_transform_actor, bool, "Transforms an actor into the specified target. Returns false if it fails. Empty names causes random selection.", 
			"<object> <transform_name> <target_name>", 3,
		HS_TYPE(object),
		HS_TYPE(string),
		HS_TYPE(string)
	);

	HS_FUNCTION_WITH_PARAMS(ai_transform_actors, bool, "Transforms a list of actors into the specified target. Returns false if it fails. Empty names causes random selection.", 
			"<objects> <transform_name> <target_name>", 3,
		HS_TYPE(object_list),
		HS_TYPE(string),
		HS_TYPE(string)
	);

	HS_FUNCTION_WITH_PARAMS(ai_transform_actors_by_type, bool, "Transforms actors in a list of a specific type into the specified target. Returns false if it fails. Empty names causes random selection.", 
			"<objects> <actor_variant> <transform_name> <target_name>", 4,
		HS_TYPE(object_list),
		HS_TYPE(actor_variant),
		HS_TYPE(string),
		HS_TYPE(string)
	);

	HS_FUNCTION_WITH_PARAMS(ai_actor_is_transforming, bool, "Returns true if the specified actor is transforming.", 
			"<object>", 1,
		HS_TYPE(object)
	);

	// debug functions
	#ifdef API_DEBUG
		HS_FUNCTION_WITH_PARAMS(dump_view_state, void, "dump current render view state", "<name>", 1,
			HS_TYPE(string)
		);
		HS_FUNCTION(test_networking, void, "");
	#endif


	//////////////////////////////////////////////////////////////////////////
	// Globals
	HS_GLOBAL2(rasterizer_rt_display, short, &DX9::c_gbuffer_system::g_debug_index, nullptr);
	HS_GLOBAL2(rasterizer_gbuffer_enabled, bool, &DX9::c_gbuffer_system::g_system_enabled, nullptr);

	HS_GLOBAL2(pp_external_post_processes_enabled, bool, 
		&Rasterizer::PostProcessing::Scripting::Globals::Enabled_External(), nullptr);
	HS_GLOBAL2(pp_internal_post_processes_enabled, bool, 
		&Rasterizer::PostProcessing::Scripting::Globals::Enabled_Internal(), nullptr);
	HS_GLOBAL2(pp_fxaa_enabled, bool, 
		&Rasterizer::PostProcessing::Scripting::Globals::Enabled_FXAA(), nullptr);
	HS_GLOBAL2(pp_motion_blur_enabled, bool, 
		&Rasterizer::PostProcessing::Scripting::Globals::Enabled_MotionBlur(), nullptr);
	HS_GLOBAL2(pp_motion_blur_amount, real, 
		&Rasterizer::PostProcessing::Scripting::Globals::MotionBlur_Amount(), nullptr);

	HS_GLOBAL2(ai_transforms_enabled, bool, &AI::Transform::TransformsEnabled(), nullptr);
	
	HS_GLOBAL2(rasterizer_model_normal_mapping, bool, &Rasterizer::ShaderExtension::Model::g_rasterizer_model_normal_mapping, nullptr);
	HS_GLOBAL2(rasterizer_model_detail_normal_mapping, bool, &Rasterizer::ShaderExtension::Model::g_rasterizer_model_detail_normal_mapping, nullptr);
	HS_GLOBAL2(rasterizer_model_specular_lights, bool, &Rasterizer::ShaderExtension::Model::g_rasterizer_model_specular_lights, nullptr);
	HS_GLOBAL2(rasterizer_model_specular_map, bool, &Rasterizer::ShaderExtension::Model::g_rasterizer_model_specular_map, nullptr);
	HS_GLOBAL2(rasterizer_environment_dlm_diffuse, bool, &Rasterizer::ShaderExtension::Environment::g_rasterizer_environment_dlm_diffuse, nullptr);
	HS_GLOBAL2(rasterizer_environment_dlm_specular, bool, &Rasterizer::ShaderExtension::Environment::g_rasterizer_environment_dlm_specular, nullptr);
	HS_GLOBAL2(rasterizer_effect_depth_fade, bool, &Rasterizer::ShaderExtension::Effect::g_rasterizer_effect_depth_fade, nullptr);

	// debug globals
	#ifdef API_DEBUG
	#endif

#elif BOOST_PP_ITERATION() == __SCRIPTLIBRARY_DEFINITION_LISTING

	//////////////////////////////////////////////////////////////////////////
	// Functions
	static hs_function_definition* hs_yelo_functions[] = {
		//&GET_HS_FUNCTION(null),

		&GET_HS_FUNCTION(volume_test_player_team),
		&GET_HS_FUNCTION(volume_test_player_team_all),
		&GET_HS_FUNCTION(player_team_teleport),
		&GET_HS_FUNCTION(player_team_players),

		&GET_HS_FUNCTION(player_data_get_integer),
		&GET_HS_FUNCTION(player_team_data_get_integer),
		&GET_HS_FUNCTION(player_data_get_object),
		&GET_HS_FUNCTION(player_data_get_real),
		&GET_HS_FUNCTION(player_data_set_real),
		&GET_HS_FUNCTION(player_local_get),

		&GET_HS_FUNCTION(objects_distance_to_object),
		&GET_HS_FUNCTION(object_data_get_real),
		&GET_HS_FUNCTION(object_data_set_real),
		&GET_HS_FUNCTION(weapon_data_get_real),
		&GET_HS_FUNCTION(weapon_data_set_real),
		&GET_HS_FUNCTION(weapon_data_magazine_get_integer),
		&GET_HS_FUNCTION(weapon_data_magazine_set_integer),
		&GET_HS_FUNCTION(weapon_data_trigger_set_real),
		&GET_HS_FUNCTION(unit_data_get_object),
		&GET_HS_FUNCTION(unit_data_get_integer),
		&GET_HS_FUNCTION(unit_data_set_integer),
		&GET_HS_FUNCTION(unit_data_get_real),
		&GET_HS_FUNCTION(unit_data_set_real),

		&GET_HS_FUNCTION(physics_get_gravity),
		&GET_HS_FUNCTION(physics_set_gravity),
		&GET_HS_FUNCTION(physics_constants_reset),

		&GET_HS_FUNCTION(runtime_integers_reset),
		&GET_HS_FUNCTION(runtime_integer_get),
		&GET_HS_FUNCTION(runtime_integer_set),
		&GET_HS_FUNCTION(runtime_integer_inc),
		&GET_HS_FUNCTION(runtime_integer_dec),

		&GET_HS_FUNCTION(game_change_version_id),
		&GET_HS_FUNCTION(game_engine_data_get_integer),

		&GET_HS_FUNCTION(machine_is_host),
		&GET_HS_FUNCTION(machine_is_dedi),

		&GET_HS_FUNCTION(abs_integer),
		&GET_HS_FUNCTION(abs_real),

		&GET_HS_FUNCTION(pp_load),
		&GET_HS_FUNCTION(pp_unload),

		&GET_HS_FUNCTION(pp_get_effect_instance_index_by_name),
		&GET_HS_FUNCTION(pp_set_effect_instance_active),
		&GET_HS_FUNCTION(pp_set_effect_instance_fade),
		&GET_HS_FUNCTION(pp_get_effect_instance_current_fade),
		&GET_HS_FUNCTION(pp_get_effect_instance_fade_direction),
		&GET_HS_FUNCTION(pp_get_effect_index_by_name),
		&GET_HS_FUNCTION(pp_get_effect_is_valid),
		&GET_HS_FUNCTION(pp_get_effect_shader_variable_index_by_name),
		&GET_HS_FUNCTION(pp_set_effect_shader_variable_boolean),
		&GET_HS_FUNCTION(pp_set_effect_shader_variable_integer),
		&GET_HS_FUNCTION(pp_set_effect_shader_variable_real),
		&GET_HS_FUNCTION(pp_set_effect_shader_instance_active),
		&GET_HS_FUNCTION(pp_bloom_set_size),
		&GET_HS_FUNCTION(pp_bloom_set_exposure),
		&GET_HS_FUNCTION(pp_bloom_set_mix_amount),
		&GET_HS_FUNCTION(pp_bloom_set_minimum_color),
		&GET_HS_FUNCTION(pp_bloom_set_maximum_color),

		&GET_HS_FUNCTION(bitwise_and),
		&GET_HS_FUNCTION(bitwise_or),
		&GET_HS_FUNCTION(bitwise_xor),
		&GET_HS_FUNCTION(bitwise_lhs),
		&GET_HS_FUNCTION(bitwise_rhs),
		&GET_HS_FUNCTION(bit_test),
		&GET_HS_FUNCTION(bit_toggle),
		&GET_HS_FUNCTION(bit_flags_test),
		&GET_HS_FUNCTION(bit_flags_toggle),
		&GET_HS_FUNCTION(hex_string_to_long),

		&GET_HS_FUNCTION(runtime_integer_operation),
		&GET_HS_FUNCTION(runtime_vectors_reset),
		&GET_HS_FUNCTION(runtime_vector_get_element),
		&GET_HS_FUNCTION(runtime_vector_set_element),
		&GET_HS_FUNCTION(runtime_vector_set),
		&GET_HS_FUNCTION(runtime_vector_operation),
		&GET_HS_FUNCTION(runtime_vector_to_string),
		&GET_HS_FUNCTION(object_data_set_vector),
		&GET_HS_FUNCTION(object_data_save_vector),

		&GET_HS_FUNCTION(display_scripted_ui_widget),
		&GET_HS_FUNCTION(play_bink_movie),

		&GET_HS_FUNCTION(scenario_faux_zones_reset),
		&GET_HS_FUNCTION(scenario_faux_zone_current_switch_variant),
		&GET_HS_FUNCTION(scenario_faux_zone_switch_variant),
		&GET_HS_FUNCTION(scenario_faux_zone_switch_sky),

		&GET_HS_FUNCTION(structure_bsp_set_lightmap_set),
		&GET_HS_FUNCTION(structure_bsp_set_sky_set),
		
		&GET_HS_FUNCTION(ai_transform_actor),
		&GET_HS_FUNCTION(ai_transform_actors),
		&GET_HS_FUNCTION(ai_transform_actors_by_type),
		&GET_HS_FUNCTION(ai_actor_is_transforming),

		&GET_HS_FUNCTION(vehicle_remapper_enabled),

		&GET_HS_FUNCTION(sv_httpserver_set_thread_count),
		&GET_HS_FUNCTION(sv_httpserver_set_root),
		&GET_HS_FUNCTION(sv_httpserver_set_throttle),
		&GET_HS_FUNCTION(sv_httpserver_set_ports),
		&GET_HS_FUNCTION(sv_httpserver_show_config),
		&GET_HS_FUNCTION(sv_httpserver_start),
		&GET_HS_FUNCTION(sv_httpserver_stop),
		&GET_HS_FUNCTION(sv_httpserver_log_enable),
		&GET_HS_FUNCTION(sv_httpserver_set_connection_ban),
		&GET_HS_FUNCTION(sv_httpserver_banlist),
		&GET_HS_FUNCTION(sv_httpserver_banlist_file),
		&GET_HS_FUNCTION(sv_httpserver_ban_ip),
		&GET_HS_FUNCTION(sv_httpserver_unban_ip),

		&GET_HS_FUNCTION(sv_mapdownload_start_server),
		&GET_HS_FUNCTION(sv_mapdownload_stop_server),
		&GET_HS_FUNCTION(sv_mapdownload_set_part_definitions_path),
		&GET_HS_FUNCTION(sv_mapdownload_set_host),
		&GET_HS_FUNCTION(sv_mapdownload_reload_map_part_definitions),

		&GET_HS_FUNCTION(data_array_info),

	// debug functions
	#ifdef API_DEBUG
		&GET_HS_FUNCTION(dump_view_state),
		&GET_HS_FUNCTION(test_networking),
	#endif
	};
	// Validate our definition list has the same amount as our exposed enumeration count
	BOOST_STATIC_ASSERT( NUMBEROF(hs_yelo_functions) == Enums::k_hs_function_enumeration_count );
	// Validate we haven't gone over our limit of allowed new script functions
	BOOST_STATIC_ASSERT( 
		Enums::k_hs_function_enumeration_count <= (Enums::k_hs_script_functions_count_upgrade - Enums::k_hs_functions_count) );


	//////////////////////////////////////////////////////////////////////////
	// Globals
	static hs_global_definition* hs_yelo_globals[] = {

		&GET_HS_GLOBAL(rasterizer_rt_display),
		&GET_HS_GLOBAL(rasterizer_gbuffer_enabled),

		&GET_HS_GLOBAL(pp_external_post_processes_enabled),
		&GET_HS_GLOBAL(pp_internal_post_processes_enabled),
		&GET_HS_GLOBAL(pp_fxaa_enabled),
		&GET_HS_GLOBAL(pp_motion_blur_enabled),
		&GET_HS_GLOBAL(pp_motion_blur_amount),

		&GET_HS_GLOBAL(ai_transforms_enabled),
		
		&GET_HS_GLOBAL(rasterizer_model_normal_mapping),
		&GET_HS_GLOBAL(rasterizer_model_detail_normal_mapping),
		&GET_HS_GLOBAL(rasterizer_model_specular_lights),
		&GET_HS_GLOBAL(rasterizer_model_specular_map),
		&GET_HS_GLOBAL(rasterizer_environment_dlm_diffuse),
		&GET_HS_GLOBAL(rasterizer_environment_dlm_specular),
		&GET_HS_GLOBAL(rasterizer_effect_depth_fade),

	// debug globals
	#ifdef API_DEBUG
	#endif
	};
	// Validate our definition list has the same amount as our exposed enumeration count
	BOOST_STATIC_ASSERT( NUMBEROF(hs_yelo_globals) == Enums::k_hs_global_enumeration_count );

#endif