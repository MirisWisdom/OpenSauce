/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿using System;
using BlamLib.TagInterface;

namespace BlamLib.Blam.Halo2
{
	partial class StructGroups
	{
		/// <summary>
		/// instantaneous_response_damage_effect_struct
		/// </summary>
		static TagGroup ires = new TagGroup("ires", "instantaneous_response_damage_effect_struct");

		/// <summary>
		/// instantaneous_response_damage_effect_marker_struct
		/// </summary>
		static TagGroup irem = new TagGroup("irem", "instantaneous_response_damage_effect_marker_struct");

		/// <summary>
		/// model_target_lock_on_data_struct
		/// </summary>
		static TagGroup MTLO = new TagGroup("MTLO", "model_target_lock_on_data_struct");

		/// <summary>
		/// global_geometry_section_info_struct
		/// </summary>
		static TagGroup SINF = new TagGroup("SINF", "global_geometry_section_info_struct");

		/// <summary>
		/// global_geometry_section_struct
		/// </summary>
		static TagGroup SECT = new TagGroup("SECT", "global_geometry_section_struct");

		/// <summary>
		/// global_geometry_point_data_struct
		/// </summary>
		static TagGroup PDAT = new TagGroup("PDAT", "global_geometry_point_data_struct");

		/// <summary>
		/// global_geometry_isq_info_struct
		/// </summary>
		/// <remarks>old shit</remarks>
		static TagGroup ISQI = new TagGroup("ISQI", "global_geometry_isq_info_struct");

		/// <summary>
		/// global_geometry_block_info_struct
		/// </summary>
		static TagGroup BLOK = new TagGroup("BLOK", "global_geometry_block_info_struct");

		/// <summary>
		/// global_collision_bsp_struct
		/// </summary>
		static TagGroup cbsp = new TagGroup("cbsp", "global_collision_bsp_struct");

		/// <summary>
		/// constraint_bodies_struct
		/// </summary>
		static TagGroup csbs = new TagGroup("csbs", "constraint_bodies_struct");

		/// <summary>
		/// unit_camera_struct
		/// </summary>
		static TagGroup uncs = new TagGroup("uncs", "unit_camera_struct");

		/// <summary>
		/// unit_seat_acceleration_struct
		/// </summary>
		static TagGroup usas = new TagGroup("usas", "unit_seat_acceleration_struct");

		/// <summary>
		/// unit_additional_node_names_struct
		/// </summary>
		static TagGroup uHnd = new TagGroup("uHnd", "unit_additional_node_names_struct");

		/// <summary>
		/// unit_boarding_melee_struct
		/// </summary>
		static TagGroup ubms = new TagGroup("ubms", "unit_boarding_melee_struct");

		/// <summary>
		/// unit_boost_struct
		/// </summary>
		static TagGroup _1234 = new TagGroup("!@#$", "unit_boost_struct");

		/// <summary>
		/// unit_lipsync_scales_struct
		/// </summary>
		static TagGroup ulYc = new TagGroup("ulYc", "unit_lipsync_scales_struct");

		/// <summary>
		/// biped_lock_on_data_struct
		/// </summary>
		static TagGroup blod = new TagGroup("blod", "biped_lock_on_data_struct");

		/// <summary>
		/// character_physics_ground_struct
		/// </summary>
		static TagGroup chgr = new TagGroup("chgr", "character_physics_ground_struct");

		/// <summary>
		/// character_physics_flying_struct
		/// </summary>
		static TagGroup chfl = new TagGroup("chfl", "character_physics_flying_struct");

		/// <summary>
		/// character_physics_dead_struct
		/// </summary>
		static TagGroup chdd = new TagGroup("chdd", "character_physics_dead_struct");

		/// <summary>
		/// character_physics_sentinel_struct
		/// </summary>
		static TagGroup chsn = new TagGroup("chsn", "character_physics_sentinel_struct");

		/// <summary>
		/// character_physics_struct
		/// </summary>
		static TagGroup chpy = new TagGroup("chpy", "character_physics_struct");

		/// <summary>
		/// torque_curve_struct
		/// </summary>
		static TagGroup trcv = new TagGroup("trcv", "torque_curve_struct");

		/// <summary>
		/// havok_vehicle_physics_struct
		/// </summary>
		static TagGroup HVPH = new TagGroup("HVPH", "havok_vehicle_physics_struct");

		/// <summary>
		/// mapping_function
		/// </summary>
		static TagGroup MAPP = new TagGroup("MAPP", "mapping_function");

		/// <summary>
		/// melee_aim_assist_struct
		/// </summary>
		static TagGroup masd_melee = new TagGroup("masd", "melee_aim_assist_struct");

		/// <summary>
		/// melee_damage_parameters_struct
		/// </summary>
		static TagGroup mdps = new TagGroup("mdps", "melee_damage_parameters_struct");

		/// <summary>
		/// aim_assist_struct
		/// </summary>
		static TagGroup easd = new TagGroup("easd", "aim_assist_struct");

		/// <summary>
		/// weapon_tracking_struct
		/// </summary>
		static TagGroup wtsf = new TagGroup("wtsf", "weapon_tracking_struct");

		/// <summary>
		/// weapon_shared_interface_struct
		/// </summary>
		static TagGroup wSiS = new TagGroup("wSiS", "weapon_shared_interface_struct");

		/// <summary>
		/// weapon_interface_struct
		/// </summary>
		static TagGroup wItS = new TagGroup("wItS", "weapon_interface_struct");

		/// <summary>
		/// weapon_trigger_autofire_struct
		/// </summary>
		static TagGroup wtas = new TagGroup("wtas", "weapon_trigger_autofire_struct");

		/// <summary>
		/// weapon_trigger_charging_struct
		/// </summary>
		static TagGroup wtcs = new TagGroup("wtcs", "weapon_trigger_charging_struct");

		/// <summary>
		/// weapon_barrel_damage_effect_struct
		/// </summary>
		static TagGroup wbde = new TagGroup("wbde", "weapon_barrel_damage_effect_struct");

		/// <summary>
		/// color_function_struct
		/// </summary>
		static TagGroup CLFN = new TagGroup("CLFN", "color_function_struct");

		/// <summary>
		/// scalar_function_struct
		/// </summary>
		static TagGroup SCFN = new TagGroup("SCFN", "scalar_function_struct");

		/// <summary>
		/// particle_property_scalar_struct_new
		/// </summary>
		static TagGroup PRPS = new TagGroup("PRPS", "particle_property_scalar_struct_new");

		/// <summary>
		/// particle_property_color_struct_new
		/// </summary>
		static TagGroup PRPC = new TagGroup("PRPC", "particle_property_color_struct_new");

		/// <summary>
		/// tag_block_index_struct
		/// </summary>
		static TagGroup shtb = new TagGroup("shtb", "tag_block_index_struct");

		/// <summary>
		/// shader_gpu_state_reference_struct
		/// </summary>
		static TagGroup GPUR = new TagGroup("GPUR", "shader_gpu_state_reference_struct");

		/// <summary>
		/// shader_gpu_state_struct
		/// </summary>
		static TagGroup GPUS = new TagGroup("GPUS", "shader_gpu_state_struct");

		/// <summary>
		/// sound_response_extra_sounds_struct
		/// </summary>
		static TagGroup masd_sound = new TagGroup("masd", "sound_response_extra_sounds_struct");

		/// <summary>
		/// material_physics_properties_struct
		/// </summary>
		static TagGroup mphp = new TagGroup("mphp", "material_physics_properties_struct");

		/// <summary>
		/// materials_sweeteners_struct
		/// </summary>
		static TagGroup msst = new TagGroup("msst", "materials_sweeteners_struct");

		/// <summary>
		/// super_detonation_damage_struct
		/// </summary>
		static TagGroup sd2s = new TagGroup("sd2s", "super_detonation_damage_struct");

		/// <summary>
		/// angular_velocity_lower_bound_struct
		/// </summary>
		static TagGroup avlb = new TagGroup("avlb", "angular_velocity_lower_bound_struct");

		/// <summary>
		/// visibility_struct
		/// </summary>
		static TagGroup svis = new TagGroup("svis", "visibility_struct");

		/// <summary>
		/// structure_instanced_geometry_render_info_struct
		/// </summary>
		static TagGroup igri = new TagGroup("igri", "structure_instanced_geometry_render_info_struct");

		/// <summary>
		/// global_structure_physics_struct
		/// </summary>
		static TagGroup spdf = new TagGroup("spdf", "global_structure_physics_struct");

		/// <summary>
		/// scenario_object_id_struct
		/// </summary>
		static TagGroup obj_ = new TagGroup("obj#", "scenario_object_id_struct");

		/// <summary>
		/// render_lighting_struct
		/// </summary>
		static TagGroup rnli = new TagGroup("rnli", "render_lighting_struct");

		/// <summary>
		/// scenario_object_datum_struct
		/// </summary>
		static TagGroup sobj = new TagGroup("sobj", "scenario_object_datum_struct");

		/// <summary>
		/// scenario_object_permutation_struct
		/// </summary>
		static TagGroup sper = new TagGroup("sper", "scenario_object_permutation_struct");

		/// <summary>
		/// scenario_scenery_datum_struct_v4
		/// </summary>
		static TagGroup sct3 = new TagGroup("sct3", "scenario_scenery_datum_struct_v4");

		/// <summary>
		/// scenario_unit_struct
		/// </summary>
		static TagGroup sunt = new TagGroup("sunt", "scenario_unit_struct");

		/// <summary>
		/// scenario_equipment_datum_struct
		/// </summary>
		static TagGroup seqt = new TagGroup("seqt", "scenario_equipment_datum_struct");

		/// <summary>
		/// scenario_weapon_datum_struct
		/// </summary>
		static TagGroup swpt = new TagGroup("swpt", "scenario_weapon_datum_struct");

		/// <summary>
		/// scenario_device_struct
		/// </summary>
		static TagGroup sdvt = new TagGroup("sdvt", "scenario_device_struct");

		/// <summary>
		/// scenario_machine_struct_v3
		/// </summary>
		static TagGroup smht = new TagGroup("smht", "scenario_machine_struct_v3");

		/// <summary>
		/// scenario_control_struct
		/// </summary>
		static TagGroup sctt = new TagGroup("sctt", "scenario_control_struct");

		/// <summary>
		/// scenario_light_fixture_struct
		/// </summary>
		static TagGroup slft = new TagGroup("slft", "scenario_light_fixture_struct");

		/// <summary>
		/// sound_scenery_datum_struct
		/// </summary>
		static TagGroup _sc_ = new TagGroup("#sc#", "sound_scenery_datum_struct");

		/// <summary>
		/// scenario_light_struct
		/// </summary>
		static TagGroup slit = new TagGroup("slit", "scenario_light_struct");

		/// <summary>
		/// scenario_netgame_equipment_orientation_struct
		/// </summary>
		static TagGroup ntor = new TagGroup("ntor", "scenario_netgame_equipment_orientation_struct");

		/// <summary>
		/// static_spawn_zone_data_struct
		/// </summary>
		static TagGroup sszd = new TagGroup("sszd", "static_spawn_zone_data_struct");

		/// <summary>
		/// damage_outer_cone_angle_struct
		/// </summary>
		static TagGroup masd_damage = new TagGroup("masd", "damage_outer_cone_angle_struct");

		/// <summary>
		/// screen_flash_definition_struct
		/// </summary>
		static TagGroup SFDS = new TagGroup("SFDS", "screen_flash_definition_struct");

		/// <summary>
		/// vibration_frequency_definition_struct
		/// </summary>
		static TagGroup RFDS = new TagGroup("RFDS", "vibration_frequency_definition_struct");

		/// <summary>
		/// vibration_definition_struct
		/// </summary>
		static TagGroup RBDS = new TagGroup("RBDS", "vibration_definition_struct");

		/// <summary>
		/// damage_effect_sound_effect_definition
		/// </summary>
		static TagGroup dsfx = new TagGroup("dsfx", "damage_effect_sound_effect_definition");

		/// <summary>
		/// hud_widget_inputs_struct
		/// </summary>
		static TagGroup hwis = new TagGroup("hwis", "hud_widget_inputs_struct");

		/// <summary>
		/// hud_widget_state_definition_struct
		/// </summary>
		static TagGroup hwsd = new TagGroup("hwsd", "hud_widget_state_definition_struct");

		/// <summary>
		/// hud_widget_effect_function_struct
		/// </summary>
		static TagGroup hwef = new TagGroup("hwef", "hud_widget_effect_function_struct");

		/// <summary>
		/// new_hud_dashlight_data_struct
		/// </summary>
		static TagGroup nhd2 = new TagGroup("nhd2", "new_hud_dashlight_data_struct");

		/// <summary>
		/// screen_effect_bonus_struct
		/// </summary>
		static TagGroup sebs = new TagGroup("sebs", "screen_effect_bonus_struct");

		/// <summary>
		/// global_new_hud_globals_constants_struct
		/// </summary>
		static TagGroup nhgs_const = new TagGroup("nhgs", "global_new_hud_globals_constants_struct");

		/// <summary>
		/// global_new_hud_globals_struct
		/// </summary>
		static TagGroup nhgs = new TagGroup("nhgs", "global_new_hud_globals_struct");

		/// <summary>
		/// create_new_variant_struct
		/// </summary>
		static TagGroup cnvs = new TagGroup("cnvs", "create_new_variant_struct");

		/// <summary>
		/// grenade_and_powerup_struct
		/// </summary>
		static TagGroup gapu = new TagGroup("gapu", "grenade_and_powerup_struct");

		/// <summary>
		/// sound_playback_parameter_definition
		/// </summary>
		static TagGroup spl1 = new TagGroup("spl1", "sound_playback_parameter_definition");

		/// <summary>
		/// sound_effect_struct_definition
		/// </summary>
		static TagGroup ssfx = new TagGroup("ssfx", "sound_effect_struct_definition");

		/// <summary>
		/// platform_sound_playback_struct
		/// </summary>
		static TagGroup plsn = new TagGroup("plsn", "platform_sound_playback_struct");

		/// <summary>
		/// primary_light_struct
		/// </summary>
		static TagGroup prli = new TagGroup("prli", "primary_light_struct");

		/// <summary>
		/// secondary_light_struct
		/// </summary>
		static TagGroup scli = new TagGroup("scli", "secondary_light_struct");

		/// <summary>
		/// ambient_light_struct
		/// </summary>
		static TagGroup amli = new TagGroup("amli", "ambient_light_struct");

		/// <summary>
		/// lightmap_shadows_struct
		/// </summary>
		static TagGroup lmsh = new TagGroup("lmsh", "lightmap_shadows_struct");

		/// <summary>
		/// animation_aiming_screen_struct
		/// </summary>
		static TagGroup aaim = new TagGroup("aaim", "animation_aiming_screen_struct");

		/// <summary>
		/// packed_data_sizes_struct
		/// </summary>
		static TagGroup apds = new TagGroup("apds", "packed_data_sizes_struct");

		/// <summary>
		/// quantized_orientation_struct
		/// </summary>
		static TagGroup qoSS = new TagGroup("qoSS", "quantized_orientation_struct");

		/// <summary>
		/// animation_graph_resources_struct
		/// </summary>
		static TagGroup MAgr = new TagGroup("MAgr", "animation_graph_resources_struct");

		/// <summary>
		/// animation_index_struct
		/// </summary>
		static TagGroup ANII = new TagGroup("ANII", "animation_index_struct");

		/// <summary>
		/// animation_transition_state_struct
		/// </summary>
		static TagGroup ATSS_trans = new TagGroup("ATSS", "animation_transition_state_struct");

		/// <summary>
		/// animation_destination_state_struct
		/// </summary>
		static TagGroup ATSS_dest = new TagGroup("ATSS", "animation_destination_state_struct");

		/// <summary>
		/// animation_graph_contents_struct
		/// </summary>
		static TagGroup MAgc = new TagGroup("MAgc", "animation_graph_contents_struct");

		/// <summary>
		/// model_animation_runtime_data_struct
		/// </summary>
		static TagGroup MArt = new TagGroup("MArt", "model_animation_runtime_data_struct");

		/// <summary>
		/// cloth_properties
		/// </summary>
		static TagGroup clpr = new TagGroup("clpr", "cloth_properties");

		/// <summary>
		/// global_wind_model_struct
		/// </summary>
		static TagGroup WNDM = new TagGroup("WNDM", "global_wind_model_struct");

		/// <summary>
		/// sound_global_mix_struct
		/// </summary>
		static TagGroup sngl = new TagGroup("sngl", "sound_global_mix_struct");

		/// <summary>
		/// sound_playback_parameters_struct
		/// </summary>
		static TagGroup snpl = new TagGroup("snpl", "sound_playback_parameters_struct");

		/// <summary>
		/// sound_scale_modifiers_struct
		/// </summary>
		static TagGroup snsc = new TagGroup("snsc", "sound_scale_modifiers_struct");

		/// <summary>
		/// simple_platform_sound_playback_struct
		/// </summary>
		static TagGroup plsn_simple = new TagGroup("plsn", "simple_platform_sound_playback_struct");

		/// <summary>
		/// sound_promotion_parameters_struct
		/// </summary>
		static TagGroup snpr = new TagGroup("snpr", "sound_promotion_parameters_struct");
	};
};